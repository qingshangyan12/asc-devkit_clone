#!/usr/bin/env python3

import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def raw_source(rel, kernel, api, aic_body, input_bytes, output_bytes, golden=""):
    binary = "c_api_cube_datamove_" + rel.replace("/", "_").replace(".asc", "")
    if not golden:
        golden = '''    for (uint32_t i = 0; i < OUTPUT_BYTES; ++i) golden[i] = input[i];'''
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/{binary} && /tmp/{binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t INPUT_BYTES = {input_bytes};
constexpr uint32_t OUTPUT_BYTES = {output_bytes};

__global__ __mix__(1, 2) void {kernel}(__gm__ uint8_t* output, __gm__ uint8_t* input)
{{
    asc_init();
    __cbuf__ uint8_t l1[256];
    __ubuf__ uint8_t readback[256];
    constexpr uint64_t intra_id = 2;
    if ASC_IS_AIC {{
{aic_body}
        asc_sync_intra_arrive(PIPE_MTE1, intra_id);
    }}
    if ASC_IS_AIV {{
        if (asc_get_sub_block_id() == 0) {{
            asc_sync_intra_wait(PIPE_MTE3, intra_id);
            asc_copy_ub2gm_align(output, readback, OUTPUT_BYTES);
            asc_sync_mte3(0);
        }}
    }}
}}

void PrintData(const char* label, const std::vector<uint8_t>& data)
{{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 16 && i < data.size(); ++i) std::cout << ' ' << +data[i];
    std::cout << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> input(INPUT_BYTES), output(OUTPUT_BYTES), golden(OUTPUT_BYTES);
    for (uint32_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i + 1);
{golden}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size(), output_device, output.size(), ACL_MEMCPY_DEVICE_TO_HOST);
    PrintData("Input", input);
    PrintData("Output", output);
    PrintData("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {api} demonstrated its data layout."
                         : "[Failed] {api} result mismatch.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def fill_source():
    rel = "asc_fill_l1.asc"
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/c_api_cube_datamove_asc_fill_l1 && /tmp/c_api_cube_datamove_asc_fill_l1
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t ELEMENTS = 64;

__global__ __mix__(1, 2) void AscFillL1Kernel(__gm__ float* output)
{{
    asc_init();
    __cbuf__ float l1[ELEMENTS];
    __ubuf__ float readback[ELEMENTS];
    constexpr uint64_t intra_id = 2;
    if ASC_IS_AIC {{
        asc_fill_value_config config{{}};
        config.repeat = 1;
        config.blk_num = ELEMENTS * sizeof(float) / 32;
        config.dst_gap = 0;
        // Fill 8 data blocks in L1 with the FP32 bit pattern for 1.0.
        asc_fill_l1(l1, 0x3f800000U, config);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12ub(readback, l1, false, 1, ELEMENTS * sizeof(float) / 32, 0, 0);
        asc_sync_intra_arrive(PIPE_MTE1, intra_id);
    }}
    if ASC_IS_AIV {{
        if (asc_get_sub_block_id() == 0) {{
            asc_sync_intra_wait(PIPE_MTE3, intra_id);
            asc_copy_ub2gm_align(output, readback, ELEMENTS * sizeof(float));
            asc_sync_mte3(0);
        }}
    }}
}}
}} // namespace

