/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "resource_manifest.h"

#include <functional>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

namespace ascendc {
namespace resource {
namespace {

using Json = nlohmann::json;

Json ValidManifest()
{
    return Json::parse(R"json({
      "schema_version": "1.0",
      "resource_id": "resource_a",
      "soc_version": "Ascend910B",
      "source_file": "kernel.cpp",
      "base_dir": "assets",
      "options": {"common_compile": ["-O2", "-I${resource}/assets/include"]},
      "kernels": [{
        "kernel_name": "kernel_a",
        "constants_info": [{
          "name": "CONST_A",
          "parameter_index": 0,
          "byte_size": 4,
          "file": "${resource}/assets/const.bin",
          "template": "uint32_t"
        }],
        "objects": [{
          "objects_name": "kernel.o",
          "objects_type": "basic",
          "commands": [
            {"type": "compile", "stage": 0,
             "options": ["${options:common_compile}", "${source_dir}", "-o", "${output:assets/kernel.o}"]},
            {"type": "ld", "stage": 1, "options": ["${output:assets/kernel.o}"]},
            {"type": "objcopy", "stage": 2, "options": ["--strip-debug"]}
          ],
          "output": ["${output:assets/kernel.o}"]
        }],
        "link_options": ["${resource}/assets/link.ld", "${env:ASCEND_HOME_PATH}"]
      }]
    })json");
}

bool Parse(const Json& json, ResourceManifest& manifest)
{
    return ParseResourceManifest(json.dump(), "unit-test", manifest);
}

void ExpectInvalid(const Json& json)
{
    ResourceManifest manifest;
    EXPECT_FALSE(Parse(json, manifest));
}

TEST(ResourceManifestTest, ParsesCompleteManifest)
{
    ResourceManifest manifest;
    ASSERT_TRUE(Parse(ValidManifest(), manifest));
    EXPECT_EQ(manifest.resourceId, "resource_a");
    EXPECT_EQ(manifest.socVersion, "Ascend910B");
    EXPECT_EQ(manifest.sourceFile, "kernel.cpp");
    EXPECT_EQ(manifest.baseDir, "assets");
    ASSERT_EQ(manifest.kernels.size(), 1U);
    const ManifestKernel& kernel = manifest.kernels.front();
    EXPECT_EQ(kernel.kernelName, "kernel_a");
    ASSERT_EQ(kernel.constants.size(), 1U);
    EXPECT_EQ(kernel.constants.front().file, "${resource}/assets/const.bin");
    ASSERT_EQ(kernel.objects.size(), 1U);
    ASSERT_EQ(kernel.objects.front().commands.front().options.size(), 5U);
    EXPECT_EQ(kernel.objects.front().commands.front().options[0], "-O2");
    EXPECT_EQ(kernel.linkOptions.size(), 2U);

    Json minimal = ValidManifest();
    minimal.erase("source_file");
    minimal.erase("base_dir");
    minimal.erase("options");
    minimal["kernels"][0].erase("constants_info");
    minimal["kernels"][0].erase("link_options");
    minimal["kernels"][0]["objects"][0]["commands"][0]["options"] = {"${output:kernel.o}"};
    minimal["kernels"][0]["objects"][0]["output"] = {"${output:kernel.o}"};
    ASSERT_TRUE(Parse(minimal, manifest));
    EXPECT_TRUE(manifest.baseDir.empty());
}

TEST(ResourceManifestTest, RejectsInvalidTopLevelAndBaseFields)
{
    ResourceManifest manifest;
    EXPECT_FALSE(ParseResourceManifest("{", "", manifest));
    EXPECT_FALSE(ParseResourceManifest("[]", "unit-test", manifest));

    const std::vector<std::function<void(Json&)>> mutations = {
        [](Json& value) { value.erase("schema_version"); },
        [](Json& value) { value["schema_version"] = 1; },
        [](Json& value) { value.erase("resource_id"); },
        [](Json& value) { value["resource_id"] = ""; },
        [](Json& value) { value.erase("soc_version"); },
        [](Json& value) { value["soc_version"] = ""; },
        [](Json& value) { value.erase("kernels"); },
        [](Json& value) { value["kernels"] = Json::array(); },
        [](Json& value) { value["source_file"] = 1; },
        [](Json& value) { value["source_file"] = "dir/kernel.cpp"; },
        [](Json& value) { value["source_file"] = "."; },
        [](Json& value) { value["base_dir"] = 1; },
        [](Json& value) { value["base_dir"] = "/assets"; },
        [](Json& value) { value["base_dir"] = "assets/../escape"; },
        [](Json& value) { value["options"] = 1; },
        [](Json& value) { value["options"] = Json::object(); },
        [](Json& value) { value["options"]["common_compile"] = 1; },
        [](Json& value) { value["options"]["common_compile"] = {1}; },
        [](Json& value) { value["options"]["common_compile"] = {"${unknown}"}; },
    };
    for (const auto& mutate : mutations) {
        Json value = ValidManifest();
        mutate(value);
        if (value["options"].is_object() && !value["options"].contains("common_compile")) {
            value["kernels"][0]["objects"][0]["commands"][0]["options"] = {"${output:assets/kernel.o}"};
            ResourceManifest parsed;
            EXPECT_TRUE(Parse(value, parsed));
        } else {
            ExpectInvalid(value);
        }
    }
}

TEST(ResourceManifestTest, ValidatesRelativePathsAndMarkers)
{
    EXPECT_TRUE(ValidateRelativeResourcePath("assets/file.bin"));
    EXPECT_FALSE(ValidateRelativeResourcePath(""));
    EXPECT_FALSE(ValidateRelativeResourcePath("/absolute"));
    EXPECT_FALSE(ValidateRelativeResourcePath("a\\b"));
    EXPECT_FALSE(ValidateRelativeResourcePath("a/./b"));
    EXPECT_FALSE(ValidateRelativeResourcePath("a/../b"));
    EXPECT_FALSE(ValidateRelativeResourcePath(std::string("a\0b", 3U)));

    const std::vector<std::string> invalidOptions = {
        "${unterminated",
        "${unknown}",
        "${resource}",
        "${resource}/assets/a${resource}/assets/b",
        "${resource}/outside/file",
        "${output:../escape}",
        std::string("option\0tail", 11U),
    };
    for (const std::string& option : invalidOptions) {
        Json value = ValidManifest();
        value["options"]["common_compile"] = {option};
        ExpectInvalid(value);
    }

    Json emptyCommon = ValidManifest();
    emptyCommon["options"]["common_compile"] = Json::array();
    ExpectInvalid(emptyCommon);

    Json unusedEmptyCommon = ValidManifest();
    unusedEmptyCommon["options"]["common_compile"] = Json::array();
    unusedEmptyCommon["kernels"][0]["objects"][0]["commands"][0]["options"] = {"${output:assets/kernel.o}"};
    ResourceManifest parsed;
    EXPECT_TRUE(Parse(unusedEmptyCommon, parsed));
}

TEST(ResourceManifestTest, RejectsInvalidKernelAndConstants)
{
    const std::vector<std::function<void(Json&)>> kernelMutations = {
        [](Json& value) { value["kernels"][0] = 1; },
        [](Json& value) { value["kernels"][0].erase("kernel_name"); },
        [](Json& value) { value["kernels"][0]["kernel_name"] = ""; },
        [](Json& value) { value["kernels"][0]["constants_info"] = 1; },
        [](Json& value) { value["kernels"][0].erase("objects"); },
        [](Json& value) { value["kernels"][0]["objects"] = Json::array(); },
        [](Json& value) { value["kernels"][0]["link_options"] = 1; },
        [](Json& value) { value["kernels"].push_back(value["kernels"][0]); },
    };
    for (const auto& mutate : kernelMutations) {
        Json value = ValidManifest();
        mutate(value);
        ExpectInvalid(value);
    }

    const std::vector<std::function<void(Json&)>> constantMutations = {
        [](Json& value) { value["kernels"][0]["constants_info"][0] = 1; },
        [](Json& value) { value["kernels"][0]["constants_info"][0].erase("name"); },
        [](Json& value) { value["kernels"][0]["constants_info"][0]["name"] = ""; },
        [](Json& value) { value["kernels"][0]["constants_info"][0]["parameter_index"] = -1; },
        [](Json& value) { value["kernels"][0]["constants_info"][0]["byte_size"] = 0; },
        [](Json& value) { value["kernels"][0]["constants_info"][0]["file"] = 1; },
        [](Json& value) { value["kernels"][0]["constants_info"][0]["template"] = ""; },
        [](Json& value) { value["kernels"][0]["constants_info"][0]["file"] = "assets/const.bin"; },
        [](Json& value) { value["kernels"][0]["constants_info"][0]["file"] = "${resource}/outside/const.bin"; },
        [](Json& value) { value["kernels"][0]["constants_info"].push_back(value["kernels"][0]["constants_info"][0]); },
    };
    for (const auto& mutate : constantMutations) {
        Json value = ValidManifest();
        mutate(value);
        ExpectInvalid(value);
    }
}

TEST(ResourceManifestTest, RejectsInvalidObjectsCommandsOutputsAndLinkOptions)
{
    const std::vector<std::function<void(Json&)>> mutations = {
        [](Json& value) { value["kernels"][0]["objects"][0] = 1; },
        [](Json& value) { value["kernels"][0]["objects"][0].erase("objects_name"); },
        [](Json& value) { value["kernels"][0]["objects"][0]["objects_name"] = "dir/name"; },
        [](Json& value) { value["kernels"][0]["objects"][0].erase("objects_type"); },
        [](Json& value) { value["kernels"][0]["objects"][0]["objects_type"] = "unsupported"; },
        [](Json& value) { value["kernels"][0]["objects"][0]["commands"] = Json::array(); },
        [](Json& value) { value["kernels"][0]["objects"][0]["output"] = Json::array(); },
        [](Json& value) { value["kernels"][0]["objects"].push_back(value["kernels"][0]["objects"][0]); },
        [](Json& value) { value["kernels"][0]["objects"][0]["output"][0] = 1; },
        [](Json& value) { value["kernels"][0]["objects"][0]["output"][0] = "assets/kernel.o"; },
        [](Json& value) { value["kernels"][0]["objects"][0]["output"][0] = "${output:assets/other.o}"; },
        [](Json& value) { value["kernels"][0]["objects"][0]["commands"][0] = 1; },
        [](Json& value) { value["kernels"][0]["objects"][0]["commands"][0].erase("type"); },
        [](Json& value) { value["kernels"][0]["objects"][0]["commands"][0].erase("stage"); },
        [](Json& value) { value["kernels"][0]["objects"][0]["commands"][0].erase("options"); },
        [](Json& value) { value["kernels"][0]["objects"][0]["commands"][0]["type"] = "run"; },
        [](Json& value) { value["kernels"][0]["objects"][0]["commands"][0]["options"] = {1}; },
        [](Json& value) { value["kernels"][0]["link_options"] = {1}; },
        [](Json& value) { value["kernels"][0]["link_options"] = {"${unknown}"}; },
    };
    for (const auto& mutate : mutations) {
        Json value = ValidManifest();
        mutate(value);
        ExpectInvalid(value);
    }
}

} // namespace
} // namespace resource
} // namespace ascendc
