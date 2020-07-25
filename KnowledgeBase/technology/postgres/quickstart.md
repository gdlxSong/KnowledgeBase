

## quick start

### 创建用户和数据库

1. 首先创建linux系统用户

`useradd dbuser`

2. 切换到postgres

安装postgres之后，会自动创建postgres用户。

`su postgres`

3. 启动pg控制台

`psql`

4. 个体当前postgres设置密码

`\password postgres`

5. 给最开始创建的linux用户dbuser创建postgres账户

`CREATE USER dbuser WITH PASSWORD 'password';`

6. 创建数据库

`CREATE DATABASE exampledb OWNER dbuser;`


7. 给dbuser赋予pg操作权限，否则只能登陆。

`GRANT ALL PRIVILEGES ON DATABASE exampledb to dbuser;`

8. 退出控制台

`\q`


或者也可以直接在shell中创建postgres的用户：

```bash
sudo -u postgres createuser --superuser dbuser
sudo -u postgres psql
\password dbuser
\q
# 在shell创建postgres数据库
sudo -u postgres createdb -O dbuser exampledb
```


### 登录

`psql -U dbuser -d exampledb -h 127.0.0.1 -p 5432`


在pg的用户对应的linux用户登录的情况下可省略-U：

`psql exampledb`


### 导入外部数据

`psql exampledb < exampledb.sql`



### 控制台基本命令

```bash
\h：查看SQL命令的解释，比如\h select。
\?：查看psql命令列表。
\l：列出所有数据库。
\c [database_name]：连接其他数据库。
\d：列出当前数据库的所有表格。
\d [table_name]：列出某一张表格的结构。
\du：列出所有用户。
\e：打开文本编辑器。
\conninfo：列出当前数据库和连接的信息。
```

### 数据库sql


基本于sql操作一致。










