# SIMT栈溢出场景及优化样例

## 概述

本样例以Kernel内定义大数组变量、递归函数调用导致SIMT栈溢出为例，介绍Ascend C SIMT编程方式下栈溢出问题的定位方法和优化思路。

样例包含两个Case：

- Case 1：定义大数组变量，采用默认栈大小复现栈溢出，通过配置文件扩大SIMT栈空间，解决栈溢出问题。
- Case 2：使用递归函数计算阶乘复现栈溢出，使用迭代方法计算阶乘以规避递归调用导致的栈溢出问题。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>=CANN 9.1.0

## 目录结构介绍

```text
├── stack_overflow
│   ├── CMakeLists.txt          // 样例构建脚本
│   ├── stack_overflow.asc      // SIMT栈溢出复现和优化样例
│   ├── acl.json                // aclInit配置文件，用于配置SIMT栈大小
│   ├── README_en.md
│   └── README.md
```

## 样例描述

本样例总共包含4个场景，分别复现或优化SIMT栈溢出问题，样例通过SCENARIO_NUM选择一个待测场景：

**2组Case**：

|  Case  | 对照维度                         | 参与场景      |
|:------:|------------------------------|-----------|
| Case 1 | 大数组局部变量：默认栈大小 → 配置更大的SIMT栈空间 | 场景1 ↔ 场景2 |
| Case 2 | 递归调用：递归计算阶乘 → 迭代计算阶乘         | 场景3 ↔ 场景4 |

**样例规格**：

<table >
<tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SimtStackOverflow</td></tr>
<tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">input</td><td align="center">[2048]</td><td align="center">int32_t</td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[2048]</td><td align="center">float</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">simt_stack_overflow / simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized</td></tr>
</table>

## 样例实现

本节以两组Case逐一分析大数组局部变量和递归调用两类SIMT栈溢出问题。每组Case包含复现场景和优化场景，并给出对应的核心实现、错误定位方法和优化建议。

| Case   | 实现特点                                  | 使用的核函数                                                                  | 解决方案          |
|--------|---------------------------------------|-------------------------------------------------------------------------|---------------|
| Case 1 | 先使用默认配置复现大数组导致的栈溢出，再通过配置更大的栈空间解决栈溢出问题 | simt_stack_overflow                                                     | 通过配置文件增加栈空间大小 |
| Case 2 | 先使用递归函数计算阶乘复现栈溢出，再使用迭代方法计算阶乘          | simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized | 通过迭代方法计算阶乘    |

> **栈溢出问题说明**
> 
> SIMT线程的栈空间默认大小有限。当Kernel函数中定义较大的线程局部数组，或函数递归调用深度过大时，SIMT栈空间需求会持续增加。若运行时栈空间需求超过配置上限，就会触发SIMT栈溢出。

---
### Case 1：大数组局部变量栈溢出及配置栈大小

**样例目标**：复现Kernel内定义大数组导致的SIMT栈溢出，并通过配置更大的SIMT栈空间验证修复。

**场景配置**：

| 场景 | 局部数组                       | 单线程栈需求 | SIMT栈大小配置 | GM占用                 | 预期结果           |
|:--:|:---------------------------|:-------|:----------|:---------------------|:---------------|
| 1  | `float local_buffer[320]`  | 1280B  | 默认值，1152B | `1152B*2*72=165888B` | 栈溢出，Kernel执行失败 |
| 2  | `float local_buffer[320]`  | 1280B  | 配置值，1280B | `1280B*2*72=184320B` | Kernel正常执行     |

**核心实现**：Case 1的两个场景使用相同Kernel。`simt_stack_overflow`在每个线程的栈上分配`float local_buffer[320]`，单线程数组占用`320 * 4B = 1280B`。场景1使用默认初始化，不加载配置文件；场景2加载自定义栈空间大小，参考[aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit)接口中的“SIMT算子栈空间大小配置示例”章节进行配置。

```cpp
__global__ void simt_stack_overflow(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    float local_buffer[BUFFER_SIZE];

    for (uint32_t i = 0; i < BUFFER_SIZE; i++) {
        local_buffer[i] = i + 1;
    }

    float sum = 1.0f;
    for (uint32_t i = 0; i < input[idx]; i++) {
        sum *= local_buffer[i];
    }
    output[idx] = sum;
}
```

**自定义栈空间大小配置**：

```cpp
aclInit("acl.json");
```

```json
{
    "StackSize": {
        "simt_stack_size": 1280,
        "simt_divergence_stack_size": 512
    }
}
```

**获取失败日志**

- HOST侧在`aclrtSynchronizeStream`后添加`aclGetRecentErrMsg()`获取错误日志，确认日志中出现`The VEC SIMT stack overflows`。
- 在日志中查找`fault kernel_name`，确认异常函数是否为当前问题函数。
- 在编译选项添加`--cce-res-usage`，在编译日志中查找`Stack size`，有`Stack size: xxx bytes`显示，为编译器计算出核函数变量需要栈空间大小，此样例为`1280bytes`，超出默认值`1152bytes`。

**分析**：

