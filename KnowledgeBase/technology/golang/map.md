##### golang map



##### what ?

```go
在c++中map是使用红黑树来实现的(unordered_map是散列表)

在golang中：
Map 是一种无序的键值对的集合。Map 最重要的一点是通过 key 来快速检索数据，key 类似于索引，指向数据的值。

Map 是一种集合，所以我们可以像迭代数组和切片那样迭代它。不过，Map 是无序的，我们无法决定它的返回顺序，这是因为 Map 是使用 hash 表来实现的。
```







##### how ?

```go
(1) declaration
var m map[key_type]value_type

(2) 使用make内建函数创建实例
m := make(map[key_type]value_type)

(3) delete内建函数删除pair<key, value>
delete(map, key)
```



##### why ?

```go
显然这里的map使用的是散列表来实现的。
```

