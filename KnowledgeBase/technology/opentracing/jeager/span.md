

## 简介



### SpanContext



```go

// TraceID represents unique 128bit identifier of a trace
type TraceID struct {
	High, Low uint64
}

// SpanID represents unique 64bit identifier of a span
type SpanID uint64

// SpanContext represents propagated span identity and state
type SpanContext struct {
	// traceID represents globally unique ID of the trace.
	// Usually generated as a random number.
	traceID TraceID

	// spanID represents span ID that must be unique within its trace,
	// but does not have to be globally unique.
	spanID SpanID

	// parentID refers to the ID of the parent span.
	// Should be 0 if the current span is a root span.
	parentID SpanID

	// Distributed Context baggage. The is a snapshot in time.
	baggage map[string]string

	// debugID can be set to some correlation ID when the context is being
	// extracted from a TextMap carrier.
	//
	// See JaegerDebugHeader in constants.go
	debugID string

	// samplingState is shared across all spans
	samplingState *samplingState

	// remote indicates that span context represents a remote parent
	remote bool
}
```



### Span


```go
// Span implements opentracing.Span
type Span struct {
	// referenceCounter used to increase the lifetime of
	// the object before return it into the pool.
	referenceCounter int32

	sync.RWMutex

	tracer *Tracer

	// TODO: (breaking change) change to use a pointer
	context SpanContext

	// The name of the "operation" this span is an instance of.
	// Known as a "span name" in some implementations.
	operationName string

	// firstInProcess, if true, indicates that this span is the root of the (sub)tree
	// of spans in the current process. In other words it's true for the root spans,
	// and the ingress spans when the process joins another trace.
	firstInProcess bool

	// startTime is the timestamp indicating when the span began, with microseconds precision.
	startTime time.Time

	// duration returns duration of the span with microseconds precision.
	// Zero value means duration is unknown.
	duration time.Duration

	// tags attached to this span
	tags []Tag

	// The span's "micro-log"
	logs []opentracing.LogRecord

	// The number of logs dropped because of MaxLogsPerSpan.
	numDroppedLogs int

	// references for this span
	references []Reference

	observer ContribSpanObserver
}
```