## 简介




>https://www.cnblogs.com/chyingp/p/pm2-documentation.html







## 启动


```bash
pm2 start <path> [options [args]]

    path: 通常是配置文件（yml，js，json...）

# options
    --watch：监听应用目录的变化，一旦发生变化，自动重启。如果要精确监听、不见听的目录，最好通过配置文件。
    -i --instances：启用多少个实例，可用于负载均衡。如果-i 0或者-i max，则根据当前机器核数确定实例数目。
    --ignore-watch：排除监听的目录/文件，可以是特定的文件名，也可以是正则。比如--ignore-watch="test node_modules "some scripts""
    -n --name：应用的名称。查看应用信息的时候可以用到。
    -o --output <path>：标准输出日志文件的路径。
    -e --error <path>：错误输出日志文件的路径。
    --interpreter <interpreter>：the interpreter pm2 should use for executing app (bash, python...)。比如你用的coffee script来编写应用。

## example
pm2 start app.js --watch -i 2
```


## 重启

```bash
pm2 restart <path|id|name|namespace|all>

# example
pm2 restart app.js
```

## 自动重启

```bash
pm2 start <path> --watch
# --watch监控整个项目，可在配置文件中配置watch、ignore_watch字段来设置。
```

## 停止

```bash
pm2 stop <path|id|name|namespace|all>

# example
pm2 stop app.js
```


## 删除


```bash
pm2 delete <path|id|name|namespace|all>

# example
pm2 delete app.js
```

## 查看


```bash
# 列出所有进程
pm2 list [<namespace>]

# 查看某一进程的详细信息
pm2 describe <id|name|>
```

## 日志查看

```bash
pm2 logs
```



## Configuration


```bash
{
  "name"        : "fis-receiver",  // 应用名称
  "script"      : "./bin/www",  // 实际启动脚本
  "cwd"         : "./",  // 当前工作路径
  "watch": [  // 监控变化的目录，一旦变化，自动重启
    "bin",
    "routers"
  ],
  "ignore_watch" : [  // 从监控目录中排除
    "node_modules", 
    "logs",
    "public"
  ],
  "watch_options": {
    "followSymlinks": false
  },
  "error_file" : "./logs/app-err.log",  // 错误日志路径
  "out_file"   : "./logs/app-out.log",  // 普通日志路径
  "env": {
      "NODE_ENV": "production"  // 环境参数，当前指定为生产环境
  }
}
```

### 环境切换



首先通过在配置文件中通过env_xx来声明不同环境的配置，然后在启动应用时，通过--env参数指定运行的环境。

```json
// env为默认的环境配置（生产环境），env_dev、env_test则分别是开发、测试环境。可以看到，不同环境下的NODE_ENV、REMOTE_ADDR字段的值是不同的。
// 在应用中，可以通过process.env.REMOTE_ADDR等来读取配置中生命的变量。

 "env": {
    "NODE_ENV": "production",
    "REMOTE_ADDR": "http://www.example.com/"
  },
  "env_dev": {
    "NODE_ENV": "development",
    "REMOTE_ADDR": "http://wdev.example.com/"
  },
  "env_test": {
    "NODE_ENV": "test",
    "REMOTE_ADDR": "http://wtest.example.com/"
  }
```

`pm2 start app.js --env dev`



### 负载均衡
命令如下，表示开启三个进程。如果-i 0，则会根据机器当前核数自动开启尽可能多的进程。

```bash

pm2 start app.js -i 3 # 开启三个进程
pm2 start app.js -i max # 根据机器CPU核数，开启对应数目的进程 
```



## monitor

`pm2 monit`


## 内存超上限自动重启


`pm2 start big-array.js --max-memory-restart 20M`



## 配置文件详解


```json
{
    "apps": {
        "name": "wuwu",                             // 项目名          
        "script": "./bin/www",                      // 执行文件
        "cwd": "./",                                // 根目录
        "args": "",                                 // 传递给脚本的参数
        "interpreter": "",                          // 指定的脚本解释器
        "interpreter_args": "",                     // 传递给解释器的参数
        "watch": true,                              // 是否监听文件变动然后重启
        "ignore_watch": [                           // 不用监听的文件
            "node_modules",
            "logs"
        ],
        "exec_mode": "cluster_mode",                // 应用启动模式，支持fork和cluster模式
        "instances": 4,                             // 应用启动实例个数，仅在cluster模式有效 默认为fork；或者 max
        "max_memory_restart": 8,                    // 最大内存限制数，超出自动重启
        "error_file": "./logs/app-err.log",         // 错误日志文件
        "out_file": "./logs/app-out.log",           // 正常日志文件
        "merge_logs": true,                         // 设置追加日志而不是新建日志
        "log_date_format": "YYYY-MM-DD HH:mm:ss",   // 指定日志文件的时间格式
        "min_uptime": "60s",                        // 应用运行少于时间被认为是异常启动
        "max_restarts": 30,                         // 最大异常重启次数，即小于min_uptime运行时间重启次数；
        "autorestart": true,                        // 默认为true, 发生异常的情况下自动重启
        "cron_restart": "",                         // crontab时间格式重启应用，目前只支持cluster模式;
        "restart_delay": "60s"                      // 异常重启情况下，延时重启时间
        "env": {
           "NODE_ENV": "production",                // 环境参数，当前指定为生产环境 process.env.NODE_ENV
           "REMOTE_ADDR": "爱上大声地"               // process.env.REMOTE_ADDR
        },
        "env_dev": {
            "NODE_ENV": "development",              // 环境参数，当前指定为开发环境 pm2 start app.js --env_dev
            "REMOTE_ADDR": ""
        },
        "env_test": {                               // 环境参数，当前指定为测试环境 pm2 start app.js --env_test
            "NODE_ENV": "test",
            "REMOTE_ADDR": ""
        }
    }
}
```



