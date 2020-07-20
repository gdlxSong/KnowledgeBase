##### declaration

```
func function_name([parameter list]) [return types]
```



函数可能返回多个值：

```go
package main

import "fmt"

func swap(x, y string) (string, string){
	return y, x
}

func main(){
	a, b := swap("monkey", "pig")
	fmt.Println(a, b)
}
```

```
在很多的语言中都是支持函数返回多值的，c++我们也可以使用tuple简化嘛，然后比如python，比如matlab，但其实在这表面之下，返回值传递的是一个聚合类型，如python的多值是tuple实现的。
```



##### 关于闭包

```
各种编程语言，基本上都是使用匿名函数和局部函数来实现闭包的。如c++的lambda，python的剧本函数。这样的目的在于“携带私货”和延迟调用。
```

```
package main

import "fmt"

func getSequence() func() int {
   i:=0
   return func() int {
      i+=1
     return i  
   }
}

func main(){
   /* nextNumber 为一个函数，函数 i 为 0 */
   nextNumber := getSequence()  

   /* 调用 nextNumber 函数，i 变量自增 1 并返回 */
   fmt.Println(nextNumber())
   fmt.Println(nextNumber())
   fmt.Println(nextNumber())
   
   /* 创建新的函数 nextNumber1，并查看结果 */
   nextNumber1 := getSequence()  
   fmt.Println(nextNumber1())
   fmt.Println(nextNumber1())
}
//go语言使用匿名函数来实现闭包
```

