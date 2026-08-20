# Fixpipe(L0C→GM/L0C→L1/L0C→UB)接口边界值测试需求设计文档

## 需求

### 需求概述

对 Fixpipe 三个数据搬运接口在 L0C Buffer 中存在 inf/-inf/nan 等边界值时的输出行为进行端到端实测验证，确认实测结果与 [Fixpipe 接口边界值汇总](../../../docs/zh/api/appendix/fixpipe_interface_boundary_value_summary.md) 中的理论定义一致。

### 被测接口

| 接口 | 搬运路径 | C API | 高阶 API |
| --- | --- | --- | --- |
| asc_copy_l0c2gm | L0C → GM | asc_copy_l0c2gm | Fixpipe |
| asc_copy_l0c2l1 | L0C → L1 | asc_copy_l0c2l1 | — |
| asc_copy_l0c2ub | L0C → UB | asc_copy_l0c2ub | — |

### 适用型号

- Ascend 950PR / Ascend 950DT（dav-3510）

### 测试范围

#### 测试维度一：L0C 源数据类型 × 边界值

| L0C 源数据类型 | 边界值类型 | 边界值 | 理论依据 |
| --- | --- | --- | --- |
| float | inf/nan | +inf, -inf, nan | 文档表1 |
| int32_t | 量化溢出 | 量化后超出目标类型表示范围 | 文档表2 |

#### 测试维度二：输出数据类型 × 量化模式

| 输出数据类型 | 量化模式 | L0C 源类型 | 文档表 |
| --- | --- | --- | --- |
| half | VQF322F16_PRE, F322F16(Cast), DEQF16, VDEQF16 | float / int32_t | 表1 / 表2 |
| bfloat16_t | VQF322BF16_PRE, F322BF16(Cast), VQS322BF16_PRE | float / int32_t | 表1 / 表2 |
| float | VQF322F32_PRE, NoQuant | float | 表1 / 表4 |
| fp8_e4m3fn_t | VQF322FP8_PRE | float | 表1 |
| hifloat8_t | VQF322HIF8_PRE, VQF322HIF8_PRE_HYBRID | float | 表1 |
| int8_t | QF322B8_PRE, VQF322B8_PRE, REQ8, VREQ8 | float / int32_t | 表1 / 表2 |
| int4b_t | VQF322S4_PRE, VREQ4 | float / int32_t | 表1 / 表2 |
| — (NoQuant) | NoQuant | float | 表4 |

#### 测试维度三：饱和模式

| 模式 | CTRL[48] | 行为 |
| --- | --- | --- |
| 饱和模式 | 0 | inf→±MAX, nan→0 |
| 非饱和模式 | 1 | inf/nan 保持原输出 |

> CTRL[48] 仅对 half、bfloat16_t、fp8_e4m3fn_t、hifloat8_t 生效。float 类型 CTRL[48] 不生效，两种模式结果一致。整数类型只有饱和模式。

#### 测试维度四：搬运路径差异

| 路径 | 验证方式 | 特殊限制 |
| --- | --- | --- |
| L0C→GM | 直接搬运到 GM，无中转 | 无限制 |
| L0C→UB | L0C→UB→GM（asc_copy_ub2gm） | 需 AIC/AIV 跨核同步 |
| L0C→L1 | L0C→L1→(第二次Mmad)→L0C→GM | 非饱和模式 inf×0=nan 污染整行 |

#### 测试维度五：NoQuant 输出格式

| 场景 | 输出格式 | 双目标 | 文档表 |
| --- | --- | --- | --- |
| 1 | Nz | 否 | 表4 |
| 2 | ND | 否 | 表4 |
| 3 | ND | 是（按M拆分） | 表4 |
| 4 | ND | 是（按N拆分） | 表4 |

### 预期输出（理论值汇总）

#### float L0C → 各输出类型（文档表1摘要）

