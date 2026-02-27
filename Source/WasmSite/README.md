# WasmSite

WebAssembly (Emscripten) experience that now powers the entire site. The static template (`template.html`) defines the navigation shell; `main.cpp` renders every page (blog, projects, about, research, work, contact) from C++ structs.

## Project data
- Update the `PROJECTS` array in `main.cpp` to add new entries.
- Each entry follows `Project{"slug","Display Name","Summary","Stack","Status","Detailed description."};`
- The “Projects” view and individual project detail routes are generated from this data.

## Build requirements
- Emscripten SDK (`emcc` available on PATH)
- Python 3 (for the staging script)

## Local build/preview
```
python Stage/stage.py webserver --name local --port 4173
```
This regenerates `Source/WebSite/updates.html`, compiles the WASM site (promoting `index.html/index.js/index.wasm` to the build root), and serves everything via `python -m http.server`.

The GitHub Actions workflow runs the same staging command (`python Stage/stage.py build --name github-pages`) before deploying to Pages, so the published site is 100% static.
