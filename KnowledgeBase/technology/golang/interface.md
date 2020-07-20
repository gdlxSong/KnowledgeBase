##### Interface

```
在众多的语言中，接口是一种行为的抽象。即抽象共同行为。
```



```go
/* 定义接口 */
type interface_name interface {
   method_name1 [return_type]
   method_name2 [return_type]
   method_name3 [return_type]
   ...
   method_namen [return_type]
}

/* 定义结构体 */
type struct_name struct {
   /* variables */
}

/* 实现接口方法 */
func (struct_name_variable struct_name) method_name1() [return_type] {
   /* 方法实现 */
}
...
func (struct_name_variable struct_name) method_namen() [return_type] {
   /* 方法实现*/
}
```

```
是不是有点和面向对象背道而驰的意思。其实原理都是一样的，只是this指针显式化了而已。
```

