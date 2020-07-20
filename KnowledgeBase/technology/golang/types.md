###### bool

```go
var b bool = true
```



###### numeric

```
uint8, uint16, uint32, uint64, int8, int8, int16, int32, int64.

float32, float64, complex64, complex128.

byte like uint8
rune like int32
int, uint
uintptr
```



##### string

```go
var str string = "lallalal"
//go语言中string是不可变的。
//go语言中的string采用的是utf8编码. 字符串是一窜固定长度的字符链接的字符序列。
//字符串是一个定长的字节数组实现+utf8编码
//所以除了纯ascii字符的情况下对字符串进行[i]字符索引是错误的(utf8一个字符长度是1-4byte)

``反引号实现原字符串



```



##### Derived type

```go
Pointer
Arrary
struct
Channel
func
Slice
interface
Map
```



###### Channel

```go
var identifier chan type
通道类型：通道内的数据类型。
通道变量：保存通道的变量。
Channel类型的零值是nil.

通道时引用类型需要使用make来进行创建.
通道实例 := make(chan 数据类型)//加载资源.

for instance:
ch1 := make(chan int)                 // 创建一个整型类型的通道
ch2 := make(chan interface{})         // 创建一个空接口类型的通道, 可以存放任意格式

type Equip struct{ /* 一些字段 */ }
ch2 := make(chan *Equip)             // 创建Equip指针类型的通道, 可以存放*Equip

使用<-可行使用通道收发数据.
通道存在的目的在于goroutine之间实现通信.
```





###### struc

```go
struct def.
---------------
type struct_type_name struct{
	member definition
	member definition
	member definition
	...
}

-----
type Book struct{
	title string
	author string
	subject string
	id	int
}

//声明struct变量.
variable_name := structure_variable_type {value1, value2...valuen}
或
variable_name := structure_variable_type { key1: value1, key2: value2..., keyn: valuen}
-------

	// 创建一个新的结构体
    fmt.Println(Books{"Go 语言", "www.runoob.com", "Go 语言教程", 6495407})

    // 也可以使用 key => value 格式
    fmt.Println(Books{title: "Go 语言", author: "www.runoob.com", subject: "Go 语言教程", 	id: 6495407})

```

```go
struct是可以定义匿名成员的
type Test struct{
	string//匿名string
	a int
}

func main(){
	t := Test{string:"test"}
	//t := Test{"test", 20}
    //t := Test{"test"}		//Error.
	fmt.Println(t.string)
	//哪如果有多个匿名成员呢？ ambiguous selector t.string
}
```



```
关于struct注意点：
1. struct是值类型(聚合)
2. struct的指针也使用点.访问成员
```



##### value-type And reference-type

```go
在go中bool，numeric，string都是属于值类型。
```



