/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the LICENSE.
 */

#include <gtest/gtest.h>

#include "bundle_source_generator.h"
#include "file_utils.h"
#include "make_bundle_builder.h"
#include "test_support.h"

namespace ascendc {
namespace manifest_generator {
namespace {

using asc_compile_exporter_test::ModuleTest;
using asc_compile_exporter_test::ScopedEnvironment;

TEST_F(ModuleTest, MakeBundleBuilderBuildsGeneratedSourcesWithoutCannHome)
{
    const std::string work = FileUtils::JoinPath(root_, "standalone-build");
    const std::string output = FileUtils::JoinPath(root_, "bundle.so");
    ASSERT_TRUE(FileUtils::CreateDirectories(work));
    const std::vector<ManifestUnit> units = {{"{\"base_dir\":\"resources\"}", {}}};
    BundleSourceGenerator(work, units).Generate();
    ScopedEnvironment cannHome("ASCEND_HOME_PATH", nullptr);

    EXPECT_NO_THROW(MakeBundleBuilder(work, output, "/usr/bin/make", "/usr/bin/c++", 1U).Build());
    EXPECT_TRUE(FileUtils::IsRegularFile(output));
}

} // namespace
} // namespace manifest_generator
} // namespace ascendc
