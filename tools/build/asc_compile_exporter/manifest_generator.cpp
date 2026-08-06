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
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <limits>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>
#include <vector>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ascendc_tool_log.h"
#include "nlohmann/json.hpp"

using Json = nlohmann::json;

namespace ascendc {
namespace manifest_generator {
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

std::string JoinPath(const std::string &left, const std::string &right)
{
    if (left.empty()) {
        return right;
    }
    if (right.empty()) {
        return left;
    }
    return left[left.size() - 1U] == '/' ? left + right : left + "/" + right;
}

std::string ParentPath(const std::string &path)
{
    const size_t separator = path.find_last_of('/');
    if (separator == std::string::npos) {
        return std::string();
    }
    return separator == 0U ? "/" : path.substr(0U, separator);
}

std::string FileName(const std::string &path)
{
    if (path.empty() || path[path.size() - 1U] == '/') {
        return std::string();
    }
    const size_t separator = path.find_last_of('/');
    return separator == std::string::npos ? path : path.substr(separator + 1U);
}

std::string CurrentDirectory()
{
    size_t size = 256U;
    while (size <= 1024U * 1024U) {
        std::vector<char> buffer(size);
        if (getcwd(buffer.data(), buffer.size()) != nullptr) {
            return std::string(buffer.data());
        }
        if (errno != ERANGE) {
            throw SystemError("failed to get current directory");
        }
        size *= 2U;
    }
    throw std::runtime_error("current directory path is too long");
}

std::string AbsolutePath(const std::string &path)
{
    return !path.empty() && path[0] == '/' ? path : JoinPath(CurrentDirectory(), path);
}

bool ResolveCanonicalPath(const std::string &path, std::string &resolved)
{
    char resolvedPath[PATH_MAX] = {0};
    if (realpath(path.c_str(), resolvedPath) == nullptr) {
        return false;
    }
    resolved = std::string(resolvedPath);
    return true;
}

bool IsDirectory(const std::string &path)
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
}

bool IsWithin(const std::string &path, const std::string &root)
{
    if (path == root) {
        return true;
    }
    if (root == "/") {
        return !path.empty() && path[0] == '/';
    }
    return path.size() > root.size() && path.compare(0U, root.size(), root) == 0 && path[root.size()] == '/';
}

std::string CanonicalDirectory(const std::string &path, const std::string &field)
{
    struct stat inputInfo {};
    if (lstat(path.c_str(), &inputInfo) != 0 || S_ISLNK(inputInfo.st_mode)) {
        throw std::runtime_error(field + " must exist and must not be a symlink: " + path);
    }
    std::string canonical;
    if (!ResolveCanonicalPath(path, canonical)) {
        throw SystemError("failed to resolve " + field);
    }
    if (!IsDirectory(canonical)) {
        throw std::runtime_error(field + " is not a directory: " + path);
    }
    return canonical;
}

std::string CanonicalDirectoryWithin(const std::string &path, const std::string &root,
                                     const std::string &field)
{
    const std::string canonical = CanonicalDirectory(path, field);
    if (!IsWithin(canonical, root)) {
        throw std::runtime_error(field + " is not a directory below the manifest: " + path);
    }
    return canonical;
}

void EnsureDirectory(const std::string &path)
{
    if (path.empty() || path == "/") {
        return;
    }
    if (mkdir(path.c_str(), 0700) == 0) {
        return;
    }
    const int mkdirError = errno;
    if (mkdirError == EEXIST && IsDirectory(path)) {
        return;
    }
    throw std::runtime_error("failed to create directory " + path + ": " + std::strerror(mkdirError));
}

void CreateDirectories(const std::string &path)
{
    const std::string absolute = AbsolutePath(path);
    size_t begin = 1U;
    while (begin < absolute.size()) {
        const size_t separator = absolute.find('/', begin);
        EnsureDirectory(absolute.substr(0U, separator));
        if (separator == std::string::npos) {
            break;
        }
        begin = separator + 1U;
    }
}

void RemoveTree(const std::string &path) noexcept
{
    struct stat info {};
    if (lstat(path.c_str(), &info) != 0) {
        return;
    }
    if (!S_ISDIR(info.st_mode) || S_ISLNK(info.st_mode)) {
        (void)unlink(path.c_str());
        return;
    }
    DIR *directory = opendir(path.c_str());
    if (directory != nullptr) {
        while (dirent *entry = readdir(directory)) {
            const std::string name(entry->d_name);
            if (name != "." && name != "..") {
                RemoveTree(JoinPath(path, name));
            }
        }
        (void)closedir(directory);
    }
    (void)rmdir(path.c_str());
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

std::vector<uint8_t> ReadRegularFile(const std::string &path, uintmax_t maximum,
                                     const std::string &field)
{
    const int descriptor = open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (descriptor < 0) {
        throw SystemError("failed to open " + field + " " + path);
    }
    struct stat info {};
    if (fstat(descriptor, &info) != 0 || !S_ISREG(info.st_mode) || info.st_size < 0 ||
        static_cast<uintmax_t>(info.st_size) > maximum ||
        static_cast<uintmax_t>(info.st_size) > static_cast<uintmax_t>(std::numeric_limits<size_t>::max())) {
        (void)close(descriptor);
        throw std::runtime_error(field + " is not a permitted regular file: " + path);
    }
    std::vector<uint8_t> data(static_cast<size_t>(info.st_size));
    size_t offset = 0U;
    while (offset < data.size()) {
        const ssize_t count = read(descriptor, data.data() + offset, data.size() - offset);
        if (count < 0 && errno == EINTR) {
            continue;
        }
        if (count <= 0) {
            (void)close(descriptor);
            throw std::runtime_error(field + " changed while being read: " + path);
        }
        offset += static_cast<size_t>(count);
    }
    uint8_t extra = 0U;
    ssize_t extraCount = -1;
    do {
        extraCount = read(descriptor, &extra, 1U);
    } while (extraCount < 0 && errno == EINTR);
    const int readError = errno;
    (void)close(descriptor);
    if (extraCount > 0) {
        throw std::runtime_error(field + " grew while being read: " + path);
    }
    if (extraCount < 0) {
        throw std::runtime_error("failed to read " + field + ": " + std::strerror(readError));
    }
    return data;
}

std::ofstream OpenGeneratedFile(const std::string &path)
{
    const int descriptor = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
    if (descriptor < 0) {
        throw SystemError("failed to create generated file " + path);
    }
    (void)close(descriptor);
    std::ofstream output(path.c_str(), std::ios::trunc);
    if (!output.is_open()) {
        (void)unlink(path.c_str());
        throw std::runtime_error("failed to open generated file: " + path);
    }
    return output;
}

void FinishOutput(std::ofstream &output, const std::string &path)
{
    output.flush();
    if (!output) {
        throw std::runtime_error("failed to write generated file: " + path);
    }
    output.close();
    if (!output) {
        throw std::runtime_error("failed to close generated file: " + path);
    }
}

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
    explicit CollectedManifestRepository(std::string manifestSearchRoot) noexcept
        : manifestSearchRoot_(std::move(manifestSearchRoot))
    {
    }

    std::vector<ManifestUnit> Load() const
    {
        ASCENDLOGD("Discovering manifests under %s", manifestSearchRoot_.c_str());
        ResourceBudget budget;
        const std::vector<std::string> manifests = DiscoverManifestFiles(manifestSearchRoot_);
        ASCENDLOGI("Discovered %zu manifest(s)", manifests.size());
        std::vector<ManifestUnit> units;
        units.reserve(manifests.size());
        for (const std::string &manifest : manifests) {
            units.push_back(LoadUnit(manifest, budget));
        }
        return units;
    }

private:
    static std::string ReadBaseDir(const std::string &text, const std::string &manifestPath)
    {
        Json manifest;
        try {
            manifest = Json::parse(text);
        } catch (const Json::parse_error &error) {
            throw std::runtime_error("manifest is not valid JSON: " + manifestPath +
                                     ": " + error.what());
        }

        try {
            return manifest.at("base_dir").get<std::string>();
        } catch (const Json::exception &error) {
            throw std::runtime_error("manifest base_dir is unavailable: " +
                                     manifestPath + ": " + error.what());
        }
    }

    static bool IsManifestFile(const std::string &path)
    {
        constexpr const char *suffix = "_manifest.json";
        const std::string name = FileName(path);
        return name.size() > std::strlen(suffix) &&
            name.compare(name.size() - std::strlen(suffix), std::strlen(suffix), suffix) == 0;
    }

    static void DiscoverManifestFilesBelow(const std::string &directory,
                                           std::vector<std::string> &manifests)
    {
        DIR *stream = opendir(directory.c_str());
        if (stream == nullptr) {
            throw SystemError("failed to enumerate " + directory);
        }
        try {
            while (dirent *entry = readdir(stream)) {
                const std::string name(entry->d_name);
                if (name == "." || name == "..") {
                    continue;
                }
                const std::string path = JoinPath(directory, name);
                struct stat info {};
                if (lstat(path.c_str(), &info) != 0) {
                    throw SystemError("failed to inspect collection entry " + path);
                }
                if (S_ISLNK(info.st_mode)) {
                    continue;
                }
                if (S_ISDIR(info.st_mode)) {
                    DiscoverManifestFilesBelow(path, manifests);
                    continue;
                }
                if (!S_ISREG(info.st_mode) || !IsManifestFile(path)) {
                    continue;
                }
                if (manifests.size() >= kMaxManifestCount) {
                    throw std::runtime_error("collection exceeds manifest count limit of " +
                                             std::to_string(kMaxManifestCount));
                }
                manifests.push_back(path);
            }
        } catch (...) {
            (void)closedir(stream);
            throw;
        }
        (void)closedir(stream);
    }

    static std::vector<std::string> DiscoverManifestFiles(const std::string &manifestSearchRoot)
    {
        std::vector<std::string> manifests;
        DiscoverManifestFilesBelow(manifestSearchRoot, manifests);
        std::sort(manifests.begin(), manifests.end());
        if (manifests.empty()) {
            throw std::runtime_error("collected resources contain no manifest units");
        }
        return manifests;
    }

    static void LoadResourceFiles(const std::string &directory, const std::string &manifestRoot,
                                  const std::string &manifestPath, ResourceBudget &budget,
                                  ManifestUnit &unit)
    {
        DIR *stream = opendir(directory.c_str());
        if (stream == nullptr) {
            throw SystemError("failed to enumerate manifest base_dir " + directory);
        }
        try {
            while (dirent *entry = readdir(stream)) {
                const std::string name(entry->d_name);
                if (name == "." || name == "..") {
                    continue;
                }
                const std::string source = JoinPath(directory, name);
                struct stat info {};
                if (lstat(source.c_str(), &info) != 0) {
                    throw SystemError("failed to inspect manifest resource " + source);
                }
                if (S_ISLNK(info.st_mode)) {
                    throw std::runtime_error("manifest base_dir contains a symlink: " + source);
                }
                if (S_ISDIR(info.st_mode)) {
                    LoadResourceFiles(source, manifestRoot, manifestPath, budget, unit);
                    continue;
                }
                if (!S_ISREG(info.st_mode)) {
                    throw std::runtime_error("manifest base_dir contains a non-regular file: " + source);
                }
                const std::vector<uint8_t> data =
                    ReadRegularFile(source, kMaxResourceFileBytes, "manifest resource");
                if (data.empty()) {
                    ASCENDLOGD("Skipping empty manifest resource %s", source.c_str());
                    continue;
                }
                if (unit.files.size() >= kMaxFilesPerManifest) {
                    throw std::runtime_error("manifest exceeds resource file count limit: " + manifestPath);
                }
                budget.AddResource(data.size(), "manifest resource " + source);
                const std::string filePath = source.substr(manifestRoot.size() + 1U);
                ResourceFile file;
                file.fileName = name;
                file.filePath = filePath;
                file.data = data;
                unit.files.push_back(std::move(file));
            }
        } catch (...) {
            (void)closedir(stream);
            throw;
        }
        (void)closedir(stream);
    }

    static ManifestUnit LoadUnit(const std::string &manifestPath, ResourceBudget &budget)
    {
        ASCENDLOGD("Loading manifest %s", manifestPath.c_str());
        ManifestUnit unit;
        const std::vector<uint8_t> manifestBytes = ReadRegularFile(manifestPath, kMaxManifestBytes, "manifest");
        budget.AddPayload(manifestBytes.size(), "manifest " + manifestPath);
        unit.json.assign(manifestBytes.begin(), manifestBytes.end());
        const std::string baseDir = ReadBaseDir(unit.json, manifestPath);

        const std::string manifestRoot = ParentPath(manifestPath);
        const std::string resourceInput = !baseDir.empty() && baseDir[0] == '/' ?
            baseDir : JoinPath(manifestRoot, baseDir);
        const std::string resourceRoot =
            CanonicalDirectoryWithin(resourceInput, manifestRoot, "manifest base_dir");
        LoadResourceFiles(resourceRoot, manifestRoot, manifestPath, budget, unit);
        std::sort(unit.files.begin(), unit.files.end(),
                  [](const ResourceFile &left, const ResourceFile &right) {
                      return left.filePath < right.filePath;
                  });
        ASCENDLOGD("Loaded manifest %s with %zu resource file(s)", manifestPath.c_str(),
                 unit.files.size());
        return unit;
    }

    std::string manifestSearchRoot_;
};

class BundleSourceGenerator final {
public:
    BundleSourceGenerator(std::string workDir, std::string packageId,
                          const std::vector<ManifestUnit> &units) noexcept
        : workDir_(std::move(workDir)), packageId_(std::move(packageId)), units_(units)
    {
    }

