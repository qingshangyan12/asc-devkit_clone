# SIMT栈溢出场景及优化样例

## 概述

本样例以Kernel内定义大数组变量、递归函数调用导致SIMT栈溢出为例，介绍Ascend C SIMT编程方式下栈溢出问题的定位方法和优化思路。

样例包含两个Case：

- Case 1：在栈空间中使用大数组导致栈溢出，可通过配置文件扩大栈空间，但更推荐使用UB存储大量临时数据。
- Case 2：使用递归函数时，递归层数过大导致栈溢出，需控制递归层数或使用迭代算法替代递归算法。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>=CANN 9.1.0

## 目录结构介绍

```text
├── stack_overflow
│   ├── CMakeLists.txt          // 样例构建脚本
│   ├── stack_overflow.asc      // SIMT栈溢出展现和优化样例
│   ├── acl.json                // aclInit配置文件，用于配置SIMT栈大小
│   ├── README_en.md
│   └── README.md
```

## 样例描述

本样例总共包含2个典型栈溢出场景：

**Case说明**：

| Case   | 核心问题        | 对应核函数                                                                   | 解决方案          |
|--------|-------------|-------------------------------------------------------------------------|---------------|
| Case 1 | 在栈空间中存储过大数据 | simt_stack_overflow                                                     | 通过配置文件增加栈空间大小 |
| Case 2 | 递归函数调用深度过大  | simt_recursive_stack_overflow/simt_recursive_stack_overflow_optimized | 使用迭代算法替代递归算法  |

> **栈溢出问题说明**
>
> SIMT线程的栈空间默认大小有限。当Kernel函数中定义较大的线程局部数组，或函数递归调用深度过大时，SIMT栈空间需求会持续增加。若运行时栈空间需求超过配置上限，就会触发SIMT栈溢出。栈空间详情具体参考[内存层级](../../../../docs/zh/guide/%E7%BC%96%E7%A8%8B%E6%8C%87%E5%8D%97/%E7%BC%96%E7%A8%8B%E6%A8%A1%E5%9E%8B/AI-Core-SIMT%E7%BC%96%E7%A8%8B/%E5%86%85%E5%AD%98%E5%B1%82%E7%BA%A7.md)。

**样例规格**：

<table>
<tr><td rowspan="1" align="center">样例类型（OpType）</td><td colspan="3" align="center">SimtStackOverflow</td></tr>
<tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">input</td><td align="center">[2048]</td><td align="center">int32_t</td></tr>
<tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[2048]</td><td align="center">float</td></tr>
<tr><td rowspan="1" align="center">核函数名</td><td colspan="3" align="center">simt_stack_overflow/simt_recursive_stack_overflow/simt_recursive_stack_overflow_optimized</td></tr>
</table>

## 样例实现

本节以两组Case逐一分析局部大数组和递归调用两类SIMT栈溢出问题。每组Case包含展现溢出场景和优化场景，并给出对应的核心实现、错误定位方法和优化建议。

---

### Case 1：局部大数组栈溢出及配置栈大小

**样例目标**：展现在栈空间中使用大数组导致的SIMT栈溢出问题，并通过配置更大的SIMT栈空间验证修复。

**场景配置**：

| 场景 | 局部大数组                     | 单线程栈需求 | SIMT栈大小配置 | 预期结果           |
|:--:|:--------------------------|:-------|:----------|:---------------|
| 1  | `float local_buffer[320]` | 1280B  | 默认值，1152B | 栈溢出，Kernel执行失败 |
| 2  | `float local_buffer[320]` | 1280B  | 配置值，1280B | Kernel正常执行     |

**核心实现**：使用Kernel函数`simt_stack_overflow`，在每个线程的栈上分配大数组`float local_buffer[320]`，单线程数组占用`320 * 4B = 1280B`。

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

**获取失败日志**：

- 执行上述实现出现精度校验失败，本样例中是`[Failed] Case accuracy verification failed!`。
- 若确认问题不是由计算逻辑引起，则需要获取失败日志。
- 获取失败日志，HOST侧在`aclrtSynchronizeStream`之后添加`aclGetRecentErrMsg()`获取到的错误日志如下：

