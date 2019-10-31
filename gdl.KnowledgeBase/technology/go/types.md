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
//go语言中的string采用的是utf8编码. 字符串是一窜固定长度的字符链接的字符序列。
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



##### value-type And reference-type

```go
在go中bool，numeric，string都是属于值类型。
```

