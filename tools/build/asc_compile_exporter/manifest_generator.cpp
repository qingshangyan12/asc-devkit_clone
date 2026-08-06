/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "manifest_generator.h"

#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <system_error>
#include <thread>
#include <utility>
#include <vector>

#include <sys/wait.h>
#include <unistd.h>

#include "ascendc_tool_log.h"
#include "nlohmann/json.hpp"

namespace fs = std::filesystem;
using Json = nlohmann::json;

namespace ascendc::manifest_generator {
namespace {

constexpr uintmax_t kMaxManifestBytes = 8U * 1024U * 1024U;
constexpr uintmax_t kMaxResourceFileBytes = 1U * 1024U * 1024U;
constexpr uintmax_t kMaxBundleBytes = 256U * 1024U * 1024U;
constexpr size_t kMaxManifestCount = 4096U;
constexpr size_t kMaxFilesPerManifest = 4096U;
constexpr size_t kMaxFilesPerBundle = 65536U;

std::runtime_error SystemError(const std::string &operation)
{
    return std::runtime_error(operation + ": " + std::strerror(errno));
}

struct ResourceFile {
    std::string fileName;
    std::string filePath;
    std::vector<uint8_t> data;
};

struct ManifestUnit {
    std::string json;
    std::vector<ResourceFile> files;
};

class FileSystem final {
public:
    static fs::path CanonicalDirectoryWithin(const fs::path &path, const fs::path &root,
                                             const std::string &field)
    {
        const fs::path canonical = CanonicalDirectory(path, field);
        if (!IsWithin(canonical, root)) {
            throw std::runtime_error(field + " is not a directory below the manifest: " +
                                     path.string());
        }
        return canonical;
    }

    static uintmax_t FileSize(const fs::path &path, const std::string &field)
    {
        std::error_code error;
        const uintmax_t size = fs::file_size(path, error);
        if (error) {
            throw std::runtime_error("failed to get " + field + " size: " + error.message());
        }
        return size;
    }

    static std::vector<uint8_t> ReadBytes(const fs::path &path, uintmax_t expectedSize,
                                          const std::string &field)
    {
        if (expectedSize > static_cast<uintmax_t>(std::numeric_limits<size_t>::max()) ||
            expectedSize > static_cast<uintmax_t>(std::numeric_limits<std::streamsize>::max())) {
            throw std::runtime_error(field + " is too large to read: " + path.string());
        }
        std::ifstream input(path, std::ios::binary);
        if (!input.is_open()) {
            throw std::runtime_error("failed to read " + field + ": " + path.string());
        }
        std::vector<uint8_t> data(static_cast<size_t>(expectedSize));
        if (!data.empty()) {
            input.read(reinterpret_cast<char *>(data.data()),
                       static_cast<std::streamsize>(data.size()));
            if (input.gcount() != static_cast<std::streamsize>(data.size())) {
                throw std::runtime_error(field + " changed while being read: " + path.string());
            }
        }
        char extra = '\0';
        if (input.get(extra)) {
            throw std::runtime_error(field + " grew while being read: " + path.string());
        }
        if (!input.eof()) {
            throw std::runtime_error("failed to read " + field + ": " + path.string());
        }
        return data;
    }

    static std::string ReadText(const fs::path &path, uintmax_t expectedSize,
                                const std::string &field)
    {
        const std::vector<uint8_t> bytes = ReadBytes(path, expectedSize, field);
        return std::string(bytes.begin(), bytes.end());
    }

    static std::ofstream OpenOutput(const fs::path &path)
    {
        std::error_code error;
        const bool exists = fs::exists(path, error);
        if (error) {
            throw std::runtime_error("failed to inspect generated file path: " + error.message());
        }
        if (exists) {
            throw std::runtime_error("generated file already exists in private work directory: " +
                                     path.string());
        }
        std::ofstream output(path, std::ios::trunc);
        if (!output.is_open()) {
            throw std::runtime_error("failed to create generated file: " + path.string());
        }
        return output;
    }

