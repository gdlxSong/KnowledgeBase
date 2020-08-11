
## 简介


>https://jasper-zhang1.gitbooks.io/influxdb/content/Guide/writing_data.html



influxdb并没有提供restful的api，但是只提供了三个api，简单方便。



### /query

```bash
curl -i -XPOST http://localhost:8086/query --data-urlencode "q=CREATE DATABASE mydb"
```




### /write

```bash
# 单点写入
curl -i -XPOST 'http://localhost:8086/write?db=mydb' --data-binary 'cpu_load_short,host=server01,region=us-west value=0.64 1434055562000000000'

# 多点写入
curl -i -XPOST 'http://localhost:8086/write?db=mydb' --data-binary 'cpu_load_short,host=server02 value=0.67
cpu_load_short,host=server02,region=us-west value=0.55 1422568543702900257
cpu_load_short,direction=in,host=server01,region=us-west value=2.0 1422568543702900257'

# 文件写入
curl -i -XPOST 'http://localhost:8086/write?db=mydb' --data-binary @cpu_data.txt
# cpu_data.txt满足写入格式规范
```






