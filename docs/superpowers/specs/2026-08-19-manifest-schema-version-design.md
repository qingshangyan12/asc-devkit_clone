# Manifest Schema Version Injection

## Goal

Ensure every manifest bundled by `asc_compile_exporter` contains a generated
top-level `schema_version` field with the string value `"1.0"`.

## Input Contract

Source manifest JSON must not contain `schema_version`. If it does, repository
loading fails and logs a manifest validation error. This prevents callers from
overriding or duplicating the exporter-owned schema version.

## Processing

`CollectedManifestRepository` parses the source text once. It rejects a parsed
object containing `schema_version`, then runs the existing resource manifest
validation. After validation succeeds, it adds `schema_version: "1.0"`, reads
`resource_path`, and serializes the modified JSON back into `ManifestUnit::json`.
The generated bundle therefore embeds the versioned JSON text rather than the
original source text.

The source manifest file on disk is not modified. JSON whitespace and key order
are not part of the output contract because serialization normalizes them.

## Failure Behavior

Malformed JSON, an existing `schema_version`, and all existing validation errors
continue to make `CollectedManifestRepository::Load` return `false`. Version
injection occurs only after validation succeeds.

## Tests

- Load a valid source manifest without `schema_version` and assert that the
  parsed `ManifestUnit::json` contains the string value `"1.0"`.
- Load an otherwise valid source manifest containing `schema_version` and assert
  that repository loading fails.
- Keep the existing repository and exporter tests passing after test fixtures
  stop supplying `schema_version` themselves.
