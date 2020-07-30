package main

import (
	"context"
	"fmt"
	"log"
	"time"

	"github.com/coreos/etcd/mvcc/mvccpb"
	"github.com/etcd-io/etcd/clientv3"
)

func fatalIf(err error) {
	if nil != err {
		log.Fatal(err)
	}
}

func watch() {

	cli, err := clientv3.New(clientv3.Config{
		Endpoints:   []string{"139.198.16.178:2379"},
		DialTimeout: 1 * time.Second,
	})
	fatalIf(err)
	defer cli.Close()

	fmt.Println("watch foo")
	watchChannel := cli.Watch(context.Background(), "foo", clientv3.WithPrefix())

	for change := range watchChannel {
		//fmt.Println(change)
		for _, ev := range change.Events {
			switch ev.Type {
			case mvccpb.PUT: //修改或者新增
				fmt.Println(string(ev.Kv.Key), string(ev.Kv.Value))
			case mvccpb.DELETE: //删除
				fmt.Println(string(ev.Kv.Key))
			}
		}
	}

}

func main() {

	watch()
}
