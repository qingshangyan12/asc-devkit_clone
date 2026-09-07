/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "manifest_constant.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <limits>
#include <vector>

namespace {
using Json = nlohmann::json;
using ascendc::ManifestConstant;
using ascendc::ParseManifestConstant;

Json MakeConstant()
{
    return {{"name", "tiling"}, {"parameter_index", 2}, {"file", "${resource}/tiling.h"}, {"template", "@@TILING@@"}};
}

TEST(ManifestConstantTest, ResolvesDefaultAndExplicitStructSizes)
{
    for (bool explicitType : {false, true}) {
        Json input = MakeConstant();
        if (explicitType) {
            input["arg_type"] = "struct";
        }
        ManifestConstant constant;
        std::string error = "previous failure";
        ASSERT_TRUE(ParseManifestConstant(input, constant, error)) << error;
        EXPECT_TRUE(error.empty());
        input.clear();
        EXPECT_EQ(constant.name, "tiling");
        EXPECT_EQ(constant.parameterIndex, 2U);
        EXPECT_EQ(constant.targetFile, "${resource}/tiling.h");
        EXPECT_EQ(constant.templateText, "@@TILING@@");
        EXPECT_TRUE(constant.sizeRule.UsesArgumentSize());
        EXPECT_EQ(constant.sizeRule.ResolveByteCount(17U), 17U);
    }
}

TEST(ManifestConstantTest, PreservesSignedAndUnsignedPointerSizeBoundaries)
{
    for (const Json& size : std::vector<Json>{1, int64_t{4}, std::numeric_limits<uint64_t>::max()}) {
        Json input = MakeConstant();
        input["arg_type"] = "pointer";
        input["byte_size"] = size;
        input["parameter_index"] = std::numeric_limits<uint64_t>::max();
        ManifestConstant constant;
        std::string error;
        ASSERT_TRUE(ParseManifestConstant(input, constant, error)) << error;
        EXPECT_EQ(constant.parameterIndex, std::numeric_limits<uint64_t>::max());
        EXPECT_FALSE(constant.sizeRule.UsesArgumentSize());
        EXPECT_EQ(constant.sizeRule.ResolveByteCount(0U), size.get<uint64_t>());
        EXPECT_EQ(constant.sizeRule.ResolveByteCount(999U), size.get<uint64_t>());
    }
}

TEST(ManifestConstantTest, RejectsInvalidFieldsWithoutChangingParsedConstant)
{
    std::vector<Json> inputs{nullptr, Json::array(), 1};
    for (const char* field : {"name", "parameter_index", "file", "template"}) {
        Json input = MakeConstant();
        input.erase(field);
        inputs.push_back(input);
    }
    for (const Json& fields : std::vector<Json>{
             {{"name", ""}},
             {{"name", 1}},
             {{"file", ""}},
             {{"file", nullptr}},
             {{"template", ""}},
             {{"template", true}},
             {{"parameter_index", -1}},
             {{"parameter_index", 1.5}},
             {{"parameter_index", "2"}},
             {{"parameter_index", true}},
             {{"arg_type", nullptr}},
             {{"arg_type", "other"}},
             {{"extra", 1}},
             {{"byte_size", 4}},
             {{"arg_type", "struct"}, {"byte_size", 4}},
             {{"arg_type", "pointer"}},
             {{"arg_type", "pointer"}, {"byte_size", 0}},
             {{"arg_type", "pointer"}, {"byte_size", -1}},
             {{"arg_type", "pointer"}, {"byte_size", 1.5}},
             {{"arg_type", "pointer"}, {"byte_size", "4"}},
             {{"arg_type", "pointer"}, {"byte_size", true}}}) {
        Json input = MakeConstant();
        input.update(fields);
        inputs.push_back(input);
    }
    ManifestConstant constant;
    std::string error;
    Json valid = MakeConstant();
    valid.update({{"arg_type", "pointer"}, {"byte_size", 7}});
    ASSERT_TRUE(ParseManifestConstant(valid, constant, error));
    for (const Json& input : inputs) {
        SCOPED_TRACE(input.dump());
        EXPECT_FALSE(ParseManifestConstant(input, constant, error));
        EXPECT_FALSE(error.empty());
        EXPECT_EQ(constant.name, "tiling");
        EXPECT_EQ(constant.parameterIndex, 2U);
        EXPECT_EQ(constant.targetFile, "${resource}/tiling.h");
        EXPECT_EQ(constant.templateText, "@@TILING@@");
        EXPECT_FALSE(constant.sizeRule.UsesArgumentSize());
        EXPECT_EQ(constant.sizeRule.ResolveByteCount(99U), 7U);
    }
}
} // namespace
