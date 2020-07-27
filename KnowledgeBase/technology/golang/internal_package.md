
## Internal packages

Go's package system makes it easy to structure programs into components with clean boundaries, but there are only two forms of access: local (unexported) and global (exported). Sometimes one wishes to have components that are not exported, for instance to avoid acquiring clients of interfaces to code that is part of a public repository but not intended for use outside the program to which it belongs.

The Go language does not have the power to enforce this distinction, but as of Go 1.4 the go command introduces a mechanism to define "internal" packages that may not be imported by packages outside the source subtree in which they reside.

To create such a package, place it in a directory named internal or in a subdirectory of a directory named internal. When the go command sees an import of a package with internal in its path, it verifies that the package doing the import is within the tree rooted at the parent of the internal directory. For example, a package .../a/b/c/internal/d/e/f can be imported only by code in the directory tree rooted at .../a/b/c. It cannot be imported by code in .../a/b/g or in any other repository.

For Go 1.4, the internal package mechanism is enforced for the main Go repository; from 1.5 and onward it will be enforced for any repository.

Full details of the mechanism are in the design documen.




## 简而言之

从go1.4开始有了`local (unexported) and global (exported)`两种访问权限的package，具有内部访问权限的package放在internal目录中（本项目的任意internal）。

### 导入权限检查

在我们执行import package的时候会对包的访问权限做检查，检查它是否是内部包，此次import是否具有访问内部包的权限。

### 具有访问内部包的条件

/projects
        \web\server
            \internal
                    \dao
                    \dto
                    \auth
                    \\...

在internal父目录为根目录的下面的所有package具有使用此`internal package`的访问权限。