| 输出类型 | L0C 值 | 饱和模式 | 非饱和模式 |
| --- | --- | --- | --- |
| half | +inf | 65504.0 (MAX) | +inf |
| half | -inf | -65504.0 (MIN) | -inf |
| half | nan | 0.0 | nan |
| bfloat16_t | +inf | 0x7F7F (MAX) | 0x7F80 (+inf) |
| bfloat16_t | -inf | 0xFF7F (MIN) | 0xFF80 (-inf) |
| bfloat16_t | nan | 0x0000 (0) | 0x7FFF (nan) |
| float | +inf | +inf | +inf |
| float | -inf | -inf | -inf |
| float | nan | nan | nan |
| fp8_e4m3fn_t | +inf | 0x7E (448.0, MAX) | 0x7F (nan) |
| fp8_e4m3fn_t | -inf | 0xFE (-448.0, MIN) | 0x7F (nan) |
| fp8_e4m3fn_t | nan | 0x00 (0), CTRL[50]=0时 | 0x7F (nan) |
| hifloat8_t | +inf | 111 (HiF8+inf) | 111 (HiF8+inf) |
| hifloat8_t | -inf | 239 (HiF8-inf) | 239 (HiF8-inf) |
| hifloat8_t | nan | 0 | 128 (HiF8NAN) |
| int8_t | +inf | 127 (MAX) | 127 (MAX) |
| int8_t | -inf | -128 (MIN) | -128 (MIN) |
| int8_t | nan | 0 | 0 |
| int4b_t | +inf | 7 (MAX) | 7 (MAX) |
| int4b_t | -inf | -8 (MIN) | -8 (MIN) |
| int4b_t | nan | 0 | 0 |

> 完整理论值见文档表1，含 Cast 模式（F322F16/F322BF16）、HYBRID 舍入模式等变体。

#### int32_t L0C → 各输出类型（文档表2摘要）

| 输出类型 | 量化模式 | 溢出方向 | 输出值 |
| --- | --- | --- | --- |
| half | DEQF16 / VDEQF16 | 超出 half 范围 | 65504.0 (MAX) / -65504.0 (MIN) |
| bfloat16_t | VQS322BF16_PRE | 超出 bf16 范围 | 0x7F7F (MAX) / 0xFF7F (MIN) |
| int8_t | REQ8 / VREQ8 | >127 / <-128 | 127 (MAX) / -128 (MIN) |
| int4b_t | VREQ4 | >7 / <-8 | 7 (MAX) / -8 (MIN) |

#### NoQuant 场景（文档表4）

| 场景 | 输出格式 | L0C 值 | 输出值 |
| --- | --- | --- | --- |
| 1-4 | Nz / ND / 双目标 | +inf/-inf/nan | 原样保留 |

### 验收标准

1. 所有已验证场景的 output 与 golden 逐点匹配，通过率 100%。
2. float 输出类型：饱和模式与非饱和模式结果一致（CTRL[48] 不生效）。
3. half/bfloat16_t 输出类型：饱和模式 inf→±MAX、nan→0；非饱和模式 inf/nan 保持。
4. 整数类型：只有饱和模式，inf→±MAX、nan→0。
5. L0C→L1 非饱和模式：golden 已适配第二次 Mmad 的 inf×0=nan 行为。
6. 未验证场景（CANN 限制）明确记录原因，不误报为失败。

## 背景简述

### 环境信息

| 项目 | 值 |
| --- | --- |
| DEVKIT_PATH | asc-devkit 仓库根目录 |
| CANN_PATH | CANN 9.2.0 安装路径 |
| SOC_ARCH | dav-3510（Ascend 950PR/950DT） |
| 运行模式 | sim（仿真）/ npu（实机） |

### 文档依据

