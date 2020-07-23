


1. URL.Query和URL.ParseQuery会将+转义为空格

```golang
package main

import (
	"encoding/json"
	"fmt"
	"log"
	"net/url"
	"strings"
)

func main() {
	m, err := url.ParseQuery(`x=1&y=2&y=3&z=s+a`)
	if err != nil {
		log.Fatal(err)
	}
	fmt.Println(toJSON(m))
}

func toJSON(m interface{}) string {
	js, err := json.Marshal(m)
	if err != nil {
		log.Fatal(err)
	}
	return strings.ReplaceAll(string(js), ",", ", ")
}

output:
{"x":["1"], "y":["2", "3"], "z":["s a"]}

```


[1] https://www.w3.org/TR/html4/interact/forms.html#h-17.13.4.1