    void Generate() const
    {
        ASCENDLOGD("Generating bundle sources in %s", workDir_.c_str());
        const std::string bundleSource = JoinPath(workDir_, "compile_resource_bundle.cpp");
        ASCENDLOGD("Generating bundle source %s", bundleSource.c_str());
        EmitBundleSource(bundleSource);
        for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
            const std::string manifestSource =
                JoinPath(workDir_, "manifest_" + std::to_string(unitIndex) + ".cpp");
            ASCENDLOGD("Generating manifest source %s", manifestSource.c_str());
            EmitManifestSource(manifestSource, unitIndex, units_[unitIndex]);
        }
        const std::string makefile = JoinPath(workDir_, "Makefile");
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

    static void EmitManifestSource(const std::string &path, size_t unitIndex, const ManifestUnit &unit)
    {
        std::ofstream output = OpenGeneratedFile(path);
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
        output << "} // namespace\n\nnamespace ascendc {\nnamespace resource {\nnamespace generated {\n"
               << "AcCompileResourceManifest GetManifest" << unitIndex << "()\n{\n"
               << "    return { " << prefix << "_json, "
               << (unit.files.empty() ? "nullptr" : prefix + "_files") << ", "
               << unit.files.size() << "ULL };\n}\n} // namespace generated\n"
                  "} // namespace resource\n} // namespace ascendc\n";
        FinishOutput(output, path);
    }

