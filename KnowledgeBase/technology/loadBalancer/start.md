## 由头

balabala~~

## 资料来源

[1] https://godoc.org/google.golang.org/grpc/balancer

[2] https://github.com/grpc/grpc-go/blob/master/balancer/balancer.go#L52

[3] https://www.dazhuanlan.com/2019/12/31/5e0ad2b4b5387/

[4] https://godoc.org/google.golang.org/grpc/resolver

[5] https://www.jianshu.com/p/f49c3a29b5dc

## 过程问题记录

1. Resolver由Builder产生，resolver负责地址解析，，
     Build(target Target, cc ClientConn, opts BuildOptions) (Resolver, error)
     builder的目的很明确，为ClientConn提供一个resolver，目的是将target解析并推送给cc(UpdateState来进行推送)



## 总结和展望

balabala！~~