    static void FinishOutput(std::ofstream &output, const fs::path &path)
    {
        output.flush();
        if (!output) {
            throw std::runtime_error("failed to write generated file: " + path.string());
        }
        output.close();
        if (!output) {
            throw std::runtime_error("failed to close generated file: " + path.string());
        }
    }

    template <typename Visitor>
    static void ForEachRecursiveEntry(const fs::path &root, Visitor visitor)
    {
        std::error_code error;
        fs::recursive_directory_iterator iterator(root, error);
        if (error) {
            throw std::runtime_error("failed to enumerate " + root.string() + ": " + error.message());
        }
        const fs::recursive_directory_iterator end;
        while (iterator != end) {
            visitor(*iterator);
            iterator.increment(error);
            if (error) {
                throw std::runtime_error("failed to enumerate " + root.string() + ": " + error.message());
            }
        }
    }

    static bool IsWithin(const fs::path &path, const fs::path &root)
    {
        auto pathIt = path.begin();
        for (auto rootIt = root.begin(); rootIt != root.end(); ++rootIt, ++pathIt) {
            if (pathIt == path.end() || *pathIt != *rootIt) {
                return false;
            }
        }
        return true;
    }

private:
    static fs::path CanonicalDirectory(const fs::path &path, const std::string &field)
    {
        std::error_code error;
        const fs::file_status status = fs::symlink_status(path, error);
        if (error || fs::is_symlink(status)) {
            throw std::runtime_error(field + " must exist and must not be a symlink: " +
                                     path.string());
        }
        const fs::path canonical = fs::canonical(path, error);
        if (error) {
            throw std::runtime_error("failed to resolve " + field + ": " + path.string());
        }
        if (error || !fs::is_directory(canonical, error)) {
            throw std::runtime_error(field + " is not a directory: " + path.string());
        }
        return canonical;
    }
};

class ResourceBudget final {
public:
    void AddResource(uintmax_t size, const std::string &field)
    {
        if (fileCount_ >= kMaxFilesPerBundle) {
            throw std::runtime_error("bundle exceeds resource file count limit of " +
                                     std::to_string(kMaxFilesPerBundle));
        }
        ++fileCount_;
        AddPayload(size, field);
    }

    void AddPayload(uintmax_t size, const std::string &field)
    {
        if (size > kMaxBundleBytes - totalBytes_) {
            throw std::runtime_error("bundle exceeds raw payload limit while adding " + field);
        }
        totalBytes_ += size;
    }

private:
    uintmax_t totalBytes_{0U};
    size_t fileCount_{0U};
};

class CollectedManifestRepository final {
public:
    explicit CollectedManifestRepository(fs::path manifestSearchRoot) noexcept
        : manifestSearchRoot_(std::move(manifestSearchRoot))
    {
    }

    std::vector<ManifestUnit> Load() const
    {
        ASCENDLOGD("Discovering manifests under %s", manifestSearchRoot_.c_str());
        ResourceBudget budget;
        const std::vector<fs::path> manifests = DiscoverManifestFiles(manifestSearchRoot_);
        ASCENDLOGI("Discovered %zu manifest(s)", manifests.size());
        std::vector<ManifestUnit> units;
        units.reserve(manifests.size());
        for (const fs::path &manifest : manifests) {
            units.push_back(LoadUnit(manifest, budget));
        }
        return units;
    }

private:
    static std::string ReadBaseDir(const std::string &text, const fs::path &manifestPath)
    {
        Json manifest;
        try {
            manifest = Json::parse(text);
        } catch (const Json::parse_error &error) {
            throw std::runtime_error("manifest is not valid JSON: " + manifestPath.string() +
                                     ": " + error.what());
        }

        try {
            return manifest.at("base_dir").get<std::string>();
        } catch (const Json::exception &error) {
            throw std::runtime_error("manifest base_dir is unavailable: " +
                                     manifestPath.string() + ": " + error.what());
        }
    }

