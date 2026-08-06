#!/usr/bin/env python3

from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "sync"
PREFIX = "examples/02_simd_c_api/04_c_api_call_examples"


def source(name: str, body: str, api: str, kernel_type: str = "__vector__") -> str:
    rel = f"sync/{name}"
    kernel = "".join(part.capitalize() for part in name.split("_")) + "Kernel"
    return f'''// Build/run ({PREFIX}/{rel}.asc): bash {PREFIX}/run_example.sh {rel}
// Architecture: dav-3510

#include <cstdint>
#include <iostream>
#include <vector>
#include "c_api/asc_simd.h"
#include "common/sample_host.h"

namespace {{
constexpr uint32_t ELEMENTS = 8;

__global__ {kernel_type} void {kernel}(__gm__ uint32_t* output)
{{
    asc_init();
{body}
    asc_sync();
}}
}} // namespace

int main()
{{
    std::vector<uint32_t> input(ELEMENTS, 0), output(ELEMENTS, 0xffffffffU), golden(ELEMENTS, 0);
    golden[0] = 1;
    CApiExample::Runtime runtime;
    if (!runtime.Init()) return 1;
    CApiExample::DeviceBuffer<uint32_t> output_device(ELEMENTS);
    if (!output_device.Allocate("aclrtMalloc(output)") || !output_device.CopyFrom(input)) return 1;
    {kernel}<<<1, 0>>>(output_device.Get());
    if (!CApiExample::CheckAcl(aclrtSynchronizeDevice(), "aclrtSynchronizeDevice") ||
        !output_device.CopyTo(output)) return 1;
    CApiExample::Print("Input", input);
    CApiExample::Print("Output", output);
    CApiExample::Print("Golden", golden);
    const bool passed = CApiExample::CompareRange(output, golden, 0, 1);
    std::cout << (passed ? "[Success] {api} passed." : "[Failed] {api} failed.") << std::endl;
    return passed ? 0 : 1;
}}
'''


def main() -> None:
    OUT.mkdir(parents=True, exist_ok=True)
    pair_block = "    asc_sync_block_arrive(PIPE_V, static_cast<int64_t>(1));\n    asc_sync_block_wait(PIPE_V, static_cast<int64_t>(1));\n    output[0] = 1;"
    pair_inter = "    asc_sync_inter_arrive(PIPE_V, static_cast<int64_t>(1));\n    asc_sync_inter_wait(PIPE_V, static_cast<int64_t>(1));\n    output[0] = 1;"
    pair_subblock = "    asc_sync_subblock_arrive(PIPE_V, static_cast<int64_t>(1));\n    asc_sync_subblock_wait(PIPE_V, static_cast<int64_t>(1));\n    output[0] = 1;"
    pair_event = "    asc_sync_notify(PIPE_S, PIPE_V, EVENT_ID0);\n    asc_sync_wait(PIPE_S, PIPE_V, EVENT_ID0);\n    output[0] = 1;"
    intra = '''    constexpr uint64_t sync_id = 1;
    if ASC_IS_AIV {
        asc_sync_intra_arrive(PIPE_V, sync_id);
        output[0] = 1;
    }
    if ASC_IS_AIC {
        asc_sync_intra_wait(PIPE_FIX, sync_id);
        output[0] = 1;
    }'''
    cases = {
        "asc_lock": source("asc_lock", "    asc_lock(PIPE_S, static_cast<uint8_t>(1));\n    asc_unlock(PIPE_S, static_cast<uint8_t>(1));\n    output[0] = 1;", "asc_lock"),
        "asc_unlock": source("asc_unlock", "    asc_lock(PIPE_S, static_cast<uint8_t>(1));\n    asc_unlock(PIPE_S, static_cast<uint8_t>(1));\n    output[0] = 1;", "asc_unlock"),
        "asc_sync_block_arrive": source("asc_sync_block_arrive", pair_block, "asc_sync_block_arrive"),
        "asc_sync_block_wait": source("asc_sync_block_wait", pair_block, "asc_sync_block_wait"),
        "asc_sync_data_barrier": source("asc_sync_data_barrier", "    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);\n    output[0] = 1;", "asc_sync_data_barrier"),
        "asc_sync_inter_arrive": source("asc_sync_inter_arrive", pair_inter, "asc_sync_inter_arrive"),
        "asc_sync_inter_wait": source("asc_sync_inter_wait", pair_inter, "asc_sync_inter_wait"),
        "asc_sync_intra_arrive": source("asc_sync_intra_arrive", intra, "asc_sync_intra_arrive", "__mix__(1, 1)"),
        "asc_sync_intra_wait": source("asc_sync_intra_wait", intra, "asc_sync_intra_wait", "__mix__(1, 1)"),
        "asc_sync": source("asc_sync", "    asc_sync();\n    output[0] = 1;", "asc_sync"),
        "asc_sync_mte2": source("asc_sync_mte2", "    asc_sync_mte2(0);\n    output[0] = 1;", "asc_sync_mte2"),
        "asc_sync_mte3": source("asc_sync_mte3", "    asc_sync_mte3(0);\n    output[0] = 1;", "asc_sync_mte3"),
        "asc_sync_notify": source("asc_sync_notify", pair_event, "asc_sync_notify"),
        "asc_sync_pipe": source("asc_sync_pipe", "    asc_sync_pipe(PIPE_ALL);\n    output[0] = 1;", "asc_sync_pipe"),
        "asc_sync_subblock_arrive": source("asc_sync_subblock_arrive", pair_subblock, "asc_sync_subblock_arrive"),
        "asc_sync_subblock_wait": source("asc_sync_subblock_wait", pair_subblock, "asc_sync_subblock_wait"),
        "asc_sync_vec": source("asc_sync_vec", "    asc_sync_vec();\n    output[0] = 1;", "asc_sync_vec"),
        "asc_sync_wait": source("asc_sync_wait", pair_event, "asc_sync_wait"),
    }
    for name, text in cases.items():
        (OUT / f"{name}.asc").write_text(text, encoding="utf-8")
    print(f"Generated {len(cases)} synchronization examples in {OUT}")


if __name__ == "__main__":
    main()
