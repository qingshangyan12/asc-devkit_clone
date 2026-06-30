# SIMT栈溢出场景及优化样例

## 概述

本样例以Kernel内定义大数组变量、递归函数调用导致栈溢出为例，介绍Ascend C SIMT编程方式下的栈溢出问题及优化思路。
- Case 1：定义大数组变量，采用默认栈大小，较大的栈开销会导致栈溢出。
- Case 2：定义大数组变量，通过`aclInit("acl.json")`加载配置文件，配置更大的SIMT栈空间，解决栈溢出问题。
- Case 3：使用递归函数计算阶乘，用于复现递归调用导致的栈溢出。
- Case 4：使用迭代方法计算阶乘，SIMT合理使用迭代方法可以规避递归调用导致栈溢出。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── stack_overflow
│   ├── CMakeLists.txt          // cmake编译文件
│   ├── stack_overflow.asc      // Ascend C算子栈溢出及优化样例
│   ├── acl.json                // aclInit配置文件，配置SIMT栈大小
│   ├── README_en.md
│   └── README.md
```

## 样例描述

- 样例功能

  本样例通过构建大数组和递归计算阶乘，构造出栈溢出的场景，并通过增加栈大小和递归优化指导用户解决栈溢出场景。

- 样例规格：

  <table >
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SimtStackOverflow</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[2048]</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">simt_stack_overflow / simt_recursive_stack_overflow / simt_recursive_stack_overflow_optimized</td></tr>
  </table>

## 样例实现

### 栈溢出问题说明

在SIMT的开发中，栈空间的默认大小是有限的。当函数中定义大数组，或递归调用时，栈空间的使用量会持续增加。若栈空间的使用远超栈空间限制，就会导致**栈溢出**。

### Case实现说明

本样例通过`SCENARIO_NUM`编译宏选择不同的初始化配置和核函数实现，每个Case对应特定的栈溢出场景或说明版本。

| Case   | 实现特点                                      | 使用的核函数                                    | 优化特性                |
|--------|---------------------------------------------|-----------------------------------------------|---------------------|
| Case 1 | 使用`aclInit(nullptr)`，Kernel内定义大数组`local_buffer[320]`，默认栈大小不足导致栈溢出 | simt_stack_overflow                           | 大数组版本，存在栈溢出问题        |
| Case 2 | 使用`aclInit("acl.json")`，同一Kernel通过`acl.json`配置更大的`simt_stack_size`，避免栈溢出 | simt_stack_overflow                           | 大数组栈溢出优化版本，通过配置文件扩大栈空间    |
| Case 3 | 使用递归函数计算阶乘，随着递归层级的增长并触发栈溢出 | simt_recursive_stack_overflow                 | 递归计算阶乘引起栈溢出示例        |
| Case 4 | 使用迭代方式计算阶乘 | simt_recursive_stack_overflow_optimized       | 迭代方式计算阶乘    |

#### 栈空间分析

| 配置项          | Case 1（大数组版本）      | Case 2（配置栈大小版本）       |
|--------------|--------------------|--------------------|
| 栈开销       | `320 * 4B = 1280B` | `320 * 4B = 1280B`  |
| 栈大小配置        | 默认值（较小）            | `2048B`（acl.json配置）  |
| **结果**       | **栈溢出，Kernel执行失败** | **Kernel正常执行**     |

### acl.json配置说明

通过`aclInit`加载配置文件，可在`acl.json`中配置SIMT栈空间大小等参数。CMake会将样例根目录下的`acl.json`复制到build目录，因此源码中使用`aclInit("acl.json")`加载当前运行目录下的配置文件。`acl.json`配置示例如下：

```json
{
    "StackSize": {
        "simt_stack_size": 2048,
        "simt_divergence_stack_size": 512
    }
}
```

- `simt_stack_size`：SIMT每个线程的栈空间大小，单位为Byte。配置为2048，大于本样例大数组场景的栈需求，也为递归场景预留栈空间。对于递归计算，增大栈空间只能延后失败，不能从根本上避免栈溢出。
- `simt_divergence_stack_size`：SIMT的分支栈空间大小，单位为Byte。

> **参考文档：** [aclInit接口说明](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/900/API/runtimeapi/aclcppdevg_03_0022.html)

---

#### 递归调用和迭代方式阶乘计算场景说明

| 配置项          | Case 3（递归版本）      | Case 4（迭代版本）       |
|--------------|--------------------|--------------------|
| 栈开销       | 递归调用，耗尽栈空间  | 迭代方法阶乘计算  |
| **结果**       | **栈溢出，Kernel执行失败** | **Kernel正常执行**     |

> 递归不能通过单纯增大`simt_stack_size`解决，必须控制递归深度或改写实现，即使配置了更大的`simt_stack_size`，递归深度过深时，优先考虑迭代实现。

---

### Case 1大数组版本

**样例目标**：定义大数组导致的SIMT栈溢出问题。

**核心实现**：
- 使用`aclInit(nullptr)`进行默认初始化，不加载配置文件
- 在Kernel内定义大数组`float local_buffer[320]`（1280B），分配在栈上
- 默认栈空间不足，导致栈溢出
- Host侧在`aclrtSynchronizeStream`后调用`aclGetRecentErrMsg()`，用于获取Kernel执行阶段的栈溢出错误详情

**关键代码**：

```cpp
// 大数组版本：使用默认配置初始化，不加载acl.json。
aclInit(nullptr);
```

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

**结果**：
- 编译过程中打印所需栈空间大小，`_Z19simt_stack_overflowPfj_simt_entry: Stack size: 1280 bytes, Used register number: 16`，执行失败，`aclGetRecentErrMsg()`返回栈溢出相关错误信息。

**数据分析**：
- 函数内的大数组定义为静态定义，在编译器期使用编译选项`--cce-res-usage`可以计算出所需栈空间的大小，因此可以看到需要的栈空间为`1280 bytes`
- 运行期通过`aclGetRecentErrMsg()`获取真实的错误信息，查看日志包含错误信息`The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables`，可进一步确定为栈溢出场景

**优化建议**：
> **将大数组变量定义在UB空间上**
>
> - 对于定义太多变量或数组导致栈空间不足场景，优先使用优化代码的方式来解决栈溢出的情况，将局部变量定义在`UB`空间上，减少栈上的定义。
>
> **通过自定义配置扩大SIMT的栈空间**
>
> - 若必须使用超出默认配置的栈空间来进行大数组的定义，可使用`aclInit("acl.json")`加载自定义的配置文件来调整SIMT栈空间的大小。

---

### Case 2自定义配置栈大小版本

**样例目标**：定义大数组并通过`acl.json`配置更大的SIMT栈空间，验证大数组场景下的栈溢出修复。

**核心实现**：
- 使用`aclInit("acl.json")`加载配置文件
- 函数实现与Case 1相同，仍然在栈上定义`float local_buffer[320]`
- `acl.json`中配置`simt_stack_size=2048`，提供2048B的SIMT栈空间
- 2048B栈空间大于本样例`1280B`的栈需求，函数可正常执行

**关键代码**：

```cpp
// 优化版本：通过aclInit加载acl.json配置文件，配置SIMT栈大小。
aclInit("acl.json");
```

```json
{
    "StackSize": {
        "simt_stack_size": 2048,
        "simt_divergence_stack_size": 512
    }
}
```

**结果**：
- 编译过程中打印所需栈空间的大小，`_Z19simt_stack_overflowPfj_simt_entry: Stack size: 1280 bytes, Used register number: 16`，执行成功，不再出现`The VEC SIMT stack overflows`错误。

**数据分析**：
- 函数内的大数组定义为静态定义，在编译器期使用编译选项`--cce-res-usage`可以计算出所需栈空间的大小，因此可以看到需要的栈空间为`1280 bytes`
- 运行期通过`aclInit("acl.json")`加载了自定义的栈空间大小，满足函数需要的`1280 bytes`，并未调整核函数代码，仍然可以正常运行

---

### Case 3递归栈溢出版本

**样例目标**：通过递归调用计算阶乘导致的SIMT栈溢出问题。

**核心实现**：
- 使用`aclInit(nullptr)`进行默认初始化，不加载配置文件
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
    if (idx > total_length) {
        return;
    }
    output[idx] = recursive_stack_stage(input[idx]);
}
```

