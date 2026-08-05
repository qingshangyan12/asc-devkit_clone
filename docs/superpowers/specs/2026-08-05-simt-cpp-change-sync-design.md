# SIMT C-to-C++ Change Sync Audit

## Goal

Audit the current `upstream/master` history and find changes made to an existing
SIMT C API whose corresponding existing SIMT C++ API did not receive the same
change. Fix only those missed synchronizations. Do not add C++ counterparts for
C-only APIs.

## Baseline and Scope

- Base branch: `cann/asc-devkit:master` at the branch creation point.
- Audit window: from the initial SIMT API import (`b8ffed8b4`) through `HEAD`.
- C API paths: non-`cpp` files under `include/simt_api` and `impl/simt_api`.
- C++ API paths: files under `include/simt_api/cpp` and `impl/simt_api/cpp`.
- Relevant changes include signatures, supported types, architecture guards,
  compiler built-ins, implementation semantics, constants, and boundary fixes.
- Exclude formatting, copyright, documentation-only changes, file moves that do
  not alter behavior, C-only facilities, and new C APIs with no existing C++
  counterpart.

## Audit Method

1. Enumerate behavior-affecting commits in the audit window that modify C API
   paths.
2. For each changed C symbol, map it to an existing C++ symbol by operation,
   signature, implementation, and tests rather than by name alone.
3. Inspect the same commit and later history for an equivalent C++ change.
4. Classify each candidate as synchronized, intentionally C-only, not
   applicable, or missed.
5. Count a "modification item" as one logical C-side behavior change applied to
   one corresponding C++ API family. Multiple mechanical edits implementing the
   same change count once; independent behavior changes in one commit count
   separately.
6. Record every missed item with the originating commit, C and C++ symbols,
   affected files, evidence, and proposed correction.

## Implementation and Verification

- Add a focused compile or unit regression test for each missed behavior and
  confirm it fails before changing production headers.
- Apply the minimal equivalent change to the existing C++ API.
- Run each focused test after the fix, then run the repository's relevant SIMT
  test targets and formatting/static checks available in the environment.
- Include the audited totals and missed-item table in the pull request body.

## Delivery

Commit the fixes on `cpp_sync`, push the branch to the personal fork (`origin`),
and open a pull request against `cann/asc-devkit:master` (`upstream`).
