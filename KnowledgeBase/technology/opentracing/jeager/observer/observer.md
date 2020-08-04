

## 简介

### Observer

```go
// Observer can be registered with the Tracer to receive notifications about
// new Spans.
//
// Deprecated: use jaeger.ContribObserver instead.
type Observer interface {
	OnStartSpan(operationName string, options opentracing.StartSpanOptions) SpanObserver
}

// SpanObserver is created by the Observer and receives notifications about
// other Span events.
//
// Deprecated: use jaeger.ContribSpanObserver instead.
type SpanObserver interface {
	OnSetOperationName(operationName string)
	OnSetTag(key string, value interface{})
	OnFinish(options opentracing.FinishOptions)
}
```

Observer作为观察者，在触发Span相关的事件的时候回调。