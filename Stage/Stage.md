# Stage Directory

- `Build/`: helper scripts and metadata referenced by the staging pipeline.
- `Builds/`: generated site builds; run `python Stage/stage.py build --name <build>` to refresh (defaults to `local`).
- `.emscripten_cache/`: automatically created cache for Emscripten when compiling the WASM companion (git-ignored).

Preview locally by running:

```
python Stage/stage.py webserver --name local --port 4173
```

This command rebuilds the site and launches `python -m http.server` rooted at the chosen build directory so you can navigate to `http://localhost:4173/` (and `/wasm/`).