    static bool IsManifestFile(const fs::path &path)
    {
        constexpr const char *suffix = "_manifest.json";
        const std::string name = path.filename().string();
        return name.size() > std::strlen(suffix) &&
            name.compare(name.size() - std::strlen(suffix), std::strlen(suffix), suffix) == 0;
    }

    static std::vector<fs::path> DiscoverManifestFiles(const fs::path &manifestSearchRoot)
    {
        std::vector<fs::path> manifests;
        FileSystem::ForEachRecursiveEntry(manifestSearchRoot, [&](const fs::directory_entry &entry) {
            std::error_code error;
            if (entry.is_symlink(error) || error || !entry.is_regular_file(error) || error ||
                !IsManifestFile(entry.path())) {
                return;
            }
            if (manifests.size() >= kMaxManifestCount) {
                throw std::runtime_error("collection exceeds manifest count limit of " +
                                         std::to_string(kMaxManifestCount));
            }
            manifests.push_back(entry.path());
        });
        std::sort(manifests.begin(), manifests.end());
        if (manifests.empty()) {
            throw std::runtime_error("collected resources contain no manifest units");
        }
        return manifests;
    }

    static ManifestUnit LoadUnit(const fs::path &manifestPath, ResourceBudget &budget)
    {
        ASCENDLOGD("Loading manifest %s", manifestPath.c_str());
        ManifestUnit unit;
        const uintmax_t manifestSize = FileSystem::FileSize(manifestPath, "manifest");
        if (manifestSize > kMaxManifestBytes) {
            throw std::runtime_error("manifest exceeds size limit: " + manifestPath.string());
        }
        budget.AddPayload(manifestSize, "manifest " + manifestPath.string());
        unit.json = FileSystem::ReadText(manifestPath, manifestSize, "manifest");
        const std::string baseDir = ReadBaseDir(unit.json, manifestPath);

        const fs::path manifestRoot = manifestPath.parent_path();
        const fs::path resourceRoot = FileSystem::CanonicalDirectoryWithin(
            manifestRoot / baseDir, manifestRoot, "manifest base_dir");
        FileSystem::ForEachRecursiveEntry(resourceRoot, [&](const fs::directory_entry &entry) {
            std::error_code error;
            if (entry.is_symlink(error) || error) {
                throw std::runtime_error("manifest base_dir contains a symlink: " +
                                         entry.path().string());
            }
            if (entry.is_directory(error) && !error) {
                return;
            }
            if (error || !entry.is_regular_file(error) || error) {
                throw std::runtime_error("manifest base_dir contains a non-regular file: " +
                                         entry.path().string());
            }
            const fs::path source = entry.path();
            const std::string fileName = source.filename().string();
            const std::string filePath = source.lexically_relative(manifestRoot).generic_string();
            const uintmax_t resourceSize = FileSystem::FileSize(source, "manifest resource");
            if (resourceSize == 0U) {
                ASCENDLOGD("Skipping empty manifest resource %s", source.c_str());
                return;
            }
            if (resourceSize > kMaxResourceFileBytes) {
                throw std::runtime_error("manifest resource exceeds size limit: " + source.string());
            }
            if (unit.files.size() >= kMaxFilesPerManifest) {
                throw std::runtime_error("manifest exceeds resource file count limit: " +
                                         manifestPath.string());
            }
            budget.AddResource(resourceSize, "manifest resource " + source.string());
            unit.files.push_back({fileName, filePath,
                                  FileSystem::ReadBytes(source, resourceSize, "manifest resource")});
        });
        std::sort(unit.files.begin(), unit.files.end(),
                  [](const ResourceFile &left, const ResourceFile &right) {
                      return left.filePath < right.filePath;
                  });
        ASCENDLOGD("Loaded manifest %s with %zu resource file(s)", manifestPath.c_str(),
                 unit.files.size());
        return unit;
    }