| 文档 | 路径 | 内容 |
| --- | --- | --- |
| Fixpipe 接口边界值汇总（总） | `docs/zh/api/appendix/fixpipe_interface_boundary_value_summary.md` | 三接口汇总，表1-表6 |
| Fixpipe(L0C→GM) 边界值汇总 | `docs/zh/api/appendix/fixpipe_l0c2gm_interface_boundary_value_summary.md` | L0C→GM 专用 |
| Fixpipe(L0C→UB) 边界值汇总 | `docs/zh/api/appendix/fixpipe_l0c2ub_interface_boundary_value_summary.md` | L0C→UB 专用，含 NoQuant 表4 |
| Fixpipe(L0C→L1) 边界值汇总 | `docs/zh/api/appendix/fixpipe_l0c2l1_interface_boundary_value_summary.md` | L0C→L1 专用，含已知限制表5/表6 |
| Mmad 接口边界值汇总 | `docs/zh/api/appendix/mmad_interface_boundary_value_summary.md` | L0C 边界值来源（乘法阶段/加法阶段） |

### 现有样例工程

| 样例路径 | 场景数 | 已有脚本 |
| --- | --- | --- |
| `examples/02_simd_c_api/03_c_api/00_data_movement/data_copy_l0c2gm/` | 17 | `gen_data.py`, `verify_result.py`, `test_boundary.sh` |
| `examples/02_simd_c_api/03_c_api/00_data_movement/data_copy_l0c2ub/` | 21 | 同上 |
| `examples/02_simd_c_api/03_c_api/00_data_movement/data_copy_l0c2l1/` | 17 | 同上 |

已有边界测试采用 `gen_data.py --boundary` 模式：A[0:3,0]=+inf/-inf/nan，B[0,:]=1.0，使 L0C = A×B 产生 inf/-inf/nan。该模式覆盖的 A×B 组合有限。

### 已有测试结果

| 接口 | 测试项 | 通过 | 跳过 | 未验证 | 通过率 |
| --- | --- | --- | --- | --- | --- |
| L0C→GM | 34 | 34 | 0 | 0 | 100% |
| L0C→UB | 38 | 38 | 0 | 0 | 100% |
| L0C→L1 | 34 | 20 | 2 | 12 | 58.8%（已验证部分 100%） |

## 涉及领域

| 领域 | 说明 |
| --- | --- |
| 内存搬运 (Fixpipe) | asc_copy_l0c2gm / l0c2l1 / l0c2ub，随路量化，CTRL[48] 饱和控制 |
| 矩阵乘 (Mmad) | half×half→float、int8×int8→int32，用于向 L0C 注入边界值 |
| 数据布局 | Nz (node-zag) 格式、K_ROUND 转置、NZ2ND 输出转换 |
| 量化模式 | NoQuant、QF322F16_PRE、VQF322F32_PRE、VQF322F16_PRE、DEQF16、REQ8 等 17 种 |
| 饱和控制 | CTRL[48]（饱和/非饱和）、CTRL[50]（fp8 nan 控制） |
| 编译工程 | CMake + ASC 编译器，CMAKE_ASC_RUN_MODE，CMAKE_ASC_ARCHITECTURES |
| 跨核同步 | L0C→UB 需 AIC/AIV 协同（asc_copy_l0c2ub + asc_copy_ub2gm） |

## 方案详述

### 1. 边界值注入策略

L0C Buffer 仅可由 Cube（Mmad）流水线写入，无法从 GM 直接加载任意值。因此通过 Mmad 乘法向 L0C 注入边界值。

#### 策略一：单侧注入（已有测试）

```
A[0, 0] = +inf,  A[1, 0] = -inf,  A[2, 0] = nan
B[0, :] = 1.0
→ L0C[i, j] = A[i, 0] × 1.0 + Σ(随机×随机, k>0)
```

- 优点：非边界位置有随机数据，验证更接近真实场景
- 缺点：只覆盖 A 侧边界值，B 侧始终为 1.0，无法验证 +inf×-inf、norm×inf 等组合

#### 策略二：全组合注入（本测试新增）