    void EmitBundleSource(const std::string &path) const
    {
        std::ofstream output = OpenGeneratedFile(path);
        output << "#include \"ascendc_compile_resource/ascendc_manifest_abi.h\"\n\n"
                  "namespace ascendc {\nnamespace resource {\nnamespace generated {\n";
        for (size_t unitIndex = 0; unitIndex < units_.size(); ++unitIndex) {
            output << "AcCompileResourceManifest GetManifest" << unitIndex << "();\n";
        }
        output << "} // namespace generated\n} // namespace resource\n} // namespace ascendc\n\nnamespace {\n";
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
        FinishOutput(output, path);
    }

    void EmitMakefile(const std::string &path) const
    {
        std::ofstream output = OpenGeneratedFile(path);
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
                  "\t\"$${ASCENDC_BUNDLE_CXX}\" -std=c++11 -fPIC -fvisibility=hidden -O2 "
                  "-I\"$${ASCENDC_BUNDLE_INCLUDE_DIR}\" -c $< -o $@\n";
        FinishOutput(output, path);
    }

    std::string workDir_;
    std::string packageId_;
    const std::vector<ManifestUnit> &units_;
};

class BuildWorkspace final {
public:
    static BuildWorkspace Create(const std::string &requestedParent, bool keepTemp)
    {
        const char *temporary = std::getenv("TMPDIR");
        const std::string requested = requestedParent.empty() ?
            (temporary == nullptr || temporary[0] == '\0' ? "/tmp" : temporary) : requestedParent;
        const std::string absolute = AbsolutePath(requested);
        CreateDirectories(absolute);
        const std::string parent = CanonicalDirectory(absolute, "bundle work directory");
        std::string pathTemplate = JoinPath(parent, ".manifest-generator-XXXXXX");
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
            RemoveTree(session_);
            ASCENDLOGD("Removed private bundle work directory %s", session_.c_str());
        }
    }