```text
The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
For details, see the troubleshooting document on the Ascend official website. Search for the keyword "AI Core Error".[FUNC:ProcessDavidStarsCoreErrorInfo][FILE:device_error_proc_c.cc][LINE:749]
       An error occurred in the kernel task, retCode=0x31, [vector core exception].[FUNC:PreCheckTaskErr][FILE:davinci_kernel_task.cc][LINE:1262]
       Vector Core kernel execution failed, retCode=0x31.[FUNC:GetError][FILE:stream.cc][LINE:1750]
       [AIC_INFO] after execute:args print end[FUNC:GetError][FILE:stream.cc][LINE:1750]
       [DFX_INFO]AI Core kernel execution failed, device_id=0, stream_id=61, report_stream_id=61, task_id=0, flip_num=0, fault kernel_name=_Z19simt_stack_overflowPiPfj, fault kernel info ext=_Z19simt_stack_overflowPiPfj, program id=0, hash=136201840501799437.[FUNC:GetError][FILE:stream.cc][LINE:1750]
       rtStreamSynchronize execution failed, reason=vector core exception[FUNC:FuncErrorReason][FILE:error_message_manage.cc][LINE:69]
       synchronize stream failed, runtime result = 507035[FUNC:ReportCallError][FILE:log_inner.cpp][LINE:148]
```

在日志中查找`fault kernel_name`，可定位到当前问题函数。同时通过关键字`errorStr:`检测到日志中出现`The VEC SIMT stack overflows`信息，可判断出是栈溢出。

- 在编译选项中添加`--cce-res-usage`，获取栈空间使用情况：

```text
[BISHENG] Function properties for _Z19simt_stack_overflowPiPfj_simt_entry: Stack size: 1280 bytes, Used register number: 16
```

日志中的`Stack size: 1280 bytes`，表示核函数变量所需栈空间为`1280B`，超过默认值`1152B`。

**分析日志**：

- 通过上述错误日志，判断为SIMT栈溢出错误。编译信息显示所需栈空间为1280B，超出SIMT栈空间的默认值1152B，结合代码中有定义`float local_buffer[320]`占用空间为1280B，确认栈空间有大数组的定义产生栈溢出。
- 对于大数组的变量若适合放入UB空间，可迁移到UB空间，减少栈的占用，并且可以在线程间共享。
- 若确需在栈空间存储大量临时数据，可通过增大SIMT栈空间，满足Kernel的栈空间需求解决栈溢出问题。

**解决方案**：增大SIMT栈空间，满足核函数需要的栈空间，可参考[aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit)接口中的“SIMT算子栈空间大小配置示例”章节进行配置。

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

**结论**：

- 减少临时变量：能用地址引用的尽量不用临时变量，可减少寄存器的使用。
- 优化大数组：当出现栈溢出场景时，优先优化Kernel代码，避免把大数组、大结构体等长期放在线程栈上，若临时数据适合放入UB，可迁移到UB空间，减少栈空间占用。
- 增大SIMT栈空间：配置SIMT栈空间大小会对所有算子生效，同时配置值增大后会增加GM占用，应结合业务规模评估。

> SIMT栈空间默认大小为1152B。本样例将SIMT栈空间配置为1280，即1280B，线程数为1024，因此额外占用GM空间为`(1280B - 1152B) * 1024 = 128KB`。

### Case 2：递归栈溢出及迭代优化

**样例目标**：展现递归调用导致的SIMT栈溢出，并通过迭代实现验证修复。

**场景配置**：

| 场景 | 实现方式   | 核函数                                        | 预期结果                   |
|:--:|--------|:-------------------------------------------|:-----------------------|
| 3  | 递归计算阶乘 | `simt_recursive_stack_overflow`            | 递归调用持续叠加栈帧，Kernel执行失败  |
| 4  | 迭代计算阶乘 | `simt_recursive_stack_overflow_optimized`  | 无递归栈帧累积，Kernel正常执行     |

> 递归不能通过单纯增大分支栈空间解决，必须控制递归深度或改写实现，即使配置了更大的分支栈空间，递归深度过深时仍然会出现栈溢出问题，因此应优先考虑迭代算法实现。

**核心实现**：使用递归算法实现阶乘计算。

