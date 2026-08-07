# ASCEND_SOC_SERIES Build Configuration Design

## Goal

Add `ASCEND_SOC_SERIES` as a public build configuration for selecting one or
more short SoC series. The new configuration accepts the platform's usual
mixed-case spelling, such as `Ascend910B` and `Ascend950`, without changing the
existing compilation, packaging, installation, or runtime lookup paths.

`ASCEND_SOC_SERIES` selects a series, not a full SKU. For example,
`Ascend950` is valid, while `Ascend950PR_9599` is invalid user input. Full SKUs
remain an internal compiler detail selected by the existing `SOC_MAP_EXT`
mapping before invoking OPC.

## Current Behavior

`cmake/asc/fwk_modules/config.cmake` defines `ASCEND_COMPUTE_UNIT`, defaults it
to `ascend910b`, and validates every configured value against the lowercase
`ASC_VALID_SOC_LIST`.

After validation, the same lowercase series values flow through the existing
build pipeline:

- opbuild receives them through `OPS_PRODUCT_NAME` and `--compute_unit`;
- kernel targets and intermediate directories include the series value;
- binary and configuration artifacts are written below `binary/<series>`;
- OPP installation uses `kernel/<series>` and `config/<series>`;
- opregistry packaging records the same lowercase series path key;
- kernel compilation maps the short series through `SOC_MAP_EXT` and passes the
  resulting full SKU to OPC as `--soc_version`.

Because the current value is also a path component and runtime lookup key,
mixed-case input must not propagate beyond the build configuration entry.

## Configuration Contract

### Accepted input

`ASCEND_SOC_SERIES` accepts a CMake list of short SoC series. Matching is
case-insensitive because the complete value is converted to lowercase before
validation. Lowercasing preserves CMake list separators.

Examples:

| Input | Canonical value | Result |
| --- | --- | --- |
| `Ascend950` | `ascend950` | accepted |
| `ASCEND910B` | `ascend910b` | accepted |
| `Ascend910B;Ascend950` | `ascend910b;ascend950` | accepted |
| `Ascend950PR_9599` | `ascend950pr_9599` | rejected |
| `Ascend910B1` | `ascend910b1` | rejected |
| unknown series | lowercase input | rejected |

The implementation must not add a mixed-case legality table, a full-SKU
legality table, or reverse full-SKU-to-series mapping. The existing
`ASC_VALID_SOC_LIST` remains the single source of truth for accepted series.

### Compatibility and conflicts

- If both `ASCEND_SOC_SERIES` and `ASCEND_COMPUTE_UNIT` are defined, CMake
  configuration fails immediately, regardless of whether their values match.
- If only `ASCEND_SOC_SERIES` is defined, its lowercase value becomes the
  in-scope `ASCEND_COMPUTE_UNIT` used by the existing pipeline.
- If only `ASCEND_COMPUTE_UNIT` is defined, its current behavior is unchanged;
  in particular, it is not made case-insensitive by this feature.
- If neither is defined, the existing `ascend910b` default remains unchanged.
- Switching an existing CMake build directory between configuration surfaces
  may leave both cache variables defined. This correctly triggers the conflict
  rule; callers must unset the old cache entry or use a fresh build directory.

## Design

The compatibility adapter belongs in
`cmake/asc/fwk_modules/config.cmake`, immediately before the current defaulting
and whitelist validation logic:

1. Detect simultaneous definition of the new and old configuration variables
   and report a fatal configuration error.
2. When `ASCEND_SOC_SERIES` is present, convert its value with CMake's
   `string(TOLOWER)` and assign the result to the non-cache
   `ASCEND_COMPUTE_UNIT` variable.
3. Otherwise retain the existing `ASCEND_COMPUTE_UNIT` value or default.
4. Validate the canonical `ASCEND_COMPUTE_UNIT` list against the unchanged
   `ASC_VALID_SOC_LIST`.
5. For invalid input through the new surface, report that only SoC series are
   accepted and full SKUs are not valid. The message also prints the supported
   series list. Invalid old configuration retains its existing error behavior.

No downstream interface or consumer is renamed. In particular, opbuild keeps
`--compute_unit`, Python build utilities keep `--compute-unit`, and packaging
continues to consume `ASCEND_COMPUTE_UNIT`.

## Data Flow

```text
ASCEND_SOC_SERIES=Ascend950
        |
        | string(TOLOWER)
        v
ASCEND_COMPUTE_UNIT=ascend950
        |
        | ASC_VALID_SOC_LIST validation
        v
existing build and packaging pipeline
        |                            |
        | path and runtime key       | SOC_MAP_EXT
        v                            v
.../<ascend950>/...          Ascend950PR_9599 -> OPC
```

This boundary guarantees that full SKU selection does not influence directory
names or runtime keys.

## Error Handling

CMake configuration fails before targets are generated when:

- both configuration surfaces are defined;
- the normalized new value is not in `ASC_VALID_SOC_LIST`;
- any member of a configured list is invalid.

An empty explicitly supplied value follows the existing CMake list behavior.
This feature does not introduce separate empty-value semantics.

## Verification

Focused CMake configuration tests will cover:

1. `ASCEND_SOC_SERIES=Ascend950` produces canonical `ascend950`.
2. Arbitrary casing of a valid short series is accepted.
3. Multiple mixed-case series are normalized and preserved as a list.
4. `Ascend950PR_9599` and `Ascend910B1` are rejected as full SKUs.
5. An unknown series is rejected by `ASC_VALID_SOC_LIST`.
6. Defining both new and old variables always fails, including equal values.
7. Old-only configuration retains its existing behavior.
8. Neither variable preserves the `ascend910b` default.
9. A representative build exposes only lowercase series values to downstream
   target names, intermediate binary directories, installed OPP paths, and
   opregistry path keys.
10. OPC command generation still obtains the full SKU from `SOC_MAP_EXT`.

## Out of Scope

- Adding or changing entries in `ASC_VALID_SOC_LIST` or `SOC_MAP_EXT`.
- Accepting full SKU values from users.
- Adding `soc_series` arguments to opbuild or Python build utilities.
- Renaming existing `compute_unit` APIs, variables, files, or metadata.
- Changing build directory layout, OPP installation layout, or runtime lookup.