int main()
{{
    std::vector<float> output(ELEMENTS), golden(ELEMENTS, 1.0f);
    aclInit(nullptr);
    aclrtSetDevice(0);
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    AscFillL1Kernel<<<1, 0>>>(output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Filled L1 values:";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << output[i];
    std::cout << " ..." << std::endl;
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_fill_l1 filled L1 with 1.0."
                         : "[Failed] asc_fill_l1 result mismatch.") << std::endl;
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def matrix_source(rel, kernel, api, target):
    binary = "c_api_cube_datamove_" + rel.replace("/", "_").replace(".asc", "")
    b_is_nk = target == "l12l0b"
    if target == "nd2nz":
        load_a = '''    asc_set_gm2l1_nz_para(nz_config);
    // Convert the MxK ND matrix in GM to Nz layout in L1.
    asc_copy_gm2l1_nd2nz(a_l1, a, K, 0, M, K, 0, false);'''
    else:
        load_a = '''    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(a_l1, a, K, 0, M, K, 0, false);'''
    if b_is_nk:
        load_b = '''    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, b, K, 0, N, K, 0, false);'''
    else:
        load_b = '''    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, b, N, 0, K, N, 0, false);'''
    if target == "l12l0a":
        copy_a = '''    // Load one 16x32 Nz tile from L1 into L0A.
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);'''
    else:
        copy_a = '''    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);'''
    if target == "l12l0b":
        copy_b = '''    // Load one 16x32 Nz tile from L1 into L0B without transposition.
    asc_copy_l12l0b(b_l0, b_l1, 0, 0, 1, 1, 1, 1);'''
    elif target == "l12l0b_trans":
        copy_b = '''    // Load two K blocks into L0B and transpose them for MMAD.
    asc_copy_l12l0b_trans(b_l0, b_l1, static_cast<uint16_t>(0),
        static_cast<uint8_t>(1), static_cast<uint16_t>(2), static_cast<uint16_t>(1),
        static_cast<uint16_t>(0), static_cast<uint16_t>(0));'''
    else:
        copy_b = '''    asc_copy_l12l0b_trans(b_l0, b_l1, static_cast<uint16_t>(0),
        static_cast<uint8_t>(1), static_cast<uint16_t>(2), static_cast<uint16_t>(1),
        static_cast<uint16_t>(0), static_cast<uint16_t>(0));'''
    copy_out_call = '''output, c_l0, N, M, N, M, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false'''
    if target == "l0c2gm":
        copy_out = f'''    // Convert the 16x16 L0C result from Nz to contiguous ND in GM.
    asc_copy_l0c2gm({copy_out_call});'''
    else:
        copy_out = f'''    asc_copy_l0c2gm({copy_out_call});'''
    b_index = "col * K + k" if b_is_nk else "k * N + col"
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/{binary} && /tmp/{binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t M = 16, K = 32, N = 16, ELEMENTS = M * N;

__global__ __cube__ void {kernel}(__gm__ int8_t* a, __gm__ int8_t* b,
    __gm__ int32_t* output)
{{
    asc_init();
    __cbuf__ int8_t a_l1[M * K], b_l1[K * N];
    __ca__ int8_t a_l0[M * K];
    __cb__ int8_t b_l0[K * N];
    __cc__ int32_t c_l0[ELEMENTS];
    constexpr uint64_t nz_config = (32ULL << 32) | (1ULL << 16) | 1ULL;
{load_a}
{load_b}
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
{copy_a}
{copy_b}
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, 0, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
{copy_out}
    asc_sync_pipe(PIPE_FIX);
}}

template <typename T>
void PrintRow(const char* label, const std::vector<T>& data)
{{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << +data[i];
    std::cout << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<int8_t> a(M * K), b(K * N);
    std::vector<int32_t> output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t i = 0; i < M; ++i) a[i * K + i] = 1;
    for (uint32_t i = 0; i < N; ++i) b[{('i * K + i' if b_is_nk else 'i * N + i')}] = 2;
    for (uint32_t row = 0; row < M; ++row) {{
        for (uint32_t col = 0; col < N; ++col) {{
            for (uint32_t k = 0; k < K; ++k)
                golden[row * N + col] += a[row * K + k] * b[{b_index}];
        }}
    }}
    aclInit(nullptr);
    aclrtSetDevice(0);
    int8_t *a_device = nullptr, *b_device = nullptr;
    int32_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(int32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size(), a.data(), a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size(), b.data(), b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(int32_t), output_device,
        output.size() * sizeof(int32_t), ACL_MEMCPY_DEVICE_TO_HOST);
    PrintRow("Input A row 0", a);
    PrintRow("Input B row 0", b);
    PrintRow("Output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {api} preserved the MMAD operands/result."
                         : "[Failed] {api} result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def mx_source(rel, kernel, api, target):
    binary = "c_api_cube_datamove_" + rel.replace("/", "_").replace(".asc", "")
    scale_a_load = '''    asc_set_gm2l1_nz_para(scale_config);
    asc_copy_gm2l1_dn2nz(reinterpret_cast<__cbuf__ half*>(scale_a_l1),
        reinterpret_cast<__gm__ half*>(scale_a), SCALE_K, 0, SCALE_K / 2, M, 0, false);'''
    if target == "dn2nz":
        scale_a_load = '''    asc_set_gm2l1_nz_para(scale_config);
    // Convert the per-row scale matrix from DN in GM to Nz in L1.
    asc_copy_gm2l1_dn2nz(reinterpret_cast<__cbuf__ half*>(scale_a_l1),
        reinterpret_cast<__gm__ half*>(scale_a), SCALE_K, 0, SCALE_K / 2, M, 0, false);'''
    scale_a_copy = '''    asc_copy_l12l0a_mx(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(a_l0)) / 16,
        scale_a_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);'''
    if target == "l12l0a_mx":
        scale_a_copy = '''    // Load A's E8M0 scale tile into the MX region associated with L0A.
    asc_copy_l12l0a_mx(
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(a_l0)) / 16,
        scale_a_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);'''
    scale_b_copy = '''    asc_copy_l12l0b_mx(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(b_l0)) / 16,
        scale_b_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);'''
    if target == "l12l0b_mx":
        scale_b_copy = '''    // Load B's E8M0 scale tile into the MX region associated with L0B.
    asc_copy_l12l0b_mx(
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(b_l0)) / 16,
        scale_b_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);'''
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/{binary} && /tmp/{binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t M = 16, K = 64, N = 16, SCALE_K = 2, ELEMENTS = M * N;

__global__ __cube__ void {kernel}(__gm__ uint8_t* a, __gm__ uint8_t* scale_a,
    __gm__ uint8_t* b, __gm__ uint8_t* scale_b, __gm__ float* output)
{{
    asc_init();
    __cbuf__ fp8_e4m3fn_t a_l1[M * K], b_l1[N * K];
    __cbuf__ fp8_e8m0_t scale_a_l1[M * SCALE_K], scale_b_l1[N * SCALE_K];
    __ca__ fp8_e4m3fn_t a_l0[M * K];
    __cb__ fp8_e4m3fn_t b_l0[N * K];
    __cc__ float c_l0[ELEMENTS];
    constexpr uint64_t matrix_config = (16ULL << 32) | (1ULL << 16) | 1ULL;
    constexpr uint64_t scale_config = (1ULL << 32) | (1ULL << 16) | 1ULL;
    asc_set_gm2l1_nz_para(matrix_config);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ fp8_e4m3fn_t*>(a), K, 0, M, K, 0, false);
{scale_a_load}
    asc_set_gm2l1_nz_para(matrix_config);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ fp8_e4m3fn_t*>(b), K, 0, N, K, 0, false);
    asc_set_gm2l1_nz_para(scale_config);
    asc_copy_gm2l1_dn2nz(reinterpret_cast<__cbuf__ half*>(scale_b_l1),
        reinterpret_cast<__gm__ half*>(scale_b), SCALE_K, 0, SCALE_K / 2, N, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 2, 1, 1);
{scale_a_copy}
    asc_copy_l12l0b(b_l0, b_l1, 0, 0, 1, 2, 1, 1);
{scale_b_copy}
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad_mx(c_l0, a_l0, b_l0, M, K, N, 0, true, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_FIX);
}}

void PrintRow(const char* label, const std::vector<float>& data)
{{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<uint8_t> a(M * K), b(N * K), scale_a(M * SCALE_K, 127), scale_b(N * SCALE_K, 127);
    std::vector<float> output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t i = 0; i < M; ++i) a[i * K + i] = 0x38;
    for (uint32_t i = 0; i < N; ++i) b[i * K + i] = 0x38;
    for (uint32_t i = 0; i < M; ++i) golden[i * N + i] = 1.0f;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *a_device = nullptr, *b_device = nullptr, *scale_a_device = nullptr, *scale_b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&scale_a_device), scale_a.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&scale_b_device), scale_b.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size(), a.data(), a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size(), b.data(), b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(scale_a_device, scale_a.size(), scale_a.data(), scale_a.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(scale_b_device, scale_b.size(), scale_b.data(), scale_b.size(), ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(a_device, scale_a_device, b_device, scale_b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device,
        output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input FP8 1.0 / E8M0 scale 1.0: 0x38 / " << +scale_a[0] << std::endl;
    PrintRow("Output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {api} delivered unit MX scales."
                         : "[Failed] {api} result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(scale_a_device); aclrtFree(scale_b_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def fb_source():
    rel = "asc_copy_l12fb/asc_copy_l12fb_arch_3510.asc"
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/c_api_cube_datamove_asc_copy_l12fb && /tmp/c_api_cube_datamove_asc_copy_l12fb
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t DIM = 16, ELEMENTS = DIM * DIM;
constexpr uint16_t HALF_ONE = 0x3c00, HALF_THREE = 0x4200;
constexpr uint64_t SIGNED_SCALE_TWO = 0x40000000ULL | (1ULL << 46);

__global__ __cube__ void AscCopyL12fbKernel(__gm__ uint16_t* a, __gm__ uint16_t* b,
    __gm__ uint64_t* scales, __gm__ int8_t* output)
{{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS], b_l1[ELEMENTS];
    __cbuf__ uint64_t scale_l1[DIM];
    __fbuf__ uint64_t scale_fb[DIM];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];
    constexpr uint64_t nz_config = (16ULL << 32) | (1ULL << 16) | 1ULL;
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), DIM * sizeof(half), 0, DIM, DIM, 0, false);
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), DIM * sizeof(half), 0, DIM, DIM, 0, false);
    asc_copy_gm2l1(scale_l1, scales, 1, 4, 0, 0, 0);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, 0, true, false, true);
    asc_sync_notify(PIPE_MTE2, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_FIX, EVENT_ID0);
    // Copy 16 contiguous scale entries from L1 to the Fixpipe Buffer.
    asc_copy_l12fb(scale_fb, scale_l1, DIM * sizeof(uint64_t));
    const uint64_t scale_address = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(scale_fb)) / 128;
    asc_set_l0c2gm_config(0, scale_address, false);
    asc_sync_pipe(PIPE_FIX);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::VQF322B8_PRE), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_FIX);
}}

