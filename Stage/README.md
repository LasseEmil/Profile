# Stage Directory

- `Build/`: helper scripts and metadata referenced by the staging pipeline.
- `Builds/`: generated site builds; run `python Stage/stage.py build --name <build>` to refresh (defaults to `local`).
- `.emscripten_cache/`: automatically created cache for Emscripten when compiling the WASM companion (git-ignored).
