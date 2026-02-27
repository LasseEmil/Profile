#include <cstring>
#include <sstream>
#include <string>

#include <emscripten.h>

EM_JS(void, set_app_html, (const char* content), {
  const root = document.getElementById('app-root');
  if (root) {
    root.innerHTML = UTF8ToString(content);
    window.scrollTo({ top: 0, behavior: 'instant' });
  }
});

EM_JS(void, refresh_updates, (), {
  const container = document.getElementById('updates-feed');
  if (!container) {
    return;
  }
  fetch('updates.html', { cache: 'no-store' })
    .then((response) => {
      if (!response.ok) {
        throw new Error('HTTP ' + response.status);
      }
      return response.text();
    })
    .then((html) => {
      container.innerHTML = html;
    })
    .catch((error) => {
      container.textContent = 'Unable to load updates at this time. Please refresh.';
      console.error('Failed to load updates.html', error);
    });
});

EM_JS(void, announce_page_loaded, (const char* page), {
  if (typeof window !== 'undefined' && typeof window.onWasmPageLoad === 'function') {
    window.onWasmPageLoad(UTF8ToString(page));
  }
});

struct StaticPage {
  const char* key;
  const char* html;
};

static const char BLOG_HTML[] = R"HTML(
<section class="section">
  <p class="eyebrow">Welcome</p>
  <h1>Lasse Emil — personal site and developer update blog.</h1>
  <p>Follow ongoing engineering notes, project logs, and release updates as they happen.</p>
</section>

<section class="section">
  <h2>Recent Posts</h2>
  <div id="blog-feed" class="blog-feed"></div>
</section>

<section class="section updates-section" id="updates">
  <h2>Latest Updates</h2>
  <div id="updates-feed" aria-live="polite">Loading personal updates…</div>
  <p class="updates-fallback">
    <noscript>Enable JavaScript to see the latest updates inline, or open <a href="updates.html">updates.html</a>.</noscript>
  </p>
</section>
)HTML";

static const char ABOUT_HTML[] = R"HTML(
<section class="section">
  <h1>About Lasse Emil</h1>
  <p>
    I'm a software engineer focused on turning ambiguous ideas into reliable, user-facing products. I enjoy end-to-end
    ownership: scoping, architecture, implementation, and iterative improvement.
  </p>
  <p>
    I work best on teams that value engineering quality, thoughtful collaboration, and measurable outcomes.
  </p>
</section>
)HTML";

static const char RESEARCH_HTML[] = R"HTML(
<section class="section">
  <h1>Research Interests</h1>
  <ul>
    <li><strong>AI for engineering workflows:</strong> reducing friction in coding and review.</li>
    <li><strong>Retrieval + LLM systems:</strong> making answers more grounded and reliable.</li>
    <li><strong>Delivery reliability:</strong> balancing speed with quality and maintainability.</li>
  </ul>
</section>
)HTML";

static const char WORK_HTML[] = R"HTML(
<section class="section">
  <h1>Current Work Areas</h1>
  <div class="card-grid">
    <article class="card">
      <h3>Backend Systems</h3>
      <p>APIs and service design with observability and scaling in mind.</p>
    </article>
    <article class="card">
      <h3>Applied AI Features</h3>
      <p>Embedding AI into workflows where quality and speed can be measured.</p>
    </article>
    <article class="card">
      <h3>Product Delivery</h3>
      <p>Partnering with product/design to ship high-impact outcomes quickly.</p>
    </article>
  </div>
</section>
)HTML";

static const char CONTACT_HTML[] = R"HTML(
<section class="section">
  <h1>Contact</h1>
  <p>I'm open to software engineering opportunities and technical discussions.</p>
  <p>Email: <a href="mailto:lasse.emil@example.com">lasse.emil@example.com</a></p>
  <p>GitHub: <a href="https://github.com/LasseEmil" target="_blank" rel="noopener">github.com/LasseEmil</a></p>
</section>
)HTML";

static const StaticPage STATIC_PAGES[] = {
    {"blog", BLOG_HTML},
    {"about", ABOUT_HTML},
    {"research", RESEARCH_HTML},
    {"work", WORK_HTML},
    {"contact", CONTACT_HTML},
};

struct Project {
  const char* key;
  const char* name;
  const char* summary;
  const char* stack;
  const char* status;
  const char* detail;
};

