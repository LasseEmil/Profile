# Profile Page

WebAssembly-first personal site for Lasse Emil. The GitHub Pages build serves the WASM app shell, which swaps page fragments, streams blog posts, and fetches personal updates.

## Repository Tree

```
Profile/
|-- README.md                     # High-level overview (this file)
|-- AGENTS.md                     # Codex runtime guidance + conventions
|-- requirements.txt              # Python deps for staging/utilities
|-- .gitignore                    # Ignores builds, venvs, artifacts
|-- .github/
|   `-- workflows/
|       `-- pages.yml            # GitHub Pages CI (build + deploy)
|-- Source/
|   |-- Source.md                # Documentation for the Source tree
|   |-- Updates/
|   |   |-- Updates.md           # Markdown changelog/personal feed
|   |   `-- build_updates.py     # Converts Markdown log to HTML
|   |-- WebSite/
|   |   |-- styles.css           # Shared styling for the WASM shell
|   |   `-- updates.html         # Generated updates feed (checked in)
|   `-- WasmSite/
|       |-- README.md            # Dev notes for the WebAssembly app
|       |-- main.cpp             # SPA runtime + content renderer
|       `-- template.html        # HTML shell consumed by emcc
|-- Stage/
|   |-- Stage.md                 # Docs covering staging workflows
|   |-- stage.py                 # Build orchestrator + dev server
|   |-- Build/
|   |   `-- README.md            # Extra notes for build helpers
|   `-- Builds/                  # Git-ignored staged artifacts
|-- Stack/
|   |-- Stack.md                 # Dependency/stack documentation
|   `-- Cpython/                 # Virtualenv home for tooling
`-- Source/WebSite/styles.css    # Convenience pointer for editors
```

## Architecture

- **WebAssembly shell** – `Source/WasmSite/main.cpp` + `template.html` render every route (blog, projects, about, research, work, contact). The compiled `index.{html,js,wasm}` becomes the GitHub Pages entrypoint.
- **Shared assets** – `Source/WebSite/` holds the CSS shared by both the WASM bundle and any generated HTML snippets. `updates.html` is produced from Markdown and fetched at runtime.
- **Content feeds** – `Source/Updates/Updates.md` is the canonical update log. `build_updates.py` converts it to HTML so the WASM runtime can `fetch` the feed on load.
- **Stage orchestration** – `Stage/stage.py` runs three phases (`updates`, `static copy`, `emcc`). The script also exposes `webserver` for local previews via `python -m http.server` rooted at `Stage/Builds/<name>/`.
- **CI/CD** – `.github/workflows/pages.yml` provisions Python + Emscripten, runs `python Stage/stage.py build --name github-pages`, and publishes `Stage/Builds/github-pages/` to GitHub Pages with `.nojekyll`.

## Navigation & routing

- Blog is the default landing view. Navigation links emit `load_page` calls into the WASM module, which swaps page fragments without a full reload.
- The shell synchronizes browser history: hashes (e.g., `#/projects`) map to internal routes, while `popstate` replays the correct WASM render so the browser Back/Forward buttons work as expected.
- Projects are populated from the `PROJECTS` array inside `Source/WasmSite/main.cpp`. Clicking a card loads a detail page that reuses the same struct data.

## Blog feed & personal updates

- `BLOG_LIBRARY` in `template.html` defines seed posts. The client duplicates these entries to create an infinite-scroll experience using an `IntersectionObserver` sentinel.
- Updates: add Markdown entries to `Source/Updates/Updates.md`, then run either `python Source/Updates/build_updates.py` directly or any `Stage/stage.py build` target. The generated `updates.html` is committed so GitHub Pages can serve it as a static asset.

Example Markdown entry:

```
## 2026-03-01 – Shipped Dark Mode
- Added a dark theme toggle across all pages.
- Wrote a quick note about our theming approach.
```

## Building & previewing

Create a local artifact named `local`:

```
python Stage/stage.py build --name local
```

Preview over HTTP after building:

```
python Stage/stage.py webserver --name local --port 4173
```

`stage.py` refreshes `Source/WebSite/updates.html`, compiles the C++ to WASM via `emcc`, and copies everything into `Stage/Builds/<name>/` with a `.nojekyll` marker. The GitHub Actions workflow runs the same command with `--name github-pages`, so keeping that build target reproducible locally ensures Pages deploys cleanly.
