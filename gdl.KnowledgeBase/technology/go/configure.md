​	关于vs code上编写go语言的环境配置哇.

#### 下载vs code

​	<a href=" https://code.visualstudio.com/ ">官网链接</a>

​	vs code是很轻量级的，支持各种语言和其插件。界面很简单美。



#### 下载安装Go

​	<a href=" https://studygolang.com/articles/15790 ">这是go语言中文网的资源.</a>



关于go的环境配置：

```
PS C:\Users\Administrator> go env
set GO111MODULE=
set GOARCH=amd64
set GOBIN=
set GOCACHE=C:\Users\Administrator\AppData\Local\go-build
set GOENV=C:\Users\Administrator\AppData\Roaming\go\env
set GOEXE=.exe
set GOFLAGS=
set GOHOSTARCH=amd64
set GOHOSTOS=windows
set GONOPROXY=
set GONOSUMDB=
set GOOS=windows
set GOPATH=F:\Applications\Go\work
set GOPRIVATE=
set GOPROXY=https://proxy.golang.org,direct
set GOROOT=F:\Applications\Go
set GOSUMDB=sum.golang.org
set GOTMPDIR=
set GOTOOLDIR=F:\Applications\Go\pkg\tool\windows_amd64
set GCCGO=gccgo
set AR=ar
set CC=gcc
set CXX=g++
set CGO_ENABLED=1
set GOMOD=
set CGO_CFLAGS=-g -O2
set CGO_CPPFLAGS=
set CGO_CXXFLAGS=-g -O2
set CGO_FFLAGS=-g -O2
set CGO_LDFLAGS=-g -O2
set PKG_CONFIG=pkg-config
set GOGCCFLAGS=-m64 -mthreads -fno-caret-diagnostics -Qunused-arguments -fmessage-length=0 -fdebug-prefix-map=C:\Users\A
DMINI~1\AppData\Local\Temp\go-build145133754=/tmp/go-build -gno-record-gcc-switches
PS C:\Users\Administrator>
```



```
GOROOT：软件安装的根目录.
GOBIN: 运行所需的环境变量.
GOPATH:在go使用get工具的时候下载文件的路径(用户安装外库的路径)
```



然后是使用vs code编写go程式.

```
Error: Command failed: D:\Go\bin\go.exe get -u -v github.com/ramya-rao-a/go-outline
```

你可能会遇到这个问题。



解决方案：

```powershell
# 在cmd通过以下方式下载：
go get -u -v github.com/nsf/gocode
go get -u -v github.com/rogpeppe/godef
go get -u -v github.com/tpng/gopkgs
go get -u -v golang.org/x/tools/cmd/gorename   
go get -u -v golang.org/x/tools/cmd/guru  
go get -u -v github.com/ramya-rao-a/go-outline
go get -u -v github.com/newhook/go-symbols   
go get -u -v sourcegraph.com/sqs/goreturns
go get -u -v github.com/golang/lint/golint
# F5 debug 调试插件
go get -v -u github.com/peterh/liner github.com/derekparker/delve/cmd/dlv

# 下载的库会在GOPATH路径下。
# 然后将下载编译好的exe文件移动到GOBIN路径就可以了.
```





##### reference：

```js
https://code.visualstudio.com
https://studygolang.com/articles/15790
https://blog.csdn.net/youshijian99/article/details/81782875
```

