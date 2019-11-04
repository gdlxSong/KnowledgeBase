

##### Range

```go
Go 语言中 range 关键字用于 for 循环中迭代数组(array)、切片(slice)、通道(channel)或集合(map)的元素。在数组和切片中它返回元素的索引和索引对应的值，在集合中返回 key-value 对。
```



```
其实我们往往在实现range的时候都是将操作对象抽象成一个可序列化的对象(在c++中就是可以begin和end迭代咯)
```





```
range都是返回两个值得(其实就是附加状态值)
对于data序列(数组，Slice) return enumerate(index, value)
对于map：return enumerate(key, value)
对于Channel：return enumerate(data, state) //state == false， channel was closed.
```

