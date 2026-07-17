# experimental/ — Ndtri 高阶API

本目录为 [7月社区任务 prims.ndtri](https://gitcode.com/cann/cann-ops-competitions/tree/master/04_tasks/01_community-task-2026/docs/202607) 的交付件。目录结构镜像主仓路径，评审通过后可直接 `git mv` 到主仓对应位置。

```text
experimental/
├── docs/                                              # API 接口说明文档
│   └── Ndtri接口/
├── include/adv_api/math/ndtri.h                       # API 声明（4个重载）
├── include/adv_api/math/ndtri_tiling.h                # Tiling 接口声明
├── impl/adv_api/detail/math/ndtri/ndtri_3510_impl.h   # 核心实现（Reg 寄存器级）
├── impl/adv_api/tiling/math/ndtri_tiling_impl.cpp     # Tiling 实现
└── tests/
    ├── api/adv_api/math/ndtri/test_operator_ndtri.cpp # UT（接口/参数校验/非对齐）
    └── functional/ndtri/                              # 功能+性能自验证用例（对齐 scipy）
```

## 接口

```cpp
template <typename T, bool isReuseSource = false>
void Ndtri(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t calCount);
void Ndtri(const LocalTensor<T>& dst, const LocalTensor<T>& src);
void Ndtri(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint8_t>& tmp);
void Ndtri(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint8_t>& tmp, uint32_t calCount);
```

- **T**：`float`（对齐 PyTorch `prims.ndtri`）
- **产品**：Ascend 950PR（`__NPU_ARCH__ == 3510`）
- **临时空间**：**不需要**。`GetNdtriMaxMinTmpSize` 返回 0/0；计算全程寄存器驻留，UB 每个 repeat 仅访问两次（1次 load / 1次 store）。`sharedTmpBuffer` 重载仅为接口一致性保留，不会被访问。
- **边界**：`p<0 或 p>1 -> NaN`；`p==0 -> -Inf`；`p==1 -> +Inf`；`p==0.5 -> 0`；`NaN -> NaN`（与 scipy/torch 一致）

## 算法（混合，与任务书的 Acklam 有偏差，见设计文档）

| 分支 | 条件 | 公式 |
|---|---|---|
| central | \|p-0.5\| ≤ 0.47575 | z = √(2π)·q·(1 + r·P0(r)/Q0(r))，q=p−0.5, r=q² |
| tail | 其他 | z = ±C(u)/D(u)，u = √(−2·ln(min(p,1−p))) |

**tail 用 Acklam 系数**（fp32 实测 3.7e-7，条件良好，直接采用）。

**central 未用 Acklam，改用 Cephes 因式分解形式**——这是本实现最关键的决策：Acklam 的 `A(r)·q/B(r)` 在 fp32 下**灾难性抵消**（Horner 中间量峰值 ~276，在分段点附近坍缩到 ~0.01，放大约 21000×，误差下限 1.3e-3）**无法满足双万分之一**。注意 Acklam 的*逼近*误差没问题（fp64 下 1.1e-9），坏的是 **fp32 下的求值条件数**——其系数是为 double 设计的。Cephes 把有理式变成 [0, 0.655] 区间内的**有界修正项**，主项 q 占优，不发生抵消。

## 实测结果

**精度**（fp32，对齐 `scipy.special.ndtri`，函数形状采样：51.3% 尾部点、258 个深尾点、z ∈ [−12.785, +5.295]）：

| shape | max rel err | 超差 |
|---|---|---|
| 1 | 0.000e+00 | 0/1 |
| 32 | 3.607e-05 | 0/32 |
| 1023 | 4.035e-05 | 0/1023 |
| 2048 | 4.478e-05 | 0/2048 |

**950PR 硅片、CANN 9.1.0 GA 与两个模拟器（9.0.1 / 9.1.0-beta.3）逐位一致**。双万分之一（1e-4）满足，余量约 2.2×。

**性能**（aiv_vec 占比，门限 90%）：

| 指标 | 结果 |
|---|---|
| 任务书口径（每 tile 重算 1000 次） | ~100%（1k/4k/8k/16k/32k） |
| **真实单次口径**（PERF_LOOPS=1, 128k） | **98%** |

真实单次口径下 N=128k 实测：总 128,892 cycle = MTE2 14,744 + Vector 106,083 + MTE3 8,065 → 未开双缓冲时 V 被拷贝阻塞 17.7%，aiv_vec 仅 **82.3%**。功能测试 wrapper 开启 `BUFFER_NUM=2` 双缓冲后 → **98%**（剩余 ~2% 为流水 fill/drain，16 tile 下不可消除）。

> 注：任务书的 1000 次循环口径会把拷贝摊薄到 0.1%，且 mode2/mode3 的拷贝在 `t3−t2` 相减时相互抵消，因此**在任何 shape 下都恒为 ~100%**，无法反映流水线真实情况。本实现在**真实单次口径**下同样通过 90% 门限。

## 自验证

```bash
cd tests/functional/ndtri
source ${install_path}/cann/set_env.sh
./run.sh npu --perf                      # 功能 + 性能（任务书口径）
PERF_LOOPS=1 ./run.sh npu --perf-only    # 真实单次口径
```

单次运行（size/mode/loops 均为运行时参数，无需重编译）：

```bash
cd build && python3 ../scripts/gen_data.py 2048 && ./demo 2048 1
# ./demo <size> <mode 1|2|3> [loops]    1=精度 2=纯搬运基线 3=计算循环
```

## 合入主仓时需要的注册（本目录未改动主仓文件）

1. `include/adv_api/kernel_api.h` → `#include "math/ndtri.h"`
2. `include/adv_api/tiling_api.h` → `#include "math/ndtri_tiling.h"`
3. `tests/api/adv_api/CMakeLists.txt` → 在 `ASCENDC_TEST_ascend950pr_9599_AIV_CASE_SRC_FILES` 中加入
   `math/ndtri/test_operator_ndtri.cpp`
4. Tiling UT → `tests/api/adv_api/tiling/test_tiling.cpp` + `ASCENDC_TILING_TEST_SRC_FILES`
