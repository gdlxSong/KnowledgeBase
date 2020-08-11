

## 简介

>https://www.cnblogs.com/waitig/p/6051171.html





### 数据库操作

#### 启动数据库

`systemctl start influxdb`

#### 启动客户端

`influx`

#### 创建数据库

`cteate database dbname`

#### 切换数据库

`use dbname`

#### 创建retention policy

`CREATE RETENTION POLICY`

```bash
> CREATE RETENTION POLICY "two_hours" ON "food_data" DURATION 2h REPLICATION 1 DEFAULT
```
#### 创建continuous query

`CREATE CONTINUOUS QUERY`

```bash
> CREATE CONTINUOUS QUERY "cq_30m" ON "food_data" BEGIN
  SELECT mean("website") AS "mean_website",mean("phone") AS "mean_phone"
  INTO "a_year"."downsampled_orders"
  FROM "orders"
  GROUP BY time(30m)
END
```

#### 查看当前数据库的所有的measurement

`show measurements`


#### 查看measurement的series

`show series from mmeasurement_name`

series表示这个表里面的数据，可以在图表上画成几条线，series主要通过tags排列组合算出来。

时序数据库中是不需要创建表的。



### 数据操作

>https://jasper-zhang1.gitbooks.io/influxdb/content/Introduction/getting_start.html



数据写入格式：
```bash
<measurement>[,<tag-key>=<tag-value>...] <field-key>=<field-value>[,<field2-key>=<field2-value>...] [unix-nano-timestamp]
```

>在概念上，你可以将measurement类比于SQL里面的table，其主键索引总是时间戳。tag和field是在table里的其他列，tag是被索引起来的，field没有。不同之处在于，在InfluxDB里，你可以有几百万的measurements，你不用事先定义数据的scheme，并且null值不会被存储。


### insert and select

`insert <measurement>[,<tag-key>=<tag-value>...] <field-key>=<field-value>[,<field2-key>=<field2-value>...] [unix-nano-timestamp]`

**Examples：**

```bash
> INSERT cpu,host=serverA,region=us_west value=0.64
> SELECT "host", "region", "value" FROM "cpu"
name: cpu
---------
time                                     host         region   value
2015-10-21T19:28:07.580664347Z  serverA      us_west     0.64

>
> INSERT temperature,machine=unit42,type=assembly external=25,internal=37
>
> SELECT * FROM "temperature"
name: temperature
-----------------
time                                     external      internal     machine    type
2015-10-21T19:28:08.385013942Z  25                37             unit42  assembly

>

# 字段或数据库也可以不加引号, 看版本吧
> select host, value from cpu
```


### 用户管理


```bash
# 显示用户
SHOW USERS
# 创建用户
CREATE USER username WITH PASSWORD 'password'
# 创建管理员权限的用户
CREATE USER username WITH PASSWORD 'password' WITH ALL PRIVILEGES

# 删除用户
DROP USER username

SET PASSWORD FOR admin ='test'
```


需要注意的是：username可以用双引号包裹或者不:[`"username"` | `username`], password必须使用单引号包裹。