void PrintRow(const char* label, const std::vector<int8_t>& data)
{{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << +data[i];
    std::cout << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<uint16_t> a(ELEMENTS), b(ELEMENTS);
    std::vector<uint64_t> scales(DIM, SIGNED_SCALE_TWO);
    std::vector<int8_t> output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t i = 0; i < DIM; ++i) {{
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_THREE;
        golden[i * DIM + i] = 6;
    }}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    uint64_t* scales_device = nullptr;
    int8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&scales_device), scales.size() * sizeof(uint64_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(scales_device, scales.size() * sizeof(uint64_t), scales.data(), scales.size() * sizeof(uint64_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscCopyL12fbKernel<<<1, 0>>>(a_device, b_device, scales_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size(), output_device, output.size(), ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input diagonal: 1 * 3; FB scale: 2" << std::endl;
    PrintRow("Quantized output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_l12fb loaded scale 2."
                         : "[Failed] asc_copy_l12fb result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(scales_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def l0c_local_source(target):
    is_l1 = target == "l1"
    rel = ("asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.asc" if is_l1 else "asc_copy_l0c2ub.asc")
    kernel = "AscCopyL0c2l1Kernel" if is_l1 else "AscCopyL0c2ubKernel"
    api = ("asc_copy_l0c2l1" if is_l1 else "asc_copy_l0c2ub")
    binary = "c_api_cube_datamove_" + ("asc_copy_l0c2l1" if is_l1 else "asc_copy_l0c2ub")
    if is_l1:
        local_decl = '''    __cbuf__ uint8_t packed_l1[PACKED_BYTES];
    __ubuf__ uint8_t packed_ub[PACKED_BYTES];'''
        target_call = '''        // Quantize the FP32 L0C matrix to packed signed int4 in L1.
        asc_copy_l0c2l1(reinterpret_cast<__cbuf__ int4b_t*>(packed_l1),
            c_l0, N, M, N, M, 0, 0, 0,
            static_cast<uint64_t>(QuantMode_t::QF322S4_PRE), 0, false, true, 0, 0, false, 0,
            false, false, false, false);
        asc_sync_notify(PIPE_FIX, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_FIX, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12ub(packed_ub, packed_l1, false, 1, PACKED_BYTES / 32, 0, 0);
        asc_sync_intra_arrive(PIPE_MTE1, intra_id);'''
    else:
        local_decl = '''    __ubuf__ uint8_t packed_ub[PACKED_BYTES];'''
        target_call = '''        // Quantize the FP32 L0C matrix to packed signed int4 in AIV0's UB.
        asc_copy_l0c2ub(reinterpret_cast<__ubuf__ int4b_t*>(packed_ub),
            c_l0, N, M, N, M, 0, false, 0, 0,
            static_cast<uint64_t>(QuantMode_t::QF322S4_PRE), 0, false, true, 0, 0, false, 0,
            false, false, false, false);
        asc_sync_intra_arrive(PIPE_FIX, intra_id);'''
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/{binary} && /tmp/{binary}
// Architecture: dav-3510

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t M = 16, K = 16, N = 64, ELEMENTS = M * N, PACKED_BYTES = M * 32;
constexpr uint64_t SIGNED_SCALE_ONE = 0x3f800000ULL | (1ULL << 46);

__global__ __mix__(1, 2) void {kernel}(__gm__ uint16_t* a, __gm__ uint16_t* b,
    __gm__ uint8_t* output)
{{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS], b_l1[ELEMENTS];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];
{local_decl}
    constexpr uint64_t intra_id = 2;
    if ASC_IS_AIC {{
        constexpr uint64_t nz_config = (16ULL << 32) | (1ULL << 16) | 1ULL;
        asc_set_gm2l1_nz_para(nz_config);
        asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), K * sizeof(half), 0, M, K, 0, false);
        asc_set_gm2l1_nz_para(nz_config);
        asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), N * sizeof(half), 0, K, N, 0, false);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
        asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, N / 16, 1, N / 16, 1);
        asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
        asc_mmad(c_l0, a_l0, b_l0, M, K, N, 0, false, false, true);
        asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
        asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
        asc_set_l0c_copy_prequant(SIGNED_SCALE_ONE);
{target_call}
    }}
    if ASC_IS_AIV {{
        if (asc_get_sub_block_id() == 0) {{
            asc_sync_intra_wait(PIPE_MTE3, intra_id);
            asc_copy_ub2gm_align(output,
                reinterpret_cast<__ubuf__ uint8_t*>(packed_ub), PACKED_BYTES);
            asc_sync_mte3(0);
        }}
    }}
}}

