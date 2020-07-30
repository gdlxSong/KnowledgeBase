

## 简介


> https://www.jb51.net/article/136264.htm



1. CMD和ENTRYPOINT都可以使用shell和exec两种方式启动，CMD还可以为ENTRYPOINT添加默认参数。
2. 当存在ENTRYPOINT的时候，CMD只能为ENTRYPOINT指定默认参数
3. 可以通过docker run ...命令行覆盖CMD指令，不能传参
4. 可以通过docker run ...命令行给ENTRYPOINT传参。