**结果**：
- 编译器打印所需栈空间大小为0，`_Z29simt_recursive_stack_overflowPfj_simt_entry: Stack size: 0 bytes, Used register number: 10`，核函数执行失败，`aclGetRecentErrMsg()`返回`The VEC SIMT stack overflows`相关错误信息。

**数据分析：**
- 由于递归函数在编译期无法直接计算出所需要的栈空间大小，此时会打印所需栈空间大小为0
- 递归调用在运行期会占满栈空间的大小，因此递归不能通过单纯增大`simt_stack_size`彻底修复，只能减少递归深度或改写为迭代实现处理

**优化建议：**
> **将递归改为迭代方法计算**
>
> 将递归改为迭代方式计算，如果非要使用递归计算，需保证调用递归深度不能过深，保证有足够的栈空间。

---

### Case 4迭代方法计算阶乘版本

**样例目标**：通过迭代方法计算阶乘解决递归调用导致的SIMT栈溢出问题。

**核心实现**：
- 使用`aclInit(nullptr)`进行默认初始化，不加载配置文件
- 启动`simt_recursive_stack_overflow_optimized`核函数
- 调用`recursive_stack_stage_optimized`函数，通过迭代方法计算阶乘

**关键代码**：

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
    if (idx > total_length) {
        return;
    }
    output[idx] = recursive_stack_stage_optimized(input[idx]);
}
```

**结果**：
- Kernel正常执行，日志输出`[Host] kernel completed.`。

**数据分析**：
- Case 4用于说明迭代方法可以减少栈开销。实际开发中，遇到使用递归的场景时，优先考虑使用迭代方式实现。

---

### 栈溢出对比总结

在核函数中静态变量定义占用的栈空间大小可以使用`--cce-res-usage`编译选项获取占用栈的大小。如果是递归函数的栈空间，编译器无法给出正确的栈使用大小。Host侧通过`aclGetRecentErrMsg()`获取运行时最近一次错误详情。如果有`The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables`相关关键字，则可判断是栈溢出场景。如果局部变量的大小超过了栈空间的大小，考虑将局部变量放到`UB`空间或增加SIMT栈空间大小。

> **说明：** 本样例用于演示问题定位流程，Case 1打印Kernel错误后进程返回码仍为0；判断栈溢出是否复现，应以日志中是否出现`The VEC SIMT stack overflows`为准。

---

### 问题解决步骤

当用户遇到SIMT kernel栈溢出时，可按以下步骤处理：

1. 复现问题：先运行Case 1或Case 3，确认日志中出现`The VEC SIMT stack overflows`。
2. 确认触发位置：在日志中查找`fault kernel_name`，确认异常函数是否为当前问题函数。
3. 查看栈空间开销：在编译日志中查找`Stack size`，该值表示单个SIMT每个线程的栈空间使用量，如果`Stack size`大小为0，栈空间的使用可能需要在运行期确定，无法在编译期确定。
4. 优先优化栈使用：较大的临时变量、数组尽量不要定义为栈空间的局部变量，能使用`UB`空间时优先迁移到`UB`。
5. 必须使用较大的栈空间场景需要修改配置文件：若仍需要较大的栈空间，在`acl.json`中配置更大的`simt_stack_size`。本样例配置为`2048`，即2048B。
6. 加载配置文件：Host侧通过`aclInit("acl.json")`加载配置。若从其他目录运行，请根据实际工作目录调整配置文件路径。
7. 验证修复：重新编译并运行对应配置栈大小版本。
  - Case 1对应的Case 2，确认日志输出`[Host] kernel completed.`，且不再出现SIMT栈溢出错误。
  - 对于Case 3的递归问题，应通过减少递归的调用或改为迭代实现处理，Case 4用于说明修改为迭代方式后没有栈溢出问题。

> 若配置栈大小版本仍然失败，优先检查Kernel中是否新增了更大的数组或递归调用。

## 优化建议

| 场景     | 问题原因                                       | 优化方案                                                    |
|--------|--------------------------------------------|---------------------------------------------------------|
| 栈溢出    | 使用Kernel内定义大数组（如`float buf[N]`）     | 优先将大的临时变量、数组迁移至UB空间；或通过`acl.json`配置更大的`simt_stack_size` |
| 栈溢出    | 递归调用深度过大                                   | 减少递归调用深度；避免在核函数中使用递归，改为迭代实现                             |

> **建议：** `simt_stack_size`用于解决确实需要较大栈空间的场景，但不建议把大数组、大结构体等长期放在栈上。对较大的临时变量、数组，应优先评估是否可放入UB空间，减少栈开销。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，配置环境变量。

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

  > **说明：** `--cce-res-usage` 编译选项已默认开启，编译时会输出栈空间使用情况。Case 1/Case 2为数组申请，输出`Stack size: 1280 bytes`，Case 3/Case 4是递归调用，编译期无法获取栈空间的使用情况，运行结果为准。

- 编译选项说明

  | 选项                        | 可选值        | 说明                                                 |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM`            | `1`-`4` | 样例类型，默认为1，1:大数组版本，2:大数组配置栈大小版本，3:递归栈溢出版本，4:迭代方法实现阶乘 |

  执行结果如下：
  Case 1/Case 3输出：
  ```text
  ...
  The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
  ...
  ```

  Case 2/Case 4输出：
  ```text
  ...
  test pass!
  ```
