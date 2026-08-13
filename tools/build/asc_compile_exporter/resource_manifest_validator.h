/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_RESOURCE_MANIFEST_VALIDATOR_H
#define ASCENDC_RESOURCE_MANIFEST_VALIDATOR_H

#include <string>

#include "nlohmann/json_fwd.hpp"

namespace ascendc {

bool ValidateResourceManifest(const nlohmann::json& manifest, const std::string& origin);

} // namespace ascendc

#endif // ASCENDC_RESOURCE_MANIFEST_VALIDATOR_H