void PrintBytes(const char* label, const std::vector<uint8_t>& data, uint32_t offset)
{{
    std::cout << label << ':' << std::hex;
    for (uint32_t i = 0; i < 16; ++i) std::cout << " 0x" << +data[offset + i];
    std::cout << std::dec << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    constexpr uint16_t HALF_ONE = 0x3c00, HALF_THREE = 0x4200;
    std::vector<uint16_t> a(ELEMENTS), b(ELEMENTS);
    std::vector<uint8_t> output(PACKED_BYTES), golden(PACKED_BYTES);
    for (uint32_t i = 0; i < M; ++i) a[i * K + i] = HALF_ONE;
    for (uint32_t i = 0; i < ELEMENTS; ++i) b[i] = HALF_THREE;
    for (uint32_t row = 0; row < M; ++row)
        for (uint32_t i = 0; i < N / 2; ++i) golden[row * 32 + i] = 0x33;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    uint8_t* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size(), output_device, output.size(), ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input: identity * FP16 matrix filled with 3" << std::endl;
    PrintBytes("Packed int4 output row 0", output, 0);
    PrintBytes("Packed int4 output row 1", output, 32);
    PrintBytes("Golden row 0", golden, 0);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {api} converted the L0C matrix to int4."
                         : "[Failed] {api} result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def fixpipe_setter_source(kind):
    is_lrelu = kind == "lrelu"
    rel = "asc_set_l0c2gm_lrelu_alpha.asc" if is_lrelu else "asc_set_l0c_copy_prequant.asc"
    kernel = "AscSetL0c2gmLreluAlphaKernel" if is_lrelu else "AscSetL0cCopyPrequantKernel"
    api = "asc_set_l0c2gm_lrelu_alpha" if is_lrelu else "asc_set_l0c_copy_prequant"
    binary = "c_api_cube_datamove_" + api
    if is_lrelu:
        setter = '''    float alpha = 0.5f;
    // Set the negative slope used by Fixpipe leaky-ReLU conversion to FP16.
    asc_set_l0c2gm_lrelu_alpha(alpha);'''
        output_type = "uint16_t"
        output_cast = "reinterpret_cast<__gm__ half*>(output)"
        quant = "QuantMode_t::F322F16"
        relu = "2"
        host_output = "std::vector<uint16_t> output(ELEMENTS), golden(ELEMENTS);"
        host_input = '''    for (uint32_t i = 0; i < DIM; ++i) {
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_NEG_ONE;
        golden[i * DIM + i] = HALF_NEG_HALF;
    }'''
        malloc_size = "output.size() * sizeof(uint16_t)"
        print_func = '''void PrintRow(const char* label, const std::vector<uint16_t>& data)
{
    std::cout << label << ':' << std::hex;
    for (uint32_t i = 0; i < 8; ++i) std::cout << " 0x" << data[i];
    std::cout << std::dec << " ..." << std::endl;
}'''
        print_lines = '''    PrintRow("Input B row 0 (FP16 bits)", b);
    PrintRow("Leaky ReLU output row 0", output);
    PrintRow("Golden row 0", golden);'''
        success = "applied alpha 0.5 to -1.0"
    else:
        setter = '''    constexpr uint64_t signed_scale_two = 0x40000000ULL | (1ULL << 46);
    // Apply scalar scale 2.0 during signed int8 Fixpipe pre-quantization.
    asc_set_l0c_copy_prequant(signed_scale_two);'''
        output_type = "int8_t"
        output_cast = "output"
        quant = "QuantMode_t::QF322B8_PRE"
        relu = "0"
        host_output = "std::vector<int8_t> output(ELEMENTS), golden(ELEMENTS);"
        host_input = '''    for (uint32_t i = 0; i < DIM; ++i) {
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_THREE;
        golden[i * DIM + i] = 6;
    }'''
        malloc_size = "output.size()"
        print_func = '''void PrintRow(const char* label, const std::vector<int8_t>& data)
{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << +data[i];
    std::cout << " ..." << std::endl;
}'''
        print_lines = '''    std::cout << "Input diagonal: FP16 1 * 3; scalar scale: 2" << std::endl;
    PrintRow("Quantized output row 0", output);
    PrintRow("Golden row 0", golden);'''
        success = "scaled 3.0 to signed int8 6"
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/{binary} && /tmp/{binary}
// Architecture: dav-3510

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t DIM = 16, ELEMENTS = DIM * DIM;
constexpr uint16_t HALF_ONE = 0x3c00, HALF_THREE = 0x4200;
constexpr uint16_t HALF_NEG_ONE = 0xbc00, HALF_NEG_HALF = 0xb800;

__global__ __cube__ void {kernel}(__gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ {output_type}* output)
{{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS], b_l1[ELEMENTS];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];
    constexpr uint64_t nz_config = (16ULL << 32) | (1ULL << 16) | 1ULL;
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), DIM * sizeof(half), 0, DIM, DIM, 0, false);
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), DIM * sizeof(half), 0, DIM, DIM, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, 0, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
{setter}
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm({output_cast}, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>({quant}), {relu}, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_FIX);
}}

