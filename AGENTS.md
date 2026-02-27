# Codex Agent Guide for `Profile`

## Purpose
Provide the minimum instructions so Codex agents can work on this repo without fighting GitHub authentication or workflow surprises.

## Pre-flight check for every terminal
Run these commands when the shell starts (or after reboot) **before** any `git` action:

```bash
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519_profile_repo
```

Reasoning:
- GitHub access depends on `ssh-agent` running inside the current shell and holding the key you already configured elsewhere.
- Fresh terminals inherit no agent state, so rerunning the two commands ensures pushes and pulls use SSH automatically.

To confirm the key is loaded later, run `ssh-add -l`. If it says "The agent has no identities", repeat the two commands above in that same shell.

## Syncing the working copy
```bash
cd /home/titan/Shared/Projects/Github
./github_setup.sh
cd Profile
```
The setup script fetches latest changes, checks out `main`, and switches to the configured work branch so you can start editing immediately.

## Standard GitHub workflow
1. `git status -sb` – confirm you are still on `feature/site-refresh` (or the active feature branch).
2. Make and test code/content updates.
3. `git add <files>` – stage the changes needed for the commit.
4. `git commit -m "<concise summary>"` – commit locally.
5. `git push` – branch tracks `origin/feature/site-refresh`, so this uploads over SSH automatically.
6. Open the PR link printed by `git push` (or go to the repo’s Pull Requests page) to raise a review.

## Project scaffold
Keep the repo organized using three top-level service directories plus shared config files:

```
Profile/
├─ AGENTS.md
├─ requirements.txt
├─ Source/
│  ├─ Updates/               # Markdown log + converter for personal updates
│  ├─ WebSite/               # HTML/CSS/JS served by GitHub Pages
│  └─ WasmSite/              # C++ sources + template for the WASM companion
├─ Stage/
│  ├─ Build/                 # Helper scripts + docs for staging
│  ├─ Builds/                # Generated site builds (git-ignored)
│  └─ stage.py               # Orchestration entry point
└─ Stack/
   └─ Cpython/               # Prebuilt Python virtual environment (source Stack/Cpython/bin/activate)
```

- **Source/**: Place all maintainable code, scripts, templates, and data files here. HTML/CSS live under `Source/WebSite/`; Markdown updates stay in `Source/Updates/Updates.md`; the C++ WASM module lives under `Source/WasmSite/`.
- **Stage/**: Use for build tooling (`Stage/stage.py`, `Stage/Build/`) and generated artifacts (`Stage/Builds/`). Run `python Stage/stage.py build --name <build>` to refresh a build folder or `python Stage/stage.py webserver --name local --port 4173` for local previews.
- **Stack/**: Store external dependencies downloaded at build time (e.g., Python venvs, node installs). The `Stack/Cpython` venv is pre-created; activate it with `source Stack/Cpython/bin/activate` before running Python tooling, then `pip install -r requirements.txt` as needed.

Always follow the naming rules below when adding new directories/files inside those folders.

## Naming conventions
- Markdown files and directory names: PascalCase (e.g., `ProfileNotes.md`, `AssetsLibrary/`).
- Scripts, code files, and general filenames: lowercase `snake_case` (e.g., `generate_github_key.sh`, `content_parser.py`).
- MIME-type specific assets default to `snake_case` as well (e.g., `site_styles.css`, `profile_image.png`).
- Documentation rule: keep a single root `README.md`; any directory-level doc must be named after the directory itself (`Dir/Dir.md`).

## Updates workflow
1. Edit `Source/Updates/Updates.md` and add a new `## YYYY-MM-DD – Title` section with bullet items (see README for example format).
2. Run `python Source/Updates/build_updates.py` (activate `Stack/Cpython` first if you install extra Python deps) or simply run `python Stage/stage.py build --name local` to regenerate as part of staging.
3. Commit both the Markdown file and the generated `Source/WebSite/updates.html` so the homepage “Latest Updates” section stays in sync.

## WASM workflow
1. Ensure Emscripten (`emcc`) is installed locally (the CI workflow installs it automatically).
2. Run `python Stage/stage.py build --name local` – this compiles `Source/WasmSite/main.cpp` into `Stage/Builds/local/wasm/`. Use `python Stage/stage.py webserver --name local --port 4173` for a live preview served via `http.server`.
3. Review `Stage/Builds/local/wasm/index.html` in a browser and commit any source changes under `Source/WasmSite/`.

## Quick fix checklist
- Push failing with `Permission denied (publickey)`? Run the two pre-flight commands again in that terminal and retry.
- Wrong upstream? `git branch -u origin/feature/site-refresh` resets tracking.
- Need a new branch? `git checkout -b feature/<name> origin/main && git push -u origin feature/<name>`.
