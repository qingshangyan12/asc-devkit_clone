/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file exception_dump_public.cpp
 * \brief Exception Dump 外挂回调库实现：注册异常回调，在 AICore 任务失败时自动保存
 * kernel args 区与全部登记输入（仅依赖 CANN 公开接口，算子零改动）
 */

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <mutex>
#include <string>
#include <chrono>
#include <fstream>
#include <unistd.h>

#include "acl/acl.h"
#include "acl/acl_rt.h"

#include "exception_dump_public.h"

static const int MAX_INPUTS = 64;
static const int MAX_NAME_LEN = 128;
static const int MAX_SHAPE_LEN = 256;
static const int MAX_DTYPE_LEN = 64;
static const char* OUTPUT_SUBDIR = "task_fail_dump";

struct InputInfo {
    char name[MAX_NAME_LEN];
    size_t ptrIndex;
    size_t size;
    char shape[MAX_SHAPE_LEN];
    char dtype[MAX_DTYPE_LEN];
};

static char g_kernelName[256] = {0};
static InputInfo g_inputs[MAX_INPUTS];
static int g_numInputs = 0;
static std::mutex g_mutex;
static bool g_binaryCbArmed = false; // per-binary 回调是否已挂载（回调内自动挂载完成标志）

// ============================================================================
// dump 核心逻辑（args D2H -> 按 ptrIndex 解析输入指针 -> 逐输入 D2H 落盘）
// ============================================================================

static void binary_exception_cb(aclrtExceptionInfo* exceptionInfo, void* userData); // 前置声明

static void save_bin(const char* filepath, const void* data, size_t size)
{
    std::ofstream ofs(filepath, std::ios::binary);
    if (!ofs.is_open()) {
        fprintf(stderr, "[ExcDumpPub] ERROR: failed to open %s\n", filepath);
        return;
    }
    ofs.write(reinterpret_cast<const char*>(data), size);
    ofs.close();
    fprintf(stderr, "[ExcDumpPub]   Saved %s (%zu bytes)\n", filepath, size);
}

