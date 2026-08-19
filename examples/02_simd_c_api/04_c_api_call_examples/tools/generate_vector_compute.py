#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "vector_compute"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def header(name: str, extra_headers: str = "") -> str:
    rel = f"vector_compute/{name}"
    binary = f"/tmp/c_api_vector_compute_{name}"
    return f'''// Build/run ({PREFIX}/{rel}.asc): "$ASCEND_HOME_PATH/bin/bisheng" --npu-arch=dav-3510 -std=c++17 -Wno-deprecated-declarations --asc-aicore-lang {PREFIX}/{rel}.asc -lm -o {binary} && {binary}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
{extra_headers}#include "c_api/asc_simd.h"
#include "acl/acl.h"

namespace {{
'''


def bitsort_source() -> str:
    name = "asc_bitsort"
    return header(name, "#include <algorithm>\n#include <cstring>\n") + r'''
constexpr uint32_t COUNT = 32;
constexpr uint32_t PROPOSAL_WORDS = COUNT * 2;

__global__ __vector__ void AscBitsortKernel(
    __gm__ uint32_t* output, __gm__ float* scores, __gm__ uint32_t* indices)
{
    asc_init();
    __ubuf__ float local_scores[COUNT], sorted[PROPOSAL_WORDS];
    __ubuf__ uint32_t local_indices[COUNT];
    asc_copy_gm2ub_align(local_scores, scores, COUNT * sizeof(float));
    asc_copy_gm2ub_align(local_indices, indices, COUNT * sizeof(uint32_t));
    asc_sync_mte2(0);
    // Sort 32 score/index pairs in descending score order.
    asc_bitsort(sorted, local_scores, local_indices, 1);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
    asc_copy_ub2gm_align(output, reinterpret_cast<__ubuf__ uint32_t*>(sorted), PROPOSAL_WORDS * sizeof(uint32_t));
    asc_sync_mte3(0);
}

void PrintPairs(const char* label, const std::vector<uint32_t>& data)
{
    std::cout << label << ":";
    for (uint32_t i = 0; i < 8; ++i) {
        float score;
        std::memcpy(&score, &data[i * 2], sizeof(score));
        std::cout << " (" << score << ',' << data[i * 2 + 1] << ')';
    }
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    std::vector<float> scores(COUNT);
    std::vector<uint32_t> indices(COUNT), order(COUNT), output(PROPOSAL_WORDS), golden(PROPOSAL_WORDS);
    for (uint32_t i = 0; i < COUNT; ++i) {
        scores[i] = static_cast<float>((i * 13) % COUNT) - 16.0f;
        indices[i] = 100 + i;
        order[i] = i;
    }
    std::sort(order.begin(), order.end(), [&](uint32_t a, uint32_t b) { return scores[a] > scores[b]; });
    for (uint32_t i = 0; i < COUNT; ++i) {
        uint32_t score_bits;
        std::memcpy(&score_bits, &scores[order[i]], sizeof(score_bits));
        golden[i * 2] = score_bits;
        golden[i * 2 + 1] = indices[order[i]];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    float* scores_device = nullptr;
    uint32_t *indices_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&scores_device), COUNT * sizeof(float), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&indices_device), COUNT * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(scores_device, COUNT * sizeof(float), scores.data(), COUNT * sizeof(float), ACL_MEMCPY_HOST_TO_DEVICE);
    aclrtMemcpy(
        indices_device, COUNT * sizeof(uint32_t), indices.data(), COUNT * sizeof(uint32_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscBitsortKernel<<<1, 0>>>(output_device, scores_device, indices_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(
        output.data(), output.size() * sizeof(uint32_t), output_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintPairs("Sorted output", output);
    PrintPairs("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_bitsort passed." : "[Failed] asc_bitsort failed.") << std::endl;
    aclrtFree(scores_device);
    aclrtFree(indices_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
'''


