#!/usr/bin/env python3
"""Stage build artifacts under Stage/Builds/<name>.

When running `python Stage/stage.py build` the script will:
1. Refresh `Source/WebSite/updates.html` from Markdown.
2. Copy the static site from `Source/WebSite/`.
3. Compile the WebAssembly site from `Source/WasmSite/` via Emscripten.
4. Emit everything to `Stage/Builds/<name>` with a `.nojekyll` marker.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[1]
UPDATES_SCRIPT = REPO_ROOT / "Source" / "Updates" / "build_updates.py"
WEB_SOURCE = REPO_ROOT / "Source" / "WebSite"
WASM_SOURCE = REPO_ROOT / "Source" / "WasmSite"
BUILDS_ROOT = REPO_ROOT / "Stage" / "Builds"
EM_CACHE_DIR = REPO_ROOT / "Stage" / ".emscripten_cache"


def run_updates() -> None:
    subprocess.run([sys.executable, str(UPDATES_SCRIPT)], check=True)


def copy_web_site(destination: Path) -> None:
    if destination.exists():
        shutil.rmtree(destination)
    shutil.copytree(WEB_SOURCE, destination)


def require_emcc() -> str:
    from shutil import which

    emcc = which("emcc")
    if not emcc:
        raise SystemExit(
            "[stage] emcc (Emscripten) not found. Install via https://emscripten.org/docs/getting_started/downloads.html"
        )
    return emcc


def build_wasm_site(out_dir: Path) -> None:
    import os

    emcc = require_emcc()
    out_dir.mkdir(parents=True, exist_ok=True)
    EM_CACHE_DIR.mkdir(parents=True, exist_ok=True)
    cmd = [
        emcc,
        str(WASM_SOURCE / "main.cpp"),
        "-O2",
        "--shell-file",
        str(WASM_SOURCE / "template.html"),
        "-s",
        "ALLOW_MEMORY_GROWTH=1",
        "-s",
        "ASSERTIONS=1",
        "-s",
        "ENVIRONMENT=web",
        "-o",
        str(out_dir / "index.html"),
    ]
    env = os.environ.copy()
    env["EM_CACHE"] = str(EM_CACHE_DIR.resolve())
    subprocess.run(cmd, check=True, env=env)


def build(name: str) -> None:
    run_updates()
    build_dir = BUILDS_ROOT / name
    build_dir.parent.mkdir(parents=True, exist_ok=True)
    copy_web_site(build_dir)
    (build_dir / ".nojekyll").touch()
    wasm_dir = build_dir / "wasm"
    build_wasm_site(wasm_dir)
    print(f"[stage] Built site at {build_dir}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Stage build orchestrator")
    subparsers = parser.add_subparsers(dest="command", required=True)

    build_parser = subparsers.add_parser("build", help="Create a site build under Stage/Builds")
    build_parser.add_argument(
        "--name",
        default="local",
        help="Name of the build folder under Stage/Builds (default: local)",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    if args.command == "build":
        build(args.name)
    else:
        raise SystemExit(f"Unknown command: {args.command}")


if __name__ == "__main__":
    main()
