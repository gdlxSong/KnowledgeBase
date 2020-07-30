



## 简介


>https://www.w3cschool.cn/docker/docker-nx3g2gxn.html




### docker exec

1. 切换到容器的shell

```bash
$ docker exec -it <container Name | Id> <bash | sh | ... >
```

2. 在容器中执行命令

```bash
$ docker exec <container Name | Id> <command> flag args

# 执行ps
$ docker exec testC ps aux
```



### docker run

1. 使用（-it）为容器附加shell，默认sh。

```bash
# 附加sh
$ docker run -it --name testC test:v1
# 附加bash
$ docker run -it --name testC test:v1 bash
```

2. 使用（--rm）再容器运行结束之后删除容器

```bash
$ docker run --rm --name testC test:v1
```

容器运行结束自动删除容器。

