def mrgsort_source() -> str:
    name = "asc_mrgsort4"
    return header(name, "#include <algorithm>\n#include <cstring>\n") + r'''
constexpr uint32_t QUEUES = 4;
constexpr uint32_t QUEUE_LENGTH = 4;
constexpr uint32_t PROPOSALS = QUEUES * QUEUE_LENGTH;
constexpr uint32_t WORDS = PROPOSALS * 2;

__global__ __vector__ void AscMrgsort4Kernel(__gm__ uint32_t* output, __gm__ uint32_t* input)
{
    asc_init();
    __ubuf__ uint32_t local_input[WORDS];
    __ubuf__ float merged[WORDS];
    asc_copy_gm2ub_align(local_input, input, WORDS * sizeof(uint32_t));
    asc_sync_mte2(0);
    __ubuf__ float* queues[ASC_C_API_MRGSORT_ELEMENT_LEN];
    for (uint32_t i = 0; i < QUEUES; ++i) {
        queues[i] = reinterpret_cast<__ubuf__ float*>(local_input + i * QUEUE_LENGTH * 2);
    }
    // Merge four sorted queues; valid_bit=15 enables all four inputs.
    asc_mrgsort4(merged, queues, 1, QUEUE_LENGTH, QUEUE_LENGTH, QUEUE_LENGTH, QUEUE_LENGTH, false, 15);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
    asc_copy_ub2gm_align(output, reinterpret_cast<__ubuf__ uint32_t*>(merged), WORDS * sizeof(uint32_t));
    asc_sync_mte3(0);
}

void PrintPairs(const char* label, const std::vector<uint32_t>& data)
{
    std::cout << label << ":";
    for (uint32_t i = 0; i < 8; ++i) {
        float score;
        std::memcpy(&score, &data[i * 2], sizeof(score));
        std::cout << " (" << score << ',' << data[i * 2 + 1] << ')';
    }
    std::cout << " ..." << std::endl;
}
} // namespace

int main()
{
    const float queue_scores[QUEUES][QUEUE_LENGTH] = {
        {40, 30, 20, 10}, {38, 28, 18, 8}, {36, 26, 16, 6}, {34, 24, 14, 4}};
    std::vector<uint32_t> input(WORDS), output(WORDS), golden(WORDS), order(PROPOSALS);
    for (uint32_t queue = 0; queue < QUEUES; ++queue) {
        for (uint32_t item = 0; item < QUEUE_LENGTH; ++item) {
            const uint32_t proposal = queue * QUEUE_LENGTH + item;
            std::memcpy(&input[proposal * 2], &queue_scores[queue][item], sizeof(float));
            input[proposal * 2 + 1] = 100 * queue + item;
            order[proposal] = proposal;
        }
    }
    std::sort(order.begin(), order.end(), [&](uint32_t a, uint32_t b) {
        float lhs, rhs;
        std::memcpy(&lhs, &input[a * 2], sizeof(lhs));
        std::memcpy(&rhs, &input[b * 2], sizeof(rhs));
        return lhs > rhs;
    });
    for (uint32_t i = 0; i < PROPOSALS; ++i) {
        golden[i * 2] = input[order[i] * 2];
        golden[i * 2 + 1] = input[order[i] * 2 + 1];
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint32_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), WORDS * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), output.size() * sizeof(uint32_t), ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(
        input_device, WORDS * sizeof(uint32_t), input.data(), WORDS * sizeof(uint32_t), ACL_MEMCPY_HOST_TO_DEVICE);
    AscMrgsort4Kernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(
        output.data(), output.size() * sizeof(uint32_t), output_device, output.size() * sizeof(uint32_t),
        ACL_MEMCPY_DEVICE_TO_HOST);
    PrintPairs("Merged output", output);
    PrintPairs("Golden", golden);
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_mrgsort4 passed." : "[Failed] asc_mrgsort4 failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
'''


