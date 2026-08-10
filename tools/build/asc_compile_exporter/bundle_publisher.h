/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_BUNDLE_PUBLISHER_H
#define ASCENDC_BUNDLE_PUBLISHER_H

#include <string>

namespace ascendc {
namespace manifest_generator {

class BundlePublisher final {
public:
    BundlePublisher() = delete;

    static bool Publish(const std::string& staged, const std::string& output);

private:
    static bool PublishAcrossFileSystems(const std::string& staged, const std::string& output);
};

} // namespace manifest_generator
} // namespace ascendc

#endif // ASCENDC_BUNDLE_PUBLISHER_H
