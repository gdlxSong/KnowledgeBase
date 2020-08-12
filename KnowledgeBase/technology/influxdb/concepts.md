## 简介

>https://www.cnblogs.com/shhnwangjian/p/6897216.html



- database: 数据库名，在 InfluxDB 中可以创建多个数据库，不同数据库中的数据文件是隔离存放的，存放在磁盘上的不同目录。
- retention policy: 存储策略，用于设置数据保留的时间，每个数据库刚开始会自动创建一个默认的存储策略 autogen，数据保留时间为永久，之后用户可以自己设置，例如保留最近2小时的数据。插入和查询数据时如果不指定存储策略，则使用默认存储策略，且默认存储策略可以修改。InfluxDB 会定期清除过期的数据。
- measurement: 测量指标名，例如 cpu_usage 表示 cpu 的使用率。
- tag sets: tags 在 InfluxDB 中会按照字典序排序，不管是 tagk 还是 tagv，只要不一致就分别属于两个 key，例如 host=server01,region=us-west 和 host=server02,region=us-west 就是两个不同的 tag set。
- tag--标签，在InfluxDB中，tag是一个非常重要的部分，表名+tag一起作为数据库的索引，是“key-value”的形式。
- field name: 例如上面数据中的 value 就是 fieldName，InfluxDB 中支持一条数据中插入多个 fieldName，这其实是一个语法上的优化，在实际的底层存储中，是当作多条数据来存储。
- timestamp: 每一条数据都需要指定一个时间戳，在 TSM 存储引擎中会特殊对待，以为了优化后续的查询操作。
- Point 由时间戳（time）、数据（field）、标签（tags）组成。相当于表里面的一行数据
- Series 相当于是 InfluxDB 中一些数据的集合，在同一个 database 中，retention policy、measurement、tag sets 完全相同的数据同属于一个 series，同一个 series 的数据在物理上会按照时间顺序排列存储在一起。
- Shard 在 InfluxDB 中是一个比较重要的概念，它和 retention policy 相关联。每一个存储策略下会存在许多 shard，每一个 shard 存储一个指定时间段内的数据，并且不重复，例如 7点-8点 的数据落入 shard0 中，8点-9点的数据则落入 shard1 中。每一个 shard 都对应一个底层的 tsm 存储引擎，有独立的 cache、wal、tsm file。


## 索引

在InfluxDB中，tag是一个非常重要的部分，measurement+tag一起作为数据库的索引，是“key-value”的形式。





## retention policy

一个database可以有多个retrntion policy。

设置了保存策略后，若此保存策略为设置成默认保存策略（一个库可以有多个保存策略），则在查询时，表名（measurement）前，要加上保存策略。

example:

`select * from two-hour.measure where time > now() -10`

two-hour是rp。

也可以：`select * from db_name.rp_name.measurement where time > now() -10`