    fs::path manifestSearchRoot_;
};

class BundleSourceGenerator final {
public:
    BundleSourceGenerator(fs::path workDir, std::string packageId,
                          const std::vector<ManifestUnit> &units) noexcept
        : workDir_(std::move(workDir)), packageId_(std::move(packageId)), units_(units)
    {
    }

    void Generate() const
    {
        ASCENDLOGD("Generating bundle sources in %s", workDir_.c_str());
        const fs::path bundleSource = workDir_ / "compile_resource_bundle.cpp";
        ASCENDLOGD("Generating bundle source %s", bundleSource.c_str());
        EmitBundleSource(bundleSource);
        for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
            const fs::path manifestSource =
                workDir_ / ("manifest_" + std::to_string(unitIndex) + ".cpp");
            ASCENDLOGD("Generating manifest source %s", manifestSource.c_str());
            EmitManifestSource(manifestSource, unitIndex, units_[unitIndex]);
        }
        const fs::path makefile = workDir_ / "Makefile";
        ASCENDLOGD("Generating build file %s", makefile.c_str());
        EmitMakefile(makefile);
        ASCENDLOGI("Generated bundle sources for %zu manifest(s)", units_.size());
    }

private:
    static void EmitByteArray(std::ostream &output, const std::string &name,
                              const uint8_t *data, size_t size, bool nulTerminate)
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

    static void EmitString(std::ostream &output, const std::string &name, const std::string &value)
    {
        EmitByteArray(output, name + "_bytes", reinterpret_cast<const uint8_t *>(value.data()),
                      value.size(), true);
        output << "static const AcString " << name << " = { reinterpret_cast<const char *>("
               << name << "_bytes), " << value.size() << "ULL };\n";
    }

    static void EmitManifestSource(const fs::path &path, size_t unitIndex, const ManifestUnit &unit)
    {
        std::ofstream output = FileSystem::OpenOutput(path);
        const std::string prefix = "g_manifest_" + std::to_string(unitIndex);
        output << "#include \"ascendc_compile_resource/ascendc_manifest_abi.h\"\n\nnamespace {\n";
        EmitString(output, prefix + "_json", unit.json);
        for (size_t fileIndex = 0; fileIndex < unit.files.size(); ++fileIndex) {
            const ResourceFile &file = unit.files[fileIndex];
            const std::string filePrefix = prefix + "_file_" + std::to_string(fileIndex);
            EmitString(output, filePrefix + "_name", file.fileName);
            EmitString(output, filePrefix + "_path", file.filePath);
            EmitByteArray(output, filePrefix + "_data", file.data.data(), file.data.size(), false);
        }
        if (!unit.files.empty()) {
            output << "static const AcCompileResourceFile " << prefix << "_files[] = {\n";
            for (size_t fileIndex = 0; fileIndex < unit.files.size(); ++fileIndex) {
                const std::string filePrefix = prefix + "_file_" + std::to_string(fileIndex);
                output << "    { " << filePrefix << "_name, " << filePrefix << "_path, "
                       << filePrefix << "_data, "
                       << unit.files[fileIndex].data.size() << "ULL },\n";
            }
            output << "};\n";
        }
        output << "} // namespace\n\nnamespace ascendc::resource::generated {\n"
               << "AcCompileResourceManifest GetManifest" << unitIndex << "()\n{\n"
               << "    return { " << prefix << "_json, "
               << (unit.files.empty() ? "nullptr" : prefix + "_files") << ", "
               << unit.files.size() << "ULL };\n}\n} // namespace ascendc::resource::generated\n";
        FileSystem::FinishOutput(output, path);
    }

