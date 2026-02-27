#!/usr/bin/env python3
"""Stage the static site into Stage/Builds/<build-name>.

Steps performed:
1. Run the updates converter to refresh Source/Website/updates.html.
2. Copy Source/Website/ into Stage/Builds/<build-name>.
3. Drop a .nojekyll marker so GitHub Pages serves files verbatim.
"""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
UPDATES_SCRIPT = REPO_ROOT / "Source" / "Updates" / "build_updates.py"
SOURCE_DIR = REPO_ROOT / "Source" / "Website"
BUILDS_ROOT = REPO_ROOT / "Stage" / "Builds"


def run_updates() -> None:
    subprocess.run([sys.executable, str(UPDATES_SCRIPT)], check=True)


def copy_site(destination: Path) -> None:
    if destination.exists():
        shutil.rmtree(destination)
    shutil.copytree(SOURCE_DIR, destination)
    (destination / ".nojekyll").touch()


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Stage the site into Stage/Builds")
    parser.add_argument(
        "--name",
        default="local",
        help="Name of the build folder under Stage/Builds (default: local)",
    )
    return parser.parse_args()


def main() -> None:
    args = parse_args()
    run_updates()
    build_dir = BUILDS_ROOT / args.name
    build_dir.parent.mkdir(parents=True, exist_ok=True)
    copy_site(build_dir)
    print(f"[stage] Built site at {build_dir}")


if __name__ == "__main__":
    main()
