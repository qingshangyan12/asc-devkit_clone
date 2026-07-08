# Experimental BesselI0

`prims_bessel_i0` 是 `AscendC::BesselI0` 高阶 API 的自包含 experimental 交付包。该包不接入仓库主 `include` / `impl` / `tests` 树，使用方可通过增加本目录的 `include` 路径直接验证。

## API

```cpp
#include "adv_api/math/bessel_i0.h"

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const AscendC::LocalTensor<T>& dstTensor,
    const AscendC::LocalTensor<T>& srcTensor,
    const uint32_t calCount);

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const AscendC::LocalTensor<T>& dstTensor,
    const AscendC::LocalTensor<T>& srcTensor);
```

首版支持范围：

- `T=float`
- `__NPU_ARCH__ == 3510` / `dav-3510`
- AI Vector Core
- ND elementwise，不支持 broadcast
- `isReuseSource` 保留为兼容模板参数，当前实现不修改输入

Reg 实现不需要额外 UB 临时空间，因此没有新增 `sharedTmpBuffer` 重载或 tiling-size API。`srcTensor` / `dstTensor` 需要满足 Ascend C LocalTensor 32B 对齐访问要求。

## Layout

```text
prims_bessel_i0
├── docs
│   └── prims_bessel_i0_design.md
├── impl
│   └── adv_api/detail/math/bessel_i0/bessel_i0_3510_impl.h
├── include
│   └── adv_api/math/bessel_i0.h
└── tests
    ├── self_test
    └── ut
```

## Implementation Notes

`dav-3510` 实现位于 `impl/adv_api/detail/math/bessel_i0/bessel_i0_3510_impl.h`，按 `GetVecLen() / sizeof(float)` 分 repeat 处理，尾部通过 `Reg::UpdateMask<float>(validCount)` 控制有效写回。

数值算法转写自现有 SIMT `CylBesselI0` 的 9.0 分段公式：

- `abs(x) < 9.0` 使用 `abs(x)^2` Horner 多项式
- `abs(x) >= 9.0` 使用 `1 / abs(x)` 多项式，配合 `Sqrt + Div` 得到 `1 / sqrt(abs(x))`
- 分支通过 `Reg::Compare<CMPMODE::GE>` 和 `Reg::Select` 合并
- `+Inf` / `-Inf` 最后覆盖为 `+Inf`，`NaN` 由多项式自然传播

## Self Test

在 CANN 9.1.0 + Ascend 950PR 环境中运行：

```bash
cd C:/Code/cann/asc-devkit-cy/experimental/prims_bessel_i0/tests/self_test
bash run.sh -r cpu --size 1 --is_perf 0
bash run.sh -r cpu --size 32 --is_perf 0
bash run.sh -r cpu --size 1023 --is_perf 0
bash run.sh -r cpu --size 2048 --is_perf 0
```

可用仿真环境时，将 `-r cpu` 替换为 `-r sim` 做同样功能验证。性能验证需真实 NPU：

```bash
bash run.sh -r npu --size 1024 --is_perf 1
bash run.sh -r npu --size 4096 --is_perf 1
bash run.sh -r npu --size 8192 --is_perf 1
bash run.sh -r npu --size 16384 --is_perf 1
bash run.sh -r npu --size 32768 --is_perf 1
bash run.sh -r npu --size 65536 --is_perf 1
```

## Compile-Only API Check

`tests/ut` 提供轻量编译样例，用于实例化：

- `BesselI0<float, false>(dst, src, calCount)`
- `BesselI0<float, false>(dst, src)`

该样例仅用于头文件/符号编译检查，不做大矩阵功能验证。
