# Week 10 - Day 1: The HTTP Handler & `net/http` Fundamentals

---

## 📋 Objectives
- [x] Master `net/http` primitives and server bootstrapping (`http.ListenAndServe`)
- [x] Understand handler mechanics (`http.HandleFunc`) and routing dispatch
- [x] Distinguish `http.ResponseWriter` interface semantics vs `*http.Request` pointer mechanics
- [x] Build and test basic HTTP endpoints (`/status`, `/owner`)

---

## 🗺️ Pathways & Files

| File | Description |
|---|---|
| 🧠 [**`my-take.md`**](my-take.md) | Reflections on manual TCP parsing vs `net/http` abstraction, and why `ResponseWriter` has no pointer while `Request` does. |
| 📚 [**`learning-materials/theory-and-docs.md`**](learning-materials/theory-and-docs.md) | Deep breakdown of `net/http` sorting machine mechanics and function pointer routing. |
| 🛠️ [**`learning-materials/main.go`**](learning-materials/main.go) | Hands-on HTTP server with `/status` and `/owner` endpoints inspecting request metadata. |