    const std::string &Session() const noexcept
    {
        return session_;
    }

private:
    BuildWorkspace(std::string session, bool keepTemp) noexcept
        : session_(std::move(session)), keepTemp_(keepTemp)
    {
    }

    std::string session_;
    bool keepTemp_ = false;
};

class ExecutableLocator final {
public:
    static std::string FindCompiler()
    {
        const char *configured = std::getenv("CXX");
        if (configured != nullptr && configured[0] != '\0') {
            const std::string found = Find(configured);
            if (found.empty()) {
                throw std::runtime_error("CXX does not resolve to an executable compiler: " +
                                         std::string(configured));
            }
            return found;
        }
        for (const char *candidate : {"c++", "g++"}) {
            const std::string found = Find(candidate);
            if (!found.empty()) {
                return found;
            }
        }
        throw std::runtime_error("no C++ compiler found in PATH; set CXX to an executable compiler");
    }

    static std::string FindRequired(const std::string &name)
    {
        const std::string found = Find(name);
        if (found.empty()) {
            throw std::runtime_error("executable not found in PATH: " + name);
        }
        return found;
    }

private:
    static std::string ExecutablePath(const std::string &candidate)
    {
        std::string canonical;
        if (!ResolveCanonicalPath(candidate, canonical)) {
            return std::string();
        }
        struct stat info {};
        if (stat(canonical.c_str(), &info) != 0 || !S_ISREG(info.st_mode) || access(canonical.c_str(), X_OK) != 0) {
            return std::string();
        }
        return canonical;
    }

