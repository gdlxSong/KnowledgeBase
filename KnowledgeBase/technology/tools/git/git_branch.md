## 由头

~~~

## What

branch是git的一个重点，可以用来对项目开发多个功能进行并行开发。也可以用于补丁~~

-master--|----------------------------/------->
         \ui---\uiweb--------\       /
                \uiclient-----------/



## How

### 创建分支

```bash
$ git branch branchname

options:
    -B:强行创建
```


### 查看分支

```bash
git branch
git branch -a
```

### 切换分支

```bash
$ git checkout branchname
```


### 推送新分支到remote

```bash
$ git push origin newbranch:newbranch
```


## Reference


[1] https://gitee.com/progit/3-Git-%E5%88%86%E6%94%AF.html#3.1-%E4%BD%95%E8%B0%93%E5%88%86%E6%94%AF

