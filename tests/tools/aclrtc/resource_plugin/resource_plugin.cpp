/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ascendc_manifest_abi.h"

#include <cstdlib>
#include <cstring>
#include <string>

#ifdef ACLRTC_UT_NO_ENTRY
extern "C" int AclrtcUtNoEntryPoint() { return 0; }
#else
namespace {

constexpr const char* kManifest = R"json({
  "schema_version": "1.0",
  "resource_id": "ut_resource",
  "soc_version": "Ascend910B",
  "source_file": "kernel.cpp",
  "base_dir": "assets",
  "options": {"common_compile": ["-O2", "-I${resource}/assets/include.bin"]},
  "kernels": [{
    "kernel_name": "ut_kernel",
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
      "commands": [{
        "type": "compile",
        "stage": 0,
        "options": ["${options:common_compile}", "${source_dir}", "-o", "${output:assets/kernel.o}"]
      }],
      "output": ["${output:assets/kernel.o}"]
    }],
    "link_options": ["${resource}/assets/link.ld"]
  }]
})json";

constexpr uint8_t kConstData[] = {1U, 2U, 3U, 4U};
constexpr uint8_t kIncludeData[] = {'i', 'n', 'c'};
constexpr uint8_t kLinkData[] = {'l', 'i', 'n', 'k'};

AcCompileResourceFile gFiles[] = {
    {{"const.bin", 9U}, {"assets/const.bin", 16U}, kConstData, sizeof(kConstData)},
    {{"include.bin", 11U}, {"assets/include.bin", 18U}, kIncludeData, sizeof(kIncludeData)},
    {{"link.ld", 7U}, {"assets/link.ld", 14U}, kLinkData, sizeof(kLinkData)},
};
AcCompileResourceManifest gManifest = {{kManifest, std::char_traits<char>::length(kManifest)}, gFiles, 3U};
AcCompileResourceExtension gExtension = {1U, 1U, 0U, nullptr, 0U};
AcCompileResourceBundle gBundle = {
    {AC_COMPILE_RESOURCE_MAGIC, AC_COMPILE_RESOURCE_ABI_VERSION, sizeof(AcCompileResourceBundle), 0U},
    &gManifest,
    1U,
    nullptr,
    0U,
};

void ResetBundle()
{
    gBundle = {
        {AC_COMPILE_RESOURCE_MAGIC, AC_COMPILE_RESOURCE_ABI_VERSION, sizeof(AcCompileResourceBundle), 0U},
        &gManifest,
        1U,
        nullptr,
        0U,
    };
    gExtension = {1U, 1U, 0U, nullptr, 0U};
}

} // namespace

extern "C" const AcCompileResourceBundleHeader* AscendcGetCompileResourceBundle()
{
    ResetBundle();
    const char* modeValue = std::getenv("ACLRTC_UT_BUNDLE_MODE");
    const std::string mode = modeValue == nullptr ? "valid" : modeValue;
    if (mode == "null_header") {
        return nullptr;
    }
    if (mode == "bad_magic") {
        gBundle.header.magic = 0U;
    } else if (mode == "bad_version") {
        gBundle.header.abiVersion = 0U;
    } else if (mode == "bad_size") {
        gBundle.header.structSize = 0U;
    } else if (mode == "bad_flags") {
        gBundle.header.flags = 1U;
    } else if (mode == "no_manifests") {
        gBundle.manifestCount = 0U;
    } else if (mode == "too_many_manifests") {
        gBundle.manifestCount = 4097U;
    } else if (mode == "null_manifests") {
        gBundle.manifests = nullptr;
    } else if (mode == "too_many_extensions") {
        gBundle.extensionCount = 1025U;
    } else if (mode == "null_extensions") {
        gBundle.extensionCount = 1U;
    } else if (mode == "invalid_extension_flags") {
        gExtension.flags = 2U;
        gBundle.extensions = &gExtension;
        gBundle.extensionCount = 1U;
    } else if (mode == "invalid_extension_data") {
        gExtension.dataSize = 1U;
        gBundle.extensions = &gExtension;
        gBundle.extensionCount = 1U;
    } else if (mode == "required_extension") {
        gExtension.flags = AC_COMPILE_RESOURCE_EXTENSION_REQUIRED;
        gBundle.extensions = &gExtension;
        gBundle.extensionCount = 1U;
    } else if (mode == "optional_extension") {
        gBundle.extensions = &gExtension;
        gBundle.extensionCount = 1U;
    }
    return &gBundle.header;
}
#endif