    void EmitBundleSource(const fs::path &path) const
    {
        std::ofstream output = FileSystem::OpenOutput(path);
        output << "#include \"ascendc_compile_resource/ascendc_manifest_abi.h\"\n\n"
                  "namespace ascendc::resource::generated {\n";
        for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
            output << "AcCompileResourceManifest GetManifest" << unitIndex << "();\n";
        }
        output << "} // namespace ascendc::resource::generated\n\nnamespace {\n";
        EmitString(output, "g_package_id", packageId_);
        output << "static const AcCompileResourceManifest g_manifests[] = {\n";
        for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
            output << "    ascendc::resource::generated::GetManifest" << unitIndex << "(),\n";
        }
        output << "};\nstatic const AcCompileResourceBundle g_bundle = {\n"
               << "    { AC_COMPILE_RESOURCE_MAGIC, AC_COMPILE_RESOURCE_ABI_VERSION, "
                  "sizeof(AcCompileResourceBundle), 0U },\n"
                  "    g_package_id, g_manifests, "
               << units_.size() << "ULL, nullptr, 0ULL\n};\n} // namespace\n\n"
               << "extern \"C\" __attribute__((visibility(\"default\")))\n"
                  "const AcCompileResourceBundleHeader *AscendcGetCompileResourceBundle(void)\n"
                  "{\n    return &g_bundle.header;\n}\n";
        FileSystem::FinishOutput(output, path);
    }

    void EmitMakefile(const fs::path &path) const
    {
        std::ofstream output = FileSystem::OpenOutput(path);
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
                  "\t\"$${ASCENDC_BUNDLE_CXX}\" -std=c++17 -fPIC -fvisibility=hidden -O2 "
                  "-I\"$${ASCENDC_BUNDLE_INCLUDE_DIR}\" -c $< -o $@\n";
        FileSystem::FinishOutput(output, path);
    }

    fs::path workDir_;
    std::string packageId_;
    const std::vector<ManifestUnit> &units_;
};

class BuildWorkspace final {
public:
    static BuildWorkspace Create(const fs::path &requestedParent, bool keepTemp)
    {
        std::error_code error;
        const fs::path parent = requestedParent.empty() ? fs::temp_directory_path(error) :
                                                         fs::absolute(requestedParent, error);
        if (error) {
            throw std::runtime_error("failed to resolve bundle work directory: " + error.message());
        }
        fs::create_directories(parent, error);
        if (error) {
            throw std::runtime_error("failed to create bundle work directory: " + error.message());
        }
        if (!fs::is_directory(parent, error) || error) {
            throw std::runtime_error("bundle work directory is not a directory: " + parent.string());
        }

        std::string pathTemplate = (parent / ".manifest-generator-XXXXXX").string();
        std::vector<char> buffer(pathTemplate.begin(), pathTemplate.end());
        buffer.push_back('\0');
        char *created = mkdtemp(buffer.data());
        if (created == nullptr) {
            throw SystemError("failed to create private bundle work directory");
        }
        ASCENDLOGD("Created private bundle work directory %s", created);
        return BuildWorkspace(created, keepTemp);
    }

    BuildWorkspace(const BuildWorkspace &) = delete;
    BuildWorkspace &operator=(const BuildWorkspace &) = delete;

    BuildWorkspace(BuildWorkspace &&other) noexcept
        : session_(std::move(other.session_)), keepTemp_(other.keepTemp_)
    {
        other.session_.clear();
    }

    ~BuildWorkspace()
    {
        if (!session_.empty() && !keepTemp_) {
            std::error_code ignored;
            fs::remove_all(session_, ignored);
            if (ignored) {
                ASCENDLOGE("Failed to remove private bundle work directory %s: %s",
                         session_.c_str(), ignored.message().c_str());
            } else {
                ASCENDLOGD("Removed private bundle work directory %s", session_.c_str());
            }
        }
    }

    const fs::path &Session() const noexcept
    {
        return session_;
    }

private:
    BuildWorkspace(fs::path session, bool keepTemp) noexcept
        : session_(std::move(session)), keepTemp_(keepTemp)
    {
    }

