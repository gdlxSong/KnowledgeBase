#### Golang Slice

##### what ?

```go
golang 的slice是对数组的抽象。
```

```go
Slice以阵列为基础，提供强大的功能和灵活性。

[]T就是Slice的形式，没有长度，T是Slice元素的类型。
```



##### how ?

how create?

```go
var s []type
s := []type{e, e2, e3...}
s := make([]type, len)
s := make([]type, len, cap)

-----------------------
var s []int
s := []int{1,2,3}
s := make([]int, 3)
s := make([]int, 3, 20)
```



```
slice 的零值为nil
```

```
关于Slice，golang提供三个内置操作函数：
```

```go
func make([]T, len, cap) []T
```

```go
func copy(dst, src []T) int		
```

```go
func append(s []T, x ...T) []T
```

```go
当然cap和len是通用的.
```



##### why ?

```go
type Slice struct{
	bufer *T
	len uint
	cap uint
}
```

```go
go 的Slice可以简单理解成如上结构。
```

```go
然后其实无论哪种语言中实现Slice，我们在使用的时候都应该注意，我们的Slice是对底层buffer只拥有使用权，没有拥有权。我们只是引用此资源。

func copy(dst, src []T) int	这个函数就实现了深拷贝。
```