```
A[0:6, 0] = [+inf, -inf, nan, norm(>0)=2.0, norm(<0)=-2.0, 0.0]
B[0, 0:6] = [+inf, -inf, nan, norm(>0)=3.0, norm(<0)=-3.0, 0.0]
所有其他位置 = 0
→ L0C[i, j] = A[i, 0] × B[0, j]（无累加噪声，精确可验证）
```

- 优点：6×6 = 36 种 A×B 边界组合，覆盖 Mmad 乘法阶段全部边界行为
- 缺点：非边界位置为 0，与真实场景有差异

#### 策略三：int32 溢出注入（已有测试）

```
int8 输入：A[0, :] = -128, 其余 = 127
→ L0C = A × B 中产生极端 int32 值，量化后溢出
```

### 2. 两种测试的互补关系

| 维度 | 已有测试（策略一+三） | 本测试（策略二） |
| --- | --- | --- |
| 边界值覆盖 | A 侧 3 值（+inf/-inf/nan）× B=1.0 | A×B 全 6×6 组合 |
| 场景广度 | l0c2gm 17场景, l0c2ub 21场景, l0c2l1 17场景 | 每通路 1 场景 |
| 输出类型广度 | 全类型（float/half/bf16/fp8/hif8/int8/int4b） | float + half |
| 报告格式 | 逐场景 PASS/FAIL 汇总 | 13 行规格表（A×B × 双模式） |
| 定位 | 广度覆盖 | 深度覆盖 |

### 3. 数据布局处理

样例 `gen_data.py` 已正确处理 Nz 布局、K_ROUND 转置、quant_pre.bin、D 矩阵等。本测试的 `gen_full_boundary.py` 复用相同布局约定：