```cpp
__aicore__ float recursive_stack_stage(int32_t value)
{
    if (value < 1) {
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

**获取失败日志**：

- 执行上述实现出现精度校验失败，本样例中是`[Failed] Case accuracy verification failed!`。
- 若确认问题不是由计算逻辑引起，则需要获取失败日志。
- 获取失败日志，HOST侧在`aclrtSynchronizeStream`之后添加`aclGetRecentErrMsg()`获取到的错误日志如下：

```text
The extend info: errcode:(356) errorStr: VEC SIMT DVG stack overflows, which may be caused by too many conditional branches or too manay nested loops. subErrType: 0x4.
For details, see the troubleshooting document on the Ascend official website. Search for the keyword "AI Core Error".[FUNC:ProcessDavidStarsCoreErrorInfo][FILE:device_error_proc_c.cc][LINE:749]
       An error occurred in the kernel task, retCode=0x31, [vector core exception].[FUNC:PreCheckTaskErr][FILE:davinci_kernel_task.cc][LINE:1262]
       Vector Core kernel execution failed, retCode=0x31.[FUNC:GetError][FILE:stream.cc][LINE:1750]
       [AIC_INFO] after execute:args print end[FUNC:GetError][FILE:stream.cc][LINE:1750]
       [DFX_INFO]AI Core kernel execution failed, device_id=0, stream_id=61, report_stream_id=61, task_id=0, flip_num=0, fault kernel_name=_Z29simt_recursive_stack_overflowPiPfj, fault kernel info ext=_Z29simt_recursive_stack_overflowPiPfj, program id=0, hash=136201840501799437.[FUNC:GetError][FILE:stream.cc][LINE:1750]
       rtStreamSynchronize execution failed, reason=vector core exception[FUNC:FuncErrorReason][FILE:error_message_manage.cc][LINE:69]
       synchronize stream failed, runtime result = 507035[FUNC:ReportCallError][FILE:log_inner.cpp][LINE:148]
```

在日志中查找`fault kernel_name`，可定位到当前问题函数。同时通过关键字`errorStr:`检测到日志中出现`VEC SIMT DVG stack overflows`，可判断出是SIMT分支栈溢出错误。

**日志分析**：

- 通过上述错误，可判断为SIMT分支栈溢出，查看代码可发现中存在递归函数的调用，因为递归调用会持续叠加栈帧，递归深度越大，分支栈空间消耗越大，最终导致分支栈溢出。
- 单纯增加分支栈空间只能延后触发SIMT分支栈溢出的时机，无法从根本上解决递归深度不可控的问题。
- 对于SIMT分支栈溢出场景，优先改为迭代实现，或严格控制递归深度。

**解决方案**：将递归算法为迭代算法，实现阶乘计算。

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

**结论**：

- 优化递归算法：递归调用产生栈溢出时，优先将递归改为迭代方法计算，减少分支栈空间的占用。
- 限制递归深度：设置递归的最大深度，确保递归函数有明确的结束条件。
- 扩大SIMT分支栈空间：若必须使用递归调用，需要保证有足够的分支栈空间，可通过[aclInit](https://hiascend.com/document/redirect/CannCommunityruntimeapiaclinit)配置SIMT分支栈空间大小。

## 优化建议

| 场景      | 问题原因                                | 优化方案                                            |
|---------|-------------------------------------|-------------------------------------------------|
| 大数组局部变量 | Kernel内定义较大的线程局部数组，例如`float buf[N]` | 优先迁移到UB空间或减少局部数组规模；确需较大栈空间时可自定义SIMT栈空间大小        |
| 递归调用    | 递归深度过大导致栈帧持续累积                      | 优先改为迭代实现；必须使用递归时需严格控制递归深度；增加SIMT算子的分支栈空间大小（不建议） |
| 临时变量过多  | 临时变量定义过多                            | 减少临时变量，能用地址引用的尽量不用临时变量，可减少寄存器的使用                |
| 分支过多    | 调用函数层次过深                            | 优化分支处理逻辑，减少Warp内的分支跳转                           |
| 复杂的结构体  | 传入复杂结构体                             | 对于复杂的结构体，可以规划UB或GM内存存放结构体数据，避免传参过大              |

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
  SCENARIO_NUM=1                       # 选择执行场景，可选1-4
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./demo                               # 执行样例
  ```

**编译选项说明**：

| 选项                        | 可选值        | 说明                                                                   |
|---------------------------|------------|----------------------------------------------------------------------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT）                      |
| `SCENARIO_NUM`            | `1-4`      | 样例场景编号，默认值为1。1：Case 1的溢出场景，2：Case 1的优化场景，3：Case 2的溢出场景，4：Case 2的优化场景 |

**执行结果示例**：

Case 1的栈溢出场景，日志中会出现如下错误信息：

```text
The extend info: errcode:(354) errorStr: The VEC SIMT stack overflows. Possible cause: The local variable is too large or there are too many local variables. subErrType: 0x4.
```

Case 2的栈溢出场景，日志中会出现如下错误信息：

```text
The extend info: errcode:(356) errorStr: VEC SIMT DVG stack overflows, which may be caused by too many conditional branches or too manay nested loops. subErrType: 0x4.
```

Case 1和Case 2的优化验证场景，运行成功后输出如下信息：

```text
test pass!
```
