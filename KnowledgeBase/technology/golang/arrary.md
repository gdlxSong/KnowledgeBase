

##### declaration

```go
var identifier [SIZE][[SIZE]...] variable_type
var identifier = []type{}

--------
var arr1 [10][20]int;
var arr2 = [10]{10, 9, 5}
var arr3 = [20][10]{{10}, {20}}
var arr4 = [...]int{10,5,2}//注意和切片的差别。
```



```
从本质上来说，多维数组在物理上也是1维的。
```

