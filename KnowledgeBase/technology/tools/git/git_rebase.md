## 简介

>https://gitee.com/progit/3-Git-%E5%88%86%E6%94%AF.html#3.6-%E5%88%86%E6%94%AF%E7%9A%84%E8%A1%8D%E5%90%88


rebase其实就是一个补丁过程，它会将源branch上的有异于目标branch的所有提交过程在目的branch上执行一遍。

\master\--c5
        \c1--c2--c3--c4
                      |
                    branch1

`git rebase master branch1`, 将branch的进度应用到master的基础上。

\master\c5--c1\`--c2\`--c3\`--c4\`
                               |
                             branch1
                             master

### Syntax


```bash
# 将branchname衍合到当前分支
git rebase branchname
    
# 将branch2衍合到branch1
git rebase branch1 branch2
```