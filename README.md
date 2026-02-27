# Profile Page

Personal blog/portfolio website for GitHub Pages.

## Current Structure

Primary site content lives under `Source/WebSite/`:
- `Source/WebSite/index.html` (home)
- `Source/WebSite/about.html`
- `Source/WebSite/research.html`
- `Source/WebSite/work.html`
- `Source/WebSite/blog.html`
- `Source/WebSite/contact.html`

All pages share a common navigation and stylesheet (`Source/WebSite/styles.css`) and now link to the WebAssembly companion at `wasm/index.html`.

Supporting directories:
- `Source/Updates/` – Markdown log plus converter script for personal updates.
- `Source/WasmSite/` – C++ + template files compiled to WebAssembly.
- `Stage/stage.py` – orchestration entry point (`python Stage/stage.py build --name local`).
- `Stage/Build/` – extra build helpers and documentation.
- `Stage/Builds/` – git-ignored build outputs written by the staging pipeline.

### Personal updates

- Log dated entries in `Source/Updates/Updates.md`.
- Run `python Source/Updates/build_updates.py` (or rely on `Stage/stage.py build`) to generate `Source/WebSite/updates.html`.
- The homepage automatically fetches `updates.html` and displays the latest items in the “Latest Updates” section.

Example entry:

```
## 2026-03-01 – Shipped Dark Mode
- Added a dark theme toggle across all pages.
- Wrote a quick note about our theming approach.
```


### WebAssembly companion

- Source lives in `Source/WasmSite/main.cpp` with a custom shell template (`template.html`).
- Requires the Emscripten SDK (`emcc` on `PATH`). The staging pipeline compiles it automatically; to test manually run `emcc Source/WasmSite/main.cpp --shell-file Source/WasmSite/template.html -O2 -o wasm/index.html`.
- Output is placed under `wasm/` inside each staged build so the main nav can link to `wasm/index.html`.

## Deployment

GitHub Pages is deployed via `.github/workflows/pages.yml`, which installs Emscripten, runs `python Stage/stage.py build --name github-pages`, and uploads `Stage/Builds/github-pages/` (which already contains `.nojekyll`).

### Staging builds locally

```
python Stage/stage.py build --name local
```

The command above refreshes `Source/WebSite/updates.html`, compiles the C++ WebAssembly module, and writes a complete static build (including `wasm/index.html`) to `Stage/Builds/local/`. Inspect that folder or serve it locally before committing; change `--name` to keep multiple build snapshots. The Pages workflow automatically uses the `github-pages` build name.

To preview the staged output with a local HTTP server:

```
python Stage/stage.py webserver --name local --port 4173
```

This will run the same build pipeline and then launch `python -m http.server` rooted at `Stage/Builds/local/` so you can navigate to `http://localhost:4173/` (and `/wasm/`) for testing.



## Todo

update here
