

## 简介


关于日志的设计，一般考虑几个方面：io性能（依赖的io库），并发性能，并发安全，记录时序性，接口的可用性。


### io性能

io性能和实现所以来的io库相关，如libc和fastio。


### 并发性能

并发情况下，我们需要良好的并发性能，才能在日志聚集采集的同时不影响系统的整体性能。

并发的性能其实就是你使用的Lock和Unlock操作的代价。

1. 使用CAS（compare and swap），来实现乐观锁，不过需要atomic和barrier支持。
2. 使用mutex，使用mutex可以保证并发安全，但是代价相对比较大，所以我们需要控制锁的粒度来优化性能。

>因为日志是write操作，所以就不讨论其他的互斥机制。

### 并发安全

安全和性能往往是一个权衡，使用mutex显然会更加安全。但是如果pv是一对一（生产者消费者已知且有限的情况下），那么可以使用其他无锁方式来实现安全机制。


### 记录的时序性


如果每一个线程没有维持log的缓冲区，当然也就是写入时序，是有序的。


但是如果log存在缓冲区：
    1. 共享log的缓冲区，使用互斥机制占有，可以保证时序。
    2. 每一个线程拥有本地log缓冲区，那么不能保证写入log是的时序。

### 接口可用性

接口设计合理，可用性高，也是评判lib的重要参数。

1. format，log的格式化输出
2. chain，链式调用


c++：可参考handy的log实现（虽然很多的c++log都是这样干的）
go：可以参考logrus




