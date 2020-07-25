## 简介

refs，引用，其实就是分支的引用。

这样说吧，其实在git repository中，版本管理的基本单位是commit对象，commit对象管理这次提交的所有信息和数据（commit-message, commit-tree, blob...）

一条分支是一条commit对象的链，我们通过分支名来管理仓库。

refs在.git/refs/文件夹，里面保存了分支和tag，其实这些都是，refs里面所有文件都是保存一个commit节点的id，引用它。






https://www.cnblogs.com/0616--ataozhijia/p/4165052.html