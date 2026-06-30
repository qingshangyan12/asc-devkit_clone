# SIMT栈溢出场景及优化样例

## 概述

本样例以Kernel内定义大数组变量、递归函数调用导致栈溢出为例，介绍Ascend C SIMT编程方式下的栈溢出问题及优化思路。

- Case 1：定义大数组变量，采用默认栈大小复现栈溢出，通过配置文件扩大SIMT栈空间，解决栈溢出问题。
- Case 2：使用递归函数计算阶乘复现栈溢出，使用迭代方法计算阶乘以规避递归调用导致的栈溢出问题。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── stack_overflowl
│   ├── CMakeLists.txt          // cmake编译文件
│   ├── stack_overflow.asc      // Ascend C算子栈溢出及优化样例
│   ├── acl.json                // aclInit配置文件，配置SIMT栈大小
│   ├── README_en.md
│   └── README.md
```

## 样例描述

- 样例功能

  本样例通过构建大数组和递归计算阶乘，构造出栈溢出的场景，并通过增加栈空间大小和递归优化指导用户解决栈溢出场景。

- 样例规格：

  <table >
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SimtStackOverflow</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">input</td><td align="center">[2048]</td><td align="center">int32_t</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[2048]</td><td align="center">float</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">simt_stack_overflow / simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized</td></tr>
  </table>

## 样例实现

### 栈溢出问题说明

在SIMT的开发中，栈空间的默认大小是有限的。当函数中定义大数组，或递归调用深度过深时，栈空间的使用量会持续增加。若栈空间的使用远超栈空间限制，就会导致**栈溢出**。

### Case实现说明

本样例通过`SCENARIO_NUM`编译宏选择不同的栈溢出场景，每个Case包含问题复现和优化验证两个步骤。

| Case   | 实现特点                                  | 使用的核函数                                                                  | 解决方案          |
|--------|---------------------------------------|-------------------------------------------------------------------------|---------------|
| Case 1 | 先使用默认配置复现大数组导致的栈溢出，再通过配置更大的栈空间解决栈溢出问题 | simt_stack_overflow                                                     | 通过配置文件增加栈空间大小 |
| Case 2 | 先使用递归函数计算阶乘复现栈溢出，再使用迭代方法计算阶乘          | simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized | 通过迭代方法计算阶乘    |

#### 大数组栈溢出及配置栈大小版本场景说明

| 配置项    | Case 1复现步骤（大数组默认栈）   | Case 1优化步骤（配置栈大小）        |
|--------|----------------------|--------------------------|
| 栈开销    | `320 * 4B = 1280B`   | `320 * 4B = 1280B`       |
| 栈大小配置  | `1152B`（默认值）         | `1280B`（栈空间配置）           |
| GM占用   | `1152B*2*72=165888B` | `1280B*2*72=184320B`     |
| **结果** | **栈溢出，Kernel执行失败**   | **占用更多的GM空间，Kernel正常执行** | 

> **栈空间配置：** 参考[aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit)接口“SIMT算子栈空间大小配置示例”章节。

---

#### 递归调用和迭代方法阶乘计算场景说明

| 配置项    | Case 2复现步骤（递归版本）   | Case 2优化步骤（迭代版本） |
|--------|--------------------|------------------|
| 栈开销    | 递归调用，耗尽栈空间         | 迭代方法阶乘计算         |
| **结果** | **栈溢出，Kernel执行失败** | **Kernel正常执行**   |

> 递归不能通过单纯增大栈空间解决，必须控制递归深度或改写实现，即使配置了更大的栈空间，递归深度过深时，优先考虑迭代实现。

---

### Case 1大数组栈溢出及配置栈大小版本

**样例目标**：定义大数组导致的SIMT栈溢出问题，并通过配置更大的SIMT栈空间验证修复。

**核心实现**：

- 使用`aclInit(nullptr)`进行默认初始化，不加载配置文件
- 在Kernel内定义大数组`float local_buffer[320]`（1280B），分配在栈上
- 默认栈空间不足，导致栈溢出
- Host侧在`aclrtSynchronizeStream`后调用`aclGetRecentErrMsg()`，用于获取Kernel执行阶段的栈溢出错误详情

**关键代码**：

```cpp
__global__ void simt_stack_overflow(int32_t* input, float* output, uint32_t total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= total_length) {
        return;
    }
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

**获取失败日志及分析**

- HOST侧在`aclrtSynchronizeStream`后添加`aclGetRecentErrMsg()`获取错误日志，确认日志中出现`The VEC SIMT stack overflows`。
- 在日志中查找`fault kernel_name`，确认异常函数是否为当前问题函数。
- 在编译选项添加`--cce-res-usage`，在编译日志中查找`Stack size`，有`Stack size: xxx bytes`显示，为编译器计算出核函数变量需要栈空间大小，此样例为`1280bytes`，超出默认值`1152bytes`。
- 可通过优化核函数内部变量来减少栈空间的需求。若必须使用较大的栈空间，可通过`aclInit`接口配置自定义的栈空间大小，本样例配置为1280，即1280B。

