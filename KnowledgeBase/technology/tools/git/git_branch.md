## 由头

~~~

## What

branch是git的一个重点，可以用来对项目开发多个功能进行并行开发。也可以用于补丁~~

-master--|----------------------------/------->
         \ui---\uiweb--------\       /
                \uiclient-----------/

此外你需要知道分支是分类的，首先本地有远程分支和本地分支。fetch用于同步远程分支（将远程服务器仓库的最新内容同步到本机的远程分支上）。

### 跟踪分支

从远程分支 checkout 出来的本地分支，称为 跟踪分支 (tracking branch)。直接与远程主机的分支联系，可以通过push和pull来进行同步。如clone的时候master就是跟踪分支，explain：跟踪分支在远程主机有对应的分支（区别于本地创建但没有push的分支）。




## How

### 创建分支

```bash
$ git branch branchname

options:
    -B:强行创建，覆盖已有同名分支。
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

