

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
> CREATE RETENTION POLICY "rp_name" ON "db_name" DURATION 2h REPLICATION 1 DEFAULT
# replication 1：副本个数，一般为1就可以了；
# default：设置为默认策略
```
#### 修改rp

```bash
alter retention policy "rp_name" on "db_name" duration 30d default
```

#### 删除rp

```bash
drop retention policy "rp_name" on "db_name"
```


#### 创建连续查询continuous query

InfluxDB的连续查询是在数据库中自动定时启动的一组语句，语句中必须包含 SELECT 关键词和 GROUP BY time() 关键词。

InfluxDB会将查询结果放在指定的数据表中。

目的：使用连续查询是最优的降低采样率的方式，连续查询和存储策略搭配使用将会大大降低InfluxDB的系统占用量。而且使用连续查询后，数据会存放到指定的数据表中，这样就为以后统计不同精度的数据提供了方便。

`CREATE CONTINUOUS QUERY`

>https://www.cnblogs.com/shhnwangjian/p/6897216.html

```bash
# create syntax
CREATE CONTINUOUS QUERY <cq_name> ON <database_name>
[RESAMPLE [EVERY <interval>] [FOR <interval>]]
BEGIN SELECT <function>(<stuff>)[,<function>(<stuff>)] INTO <different_measurement>
FROM <current_measurement> [WHERE <stuff>] GROUP BY time(<interval>)[,<stuff>]
END

# example
CREATE CONTINUOUS QUERY "cq_30m" ON "food_data" BEGIN
  SELECT mean("website") AS "mean_website",mean("phone") AS "mean_phone"
  INTO "a_year"."downsampled_orders"
  FROM "orders"
  GROUP BY time(30m)
END

# example
CREATE CONTINUOUS QUERY wj_30m ON shhnwangjian BEGIN SELECT mean(connected_clients), MEDIAN(connected_clients), MAX(connected_clients), MIN(connected_clients) INTO redis_clients_30m FROM redis_clients GROUP BY ip,port,time(30m)

# example
CREATE CONTINUOUS QUERY wj_30m ON shhnwangjian_30 BEGIN SELECT mean(connected_clients), MEDIAN(connected_clients), MAX(connected_clients), MIN(connected_clients) INTO shhnwangjian_30.autogen.redis_clients_30m FROM shhnwangjian.autogen.redis_clients GROUP BY ip,port,time(30m)
```

#### 查看当前数据库的所有的measurement

`show measurements`


#### 查看measurement的series

`show series from mmeasurement_name`

series表示这个表里面的数据，可以在图表上画成几条线，series主要通过tags排列组合算出来。

时序数据库中是不需要创建表的。


### 产看当前数据库的retention policy

`show retention policies on "db_name"`


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


#### Group By

注意




### influxdb的字段类型

```bash
> select uptime::integer from iot_gateway
```

influxdb的字段value类型有integer，float，boolean，string，可以通过在select <fieldname>[::<type>]指定，如果指定正确（解析正确），那么返回，否之返回空结果集。




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

