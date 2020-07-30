

## 简介


首先理解name-resolver:

就是名字解析器，如dns，但不局限于dns。

在分布式系统中，name-resolver为当前负载均衡器提供服务发现的服务地址列表。



>https://godoc.org/google.golang.org/grpc/resolver


其实resolver的流程相对简单：

首先就是通过Builder创建Resolver，Resolver启动（在Builder.Build）,然后通过State（包含Address）向ClientConn推送最新的地址列表（ClientConn.UpdateState）。


### Address

```go
type Address struct {
    // Addr is the server address on which a connection will be established.
    Addr string

    // ServerName is the name of this address.
    // If non-empty, the ServerName is used as the transport certification authority for
    // the address, instead of the hostname from the Dial target string. In most cases,
    // this should not be set.
    //
    // If Type is GRPCLB, ServerName should be the name of the remote load
    // balancer, not the name of the backend.
    //
    // WARNING: ServerName must only be populated with trusted values. It
    // is insecure to populate it with data from untrusted inputs since untrusted
    // values could be used to bypass the authority checks performed by TLS.
    ServerName string

    // Attributes contains arbitrary data about this address intended for
    // consumption by the load balancing policy.
    Attributes *attributes.Attributes

    // Type is the type of this address.
    //
    // Deprecated: use Attributes instead.
    Type AddressType

    // Metadata is the information associated with Addr, which may be used
    // to make load balancing decision.
    //
    // Deprecated: use Attributes instead.
    Metadata interface{}
}

const (
    // Backend indicates the address is for a backend server.
    //
    // Deprecated: use Attributes in Address instead.
    Backend AddressType = iota
    // GRPCLB indicates the address is for a grpclb load balancer.
    //
    // Deprecated: to select the GRPCLB load balancing policy, use a service
    // config with a corresponding loadBalancingConfig.  To supply balancer
    // addresses to the GRPCLB load balancing policy, set State.Attributes
    // using balancer/grpclb/state.Set.
    GRPCLB
)
```

1. 需要注意的是Address可以是后端服务器的地址，也可以是负载均衡器的地址，其类型取决于Type字段，此外Type字段现在应当置入Attributes。
2. Attributes可以看作一个map[interface{}]interface{}, 我们将Address的地址属性存放其中，如Type(backend，load-balance)，weight（权重）等。
3. 基本上resolver的实现都是需要依赖服务发现来提供地址列表[]Address