    static std::string Find(const std::string &name)
    {
        if (name.find('/') != std::string::npos) {
            return ExecutablePath(name);
        }
        const char *pathEnvironment = std::getenv("PATH");
        const std::string searchPath = pathEnvironment == nullptr ? "/usr/bin:/bin" : pathEnvironment;
        size_t begin = 0U;
        while (begin <= searchPath.size()) {
            const size_t end = searchPath.find(':', begin);
            const std::string directory = searchPath.substr(
                begin, end == std::string::npos ? std::string::npos : end - begin);
            const std::string found = ExecutablePath(JoinPath(directory.empty() ? "." : directory, name));
            if (!found.empty()) {
                return found;
            }
            if (end == std::string::npos) {
                break;
            }
            begin = end + 1U;
        }
        return std::string();
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
    MakeBundleBuilder(std::string workDir, std::string output, uint32_t jobs) noexcept
        : workDir_(std::move(workDir)), output_(std::move(output)), jobs_(jobs)
    {
    }

    void Build() const
    {
        const std::string make = ExecutableLocator::FindRequired("make");
        const std::string compiler = ExecutableLocator::FindCompiler();
        const std::string includeDir = CannIncludeDirectory();
        ASCENDLOGD("Building bundle with make=%s compiler=%s include=%s jobs=%u", make.c_str(),
                 compiler.c_str(), includeDir.c_str(), jobs_);
        ProcessRunner::Run(
            {make, "-C", workDir_, "-f", "Makefile", "-B",
             "-j" + std::to_string(jobs_)},
            {{"ASCENDC_BUNDLE_CXX", compiler},
             {"ASCENDC_BUNDLE_INCLUDE_DIR", includeDir},
             {"ASCENDC_BUNDLE_OUTPUT", output_}});
        ASCENDLOGI("Built staged compile resource bundle %s", output_.c_str());
    }

private:
    static std::string CannIncludeDirectory()
    {
        const char *cannHome = std::getenv("ASCEND_HOME_PATH");
        if (cannHome == nullptr || cannHome[0] == '\0') {
            throw std::runtime_error("ASCEND_HOME_PATH is not configured");
        }
        const std::string includeDir = JoinPath(cannHome, "include");
        const std::string abiHeader =
            JoinPath(includeDir, "ascendc_compile_resource/ascendc_manifest_abi.h");
        struct stat info {};
        if (lstat(abiHeader.c_str(), &info) != 0 || !S_ISREG(info.st_mode)) {
            throw std::runtime_error("CANN package include is missing ascendc_manifest_abi.h: " +
                                     includeDir);
        }
        return CanonicalDirectory(includeDir, "CANN package include directory");
    }

    std::string workDir_;
    std::string output_;
    uint32_t jobs_ = 1U;
};

class BundlePublisher final {
public:
    static void Prepare(const std::string &output)
    {
        RejectSymlink(output);
    }

