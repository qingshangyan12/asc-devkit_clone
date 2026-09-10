---
name: asc-api-okf-read
description: Read one known Ascend DevKit API with bounded progressive disclosure from OKF/YAML, one Markdown section, and exact declared evidence. Use when a user asks what a named API does, its signatures, supported architectures or dtypes, include headers, parameters, constraints, return values, or example code. Do not use for similar, alternative, related, prerequisite, companion, or reverse-impact API queries; use asc-api-similar-search instead.
---

# Ascend API OKF Reader

Answer direct questions about one known API with the smallest sufficient disclosure.
Use `scripts/progressive_api_read.py` for every normal read. Treat OKF as an
evidence-backed locator, not as a replacement for source documentation.

## Workflow

1. Derive the DevKit root from this Skill's `.agents/skills/asc-api-okf-read/`
   location. Run the reader with `python3`, placing global options before the command:

   ```bash
   python3 <skill-dir>/scripts/progressive_api_read.py --repo-root <devkit-root> locate --api asc_name
   ```

2. Use a provided repository-relative document path, or run `locate`. Require one
   exact candidate; never select between duplicate names or substitute a similar name.
3. Read YAML first with `metadata --document <path>`. Use repeatable `--field` to
   narrow a known request. Answer from these fields when present:
   - `description` for the API purpose.
   - `signature` for function prototypes, architectures, headers and dtypes.
   - `examples` for the example project, source file and snippet id.
   - `usage_dependencies` only when the user explicitly asks about prerequisites.
4. Escalate automatically to one Markdown section only when YAML cannot answer the
   original question. Use `sections --document <path>` only when the exact heading is
   unknown, then `section --document <path> --heading <exact-heading>`.
5. Escalate to `evidence` only for exact declaration or example code. The command
   accepts one-based OKF indexes and reads only YAML-declared targets:

   ```bash
   python3 <reader> --repo-root <root> evidence --document <path> --kind header --signature-index 1 --header-index 1 [--dtype float]
   python3 <reader> --repo-root <root> evidence --document <path> --kind example --example-index 1 --signature-index 1 --snippet-index 1
   ```

6. State the repository-relative document path in the answer. Distinguish a
   documented fact from unavailable information; never infer architecture, dtype,
   header or API behavior from a name or directory.

Stop as soon as the disclosed layer answers the question. When a result has
`truncated: true`, request the same command and region with `--cursor <next_cursor>`
only if the omitted portion is necessary. Never raise `--max-chars` above 12000 or
`--max-lines` above 200; lower limits are allowed.

## Fallback

If YAML is absent or malformed, continue with the smallest relevant Markdown
section. If the controlled reader cannot expose a necessary fact because document
structure or exact evidence markers are missing, a direct file read is an exceptional
fallback. Limit it to the smallest useful range and disclose the reason and actual
read scope in the final answer. Do not emit a disclosure receipt during normal use.

Do not use `cat`, `sed`, or another whole-file read for normal API inspection. Do
not report that an API is unavailable merely because it has not been annotated.

## Boundaries

For related APIs, alternatives, call-before requirements, companions, or a
configuration's affected APIs, use `asc-api-similar-search`. When a direct API
question also asks one of these relationship questions, use both Skills and keep
the two answer types separate.
