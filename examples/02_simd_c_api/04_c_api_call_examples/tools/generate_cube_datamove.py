#!/usr/bin/env python3

import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def raw_source(rel, kernel, api, aic_body, input_bytes, output_bytes, launches=2, golden=""):
    binary = "c_api_cube_datamove_" + rel.replace("/", "_").replace(".asc", "")
    if not golden:
        golden = f'''    for (uint32_t mode = 0; mode < {launches}; ++mode) {{
        for (uint32_t i = 0; i < OUTPUT_BYTES; ++i) golden[mode * OUTPUT_BYTES + i] = input[i];
    }}'''
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

__global__ __mix__(1, 2) void {kernel}(__gm__ uint8_t* output, __gm__ uint8_t* input, uint32_t mode)
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
            asc_copy_ub2gm_align(output + mode * OUTPUT_BYTES, readback, OUTPUT_BYTES);
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
    std::vector<uint8_t> input(INPUT_BYTES), output(OUTPUT_BYTES * {launches}), golden(output.size());
    for (uint32_t i = 0; i < INPUT_BYTES; ++i) input[i] = static_cast<uint8_t>(i + 1);
{golden}
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size(), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    for (uint32_t mode = 0; mode < {launches}; ++mode) {{
        {kernel}<<<1, 0>>>(output_device, input_device, mode);
        aclrtSynchronizeDevice();
    }}
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

__global__ __mix__(1, 2) void AscFillL1Kernel(__gm__ float* output, uint32_t mode)
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
        if (mode == 0) asc_fill_l1(l1, 0x3f800000U, config);
        else asc_fill_l1_sync(l1, 0x3f800000U, config);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_copy_l12ub(readback, l1, false, 1, ELEMENTS * sizeof(float) / 32, 0, 0);
        asc_sync_intra_arrive(PIPE_MTE1, intra_id);
    }}
    if ASC_IS_AIV {{
        if (asc_get_sub_block_id() == 0) {{
            asc_sync_intra_wait(PIPE_MTE3, intra_id);
            asc_copy_ub2gm_align(output + mode * ELEMENTS, readback, ELEMENTS * sizeof(float));
            asc_sync_mte3(0);
        }}
    }}
}}
}} // namespace

