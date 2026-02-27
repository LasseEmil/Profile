# Stage/Build

Helper scripts and metadata for producing deployable site builds. Use `python Stage/stage.py build --name <target>` to orchestrate Markdown conversion, static site copy, and WebAssembly compilation into `Stage/Builds/<target>/`. For live previews, run `python Stage/stage.py webserver --name <target> --port 4173`.
