# Tiling SO Log Format Fix Design

## Scope

Fix the six `when load tiling so` log messages in the mirrored implementations:

- `tools/build/asc_op_compile_base/adapter/get_op_tiling.py`
- `tools/build/asc_op_compile_base/asc_op_compiler/get_op_tiling.py`

Other backslash-continued strings are outside this change.

## Implementation

Replace each backslash-continued f-string with implicitly concatenated f-strings
inside parentheses. The resulting message must contain exactly one space between
the exception text and `when load tiling so`, with no embedded newline.

For the runtime fallback message, separate the two complete candidate paths with
` or ` instead of `/`. Loading behavior and exception handling remain unchanged.

Apply equivalent changes to both mirrored implementations.

## Verification

Extend the existing `get_op_tiling` tests for both package layouts. Mock the load
failures and `LogUtil.print_compile_log`, then assert the exact message passed to
the logger for compile-time, runtime, and custom SO failures.

Run the focused test suites, Ruff formatting/checks for touched Python files, and
a final search confirming all six messages no longer use backslash continuation.