| 数据 | 布局 | 说明 |
| --- | --- | --- |
| A (x1_gm.bin) | [K_ROUND=2, M, K//K_ROUND=64] | `reshape(M, 2, 64).transpose(1, 0, 2)` |
| B (x2_gm.bin) | [K=128, N] | 不转置 |
| golden (l0c2ub/l0c2gm) | Nz: [N//16, M//16, 16, 16] | `reshape(M//16, 16, N//16, 16).transpose(2,0,1,3)` |
| golden (l0c2l1) | flat [M, N] | kernel 用 NZ2ND 输出 |
| D (x3_gm.bin, 仅 l0c2l1) | [N, N] half | 单位矩阵 |
| quant_pre.bin | [N] uint64 | alpha=2.0, sign=1 |

Nz 反变换（读取时）：`reshape(N//16, M//16, 16, 16).transpose(1, 2, 0, 3).reshape(M, N)`

### 4. 三条通路测试设计

#### 4.1 L0C→GM (asc_copy_l0c2gm)

**数据流**：GM(A,B) → Mmad → L0C → asc_copy_l0c2gm → GM(output)

**场景覆盖**（17 种量化 × 2 模式 = 34 项）：

| 场景 | 输出类型 | 量化模式 | 输入类型 | 文档表 |
| --- | --- | --- | --- | --- |
| 1-2 | half | DEQF16 / VDEQF16 | int8 | 表2 |
| 3-4 | int8 | QF322B8_PRE / VQF322B8_PRE | half | 表1 |
| 5-6 | int8 | REQ8 / VREQ8 | int8 | 表2 |
| 7 | int4b | VREQ4 | int8 | 表2 |
| 8 | bf16 | VQS322BF16_PRE | int8 | 表2 |
| 9 | half | VQF322F16_PRE | half | 表1 |
| 10 | bf16 | VQF322BF16_PRE | half | 表1 |
| 11 | int4b | VQF322S4_PRE | half | 表1 |
| 12 | fp8 | VQF322FP8_PRE | half | 表1 |
| 13-14 | hif8 | VQF322HIF8_PRE / HYBRID | half | 表1 |
| 15 | float | VQF322F32_PRE | half | 表1 |
| 16-17 | half/bf16 | F322F16 / F322BF16 (Cast) | half | 表1 |

**特点**：直接搬运到 GM，无中转路径，非饱和模式 inf/nan 完整传递。

#### 4.2 L0C→UB (asc_copy_l0c2ub)

**数据流**：GM(A,B) → Mmad → L0C → asc_copy_l0c2ub → UB → asc_copy_ub2gm → GM(output)

**场景覆盖**（21 种 = 4 NoQuant + 17 量化 × 2 模式 = 38 项）：

| 场景 | 类型 | 说明 |
| --- | --- | --- |
| 1-4 | NoQuant | Nz / ND / 双目标M拆 / 双目标N拆，仅测1次（CTRL[48]不生效） |
| 5-21 | 量化 | 同 L0C→GM 场景 1-17，每种 × 2 模式 |

**特点**：需 AIC/AIV 跨核同步；NoQuant 场景验证 inf/-inf/nan 在 Nz/ND/双目标格式下正确传递。

#### 4.3 L0C→L1 (asc_copy_l0c2l1)

**数据流**：GM(A,B) → Mmad → L0C → asc_copy_l0c2l1 → L1 → (第二次Mmad: L1×D→L0C→GM, NZ2ND) → GM(output)

**场景覆盖**（17 种 × 2 模式 = 34 项，已验证 20 项，跳过 2 项，未验证 12 项）：

| 场景 | 输出类型 | 量化模式 | 状态 | 原因 |
| --- | --- | --- | --- | --- |
| 1-10 | half/int8/bf16 | 多种 | ✅ 已验证 | — |
| 11 | int4b | VREQ4 | ❌ 未验证 | asc_mmad_s4 未实现 |
| 12 | bf16 | VQS322BF16_PRE | ❌ 未验证 | bf16/int32 重载未提供 |
| 13 | int4b | VQF322S4_PRE | ❌ 未验证 | asc_mmad_s4 未实现 |
| 14 | fp8 | VQF322FP8_PRE | ❌ 未验证 | fp8/float 重载未提供 |
| 15-16 | hif8 | VQF322HIF8_PRE / HYBRID | ❌ 未验证 | hif8/float 重载未提供 |
| 17 | float | VQF322F32_PRE | ⏭ 跳过 | float×float Mmad 含 inf 时硬件错误(ret=507015) |

**验证路径限制**：非饱和模式下，L1 中含 inf 时，第二次 Mmad 中 inf×0=nan 污染整行。golden 已适配此行为。文档明确指出："L1 Buffer 上的量化结果实际保留了 inf（符合表 1 预期），仅经第二次 Mmad 后被破坏为 nan。"

### 5. Golden 计算逻辑

#### float L0C 边界值（策略二，6×6 组合）

L0C[i,j] = mmad_mul(A[i,0], B[0,j])，遵循 IEEE 754：
- inf × inf = ±inf（同号正、异号负）
- inf × 0 = nan
- nan × 任意 = nan
- 正常值 × 正常值 = 正常乘积

#### Fixpipe 输出（按通路和模式）

| 通路 | 饱和模式 golden | 非饱和模式 golden |
| --- | --- | --- |
| L0C→GM (float输出) | = L0C（CTRL[48]不生效） | = L0C（同左） |
| L0C→GM (half输出) | inf→65504, nan→0 | inf→+inf(half), nan→nan(half) |
| L0C→UB (NoQuant) | = L0C（原样保留） | = L0C（同左） |
| L0C→L1 (half输出) | fixpipe_sat: inf→65504, nan→0 | fixpipe_nonsat: inf→±inf(half), nan→nan |
| L0C→L1 第二次Mmad | 干净直通 = L1[i,j] | 整行 nan（inf×0=nan污染） |

#### int32_t L0C 溢出（策略三）

- A 某行 = -128，其余 = 127
- L0C 产生极端 int32 值
- 量化后超出目标类型范围 → 饱和为 ±MAX

### 6. 验证方法

| 步骤 | 说明 |
| --- | --- |
| 读取 output | 按通路选择 Nz 反变换或直接 reshape 为 [M, N] |
| 读取 golden | 同上 |
| 逐点比对 | 在 6×6 边界位置（行 0-5, 列 0-5）逐点比对 |
| 匹配规则 | nan==nan → PASS；±inf 符号一致 → PASS；有限值误差 < tol → PASS |
| 容差 | float/half: rtol=1e-3, atol=1e-3；int8/int4b: atol=1 |

### 7. 报告格式

报告采用 13 行规格表格式，表头：

| C矩阵的数据类型 | L0C数据来源 | A矩阵的元素取值 | B矩阵的元素取值 | dst的元素取值<br>（INF/NAN模式） | dst的元素取值<br>（饱和模式） | 结果 |
| --- | --- | --- | --- | --- | --- | --- |

- `dst` 列显示 Fixpipe 实际输出值
- `L0C数据来源` 列值为"乘法阶段"，表示边界值由 Mmad 乘法产生
- 13 行覆盖文档表 1 的全部 inf/-inf/nan 组合
- 每通路 26 项检查（13 行 × 2 模式），另含第 13 行溢出标记 N/A

### 8. 运行方式

```bash
cd examples/02_simd_c_api/03_c_api/00_data_movement

# 已有测试（广度覆盖，全场景全类型）
bash data_copy_l0c2gm/scripts/test_boundary.sh
bash data_copy_l0c2ub/scripts/test_boundary.sh
bash data_copy_l0c2l1/scripts/test_boundary.sh

# 本测试（深度覆盖，全 A×B 边界组合）
bash run_mmad_spec_report.sh all sim
```

### 9. 已知限制与未验证场景

#### 硬件限制

| 限制 | 影响接口 | 说明 |
| --- | --- | --- |
| float×float Mmad 含 inf 时硬件错误 (ret=507015) | L0C→L1 场景17 | CANN 版本限制，非 Fixpipe 本身问题 |

#### CANN 版本限制（L0C→L1 场景11-16）

| 场景 | 未验证原因 |
| --- | --- |
| 11, 13 | asc_mmad_s4 (int4b×int4b→int32) 未实现 |
| 12 | asc_copy_l0c2l1 的 bf16/int32 重载未提供 |
| 14 | asc_copy_l0c2l1 的 fp8/float 重载未提供 |
| 15, 16 | asc_copy_l0c2l1 的 hif8/float 重载未提供 |

#### 验证路径限制

| 限制 | 影响接口 | 说明 |
| --- | --- | --- |
| 第二次 Mmad inf×0=nan 污染 | L0C→L1 非饱和模式 | golden 已适配，不影响 Fixpipe 本身行为判定 |
| half 输入无法触发乘法溢出 | 全部通路第13行 | 65504² ≈ 4.29×10⁹ < float32 MAX 3.40×10³⁸ |

### 10. 交付物清单

| 交付物 | 路径 | 说明 |
| --- | --- | --- |
| 测试数据生成脚本 | `gen_full_boundary.py` | 6×6 边界值组合，正确处理 Nz/K_ROUND/D矩阵/quant_pre |
| 结果验证脚本 | `verify_full_boundary.py` | 逐位置比对 output 与 golden |
| 报告生成脚本 | `gen_mmad_spec_report.py` | 13 行规格表格式 Markdown 报告 |
| 编排脚本 | `run_mmad_spec_report.sh` | 编译→运行→验证→报告 |
| 单通路报告 | `mmad_spec_results/report_l0c2{ub,gm,l1}.md` | 每通路 13 行规格表 |
| 汇总报告 | `mmad_spec_results/mmad_spec_report_all.md` | 三通路汇总 |
| 需求设计文档 | `fixpipe_boundary_test_requirement.md` | 本文档 |