    fs::path session_;
    bool keepTemp_ = false;
};

class ExecutableLocator final {
public:
    static std::string FindCompiler()
    {
        const char *configured = std::getenv("CXX");
        if (configured != nullptr && configured[0] != '\0') {
            const fs::path found = Find(configured);
            if (found.empty()) {
                throw std::runtime_error("CXX does not resolve to an executable compiler: " +
                                         std::string(configured));
            }
            return found.string();
        }
        for (const char *candidate : {"c++", "g++", "clang++"}) {
            const fs::path found = Find(candidate);
            if (!found.empty()) {
                return found.string();
            }
        }
        throw std::runtime_error("no C++ compiler found in PATH; set CXX to an executable compiler");
    }

    static fs::path FindRequired(const std::string &name)
    {
        const fs::path found = Find(name);
        if (found.empty()) {
            throw std::runtime_error("executable not found in PATH: " + name);
        }
        return found;
    }

private:
    static fs::path ExecutablePath(const fs::path &candidate)
    {
        std::error_code error;
        if (!fs::is_regular_file(candidate, error) || error || access(candidate.c_str(), X_OK) != 0) {
            return {};
        }
        const fs::path canonical = fs::canonical(candidate, error);
        return error ? fs::path{} : canonical;
    }

    static fs::path Find(const std::string &name)
    {
        const fs::path requested(name);
        if (requested.has_parent_path()) {
            return ExecutablePath(requested);
        }
        const char *pathEnvironment = std::getenv("PATH");
        const std::string searchPath = pathEnvironment == nullptr ? "/usr/bin:/bin" : pathEnvironment;
        size_t begin = 0U;
        while (begin <= searchPath.size()) {
            const size_t end = searchPath.find(':', begin);
            const std::string directory = searchPath.substr(
                begin, end == std::string::npos ? std::string::npos : end - begin);
            const fs::path found = ExecutablePath(
                (directory.empty() ? fs::path(".") : fs::path(directory)) / name);
            if (!found.empty()) {
                return found;
            }
            if (end == std::string::npos) {
                break;
            }
            begin = end + 1U;
        }
        return {};
    }
};

class ProcessRunner final {
public:
    static void Run(const std::vector<std::string> &arguments,
                    const std::vector<std::pair<std::string, std::string>> &environment)
    {
        if (arguments.empty()) {
            throw std::invalid_argument("subprocess arguments must not be empty");
        }
        std::vector<char *> argv;
        argv.reserve(arguments.size() + 1U);
        for (const std::string &argument : arguments) {
            argv.push_back(const_cast<char *>(argument.c_str()));
        }
        argv.push_back(nullptr);
        const pid_t child = fork();
        if (child < 0) {
            const std::runtime_error error = SystemError("failed to fork subprocess");
            ASCENDLOGD("Subprocess launch failed: %s", error.what());
            throw error;
        }
        if (child == 0) {
            for (const char *name : {"MAKEFLAGS", "MFLAGS", "MAKEFILES", "GNUMAKEFLAGS"}) {
                unsetenv(name);
            }
            for (const auto &item : environment) {
                if (setenv(item.first.c_str(), item.second.c_str(), 1) != 0) {
                    _exit(126);
                }
            }
            execv(argv[0], argv.data());
            _exit(127);
        }

        ASCENDLOGD("Started subprocess pid=%d executable=%s", static_cast<int>(child),
                 arguments.front().c_str());

        int status = 0;
        pid_t waited = -1;
        do {
            waited = waitpid(child, &status, 0);
        } while (waited < 0 && errno == EINTR);
        if (waited < 0) {
            const std::runtime_error error = SystemError("failed to wait for subprocess");
            ASCENDLOGD("Subprocess wait failed: %s", error.what());
            throw error;
        }
        if (WIFSIGNALED(status)) {
            const std::string message = "subprocess terminated by signal " +
                std::to_string(WTERMSIG(status)) + ": " + arguments.front();
            ASCENDLOGD("%s", message.c_str());
            throw std::runtime_error(message);
        }
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
            const int exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
            const std::string message = "subprocess exited with code " +
                std::to_string(exitCode) + ": " + arguments.front();
            ASCENDLOGD("%s", message.c_str());
            throw std::runtime_error(message);
        }
        ASCENDLOGD("Subprocess pid=%d completed successfully", static_cast<int>(child));
    }
};

