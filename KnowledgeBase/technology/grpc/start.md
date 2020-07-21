## 由头

因为这段是见做项目是基于grpc，所以需要深入grpc进行学习。


## 资料来源

[1] https://golang.google.cn/pkg/net/rpc/

[2] https://godoc.org/google.golang.org/grpc

[3] https://github.com/gdlxSong/go-grpc-example




## 问题记录和回答

1. grpc.ClientConn.DialContext是如何工作的，其target的规则是什么？（https://godoc.org/google.golang.org/grpc#DialContext）

    target：https://github.com/grpc/grpc/blob/master/doc/naming.md

2. grpc的域名系统解析的原理是什么？也就是grpc.WithBalancerName(balancer.RoundRobin)



## 测试例子

[1] 测试基于tls和token的验证。（/examples/grpc/test1）













