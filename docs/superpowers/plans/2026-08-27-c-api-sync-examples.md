# C API Sync Executable Examples Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add minimal, self-contained, directly executable examples to all 11 C API sync interface pages that currently lack them and verify every example on DAV 3510 NPU.

**Architecture:** Keep each example inline in its own API page and follow the existing `asc_sync_inter_arrive.md` Host/Kernel/ACL pattern. Reuse only already documented sync pairs and use compact float or half data paths with deterministic Host-side result checks.

**Tech Stack:** AscendC C API, Bisheng, ACL runtime, DAV 3510 NPU, Markdown, shell validation.

---

### Task 1: Establish the failing completeness baseline

**Files:**
- Test: `docs/zh/api/SIMD-API/c_api/sync/*.md`

- [ ] Check every interface page for `## 调用示例`, `bisheng example.asc`, `int main(`, and a `cpp` code block.
- [ ] Require the check to fail and report exactly the 11 known incomplete pages.
- [ ] Record `/usr/local/Ascend/cann`, `dav-3510`, and `/dev/davinci0` as the execution environment.

### Task 2: Replace the lock and unlock examples

**Files:**
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_lock.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_unlock.md`

- [ ] Replace the entire complex `asc_lock` example with one complete vector-add program.
- [ ] Use matching `asc_lock` and `asc_unlock` calls around MTE2, V, and MTE3 stages with one `mutex_id`.
- [ ] Add the same minimal executable scenario to `asc_unlock.md`, with comments and result text focused on `asc_unlock`.
- [ ] Compile and run both extracted examples on DAV 3510 NPU.

### Task 3: Complete the wait-side pair examples

**Files:**
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_block_wait.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_inter_wait.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_intra_wait.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_subblock_wait.md`

- [ ] Copy the complete program structure from each paired `arrive` page into its `wait` page.
- [ ] Keep the paired arguments identical and change the narrative, comments, and result text to focus on the `wait` interface.
- [ ] Compile and run all 4 extracted examples on DAV 3510 NPU.

### Task 4: Complete the scalar barrier and legacy sync examples

**Files:**
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_data_barrier.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_mte2.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_mte3.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_vec.md`
- Modify: `docs/zh/api/SIMD-API/c_api/sync/asc_sync_wait.md`

- [ ] Add a complete scalar store/barrier program for `asc_sync_data_barrier` using a literal `mem_dsb_t` enumerator.
- [ ] Replace the 4 fragment examples with complete float vector-add programs that preserve each documented synchronization pattern.
- [ ] Compile and run all 5 extracted examples on DAV 3510 NPU.

### Task 5: Verify the entire sync interface set

**Files:**
- Test: `docs/zh/api/SIMD-API/c_api/sync/*.md`

- [ ] Re-run completeness checks and require all 18 API pages to contain a direct executable example.
- [ ] Extract, compile, and run all 18 examples on DAV 3510 NPU.
- [ ] Scan changed prose and comments for Chinese-English or Chinese-number spaces.
- [ ] Check Markdown code fences, heading order, terminology, and deprecated interface mappings.
- [ ] Inspect `git diff --check`, `git diff`, and `git status --short` before reporting completion.
