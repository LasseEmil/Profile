#!/usr/bin/env python3
"""Convert Updates.md (Farmdown-style Markdown) into an HTML snippet.

The converter is intentionally simple: it supports headings that start with
"## " and bullet lists beginning with "- ". The output HTML snippet is
written to `Source/Website/updates.html` so it can be injected into the
homepage at runtime.
"""

from __future__ import annotations

import argparse
import html
from pathlib import Path

DEFAULT_INPUT = Path("Source/Updates/Updates.md")
DEFAULT_OUTPUT = Path("Source/Website/updates.html")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Convert Updates.md to HTML snippet")
    parser.add_argument("--input", "-i", type=Path, default=DEFAULT_INPUT,
                        help="Path to the Markdown updates file")
    parser.add_argument("--output", "-o", type=Path, default=DEFAULT_OUTPUT,
                        help="Destination HTML snippet")
    return parser.parse_args()


def convert(lines: list[str]) -> str:
    body: list[str] = []
    in_list = False

    def close_list() -> None:
        nonlocal in_list
        if in_list:
            body.append("</ul>")
            in_list = False

    for raw_line in lines:
        line = raw_line.rstrip("\n")
        stripped = line.strip()
        if not stripped:
            close_list()
            continue
        if stripped.startswith("## "):
            close_list()
            heading = html.escape(stripped[3:].strip())
            body.append(f"<h3>{heading}</h3>")
            continue
        if stripped.startswith("- "):
            if not in_list:
                body.append('<ul class="updates-list">')
                in_list = True
            body.append(f"  <li>{html.escape(stripped[2:].strip())}</li>")
            continue
        close_list()
        body.append(f"<p>{html.escape(stripped)}</p>")

    close_list()

    content = "\n".join(body)
    return (
        "<div class=\"updates-feed\">\n"
        f"{content}\n"
        "</div>\n"
    )


def main() -> None:
    args = parse_args()
    text = args.input.read_text(encoding="utf-8")
    html_snippet = convert(text.splitlines())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(html_snippet, encoding="utf-8")
    print(f"[updates] Wrote {args.output}")


if __name__ == "__main__":
    main()
