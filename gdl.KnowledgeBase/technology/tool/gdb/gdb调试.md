##### GDB

​	 http://ftp.gnu.org/gnu/gdb/ 

​	 http://www.gnu.org/software/gdb/ 

​	 GDB, the GNU Project debugger, allows you to see what is going on `inside' another program while it executes -- or what another program was doing at the moment it crashed. 



### GDB Command

###### compiler and start.

```c++
	g++ test.cpp -g

		//在编译的时候加入-g选项告诉gcc加入调试信息，否则在调试的时候会找不到符号表.

​	gdb -q a.out

		//在启动的时候加入-q选项可以屏蔽版本等无用信息.
```



###### 查看源码

```c++
	<list | l> 5,10

		//查看5-10行的源码.

​	<list | l> func

		//查看func函数的源码.

​	<list | l> test.cpp:5,10

		//查看指定文件行
```



###### 设置断点

```c++
	<b | break | breakpoint> 10

		//在当前文件行号为10的地方设置断点.

​	<b | break | breakpoint> func

		//在当前文件func函数入口处设置断点.

​	<b | break | breakpoint> test.cpp:10

		//在test.cpp源文件第10行设置断点.

​	<b | break | breakpoint> 10 if n == 10

		//在当前文件第十行设置一个条件断点.

​	info break

		//查看断点信息
```



###### 运行程序

```c++
	<r | run>
```



###### 执行下一跳语句

```c++
	<n | next>
```



###### 继续执行

```c++
	<c | continue>
		//让程序继续执行到下一个断点处或者到结束.	
```



###### 打印变量的值

```c++
	<p | print> var
		//打印变量的值.
```



###### 自动打印变量的值

```c++
	<display> var
		//自动打印变量的值.
```



###### 命令概览

```bash
gdb *** : 启动调试
gdb -tui ***: 显示源码界面和调试界面
【常用命令】：
（gdb）help：查看命令帮助，具体命令查询在gdb中输入help + 命令,简写h
（gdb）run：重新开始运行文件（run-text：加载文本文件，run-bin：加载二进制文件）,简写r
（gdb）start：单步执行，运行程序，停在第一执行语句
（gdb）list：查看原代码（list-n,从第n行开始查看代码。list+ 函数名：查看具体函数）,简写l
（gdb）set：设置变量的值
（gdb）next：单步调试（逐过程，函数直接执行）,简写n
（gdb）step：单步调试（逐语句：跳入自定义函数内部执行）,简写s
（gdb）backtrace：查看函数的调用的栈帧和层级关系,简写bt
（gdb）frame：切换函数的栈帧,简写f
（gdb）info：查看函数内部局部变量的数值,简写i
（gdb）finish：结束当前函数，返回到函数调用点
（gdb）continue：继续运行,简写c
（gdb）print：打印值及地址,简写p
（gdb）quit：退出gdb,简写q
（gdb）break+num：在第num行设置断点,简写b
（gdb）info breakpoints：查看当前设置的所有断点
（gdb）delete breakpoints num：删除第num个断点,简写d
（gdb）display：追踪查看具体变量值
（gdb）undisplay：取消追踪观察变量
（gdb）watch：被设置观察点的变量发生修改时，打印显示
（gdb）i watch：显示观察点
（gdb）enable breakpoints：启用断点
（gdb）disable breakpoints：禁用断点
（gdb）x：查看内存x/20xw 显示20个单元，16进制，4字节每单元
（gdb）run argv[1] argv[2]：调试时命令行传参
（gdb）set follow-fork-mode child#Makefile项目管理：选择跟踪父子进程（fork()）
```





##### Coredump

​	其实就是操作系统在进程因为某些信号而终止时，将这时的进程空间的快照dump到一个磁盘镜像文件中。

```
常见的core dump错误：
1. 无效指针：对空指针进行了操作；对未初始化的指针进行了操作；使用一个已经释放内存过的指针再次delete 重复释放，（所以说释放后要置空，置空就不会报错）；多线程访问全局变量，导致内存值异常而程序核心转存。

2. 指针越界：检查赋值语句，检查定位到错误上下变量的值，可以结合注释来定位；内存变量值异常，检查定位行，代码走读排查函数调用是使用否有问题。

3. 操作系统特殊性：字节对齐方式引起的程序核心转储；引用模块与自身模块所定义的结构体的字节对齐方式不同。


ulimit -a
ulimit -c

启动coredump: ulimit -c unlimited
调试coredump：gdb a.out core.xxxxx
```



gdb多线程调试： https://blog.csdn.net/zhangye3017/article/details/80382496 

reference： https://blog.csdn.net/daaikuaichuan/article/details/89791255 







