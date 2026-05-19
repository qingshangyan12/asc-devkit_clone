# Experimental Feature Development Standards

## What is an Experimental Feature

Experimental features are features that are not yet stable and whose APIs may change. By releasing in experimental form, it is possible to:

- Open new capabilities to the community early for trial and feedback
- Validate API design rationality in real usage scenarios
- Make breaking changes to the API when necessary without affecting the compatibility commitment of stable APIs

---

## Directory Location

### Primary Location: tensor_api

`tensor_api` is the primary experimental area of this project:

```cpp
include/tensor_api/         # Experimental API declarations
impl/tensor_api/             # Experimental API implementations
tests/api/tensor_api/        # Experimental API tests
```

Marked as `experimental` in the `README.md` directory description.

### External Reference Path

External references to experimental features use the `tensor_api/` prefix:

```cpp
#include "include/tensor_api/tensor.h"
```

### In-Code Experimental Annotations

Some implementations in non-experimental directories such as `adv_api` are also marked as experimental (e.g., matmul scheduler), declaring their status through code comments.

---

## Code Annotation Standards

### Header File Annotation (Recommended Format)

Clearly annotate in the header file top comment:

```cpp
/*!
 * \file tensor.h
 * \brief Tensor API (Experimental)
 *
 * \note This API is experimental and may change in future releases.
 */
```

Or use line comments:

```cpp
// NOTE: This API is experimental. Interface may change without notice.
```

### Impl File Annotation (matmul scheduler mode)

```cpp
// MatmulScheduler is considered entirely experimental.
// The interface and behavior may change in future releases.
```

### README Directory Description (tensor_api mode)

The `tensor_api` directory in the project README directory tree is not currently annotated with `experimental`:

```text
├── tensor_api                      # Ascend C TENSOR API implementation source code
```

To annotate the experimental status, consider adding `（experimental）` to the comment.

### Documentation Annotation

Add `\note` to API documentation:

```cpp
/*!
 * \note This API is experimental. The interface may change without notice.
 * \note For stable alternatives, see ...
 */
```

---

## Quality Bar

Experimental does not mean low quality. Even experimental code must meet:

| Requirement | Description |
|------|------|
| Compiles successfully | Header files can be compiled independently |
| Functional test | Provide basic functional UT |
| Code review | Pass through normal Code Review process |
| Documentation | Have API documentation with experimental annotation |
| CMake integration | Correctly integrated into the build system |

---

## Graduation Process

### Graduation Conditions

Experimental features can apply for graduation to stable API when the following conditions are met:

1. Released as experimental in at least one official version
2. Community feedback confirms API design is reasonable, with no outstanding design issues
3. Complete functional and performance testing passes
4. Documentation is complete (remove experimental annotations, add complete parameter descriptions and constraints)
5. No known critical bugs

### Graduation Steps

```text
□ Submit an Issue proposing graduation, describe usage and stability evidence
□ Reach consensus through community discussion
□ Update code comments: remove experimental annotations
□ Update API documentation: remove experimental warnings
□ Update README.md: remove experimental directory annotation
□ Update CHANGELOG.md: record graduation
□ If include paths change, update all references
```

### When Not to Graduate

If an experimental feature is evaluated as not suitable for graduation, it may:

- **Merge into existing stable API:** Functionality implemented by extending existing API
- **Deprecate and remove:** Removed in subsequent versions, deprecation warning must be given one version in advance

---

## Breaking Change Strategy

### During Experimental Phase

- Non-backward-compatible changes allowed
- Breaking changes must be recorded in CHANGELOG.md
- Recommended to annotate in PR description with `**BREAKING:**` prefix

### After Graduation

- Breaking changes no longer allowed (follow stable API compatibility strategy)
- If changes are needed, follow the standard deprecation → addition → removal process

---

## Comparison of Two Experimental Modes

The project has two experimental forms:

| Dimension | tensor_api mode | matmul scheduler mode |
|------|----------------|---------------------|
| Location | Independent `tensor_api/` directory | Inside `adv_api/detail/matmul/scheduler/` |
| Annotation Method | README + directory description | Code comments |
| Independence | Complete independent API system | Internal experimental algorithm of existing API |
| Graduation Method | Independent API graduation | Merge into main API or remove |
| Expected Lifecycle | May become a long-term independent API | Merge or deprecate after experimental validation |

---

## New Experimental Feature Checklist

```text
□ Determine experimental mode: Independent API (tensor_api) or internal experiment (comment annotation)

□ Independent API Mode:
    □ include/tensor_api/<category>/<api>.h
    □ impl/tensor_api/<category>/<api>_impl.h
    □ tests/api/tensor_api/<category>/test_tensor_api_<api>.cpp
    □ Update README.md to annotate experimental
    □ Update relevant CMakeLists.txt

□ Internal Experiment Mode (including matmul scheduler type):
    □ Add "is considered entirely experimental" comment to corresponding impl file
    □ Annotate experimental in public header file name or comments
    □ Add corresponding tests
    □ State as experimental in commit message or PR description

□ Common Requirements for Both Modes:
    □ Annotate experimental in documentation
    □ Record in CHANGELOG (if externally visible)
    □ Pass Code Review
```
