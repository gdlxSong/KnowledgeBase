


## 简介




### tracer

```go
// TraceID represents unique 128bit identifier of a trace
type TraceID struct {
	High, Low uint64
}
// Tracer implements opentracing.Tracer.
type Tracer struct {
	serviceName string
	hostIPv4    uint32 // this is for zipkin endpoint conversion

	sampler  SamplerV2
	reporter Reporter
	metrics  Metrics
	logger   log.DebugLogger

	timeNow      func() time.Time
	randomNumber func() uint64

	options struct {
		gen128Bit                   bool // whether to generate 128bit trace IDs
		zipkinSharedRPCSpan         bool
		highTraceIDGenerator        func() uint64 // custom high trace ID generator
		maxTagValueLength           int
		noDebugFlagOnForcedSampling bool
		maxLogsPerSpan              int
		// more options to come
	}
	// allocator of Span objects
	spanAllocator SpanAllocator

	injectors  map[interface{}]Injector
	extractors map[interface{}]Extractor

	observer compositeObserver

	tags    []Tag
	process Process

	baggageRestrictionManager baggage.RestrictionManager
	baggageSetter             *baggageSetter

	debugThrottler throttler.Throttler
}
```