int main()
{{
    std::vector<float> output(ELEMENTS * 2), golden(ELEMENTS * 2, 1.0f);
    aclInit(nullptr);
    aclrtSetDevice(0);
    float* output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    for (uint32_t mode = 0; mode < 2; ++mode) {{
        AscFillL1Kernel<<<1, 0>>>(output_device, mode);
        aclrtSynchronizeDevice();
    }}
    aclrtMemcpy(output.data(), output.size() * sizeof(float), output_device, output.size() * sizeof(float), ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Filled L1 values:";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << output[i];
    std::cout << " ..." << std::endl;
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_fill_l1/asc_fill_l1_sync filled L1 with 1.0."
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
    if (mode == 0) asc_copy_gm2l1_nd2nz(a_l1, a, K, 0, M, K, 0, false);
    else asc_copy_gm2l1_nd2nz_sync(a_l1, a, K, 0, M, K, 0, false);'''
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
        copy_a = '''    if (mode == 0) asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);
    else asc_copy_l12l0a_sync(a_l0, a_l1, 0, 0, 1, 1, 1, 1);'''
    else:
        copy_a = '''    asc_copy_l12l0a(a_l0, a_l1, 0, 0, 1, 1, 1, 1);'''
    if target == "l12l0b":
        copy_b = '''    if (mode == 0) asc_copy_l12l0b(b_l0, b_l1, 0, 0, 1, 1, 1, 1);
    else asc_copy_l12l0b_sync(b_l0, b_l1, 0, 0, 1, 1, 1, 1);'''
    elif target == "l12l0b_trans":
        copy_b = '''    if (mode == 0) asc_copy_l12l0b_trans(b_l0, b_l1, static_cast<uint16_t>(0),
        static_cast<uint8_t>(1), static_cast<uint16_t>(2), static_cast<uint16_t>(1),
        static_cast<uint16_t>(0), static_cast<uint16_t>(0));
    else asc_copy_l12l0b_trans_sync(b_l0, b_l1, static_cast<uint16_t>(0),
        static_cast<uint8_t>(1), static_cast<uint16_t>(2), static_cast<uint16_t>(1),
        static_cast<uint16_t>(0), static_cast<uint16_t>(0));'''
    else:
        copy_b = '''    asc_copy_l12l0b_trans(b_l0, b_l1, static_cast<uint16_t>(0),
        static_cast<uint8_t>(1), static_cast<uint16_t>(2), static_cast<uint16_t>(1),
        static_cast<uint16_t>(0), static_cast<uint16_t>(0));'''
    copy_out_call = '''output + mode * ELEMENTS, c_l0, N, M, N, M, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false'''
    if target == "l0c2gm":
        copy_out = f'''    if (mode == 0) asc_copy_l0c2gm({copy_out_call});
    else asc_copy_l0c2gm_sync({copy_out_call});'''
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
    __gm__ int32_t* output, uint32_t mode)
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
    asc_sync_pipe(PIPE_ALL);
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
    std::vector<int32_t> output(ELEMENTS * 2), golden(ELEMENTS * 2);
    for (uint32_t i = 0; i < M; ++i) a[i * K + i] = 1;
    for (uint32_t i = 0; i < N; ++i) b[{('i * K + i' if b_is_nk else 'i * N + i')}] = 2;
    for (uint32_t mode = 0; mode < 2; ++mode) {{
        for (uint32_t row = 0; row < M; ++row) {{
            for (uint32_t col = 0; col < N; ++col) {{
                for (uint32_t k = 0; k < K; ++k)
                    golden[mode * ELEMENTS + row * N + col] += a[row * K + k] * b[{b_index}];
            }}
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
    for (uint32_t mode = 0; mode < 2; ++mode) {{
        {kernel}<<<1, 0>>>(a_device, b_device, output_device, mode);
        aclrtSynchronizeDevice();
    }}
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
    if (mode == 0) asc_copy_gm2l1_dn2nz(reinterpret_cast<__cbuf__ half*>(scale_a_l1),
        reinterpret_cast<__gm__ half*>(scale_a), SCALE_K, 0, SCALE_K / 2, M, 0, false);
    else asc_copy_gm2l1_dn2nz_sync(reinterpret_cast<__cbuf__ half*>(scale_a_l1),
        reinterpret_cast<__gm__ half*>(scale_a), SCALE_K, 0, SCALE_K / 2, M, 0, false);'''
    scale_a_copy = '''    asc_copy_l12l0a_mx(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(a_l0)) / 16,
        scale_a_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);'''
    if target == "l12l0a_mx":
        scale_a_copy = '''    if (mode == 0) asc_copy_l12l0a_mx(
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(a_l0)) / 16,
        scale_a_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);
    else asc_copy_l12l0a_mx_sync(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(a_l0)) / 16,
        scale_a_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);'''
    scale_b_copy = '''    asc_copy_l12l0b_mx(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(b_l0)) / 16,
        scale_b_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);'''
    if target == "l12l0b_mx":
        scale_b_copy = '''    if (mode == 0) asc_copy_l12l0b_mx(
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(b_l0)) / 16,
        scale_b_l1, 0, 0, 1, SCALE_K / 2, SCALE_K / 2, SCALE_K / 2);
    else asc_copy_l12l0b_mx_sync(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(b_l0)) / 16,
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
    __gm__ uint8_t* b, __gm__ uint8_t* scale_b, __gm__ float* output, uint32_t mode)
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
    asc_copy_l0c2gm(output + mode * ELEMENTS, c_l0, N, M, N, M, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::NoQuant), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_ALL);
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
    std::vector<float> output(ELEMENTS * 2), golden(ELEMENTS * 2);
    for (uint32_t i = 0; i < M; ++i) a[i * K + i] = 0x38;
    for (uint32_t i = 0; i < N; ++i) b[i * K + i] = 0x38;
    for (uint32_t mode = 0; mode < 2; ++mode)
        for (uint32_t i = 0; i < M; ++i) golden[mode * ELEMENTS + i * N + i] = 1.0f;
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
    for (uint32_t mode = 0; mode < 2; ++mode) {{
        {kernel}<<<1, 0>>>(a_device, scale_a_device, b_device, scale_b_device, output_device, mode);
        aclrtSynchronizeDevice();
    }}
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
    __gm__ uint64_t* scales, __gm__ int8_t* output, uint32_t mode)
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
    if (mode == 0) asc_copy_l12fb(scale_fb, scale_l1, 1, 2, 0, 0);
    else asc_copy_l12fb_sync(scale_fb, scale_l1, DIM * sizeof(uint64_t));
    const uint64_t scale_address = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(scale_fb)) / 128;
    asc_set_l0c2gm_config(0, scale_address, false);
    asc_sync_pipe(PIPE_FIX);
    asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
    asc_set_l0c2gm_nz2nd(1, 0, 0);
    asc_copy_l0c2gm(output + mode * ELEMENTS, c_l0, DIM, DIM, DIM, DIM, 0, 0, 0,
        static_cast<uint64_t>(QuantMode_t::VQF322B8_PRE), 0, false, true,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, 0, false, false, false, false);
    asc_sync_pipe(PIPE_ALL);
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
    std::vector<int8_t> output(ELEMENTS * 2), golden(ELEMENTS * 2);
    for (uint32_t i = 0; i < DIM; ++i) {{
        a[i * DIM + i] = HALF_ONE;
        b[i * DIM + i] = HALF_THREE;
        golden[i * DIM + i] = golden[ELEMENTS + i * DIM + i] = 6;
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
    for (uint32_t mode = 0; mode < 2; ++mode) {{
        AscCopyL12fbKernel<<<1, 0>>>(a_device, b_device, scales_device, output_device, mode);
        aclrtSynchronizeDevice();
    }}
    aclrtMemcpy(output.data(), output.size(), output_device, output.size(), ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input diagonal: 1 * 3; FB scale: 2" << std::endl;
    PrintRow("Quantized output row 0", output);
    PrintRow("Golden row 0", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_copy_l12fb/asc_copy_l12fb_sync loaded scale 2."
                         : "[Failed] asc_copy_l12fb result mismatch.") << std::endl;
    aclrtFree(a_device); aclrtFree(b_device); aclrtFree(scales_device); aclrtFree(output_device);
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
    body = '''        if (mode == 0) asc_copy_gm2l1(l1, input, 1, OUTPUT_BYTES / 32, 0, 0, 0);
        else asc_copy_gm2l1_sync(l1, input, 1, OUTPUT_BYTES / 32, 0, 0, 0);
''' + plain_readback
    items["asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.asc"] = raw_source(
        "asc_copy_gm2l1/asc_copy_gm2l1_arch_3510.asc", "AscCopyGm2l1Kernel",
        "asc_copy_gm2l1/asc_copy_gm2l1_sync", body, 64, 64)

    body = '''        if (mode == 0) asc_copy_gm2l1_align(l1, input, 2, 32, 0, 0, false, 0, 32, 32);
        else asc_copy_gm2l1_align_sync(l1, input, 2, 32, 0, 0, false, 0, 32, 32);
''' + plain_readback
    items["asc_copy_gm2l1_align.asc"] = raw_source(
        "asc_copy_gm2l1_align.asc", "AscCopyGm2l1AlignKernel",
        "asc_copy_gm2l1_align/asc_copy_gm2l1_align_sync", body, 64, 64)

    body = '''        asc_copy_gm2l1(l1, input, 1, OUTPUT_BYTES / 32, 0, 0, 0);
        asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
        if (mode == 0) asc_copy_l12ub(readback, l1, false, 1, OUTPUT_BYTES / 32, 0, 0);
        else asc_copy_l12ub_sync(readback, l1, false, 1, OUTPUT_BYTES / 32, 0, 0);'''
    items["asc_copy_l12ub.asc"] = raw_source(
        "asc_copy_l12ub.asc", "AscCopyL12ubKernel", "asc_copy_l12ub/asc_copy_l12ub_sync", body, 64, 64)

    loop_cases = [
        ("asc_set_gm2l1_loop1_stride.asc", "AscSetGm2l1Loop1StrideKernel", "asc_set_gm2l1_loop1_stride",
         "asc_set_gm2l1_loop_size(2, 1);\n        asc_set_gm2l1_loop1_stride(64, 32);", 96, 64,
         '''    for (uint32_t i = 0; i < 32; ++i) { golden[i] = input[i]; golden[32 + i] = input[64 + i]; }'''),
        ("asc_set_gm2l1_loop2_stride.asc", "AscSetGm2l1Loop2StrideKernel", "asc_set_gm2l1_loop2_stride",
         "asc_set_gm2l1_loop_size(1, 2);\n        asc_set_gm2l1_loop2_stride(64, 32);", 96, 64,
         '''    for (uint32_t i = 0; i < 32; ++i) { golden[i] = input[i]; golden[32 + i] = input[64 + i]; }'''),
        ("asc_set_gm2l1_loop_size.asc", "AscSetGm2l1LoopSizeKernel", "asc_set_gm2l1_loop_size",
         "asc_set_gm2l1_loop_size(2, 2);\n        asc_set_gm2l1_loop1_stride(64, 32);\n        asc_set_gm2l1_loop2_stride(128, 64);", 224, 128,
         '''    for (uint32_t block = 0; block < 4; ++block) {
        for (uint32_t i = 0; i < 32; ++i) golden[block * 32 + i] = input[block * 64 + i];
    }'''),
    ]
    for rel, kernel, api, setup, inp, out, golden in loop_cases:
        body = f'''        {setup}
        asc_copy_gm2l1_align(l1, input, 1, 32, 0, 0, false, 0, 0, 0);
        asc_set_gm2l1_loop_size(1, 1);
''' + plain_readback
        items[rel] = raw_source(rel, kernel, api, body, inp, out, 1, golden)

    pad_golden = '''    for (uint32_t i = 0; i < 8; ++i) golden[i] = golden[24 + i] = 0xa5;
    for (uint32_t i = 0; i < 16; ++i) golden[8 + i] = input[i];'''
    body = '''        asc_set_gm2l1_pad(0xa5a5a5a5U);
        asc_copy_gm2l1_align(l1, input, 1, 16, 8, 8, false, 0, 0, 0);
''' + plain_readback
    items["asc_set_gm2l1_pad.asc"] = raw_source(
        "asc_set_gm2l1_pad.asc", "AscSetGm2l1PadKernel", "asc_set_gm2l1_pad", body, 16, 32, 1, pad_golden)
    items["asc_fill_l1.asc"] = fill_source()
    matrix_cases = [
        ("asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_arch_3510.asc", "AscCopyGm2l1Nd2nzKernel",
         "asc_copy_gm2l1_nd2nz/asc_copy_gm2l1_nd2nz_sync", "nd2nz"),
        ("asc_copy_l12l0a/asc_copy_l12l0a_arch_3510.asc", "AscCopyL12l0aKernel",
         "asc_copy_l12l0a/asc_copy_l12l0a_sync", "l12l0a"),
        ("asc_copy_l12l0b/asc_copy_l12l0b_arch_3510.asc", "AscCopyL12l0bKernel",
         "asc_copy_l12l0b/asc_copy_l12l0b_sync", "l12l0b"),
        ("asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_arch_3510.asc", "AscCopyL12l0bTransKernel",
         "asc_copy_l12l0b_trans/asc_copy_l12l0b_trans_sync", "l12l0b_trans"),
        ("asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.asc", "AscCopyL0c2gmKernel",
         "asc_copy_l0c2gm/asc_copy_l0c2gm_sync", "l0c2gm"),
    ]
    for rel, kernel, api, target in matrix_cases:
        items[rel] = matrix_source(rel, kernel, api, target)
    mx_cases = [
        ("asc_copy_gm2l1_dn2nz.asc", "AscCopyGm2l1Dn2nzKernel",
         "asc_copy_gm2l1_dn2nz/asc_copy_gm2l1_dn2nz_sync", "dn2nz"),
        ("asc_copy_l12l0a_mx.asc", "AscCopyL12l0aMxKernel",
         "asc_copy_l12l0a_mx/asc_copy_l12l0a_mx_sync", "l12l0a_mx"),
        ("asc_copy_l12l0b_mx.asc", "AscCopyL12l0bMxKernel",
         "asc_copy_l12l0b_mx/asc_copy_l12l0b_mx_sync", "l12l0b_mx"),
    ]
    for rel, kernel, api, target in mx_cases:
        items[rel] = mx_source(rel, kernel, api, target)
    items["asc_copy_l12fb/asc_copy_l12fb_arch_3510.asc"] = fb_source()
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
