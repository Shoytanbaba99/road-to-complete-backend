# Week 7 - Day 3 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Data Structures: Arrays, Slices (`len` vs `cap` header), Maps (Hash Table bucket array & `nil` map behavior), and Strings vs. UTF-8 Runes (`int32` code points).

---

## 🌐 Slice & String Memory Layout Architecture

```text
[ SLICE HEADER (3 Words / 24 Bytes on 64-bit) ]
  ├── Pointer ──► Backing Array in Memory [ Element 0 | Element 1 | Element 2 | ... ]
  ├── Length   ──► (Number of accessible elements: len(slice))
  └── Capacity ──► (Total allocated backing array space: cap(slice))

[ STRING UTF-8 MEMORY ]
  "Hello 🌍"
  ├── ASCII Bytes: 'H','e','l','l','o',' ' (6 bytes)
  └── Emoji Byte Sequence: 0xF0, 0x9F, 0x8D, 0x8E (4 bytes for 🌍)
  ├── len(str)                     = 10 bytes
  └── utf8.RuneCountInString(str)  = 7 runes (human characters)
```

---

## 1. Core Go Data Structure Mechanics

| Data Structure | Internal Implementation | Key Gotchas / Rules |
|---|---|---|
| **Array** | Fixed-size value type `[N]T` | Passing an array to a function copies the entire array contents. |
| **Slice** | Dynamic descriptor `[]T` pointing to backing array | Growing past `cap` triggers reallocation (doubles backing array size). |
| **Map** | Hash table bucket array | Writing to a `nil` map causes a runtime panic; initialize with `make(map[K]V)`. |
| **String** | Read-only slice of bytes | Indexing `str[i]` returns a raw `byte`, not a character. Use `[]rune` for Unicode. |
| **Rune** | Alias for `int32` | Represents a single Unicode code point. |
