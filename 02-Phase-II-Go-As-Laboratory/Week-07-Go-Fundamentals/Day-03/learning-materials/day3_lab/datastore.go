package main

import (
	"errors"
)

type Cache struct {
	lookup map[string][]byte
	tags   []string
}

func NewCache() *Cache {
	return &Cache{
		lookup: make(map[string][]byte),
		tags:   make([]string, 0, 10),
	}
}

func (c *Cache) Put(key string, val []byte) {
	c.lookup[key] = val
	c.tags = append(c.tags, key)
}

func (c *Cache) Get(key string) ([]byte, error) {
	val, exists := c.lookup[key]
	if !exists {
		return nil, errors.New("key not found")
	}
	return val, nil
}
