

## Issues


### refs/for/<branch>将代码提交到哪里？

https://www.cnblogs.com/wangqiang9/p/9699413.html

>When you push to Gerrit, you do git push gerrit HEAD:refs/for/<BRANCH>. This pushes your changes to the staging area (in the diagram, "Pending Changes"). Gerrit doesn't actually have a branch called <BRANCH>; it lies to the git client.