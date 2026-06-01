# Ternary Scalar 操作示例

## 概述

本示例演示使用 Ascend C 基础 API 的 `Adds` 实现向量加标量操作。

数学公式：
```
out = src + scalar
```

**注意**：tensor_api 的 `Transform<Inst::AddScalar>` 是 asc-devkit 仓库中的新功能，尚未发布到 CANN 包中。本示例使用已发布的基础 API (`Adds`) 来演示相同的概念。当 tensor_api 发布到 CANN 包后，可以使用 `Transform<Inst::AddScalar>` 替代 `Adds`。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 目录结构介绍
```
├── tensor_api/
│   ├── scripts/
│   │   ├── gen_data.py              // 输入数据和真值数据生成脚本
│   │   └── verify_result.py         // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── data_utils.h                 // 数据读入写出函数
│   ├── run.sh                       // 一键运行脚本
│   └── tensor_api_ternary.asc       // Ascend C算子实现 & 调用样例
```

## 算子描述
- 算子功能：
  使用 Ascend C 的 `Adds` API 对源操作数 src 中每个元素加上标量 scalar，
  将结果写入目的操作数 dst。

  对应的数学表达式为：
  ```
  out = src + scalar
  ```

- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="5" align="center"> Adds </td></tr>
  <tr><td rowspan="2" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td><td align="center">default</td></tr>
  <tr><td align="center">x</td><td align="center">512</td><td align="center">float16</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td rowspan="1" align="center">算子输出</td><td align="center">out</td><td align="center">512</td><td align="center">float16</td><td align="center">ND</td><td align="center">\</td></tr>
  <tr><td rowspan="1" align="center">标量</td><td align="center">scalar</td><td align="center">-</td><td align="center">float16</td><td align="center">-</td><td align="center">2.0</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="5" align="center">kernel_add_scalar</td></tr>
  </table>

- 算子实现：
  本样例中实现的是固定 shape 为 512 的 Adds 算子。
  - Kernel 实现
    计算逻辑是：使用 Ascend C 的 `Adds` 接口完成 src 加上标量 scalar 的计算。
    实现流程分为 3 个基本任务：CopyIn，Compute，CopyOut。
    CopyIn 任务负责将 Global Memory 上的输入 Tensor 搬运至 Local Memory，
    Compute 任务负责调用 `Adds` 执行计算，
    CopyOut 任务负责将输出数据从 Local Memory 搬运至 Global Memory。

  - 调用实现
    使用内核调用符 `<<<>>>` 调用核函数。

## Tensor API 与基础 API 的对应关系

tensor_api 提供了更高层次的抽象，以下是 tensor_api 与基础 API 的对应关系：

| 基础 API | Tensor API | 说明 |
|---------|-----------|------|
| `AscendC::Adds(dst, src, scalar, count)` | `Transform<Inst::AddScalar>(dst, src, scalar)` | 向量加标量 |
| `AscendC::Muls(dst, src, scalar, count)` | `Transform<Inst::MulScalar>(dst, src, scalar)` | 向量乘标量 |
| `AscendC::Axpy(dst, src, scalar, count)` | `Transform<Inst::Axpy>(dst, src, scalar)` | dst = src * scalar + dst |

tensor_api 的优势：
- 编译期类型检查和分发
- 自动处理向量化循环和 mask
- 统一的 Transform 接口支持多种操作

## 编译运行

本示例支持三种运行模式：NPU 模式、CPU Debug 模式和仿真模式。

### NPU 模式（默认）

NPU 模式需要 Ascend NPU 硬件，可获得真实硬件性能。

```bash
# 配置环境变量
source /usr/local/Ascend/cann/set_env.sh  # 或其他安装路径

# 编译
mkdir -p build && cd build
cmake ..
make -j

# 运行
./demo
```

### CPU Debug 模式

CPU Debug 模式可以在没有 NPU 硬件的环境中编译运行，用于代码调试和逻辑验证。

```bash
# 配置环境变量
source /usr/local/Ascend/cann/set_env.sh

# 编译（CPU 模式）
mkdir -p build && cd build
cmake -DRUN_MODE=cpu ..
make -j

# 运行
./demo
```

#### GDB 调试

CPU Debug 模式支持完整的 GDB 调试功能：

```bash
# 编译
mkdir -p build && cd build
cmake -DRUN_MODE=cpu ..
make -j

# 启动 GDB 调试
gdb --args ./demo

# 在 GDB 中：
(gdb) break Compute          # 在 Compute 函数设置断点
(gdb) run                    # 运行程序
(gdb) print srcLocal         # 查看变量
(gdb) next                   # 单步执行
(gdb) bt                     # 查看调用栈
```

### 仿真模式

仿真模式可在无 NPU 硬件环境下模拟执行，用于性能分析。

```bash
# 配置环境变量
source /usr/local/Ascend/cann/set_env.sh

# 编译（仿真模式，需指定 SoC 型号）
mkdir -p build && cd build
cmake -DRUN_MODE=sim -DSOC_VERSION=Ascend910B1 ..
make -j

# 设置仿真库路径（替换 ${SOC_VERSION} 为实际型号）
export LD_LIBRARY_PATH=${ASCEND_HOME_PATH}/tools/simulator/${SOC_VERSION}/lib/:$LD_LIBRARY_PATH

# 运行
./demo
```

### 模式对比

| 特性 | NPU 模式 | CPU Debug 模式 | 仿真模式 |
|------|---------|---------------|---------|
| 硬件要求 | 需要 Ascend NPU | 无 | 无 |
| 编译器 | ASC (bisheng) | GCC | ASC (bisheng) |
| 内核启动 | `<<<>>>` | `ICPU_RUN_KF` | `<<<>>>` |
| 调试支持 | 有限支持 | GDB 完整支持 | 有限支持 |
| 性能 | 真实硬件性能 | 模拟执行，较慢 | 模拟执行，较慢 |
| 适用场景 | 生产环境 | 开发调试 | 性能分析 |

### 完整运行流程

```bash
# 1. 生成测试数据
python3 scripts/gen_data.py

# 2. 编译（选择运行模式）
mkdir -p build && cd build
cmake -DRUN_MODE=cpu ..  # 或 cmake .. 用于 NPU 模式
make -j

# 3. 运行
./demo

# 4. 验证结果
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

预期输出：
```
test pass!
```

  - 默认路径，非 root 用户安装 CANN 软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径 install_path，安装 CANN 软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行
  
  **方式一：一键运行（推荐）**
  ```bash
  ./run.sh
  ```
  
  **方式二：分步执行**
  ```bash
  mkdir -p build && cd build;   # 创建并进入build目录
  cmake ..;make -j;             # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                        # 执行编译生成的可执行程序
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证结果
  ```
  
  **注意**：运行示例需要实际的 Ascend NPU 硬件环境。在没有 NPU 硬件的开发环境中，程序可能会挂起或报错。
  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
