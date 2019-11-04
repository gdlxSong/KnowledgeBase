##### gitCommand



##### git help

```
git help command.
-------
git help config
```



##### git config

```groovy
git config -l
	列出所有配置值	//git config --list
git config user.name
	列出用户名. 同理其他字段属性.
git config --global user.name "John Doe"
	对全局进行配置属性字段.
```



##### git status



##### git diff

```
git diff
	比较工作区和暂存区的文件的不同
git diff --cached
	比较暂存区和仓库中的不同
-----------------------------------
git diff [<options>] [<commit>] [--] [<path>…​]
git diff [<options>] --cached [<commit>] [--] [<path>…​]
git diff [<options>] <commit> <commit> [--] [<path>…​]
git diff [<options>] <blob> <blob>
git diff [<options>] --no-index [--] <path> <path>
------------------------------------
git diff [<options>] <blob> <blob>
git diff [<options>] filename

```

##### git rm

```
git rm filename
	移除文件并停止对其的追踪(其实就是暂存里面不包含外加工作目录删除)
git rm --cached filename
	只是从暂存中移除，停止对其的跟踪.

//其实git rm 等效:
	rm filename
	git add *
	//其原因是git add 会更新索引文件
```



##### git log

```
git log
git log -p
	-p可以展开每次提交的内容差异
git log -p -2
	用 -2 则仅显示最近的两次更新
git log --stat
	仅显示简要的增改行数统计
git log --pretty=oneline
	用 oneline 将每个提交放在一行显示，这在提交数很大时非常有用
git log --pretty=format:"%h - %an, %ar : %s"
	示例格式输出.
git log --graph
```

```
选项 说明
    -p 按补丁格式显示每个更新之间的差异。
    --stat 显示每次更新的文件修改统计信息。
    --shortstat 只显示 --stat 中最后的行数修改添加移除统计。
    --name-only 仅在提交信息后显示已修改的文件清单。
    --name-status 显示新增、修改、删除的文件清单。
    --abbrev-commit 仅显示 SHA-1 的前几个字符，而非所有的 40 个字符。
    --relative-date 使用较短的相对时间显示（比如，“2 weeks ago”）。
    --graph 显示 ASCII 图形表示的分支合并历史。
    --pretty 使用其他格式显示历史提交信息。可用的选项包括 oneline，short，full，fuller 和 format（后跟指定格式）。
```

```
选项 说明
    %H 提交对象（commit）的完整哈希字串
    %h 提交对象的简短哈希字串
    %T 树对象（tree）的完整哈希字串
    %t 树对象的简短哈希字串
    %P 父对象（parent）的完整哈希字串
    %p 父对象的简短哈希字串
    %an 作者（author）的名字
    %ae 作者的电子邮件地址
    %ad 作者修订日期（可以用 -date= 选项定制格式）
    %ar 作者修订日期，按多久以前的方式显示
    %cn 提交者(committer)的名字
    %ce 提交者的电子邮件地址
    %cd 提交日期
    %cr 提交日期，按多久以前的方式显示
    %s 提交说明
```

```
git log --since=2.weeks
	查看最近两周的提交
git log --pretty="%h - %s" --author=gitster --since="2008-10-01" \
    --before="2008-11-01" --no-merges -- t/
```

```
选项 说明
    -(n) 仅显示最近的 n 条提交
    --since, --after 仅显示指定时间之后的提交。
    --until, --before 仅显示指定时间之前的提交。
    --author 仅显示指定作者相关的提交。
    --committer 仅显示指定提交者相关的提交。
```

##### git commit

```
git commit -m "comment"
git commit --amend
	追加到上一次提交
```



---------

#### remote repository

##### git remote

```
git remote
	查看远程仓库名称列表
git remote -v
	查看远程仓库+地址(url)

git remote add pb git://github.com/paulboone/ticgit.git
	添加远程仓库

```

```
verbose:
git remote [-v | --verbose]
git remote add [-t <branch>] [-m <master>] [-f] [--[no-]tags] [--mirror=<fetch|push>] <name> <url>
git remote rename <old> <new>
git remote remove <name>
git remote set-head <name> (-a | --auto | -d | --delete | <branch>)
git remote set-branches [--add] <name> <branch>…​
git remote get-url [--push] [--all] <name>
git remote set-url [--push] <name> <newurl> [<oldurl>]
git remote set-url --add [--push] <name> <newurl>
git remote set-url --delete [--push] <name> <url>
git remote [-v | --verbose] show [-n] <name>…​
git remote prune [-n | --dry-run] <name>…​
git remote [-v | --verbose] update [-p | --prune] [(<group> | <remote>)…​]
```