class MakeBundleBuilder final {
public:
    MakeBundleBuilder(fs::path workDir, fs::path output, uint32_t jobs) noexcept
        : workDir_(std::move(workDir)), output_(std::move(output)), jobs_(jobs)
    {
    }

    void Build() const
    {
        const fs::path make = ExecutableLocator::FindRequired("make");
        const std::string compiler = ExecutableLocator::FindCompiler();
        const fs::path includeDir = CannIncludeDirectory();
        ASCENDLOGD("Building bundle with make=%s compiler=%s include=%s jobs=%u", make.c_str(),
                 compiler.c_str(), includeDir.c_str(), jobs_);
        ProcessRunner::Run(
            {make.string(), "-C", workDir_.string(), "-f", "Makefile", "-B",
             "-j" + std::to_string(jobs_)},
            {{"ASCENDC_BUNDLE_CXX", compiler},
             {"ASCENDC_BUNDLE_INCLUDE_DIR", includeDir.string()},
             {"ASCENDC_BUNDLE_OUTPUT", output_.string()}});
        ASCENDLOGI("Built staged compile resource bundle %s", output_.c_str());
    }

private:
    static fs::path CannIncludeDirectory()
    {
        const char *cannHome = std::getenv("ASCEND_HOME_PATH");
        if (cannHome == nullptr || cannHome[0] == '\0') {
            throw std::runtime_error("ASCEND_HOME_PATH is not configured");
        }
        const fs::path includeDir = fs::path(cannHome) / "include";
        const fs::path abiHeader = includeDir / "ascendc_compile_resource/ascendc_manifest_abi.h";
        std::error_code error;
        if (!fs::is_regular_file(abiHeader, error) || error) {
            throw std::runtime_error("CANN package include is missing ascendc_manifest_abi.h: " +
                                     includeDir.string());
        }
        const fs::path canonical = fs::canonical(includeDir, error);
        if (error || !fs::is_directory(canonical)) {
            throw std::runtime_error("failed to resolve CANN package include directory: " +
                                     includeDir.string());
        }
        return canonical;
    }

    fs::path workDir_;
    fs::path output_;
    uint32_t jobs_ = 1U;
};

class BundlePublisher final {
public:
    static void Prepare(const fs::path &output)
    {
        std::error_code error;
        fs::create_directories(output.parent_path(), error);
        if (error) {
            throw std::runtime_error("failed to create bundle output directory: " + error.message());
        }
        RejectSymlink(output);
    }

    static void Publish(const fs::path &staged, const fs::path &output)
    {
        RejectSymlink(output);
        std::error_code error;
        fs::rename(staged, output, error);
        if (!error) {
            ASCENDLOGI("Published compile resource bundle %s", output.c_str());
            return;
        }
        if (error != std::make_error_code(std::errc::cross_device_link)) {
            throw std::runtime_error("failed to publish bundle output: " + error.message());
        }
        PublishAcrossFileSystems(staged, output);
        ASCENDLOGI("Published compile resource bundle %s", output.c_str());
    }

private:
    static void RejectSymlink(const fs::path &output)
    {
        std::error_code error;
        const fs::file_status status = fs::symlink_status(output, error);
        if (error == std::make_error_code(std::errc::no_such_file_or_directory)) {
            return;
        }
        if (error) {
            throw std::runtime_error("failed to inspect bundle output path: " + error.message());
        }
        if (fs::is_symlink(status)) {
            throw std::runtime_error("bundle output must not be a symlink");
        }
    }

