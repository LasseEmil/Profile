#include <emscripten.h>
#include <string>
#include <vector>

struct Highlight {
    const char* title;
    const char* detail;
};

struct Update {
    const char* title;
    const char* body;
};

EM_JS(void, set_text, (const char* element_id, const char* content), {
    const el = document.getElementById(UTF8ToString(element_id));
    if (el) {
        el.textContent = UTF8ToString(content);
    }
});

EM_JS(void, append_highlight, (const char* title, const char* detail), {
    const grid = document.getElementById('wasm-highlights');
    if (!grid) return;
    const wrapper = document.createElement('article');
    wrapper.className = 'card';
    const heading = document.createElement('h3');
    heading.textContent = UTF8ToString(title);
    const paragraph = document.createElement('p');
    paragraph.textContent = UTF8ToString(detail);
    wrapper.appendChild(heading);
    wrapper.appendChild(paragraph);
    grid.appendChild(wrapper);
});

EM_JS(void, append_update, (const char* title, const char* body), {
    const list = document.getElementById('wasm-updates');
    if (!list) return;
    const item = document.createElement('li');
    const strong = document.createElement('strong');
    strong.textContent = UTF8ToString(title);
    const span = document.createElement('span');
    span.textContent = UTF8ToString(body);
    item.appendChild(strong);
    item.appendChild(document.createElement('br'));
    item.appendChild(span);
    list.appendChild(item);
});

int main() {
    set_text("wasm-hero-title", "WebAssembly Companion");
    set_text("wasm-hero-desc",
             "Small C++ runtime that mirrors the main site while showcasing a WASM delivery pipeline.");

    const std::vector<Highlight> highlights = {
        {"Compiled UI", "DOM elements are created from C++ via Emscripten bindings."},
        {"Shared Palette", "Uses the same CSS assets as the main site for a seamless jump."},
        {"Deterministic Builds", "`stage.py build` compiles this module alongside the static pages."},
    };

    for (const auto& highlight : highlights) {
        append_highlight(highlight.title, highlight.detail);
    }

    const std::vector<Update> updates = {
        {"Stage Pipeline", "Invoking `python Stage/stage.py build` emits wasm assets under Stage/Builds/"},
        {"Farmdown Bridge", "Markdown updates still hydrate the main landing page."},
        {"Future Ready", "Extend this module with more interactive widgets as needed."},
    };

    for (const auto& update : updates) {
        append_update(update.title, update.body);
    }

    return 0;
}
