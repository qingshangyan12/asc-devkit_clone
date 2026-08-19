# Manifest Schema Version Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make `asc_compile_exporter` inject `"schema_version":"1.0"` into bundled manifest JSON while rejecting source manifests that already declare the field.

**Architecture:** Keep the rule at the `CollectedManifestRepository` input boundary. Parse each source manifest once, reject caller-supplied `schema_version`, run existing validation, inject the exporter-owned version, and serialize the modified object back into `ManifestUnit::json` before resource collection continues.

**Tech Stack:** C++11, nlohmann/json, GoogleTest, CMake

---

### Task 1: Add Schema Version Regression Tests

**Files:**
- Modify: `tests/tools/utils/asc_compile_exporter/test_support.h:117`
- Modify: `tests/tools/utils/asc_compile_exporter/test_asc_compile_exporter.cpp:210`
- Modify: `tests/tools/utils/asc_compile_exporter/test_collected_manifest_repository.cpp:17`

- [x] **Step 1: Remove the exporter-owned field from test input fixtures**

In both `ModuleTest::CreateManifest` in `test_support.h` and `AscCompileExporterTest::CreateManifest` in
`test_asc_compile_exporter.cpp`, change the source JSON prefix from:

```cpp
"{\"schema_version\":\"1.0\",\"resource_id\":\"" + name +
```

to:

```cpp
"{\"resource_id\":\"" + name +
```

This makes all existing tests exercise valid caller input under the new contract.

- [x] **Step 2: Add a failing injection-and-text-synchronization test**

Add the JSON header and alias to `test_collected_manifest_repository.cpp`:

```cpp
#include "nlohmann/json.hpp"

using Json = nlohmann::json;
```

Then add this test after `RepositorySortsManifestsAndResourcesAndSkipsEmptyFiles`:

```cpp
TEST_F(ModuleTest, RepositoryAddsSchemaVersionToManifestJson)
{
    const std::string unit = FileUtils::JoinPath(root_, "collection-schema-version/Unit");
    WriteTestFile(FileUtils::JoinPath(unit, "resources/kernel.cpp"), "source");
    CreateManifest(unit, "Unit");

    std::vector<ManifestUnit> units;
    ASSERT_TRUE(CollectedManifestRepository(FileUtils::JoinPath(root_, "collection-schema-version")).Load(units));

    ASSERT_EQ(units.size(), 1U);
    const Json manifest = Json::parse(units[0].json);
    ASSERT_TRUE(manifest.contains("schema_version"));
    EXPECT_EQ(manifest.at("schema_version"), "1.0");
}
```

Parsing `units[0].json` proves that the text carried forward to bundle generation was updated, rather than only a temporary JSON object.

- [x] **Step 3: Build and run the injection test to verify RED**

Run:

```bash
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  cmake --build build --target ascendc_ut_asc_compile_exporter -j4
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  build/tests/tools/utils/ascendc_ut_asc_compile_exporter \
  --gtest_filter='ModuleTest.RepositoryAddsSchemaVersionToManifestJson'
```

Expected: the target builds, then the test fails because `manifest.contains("schema_version")` is false.

- [x] **Step 4: Add a failing caller-supplied-field rejection test**

Add this test immediately after the injection test:

```cpp
TEST_F(ModuleTest, RepositoryRejectsManifestWithSchemaVersion)
{
    const std::string collection = FileUtils::JoinPath(root_, "collection-existing-schema-version");
    const std::string unit = FileUtils::JoinPath(collection, "Unit");
    WriteTestFile(FileUtils::JoinPath(unit, "resources/kernel.cpp"), "source");
    CreateManifest(unit, "Unit");
    const std::string manifestPath = FileUtils::JoinPath(unit, "Unit_manifest.json");
    Json manifest = Json::parse(ReadTestFile(manifestPath));
    manifest["schema_version"] = "2.0";
    WriteTestFile(manifestPath, manifest.dump());

    std::vector<ManifestUnit> units;
    EXPECT_FALSE(CollectedManifestRepository(collection).Load(units));
}
```

- [x] **Step 5: Build and run the rejection test to verify RED**

Run:

```bash
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  cmake --build build --target ascendc_ut_asc_compile_exporter -j4
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  build/tests/tools/utils/ascendc_ut_asc_compile_exporter \
  --gtest_filter='ModuleTest.RepositoryRejectsManifestWithSchemaVersion'
```

Expected: the test fails because `CollectedManifestRepository::Load` currently accepts the manifest and returns `true`.

