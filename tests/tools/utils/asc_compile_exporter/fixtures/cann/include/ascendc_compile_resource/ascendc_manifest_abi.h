#ifndef ASCENDC_MANIFEST_ABI_H
#define ASCENDC_MANIFEST_ABI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AC_COMPILE_RESOURCE_MAGIC 0x41435253u
#define AC_COMPILE_RESOURCE_ABI_VERSION 1u

typedef struct AcString {
    const char *data;
    uint64_t size;
} AcString;

typedef struct AcCompileResourceExtension {
    uint32_t type;
    uint32_t version;
    uint64_t flags;
    const void *data;
    uint64_t data_size;
} AcCompileResourceExtension;

typedef struct AcCompileResourceFile {
    AcString file_name;
    AcString file_path;
    const uint8_t *data;
    uint64_t size;
} AcCompileResourceFile;

typedef struct AcCompileResourceManifest {
    AcString json;
    const AcCompileResourceFile *files;
    uint64_t file_count;
} AcCompileResourceManifest;

typedef struct AcCompileResourceBundleHeader {
    uint32_t magic;
    uint32_t abi_version;
    uint32_t struct_size;
    uint32_t flags;
} AcCompileResourceBundleHeader;

typedef struct AcCompileResourceBundle {
    AcCompileResourceBundleHeader header;
    AcString package_id;
    const AcCompileResourceManifest *manifests;
    uint64_t manifest_count;
    const AcCompileResourceExtension *extensions;
    uint64_t extension_count;
} AcCompileResourceBundle;

typedef const AcCompileResourceBundleHeader *(*AscendcGetCompileResourceBundleFn)(void);

#ifdef __cplusplus
}
#endif

#endif // ASCENDC_MANIFEST_ABI_H