- 场景1使用配置默认初始化，SIMT栈空间默认值为1152B，小于该Kernel所需的1280B，因此执行阶段会触发栈溢出。
- 场景2通过自定义配置为1280B，满足该Kernel的单线程栈需求，因此Kernel可以正常执行。
- 对于静态大小的局部变量，编译日志中的`Stack size`可用于确认单线程栈空间需求。本样例中`simt_stack_overflow`对应的栈需求为1280B。

**结论**：

- 当出现栈溢出场景时，优先优化Kernel代码，避免把大数组、大结构体等长期放在线程栈上。若临时数据适合放入Unified Buffer，可迁移到UB空间，减少栈空间占用。
- 确实需要更大线程栈空间时，可通过[aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit)配置SIMT的栈空间大小，本样例配置为1280，即1280B。配置值增大后会增加GM占用，应结合业务规模评估。

### Case 2：递归栈溢出及迭代优化

**样例目标**：复现递归调用导致的SIMT栈溢出，并通过迭代实现验证修复。

**场景配置**：

| 场景 | 实现方式   | 核函数                                        | 预期结果                   |
|:--:|--------|:-------------------------------------------|:-----------------------|
| 3  | 递归计算阶乘 | `simt_recursive_stack_overflow`            | 递归调用持续叠加栈帧，Kernel执行失败  |
| 4  | 迭代计算阶乘 | `simt_recursive_stack_overflow_optimized`  | 无递归栈帧累积，Kernel正常执行     |

> 递归不能通过单纯增大栈空间解决，必须控制递归深度或改写实现，即使配置了更大的栈空间，递归深度过深时，优先考虑迭代实现。

**递归版本核心实现**：

```cpp
__aicore__ float recursive_stack_stage(int32_t value)
{
    if (value < 1.0f) {
        return 1.0f;
    }
    return recursive_stack_stage(value - 1) * value;
}

__global__ void simt_recursive_stack_overflow(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= total_length) {
        return;
    }
    output[idx] = recursive_stack_stage(input[idx]);
}
```

**迭代版本核心实现**：

```cpp
__aicore__ float recursive_stack_stage_optimized(int32_t value)
{
    float result = 1.0f;
    for (int32_t i = 2; i <= value; ++i) {
        result *= i;
    }
    return result;
}

__global__ void simt_recursive_stack_overflow_optimized(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= total_length) {
        return;
    }
    output[idx] = recursive_stack_stage_optimized(input[idx]);
}
```
**获取失败日志**：

- HOST侧在`aclrtSynchronizeStream`后添加`aclGetRecentErrMsg()`获取错误日志，确认日志中出现`The VEC SIMT stack overflows`。
- 在日志中查找`fault kernel_name`，确认异常函数是否为当前问题函数。
- 在编译选项添加`--cce-res-usage`，由于是递归调用，无法在编译期计算出所需栈空间大小，`Stack size`大小为`0`。 

**分析**：
- 递归调用会在运行时持续叠加栈帧，递归深度越大，栈空间消耗越高。
- 递归函数的实际栈需求依赖运行时的调用深度，编译器无法可靠给出完整栈使用量。因此该类问题不能只依赖编译日志中的`Stack size`判断。
- 单纯增大栈空间大小只能延后递归触发栈溢出的时机，不能从根本上解决递归深度不可控的问题。实际开发中应优先改为迭代实现，或严格控制递归深度。

**结论**
- 递归调用产生栈溢出场景时，优先将递归改为迭代方法计算，减少栈空间大小的占用。
- 若必须使用递归调用，调用递归深度不能过深，保证有足够的栈空间。

## 优化建议

| 场景      | 问题原因                                | 优化方案                                      |
|---------|-------------------------------------|-------------------------------------------|
| 大数组局部变量 | Kernel内定义较大的线程局部数组，例如`float buf[N]` | 优先迁移到UB空间或减少局部数组规模；确需较大栈空间时可自定义配置的SIMT栈空间 |
| 递归调用    | 递归深度过大导致栈帧持续累积                      | 优先改为迭代实现；必须使用递归时需严格控制递归深度                 |

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量。

  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明**：`${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 编译并运行样例。

  ```bash
  SCENARIO_NUM=1                                # Select the execution scenario, options 1-4
  mkdir -p build && cd build                    # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..;make -j # Build the project
  ./demo                                        # Run the example
  ```

  `SCENARIO_NUM`可选值为`1-4`。其中场景1和场景2属于Case 1，场景3和场景4属于Case 2。修改场景后需要重新执行`cmake`和`make`，确保编译宏生效。

**编译选项说明**：

| 选项                        | 可选值        | 说明                                              |
|---------------------------|------------|-------------------------------------------------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
| `SCENARIO_NUM`            | `1-4`      | 样例场景编号，默认值为`1`。场景1和场景2对应Case 1，场景3和场景4对应Case 2  |

**执行结果示例**：

Case 1的场景1和Case 2的场景3为栈溢出复现场景，日志中会出现如下错误信息：

```text
The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
```

Case 1的场景2和Case 2的场景4为优化验证场景，运行成功后输出如下信息：

```text
test pass!
```
