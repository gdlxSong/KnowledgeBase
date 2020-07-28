

## 简介


### Server

```golang
type Server
func NewServer(opt ...ServerOption) *Server
func (s *Server) GetServiceInfo() map[string]ServiceInfo
func (s *Server) GracefulStop()
func (s *Server) RegisterService(sd *ServiceDesc, ss interface{})
func (s *Server) Serve(lis net.Listener) error
func (s *Server) ServeHTTP(w http.ResponseWriter, r *http.Request)
func (s *Server) Stop()
```


### ServerOption

```golang
func ChainStreamInterceptor(interceptors ...StreamServerInterceptor) ServerOption
func ChainUnaryInterceptor(interceptors ...UnaryServerInterceptor) ServerOption
func ConnectionTimeout(d time.Duration) ServerOption
func Creds(c credentials.TransportCredentials) ServerOption
func CustomCodec(codec Codec) ServerOption
func HeaderTableSize(s uint32) ServerOption
func InTapHandle(h tap.ServerInHandle) ServerOption
func InitialConnWindowSize(s int32) ServerOption
func InitialWindowSize(s int32) ServerOption
func KeepaliveEnforcementPolicy(kep keepalive.EnforcementPolicy) ServerOption
func KeepaliveParams(kp keepalive.ServerParameters) ServerOption
func MaxConcurrentStreams(n uint32) ServerOption
func MaxHeaderListSize(s uint32) ServerOption
func MaxMsgSize(m int) ServerOption
func MaxRecvMsgSize(m int) ServerOption
func MaxSendMsgSize(m int) ServerOption
func NumStreamWorkers(numServerWorkers uint32) ServerOption
func RPCCompressor(cp Compressor) ServerOption
func RPCDecompressor(dc Decompressor) ServerOption
func ReadBufferSize(s int) ServerOption
func StatsHandler(h stats.Handler) ServerOption
func StreamInterceptor(i StreamServerInterceptor) ServerOption
func UnaryInterceptor(i UnaryServerInterceptor) ServerOption
func UnknownServiceHandler(streamHandler StreamHandler) ServerOption
func WriteBufferSize(s int) ServerOption
```