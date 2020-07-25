## 简介


~~~

## remote: GitLab: You are not allowed to push code to protected branches on this project.

没有权限push，delete。

1. 首先确认你是否是项目的成员或管理员。不是的话需要在gitlab里面加。
2. 确认分支是否是protected branch，如果是，developer是没有权限push的。可以创建新分支提交。
3. 是否需要review提交，如果是，需要带上refs/for/xxx来push。


###  ! [remote rejected] xxx -> xxx (pre-receive hook declined)


原因在于：
    1. master分支是保护分支，需要创建新分支提交。
    2. 需要以review方式提交。

解决方案：
```bash
$ git push origin example:refs/for/example
```



## error: RPC failed; HTTP 411 curl 22 The requested URL returned error: 411 Length Required


>If you attempt to push a large set of changes to a Git repository with HTTP or HTTPS, you may get an error message such as error: RPC failed; result=22, HTTP code = 411. This is caused by a Git configuration default which limits certain HTTP operations to 1 megabyte.


解决方案：

```bash
git config http.postBuffer *bytes*
# 500M buffer.
git config http.postBuffer 524288000
```