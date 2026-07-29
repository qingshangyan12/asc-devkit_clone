## 使用C_API验证Saturation Flag功能效果样例

## 概述

本样例验证 `asc_set_saturation_flag` 接口设置不同饱和模式对实际计算结果的影响。通过同一组溢出输入，分别在饱和 ON 和 OFF 两种模式下执行计算，比对输出结果验证接口功能。

与 [00_saturation_flag](../00_saturation_flag) 的区别：
- `00_saturation_flag`：验证 set→get 寄存器回环一致性（接口契约正确性）
- `02_saturation_flag_perf`：验证饱和/截断模式对实际计算结果的影响（功能效果验证）

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 02_saturation_flag_perf
│   ├── scripts
│   │   └── gen_data.py                  // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                   // 编译工程文件
│   ├── saturation_flag_perf.asc         // 样例入口（kernel 调用 + main 函数）
│   ├── saturation_flag_perf.h           // Kernel 实现（3 个 Case）
│   ├── data_utils.h                     // 数据读入写出函数
│   ├── README.md                        // 样例说明文档
│   └── README_en.md                     // 样例说明文档（英文）
```

## 接口配合关系

`asc_set_saturation_flag` 只写 CTRL 寄存器，不产生计算。需配合**会溢出的计算 API**验证功能效果：

| 模式 | 配合计算 API | API 层级 | CTRL bit | 极性 | 前置 strategy |
|------|------------|---------|----------|------|-------------|
| FLOAT | `asc_add`（half，寄存器级） | `__simd_vf__` | [48] | 常规(0=饱和) | `USE_API`（默认） |
| INT | `asc_add`（int16，寄存器级） | `__simd_vf__` | [53] | 反转(1=饱和) | `USE_API`（默认） |
| CAST | `asc_int322int16`（寄存器级） | `__simd_vf__` | [59] | 常规(0=饱和) | **`USE_GLOBAL`**（必须） |

> **注意**：CAST 模式（CTRL[59]）仅在 CTRL[60]=1（`USE_GLOBAL`）时生效（仅 `asc_set_ctrl.md` 提到此约束）。
>
> UB 级转换 API（如 `asc_float2int16_rn_sync`）在 dav-3510 上不可用，故使用寄存器级 API + `__simd_vf__` 辅助函数。非 `_sat` 变体（如 `asc_int322int16`）受 CTRL[59] 控制，`_sat` 变体（如 `asc_int322int16_sat`）始终饱和（绕过 CTRL），故使用非 `_sat` 变体验证。

## 样例描述

### Case 1: FLOAT — half 加法溢出

- **输入**：x = 60000.0(half), y = 10000.0(half)
- **计算**：60000 + 10000 = 70000 > 65504(half MAX) → 溢出为 +INF
- **饱和 ON**（CTRL[48]=0）：INF → 65504.0（half MAX）
- **饱和 OFF**（CTRL[48]=1）：+INF 保持原样

### Case 2: INT — int16 加法溢出

- **输入**：x = 30000(int16), y = 30000(int16)
- **计算**：30000 + 30000 = 60000 > 32767(int16 MAX) → 溢出
- **饱和 ON**（CTRL[53]=1）：32767（int16 MAX）
- **饱和 OFF**（CTRL[53]=0）：60000 截断为 int16 = -5536

### Case 3: CAST — int32→int16 转换溢出

- **输入**：x = 50000(int32)
- **转换**：50000 > 32767(int16 MAX) → 溢出
- **饱和 ON**（CTRL[59]=0）：32767（int16 MAX）
- **饱和 OFF**（CTRL[59]=1）：50000 截断为 int16 = -15536

> **输出布局**：int32→int16 转换使用 PART_EVEN 模式，有效值存储在偶数索引，奇数索引为零。

## 验证判定

| Case | 饱和 ON 结果 | 饱和 OFF 结果 | 判定 |
|------|------------|-------------|------|
| 1 (FLOAT) | 65504.0 | +INF | 两者不同且匹配 golden → PASS |
| 2 (INT) | 32767 | -5536 | 两者不同且匹配 golden → PASS |
| 3 (CAST) | 32767 | -15536 | 两者不同且匹配 golden → PASS |

**核心验证点**：饱和 ON 和 OFF 的结果**必须不同**，证明 `asc_set_saturation_flag` 确实影响了计算行为。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行（以 Case 1 为例）

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM
  ./saturation_flag_perf
  ```

  执行结果如下，说明验证通过。
  ```bash
  [PASS] FLOAT sat-ON  (expect 65504)
  [PASS] FLOAT sat-OFF (expect +INF)
  [Success] Saturation flag FLOAT verification passed.
  ```

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | `SCENARIO_NUM` | 场景编号 | 1-3 | 1 |
  | `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-3510`（仅该架构支持） | `dav-3510` |
