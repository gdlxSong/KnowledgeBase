package main

import (
	"context"
	"fmt"
	"log"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials"
	"yqun.xyz/proto"
)

func fatalIf(err error) {
	if nil != err {
		log.Fatal(err)
	}
}

// type PerRPCCredentials interface {
//     GetRequestMetadata(ctx context.Context, uri ...string) (map[string]string, error)
//     RequireTransportSecurity() bool
// }
//实现Token.

// Token token认证
type Token struct {
	AppID     string
	AppSecret string
}

// GetRequestMetadata 获取当前请求认证所需的元数据（metadata）
func (t *Token) GetRequestMetadata(ctx context.Context, uri ...string) (map[string]string, error) {
	return map[string]string{"app_id": t.AppID, "app_secret": t.AppSecret}, nil
}

// RequireTransportSecurity 是否需要基于 TLS 认证进行安全传输
func (t *Token) RequireTransportSecurity() bool {
	return true
}

var Address = ":9999"

func main() {

	creds, err := credentials.NewClientTLSFromFile("../server.pem", "xGdl")
	fatalIf(err)
	//构建Token
	token := Token{
		AppID:     "grpc_token",
		AppSecret: "123456",
	}

	// 连接服务器
	conn, err := grpc.Dial(Address,
		grpc.WithTransportCredentials(creds),
		grpc.WithPerRPCCredentials(&token))

	fatalIf(err)
	fmt.Println("dialing server...")
	client := proto.NewMessageClient(conn)
	resp, err := client.GetMessage(context.Background(), &proto.Request{
		Req: "haha",
	})
	fatalIf(err)
	fmt.Println(resp)
}
