---
name: asc-api-similar-search
description: Find similar, related, alternative, same-category, prerequisite, or companion AscendC APIs from generated indexes. Use when a user asks which APIs are similar to a known API, asks for alternatives, prerequisites, setup calls, or APIs in the same functional category.
---

# AscendC API Search

Use generated indexes before searching the API documentation broadly. Dependency
relationships are reviewed facts in API OKF fields and `api-dependency-index.yaml`;
never infer them from API names, parameter names, or prose while answering.

## Similar APIs

1. Locate the current API document and read its evidence-backed `tag` YAML field.
2. Read `<devkit-root>/api-tag-index.yaml`; derive the DevKit root from this skill's
   `.agents/skills/asc-api-similar-search/` location.
3. Select the tag namespace and values relevant to the request, then find APIs with
   shared tag values. Exclude the current API. Do not treat an unselected namespace as
   an implied match.
4. Open only the candidate documents needed for the requested comparison. Include each
   API name and repository-relative path.

## Dependency Lookup

1. For “what is needed before calling this API?” questions, read that execution API's
   `usage_dependencies` field before `signature`. This is the sole forward source.
2. Read `status` first. For `unconfirmed`, say no confirmed cross-API prerequisite
   was found. For `not_applicable`, say the document does not describe an executable API.
3. Present `required_before`, `conditional_before`, and `companion` separately.
   Preserve `all_of` versus `any_of`; every path in `all_of` is required.
4. For each conditional entry, copy its `when` text exactly. Do not evaluate, rewrite,
   or broaden the condition. A companion is not a call-before requirement.
5. Present `example_before` separately as an observed example sequence, never as a
   required dependency.
6. For “what does this configuration affect?” questions only, read
   `<devkit-root>/api-dependency-index.yaml` and look up the exact normalized
   configuration API path under `reverse`.
7. Read `references/cube-datamove.md` for cube data movement requests and
   `references/reg-broadcast.md` for register broadcast requests.

## Fallbacks

- If the current API has no `tag`, or `api-tag-index.yaml` is absent, use ordinary
  documentation search. Do not infer tag relationships from names alone.
- If an execution API has no `usage_dependencies`, say that dependency review has not
  been completed. If the reverse index is absent or has no exact-path record,
  say that no reviewed reverse relationship is available. Do not promote a prose
  mention or a “配合使用” reference into a prerequisite.
- Treat indexes as locators. Read candidate documents before asserting parameter,
  architecture, or data-type compatibility.
