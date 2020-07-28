
## 简介

首先需要pb的支持，生成对应语言的pb代码。但是希望client的pb代码来自于server。


## ClientConn

```golang
func Dial(target string, opts ...DialOption) (*ClientConn, error)
func DialContext(ctx context.Context, target string, opts ...DialOption) (conn *ClientConn, err error)
func (cc *ClientConn) Close() error
func (cc *ClientConn) GetMethodConfig(method string) MethodConfig
func (cc *ClientConn) GetState() connectivity.State
func (cc *ClientConn) Invoke(ctx context.Context, method string, args, reply interface{}, opts ...CallOption) error
func (cc *ClientConn) NewStream(ctx context.Context, desc *StreamDesc, method string, opts ...CallOption) (ClientStream, error)
func (cc *ClientConn) ResetConnectBackoff()
func (cc *ClientConn) Target() string
func (cc *ClientConn) WaitForStateChange(ctx context.Context, sourceState connectivity.State) bool
```

### Dial

连接grpc-server，返回一个ClientConn。

注意：当不需要传输安全保障的时候需要提供WithInsecure的DialOption。

```golang
conn, err := grpc.Dial("host:post", grpc.WithInsecure())
```


### DialContext

1. DialContext默认是非阻塞执行的，可用WithBlock阻塞connect服务端。
2. 在非阻塞DialContext，context.Context的超时对连接不起作用，只对函数本身执行时间起作用。
3. target可以是直接提供的服务端地址，也可以是一个需要解析的url，ClientConn调用name-resolver解析。

`ps: ClientConn是一个客户端抽象，但并不是代表一条连接。`


### GetMethodConfig

获取Method的配置或默认配置。


## CallOption

```golang
func CallContentSubtype(contentSubtype string) CallOption
func CallCustomCodec(codec Codec) CallOption
func FailFast(failFast bool) CallOption
func ForceCodec(codec encoding.Codec) CallOption
func Header(md *metadata.MD) CallOption
func MaxCallRecvMsgSize(bytes int) CallOption
func MaxCallSendMsgSize(bytes int) CallOption
func MaxRetryRPCBufferSize(bytes int) CallOption
func Peer(p *peer.Peer) CallOption
func PerRPCCredentials(creds credentials.PerRPCCredentials) CallOption
func Trailer(md *metadata.MD) CallOption
func UseCompressor(name string) CallOption
func WaitForReady(waitForReady bool) CallOption
```