def transpose_source() -> str:
    name = "asc_transpose"
    return header(name) + r'''
constexpr uint32_t SIDE = 16;
constexpr uint32_t ELEMENTS = SIDE * SIDE;
constexpr uint32_t BYTES = ELEMENTS * sizeof(uint16_t);

__global__ __vector__ void AscTransposeKernel(__gm__ uint16_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint16_t src[ELEMENTS], dst[ELEMENTS];
    asc_copy_gm2ub_align(src, input, BYTES);
    asc_sync_mte2(0);
    // Transpose one 16x16 uint16_t matrix in UB.
    asc_transpose(dst, src);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
    asc_copy_ub2gm_align(output, dst, BYTES);
    asc_sync_mte3(0);
}
} // namespace

int main()
{
    std::vector<uint16_t> input(ELEMENTS), output(ELEMENTS), golden(ELEMENTS);
    for (uint32_t row = 0; row < SIDE; ++row) {
        for (uint32_t column = 0; column < SIDE; ++column) {
            input[row * SIDE + column] = row * 100 + column;
            golden[row * SIDE + column] = column * 100 + row;
        }
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BYTES, input.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    AscTransposeKernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES, output_device, BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "Input row 0:";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << input[i];
    std::cout << " ...\nOutput row 0:";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << output[i];
    std::cout << " ..." << std::endl;
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_transpose passed." : "[Failed] asc_transpose failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
'''


def set_va_source() -> str:
    name = "asc_set_va_reg"
    return header(name) + r'''
constexpr uint32_t SIDE = 16;
constexpr uint32_t ELEMENTS = SIDE * SIDE;
constexpr uint32_t BYTES = ELEMENTS * sizeof(uint16_t);

__global__ __vector__ void AscSetVaRegKernel(__gm__ uint16_t* output, __gm__ uint16_t* input)
{
    asc_init();
    __ubuf__ uint16_t src[ELEMENTS], first[ELEMENTS], rebound[ELEMENTS];
    asc_copy_gm2ub_align(src, input, BYTES);
    asc_sync_mte2(0);
    __ubuf__ uint16_t* src_list[SIDE];
    __ubuf__ uint16_t* first_list[SIDE];
    __ubuf__ uint16_t* rebound_list[SIDE];
    for (uint32_t i = 0; i < SIDE; ++i) {
        src_list[i] = src + i * SIDE;
        first_list[i] = first + i * SIDE;
        rebound_list[i] = rebound + i * SIDE;
    }
    asc_set_va_reg(VA0, first_list);
    asc_set_va_reg(VA1, first_list + 8);
    asc_set_va_reg(VA2, src_list);
    asc_set_va_reg(VA3, src_list + 8);
    asc_transto5hd_b16(VA0, VA2, 1, 0, 0);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
    asc_set_va_reg(VA0, rebound_list);
    asc_set_va_reg(VA1, rebound_list + 8);
    // Reusing VA0 redirects the same transpose to rebound without changing the source VA.
    asc_transto5hd_b16(VA0, VA2, 1, 0, 0);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
    asc_copy_ub2gm_align(output, first, BYTES);
    asc_copy_ub2gm_align(output + ELEMENTS, rebound, BYTES);
    asc_sync_mte3(0);
}
} // namespace

int main()
{
    std::vector<uint16_t> input(ELEMENTS), output(ELEMENTS * 2), golden(ELEMENTS * 2);
    for (uint32_t row = 0; row < SIDE; ++row) {
        for (uint32_t column = 0; column < SIDE; ++column) {
            input[row * SIDE + column] = row * 100 + column;
            golden[row * SIDE + column] = golden[ELEMENTS + row * SIDE + column] = column * 100 + row;
        }
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint16_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), BYTES * 2, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, BYTES, input.data(), BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    AscSetVaRegKernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), BYTES * 2, output_device, BYTES * 2, ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "VA0 first target:";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << output[i];
    std::cout << " ...\nVA0 rebound target:";
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << output[ELEMENTS + i];
    std::cout << " ..." << std::endl;
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_set_va_reg rebind passed."
                         : "[Failed] asc_set_va_reg rebind failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
'''