    static void Publish(const std::string &staged, const std::string &output)
    {
        RejectSymlink(output);
        if (rename(staged.c_str(), output.c_str()) == 0) {
            ASCENDLOGI("Published compile resource bundle %s", output.c_str());
            return;
        }
        if (errno != EXDEV) {
            throw SystemError("failed to publish bundle output");
        }
        PublishAcrossFileSystems(staged, output);
        ASCENDLOGI("Published compile resource bundle %s", output.c_str());
    }

private:
    static void RejectSymlink(const std::string &output)
    {
        struct stat info {};
        if (lstat(output.c_str(), &info) != 0) {
            if (errno == ENOENT) {
                return;
            }
            throw SystemError("failed to inspect bundle output path");
        }
        if (S_ISLNK(info.st_mode)) {
            throw std::runtime_error("bundle output must not be a symlink");
        }
    }

    static void CopyRegularFile(const std::string &source, const std::string &destination)
    {
        const int input = open(source.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
        if (input < 0) {
            throw SystemError("failed to open staged bundle output");
        }
        struct stat info {};
        if (fstat(input, &info) != 0 || !S_ISREG(info.st_mode)) {
            (void)close(input);
            throw std::runtime_error("staged bundle output is not a regular file");
        }
        const int output = open(destination.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC,
                                info.st_mode & 0777);
        if (output < 0) {
            (void)close(input);
            throw SystemError("failed to create copied bundle output");
        }
        try {
            std::vector<uint8_t> buffer(64U * 1024U);
            while (true) {
                const ssize_t count = read(input, buffer.data(), buffer.size());
                if (count < 0 && errno == EINTR) {
                    continue;
                }
                if (count < 0) {
                    throw SystemError("failed to read staged bundle output");
                }
                if (count == 0) {
                    break;
                }
                size_t offset = 0U;
                while (offset < static_cast<size_t>(count)) {
                    const ssize_t written = write(output, buffer.data() + offset,
                                                  static_cast<size_t>(count) - offset);
                    if (written < 0 && errno == EINTR) {
                        continue;
                    }
                    if (written <= 0) {
                        throw SystemError("failed to write copied bundle output");
                    }
                    offset += static_cast<size_t>(written);
                }
            }
        } catch (...) {
            (void)close(input);
            (void)close(output);
            (void)unlink(destination.c_str());
            throw;
        }
        (void)close(input);
        if (close(output) != 0) {
            (void)unlink(destination.c_str());
            throw SystemError("failed to close copied bundle output");
        }
    }

    static void PublishAcrossFileSystems(const std::string &staged, const std::string &output)
    {
        ASCENDLOGD("Copying staged bundle across filesystems: %s -> %s", staged.c_str(),
                 output.c_str());
        BuildWorkspace publishWorkspace = BuildWorkspace::Create(ParentPath(output), false);
        const std::string copied = JoinPath(publishWorkspace.Session(), "compile_resource_bundle.so");
        CopyRegularFile(staged, copied);
        RejectSymlink(output);
        if (rename(copied.c_str(), output.c_str()) != 0) {
            throw SystemError("failed to publish copied bundle output");
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
    const std::string manifestSearchRoot =
        CanonicalDirectory(request_.manifestSearchRoot, "manifest search root");
    if (request_.outputSo.empty()) {
        throw std::runtime_error("bundle output path must not be empty");
    }
    const std::string outputParent = ParentPath(request_.outputSo);
    const std::string outputName = FileName(request_.outputSo);
    if (outputParent.empty()) {
        throw std::runtime_error("bundle output path must include a parent directory");
    }
    if (outputName.empty() || outputName == "." || outputName == "..") {
        throw std::runtime_error("bundle output path has an invalid file name");
    }
    const std::string absoluteOutputParent = AbsolutePath(outputParent);
    CreateDirectories(absoluteOutputParent);
    const std::string output = JoinPath(
        CanonicalDirectory(absoluteOutputParent, "bundle output directory"), outputName);
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
    const std::string stagedOutput = JoinPath(workspace.Session(), "compile_resource_bundle.so");
    MakeBundleBuilder(workspace.Session(), stagedOutput, jobs).Build();
    BundlePublisher::Publish(stagedOutput, output);
    ASCENDLOGI("Bundle generation completed successfully: %s", output.c_str());
}

} // namespace manifest_generator
} // namespace ascendc
