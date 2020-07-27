## 简介

merge用于合并分支。

### Syntax


```bash
git merge [-n] [--stat] [--no-commit] [--squash] [--[no-]edit]
    [-s <strategy>] [-X <strategy-option>] [-S[<keyid>]]
    [--[no-]allow-unrelated-histories]
    [--[no-]rerere-autoupdate] [-m <msg>] [<commit>…​]
git merge --abort
git merge --continue
```

### 常用方法

`git merge <branchname>`

- 将branchname分支合并到当前分支。
- 如果当前分支是branchname分支的先祖，则fast forword，不产生新的commit，当前分支引用branchname分支所指commit-id。
- 如果当前分支不是branchname分支的直接始祖，那么将产生新的commit，并且有可能发生冲突。


### 解决合并冲突

在发生冲突的情况下，git会进行合并，但是却不会提交，需要手动解决冲突，然后才能提交。