{print_func}
}} // namespace

int main()
{{
    std::vector<uint16_t> a(ELEMENTS), b(ELEMENTS);
    {host_output}
{host_input}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    {output_type}* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), {malloc_size}, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), {malloc_size}, output_device, {malloc_size}, ACL_MEMCPY_DEVICE_TO_HOST);
{print_lines}
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {api} {success}."
                         : "[Failed] {api} result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def bias_source():
    rel = "asc_copy_l12bt/asc_copy_l12bt_arch_3510.asc"
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/c_api_cube_datamove_asc_copy_l12bt && /tmp/c_api_cube_datamove_asc_copy_l12bt
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t DIM = 16, ELEMENTS = DIM * DIM, BIAS_BYTES = DIM * sizeof(float);

__global__ __cube__ void AscCopyL12btKernel(__gm__ float* bias, __gm__ float* output)
{{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS], b_l1[ELEMENTS];
    __cbuf__ float bias_l1[DIM];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];
    asc_fill_value_config fill{{}};
    fill.repeat = 1;
    fill.blk_num = ELEMENTS * sizeof(half) / 32;
    fill.dst_gap = 0;
    asc_fill_l1(a_l1, 0U, fill);
    asc_fill_l1(b_l1, 0U, fill);
    asc_copy_gm2l1(bias_l1, bias, 1, BIAS_BYTES / 32, 0, 0, 0);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    // Load 16 FP32 bias values from L1 into Bias Table offset 0.
    asc_copy_l12bt(0, bias_l1, BIAS_BYTES);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, 0, false, true, false);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_FIX);
}}

