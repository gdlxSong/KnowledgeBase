
## 简介


>https://man.linuxde.net/curl



### 常用选项

```bash
# -H 用于指定Header Field
curl -H "accept:application/json" http://www.yqun.xyz

# -G 用于指定Get方法
curl -G http://www.yqun.xyz

# -X 用于指定命令，其实就是POST，PUT，HEAD等
curl -H "accept:application/json" -X POST http://www.yqun.xyz

# -d 用于指定body数据
curl -H "Accept: application/json" -H "Content-type: application/json" -X POST -d '{"phone": "18000011005","password": "xxxxx", "status":40,"order_no":"1998708","config":{"loading":true},"data": "123", "appVersion": "1.2.3","CHEN_ZHE_TEST_ONE_TWO_THREE": 1}'  http://192.168.57.80/mjyx-mall-gateway2/web/index.php/auth/login

# -I或-head用于指定响应返回后，只打印头部字段
curl -I http://www.baidu.com


```

