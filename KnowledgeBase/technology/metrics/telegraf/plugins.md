## 简介


>https://docs.influxdata.com/telegraf/v1.8/plugins/

>https://www.influxdata.com/blog/telegraf-go-collection-agent/


在telegraf的插件中，可以分为四种：

1. input
2. processer
3. aggregator
4. output



### 插件编写

>https://www.influxdata.com/blog/telegraf-go-collection-agent/

>https://godoc.org/github.com/influxdata/telegraf#Input


》https://github.com/influxdata/telegraf/blob/master/CONTRIBUTING.md#input-plugin-example

》https://github.com/influxdata/telegraf/blob/master/docs/INPUTS.md



插件编写：

1. 首先在telegraf/plugins/对应的插件目录下（如inputs）下开始自己的插件编写。插件编写的规则https://github.com/influxdata/telegraf/blob/master/docs/INPUTS.md
2. 在对用插件下的all/all.go下导入插件










