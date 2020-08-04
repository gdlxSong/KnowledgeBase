## 简介


### Log

```go
type Logger
type Logger interface {
    // Error logs a message at error priority
    Error(msg string)

    // Infof logs a message at info priority
    Infof(msg string, args ...interface{})
}
//Logger provides an abstract interface for logging from Reporters. Applications can provide their own implementation of this interface to adapt reporters logging to whatever logging library they prefer (stdlib log, logrus, go-logging, etc).
```