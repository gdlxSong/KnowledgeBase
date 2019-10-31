#### chrono

在std::chrono这个namespace中有三个重要的关于时间的概念：

###### Duration

​	时间段，如1min，2hours，........

###### Time points

​	时间点(时刻)，如tom的生日，今天的黄昏，chrono的时间点的公共起点是1970.1.1。

###### Clocks

​	A framework that relates a time point to real physical time. std::chrono这个库的时钟有三个： system_clock, steady_clock, high_resolution_clock. high_resolution_clock是前两者中的其中一个的宏.





###### std::chrono::duration	

```c++
template <class Rep, class Period = ratio<1> >
class duration;

Rep: duration内部存储数据的类型(我的事int64_t)
Period: A ratio type that represents the period in seconds.


    
Method:

count(): 返回一个和Rep相关的时间段值，单位并不一定是秒。



```

​	需要注意的是，chrono库适合ratio这个库耦合的，可以使用ratio进行时间单位的转换。





















