    static void PublishAcrossFileSystems(const fs::path &staged, const fs::path &output)
    {
        ASCENDLOGD("Copying staged bundle across filesystems: %s -> %s", staged.c_str(),
                 output.c_str());
        BuildWorkspace publishWorkspace = BuildWorkspace::Create(output.parent_path(), false);
        const fs::path copied = publishWorkspace.Session() / "compile_resource_bundle.so";
        std::error_code error;
        fs::copy_file(staged, copied, error);
        if (error) {
            throw std::runtime_error("failed to copy staged bundle output: " + error.message());
        }

        const fs::file_status stagedStatus = fs::status(staged, error);
        if (error) {
            throw std::runtime_error("failed to inspect staged bundle permissions: " +
                                     error.message());
        }
        fs::permissions(copied, stagedStatus.permissions(), fs::perm_options::replace, error);
        if (error) {
            throw std::runtime_error("failed to preserve staged bundle permissions: " +
                                     error.message());
        }

        RejectSymlink(output);
        fs::rename(copied, output, error);
        if (error) {
            throw std::runtime_error("failed to publish copied bundle output: " + error.message());
        }
    }
};

uint32_t ResolveJobCount(uint32_t requested) noexcept
{
    if (requested != 0U) {
        return requested;
    }
    const uint32_t cpuCount = static_cast<uint32_t>(std::thread::hardware_concurrency());
    return cpuCount < 2U ? 1U : cpuCount / 2U;
}

} // namespace

ManifestBundleCompiler::ManifestBundleCompiler(BuildCollectedBundleRequest request) noexcept
    : request_(std::move(request))
{
}

void ManifestBundleCompiler::Compile() const
{
    ASCENDLOGI("Starting bundle generation: package_id=%s manifest_root=%s output=%s keep_temp=%s",
             request_.packageId.c_str(), request_.manifestSearchRoot.c_str(),
             request_.outputSo.c_str(), request_.keepTemp ? "true" : "false");
    if (request_.packageId.empty()) {
        throw std::runtime_error("bundle package ID must not be empty");
    }
    std::error_code error;
    const fs::path manifestSearchRoot = fs::canonical(request_.manifestSearchRoot, error);
    if (error || !fs::is_directory(manifestSearchRoot)) {
        throw std::runtime_error("manifest search root is not a directory");
    }
    if (request_.outputSo.empty()) {
        throw std::runtime_error("bundle output path must not be empty");
    }
    if (!request_.outputSo.has_parent_path()) {
        throw std::runtime_error("bundle output path must include a parent directory");
    }
    const fs::path output = fs::absolute(request_.outputSo, error).lexically_normal();
    if (error) {
        throw std::runtime_error("failed to resolve bundle output path: " + error.message());
    }
    BundlePublisher::Prepare(output);
    const uint32_t jobs = ResolveJobCount(request_.jobs);
    ASCENDLOGI("Using %u parallel bundle build job(s)", jobs);

    const std::vector<ManifestUnit> units =
        CollectedManifestRepository(manifestSearchRoot).Load();
    BuildWorkspace workspace = BuildWorkspace::Create(request_.workDir, request_.keepTemp);
    if (request_.keepTemp) {
        std::cerr << "asc_compile_exporter temporary directory: " << workspace.Session() << '\n';
        ASCENDLOGI("Keeping temporary bundle directory %s", workspace.Session().c_str());
    }

    BundleSourceGenerator(workspace.Session(), request_.packageId, units).Generate();
    const fs::path stagedOutput = workspace.Session() / "compile_resource_bundle.so";
    MakeBundleBuilder(workspace.Session(), stagedOutput, jobs).Build();
    BundlePublisher::Publish(stagedOutput, output);
    ASCENDLOGI("Bundle generation completed successfully: %s", output.c_str());
}

} // namespace ascendc::manifest_generator
