## 概要


push用于推送本地分支到远程分支。



### 省略本地分支，可以删除远程分支上

```bash
$ git push origin :test
```

将空传递给test，导致分支删除。



### 推送标签

git push的时候不会推送tag，需要--tags。

```bash
$ git push origin --tags
```


### review推送

push代码，如果需要review推送，需要加refs/for/

```bash
$ git push origin HEAD:refs/for/branchname
```


否之，直接`git push origin`会 [remote rejected] master -> master (prohibited by Gerrit)错误。







