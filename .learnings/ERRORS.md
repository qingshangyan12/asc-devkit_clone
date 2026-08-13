## [ERR-20260810-007] invalid-search-path

**Logged**: 2026-08-10T19:05:00+08:00
**Priority**: low
**Status**: resolved
**Area**: tooling

### Summary
A read-only ripgrep command included a nonexistent candidate directory.

### Error
```
rg: build/_deps/cann-cmake-src/config: No such file or directory (os error 2)
```

### Context
- Searched several possible cann-cmake locations without validating each directory first.
- The required definition was found in the valid `function/prepare.cmake` path.

### Suggested Fix
Use `rg --files` or validate optional directories before passing them to ripgrep.

### Metadata
- Reproducible: yes
- Related Files: none

### Resolution
- **Resolved**: 2026-08-10T19:05:00+08:00
- **Notes**: Used the valid search results and confirmed the configured cache value separately.

---

## [ERR-20260810-006] shell-quoting

**Logged**: 2026-08-10T18:55:00+08:00
**Priority**: low
**Status**: resolved
**Area**: tooling

### Summary
Parallel verification commands did not quote shell metacharacters in ripgrep arguments.

### Error
```
zsh:1: no matches found: !build/**
zsh:1: command not found: ASC_BOOST_FILESYSTEM_SOURCES
```

### Context
- Passed a glob exclusion and an alternation expression through zsh without single-quoting them.
- The verification was rerun with quoted arguments and completed successfully.

### Suggested Fix
Single-quote ripgrep patterns and negated glob arguments when commands are evaluated by a shell.

### Metadata
- Reproducible: yes
- Related Files: none

### Resolution
- **Resolved**: 2026-08-10T18:55:00+08:00
- **Notes**: Reran both checks with shell-safe quoting; the old target name is absent and the new references are correct.

---

## [ERR-20260810-001] apply_patch

**Logged**: 2026-08-10T17:40:09+08:00
**Priority**: low
**Status**: resolved
**Area**: backend

### Summary
An edit failed because the patch expected an outdated log message in the target function.

### Error
```
apply_patch verification failed: Failed to find expected lines in bundle_source_generator.cpp
```

### Context
- Attempted to replace `FileUtils::OpenExclusiveOutput` without first rereading the exact latest function body.
- The behavior matched, but the existing error text differed from the patch context.

### Suggested Fix
Read the immediate target block before applying patches in files with existing user changes.

### Metadata
- Reproducible: no
- Related Files: tools/build/asc_compile_exporter/bundle_source_generator.cpp

### Resolution
- **Resolved**: 2026-08-10T17:40:09+08:00
- **Notes**: Reread the current function and regenerated the patch against exact context.

---

## [ERR-20260810-005] cmake-build

**Logged**: 2026-08-10T18:51:20+08:00
**Priority**: low
**Status**: resolved
**Area**: config

### Summary
Requested a newly added CMake target before regenerating the existing Makefile.

### Error
```
gmake: *** No rule to make target 'asc_boost_filesystem'. Stop.
```

### Context
- Moved and renamed a CMake target while reusing an already configured build directory.
- `cmake --build` could not dispatch an unknown target before CMake regenerated the build system.

### Suggested Fix
Run `cmake -S . -B build` after adding or renaming a target, then request the new target from the generated build system.

### Metadata
- Reproducible: yes
- Related Files: tools/CMakeLists.txt, tools/build/asc_compile_exporter/CMakeLists.txt

### Resolution
- **Resolved**: 2026-08-10T18:51:20+08:00
- **Notes**: Reconfigure the existing build directory before compiling the renamed target.

---

## [ERR-20260810-004] unit-test-expectation

**Logged**: 2026-08-10T17:50:26+08:00
**Priority**: low
**Status**: resolved
**Area**: tests

### Summary
A legacy removal test expected failure for a path that Boost.Filesystem treats as an absent target.

### Error
```
FileUtils::RemoveAll("/dev/null/child") returned true instead of false.
```

### Context
- The old recursive implementation failed while traversing the non-directory parent.
- Boost `remove_all` defines removal of a nonexistent target as a successful no-op.
- No object was removed or modified.

### Suggested Fix
Test idempotent removal behavior rather than the old traversal implementation detail.

### Metadata
- Reproducible: yes
- Related Files: tests/tools/utils/asc_compile_exporter/test_file_utils.cpp
- See Also: ERR-20260810-001

### Resolution
- **Resolved**: 2026-08-10T17:50:26+08:00
- **Notes**: Updated the test to accept successful no-op removal.

---

## [ERR-20260810-003] unit-test-runtime

**Logged**: 2026-08-10T17:49:12+08:00
**Priority**: high
**Status**: resolved
**Area**: config

### Summary
The manually built Boost.Filesystem static library used a different libstdc++ ABI from its consumers.

### Error
```
The unit test process exited with code 139 in the first filesystem-backed test.
```

### Context
- Production and unit-test targets define `_GLIBCXX_USE_CXX11_ABI=0`.
- `asc_compile_exporter_boost_filesystem` initially used the compiler default ABI.
- Boost.Filesystem path objects crossed the static-library boundary and caused a runtime layout mismatch.

### Suggested Fix
Every locally built C++ third-party static library must use the same libstdc++ ABI definition as its consumers.

### Metadata
- Reproducible: yes
- Related Files: tools/build/asc_compile_exporter/CMakeLists.txt
- See Also: ERR-20260810-002

### Resolution
- **Resolved**: 2026-08-10T17:49:12+08:00
- **Notes**: Added `_GLIBCXX_USE_CXX11_ABI=0` to the Boost.Filesystem target.

---

## [ERR-20260810-002] cmake-build

**Logged**: 2026-08-10T17:47:08+08:00
**Priority**: medium
**Status**: resolved
**Area**: config

### Summary
Loading the guarded Boost third-party script from a child directory hid `BOOST_SRC_PATH` from later test configuration.

### Error
```
/usr/bin/ld: cannot find -lmockcpp_static
```

### Context
- `add_cann_third_party(boost)` was initially added inside `tools/build/asc_compile_exporter`.
- The Boost script has a global include guard, while its path variables are normal directory-scope variables.
- The later test configuration skipped the guarded script and could not resolve its mockcpp imported target.

### Suggested Fix
Initialize guarded third-party recipes from the top-level dependency scope when multiple child directories consume their variables.

### Metadata
- Reproducible: yes
- Related Files: cmake/dependencies.cmake, tools/build/asc_compile_exporter/CMakeLists.txt
- See Also: ERR-20260810-001

### Resolution
- **Resolved**: 2026-08-10T17:47:08+08:00
- **Notes**: Moved `add_cann_third_party(boost)` to `cmake/dependencies.cmake`.

---
