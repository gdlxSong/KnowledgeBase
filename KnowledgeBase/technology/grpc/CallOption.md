


## 简介

>CallOption configures a Call before it starts or extracts information from a Call after it completes.

```golang
type CallOption interface {
	// before is called before the call is sent to any server.  If before
	// returns a non-nil error, the RPC fails with that error.
	before(*callInfo) error

	// after is called after the call has completed.  after cannot return an
	// error, so any failures should be reported via output parameters.
	after(*callInfo, *csAttempt)
}
```

所以所有的CallOption都是被grpc预定义的了的。在grpc/rpc_util.go。

```go
// 指定Content-Type的子类型,其实就是指定CodeC（编码解码器）,需要配合ForceCodec
func CallContentSubtype(contentSubtype string) CallOption
// CallCustomCodec behaves like ForceCodec, but accepts a grpc.Codec instead of an encoding.Codec.
func CallCustomCodec(codec Codec) CallOption
// FailFast is the opposite of WaitForReady. Deprecated: use WaitForReady.
func FailFast(failFast bool) CallOption
// 指定消息的编码解码器
func ForceCodec(codec encoding.Codec) CallOption
// Header returns a CallOptions that retrieves the header metadata for a unary RPC.
func Header(md *metadata.MD) CallOption
// sets the maximum message size in bytes the client can receive.
func MaxCallRecvMsgSize(bytes int) CallOption
// sets the maximum message size in bytes the client can send.
func MaxCallSendMsgSize(bytes int) CallOption
// ？？？
func MaxRetryRPCBufferSize(bytes int) CallOption
// 接受对端信息
func Peer(p *peer.Peer) CallOption
// 用于指定call的公钥（服务端证书提取）
func PerRPCCredentials(creds credentials.PerRPCCredentials) CallOption
// 获取服务端设置的trailer
func Trailer(md *metadata.MD) CallOption
//sets the compressor used when sending the request. If WithCompressor is also set, UseCompressor has higher priority.
func UseCompressor(name string) CallOption
// 连接行为配置
func WaitForReady(waitForReady bool) CallOption
```

就像是一个面向方面编程的一个切面。


可以从源码去看，CallOption：

无论是从自定义的rpc接口还是直接Invoke进入，首先都会创建ClientStream，然后再stream传输前执行CallOption的before，在stream返回，finish函数（grpc/clientconn.go）会执行CallOption的after。



### CallOption如何被执行？

在grpc/clientconn.go有grpc.ClientConn的定义，其中有dopts值得注意，和CallOption相关。


```go

// ClientConn represents a virtual connection to a conceptual endpoint, to
// perform RPCs.
//
// A ClientConn is free to have zero or more actual connections to the endpoint
// based on configuration, load, etc. It is also free to determine which actual
// endpoints to use and may change it every RPC, permitting client-side load
// balancing.
//
// A ClientConn encapsulates a range of functionality including name
// resolution, TCP connection establishment (with retries and backoff) and TLS
// handshakes. It also handles errors on established connections by
// re-resolving the name and reconnecting.
type ClientConn struct {
	ctx    context.Context
	cancel context.CancelFunc

	target       string
	parsedTarget resolver.Target
	authority    string
	dopts        dialOptions
	csMgr        *connectivityStateManager

	balancerBuildOpts balancer.BuildOptions
	blockingpicker    *pickerWrapper

	mu              sync.RWMutex
	resolverWrapper *ccResolverWrapper
	sc              *ServiceConfig
	conns           map[*addrConn]struct{}
	// Keepalive parameter can be updated if a GoAway is received.
	mkp             keepalive.ClientParameters
	curBalancerName string
	balancerWrapper *ccBalancerWrapper
	retryThrottler  atomic.Value

	firstResolveEvent *grpcsync.Event

	channelzID int64 // channelz unique identification number
	czData     *channelzData

	lceMu               sync.Mutex // protects lastConnectionError
	lastConnectionError error
}
```


在grpc/dialoptions.go中有关于dialOptions的定义，其中有字段callOptions：

