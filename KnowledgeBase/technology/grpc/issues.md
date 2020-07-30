
## Issues


1. 今天早上(2020.7.30)我问我自己，在grpc的内部的roundrobin的picker，只有一个mutex来保证next的顺序性（但不保证唯一），那么是否能防止SubChannel上面的数据竞争，safety？

    A：无需保证SubChannel的安全性，可以参考grpc/balancer/base/balancer.go:UpdateClientConnState， 可能删除或创建SubConn，而SubConn代表一个后端服务，那么一个SubChannel就是一个c/s的h2连接，而我们使用连接的方式是stream，stream之间相互隔离，所以无需再picker哪里保证safety，而只需要均衡的分配流量即可。


