##### declaration

```go
(1)
var identifier1, identifier2 [,...] type
//指定变量类型，如果没有初始化，则变量默认为零值，零值就是变量没有做初始化时系统默认设置的值。
//bool的零值为false，整形为0...

(2)根据值自动推断.
var identifier3 = value
var s = "just a test."
//初始化声明.
(3)使用:=来忽略var，但是:=的左边没有声明新的变量就会产生错误.
identifier4 := value
str := "ignore var."
str, s := "", ""//有新的就行.

(4)多变量声明
//类型相同多个变量, 非全局变量
var vname1, vname2, vname3 type
vname1, vname2, vname3 = v1, v2, v3

var vname1, vname2, vname3 = v1, v2, v3 // 和 python 很像,不需要显示声明类型，自动推断

vname1, vname2, vname3 := v1, v2, v3 // 出现在 := 左侧的变量不应该是已经被声明过的，否则会导致编译错误

// 这种因式分解关键字的写法一般用于声明全局变量
var (
    vname1 v_type1
    vname2 v_type2
)
```



```
局部变量声明必须使用，全局变量可以声明而不使用。
```



##### const variable declaration：

```go
(1)
const identifier20 [type] = value
(2)
const identifier21, identifier22 = value1, value2
(3)
const(
	OK = 200
	NotFind = 404
)
//以枚举的形式定义.

```



##### itoa

```go
iota，特殊常量，可以认为是一个可以被编译器修改的常量。

iota 在 const关键字出现时将被重置为 0(const 内部的第一行之前)，const 中每新增一行常量声明将使 iota 计数一次(iota 可理解为 const 语句块中的行索引)。
package main

import "fmt"

func main() {
    const (
            a = iota   //0
            b          //1
            c          //2
            d = "ha"   //独立值，iota += 1
            e          //"ha"   iota += 1
            f = 100    //iota +=1
            g          //100  iota +=1
            h = iota   //7,恢复计数
            i          //8
    )
    fmt.Println(a,b,c,d,e,f,g,h,i)
}
```

