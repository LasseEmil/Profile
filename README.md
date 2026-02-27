# Profile Page

Personal blog/portfolio website for GitHub Pages.

## Current Structure

This iteration now includes separate pages stored under `Source/Website/`:
- `Source/Website/index.html` (home)
- `Source/Website/about.html`
- `Source/Website/research.html`
- `Source/Website/work.html`
- `Source/Website/blog.html`
- `Source/Website/contact.html`

All pages share a common top navigation and stylesheet (`Source/Website/styles.css`).


## Deployment

GitHub Pages is deployed via `.github/workflows/pages.yml`, which copies `Source/Website/` into the Pages artifact and ships it with a `.nojekyll` file so the static assets publish without the Jekyll pipeline.



## Todo

update here
