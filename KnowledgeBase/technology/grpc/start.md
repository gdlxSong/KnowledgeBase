## 由头

因为这段是见做项目是基于grpc，所以需要深入grpc进行学习。


## 资料来源

[1] https://golang.google.cn/pkg/net/rpc/

[2] https://godoc.org/google.golang.org/grpc

[3] https://github.com/gdlxSong/go-grpc-example

[4] https://www.dazhuanlan.com/2019/12/31/5e0ad2b4b5387/

[5] https://godoc.org/google.golang.org/grpc/resolver

## 问题记录和回答

1. grpc.ClientConn.DialContext是如何工作的，其target的规则是什么？（https://godoc.org/google.golang.org/grpc#DialContext）

    target：https://github.com/grpc/grpc/blob/master/doc/naming.md

2. grpc的域名系统解析的原理是什么？也就是grpc.WithBalancerName(balancer.RoundRobin)

    https://golang.google.cn/pkg/net/url/#URL , 这里面是url的格式


3. grpc的ClientConn， ServerConn，Stream是否协程安全？

    https://www.zhihu.com/question/50366000

4. grpc的一个ClientConn代表多少个连接？
    https://godoc.org/google.golang.org/grpc#ClientConn， 文档指出0~n个


5. undefined: grpc.SupportPackageIsVersion6?

    https://blog.csdn.net/yzf279533105/article/details/104416459

    原因是protobuf生成的go代码版本比当前grpc的版本高


## 测试例子

[1] 测试基于tls和token的验证。（/examples/grpc/test1）













