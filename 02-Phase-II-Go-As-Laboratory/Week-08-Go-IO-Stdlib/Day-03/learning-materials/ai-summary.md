# Week 8 - Day 3 Architectural Overview & Technical Reference

> **Scope:** High-level architectural reference of Go Configuration Management: Command-Line Flags (`flag` package), Environment Variables (`os.Getenv`), and Config Precedence Cascading (Flags > Env Vars > Defaults).

---

## 🌐 Cascading Configuration Precedence Architecture

```text
[ 1. DEFAULTS ] ──────► DefaultConfig() (e.g. storagePath: ".tasks.json", verbose: false)
                               │
                               ▼
[ 2. ENV VARS ] ──────► os.Getenv("TASK_STORAGE_PATH") (Overrides Default if set)
                               │
                               ▼
[ 3. CLI FLAGS ] ─────► flag.StringVar(&cfg.StoragePath, "storage", ...)
                        (Overrides Env Var & Default if passed on command line)
```

---

## 1. Core Go Configuration Patterns

| Pattern / Utility | Syntax Example | Engineering Advantage |
|---|---|---|
| **Environment Lookup** | `os.Getenv("KEY")` | Reads OS environment variables without failing if unset (returns `""`). |
| **CLI Flag Set** | `fs := flag.NewFlagSet(name, flag.ContinueOnError)` | Parses flags flexibly for subcommands or isolated application testing. |
| **Flag Delimiter `--`** | `app -storage s.json -- positional_arg` | Explicitly marks the end of flag options for POSIX compliance. |
| **Cascading Config** | Defaults ➔ `os.Getenv` ➔ `flag.Parse` | Industry-standard Twelve-Factor App configuration layering. |
