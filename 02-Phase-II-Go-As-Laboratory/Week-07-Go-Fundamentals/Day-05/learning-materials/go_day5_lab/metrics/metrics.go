package metrics

import "fmt"

type Publisher interface {
	Publish(metricName string, value float64) error
}

type consoleEmitter struct {
}

func (c *consoleEmitter) Publish(metricName string, value float64) error {
	fmt.Printf("[METRIC] %s: %.2f\n", metricName, value)
	return nil
}

func NewConsoleEmitter() Publisher {
	return &consoleEmitter{}
}