static void save_info_txt(
    const char* filepath, uint32_t deviceid, uint32_t taskid, uint32_t streamid, uint32_t retcode,
    const char* kernelName, int saved_count)
{
    std::ofstream ofs(filepath);
    if (!ofs.is_open()) {
        fprintf(stderr, "[ExcDumpPub] ERROR: failed to open %s\n", filepath);
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    ofs << "# Task fail dump info (public API chain)\n";
    ofs << "# kernel: " << (kernelName ? kernelName : g_kernelName) << "\n";
    ofs << "# deviceId=" << deviceid << ", taskId=" << taskid << ", streamId=" << streamid << ", retcode=0x" << std::hex
        << retcode << std::dec << "\n";
    ofs << "# timestamp=" << (long long)ms << "\n";
    ofs << "# saved_inputs=" << saved_count << "/" << g_numInputs << "\n";
    ofs << "# binary_callback_armed=" << (g_binaryCbArmed ? 1 : 0) << "\n";
    ofs << "\n";

    for (int i = 0; i < g_numInputs; ++i) {
        const InputInfo& info = g_inputs[i];
        ofs << "[" << info.name << "]\n";
        ofs << "  ptrIndex = " << info.ptrIndex << "\n";
        ofs << "  shape    = " << info.shape << "\n";
        ofs << "  dtype    = " << info.dtype << "\n";
        ofs << "  size     = " << info.size << " bytes\n";
        ofs << "  file     = " << info.name << ".bin\n";
        ofs << "\n";
    }

    ofs.close();
    fprintf(stderr, "[ExcDumpPub]   Saved %s\n", filepath);
}

// kernel 名匹配：注册名以子串形式包含于上报名（兼容 Itanium mangled 名，如
// _Z21incre_flash_attentionILh0ELh1ELh0EE...）
static bool match_kernel_name(const char* reported)
{
    if (g_kernelName[0] == '\0')
        return true; // 未设置过滤名：不过滤
    if (reported == nullptr)
        return true; // 拿不到名：不过滤（保守放行）
    if (strncmp(reported, g_kernelName, strlen(g_kernelName)) == 0)
        return true;
    return strstr(reported, g_kernelName) != nullptr;
}

// 异常信息 -> dump
// 返回值: 0=已处理(匹配并完成dump), 1=不匹配跳过, 负值=错误
static int do_dump(aclrtExceptionInfo* exceptionInfo, const char* chainTag)
{
    if (!exceptionInfo)
        return -1;

    std::lock_guard<std::mutex> lock(g_mutex);

    uint32_t deviceid = aclrtGetDeviceIdFromExceptionInfo(exceptionInfo);
    uint32_t taskid = aclrtGetTaskIdFromExceptionInfo(exceptionInfo);
    uint32_t streamid = aclrtGetStreamIdFromExceptionInfo(exceptionInfo);
    uint32_t retcode = aclrtGetErrorCodeFromExceptionInfo(exceptionInfo);

    fprintf(
        stderr,
        "[ExcDumpPub] (%s) Task fail detected: deviceId=%u, taskId=%u, "
        "streamId=%u, retcode=0x%x\n",
        chainTag, deviceid, taskid, streamid, retcode);

    // ---- kernel 名匹配（公开链路: funcHandle -> aclrtGetFunctionName）----
    aclrtFuncHandle funcHandle = nullptr;
    bool hasFunc =
        (aclrtGetFuncHandleFromExceptionInfo(exceptionInfo, &funcHandle) == ACL_SUCCESS && funcHandle != nullptr);
    char knameBuf[512] = {0};
    const char* kname = nullptr;
    if (hasFunc && aclrtGetFunctionName(funcHandle, sizeof(knameBuf), knameBuf) == ACL_SUCCESS) {
        kname = knameBuf;
    }
    fprintf(stderr, "[ExcDumpPub] (%s) reported kernel: %s\n", chainTag, kname ? kname : "(unavailable)");
    if (!match_kernel_name(kname)) {
        fprintf(
            stderr, "[ExcDumpPub] (%s) Kernel name mismatch (got=%s, expected=%s), skipping\n", chainTag,
            kname ? kname : "(null)", g_kernelName);
        return 1;
    }

    // ---- 回调内自动 per-binary 挂载: funcHandle -> binHandle -> per-binary 回调（只挂一次）----
    if (hasFunc && !g_binaryCbArmed) {
        aclrtBinHandle binHandle = nullptr;
        if (aclrtFunctionGetBinary(funcHandle, &binHandle) == ACL_SUCCESS && binHandle != nullptr) {
            aclError rcb = aclrtBinarySetExceptionCallback(binHandle, binary_exception_cb, nullptr);
            if (rcb == ACL_SUCCESS) {
                g_binaryCbArmed = true;
                fprintf(
                    stderr,
                    "[ExcDumpPub] (%s) per-binary chain armed: funcHandle=%p -> "
                    "binHandle=%p -> BinarySetExceptionCallback OK\n",
                    chainTag, (void*)funcHandle, (void*)binHandle);
            } else {
                fprintf(
                    stderr,
                    "[ExcDumpPub] (%s) WARNING: BinarySetExceptionCallback ret=%d "
                    "(fallback to global chain)\n",
                    chainTag, (int)rcb);
            }
        } else {
            fprintf(
                stderr,
                "[ExcDumpPub] (%s) WARNING: FunctionGetBinary failed, "
                "fallback to global chain\n",
                chainTag);
        }
    }

    // ---- args 区获取（公开链路: aclrtGetArgsFromExceptionInfo）----
    void* devArgsPtr = nullptr;
    uint32_t devArgsLen = 0;
    aclError rArgs = aclrtGetArgsFromExceptionInfo(exceptionInfo, &devArgsPtr, &devArgsLen);
    if (rArgs != ACL_SUCCESS || devArgsPtr == nullptr || devArgsLen == 0) {
        fprintf(
            stderr,
            "[ExcDumpPub] (%s) ERROR: GetArgsFromExceptionInfo failed ret=%d "
            "ptr=%p len=%u\n",
            chainTag, (int)rArgs, devArgsPtr, devArgsLen);
        return -2;
    }
    fprintf(
        stderr, "[ExcDumpPub] (%s) devArgs: ptr=%p, len=%u, numInputs=%d\n", chainTag, devArgsPtr, devArgsLen,
        g_numInputs);

    if (g_numInputs == 0) {
        fprintf(stderr, "[ExcDumpPub] (%s) WARNING: no inputs registered, nothing to dump\n", chainTag);
        return -3;
    }

    // ---- dump 落盘 ----
    void* host_args = malloc(devArgsLen);
    if (!host_args)
        return -4;

    if (aclrtSetDevice(deviceid) != ACL_SUCCESS) {
        fprintf(stderr, "[ExcDumpPub] (%s) ERROR: aclrtSetDevice(%u) failed\n", chainTag, deviceid);
        free(host_args);
        return -5;
    }

    if (aclrtMemcpy(host_args, devArgsLen, devArgsPtr, devArgsLen, ACL_MEMCPY_DEVICE_TO_HOST) != ACL_SUCCESS) {
        fprintf(stderr, "[ExcDumpPub] (%s) ERROR: aclrtMemcpy args D2H failed\n", chainTag);
        aclrtResetDevice(deviceid);
        free(host_args);
        return -6;
    }

    // args 区前 40 个 8 字节槽原始值（探测日志，供 ptrIndex 校准）
    {
        uint32_t probeSlots = devArgsLen / 8 < 40 ? devArgsLen / 8 : 40;
        for (uint32_t i = 0; i < probeSlots; i++) {
            uint64_t v = 0;
            memcpy(&v, reinterpret_cast<uint8_t*>(host_args) + i * sizeof(uint64_t), sizeof(uint64_t));
            fprintf(stderr, "[ExcDumpPub]   probe args[%2u] = 0x%016lx\n", i, (unsigned long)v);
        }
    }

    char cwd[4096] = {0};
    if (getcwd(cwd, sizeof(cwd)) == nullptr) {
        snprintf(cwd, sizeof(cwd), "/tmp");
    }
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    char output_dir[4608];
    snprintf(
        output_dir, sizeof(output_dir), "%s/%s/dev%u_task%u_ts%lld", cwd, OUTPUT_SUBDIR, deviceid, taskid,
        (long long)ms);
    std::string mkdir_cmd = "mkdir -p " + std::string(output_dir);
    system(mkdir_cmd.c_str());
    fprintf(stderr, "[ExcDumpPub] (%s) Output dir: %s\n", chainTag, output_dir);

    { // args_raw.bin（含 tiling 结构体）
        char argsBin[4700];
        snprintf(argsBin, sizeof(argsBin), "%s/args_raw.bin", output_dir);
        FILE* fp = fopen(argsBin, "wb");
        if (fp) {
            fwrite(host_args, 1, devArgsLen, fp);
            fclose(fp);
        }
    }

    constexpr size_t PTR_SIZE = sizeof(void*);
    int saved_count = 0;
    for (int i = 0; i < g_numInputs; ++i) {
        const InputInfo& info = g_inputs[i];
        if (devArgsLen < (info.ptrIndex + 1) * PTR_SIZE) {
            fprintf(
                stderr, "[ExcDumpPub] WARNING: devArgsLen=%u too small for %s (index %zu), skip\n", devArgsLen,
                info.name, info.ptrIndex);
            continue;
        }
        uint64_t dev_addr = 0;
        memcpy(&dev_addr, reinterpret_cast<uint8_t*>(host_args) + info.ptrIndex * PTR_SIZE, PTR_SIZE);
        fprintf(
            stderr, "[ExcDumpPub]   [%d] %s: dev_addr=0x%016lx, size=%zu bytes\n", i, info.name,
            (unsigned long)dev_addr, info.size);
        if (dev_addr == 0)
            continue;

        void* host_buf = malloc(info.size);
        if (!host_buf)
            continue;
        if (aclrtMemcpy(host_buf, info.size, reinterpret_cast<void*>(dev_addr), info.size, ACL_MEMCPY_DEVICE_TO_HOST) !=
            ACL_SUCCESS) {
            fprintf(stderr, "[ExcDumpPub]   ERROR: aclrtMemcpy failed for %s\n", info.name);
            free(host_buf);
            continue;
        }
        char filepath[5120];
        snprintf(filepath, sizeof(filepath), "%s/%s.bin", output_dir, info.name);
        save_bin(filepath, host_buf, info.size);
        free(host_buf);
        ++saved_count;
    }

    char info_path[5120];
    snprintf(info_path, sizeof(info_path), "%s/inputs_info.txt", output_dir);
    save_info_txt(info_path, deviceid, taskid, streamid, retcode, kname, saved_count);

    fprintf(
        stderr, "[ExcDumpPub] (%s) Done. Saved %d/%d inputs to %s\n", chainTag, saved_count, g_numInputs, output_dir);

    free(host_args);
    aclrtResetDevice(deviceid);
    return 0;
}

// ============================================================================
// 两条回调链路
// ============================================================================

// per-binary 回调（回调内自动挂载后生效: binHandle -> BinarySetExceptionCallback）
static void binary_exception_cb(aclrtExceptionInfo* exceptionInfo, void* userData)
{
    (void)userData;
    do_dump(exceptionInfo, "binary-cb");
}

// 全局异常回调（引导链路: aclrtExceptionInfoCallbackRegister）
static void global_exception_cb(aclrtExceptionInfo* exceptionInfo) { do_dump(exceptionInfo, "global-cb"); }

// ============================================================================
// C API for Python (ctypes)
// ============================================================================

extern "C" int AddInputInfo(const char* name, size_t ptrIndex, size_t size, const char* shapeStr, const char* dtypeStr)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    if (g_numInputs >= MAX_INPUTS) {
        fprintf(stderr, "[ExcDumpPub] ERROR: max inputs (%d) reached\n", MAX_INPUTS);
        return -1;
    }
    InputInfo& info = g_inputs[g_numInputs];
    snprintf(info.name, MAX_NAME_LEN, "%s", name);
    info.ptrIndex = ptrIndex;
    info.size = size;
    snprintf(info.shape, MAX_SHAPE_LEN, "%s", shapeStr ? shapeStr : "");
    snprintf(info.dtype, MAX_DTYPE_LEN, "%s", dtypeStr ? dtypeStr : "");
    fprintf(
        stderr,
        "[ExcDumpPub] AddInputInfo[%d]: name=%s, ptrIndex=%zu, size=%zu, "
        "shape=%s, dtype=%s\n",
        g_numInputs, info.name, info.ptrIndex, info.size, info.shape, info.dtype);
    ++g_numInputs;
    return 0;
}

