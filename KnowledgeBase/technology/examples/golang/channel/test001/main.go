package main

import "fmt"

func main() {

	ch := make(chan int, 1)
	fmt.Println(len(ch), " ", cap(ch))
}
