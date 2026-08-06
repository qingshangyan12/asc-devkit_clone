/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "bundle_source_generator.h"

#include <fstream>
#include <ostream>
#include <stdexcept>
#include <utility>

#include "ascendc_tool_log.h"
#include "file_utils.h"
#include "manifest_abi_header_content.h"

namespace ascendc {
namespace manifest_generator {
namespace {

std::ofstream OpenGeneratedOutput(const std::string& path)
{
    std::ofstream output;
    if (!FileUtils::OpenExclusiveOutput(path, output)) {
        throw std::runtime_error("failed to create generated output: " + path);
    }
    return output;
}

void FinishGeneratedOutput(std::ofstream& output, const std::string& path)
{
    if (!FileUtils::FinalizeOutput(output)) {
        throw std::runtime_error("failed to finalize generated output: " + path);
    }
}

} // namespace

BundleSourceGenerator::BundleSourceGenerator(std::string workDir, const std::vector<ManifestUnit>& units) noexcept
    : workDir_(std::move(workDir)), units_(units)
{}

void BundleSourceGenerator::Generate() const
{
    ASCENDLOGD("Generating bundle sources in %s", workDir_.c_str());
    const std::string abiHeader = FileUtils::JoinPath(workDir_, "ascendc_manifest_abi.h");
    ASCENDLOGD("Generating bundle ABI header %s", abiHeader.c_str());
    EmitAbiHeader(abiHeader);
    const std::string bundleSource = FileUtils::JoinPath(workDir_, "compile_resource_bundle.cpp");
    ASCENDLOGD("Generating bundle source %s", bundleSource.c_str());
    EmitBundleSource(bundleSource);
    for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
        const std::string manifestSource =
            FileUtils::JoinPath(workDir_, "manifest_" + std::to_string(unitIndex) + ".cpp");
        ASCENDLOGD("Generating manifest source %s", manifestSource.c_str());
        EmitManifestSource(manifestSource, unitIndex, units_[unitIndex]);
    }
    const std::string makefile = FileUtils::JoinPath(workDir_, "Makefile");
    ASCENDLOGD("Generating build file %s", makefile.c_str());
    EmitMakefile(makefile);
    ASCENDLOGI("Generated bundle sources for %zu manifest(s)", units_.size());
}

void BundleSourceGenerator::EmitAbiHeader(const std::string& path)
{
    std::ofstream output = OpenGeneratedOutput(path);
    output << kManifestAbiHeaderContent;
    FinishGeneratedOutput(output, path);
}

void BundleSourceGenerator::EmitByteArray(
    std::ostream& output, const std::string& name, const uint8_t* data, size_t size, bool nulTerminate)
{
    output << "static const uint8_t " << name << "[] = {";
    for (size_t index = 0; index < size; ++index) {
        if (index % 16U == 0U) {
            output << "\n    ";
        }
        output << static_cast<unsigned int>(data[index]) << ", ";
    }
    if (nulTerminate) {
        output << "0";
    }
    output << "\n};\n";
}

void BundleSourceGenerator::EmitString(std::ostream& output, const std::string& name, const std::string& value)
{
    EmitByteArray(output, name + "_bytes", reinterpret_cast<const uint8_t*>(value.data()), value.size(), true);
    output << "static const AcString " << name << " = { reinterpret_cast<const char *>(" << name << "_bytes), "
           << value.size() << "ULL };\n";
}

void BundleSourceGenerator::EmitManifestSource(const std::string& path, size_t unitIndex, const ManifestUnit& unit)
{
    std::ofstream output = OpenGeneratedOutput(path);
    const std::string prefix = "g_manifest_" + std::to_string(unitIndex);
    output << "#include \"ascendc_manifest_abi.h\"\n\nnamespace {\n";
    EmitString(output, prefix + "_json", unit.json);
    for (size_t fileIndex = 0; fileIndex < unit.files.size(); ++fileIndex) {
        const ResourceFile& file = unit.files[fileIndex];
        const std::string filePrefix = prefix + "_file_" + std::to_string(fileIndex);
        EmitString(output, filePrefix + "_name", file.fileName);
        EmitString(output, filePrefix + "_path", file.filePath);
        EmitByteArray(output, filePrefix + "_data", file.data.data(), file.data.size(), false);
    }
    if (!unit.files.empty()) {
        output << "static const AcCompileResourceFile " << prefix << "_files[] = {\n";
        for (size_t fileIndex = 0; fileIndex < unit.files.size(); ++fileIndex) {
            const std::string filePrefix = prefix + "_file_" + std::to_string(fileIndex);
            output << "    { " << filePrefix << "_name, " << filePrefix << "_path, " << filePrefix << "_data, "
                   << unit.files[fileIndex].data.size() << "ULL },\n";
        }
        output << "};\n";
    }
    output << "} // namespace\n\nnamespace ascendc {\nnamespace resource {\nnamespace generated {\n"
           << "AcCompileResourceManifest GetManifest" << unitIndex << "()\n{\n"
           << "    return { " << prefix << "_json, " << (unit.files.empty() ? "nullptr" : prefix + "_files") << ", "
           << unit.files.size()
           << "ULL };\n}\n} // namespace generated\n"
              "} // namespace resource\n} // namespace ascendc\n";
    FinishGeneratedOutput(output, path);
}

void BundleSourceGenerator::EmitBundleSource(const std::string& path) const
{
    std::ofstream output = OpenGeneratedOutput(path);
    output << "#include \"ascendc_manifest_abi.h\"\n\n"
              "namespace ascendc {\nnamespace resource {\nnamespace generated {\n";
    for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
        output << "AcCompileResourceManifest GetManifest" << unitIndex << "();\n";
    }
    output << "} // namespace generated\n} // namespace resource\n} // namespace ascendc\n\nnamespace {\n";
    output << "static const AcCompileResourceManifest g_manifests[] = {\n";
    for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
        output << "    ascendc::resource::generated::GetManifest" << unitIndex << "(),\n";
    }
    output << "};\nstatic const AcCompileResourceBundle g_bundle = {\n"
           << "    { AC_COMPILE_RESOURCE_MAGIC, AC_COMPILE_RESOURCE_ABI_VERSION, "
              "sizeof(AcCompileResourceBundle), 0U },\n"
              "    g_manifests, "
           << units_.size() << "ULL, nullptr, 0ULL\n};\n} // namespace\n\n"
           << "extern \"C\" __attribute__((visibility(\"default\")))\n"
              "const AcCompileResourceBundleHeader *AscendcGetCompileResourceBundle(void)\n"
              "{\n    return &g_bundle.header;\n}\n";
    FinishGeneratedOutput(output, path);
}

void BundleSourceGenerator::EmitMakefile(const std::string& path) const
{
    std::ofstream output = OpenGeneratedOutput(path);
    output << "SOURCES := compile_resource_bundle.cpp";
    for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
        output << " manifest_" << unitIndex << ".cpp";
    }
    output << "\nOBJECTS := $(SOURCES:.cpp=.o)\n\n"
              ".PHONY: all\n"
              "all: $(OBJECTS)\n"
              "\t\"$${ASCENDC_BUNDLE_CXX}\" -shared $(OBJECTS) "
              "-Wl,-z,relro,-z,now,-z,noexecstack "
              "-o \"$${ASCENDC_BUNDLE_OUTPUT}\"\n\n"
              "%.o: %.cpp\n"
              "\t\"$${ASCENDC_BUNDLE_CXX}\" -std=c++11 -fPIC -fvisibility=hidden -O2 -c $< -o $@\n";
    FinishGeneratedOutput(output, path);
}

} // namespace manifest_generator
} // namespace ascendc
