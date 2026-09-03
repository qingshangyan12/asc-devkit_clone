在 Ascend 950（dav-3510）上，一个极简的 __vector__ kernel 中调用：
int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
asc_icache_preload(reinterpret_cast<void *>(pc), 8);
之后 kernel 本身和紧随其后的 torch.npu.synchronize() 都能正常返回，但接下来执行无关的 NPU 算子（例如 torch.zeros(..., dtype=torch.bool)、fill_、masked_fill 等）会报 NPU error 507035。如果去掉 asc_icache_preload，同一个 kernel 和同一个后续算子都能正常执行。
最小复现代码
#!/usr/bin/env python3"""Minimal reproducer without asckit.

Usage:
    python3 mwe_icache_preload_wo_asckit.py --no-preload   # PROBE_OK
    python3 mwe_icache_preload_wo_asckit.py --preload      # PROBE_FAIL
"""import argparse
import ctypes
import os
import shutil
import subprocess
import tempfile

import torch
import torch_npu


class AscendKernel:
    def __init__(self, code: str, extra_flags=()):
        src = tempfile.NamedTemporaryFile("w", suffix=".asc", delete=False)
        src.write(code)
        src.close()
        so = src.name.replace(".asc", ".so")
        arch = os.environ.get("ASCEND_NPU_ARCH", "dav-3510")
        inc = os.path.join(
            os.environ.get(
                "ASCEND_HOME_PATH", "/usr/local/Ascend/ascend-toolkit/latest"
            ),
            "aarch64-linux/asc/include",
        )
        r = subprocess.run(
            [
                shutil.which("bisheng"),
                "-fPIC",
                "--shared",
                f"--npu-arch={arch}",
                f"-I{inc}",
                *extra_flags,
                "-o",
                so,
                src.name,
            ],
            capture_output=True,
            text=True,
        )
        os.remove(src.name)
        if r.returncode:
            print("=== COMPILE STDERR (last 3000 chars) ===")
            print(r.stderr[-3000:])
            raise RuntimeError("Compile failed!")
        self.lib = ctypes.CDLL(so)
        self.lib.init()

    def __call__(self, *tensors, scalars=()):
        stream = torch.npu.current_stream()
        args = [ctypes.c_void_p(t.data_ptr()) for t in tensors]
        args += list(scalars) + [ctypes.c_void_p(stream.npu_stream)]
        self.lib.call(*args)


def build_source() -> str:
    return r'''
#include "acl/acl.h"
#include "kernel_operator.h"
#include "c_api/sys_var/sys_var.h"
#include "c_api/cache_ctrl/cache_ctrl.h"

using namespace AscendC;

extern "C" __global__ __vector__
void icache_probe_kernel(__gm__ float* x, int64_t n) {
#ifdef USE_PRELOAD
    int64_t pc = asc_get_program_counter() & 0xFFFFFFFFFFFF;
    asc_icache_preload(reinterpret_cast<void *>(pc), 8);
#endif
    for (int64_t i = 0; i < n; ++i) {
        x[i] = x[i] + 1.0f;
    }
}

extern "C" int init() { return 0; }

extern "C" int call(float* x, int64_t n, void* stream) {
    icache_probe_kernel<<<1, nullptr, stream>>>(x, n);
    return 0;
}
'''def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--preload", action="store_true")
    parser.add_argument("--no-preload", action="store_true")
    args = parser.parse_args()
    use_preload = args.preload and not args.no_preload

    torch.npu.set_device("npu:0")
    print("Compiling ...", flush=True)
    flags = ["-DUSE_PRELOAD"] if use_preload else []
    kernel = AscendKernel(build_source(), extra_flags=flags)
    print("  OK", flush=True)

    x = torch.zeros(4, dtype=torch.float32, device="npu")
    kernel(x, scalars=(ctypes.c_int64(4),))
    torch.npu.synchronize()

    try:
        y = torch.zeros(1, 128, dtype=torch.bool, device="npu")
        torch.npu.synchronize()
    except Exception as exc:
        print("PROBE_FAIL", repr(exc))
        raise SystemExit(1) from exc

    print("PROBE_OK")


if __name__ == "__main__":
    main()
代码行为
运行 python3 mwe_icache_preload_wo_asckit.py --no-preload：输出 PROBE_OK。
运行 python3 mwe_icache_preload_wo_asckit.py --preload：输出 PROBE_FAIL，并伴随：
RuntimeError: npuSynchronizeDevice: ... NPU function error: device error type 3, error code is 507035
期望行为
asc_icache_preload 是 kernel 内部的指令缓存预取操作，不应影响 kernel 结束之后其他独立 NPU 算子的执行。期望在调用过该 preload 的 kernel 之后，torch.zeros、fill_、masked_fill 等普通 PyTorch NPU 算子仍能正常执行。
当 icache preload 的大小过大时，本 kernel 和 npu synchronize 都不会报错，但后面的其他 kernel 有可能报错，这个行为符合预期吗