def transto5hd_source() -> str:
    name = "asc_transto5hd"
    return header(name) + r'''
constexpr uint32_t INPUT_BYTES = 1536;
constexpr uint32_t OUTPUT_BYTES = 2560;

__global__ __vector__ void AscTransto5hdKernel(__gm__ uint8_t* output, __gm__ uint8_t* input)
{
    asc_init();
    __ubuf__ uint16_t src16[256], dst16[512];
    __ubuf__ uint32_t src32[128], dst32[256];
    __ubuf__ uint8_t src8[512], dst8[512];
    asc_copy_gm2ub_align(src16, reinterpret_cast<__gm__ uint16_t*>(input), 512);
    asc_copy_gm2ub_align(src32, reinterpret_cast<__gm__ uint32_t*>(input + 512), 512);
    asc_copy_gm2ub_align(src8, input + 1024, 512);
    asc_sync_mte2(0);

    __ubuf__ uint16_t *src16_list[16], *dst16_list[16], *dst16_second_list[16];
    for (uint32_t i = 0; i < 16; ++i) {
        src16_list[i] = src16 + i * 16;
        dst16_list[i] = dst16 + i * 16;
        dst16_second_list[i] = dst16 + 256 + i * 16;
    }
    asc_set_va_reg(VA0, dst16_list);
    asc_set_va_reg(VA1, dst16_list + 8);
    asc_set_va_reg(VA2, src16_list);
    asc_set_va_reg(VA3, src16_list + 8);
    // Transpose one b16 tile addressed by 16 source and destination VA entries.
    asc_transto5hd_b16(VA0, VA2, 1, 0, 0);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
    asc_set_va_reg(VA4, dst16_second_list);
    asc_set_va_reg(VA5, dst16_second_list + 8);
    asc_transto5hd_b16(VA4, VA2, 1, 0, 0);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);

    __ubuf__ uint32_t *src32_list[16], *dst32_list[16], *dst32_second_list[16];
    for (uint32_t i = 0; i < 16; ++i) {
        src32_list[i] = src32 + i * 8;
        dst32_list[i] = dst32 + i * 8;
        dst32_second_list[i] = dst32 + 128 + i * 8;
    }
    asc_set_va_reg(VA0, dst32_list);
    asc_set_va_reg(VA1, dst32_list + 8);
    asc_set_va_reg(VA2, src32_list);
    asc_set_va_reg(VA3, src32_list + 8);
    // Transpose one b32 tile addressed by 16 source and destination VA entries.
    asc_transto5hd_b32(VA0, VA2, 1, 0, 0);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
    asc_set_va_reg(VA4, dst32_second_list);
    asc_set_va_reg(VA5, dst32_second_list + 8);
    asc_transto5hd_b32(VA4, VA2, 1, 0, 0);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);

    __ubuf__ uint8_t *src8_list[16], *dst8_list[16];
    for (uint32_t i = 0; i < 16; ++i) {
        src8_list[i] = src8 + i * 32;
        dst8_list[i] = dst8 + i * 32;
    }
    asc_set_va_reg(VA0, dst8_list);
    asc_set_va_reg(VA1, dst8_list + 8);
    asc_set_va_reg(VA2, src8_list);
    asc_set_va_reg(VA3, src8_list + 8);
    // Fill the lower 16 bytes of each b8 destination block first.
    asc_transto5hd_b8(VA0, VA2, 1, 0, 0, false, false);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
    // high_half=true and dst_high_half=true fill the upper 16 bytes of every output block.
    asc_transto5hd_b8(VA0, VA2, 1, 0, 0, true, true);
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);

    asc_copy_ub2gm_align(reinterpret_cast<__gm__ uint16_t*>(output), dst16, 1024);
    asc_copy_ub2gm_align(reinterpret_cast<__gm__ uint32_t*>(output + 1024), dst32, 1024);
    asc_copy_ub2gm_align(output + 2048, dst8, 512);
    asc_sync_mte3(0);
}
} // namespace

int main()
{
    std::vector<uint8_t> input(INPUT_BYTES), output(OUTPUT_BYTES), golden(OUTPUT_BYTES);
    auto* input16 = reinterpret_cast<uint16_t*>(input.data());
    auto* golden16 = reinterpret_cast<uint16_t*>(golden.data());
    for (uint32_t row = 0; row < 16; ++row) {
        for (uint32_t column = 0; column < 16; ++column) {
            input16[row * 16 + column] = row * 100 + column;
            golden16[row * 16 + column] = golden16[256 + row * 16 + column] = column * 100 + row;
        }
    }
    auto* input32 = reinterpret_cast<uint32_t*>(input.data() + 512);
    auto* golden32 = reinterpret_cast<uint32_t*>(golden.data() + 1024);
    for (uint32_t row = 0; row < 16; ++row) {
        for (uint32_t column = 0; column < 8; ++column) input32[row * 8 + column] = row * 100 + column;
    }
    for (uint32_t list = 0; list < 16; ++list) {
        for (uint32_t lane = 0; lane < 8; ++lane) {
            const uint32_t row = (list % 2) * 8 + lane;
            const uint32_t value = input32[row * 8 + list / 2];
            golden32[list * 8 + lane] = golden32[128 + list * 8 + lane] = value;
        }
    }
    uint8_t* input8 = input.data() + 1024;
    uint8_t* golden8 = golden.data() + 2048;
    for (uint32_t row = 0; row < 16; ++row) {
        for (uint32_t column = 0; column < 32; ++column) input8[row * 32 + column] = row * 10 + column;
    }
    for (uint32_t list = 0; list < 16; ++list) {
        for (uint32_t row = 0; row < 16; ++row) {
            golden8[list * 32 + row] = input8[row * 32 + list];
            golden8[list * 32 + 16 + row] = input8[row * 32 + 16 + list];
        }
    }
    aclInit(nullptr);
    aclrtSetDevice(0);
    uint8_t *input_device = nullptr, *output_device = nullptr;
    aclrtMalloc(reinterpret_cast<void**>(&input_device), INPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMalloc(reinterpret_cast<void**>(&output_device), OUTPUT_BYTES, ACL_MEM_MALLOC_HUGE_FIRST);
    aclrtMemcpy(input_device, INPUT_BYTES, input.data(), INPUT_BYTES, ACL_MEMCPY_HOST_TO_DEVICE);
    AscTransto5hdKernel<<<1, 0>>>(output_device, input_device);
    aclrtSynchronizeDevice();
    aclrtMemcpy(output.data(), OUTPUT_BYTES, output_device, OUTPUT_BYTES, ACL_MEMCPY_DEVICE_TO_HOST);
    std::cout << "b16 output row 0:";
    auto* output16 = reinterpret_cast<uint16_t*>(output.data());
    for (uint32_t i = 0; i < 8; ++i) std::cout << ' ' << output16[i];
    std::cout << " ...\nb32 output blocks 0/1:";
    auto* output32 = reinterpret_cast<uint32_t*>(output.data() + 1024);
    for (uint32_t i = 0; i < 16; ++i) std::cout << ' ' << output32[i];
    std::cout << "\nb8 output block 0:";
    for (uint32_t i = 2048; i < 2080; ++i) std::cout << ' ' << +output[i];
    std::cout << std::endl;
    const bool passed = output == golden;
    std::cout << (passed ? "[Success] asc_transto5hd b8/b16/b32 passed."
                         : "[Failed] asc_transto5hd failed.") << std::endl;
    aclrtFree(input_device);
    aclrtFree(output_device);
    aclrtResetDevice(0);
    aclFinalize();
    return passed ? 0 : 1;
}
'''


def main() -> None:
    cases = {
        "asc_bitsort": bitsort_source(),
        "asc_mrgsort4": mrgsort_source(),
        "asc_set_va_reg": set_va_source(),
        "asc_transpose": transpose_source(),
        "asc_transto5hd": transto5hd_source(),
    }
    OUT.mkdir(parents=True, exist_ok=True)
    for name, text in cases.items():
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    print(f"Generated {len(cases)} functional vector compute examples in {OUT}")


if __name__ == "__main__":
    main()
