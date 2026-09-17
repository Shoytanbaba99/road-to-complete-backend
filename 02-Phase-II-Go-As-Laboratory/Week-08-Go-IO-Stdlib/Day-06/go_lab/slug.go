package golab

import (
	"strings"
	"unicode"
)

func Slugify(s string) string {
	var b strings.Builder
	previousSpace := false
	for _, r := range s {
		switch {
		case r == ' ':
			if !previousSpace {
				b.WriteRune('-')
				previousSpace = true
			}
		case r == '!' || r == '@' || r == '?':

		default:
			b.WriteRune(unicode.ToLower(r))
			previousSpace = false
		}
	}
	return b.String()
}