void PrintRow(const char* label, const std::vector<float>& data)
{{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[i];
    std::cout << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<float> bias(DIM), output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t i = 0; i < DIM; ++i) bias[i] = static_cast<float>(i + 1);
    for (uint32_t row = 0; row < DIM; ++row)
        for (uint32_t col = 0; col < DIM; ++col) golden[row * DIM + col] = bias[col];
    aclInit(nullptr);
    aclrtSetDevice(0);
    float *bias_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&bias_device), BIAS_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(bias_device, BIAS_BYTES, bias.data(), BIAS_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    AscCopyL12btKernel<<<1, 0>>>(bias_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device,
        output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    PrintRow("Input Bias", bias);
    PrintRow("MMAD output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_l12bt initialized MMAD from Bias Table."
                         : "[Failed] asc_copy_l12bt result mismatch.") << std::endl;
    aclrtFree(bias_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def load3d_padding_source(target):
    is_b = target == "fmatrix_b"
    api = "asc_set_l13d_fmatrix_b" if is_b else ("asc_set_l13d_fmatrix" if target == "fmatrix" else "asc_set_l13d_padding")
    rel = api + ".asc"
    kernel = "".join(part.capitalize() for part in api.split("_")) + "Kernel"
    binary = "c_api_cube_datamove_" + api
    if is_b:
        copy_a = '''    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);'''
        setup = '''    asc_l13d_fmatrix_config feature_map{};
    feature_map.l1_width = 1;
    feature_map.l1_height = DIM - 1;
    feature_map.padding_top_size = 1;
    // Describe B as 15 L1 rows plus one logical top-padding row.
    asc_set_l13d_fmatrix_b(feature_map);
    asc_load3d_v2_config repeat;
    asc_set_l13d_rpt(repeat);
    asc_set_l12l0_padding_val(HALF_ONE);
    asc_copy_l12l0b(b_l0, b_l1, DIM, DIM, 0, 0, 1, 1, 1, 1, 1, 1,
        false, false, true, true, DIM);'''
        load_a_rows, load_b_rows = "DIM", "DIM - 1"
        host_a = '''    for (uint32_t i = 0; i < DIM; ++i) a[i * DIM + i] = HALF_ONE;'''
    else:
        copy_a = '''    asc_l13d_fmatrix_config feature_map{};
    feature_map.l1_width = 1;
    feature_map.l1_height = DIM - 1;
    feature_map.padding_top_size = 1;
    // Describe A as 15 L1 rows plus one logical top-padding row.
    asc_set_l13d_fmatrix(feature_map);
    asc_load3d_v2_config repeat;
    asc_set_l13d_rpt(repeat);
    // Supply 1.0 for positions outside the configured feature map.
    asc_set_l13d_padding(static_cast<half>(1.0f));
    asc_copy_l12l0a(a_l0, a_l1, DIM, DIM, 0, 0, 1, 1, 1, 1, 1, 1,
        false, false, false, false, DIM);'''
        setup = '''    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);'''
        load_a_rows, load_b_rows = "DIM - 1", "DIM"
        host_a = '''    for (uint32_t i = 0; i < DIM; ++i) b[i * DIM + i] = HALF_ONE;'''
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/{binary} && /tmp/{binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t DIM = 16, ELEMENTS = DIM * DIM;
constexpr uint16_t HALF_ONE = 0x3c00;

__global__ __cube__ void {kernel}(__gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ float* output)
{{
    asc_init();
    __cbuf__ half a_l1[ELEMENTS], b_l1[ELEMENTS];
    __ca__ half a_l0[ELEMENTS];
    __cb__ half b_l0[ELEMENTS];
    __cc__ float c_l0[ELEMENTS];
    constexpr uint64_t nz_config = (16ULL << 32) | (1ULL << 16) | 1ULL;
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), DIM * sizeof(half), 0,
        {load_a_rows}, DIM, 0, false);
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), DIM * sizeof(half), 0,
        {load_b_rows}, DIM, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
{copy_a}
{setup}
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, DIM, DIM, DIM, 0, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_FIX);
}}