**根据计算出所需的栈大小来配置SIMT栈空间大小**

```cpp
// 优化版本：通过aclInit加载acl.json配置文件，配置SIMT栈大小。
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

- 运行期通过`aclInit("acl.json")`加载了自定义的栈空间大小，满足函数需要的`1280 bytes`，因此并未调整核函数代码，仍然可以正常运行

**优化建议**
> **将大数组变量定义在UB空间上**
>
> - 对于定义太多变量或数组导致栈空间不足场景，优先使用优化代码的方式来解决栈溢出的情况，将局部变量定义在`UB`空间上，减少栈上的定义。
>
> **通过自定义配置扩大SIMT的栈空间**
>
> - 若必须使用超出默认配置的栈空间来进行大数组的定义，可使用`aclInit("acl.json")`接口加载自定义的配置文件来调整SIMT栈空间的大小。

---

### Case 2递归栈溢出及迭代优化版本

**样例目标**：通过递归调用计算阶乘导致的SIMT栈溢出问题，并通过迭代计算验证修复。

**核心实现**：

- 启动`simt_recursive_stack_overflow`核函数
- 调用`recursive_stack_stage`递归函数，SIMT栈空间触发栈溢出
- Host侧在`aclrtSynchronizeStream`后调用`aclGetRecentErrMsg()`，用于获取Kernel执行阶段的栈溢出错误详情

**关键代码**：

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

**获取错误及分析**

- HOST侧在`aclrtSynchronizeStream`后添加`aclGetRecentErrMsg()`获取错误日志，确认日志中出现`The VEC SIMT stack overflows`。
- 在日志中查找`fault kernel_name`，确认异常函数是否为当前问题函数。
- 在编译选项添加`--cce-res-usage`，在编译日志中查找`Stack size`，由于是递归调用，`Stack size`大小为`0`，无法在编译期计算出所需栈空间大小。
- 递归调用无法通过加大栈空间大小一次性解决，需要控制递归深度或修改递归调用为迭代方法。

**递归调用过深栈溢出优化**：

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

- 实际开发中，遇到使用递归的场景时，优先考虑使用迭代方法实现。

**优化建议：**
> **将递归改为迭代方法计算**
>
> 将递归改为迭代方法计算，减少栈空间的占用。
> 
> **控制递归深度**
> 
> 若必须使用递归计算，调用递归深度不能过深，保证有足够的栈空间。

---

**栈溢出对比总结**

Host侧通过`aclGetRecentErrMsg()`获取运行时最近一次错误详情。如果有`The VEC SIMT stack overflows`相关关键字，则可判断是栈溢出场景。如果局部变量的大小超过了栈空间的大小，考虑将局部变量放到`UB`空间或增加SIMT栈空间大小（会占用GM的空间）。在核函数中局部变量占用可以使用`--cce-res-usage`编译选项获取占用大小。如果是递归函数的栈空间，编译器无法给出正确的栈使用大小，优先修改递归调用为迭代方法。

--- 

## 优化建议

| 场景  | 问题原因                            | 优化方案                                                |
|-----|---------------------------------|-----------------------------------------------------|
| 栈溢出 | 使用Kernel内定义大数组（如`float buf[N]`） | 1：优先将大的临时变量、数组迁移至UB空间<br/>2：配置更大的栈空间大小（配置过大会占用GM空间） |
| 栈溢出 | 递归调用深度过大                        | 1：避免在核函数中使用递归，改为迭代实现 <br/>2：控制递归调用深度                |

> **建议：** 增加栈空间大小用于解决确实需要较大栈空间的场景，但不建议把大数组、大结构体等长期放在栈上。对较大的临时变量、数组，应优先评估是否可放入UB空间，减少栈开销。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=1                                                    # 选择执行场景，可选1-4
  mkdir -p build && cd build                                        # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..
  make -j                   # 编译工程
  ./demo                                                            # 执行样例
  ```

- 编译选项说明

  | 选项                        | 可选值        | 说明                                              |
  |---------------------------|------------|-------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM`            | `1`-`2`    | 样例类型，默认为1，1:大数组栈溢出及配置栈大小版本，2:递归栈溢出及迭代优化版本       |

  执行结果如下：
  Case 1/Case 2复现步骤输出：

  ```text
  ...
  The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
  ...
  ```

  Case 1/Case 2优化步骤输出：

  ```text
  ...
  test pass!
  ```