```go
// dialOptions configure a Dial call. dialOptions are set by the DialOption
// values passed to Dial.
type dialOptions struct {
	unaryInt  UnaryClientInterceptor
	streamInt StreamClientInterceptor

	chainUnaryInts  []UnaryClientInterceptor
	chainStreamInts []StreamClientInterceptor

	cp          Compressor
	dc          Decompressor
	bs          internalbackoff.Strategy
	block       bool
	insecure    bool
	timeout     time.Duration
	scChan      <-chan ServiceConfig
	authority   string
	copts       transport.ConnectOptions
	callOptions []CallOption
	// This is used by v1 balancer dial option WithBalancer to support v1
	// balancer, and also by WithBalancerName dial option.
	balancerBuilder             balancer.Builder
	channelzParentID            int64
	disableServiceConfig        bool
	disableRetry                bool
	disableHealthCheck          bool
	healthCheckFunc             internal.HealthChecker
	minConnectTimeout           func() time.Duration
	defaultServiceConfig        *ServiceConfig // defaultServiceConfig is parsed from defaultServiceConfigRawJSON.
	defaultServiceConfigRawJSON *string
	// This is used by ccResolverWrapper to backoff between successive calls to
	// resolver.ResolveNow(). The user will have no need to configure this, but
	// we need to be able to configure this in tests.
	resolveNowBackoff func(int) time.Duration
	resolvers         []resolver.Builder
}
```


然后在grpc/call.go中有Invoke函数，也就是我们使用CallOption直接相关的。

```go
func (cc *ClientConn) Invoke(ctx context.Context, method string, args, reply interface{}, opts ...CallOption) error {
	// allow interceptor to see all applicable call options, which means those
	// configured as defaults from dial option as well as per-call options
	opts = combine(cc.dopts.callOptions, opts)

	if cc.dopts.unaryInt != nil {
		return cc.dopts.unaryInt(ctx, method, args, reply, cc, invoke, opts...)
	}
	return invoke(ctx, method, args, reply, cc, opts...)
}
func combine(o1 []CallOption, o2 []CallOption) []CallOption {
	// we don't use append because o1 could have extra capacity whose
	// elements would be overwritten, which could cause inadvertent
	// sharing (and race conditions) between concurrent calls
	if len(o1) == 0 {
		return o2
	} else if len(o2) == 0 {
		return o1
	}
	ret := make([]CallOption, len(o1)+len(o2))
	copy(ret, o1)
	copy(ret[len(o1):], o2)
	return ret
}
```


combin会将dialoption和calloption合并，所以这里也可以看出dialoption作用于ClientConn，而calloption作用于一个请求，或者说一个Invoke。









### DialOption

注意DialOption和CallOption都会导向dialOptions, 不过关联的是dialoptions的copts字段。


```go
// ConnectOptions covers all relevant options for communicating with the server.
type ConnectOptions struct {
	// UserAgent is the application user agent.
	UserAgent string
	// Dialer specifies how to dial a network address.
	Dialer func(context.Context, string) (net.Conn, error)
	// FailOnNonTempDialError specifies if gRPC fails on non-temporary dial errors.
	FailOnNonTempDialError bool
	// PerRPCCredentials stores the PerRPCCredentials required to issue RPCs.
	PerRPCCredentials []credentials.PerRPCCredentials
	// TransportCredentials stores the Authenticator required to setup a client
	// connection. Only one of TransportCredentials and CredsBundle is non-nil.
	TransportCredentials credentials.TransportCredentials
	// CredsBundle is the credentials bundle to be used. Only one of
	// TransportCredentials and CredsBundle is non-nil.
	CredsBundle credentials.Bundle
	// KeepaliveParams stores the keepalive parameters.
	KeepaliveParams keepalive.ClientParameters
	// StatsHandler stores the handler for stats.
	StatsHandler stats.Handler
	// InitialWindowSize sets the initial window size for a stream.
	InitialWindowSize int32
	// InitialConnWindowSize sets the initial window size for a connection.
	InitialConnWindowSize int32
	// WriteBufferSize sets the size of write buffer which in turn determines how much data can be batched before it's written on the wire.
	WriteBufferSize int
	// ReadBufferSize sets the size of read buffer, which in turn determines how much data can be read at most for one read syscall.
	ReadBufferSize int
	// ChannelzParentID sets the addrConn id which initiate the creation of this client transport.
	ChannelzParentID int64
	// MaxHeaderListSize sets the max (uncompressed) size of header list that is prepared to be received.
	MaxHeaderListSize *uint32
}
```