void PrintRow(const char* label, const std::vector<float>& data, uint32_t offset)
{{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[offset + i];
    std::cout << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<uint16_t> a(ELEMENTS), b(ELEMENTS);
    std::vector<float> output(ELEMENTS), golden(ELEMENTS);
{host_a}
    for (uint32_t i = 0; i < DIM; ++i) golden[i] = 1.0f;
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    {kernel}<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device,
        output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input feature map: 15 zero rows; top padding: 1" << std::endl;
    PrintRow("Output row 0", output, 0);
    PrintRow("Output row 1", output, DIM);
    PrintRow("Golden row 0", golden, 0);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] {api} made the padded row visible to MMAD."
                         : "[Failed] {api} result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def load3d_repeat_source():
    rel = "asc_set_l13d_rpt.asc"
    return f'''// Build/run ({PREFIX}/cube_datamove/{rel}): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/cube_datamove/{rel} -lm -o /tmp/c_api_cube_datamove_asc_set_l13d_rpt && /tmp/c_api_cube_datamove_asc_set_l13d_rpt
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
constexpr uint32_t M = 32, K = 16, N = 16;
constexpr uint32_t A_ELEMENTS = M * K, B_ELEMENTS = K * N, C_ELEMENTS = M * N;
constexpr uint16_t HALF_ONE = 0x3c00, HALF_NEG_ONE = 0xbc00;

__global__ __cube__ void AscSetL13dRptKernel(
    __gm__ uint16_t* a, __gm__ uint16_t* b, __gm__ float* output)
{{
    asc_init();
    __cbuf__ half a_l1[A_ELEMENTS], b_l1[B_ELEMENTS];
    __ca__ half a_l0[A_ELEMENTS];
    __cb__ half b_l0[B_ELEMENTS];
    __cc__ float c_l0[C_ELEMENTS];
    constexpr uint64_t nz_config = (16ULL << 32) | (1ULL << 16) | 1ULL;
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(a_l1, reinterpret_cast<__gm__ half*>(a), K * sizeof(half), 0, M, K, 0, false);
    asc_set_gm2l1_nz_para(nz_config);
    asc_copy_gm2l1_nd2nz(b_l1, reinterpret_cast<__gm__ half*>(b), N * sizeof(half), 0, K, N, 0, false);
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_l13d_fmatrix_config feature_map{{}};
    feature_map.l1_width = 1;
    feature_map.l1_height = M;
    asc_set_l13d_fmatrix(feature_map);
    asc_load3d_v2_config repeat;
    repeat.rpt_stride = 1;
    repeat.rpt_time = 2;
    repeat.rpt_mode = 0;
    repeat.config |= 2ULL << 32;
    // Load two height fragments with one-fragment stride from L1 to L0A.
    asc_set_l13d_rpt(repeat);
    asc_set_l13d_padding(static_cast<half>(0.0f));
    asc_copy_l12l0a(a_l0, a_l1, K, 16, 0, 0, 1, 1, 1, 1, 1, 1,
        false, false, false, false, K);
    asc_copy_l12l0b_transpose(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);
    asc_mmad(c_l0, a_l0, b_l0, M, K, N, 0, false, false, true);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output, c_l0, N, M, N, M, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_FIX);
}}

void PrintRow(const char* label, const std::vector<float>& data, uint32_t offset)
{{
    std::cout << label << ':';
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << data[offset + i];
    std::cout << " ..." << std::endl;
}}
}} // namespace

int main()
{{
    std::vector<uint16_t> a(A_ELEMENTS), b(B_ELEMENTS);
    std::vector<float> input_a(A_ELEMENTS), output(C_ELEMENTS), golden(C_ELEMENTS);
    for (uint32_t i = 0; i < 16; ++i) {{
        a[i * K + i] = HALF_ONE;
        a[(16 + i) * K + i] = HALF_NEG_ONE;
        input_a[i * K + i] = 1.0f;
        input_a[(16 + i) * K + i] = -1.0f;
        b[i * N + i] = HALF_ONE;
        golden[i * N + i] = 1.0f;
        golden[(16 + i) * N + i] = -1.0f;
    }}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *a_device = nullptr, *b_device = nullptr;
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&a_device), a.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&b_device), b.size() * sizeof(uint16_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(a_device, a.size() * sizeof(uint16_t), a.data(), a.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(b_device, b.size() * sizeof(uint16_t), b.data(), b.size() * sizeof(uint16_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscSetL13dRptKernel<<<1, 0>>>(a_device, b_device, output_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device,
        output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    PrintRow("Input A row 0", input_a, 0);
    PrintRow("Input A row 16", input_a, 16 * K);
    PrintRow("Output row 0", output, 0);
    PrintRow("Output row 16", output, 16 * N);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_l13d_rpt loaded two height fragments."
                         : "[Failed] asc_set_l13d_rpt result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}}
'''


def sources():
    plain_readback = '''        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12ub(readback, l1, false, 1, OUTPUT_BYTES / 32, 0, 0);'''
    items = {}
    body = '''        // Copy one contiguous 64-byte GM range to L1: n_burst=1, len_burst=2 blocks.
        asc_copy_gm2l1(l1, input, 1, OUTPUT_BYTES / 32, 0, 0, 0);
''' + plain_readback
    items["asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.asc"] = raw_source(
        "asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.asc", "AscCopyGm2l1Kernel",
        "asc_copy_gm2l1", body, 64, 64)

    body = '''        // Copy two 32-byte aligned bursts without head/tail padding.
        asc_copy_gm2l1_align(l1, input, 2, 32, 0, 0, false, 0, 32, 32);
''' + plain_readback
    items["asc_copy_gm2l1_align.asc"] = raw_source(
        "asc_copy_gm2l1_align.asc", "AscCopyGm2l1AlignKernel",
        "asc_copy_gm2l1_align", body, 64, 64)

    body = '''        asc_copy_gm2l1(l1, input, 1, OUTPUT_BYTES / 32, 0, 0, 0);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        // sub_blockid=false selects AIV0's UB for one two-block L1 transfer.
        asc_copy_l12ub(readback, l1, false, 1, OUTPUT_BYTES / 32, 0, 0);'''
    items["asc_copy_l12ub.asc"] = raw_source(
        "asc_copy_l12ub.asc", "AscCopyL12ubKernel", "asc_copy_l12ub", body, 64, 64)

    loop_cases = [
        ("asc_set_gm2l1_loop1_stride.asc", "AscSetGm2l1Loop1StrideKernel", "asc_set_gm2l1_loop1_stride",
         "asc_set_gm2l1_loop_size(2, 1);\n"
         "        // Select source/destination byte strides for the first GM-to-L1 loop.\n"
         "        asc_set_gm2l1_loop1_stride(64, 32);", 96, 64,
         '''    for (uint32_t i = 0; i < 32; ++i) { golden[i] = input[i]; golden[32 + i] = input[64 + i]; }'''),
        ("asc_set_gm2l1_loop2_stride.asc", "AscSetGm2l1Loop2StrideKernel", "asc_set_gm2l1_loop2_stride",
         "asc_set_gm2l1_loop_size(1, 2);\n"
         "        // Select source/destination byte strides for the second GM-to-L1 loop.\n"
         "        asc_set_gm2l1_loop2_stride(64, 32);", 96, 64,
         '''    for (uint32_t i = 0; i < 32; ++i) { golden[i] = input[i]; golden[32 + i] = input[64 + i]; }'''),
        ("asc_set_gm2l1_loop_size.asc", "AscSetGm2l1LoopSizeKernel", "asc_set_gm2l1_loop_size",
         "// Execute a 2x2 nested GM-to-L1 transfer.\n"
         "        asc_set_gm2l1_loop_size(2, 2);\n"
         "        asc_set_gm2l1_loop1_stride(64, 32);\n"
         "        asc_set_gm2l1_loop2_stride(128, 64);", 224, 128,
         '''    for (uint32_t block = 0; block < 4; ++block) {
        for (uint32_t i = 0; i < 32; ++i) golden[block * 32 + i] = input[block * 64 + i];
    }'''),
    ]
    for rel, kernel, api, setup, inp, out, golden in loop_cases:
        body = f'''        {setup}
        asc_copy_gm2l1_align(l1, input, 1, 32, 0, 0, false, 0, 0, 0);
        asc_set_gm2l1_loop_size(1, 1);
''' + plain_readback
        items[rel] = raw_source(rel, kernel, api, body, inp, out, golden)

    pad_golden = '''    for (uint32_t i = 0; i < 8; ++i) golden[i] = golden[24 + i] = 0xa5;
    for (uint32_t i = 0; i < 16; ++i) golden[8 + i] = input[i];'''
    body = '''        // Use byte value 0xa5 for the configured head and tail padding.
        asc_set_gm2l1_pad(0xa5a5a5a5U);
        asc_copy_gm2l1_align(l1, input, 1, 16, 8, 8, false, 0, 0, 0);
''' + plain_readback
    items["asc_set_gm2l1_pad.asc"] = raw_source(
        "asc_set_gm2l1_pad.asc", "AscSetGm2l1PadKernel", "asc_set_gm2l1_pad", body, 16, 32, pad_golden)
    items["asc_fill_l1.asc"] = fill_source()
    matrix_cases = [
        ("asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.asc", "AscCopyGm2l1Nd2nzKernel",
         "asc_copy_gm2l1_nd2nz", "nd2nz"),
        ("asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.asc", "AscCopyL12l0aKernel",
         "asc_copy_l12l0a", "l12l0a"),
        ("asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.asc", "AscCopyL12l0bKernel",
         "asc_copy_l12l0b", "l12l0b"),
        ("asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.asc", "AscCopyL12l0bTransKernel",
         "asc_copy_l12l0b_trans", "l12l0b_trans"),
        ("asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.asc", "AscCopyL0c2gmKernel",
         "asc_copy_l0c2gm", "l0c2gm"),
    ]
    for rel, kernel, api, target in matrix_cases:
        items[rel] = matrix_source(rel, kernel, api, target)
    mx_cases = [
        ("asc_copy_gm2l1_dn2nz.asc", "AscCopyGm2l1Dn2nzKernel",
         "asc_copy_gm2l1_dn2nz", "dn2nz"),
        ("asc_copy_l12l0a_mx.asc", "AscCopyL12l0aMxKernel",
         "asc_copy_l12l0a_mx", "l12l0a_mx"),
        ("asc_copy_l12l0b_mx.asc", "AscCopyL12l0bMxKernel",
         "asc_copy_l12l0b_mx", "l12l0b_mx"),
    ]
    for rel, kernel, api, target in mx_cases:
        items[rel] = mx_source(rel, kernel, api, target)
    items["asc_copy_l12fb/asc_copy_l12fb_arch_3510.asc"] = fb_source()
    items["asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.asc"] = l0c_local_source("l1")
    items["asc_copy_l0c2ub.asc"] = l0c_local_source("ub")
    items["asc_set_l0c2gm_lrelu_alpha.asc"] = fixpipe_setter_source("lrelu")
    items["asc_set_l0c_copy_prequant.asc"] = fixpipe_setter_source("prequant")
    items["asc_copy_l12bt/asc_copy_l12bt_arch_3510.asc"] = bias_source()
    items["asc_set_l13d_fmatrix.asc"] = load3d_padding_source("fmatrix")
    items["asc_set_l13d_fmatrix_b.asc"] = load3d_padding_source("fmatrix_b")
    items["asc_set_l13d_padding.asc"] = load3d_padding_source("padding")
    items["asc_set_l13d_rpt.asc"] = load3d_repeat_source()
    return items


def main():
    if len(sys.argv) != 2:
        raise SystemExit(f"Usage: {sys.argv[0]} <C-API/cube_datamove directory>")
    doc_dir = Path(sys.argv[1]).resolve()
    if not doc_dir.is_dir():
        raise SystemExit(f"Documentation directory does not exist: {doc_dir}")
    rendered = sources()
    for rel, text in rendered.items():
        destination = ROOT / "cube_datamove" / rel
        destination.parent.mkdir(parents=True, exist_ok=True)
        destination.write_text(text, encoding="utf-8")
    print(f"Generated {len(rendered)} functional Cube data movement examples")


if __name__ == "__main__":
    main()
