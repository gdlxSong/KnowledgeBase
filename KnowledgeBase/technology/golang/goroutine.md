#### goroutine

```go
go语言最大的有点就是对goroutine的内置支持，实现高效的并发。	
```



##### how ？

```go
golang对于goroutine的支持只需要使用go关键字就可以简单实现。	
```

###### Syntax

```go
go 函数名( 参数列表 )
```

```
goroutine区别于coroutine之处在于，coroutine是sigle-kernel-thread内执行的，而goroutine是可以并行执行的，真正实现LWP。
```

