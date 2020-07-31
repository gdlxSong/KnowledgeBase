





## 简介


>https://github.com/opentracing/specification/blob/master/specification.md

>https://blog.csdn.net/u013970991/article/details/77822060





### What

从概念上而言，tracing是分布式追踪工具，对分布式系统中的某request的流向进行追踪。如上面的文章所言。


**入侵式：**

也因为tracing是追踪具体的request的执行流，所以须有侵入式的方式植入代码。

因为入侵式需要深入客户代码，显然是不能依赖于其他公司的三方库，所以大家只有规范接口，而不同的公司的接口规范可能是不同的（大家想法不一样嘛），所以需要标准化，接口的具体内容格式化显然是不现实的（大家的需求不同），所以需要一个抽象层次比较高，且开源的标准。如是opentracing。

**自定义实现：**

在opentracing中只是规范了交互标准，并没有内容实现，所以我们需要去自定义实现Tracer，同时依赖开源的Tracer也是可以的。

OpenTracing的实现中，比较流行的为 Jaeger 和 Zipkin。








