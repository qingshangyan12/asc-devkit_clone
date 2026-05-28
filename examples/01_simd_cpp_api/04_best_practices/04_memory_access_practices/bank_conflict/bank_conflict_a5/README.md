# A5 Bank 冲突算子直调样例

## 概述

本样例基于 Mul 算子，演示 Atlas A5 架构（`dav-3510`）上 UB Bank 冲突的不同场景及其性能影响。通过对比不同 buffer 地址配置下 Mul 指令的执行周期数，验证 A5 架构中 bank 冲突的触发条件。

## A5 UB 关键特性

- **256KB** UB，8 个 bank group，每组 **2 个 bank**（共 16 bank）
- 每 bank 512 行 × 32B = 16KB，每组 32KB
- 每组 **2 个读口 + 1 个写口**（vs A2/A3 的 1 读口）
- **低位地址交织**：`ADDR = {DEPTH[8:0], BANK[0], BG[2:0], WIDTH[4:0]}`
- 每 bank 含 4 个 **subbank**（仅影响 gather/scatter 离散访存）

## 与 A2/A3 的主要差异

| 特性 | A2/A3 | A5 |
|---|---|---|
| UB 总容量 | 192KB | 256KB |
| Bank Group 数 | 16 | 8 |
| 每 BG 的 Bank 数 | 3 | 2 |
| 每 BG 读口数 | 1 | **2** |
| 地址交织 | 高位 | 低位 |

## 支持的产品

- Atlas A5 训练系列产品 / Atlas A5 推理系列产品（`dav-3510` / `__NPU_ARCH__=3510`）

## 目录结构

```
├── bank_conflict_a5
│   ├── scripts
│   │   ├── gen_data.py                    // 输入数据和真值数据生成脚本
│   │   └── verify_result.py               // 输出精度验证脚本
│   ├── CMakeLists.txt                     // 编译工程文件 (arch: dav-3510)
│   ├── data_utils.h                       // 数据读入写出工具函数
│   ├── bank_conflict_a5.asc               // Ascend C 算子实现 & 调用样例
│   ├── analysis.md                        // 详细地址解码与冲突分析
│   └── README.md
```

## 算子描述

- **算子功能**：固定 shape 为 [1, 4096] 的 Mul 算子（`z = x * y`）
- **数据类型**：float32
- **指令**：`Mul(dst, src0, src1, mask, repeatTimes, {1,1,1,8,8,8})`
- **演示场景**：通过 `CASE_NUM` 宏切换 4 种 buffer 地址配置，验证不同 bank 冲突场景下的性能差异

### Case 概览

| CASE_NUM | 场景 | 预期性能 |
|---|---|---|
| 1 | dst 覆盖 src1（硬件优化，无冲突） | 基线 |
| 2 | dst 偏移 src1（读写冲突） | ~1.5x |
| 3 | 同 bank 双读 + dst 异 bank（A5 2口优势） | ~1.0x |
| 4 | 同 bank 双读 + dst 同 bank（读写冲突） | ~1.5x |

详细地址解码与分析见 `analysis.md`。

## 运行方式

### 仿真环境

在芯片仿真环境下运行本样例，需要先配置仿真环境变量并启动仿真器，然后按以下步骤编译和运行：

```bash
# 1. 配置 CANN 环境变量
source /path/to/cann/set_env.sh

# 2. 编译（注意：仿真环境需要用仿真模式编译）
mkdir -p build && cd build
cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510
make -j

# 3. 生成测试数据
python3 ../scripts/gen_data.py

# 4. 运行仿真
# 在仿真环境下，demo 可执行文件通过仿真器加载运行。
# 具体仿真器启动命令取决于当前仿真环境配置。
# 常见方式（以 ACPU 仿真器为例）：
run_simulator ./demo

# 5. 验证结果
python3 ../scripts/verify_result.py output/output_z.bin output/golden.bin
```

### 切换 Case

修改 `bank_conflict_a5.asc` 文件开头的 `CASE_NUM` 常量（1~4），重新编译运行即可测试不同场景：

```cpp
constexpr uint32_t CASE_NUM = 1;  // 改为 2, 3, 或 4
```

### 仿真调试建议

1. **获取物理地址**：代码中已通过 `GetPhyAddr()` 打印各 buffer 的物理地址，可对照 `analysis.md` 中的地址解码公式验证 buffer 是否放置在预期的 bank/bg 位置。
2. **性能打点**：代码通过 `GetSystemCycle()` 记录 Mul 指令循环前后的 cycle 数，差值即为 1000 次 Mul 的总执行周期。注意仿真环境的 cycle 可能包含仿真器开销，建议关注各 case 间的**相对比值**而非绝对数值。
3. **波形查看**：如需深入分析每个 cycle 的 bank 访问冲突情况，可在仿真器中导出 UB 访问波形，观察 bank group 的读口/写口占用时序。

### 预期结果

编译执行成功且精度对比通过后，输出如下：

```
[A5] GetPhyAddr(xLocal) = 0x...
[A5] GetPhyAddr(yLocal) = 0x...
[A5] GetPhyAddr(zLocal) = 0x...
[A5] Mul x1000 total cycles: ...
[A5] CASE_NUM = 1

test pass!
```
