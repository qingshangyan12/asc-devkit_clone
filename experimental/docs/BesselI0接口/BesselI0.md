# experimental/ — BesselI0 高阶API

本文档为 [7月社区任务 prims.bessel_i0](https://gitcode.com/cann/cann-ops-competitions/tree/master/04_tasks/01_community-task-2026/docs/202607) 的交付件说明。目录结构镜像主仓路径，评审通过后可直接 `git mv` 到主仓对应位置。与 [Ndtri](../Ndtri接口/Ndtri.md) 同骨架、同设备、同自验证框架。

```text
experimental/
├── include/adv_api/math/bessel_i0.h                         # API 声明（4个重载）
├── include/adv_api/math/bessel_i0_tiling.h                  # Tiling 接口声明
├── impl/adv_api/detail/math/bessel_i0/bessel_i0_3510_impl.h # 核心实现（Reg 寄存器级）
├── impl/adv_api/tiling/math/bessel_i0_tiling_impl.cpp       # Tiling 实现
└── tests/
    ├── api/adv_api/math/bessel_i0/test_operator_bessel_i0.cpp # UT（接口/参数校验/非对齐）
    └── functional/bessel_i0/                                  # 功能+性能自验证用例（对齐 scipy.special.i0）
```

## 接口

```cpp
template <typename T, bool isReuseSource = false>
void BesselI0(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t calCount);
void BesselI0(const LocalTensor<T>& dst, const LocalTensor<T>& src);
void BesselI0(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint8_t>& tmp);
void BesselI0(const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint8_t>& tmp, uint32_t calCount);
```

- **T**：`float`（对齐 PyTorch `prims.bessel_i0` / `torch.i0`）
- **产品**：Ascend 950PR（`__NPU_ARCH__ == 3510`）
- **临时空间**：**不需要**。`GetBesselI0MaxMinTmpSize` 返回 0/0；计算全程寄存器驻留，UB 每个 repeat 仅访问两次（1次 load / 1次 store）。`sharedTmpBuffer` 重载仅为接口一致性保留，不会被访问。
  > ⚠ 该结论依赖片上 `Reg::Exp` 精度达标（见下）。若 region2 因 `Reg::Exp` 精度不足需改用「整数部分 vexp + 小数部分 Taylor」精化，则需约 256B 临时空间，`GetBesselI0MaxMinTmpSize` 将返回非 0。
- **边界**（与 `scipy.special.i0`、`torch.i0` 一致，已核对）：
  - `x==0 -> 1`（region1 t=0 精确，自然）；`NaN -> NaN`（region2 传播，自然）；`|x|` 溢出 fp32（约 |x|>91.9）`-> +Inf`（r2 自然溢出）。
  - `±Inf -> NaN`：**显式 Select**（非自然）。自然路径 region2 = `exp(Inf)·(P2S/√Inf)` = `exp(Inf)·0`，仅当片上 `Reg::Exp(Inf)==+Inf` 时才 = NaN；若硬件 exp **钳位到 FLT_MAX**，则 `FLT_MAX·0 = 0`（错）。故用 `a==Inf` 强制 NaN，对两种 exp 行为都稳。`a=|x|` 同时覆盖 ±Inf。

## 算法（Abramowitz & Stegun 两段有理逼近；I0 为偶函数，取 a=|x|，分段点 3.75）

| 分支 | 条件 | 公式 |
|---|---|---|
| region1 | a ≤ 3.75 | I0 = P1(t)，t = (a/3.75)² |
| region2 | a > 3.75 | I0 = exp(a−3.75)·P2S(u)/√a，u = 3.75/a，P2S = P2·e³·⁷⁵ |

**region2 采用 shift-fold（移位折叠）指数**——本实现的关键决策：

1. **溢出对齐**：朴素 `exp(a)` 在 a>88.72 即溢出 fp32（max 3.4e38），但真实 I0 因 `P2/√a≈0.04` 的因子在 a≈91.9 才溢出。`exp(a−3.75)` 在 region2 恒 < I0，绝不早于 I0 溢出，故 [88.7, 91.9] 这段有限值完整覆盖，与 fp64 的 scipy 金标准逐点对齐。
   > 注：`torch.i0(89.0)` (fp32) 因朴素 exp 提前溢出返回 `inf`，而 scipy(fp64→cast)=1.9e37。金标准取 scipy，故 shift-fold 才是正确匹配；朴素写法在此段会判错。
2. **误差只进一次**：`e^3.75` 在编译期折进 P2 全部系数（**零运行时开销**），指数只调用一次，相对误差按 1×δ 进入结果（拆分成 `exp(a/2)` 用两次会翻倍为 2×δ）。较拆分式还少一条指令。

## 实测结果

**精度（950PR 模拟器，CANN 9.1.0-beta.3，对齐 `scipy.special.i0`）** — 功能自验证用例逐点比对，含 ±Inf/NaN/溢出边缘：

| shape | max rel err | 超差 |
|---|---|---|
| 1 | 0.000e+00 | 0/1 |
| 32 | 3.912e-07 | 0/32 |
| 1023 | 5.049e-07 | 0/1023 |
| 2048 | 5.631e-07 | 0/2048 |

双万分之一（1e-4）满足，余量约 **180×**。sim 实测 5.6e-7 与 host 模型预测 5.4e-7（`talk/bessel_i0_check.py`，31001 点函数形状采样，59.9% region2）**几乎一致**。边缘块（0 / ±Inf / NaN / 溢出）全部逐点匹配 → 显式 `±Inf→NaN` Select 生效。

> `Reg::Exp` 在 region2 的 a=|x|≤3.75 lane（被丢弃）上会触发 `vec_err_idata_inf_nan`(RV_VEXP) 标志——良性，Select 从不读取被丢弃 lane（与 ndtri 的 Ln 标志同理）。

### `Reg::Exp` 精度风险 —— 已在 sim 消解

曾担心 region2 相对误差 ≈ 1×`Reg::Exp` 误差，若片上 vexp 仅 ~2⁻¹⁴（6e-5）则临界。host 扰动扫描（`bessel_i0_check.py::exp_sweep`）给出门限：δ_exp ≤ 6e-5 PASS，1e-4 FAIL。**sim 实测 region2 达 5.6e-7，远低于门限**，说明 RV_VEXP 精度充足，无需 Taylor 精化 fallback，「零临时空间」结论成立。

**性能**（aiv_vec 占比，门限 90%）：结构与 ndtri 完全一致（TILE_ELEMS=8192 分块 + `BUFFER_NUM=2` 双缓冲），region2 比 ndtri 多 `Exp`/`Sqrt`/`Div`，向量更重但仍应稳过 90%。**perf 需真机（`./run.sh npu`）**——`msprof op simulator` 不产出 op_summary CSV，sim 无法测 aiv_vec。

## 自验证

```bash
cd tests/functional/bessel_i0
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

1. `include/adv_api/kernel_api.h` → `#include "math/bessel_i0.h"`
2. `include/adv_api/tiling_api.h` → `#include "math/bessel_i0_tiling.h"`
3. `tests/api/adv_api/CMakeLists.txt` → 在 `ASCENDC_TEST_ascend950pr_9599_AIV_CASE_SRC_FILES` 中加入
   `math/bessel_i0/test_operator_bessel_i0.cpp`
4. Tiling UT → `tests/api/adv_api/tiling/test_tiling.cpp` + `ASCENDC_TILING_TEST_SRC_FILES`
