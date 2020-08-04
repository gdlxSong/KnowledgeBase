## 简介


>https://opentracing.io/specification/

>https://wu-sheng.gitbooks.io/opentracing-io/content/spec


>The Tracer interface creates Spans and understands how to Inject (serialize) and Extract (deserialize) them across process boundaries. Formally, it has the following capabilities.


其实Inject和Extract就是序列化和反序列化的过程，序列化之后的SpanContext可以在rpc的metadata中传递，以期望达到SpanContext可以跨越进程的目的。



Inject和Extract操作的是SpanContext，其实tracing的跨进程传播是基于每一个分布式节点都是有自己的Tracer对象，并且最终在jeager的collector聚集。

另外需要注意的是，如果tracing要跨进程传播，如果携带过多的baggage，会增大网络压力和性能开销。

