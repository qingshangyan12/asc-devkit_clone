## 使用C_API验证Saturation Strategy功能效果样例

## 概述

本样例验证 `asc_set_saturation_strategy` 接口设置不同覆盖策略（USE_API / USE_GLOBAL）对饱和控制来源的切换效果。通过 4 个交叉组合用例，证明策略切换确实改变了饱和控制的生效来源。

与 [01_saturation_strategy](../01_saturation_strategy) 的区别：
- `01_saturation_strategy`：验证 set→get 寄存器回环一致性（接口契约正确性）
- `03_saturation_strategy_perf`：验证策略切换对实际计算结果的影响（功能效果验证）

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```
├── 03_saturation_strategy_perf
│   ├── scripts
│   │   └── gen_data.py                      // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                       // 编译工程文件
│   ├── saturation_strategy_perf.asc         // 样例入口（kernel 调用 + main 函数）
│   ├── saturation_strategy_perf.h           // Kernel 实现（4 个 Case）
│   ├── data_utils.h                         // 数据读入写出函数
│   ├── README.md                            // 样例说明文档
│   └── README_en.md                         // 样例说明文档（英文）
```

## CTRL[60] 的作用机制

参考 [SetCtrlSpr(ISASI).md](../../../../../../../docs/zh/api/SIMD-API/基础API/特殊寄存器访问/SetCtrlSpr(ISASI).md) 中的饱和模式全局或单指令生效配置表：

| CTRL[60] | 控制位 | 功能描述 |
|----------|--------|---------|
| 0 (USE_API) | API 的 `_sat`/非`_sat` 后缀 | **单指令级**饱和控制：`_sat`→饱和，非`_sat`→截断 |
| 1 (USE_GLOBAL) | CTRL[48]/CTRL[59] | **全局级**饱和控制：`asc_set_saturation_flag` 生效，API 后缀被忽略 |

**关键点**：CTRL[48] 和 CTRL[59] 都仅在 CTRL[60]=1（USE_GLOBAL）时生效。USE_API 模式下，`asc_set_saturation_flag` 设置的 CTRL[48]/[59] **不生效**，饱和行为完全由 API 的 `_sat`/非`_sat` 后缀决定。

## 接口配合关系

| 接口 | 角色 | 说明 |
|------|------|------|
| `asc_set_saturation_strategy` | **被测接口** | 设置 CTRL[60]，切换 USE_API/USE_GLOBAL |
| `asc_set_saturation_flag` | **配合接口** | 设置 CTRL[59]（CAST 模式），仅在 USE_GLOBAL 时生效 |
| `asc_int322int16`（非`_sat`） | **配合接口** | 寄存器级 int32→int16 转换，`_sat` 后缀仅在 USE_API 时生效 |
| `asc_int322int16_sat`（`_sat`） | **配合接口** | 寄存器级 int32→int16 饱和转换，`_sat` 后缀仅在 USE_API 时生效 |

## 样例描述

使用 int32→int16 转换（CAST 模式, CTRL[59]）作为测试载体。输入 50000（int32）超出 int16 范围（32767），溢出行为清晰可辨。

4 个 Case 构成策略×API变体的交叉组合：

| Case | Strategy | Flag 设置 | API 变体 | 预期行为 | 预期结果 |
|------|----------|----------|---------|---------|---------|
| 1 | USE_API | CAST=true（被忽略） | `asc_int322int16`（非`_sat`） | 截断 | -15536 |
| 2 | USE_API | CAST=false（被忽略） | `asc_int322int16_sat`（`_sat`） | 饱和 | 32767 |
| 3 | USE_GLOBAL | CAST=true | `asc_int322int16`（非`_sat`） | 饱和 | 32767 |
| 4 | USE_GLOBAL | CAST=false | `asc_int322int16_sat`（`_sat`） | 截断 | -15536 |

### 核心验证逻辑

- **Case 1 vs 3**：同一 API（非`_sat`），不同策略 → 不同结果（截断 vs 饱和），证明策略切换使 flag 生效
- **Case 2 vs 4**：同一 API（`_sat`），不同策略 → 不同结果（饱和 vs 截断），证明策略切换使 flag 覆盖 API 后缀
- **Case 1 vs 2**：同一策略（USE_API），不同 API 后缀 → 不同结果，证明 USE_API 下 API 后缀生效
- **Case 3 vs 4**：同一策略（USE_GLOBAL），不同 flag → 不同结果，证明 USE_GLOBAL 下 flag 生效

> **输出布局**：int32→int16 转换使用 PART_EVEN + MODE_ZEROING，有效值在偶数索引，奇数索引为零。每个 Case 输出 128 个 int16（64 有效 + 64 零），总输出 512 个 int16。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行算子。

- 配置环境变量

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- 样例执行

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py
  ./saturation_strategy_perf
  ```

  执行结果如下，说明验证通过。
  ```bash
  [PASS] Case 1: USE_API + non-_sat API (expect truncation -15536)
  [PASS] Case 2: USE_API + _sat API (expect saturation 32767)
  [PASS] Case 3: USE_GLOBAL + flag=true (expect saturation 32767)
  [PASS] Case 4: USE_GLOBAL + flag=false (expect truncation -15536)
  [Success] Saturation strategy verification passed.
  ```