static const Project PROJECTS[] = {
    {"compute-ai",
     "ComputeAI",
     "Elastic compute orchestration with LLM-aware scheduling",
     "Rust, gRPC, Kubernetes",
     "Pilot",
     "Building a scheduler that batches inference and traditional workloads without starving latency-sensitive traffic."
    },
    {"harness-ai",
     "HarnessAI",
     "Delivery insights layered on top of engineering telemetry",
     "Python, Temporal, Postgres",
     "In Progress",
     "Combining deployment analytics with AI summaries so teams can spot release risk before it hits production."
    },
    {"gateway-ai",
     "GatewayAI",
     "Unified gateway for multi-model experimentation",
     "Go, Envoy, OpenAI/Azure endpoints",
     "Prototype",
     "Provides traffic splitting, safety filters, and metrics for testing multiple foundation models behind a single API."
    },
    {"secbond-brain",
     "SecbondBrain",
     "Security knowledge base with natural-language search",
     "Next.js, Typesense, LangChain",
     "Research",
     "Captures incident retros, guardrails, and playbooks so security engineers can query tribal knowledge quickly."
    },
};

const StaticPage* find_static(const char* key) {
  for (const auto& page : STATIC_PAGES) {
    if (std::strcmp(page.key, key) == 0) {
      return &page;
    }
  }
  return nullptr;
}

const Project* find_project(const char* slug) {
  for (const auto& project : PROJECTS) {
    if (std::strcmp(project.key, slug) == 0) {
      return &project;
    }
  }
  return nullptr;
}

std::string build_projects_page() {
  std::ostringstream html;
  html << "<section class='section'>";
  html << "<h1>Projects</h1>";
  html << "<p>Scroll the cards to preview ongoing work. Click any project to open a detail page with the current status.</p>";
  html << "</section>";
  html << "<div class='card-grid project-grid'>";
  for (const auto& project : PROJECTS) {
    html << "<article class='card project-card' data-project='" << project.key << "'>";
    html << "<header><h3>" << project.name << "</h3><p>" << project.summary << "</p></header>";
    html << "<ul class='project-meta'>";
    html << "<li><strong>Stack:</strong> " << project.stack << "</li>";
    html << "<li><strong>Status:</strong> " << project.status << "</li>";
    html << "</ul>";
    html << "<span class='project-link'>View project →</span>";
    html << "</article>";
  }
  html << "</div>";
  html << "<section class='section template-section'>";
  html << "<h2>Project Scaffold</h2>";
  html << "<p>Add projects by extending the <code>PROJECTS</code> array in <code>Source/WasmSite/main.cpp</code>:</p>";
  html << "<pre>Project{\"slug\", \"DisplayName\", \"Summary\", \"Stack\", \"Status\", \"Detailed description.\"};</pre>";
  html << "</section>";
  return html.str();
}

std::string build_project_detail(const Project& project) {
  std::ostringstream html;
  html << "<section class='section'>";
  html << "<p class='eyebrow'>Project</p>";
  html << "<h1>" << project.name << "</h1>";
  html << "<p>" << project.detail << "</p>";
  html << "<ul class='project-meta'>";
  html << "<li><strong>Stack:</strong> " << project.stack << "</li>";
  html << "<li><strong>Status:</strong> " << project.status << "</li>";
  html << "</ul>";
  html << "<div class='cta-row'>";
  html << "<a class='btn btn-secondary' href='#' data-page='projects'>Back to Projects</a>";
  html << "</div>";
  html << "</section>";
  return html.str();
}

extern "C" {

EMSCRIPTEN_KEEPALIVE void load_page(const char* key) {
  std::string request = (key && *key) ? key : "blog";
  std::string render_key = request;
  std::string html;

  if (request == "projects") {
    html = build_projects_page();
  } else if (request.rfind("project-", 0) == 0) {
    const char* slug = request.c_str() + 8;
    const Project* project = find_project(slug);
    if (project) {
      html = build_project_detail(*project);
    } else {
      html = build_projects_page();
      render_key = "projects";
    }
  } else {
    const StaticPage* page = find_static(request.c_str());
    if (page) {
      html = page->html;
      render_key = page->key;
    } else {
      const StaticPage* fallback = find_static("blog");
      html = fallback ? fallback->html : "";
      render_key = "blog";
    }
  }

  set_app_html(html.c_str());
  refresh_updates();
  announce_page_loaded(render_key.c_str());
}

}

int main() {
  return 0;
}
