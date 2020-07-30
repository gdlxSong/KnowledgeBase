

## 简介


### Server

```golang
type Server
func NewServer(opt ...ServerOption) *Server
func (s *Server) GetServiceInfo() map[string]ServiceInfo
func (s *Server) GracefulStop()
func (s *Server) RegisterService(sd *ServiceDesc, ss interface{})
func (s *Server) Serve(lis net.Listener) error
# http用于http的handler
func (s *Server) ServeHTTP(w http.ResponseWriter, r *http.Request)
func (s *Server) Stop()
```


### ServerOption

```golang
# 返回一个stream的链式拦截器
func ChainStreamInterceptor(interceptors ...StreamServerInterceptor) ServerOption
# 返回一个一元的链式拦截器
func ChainUnaryInterceptor(interceptors ...UnaryServerInterceptor) ServerOption
# 指定客户端连接的超时时间，默认120s，如果传入0或负值则立即超时
func ConnectionTimeout(d time.Duration) ServerOption
# 指定服务端证书和私钥
func Creds(c credentials.TransportCredentials) ServerOption
// 指定用户自定义消息解码器
func CustomCodec(codec Codec) ServerOption
// 指定h2的动态表的大小
func HeaderTableSize(s uint32) ServerOption
// ？？？
func InTapHandle(h tap.ServerInHandle) ServerOption
// 指定connect的window-size，小于64k将被忽略，针对一个连接
func InitialConnWindowSize(s int32) ServerOption
// 指定stram的window-size，小于64k将被忽略，针对一个stream
func InitialWindowSize(s int32) ServerOption
// 指定长连接增强策略？？？？？？
func KeepaliveEnforcementPolicy(kep keepalive.EnforcementPolicy) ServerOption
// 指定Client的配置参数，如IdleNum等
func KeepaliveParams(kp keepalive.ServerParameters) ServerOption
// 指定最大并发流的数量
func MaxConcurrentStreams(n uint32) ServerOption
//
func MaxHeaderListSize(s uint32) ServerOption
// 指定recv消息的最大大小
func MaxMsgSize(m int) ServerOption
func MaxRecvMsgSize(m int) ServerOption
// 指定send消息的最大大小
func MaxSendMsgSize(m int) ServerOption
// 指定一个stream的处理incoming的goroutine数量，指定0表示默认1.
func NumStreamWorkers(numServerWorkers uint32) ServerOption
// 指定压缩器，Deprecated: use encoding.RegisterCompressor instead. Will be supported throughout 1.x.
func RPCCompressor(cp Compressor) ServerOption
// Deprecated: use encoding.RegisterCompressor instead. Will be supported throughout 1.x.
func RPCDecompressor(dc Decompressor) ServerOption
// 指定readbuf的大小，默认32k，参数为0时禁止readbuf。
func ReadBufferSize(s int) ServerOption
// StatsHandler returns a ServerOption that sets the stats handler for the server.？？？？？？
func StatsHandler(h stats.Handler) ServerOption
// stream的拦截器
func StreamInterceptor(i StreamServerInterceptor) ServerOption
// 一元的拦截器
func UnaryInterceptor(i UnaryServerInterceptor) ServerOption
// 用于处理未注册的service请求
func UnknownServiceHandler(streamHandler StreamHandler) ServerOption
// 指定写缓冲区大小，其实SendMsg也不一定会立即发送出去，数据可能在writeBuf等待fd的可读事件。
func WriteBufferSize(s int) ServerOption
```


所有的ServerOption用于配置服务端的transport.ServerConfig。















