/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_MAKE_BUNDLE_BUILDER_H
#define ASCENDC_MAKE_BUNDLE_BUILDER_H

#include <cstdint>
#include <string>

namespace ascendc {
namespace manifest_generator {

class MakeBundleBuilder final {
public:
    MakeBundleBuilder(
        std::string workDir, std::string output, std::string makeExecutable, std::string cxxCompiler,
        uint32_t jobs) noexcept;

    bool Build() const;

private:
    std::string workDir_;
    std::string output_;
    std::string makeExecutable_;
    std::string cxxCompiler_;
    uint32_t jobs_ = 1U;
};

} // namespace manifest_generator
} // namespace ascendc

#endif // ASCENDC_MAKE_BUNDLE_BUILDER_H
