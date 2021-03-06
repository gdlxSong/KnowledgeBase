

## 简介


简单谈一谈，为什么需要stream机制？

目的在于减少tcp连接数量，降低server的压力，提高网络连接的效率。

其实stream的原理就是在tcp连接内打洞，在逻辑上创造多个连接，每一个逻辑连接都是一个stream，独立维护其连接状态。这一点和一台物理ip连接，在ip连接的基础上打洞，用port标识每一个tcp或udp连接如出一辙。



grpc基于http2的实现，h2天生就支持stream，会更加方便和标准，更具有普遍性。

但是，正是基于h2的实现，所以grpc每次发送和接受都会额外的增加以此h2的编码和解码。所以性能不算高。

### Stream

```golang
// Stream defines the common interface a client or server stream has to satisfy.
//
// Deprecated: See ClientStream and ServerStream documentation instead.
type Stream interface {
	// Deprecated: See ClientStream and ServerStream documentation instead.
	Context() context.Context
	// Deprecated: See ClientStream and ServerStream documentation instead.
	SendMsg(m interface{}) error
	// Deprecated: See ClientStream and ServerStream documentation instead.
	RecvMsg(m interface{}) error
}
```

### ClientStream

```golang
// ClientStream defines the client-side behavior of a streaming RPC.
//
// All errors returned from ClientStream methods are compatible with the
// status package.
type ClientStream interface {
	// Header returns the header metadata received from the server if there
	// is any. It blocks if the metadata is not ready to read.
	Header() (metadata.MD, error)
	// Trailer returns the trailer metadata from the server, if there is any.
	// It must only be called after stream.CloseAndRecv has returned, or
	// stream.Recv has returned a non-nil error (including io.EOF).
	Trailer() metadata.MD
	// CloseSend closes the send direction of the stream. It closes the stream
	// when non-nil error is met. It is also not safe to call CloseSend
	// concurrently with SendMsg.
	CloseSend() error
	// Context returns the context for this stream.
	//
	// It should not be called until after Header or RecvMsg has returned. Once
	// called, subsequent client-side retries are disabled.
	Context() context.Context
	// SendMsg is generally called by generated code. On error, SendMsg aborts
	// the stream. If the error was generated by the client, the status is
	// returned directly; otherwise, io.EOF is returned and the status of
	// the stream may be discovered using RecvMsg.
	//
	// SendMsg blocks until:
	//   - There is sufficient flow control to schedule m with the transport, or
	//   - The stream is done, or
	//   - The stream breaks.
	//
	// SendMsg does not wait until the message is received by the server. An
	// untimely stream closure may result in lost messages. To ensure delivery,
	// users should ensure the RPC completed successfully using RecvMsg.
	//
	// It is safe to have a goroutine calling SendMsg and another goroutine
	// calling RecvMsg on the same stream at the same time, but it is not safe
	// to call SendMsg on the same stream in different goroutines. `It is also
	// not safe to call CloseSend concurrently with SendMsg.`
	SendMsg(m interface{}) error
	// RecvMsg blocks until it receives a message into m or the stream is
	// done. It returns io.EOF when the stream completes successfully. On
	// any other error, the stream is aborted and the error contains the RPC
	// status.
	//
	// It is safe to have a goroutine calling SendMsg and another goroutine
	// calling RecvMsg on the same stream at the same time, but it is not
	// safe to call RecvMsg on the same stream in different goroutines.
	RecvMsg(m interface{}) error
}
```


### ServerStream

```go
// ServerStream defines the server-side behavior of a streaming RPC.
//
// All errors returned from ServerStream methods are compatible with the
// status package.
type ServerStream interface {
	// SetHeader sets the header metadata. It may be called multiple times.
	// When call multiple times, all the provided metadata will be merged.
	// All the metadata will be sent out when one of the following happens:
	//  - ServerStream.SendHeader() is called;
	//  - The first response is sent out;
	//  - An RPC status is sent out (error or success).
	SetHeader(metadata.MD) error
	// SendHeader sends the header metadata.
	// The provided md and headers set by SetHeader() will be sent.
	// It fails if called multiple times.
	SendHeader(metadata.MD) error
	// SetTrailer sets the trailer metadata which will be sent with the RPC status.
	// When called more than once, all the provided metadata will be merged.
	SetTrailer(metadata.MD)
	// Context returns the context for this stream.
	Context() context.Context
	// SendMsg sends a message. On error, SendMsg aborts the stream and the
	// error is returned directly.
	//
	// SendMsg blocks until:
	//   - There is sufficient flow control to schedule m with the transport, or
	//   - The stream is done, or
	//   - The stream breaks.
	//
	// SendMsg does not wait until the message is received by the client. An
	// untimely stream closure may result in lost messages.
	//
	// It is safe to have a goroutine calling SendMsg and another goroutine
	// calling RecvMsg on the same stream at the same time, but it is not safe
	// to call SendMsg on the same stream in different goroutines.
	SendMsg(m interface{}) error
	// RecvMsg blocks until it receives a message into m or the stream is
	// done. It returns io.EOF when the client has performed a CloseSend. On
	// any non-EOF error, the stream is aborted and the error contains the
	// RPC status.
	//
	// It is safe to have a goroutine calling SendMsg and another goroutine
	// calling RecvMsg on the same stream at the same time, but it is not
	// safe to call RecvMsg on the same stream in different goroutines.
	RecvMsg(m interface{}) error
}
```



### 注意

1. stream在close之后是可以恢复的，也就是close并没有清楚其连接信息。
2. 请注意SendMsg的goroutine的安全性。
3. 请注意RecvMsg的goroutine的安全性，其次就是RecvMsg是阻塞的，因为属于用户的主动行为。