```go
// DialOption configures how we set up the connection.
type DialOption interface {
	apply(*dialOptions)
}

// 
func FailOnNonTempDialError(f bool) DialOption
# 用于验证，仅和WithInsecure()一起用，如果存在TransportCredentials，则该值无效。
func WithAuthority(a string) DialOption
# 退避算法
func WithBackoffConfig(b BackoffConfig) DialOption
# 退避的最大时间
func WithBackoffMaxDelay(md time.Duration) DialOption
# 指定的load balancer
func WithBalancerName(balancerName string) DialOption
# 使用block来connect，此时Dial\*的ctx超时对握手协商过程起作用
func WithBlock() DialOption
# 指定stream的链式拦截器
func WithChainStreamInterceptor(interceptors ...StreamClientInterceptor) DialOption
# 指定一元的链式拦截器
func WithChainUnaryInterceptor(interceptors ...UnaryClientInterceptor) DialOption
// 
func WithChannelzParentID(id int64) DialOption
# 用于指定编码解码器
func WithCodec(c Codec) DialOption
# 用于指定压缩器
func WithCompressor(cp Compressor) DialOption
# 指定connect协商参数
func WithConnectParams(p ConnectParams) DialOption
# 指定connect连接代理
func WithContextDialer(f func(context.Context, string) (net.Conn, error)) DialOption
# 用于指定证书和token，一起指定。
func WithCredentialsBundle(b credentials.Bundle) DialOption
# 指定接受消息的解压器
func WithDecompressor(dc Decompressor) DialOption
# 指定ClientConn的默认CallOptions
func WithDefaultCallOptions(cos ...CallOption) DialOption
func WithDefaultServiceConfig(s string) DialOption
# 就是指定connect协商过程的代理f
func WithDialer(f func(string, time.Duration) (net.Conn, error)) DialOption
# 屏蔽health check，也就是屏蔽心跳包？
func WithDisableHealthCheck() DialOption
func WithDisableRetry() DialOption
func WithDisableServiceConfig() DialOption
# 指定tcp的滑动窗口的初始大小，不小于64k
func WithInitialConnWindowSize(s int32) DialOption
# 设置stream的tcp的滑动窗口的大小，小于64k将被忽略
func WithInitialWindowSize(s int32) DialOption
# 不使用tls，(ps: 在网关以内，都这样做。)
func WithInsecure() DialOption
// 指定keepalive
func WithKeepaliveParams(kp keepalive.ClientParameters) DialOption
// 指定header list的max
func WithMaxHeaderListSize(s uint32) DialOption
// WithMaxMsgSize returns a DialOption which sets the maximum message size the client can receive.
func WithMaxMsgSize(s int) DialOption
// 忽略连接代理
func WithNoProxy() DialOption
// 指定token
func WithPerRPCCredentials(creds credentials.PerRPCCredentials) DialOption
// 指定接受缓冲区的大小
func WithReadBufferSize(s int) DialOption
// 指定name-resolver，ps： 用于服务发现
func WithResolvers(rs ...resolver.Builder) DialOption
// 
func WithReturnConnectionError() DialOption
// 
func WithServiceConfig(c <-chan ServiceConfig) DialOption
// 为此client中的所有连接指定stats handler
func WithStatsHandler(h stats.Handler) DialOption
// 为流指定拦截器
func WithStreamInterceptor(f StreamClientInterceptor) DialOption
// 指定连接超时，在WithBlock被设置的情况下有效
func WithTimeout(d time.Duration) DialOption
// 指定服务端公钥和名称（Common Name）
func WithTransportCredentials(creds credentials.TransportCredentials) DialOption
// 为unary rpc指定拦截器
func WithUnaryInterceptor(f UnaryClientInterceptor) DialOption
// 设置header：user-agent
func WithUserAgent(s string) DialOption
// 指定写缓冲区大小
func WithWriteBufferSize(s int) DialOption
```

DialOption影响ClientConn的dialOptions字段的copts字段的值，直接影响connect的配置。








