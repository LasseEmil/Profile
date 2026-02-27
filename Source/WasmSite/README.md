# WasmSite

C++ WebAssembly companion for the main portfolio. Build prerequisites:

- Emscripten SDK (`emcc` available on PATH)
- Python 3 (handled by Stage/stage.py)

To build manually:

```
emcc Source/WasmSite/main.cpp \
  -O2 \
  --shell-file Source/WasmSite/template.html \
  -s ALLOW_MEMORY_GROWTH=1 \
  -s ASSERTIONS=1 \
  -s ENVIRONMENT=web \
  -o Stage/Builds/manual-wasm/index.html
```

Stage orchestration will run the above automatically via `python Stage/stage.py build --name <build>` and place the emitted assets under `Stage/Builds/<build>/wasm/`.
