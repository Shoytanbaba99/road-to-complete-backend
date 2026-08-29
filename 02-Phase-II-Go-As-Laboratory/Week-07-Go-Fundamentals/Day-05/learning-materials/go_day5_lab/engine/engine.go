package engine

import (
	"day5lab/metrics"
	"fmt"
)

type Worker struct {
	id        int
	Publisher metrics.Publisher
}

func NewWorker(id int, pub metrics.Publisher) *Worker {
	return &Worker{
		id:        id,
		Publisher: pub,
	}
}

func (w *Worker) ProcessJob(jobName string, duration float64) error {
	fmt.Printf("[ENGINE] Worker %d processing job: %s (%.2fs)\n", w.id, jobName, duration)
	if w.Publisher == nil {
		return fmt.Errorf("no publisher configured")
	}

	return w.Publisher.Publish(jobName, duration)
}
