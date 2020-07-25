

## errors

### DENIED Redis is running in protected mode because protected mode is enabled, no bind addre

这个原因在于，配置文件中bind 127.0.0.1。

解决方案：

在配置文件中找到这一行，然后重启即可。

>https://blog.csdn.net/chenzz2560/article/details/82756484



### 编译错误： jemalloc/jemalloc.h: No such file or directory

原因在于没有jemalloc的库。


解决方案：
指定libc即可：

```bash
$ make MALLOC=libc
```




### Next....





