/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_ACLRTC_SPECIALIZATION_COMPILE_TYPES_H
#define ASCENDC_ACLRTC_SPECIALIZATION_COMPILE_TYPES_H

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace ascendc {
namespace resource {

struct ManifestConstant {
    std::string name;
    uint64_t parameterIndex = 0;
    uint64_t byteSize = 0;
    std::string file;
    std::string templateValue;
};

struct ManifestCommand {
    std::string type;
    uint64_t stage = 0;
    std::vector<std::string> options;
};

struct ManifestObject {
    std::string objectsName;
    std::string objectsType;
    std::vector<ManifestCommand> commands;
    std::vector<std::string> outputs;
};

struct ManifestKernel {
    std::string kernelName;
    std::vector<ManifestConstant> constants;
    std::vector<ManifestObject> objects;
    std::vector<std::string> linkOptions;
};

struct ResourceManifest {
    std::string resourceId;
    std::string socVersion;
    std::string sourceFile;
    std::string baseDir;
    std::vector<ManifestKernel> kernels;
};

struct ResourceFileData {
    std::string fileName;
    std::string filePath;
    std::string materializedPath;
    std::vector<uint8_t> bytes;
};

struct ResourceRecord {
    std::string resourceId;
    std::string socVersion;
    std::string sourceFile;
    std::string materializedRoot;
    std::string resourceSoPath;
    ManifestKernel kernel;
};

struct ResourceRecordPack {
    std::string socVersion;
    std::string resourceSoPath;
    std::vector<ResourceFileData> files;
    bool filesMaterialized = false;
    std::unordered_map<std::string, std::unique_ptr<ResourceRecord>> records;
};

struct CompiledOutput {
    std::string output;
    std::vector<uint8_t> data;
};

} // namespace resource
} // namespace ascendc

#endif // ASCENDC_ACLRTC_SPECIALIZATION_COMPILE_TYPES_H
