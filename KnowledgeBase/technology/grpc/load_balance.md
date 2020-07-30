

## 简介

>https://github.com/grpc/grpc/blob/master/doc/load-balancing.md

>https://www.cnblogs.com/qq332398135/p/8525425.html

在grpc的client端，我们可以配置负载均衡器，来实现对负载均衡的需求，以期达到降低服务端站点的压力，和流量均衡，从而充分利用服务资源。

### 负载均衡的流程

**创建具有指定负载均衡的客户端：**

1. 需要注册解析器（resolver）
2. 需要注册负载均衡器（load balancer）
3. 在创建grpc.ClientConn的时候指定对用schema的target。
4. 在创建grpc.ClientConn的时候指定负载均衡策略（grpc.WithBalancerName）。

**负载均衡执行流程：**

1. Client会调用resolver/或者resolver推送更新后端服务地址列表。
2. 解析service config配置的负载均衡策略（如round_robin，grpclb等）
3. 客户端初始化load balance策略.如果resolver返回的地址只要有一个是load balance的地址,客户端就会使用grpclb策略.否则的话根据service config的配置决定.如果service config没有指定load balance粗略,则默认选用pick-first
4. 创建到每一个server address的subchannel
    1. grpclb策略下,client连接load balancer,并向其请求服务器地址
    2. grpc server会向load balancer汇报其负载情况
    3. load balancer向client返回一个server list,client向每一个server都建立一个subchannel
5. 对每一个rpc请求,load balancer决定哪一个subchannel应该被使用. 使用picker来选择。




**负载均衡器的执行流程（参考grpc/balancer源码）：**

1. resolver独立运行，并向ClientConn推送最新的resolver.State, 其实就是服务地址列表。
2. ClientConn将State推送给balancer，balancer更新自己的后端服务信息列表（create或delete SubConn（一个后端服务））
3. balancer结合PickerBuildInfo（自身维护的SubConn列表信息）和PickerBuilder来创建一个Picker（携带SubConn信息列表），然后Picker.Pick()可以实现选择一个SubConn。
4. ClientConn使用Picker提供的SubConn来发送此次调用。



在grpc中实现了基本的balancer，一般情况下，我们只需要自己实现Picker和PickerBuilder，Resolver和resolver.Builder就ok。

>resolver用于服务发现，balancer用于负载均衡。













