

## 简介


>https://godoc.org/github.com/uber/jaeger-lib/metrics#Factory

### Metrics

```go
func Metrics(factory metrics.Factory) Option
// Metrics creates an Option that initializes Metrics in the tracer,
// which is used to emit statistics about spans.
func Metrics(factory metrics.Factory) Option {
	return func(c *Options) {
		c.metrics = factory
	}
}

type Factory interface {
    Counter(metric Options) Counter
    Timer(metric TimerOptions) Timer
    Gauge(metric Options) Gauge
    Histogram(metric HistogramOptions) Histogram

    // Namespace returns a nested metrics factory.
    Namespace(scope NSOptions) Factory
}
```

metrics用来上报统计信息。


