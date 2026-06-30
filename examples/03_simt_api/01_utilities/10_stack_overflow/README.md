# SIMT栈溢出场景及优化样例

## 概述

本样例以kernel内定义大局部数组导致栈溢出为例，介绍Ascend C SIMT编程方式下的栈溢出问题及优化思路。样例包含1个基线版本以及1个优化版本，两个版本使用相同的kernel代码，区别在于初始化配置不同：基线版本使用`aclInit(nullptr)`采用默认栈大小，kernel内大局部数组导致栈溢出；优化版本通过`aclInit("acl.json")`加载配置文件，配置更大的SIMT栈大小，避免栈溢出。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构介绍

```text
├── 10_stack_overflow
│   ├── CMakeLists.txt          // cmake编译文件
│   ├── stack_overflow.asc      // Ascend C算子栈溢出及优化样例
│   ├── acl.json                // aclInit配置文件，配置SIMT栈大小
│   └── README.md
```

## 样例描述

- 样例功能

  在kernel内定义大局部数组进行数据填充与求和计算，对比默认栈大小与配置栈大小下的执行结果。

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SimtStackOverflow</td></tr>
  <tr><td rowspan="1" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[128]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[128]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">simt_stack_overflow</td></tr>
  </table>

## 样例实现

### 栈溢出问题说明

在SIMT kernel中，每个线程拥有独立的栈空间，但栈空间大小有限。当在kernel内部定义较大的局部数组时，数组会被分配在栈上。若线程数较多且每个线程的栈使用量大，总栈需求将远超栈空间限制，导致**栈溢出**。

本样例通过`SCENARIO_NUM`编译宏选择不同的初始化配置，对比栈溢出问题与优化方案。

| Case   | 初始化方式                | 栈大小       | 实现特点                                      | 优化特性                |
|--------|-----------------------|-----------|---------------------------------------------|---------------------|
| Case 1 | `aclInit(nullptr)`    | 默认值（较小）  | kernel内定义大局部数组（float local_buffer[320]），默认栈大小不足导致栈溢出    | 基线版本，存在栈溢出问题        |
| Case 2 | `aclInit("acl.json")` | 配置值（1MB）  | 同一kernel，通过acl.json配置更大的simt_stack_size，避免栈溢出              | 栈溢出优化，通过配置文件扩大栈空间    |

#### 1. 栈空间分析

| 配置项          | Case 1（基线版本）      | Case 2（优化版本）       |
|--------------|--------------------|--------------------|
| 每线程栈开销       | `320 * 4B = 1280B`  | `320 * 4B = 1280B`  |
| 线程总数         | `2 * 32 = 64`      | `2 * 32 = 64`      |
| 总栈需求         | `1280B * 64 = 80KB` | `1280B * 64 = 80KB` |
| 栈大小配置        | 默认值（较小）            | `1MB`（acl.json配置）  |
| **结果**       | **栈溢出，kernel执行失败** | **kernel正常执行**     |

---

### acl.json配置说明

通过`aclInit("acl.json")`加载配置文件，可在`acl.json`中配置SIMT栈大小等参数。`acl.json`配置示例如下：

```json
{
    "StackSize": {
        "simt_stack_size": 1048576,
        "simt_divergence_stack_size": 512
    }
}
```

- `simt_stack_size`：SIMT线程栈大小，单位为字节。配置为1048576（1MB），确保kernel内大局部数组不会导致栈溢出。
- `simt_divergence_stack_size`：SIMT线程分支栈大小，单位为字节。

> **参考文档：** [aclInit接口说明](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900/API/runtimeapi/aclcppdevg_03_0022.html)

---

### Case 1: 基线版本

**样例目标**：复现kernel内定义大局部数组导致的栈溢出问题

**核心实现**：
- 使用`aclInit(nullptr)`进行默认初始化，不加载配置文件
- 在kernel内定义`float local_buffer[320]`（1280B/线程），分配在栈上
- 默认栈大小不足，64线程总栈需求达80KB，导致栈溢出

**关键代码**：

```cpp
// 基线版本：使用默认配置初始化，不加载 acl.json
aclInit(nullptr);
```

```cpp
__global__ void simt_stack_overflow(float* input, uint32_t in_shape)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    float local_buffer[BUFFER_SIZE]; // 大局部数组分配在栈上
    // ...
}
```

**预期结果**：kernel执行失败，`aclGetRecentErrMsg()`返回栈溢出相关错误信息。

---

### Case 2: 优化版本

**优化目标**：通过配置文件扩大栈空间，消除栈溢出风险

**核心优化**：
- 使用`aclInit("acl.json")`加载配置文件
- `acl.json`中配置`simt_stack_size`为1MB，远大于80KB的总栈需求
- kernel代码与基线版本完全相同，无需修改

**关键代码**：

```cpp
// 优化版本：通过 aclInit 加载 acl.json 配置文件，配置 SIMT 栈大小
aclInit("acl.json");
```

**预期结果**：kernel正常执行完成。

---

## 优化建议总结

| 场景     | 问题原因                                         | 优化方案                                   |
|--------|----------------------------------------------|----------------------------------------|
| 栈溢出    | kernel内定义大局部数组（如`float buf[N]`），多线程下栈需求超限     | 通过`acl.json`配置更大的`simt_stack_size`，或迁移至全局内存 |
| 栈溢出    | 递归调用深度过大                                     | 避免在kernel中使用递归，改为迭代实现                   |
| 栈溢出    | 函数调用层级过深、参数过多                                | 简化调用链，使用结构体/全局内存传递大量参数                 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=1                       # 选择执行场景，可选1-2
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./demo                               # 执行样例
  ```

  > **说明：** `--cce-res-usage` 编译选项已默认开启，编译时会输出寄存器、Unified Buffer等硬件资源的使用情况，便于对比问题代码与优化代码的资源开销差异。

- 编译选项说明

  | 选项                        | 可选值        | 说明                                                |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM`            | `1`-`2`    | 样例类型，默认为1，1:基线版本（栈溢出），2:优化版本（配置栈大小）              |

  执行结果如下：

  Case 1（基线版本）预期输出：
  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=1)
  Running case 1: baseline (aclInit(nullptr), stack overflow)...
  ========================================
  [Host] Running kernel (SCENARIO_NUM=1)...
  ========================================
  [Host] kernel error: ... (栈溢出相关错误信息)
  ```

  Case 2（优化版本）预期输出：
  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=2)
  Running case 2: optimized (aclInit(acl.json), stack configured)...
  ========================================
  [Host] Running kernel (SCENARIO_NUM=2)...
  ========================================
  [SIMT] thread 0: local_buffer addr = 0x...
  [SIMT] thread 1: local_buffer addr = 0x...
  ...
  [Host] kernel completed.
  ```