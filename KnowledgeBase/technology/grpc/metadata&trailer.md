
## 简介

>https://godoc.org/google.golang.org/grpc/metadata

MD其实就算是一个map，不过定义了一些操作。metadata的作用是传递请求的元数据。

```golang
type MD map[string][]string

func FromIncomingContext(ctx context.Context) (md MD, ok bool)
func FromOutgoingContext(ctx context.Context) (MD, bool)
func FromOutgoingContextRaw(ctx context.Context) (MD, [][]string, bool)
func Join(mds ...MD) MD
func New(m map[string]string) MD
func Pairs(kv ...string) MD
func (md MD) Append(k string, vals ...string)
func (md MD) Copy() MD
func (md MD) Get(k string) []string
func (md MD) Len() int
func (md MD) Set(k string, vals ...string)
```


### 使用

metadata在哪里被使用？

1. 在CallOption






