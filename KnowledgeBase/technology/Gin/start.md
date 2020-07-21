## 由头

项目中依赖gin，需要对gin进行深入学习。

## 资料来源


1. https://godoc.org/github.com/gin-gonic/gin




## 过程问题记录

1. gin.Context::Get()返回一个interface{}，这个对象是什么时候实现解析的，还是之前gin.Context.Set进去的？

2. gin的数据模型绑定，bindJson, BindXML...
    https://github.com/gin-gonic/gin#model-binding-and-validation

3. gin.Context阅读（https://godoc.org/github.com/gin-gonic/gin#Context）

4. 几个gin.Context::Abort*的区别和联系？

    1. 联系：都会调用Context.Abort.
    2. 区别：Abort不上头，只是终止此次调用过程，
             AbortWithError上头，将状态码写入header，将err推送到c.Errors, 内部回调AbortWithStatus
             AbortWithStatus上头，将状态码写入header
             AbortWithStatusJSON直接响应，按照status和json。

5. Bind处理的Content-Type？

    只能是：
    "application/json" --> JSON binding
    "application/xml"  --> XML binding

6. Context.Cookie，在key对应多个value的时候，只返回一个，那么我需要全部的value呢？

7. Context.Data的用法？

8. Context.JSON和JSONP的区别？

9. 去了解清楚：Param， Bind，Get，PostForm的数据来源：url, post-body, multi-part.

10. gin.Context.Next的用法和作用？

    https://godoc.org/github.com/gin-gonic/gin#Context.Next
    https://blog.csdn.net/heart66_A/article/details/100060865

    gin.RouterGroup.Handlers存储着其所有的中间件，中间件的执行默认是按照Handlers列表顺序执行的，但是Next可以暂停执行当前中间件，向后执行，会后递归返回执行Next暂停的中间件。

    ```golang
    //遍历该handles，并执行对应注册的handle
    func (c *Context) Next() {
        c.index++
        for c.index < int8(len(c.handlers)) {
            c.handlers[c.index](c)
            c.index++
        }
    }
    ```


## 总结和展望

balabala！~~