### Task 2: Inject and Serialize Schema Version

**Files:**
- Modify: `tools/build/asc_compile_exporter/collected_manifest_repository.cpp:72`
- Test: `tests/tools/utils/asc_compile_exporter/test_collected_manifest_repository.cpp`

- [x] **Step 1: Make parsed manifest processing update the source text**

Change `ValidateAndExtractResourcePath` to accept mutable text, reject an existing version before general validation, inject the default after validation, and serialize it back:

```cpp
bool ValidateAndExtractResourcePath(std::string& text, const std::string& manifestPath, std::string& resourcePath)
{
    Json manifest = Json::parse(text, nullptr, false);
    if (manifest.is_discarded()) {
        ASCENDLOGE("Manifest is not valid JSON: %s", manifestPath.c_str());
        return false;
    }
    if (manifest.contains("schema_version")) {
        ASCENDLOGE("Manifest must not contain schema_version: %s", manifestPath.c_str());
        return false;
    }

    if (!ValidateResourceManifest(manifest, manifestPath)) {
        return false;
    }

    const Json::const_iterator resourcePathValue = manifest.find("resource_path");
    if (resourcePathValue == manifest.end() || !resourcePathValue->is_string()) {
        ASCENDLOGE("Manifest resource_path is unavailable: %s", manifestPath.c_str());
        return false;
    }
    resourcePath = resourcePathValue->get<std::string>();
    manifest["schema_version"] = "1.0";
    text = manifest.dump();
    return true;
}
```

No `LoadUnit` call-site change is required because `unit.json` is already a mutable `std::string`.

- [x] **Step 2: Build and run both focused tests to verify GREEN**

Run:

```bash
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  cmake --build build --target ascendc_ut_asc_compile_exporter -j4
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  build/tests/tools/utils/ascendc_ut_asc_compile_exporter \
  --gtest_filter='ModuleTest.RepositoryAddsSchemaVersionToManifestJson:ModuleTest.RepositoryRejectsManifestWithSchemaVersion'
```

Expected: both tests pass.

- [x] **Step 3: Run all repository tests**

Run:

```bash
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  build/tests/tools/utils/ascendc_ut_asc_compile_exporter \
  --gtest_filter='ModuleTest.Repository*'
```

Expected: all `ModuleTest.Repository*` tests pass.

### Task 3: Verify the Exporter Suite and Formatting

**Files:**
- Verify: `tools/build/asc_compile_exporter/collected_manifest_repository.cpp`
- Verify: `tests/tools/utils/asc_compile_exporter/test_collected_manifest_repository.cpp`
- Verify: `tests/tools/utils/asc_compile_exporter/test_support.h`
- Verify: `tests/tools/utils/asc_compile_exporter/test_asc_compile_exporter.cpp`

- [x] **Step 1: Format modified C++ files**

Run:

```bash
pre-commit run clang-format --files \
  tools/build/asc_compile_exporter/collected_manifest_repository.cpp \
  tests/tools/utils/asc_compile_exporter/test_collected_manifest_repository.cpp \
  tests/tools/utils/asc_compile_exporter/test_support.h \
  tests/tools/utils/asc_compile_exporter/test_asc_compile_exporter.cpp
```

Expected: command exits with status 0.

- [x] **Step 2: Rebuild and run the complete exporter unit-test executable**

Run:

```bash
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  cmake --build build --target ascendc_ut_asc_compile_exporter -j4
ASCEND_HOME_PATH=/usr/local/Ascend/cann-9.2.0 \
  build/tests/tools/utils/ascendc_ut_asc_compile_exporter
```

Expected: build succeeds and all tests in the executable pass.

- [x] **Step 3: Check the final diff**

Run:

```bash
git diff --check
git diff --stat
git status --short
```

Expected: `git diff --check` emits no output; the diff is limited to the repository implementation, related tests, fixtures, and this plan.

- [x] **Step 4: Commit the implementation**

Run:

```bash
git add \
  tools/build/asc_compile_exporter/collected_manifest_repository.cpp \
  tests/tools/utils/asc_compile_exporter/test_collected_manifest_repository.cpp \
  tests/tools/utils/asc_compile_exporter/test_support.h \
  tests/tools/utils/asc_compile_exporter/test_asc_compile_exporter.cpp \
  docs/superpowers/plans/2026-08-19-manifest-schema-version.md
git commit -m "feat: inject manifest schema version"
```

Expected: pre-commit checks pass and the implementation commit is created.