extern "C" void ClearInputInfo()
{
    std::lock_guard<std::mutex> lock(g_mutex);
    g_numInputs = 0;
    fprintf(stderr, "[ExcDumpPub] Cleared all input info\n");
}

// 引导注册（算子零改动主链路）: 异常回调经 aclrtExceptionInfoCallbackRegister 注册；
// 首次异常时在回调内自动完成 funcHandle -> binHandle -> BinarySetExceptionCallback 的
// per-binary 挂载，当前异常与后续异常均可保存现场
extern "C" int RegisterPublicExceptionDump(const char* kernelName)
{
    std::lock_guard<std::mutex> lock(g_mutex);
    snprintf(g_kernelName, sizeof(g_kernelName), "%s", kernelName ? kernelName : "");
    fprintf(stderr, "[ExcDumpPub] RegisterPublicExceptionDump:\n");
    fprintf(stderr, "  kernelName = %s\n", g_kernelName);
    fprintf(stderr, "  numInputs  = %d\n", g_numInputs);
    if (g_numInputs == 0) {
        fprintf(stderr, "[ExcDumpPub] WARNING: no inputs registered, callback will do nothing\n");
    }
    // 新接口支持按函数指针标识的多回调共存，与宿主框架注册的异常回调互不覆盖；
    // 旧接口 aclrtSetExceptionInfoCallback 已废弃，不再使用
    aclError ret = aclrtExceptionInfoCallbackRegister(global_exception_cb);
    if (ret != ACL_SUCCESS) {
        fprintf(stderr, "[ExcDumpPub] ERROR: aclrtExceptionInfoCallbackRegister failed: %d\n", (int)ret);
        return (int)ret;
    }
    fprintf(stderr, "[ExcDumpPub] Exception callback registered (bootstrap chain)\n");
    return 0;
}
