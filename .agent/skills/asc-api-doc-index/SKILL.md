---
name: asc-api-doc-index
description: Register newly added or renamed Ascend C API Markdown pages and figures in repository navigation, repair their related documentation links, and validate final documentation assets. Use when an API page or figure under docs/zh/api is missing from README.md, C API indexes, topic overviews, or consumer-page cross-links.
---

# Ascend C API Document Indexing

Update the smallest complete set of navigation, cross-reference pages, and figure assets for a new API document. Preserve unrelated worktree changes.

## Inspect

1. Resolve the target repository and API page. Confirm the page exists and read its H1 title.
2. Locate the owning navigation hierarchy, normally:
   - `docs/zh/api/README.md` for the global API navigation.
   - The API-family index, such as `docs/zh/api/SIMD-API/c_api/c_api.md`.
   - The API-topic overview, such as `docs/zh/api/SIMD-API/c_api/cube_datamove/cube_datamove.md`.
3. Search `docs/zh/api` for the API name and its predecessor or paired API. Identify missing links and stale targets before editing.
4. For behavior-sensitive links, read the public declaration under `include/` and the relevant implementation under `impl/`. Do not infer A/B register-bank semantics from names alone.

## Update

1. Add one global-navigation entry whose label exactly matches the target page H1 and whose relative path resolves from `docs/zh/api/README.md`.
2. Add the API to the owning family index table with a concise description and the public header path.
3. Add the API to the topic overview list. Follow the local ordering convention; for setting APIs, keep related setter names grouped.
4. Repair consumer-page references so their link label, target file, and described behavior agree. For paired 3D matrix configuration APIs, verify that the selected padding and feature-map register bank matches `f_matrix_ctrl`.
5. Do not duplicate an existing entry, retarget an unrelated legacy API, or modify example/kernel code solely to add navigation.

## Figures

When adding or changing an API figure:

1. Read `API-FIGURE-*` in `docs/zh/asc_api_writing_guidelines.md` and any user-provided drawing template before editing.
2. Use a white background. Prefer 840px, 520px, or 220px width. Use FZLanTingHeiS for Chinese text and Huawei Sans for English text; do not go below the required font sizes.
3. Reuse the template palette and line treatment. For byte-layout diagrams, use distinct colors for effective data and padding, preserve exact byte ranges and offsets, and omit connector lines or whitespace that do not represent a real layout boundary.
4. Deliver the final project asset as PNG under `docs/zh/api/figures/` and preserve its editable SVG source under `docs/zh/api/figures/svg/` with the same basename. Markdown pages must reference the relative `.png` path, not the source SVG.
5. Keep the PNG and SVG source semantically identical: dimensions, colors, labels, offsets, and layout boundaries must match.
6. Render or otherwise inspect the PNG before completion. Verify its file signature and dimensions in addition to its Markdown link.

## Verify

1. Confirm each added relative target exists from its source Markdown file.
2. Run:

   ```bash
   git diff --check
   MARKDOWN_LINK_CHECK_ONLINE=0 bash scripts/markdown_link_check.sh <changed-markdown-files>
   ```

3. If the checker cannot create its cache under `.git`, use a writable temporary copy of the Git metadata or a preinstalled `lychee` binary. Keep the repository itself unchanged by this workaround.
4. For new PNG assets, run `file <image>` and verify the expected dimensions before running the Markdown checker. Parse each paired SVG source as XML.
5. Report the updated navigation locations, any repaired cross-links, the link-check result, and checks that could not run.
