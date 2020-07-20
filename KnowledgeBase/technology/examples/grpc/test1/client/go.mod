module client

go 1.14

require (
	google.golang.org/grpc v1.27.0
	google.golang.org/protobuf v1.25.0 // indirect
	yqun.xyz/proto v0.0.0
)

replace yqun.xyz/proto => ../proto
