# Tiling Types Softlink Design

## Problem

Tiling headers installed under `asc/include/tiling/<api>` include
`../utils/types.h`. The installation layout does not currently provide
`asc/include/tiling/utils/types.h`, so that relative include cannot be resolved
through the compatibility tiling path.

## Design

Extend `impl/adv_api/cmake/tiling_headers.cmake` to:

1. Create `asc/include/tiling/utils` during installation.
2. Create the symbolic link `asc/include/tiling/utils/types.h` with the relative
   target `../../adv_api/utils/types.h`.

Use a file-level link instead of linking the complete `adv_api/utils` directory,
so the compatibility path exposes only the header required by existing tiling
headers.

## Verification

Run the configured install script in a temporary installation root and verify:

- `asc/include/tiling/utils/types.h` is a symbolic link.
- Its stored target is `../../adv_api/utils/types.h`.
- The link resolves to an existing file.
- A path equivalent to `asc/include/tiling/math/../utils/types.h` is readable.
