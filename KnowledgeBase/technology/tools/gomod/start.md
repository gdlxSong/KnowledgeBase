

## 由头

https://segmentfault.com/a/1190000018536993



问题记录：


[1] go mod在检查依赖的时候会自动检查版本依赖，如protobuf和grpc之间就存在版本依赖关系

    如果protobuf生成的go代码版本比当前的grpc版本高，那么mod会自动升级grpc版本


[2] 关于版本降级？

    以protobuf为例子，首先git下载protobuf，然后checkout v1.26.1, 使用go mod管理，保持module名字于高版本go.mod一致，会省下很多麻烦，其次就是编译生成。之后将protoc.exe也替换成2.6