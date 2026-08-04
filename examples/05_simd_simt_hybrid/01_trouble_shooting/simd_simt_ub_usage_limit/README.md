# SIMD与SIMT混合编程UB越界定位样例

## 概述

本样例以SIMD与SIMT混合编程场景下UB访问越界导致输出精度异常为例，介绍如何结合UB容量、内存布局和实际访问范围定位并修复UB访问越界。样例随后通过多个Case说明不同VF配置下的UB可用上限、静态UB和动态UB的布局关系，以及越界访问不同UB区域时的运行和精度结果。

本样例主要内容如下表所示。

| 内容类别 | 对应Case | 主要内容 |
| --- | --- | --- |
| 问题定位 | Case 0 | 从输出精度异常出发，定位UB越界写入，并给出修复方式 |
| UB容量边界 | Case 1 | 说明不同VF调用、预留空间和Data Cache配置下的UB可用上限 |
| UB布局关系 | Case 2 | 说明MainScalar、SIMD VF和SIMT VF中声明的静态UB如何共同占用静态内存区域，以及动态UB共用方式 |
| 越界访问结果 | Case 3 | 说明越过已申请内存范围写入后续UB区域时的运行和精度结果 |

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── simd_simt_ub_usage_limit
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── ub_usage_limit_common.h// 公共声明和工具函数
│   ├── case0_ub_out_of_bounds.asc       // Case 0样例实现
│   ├── case1_ub_limit.asc     // Case 1样例实现
│   ├── case2_ub_layout.asc    // Case 2样例实现
│   ├── case3_out_of_bounds_result.asc   // Case 3样例实现
│   ├── figures                // README引用的图片
│   ├── README.md              // 中文说明
│   └── README_en.md           // 英文说明
```

## 样例描述

本样例包含4个Case，围绕UB访问越界定位、容量边界和内存布局展开。

**Case说明**：

| Case | 场景 | 主要内容 |
| --- | --- | --- |
| Case 0 | UB访问越界 | 展现精度问题，并根据UB布局和访问偏移定位、修复越界写 |
| Case 1 | 不同VF场景下的UB可用上限 | 确定不同VF调用、预留空间和Data Cache配置下的UB可用上限 |
| Case 2 | 不同执行位置的静态UB和动态UB布局 | 说明MainScalar、SIMD VF和SIMT VF中的静态UB和动态UB布局关系 |
| Case 3 | 静态UB数组越界写入不同区域 | 说明越过已申请内存范围写入后续UB区域时的运行及精度结果 |

**样例规格**：

<table>
<tr><td align="center">样例类型（OpType）</td><td colspan="4" align="center">SimdSimtUbUsageLimit</td></tr>
<tr><td rowspan="2" align="center">样例输入</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0、Case 3</td><td align="center">input</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">样例输出</td><td align="center">Case 0、Case 3</td><td align="center">output</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td rowspan="4" align="center">核函数名</td><td align="center">Case 0</td><td colspan="3" align="center"><code>static_ub_out_of_bounds_kernel</code></td></tr>
<tr><td align="center">Case 1</td><td colspan="3" align="center"><code>ub_limit_kernel</code> / <code>simt_ub_limit_kernel</code></td></tr>
<tr><td align="center">Case 2</td><td colspan="3" align="center"><code>ub_layout_kernel</code></td></tr>
<tr><td align="center">Case 3</td><td colspan="3" align="center"><code>static_ub_out_of_bounds_kernel</code></td></tr>
</table>

Case 1和Case 2用于验证UB容量和布局，不涉及GM输入输出数据校验，因此不在样例输入输出中单独列出。

## 样例实现

本节按照“问题展现与修复、UB可用上限确认、内存布局说明、越界结果分析”的顺序介绍各Case。

### Case 0：UB访问越界

**样例目标**：

展现分块数据校准流程中静态UB区域偏移计算错误，导致Kernel未报告运行错误但输出精度校验失败的问题，并通过检查UB容量、内存布局和实际访问范围定位原因，验证修复方法。

**核心实现**：

Case 0实现一个简化的数据校准流程：输入`input`表示待校准数据，Kernel根据`input`的低8 bit生成校准偏置`bias`，并将`input + bias`写入`output`：

```text
bias[i] = input[i] & 0xff
output[i] = input[i] + bias[i]
```

Kernel申请216KB静态UB数组作为分块计算的临时缓存，并按`TILE_COUNT`划分为4个分段，`TILE_WORDS`表示每个分段包含的`uint32_t`元素个数。每次处理时，`tile_id`选择当前使用的分段，SIMT VF在该分段内保存从GM搬入的`input`和根据`input`生成的`bias`；随后Kernel根据`tile_id`和`lane_id`计算`bias_word_offset`，向指定`bias`位置写入数据；最后SIMT VF读取同一分段中的`input`和`bias`，计算`output`并写回GM。Case 0的静态UB分段和越界写入位置如下图所示：

![Case 0静态UB分段与越界写入位置](./figures/case0_static_ub_layout.png "Case 0静态UB分段与越界写入位置")

问题代码在计算Tile步长时误把8KB预留空间计入可用静态UB范围，导致`bias_word_offset`越过216KB静态UB声明范围并落到Data Cache。

关键代码如下：

```cpp
__simt_vf__ inline void simt_prepare_input_ub(
    __gm__ const uint32_t* input, __ubuf__ uint32_t* static_ub, uint32_t tile_id)
{
    const uint32_t tile_base = (tile_id & (TILE_COUNT - 1)) * TILE_WORDS;
    ...
    static_ub[tile_base + INPUT_OFFSET + lane] = input[lane];
    static_ub[tile_base + BIAS_OFFSET + lane] = input[lane] & 0xff;
}

__simt_vf__ inline void simt_store_calibrated_output(
    __gm__ const uint32_t* input, __ubuf__ uint32_t* static_ub, __gm__ uint32_t* output,
    uint32_t word_count, uint32_t tile_id)
{
    const uint32_t tile_base = (tile_id & (TILE_COUNT - 1)) * TILE_WORDS;
    ...
    const uint32_t bias = static_ub[tile_base + BIAS_OFFSET + index];
    output[index] = input[index] + bias;
}

__aicore__ inline uint32_t calc_bias_word_offset(uint32_t tile_id, uint32_t lane_id)
{
    const uint32_t tile_stride_words =
        (UB_BYTES + RESERVED_UB_BYTES) / TILE_COUNT / sizeof(uint32_t);
    return (tile_id & (TILE_COUNT - 1)) * tile_stride_words + tile_stride_words + lane_id;
}

__global__ __vector__ void static_ub_out_of_bounds_kernel(...)
{
    __ubuf__ uint32_t static_ub[216 * 1024 / sizeof(uint32_t)];
    uint8_t mutex_id = 0;

    asc_lock(PIPE_V, mutex_id);
    asc_vf_call<simt_prepare_input_ub>(dim3(256), input, static_ub, tile_id);
    asc_unlock(PIPE_V, mutex_id);

    asc_lock(PIPE_S, mutex_id);
    const uint32_t bias_word_offset = calc_bias_word_offset(tile_id, lane_id);
    static_ub[bias_word_offset] = CORRUPTED_VALUE;
    asc_unlock(PIPE_S, mutex_id);

    asc_lock(PIPE_V, mutex_id);
    asc_vf_call<simt_store_calibrated_output>(
        dim3(THREAD_COUNT), input, static_ub, output, BUFFER_WORDS, tile_id);
    asc_unlock(PIPE_V, mutex_id);
}
```

**执行结果**：

复杂偏移计算得到的写入位置越过静态UB并落入Data Cache后，Kernel未报告运行错误，但输出精度校验失败。

关键输出：

```text
[Case0] mode=baseline, access_byte_offset=229376
[Case0] corrupted_output: expected=0xc0000000, actual=0xdeadbeef
[Case0] precision_check=FAILED (fault reproduced)
```

**问题定位**：

精度问题通常按“执行状态确认、计算逻辑排查、内存踩踏分析”的顺序定位。

1. 确认Kernel是否正常执行。先配置Runtime/ACL日志落盘路径，并按需设置日志是否打屏。随后检查日志文件中是否出现`Ascend Error`、`EZ9999`、`vector core exception`、`ASSERT`等Kernel启动、同步或AICore执行异常信息。日志落盘路径需配置为已存在目录，日志环境变量的使用方式可参考[Ascend日志功能样例](../../../01_simd_cpp_api/01_utilities/09_log/README.md)。

    ```bash
    export ASCEND_PROCESS_LOG_PATH=./log       # 日志落盘路径
    export ASCEND_SLOG_PRINT_TO_STDOUT=0       # 控制日志是否打屏
    export ASCEND_GLOBAL_LOG_LEVEL=3           # Runtime/ACL日志只输出ERROR级别
    ./ub_usage_limit                           # 执行Case 0基线场景
    ```

    执行完成后，`./log`目录下会生成Runtime/ACL日志文件。当前场景的日志文件中未观察到Kernel启动、同步或AICore执行异常，说明前述精度异常不是由Kernel执行失败直接导致。接下来需要继续排查输出数据不符合预期的原因。

2. 排查Kernel计算逻辑。先核对Host侧Golden与Kernel输出公式是否一致。Host侧输入按`INPUT_BASE_VALUE + index`生成，Golden按`input + (input & 0xff)`计算：

    ```cpp
    input[index] = INPUT_BASE_VALUE + index;
    const uint32_t expected = input + (input & 0xff);
    ```

    再核对Kernel中`bias`生成和最终输出逻辑。SIMT VF将`bias`写为`input & 0xff`，最终输出按`input + bias`写回GM：

    ```cpp
    static_ub[tile_base + BIAS_OFFSET + lane] = input[lane] & 0xff;
    const uint32_t bias = static_ub[tile_base + BIAS_OFFSET + index];
    output[index] = input[index] + bias;
    ```

    Host侧Golden和Kernel输出公式一致，输入数据和计算流程没有明显异常；正常运行仍出现精度错误时，需要进一步考虑是否存在内存被踩、数据变脏的问题。

3. 回查可能写坏数据的内存访问。错误输出为`0xDEADBEEF`，与Kernel写入静态UB数组的值一致，因此重点检查写入`bias`存储区域时使用的运行时偏移。问题代码将`216KB静态UB + 8KB预留空间`误作为可用静态UB空间，并按4个Tile分段计算偏移，每段大小为`(216KB + 8KB) / 4 = 56KB`。当`tile_id = 3`、`lane_id = 0`时，目标字节偏移为：

    ```text
    block_id = tile_id & 3 = 3
    bias_word_offset = block_id * 56KB / 4B + 56KB / 4B + lane_id
                     = 57344
    byte_offset = bias_word_offset * 4B = 229376B = 224KB
    ```

4. 结合UB布局确认越界落点。Case 0未添加关闭预留空间的编译选项，申请216KB静态UB，不配置动态UB。其余空间包括8KB预留空间和32KB Data Cache。Data Cache是SIMT专有空间，用于缓存SIMT线程访问GM的数据，空间范围为32KB～128KB。

    ```text
    低地址 0
    ├── 静态UB       216KB
    ├── 预留空间       8KB
    └── Data Cache    32KB
    高地址 256KB
    ```

    Data Cache大小为`min(256KB - 216KB - 0KB - 8KB, 128KB) = 32KB`，起始地址为`256KB - 32KB = 224KB`。前面计算出的224KB字节偏移已经超出216KB静态UB区域声明范围，并正好位于Data Cache起始位置。前置SIMT VF阶段读取GM输入后，数据进入Data Cache；越界写随后破坏缓存数据。后续SIMT VF继续读取相同输入并结合静态UB区域中的`bias`生成`output`，因此出现精度异常。

**解决方案**：

- 在写入静态UB数组前检查计算出的实际访问范围。仅当`bias_word_offset`小于静态UB数组元素个数`UB_WORDS`时执行写入，避免访问超出声明范围。

    ```cpp
    if (bias_word_offset < UB_WORDS) {
        static_ub[bias_word_offset] = CORRUPTED_VALUE;
    }
    ```

- 增加下标检查后，越界写入被阻止，输出精度校验通过：

    ```text
    [Case0] mode=fixed, access_byte_offset=229376
    [Case0] precision_check=PASSED (out-of-bounds write blocked)
    ```

**结论**：

修复模式下输出精度校验通过。UB越界不一定触发Kernel启动或运行错误，当错误访问落入Data Cache时，可能破坏SIMT访问GM时缓存的数据，并最终表现为精度异常。因此，定位和修复UB访问越界问题时，不仅要检查UB申请总量，还要检查由运行时偏移计算出的实际访问范围。

Case 0说明了UB访问越界可能表现为精度异常。混合编程场景涉及MainScalar、SIMD VF、SIMT VF等不同执行位置，UB既是各执行位置申请静态或动态内存的空间，也用于VF与MainScalar之间的数据通信。为避免开发者越界使用UB，下面通过Case 1～3说明不同VF配置下的UB可用上限、静态UB与动态UB的布局关系，以及越界写入不同区域时的结果。

### Case 1：不同VF场景下的UB可用上限

**样例目标**：

确定不同VF调用和预留配置下的UB可用上限。

**核心实现**：

本Case包含三条互斥路径：不调用VF、仅调用SIMD VF、仅调用SIMT VF。存在VF调用时，分别验证默认配置、关闭VF预留和关闭全部预留时的UB可用上限。

单个Vector Core共有256KB UB。存在VF调用时，默认配置下有8KB预留空间（6KB VF预留和2KB Ascend C API预留）；不调用VF时不按该预留计算。`--cce-disable-vf-stack-reserved-ubuf`用于关闭6KB VF预留，`--cce-disable-asc-reserved-ubuf`用于关闭2KB Ascend C API预留。根据[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制)，Data Cache是SIMT专有空间，用于缓存SIMT线程访问GM的数据，空间范围为32KB～128KB。SIMD与SIMT混合编程场景下，需要从UB中划分Data Cache；因此，确定仅调用SIMT VF场景下的UB可用上限时，应按“至少保留32KB Data Cache”反推静态UB与动态UB可占用的最大空间。Data Cache实际大小由UB总量扣除静态内存、动态内存和预留空间后的剩余空间计算。

| 场景 | VF调用 | Data Cache约束 | 默认配置场景可用UB大小（预留8KB） | 关闭VF预留场景可用UB大小（预留2KB） | 关闭全部预留场景可用UB大小（预留0KB） |
| --- | --- | ---: | ---: | ---: | ---: |
| 1 | 不调用VF | 无 | 256KB | - | - |
| 2 | 仅调用SIMD VF | 无 | 248KB | 254KB | 256KB |
| 3 | 仅调用SIMT VF | 至少32KB | 216KB | 222KB | 224KB |

本Case由CMake根据VF调用路径和预留配置计算可用UB大小，并声明对应大小的静态UB数组`static_ub`，用它验证当前配置下这段UB是否可用。根据场景不同，`static_ub`由MainScalar、SIMD VF或SIMT VF中的一个位置写入。Kernel将数组中的两个元素写回GM，使Host侧可以确认上限配置下的写入结果正常：

```cpp
#if !defined(CASE1_USE_SIMT_VF)
__global__ __vector__ void ub_limit_kernel(__gm__ uint32_t* output)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    uint8_t mutex_id = 0;

#if defined(CASE1_NO_VF)
    // 场景1：不调用VF，由MainScalar写入static_ub。
    for (uint32_t index = 0; index < UB_WORDS; ++index) {
        static_ub[index] = FILL_VALUE_BASE + index;
    }
#else
    // 场景2：仅调用SIMD VF写入static_ub。
    asc_lock(PIPE_V, mutex_id);
    asc_vf_call<simd_fill_static_ub>(static_ub, UB_WORDS);
    asc_unlock(PIPE_V, mutex_id);
    asc_lock(PIPE_S, mutex_id);
#endif

    // 写回两个元素，使Host侧可以确认写入结果。
    output[0] = static_ub[0];
    output[1] = static_ub[UB_WORDS - 1];
#if !defined(CASE1_NO_VF)
    asc_unlock(PIPE_S, mutex_id);
#endif
}
#endif

#if defined(CASE1_USE_SIMT_VF)
__global__ __vector__ void simt_ub_limit_kernel(__gm__ uint32_t* output)
{
    __ubuf__ uint32_t static_ub[UB_WORDS];
    uint8_t mutex_id = 0;

    // 场景3：仅调用SIMT VF写入static_ub。
    asc_lock(PIPE_V, mutex_id);
    asc_vf_call<simt_fill_static_ub>(dim3(THREAD_COUNT), static_ub, UB_WORDS, FILL_VALUE_BASE);
    asc_unlock(PIPE_V, mutex_id);

    asc_lock(PIPE_S, mutex_id);
    output[0] = static_ub[0];
    output[1] = static_ub[UB_WORDS - 1];
    asc_unlock(PIPE_S, mutex_id);
}
#endif
```

**执行结果**：

本Case先按上表中的可用UB上限申请`static_ub`，再将`static_ub`多申请一个`uint32_t`元素（4B）。前者Kernel启动和运行均成功；后者仍可完成编译，但Kernel启动失败，属于运行阶段错误。

| 场景 | VF调用 | 上限配置结果 | 上限基础上多申请4B后的结果 |
| --- | --- | --- | --- |
| 1 | 不调用VF | 运行通过 | Kernel启动失败 |
| 2 | 仅调用SIMD VF | 各配置均运行通过 | 各配置均在Kernel启动时失败 |
| 3 | 仅调用SIMT VF | 各配置均运行通过 | 各配置均在Kernel启动时失败 |

存在VF调用时，默认配置下6KB VF预留和2KB Ascend C API预留均不计入可用UB。关闭对应预留后，Kernel不应依赖被释放的预留空间；依赖2KB预留空间的[部分Ascend C API](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/静态Tensor编程.md#section_reserved_ubuf_api)不可使用，调用时会产生编译错误。两个选项的具体限制请参考[3510架构规格](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description)。

**结论**：

VF调用和预留空间会影响UB可用上限；SIMD与SIMT混合编程场景下，还需要为Data Cache保留足够空间。该上限约束Kernel中静态UB与动态UB的占用量之和，与本Case选用哪一种UB进行边界验证无关。在实际Kernel中确定UB可用上限时，需要统计Kernel中的全部UB声明，确认是否存在VF调用，以及是否使用依赖2KB预留空间的Ascend C API。

### Case 2：不同执行位置的静态UB和动态UB布局

**样例目标**：

说明MainScalar、SIMD VF和SIMT VF中声明的静态UB共同占用当前Kernel的静态内存区域，统计UB用量时需要按实际布局合计；同时说明三处名称不同的动态UB声明具有相同首地址，共用Kernel启动时配置的动态UB空间。

**核心实现**：

Case 2分别在MainScalar、SIMD VF和SIMT VF中声明静态UB。根据[SIMD与SIMT混合编程内存层级](../../../../docs/zh/guide/编程指南/高级编程/高级AI-Core编程模型/SIMD与SIMT混合编程/内存层级.md#ub划分)，混合编程模式下可在这三个执行空间申请静态内存；多次申请的静态内存在UB上按首地址对齐规则排布，动态内存位于静态内存之后。MainScalar声明1KB的`main_static_ub`，SIMD VF声明2KB静态UB，SIMT VF声明4KB静态UB；本Case中三处声明大小均满足32B对齐，因此静态内存区域占用为：

```text
静态内存区域占用 = 1KB + 2KB + 4KB = 7168B
```

MainScalar、SIMD VF和SIMT VF分别访问各自声明的静态UB数组，使静态UB声明参与当前Kernel布局。以下代码展示MainScalar、SIMD VF和SIMT VF中的静态UB和动态UB声明：

```cpp
// MainScalar
extern __ubuf__ uint32_t main_dynamic_ub[];
__ubuf__ uint32_t main_static_ub[MAIN_STATIC_WORDS];
// SIMD VF
extern __ubuf__ uint32_t simd_dynamic_ub[];
__ubuf__ uint32_t simd_static_ub[SIMD_STATIC_WORDS];
// SIMT VF
extern __ubuf__ uint32_t simt_dynamic_ub[];
__ubuf__ uint32_t simt_static_ub[SIMT_STATIC_WORDS];
```

MainScalar、SIMD VF和SIMT VF分别将各自静态UB数组和动态UB数组的首地址转换为整数并写回GM，由Host侧打印并比较。打印的地址属于AICore UB地址空间，仅用于本Case中的布局比较。

**执行结果**：

本Case中MainScalar、SIMD VF和SIMT VF的静态UB首地址预期分别为`0x0`、`0x400`和`0xc00`；静态内存区域总占用为7168B，换算为十六进制是`0x1c00`。动态UB位于静态内存之后，因此其预期首地址为`0x1c00`。运行结果如下：

```text
[Case2] static_total_bytes=7168
[Case2] main_static_ub=0x0, simd_static_ub=0x400, simt_static_ub=0xc00
[Case2] main_dynamic_ub=0x1c00, simd_dynamic_ub=0x1c00, simt_dynamic_ub=0x1c00
[Case2] result=PASSED
```

MainScalar、SIMD VF和SIMT VF中的静态UB首地址按声明大小依次累加；动态UB首地址均为`0x1c00`，与本Case静态内存区域占用对应的地址一致。

**结论**：

MainScalar、SIMD VF和SIMT VF中声明的静态UB共同占用当前Kernel的静态内存区域，统计UB用量时需要按实际布局和对齐规则合计。动态UB位于静态内存之后；MainScalar、SIMD VF和SIMT VF声明的动态UB名称不同，但首地址相同，共用Host侧在Kernel启动时配置的动态UB空间。

### Case 3：静态UB数组越界写入不同区域

**样例目标**：

从一块静态UB出发，分别访问静态数组合法范围、未被占用的普通UB空间、预留空间和Data Cache，判断访问是否越界，以及是否产生运行错误或精度错误。

**核心实现**：

本Case复用Case 0中的数据校准计算，输出公式为`input + (input & 0xff)`。Kernel先执行一次计算，使GM输入进入Data Cache；随后按Host侧传入的运行时下标写入目标UB区域；最后再次执行同一个计算。Host侧通过Kernel启动/同步结果和第二次计算输出，判断本次访问是否产生运行错误或精度错误。

Case 3未添加关闭预留空间的编译选项，声明4KB静态UB数组`static_ub`，不配置动态UB。在该配置下，Data Cache达到128KB上限，并位于UB高地址端。根据[3510架构规格](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description)和[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md)，Data Cache之前还包含8KB预留空间。Case 3的UB布局计算如下：

```text
Data Cache大小 = min(256KB - 4KB - 0KB - 8KB, 128KB) = 128KB
Data Cache起点 = 256KB - 128KB = 128KB
预留空间起点 = 128KB - 8KB = 120KB
未被占用的普通UB空间大小 = 120KB - 4KB = 116KB
```

`static_ub`的合法访问范围为前4KB。样例为五个目标区域分别设置访问偏移，Host侧将字节偏移换算为下标并传入Kernel：

| 目标区域 | 相对静态数组首地址的访问偏移 |
| --- | ---: |
| 静态UB合法范围的最后一个`uint32_t`元素 | 4092B |
| 未被占用的普通UB空间中部 | 63488B |
| 6KB VF预留范围中部 | 125952B |
| 2KB Ascend C API预留范围中部 | 130048B |
| Data Cache起始位置 | 131072B |

Kernel按运行时下标写入选定目标区域，关键代码如下：

```cpp
__ubuf__ uint32_t static_ub[1024];
__ubuf__ volatile uint32_t* static_ub_ptr = static_ub;
static_ub_ptr[access_index] = 0xDEADBEEF;
```

**执行结果**：

五种访问配置的执行结果如下：

| 目标区域 | 启动/同步结果 | 精度结果 | 结果说明 |
| --- | --- | --- | --- |
| 静态UB合法范围 | 成功 | 通过 | 合法访问 |
| 未被占用的普通UB空间 | 成功 | 通过 | 已越界，但本次运行无可见异常 |
| 6KB VF预留范围 | 成功 | 通过 | 已越界，本次运行未出现可见异常 |
| 2KB Ascend C API预留范围 | 成功 | 通过 | 已越界，本次运行未出现可见异常 |
| Data Cache | 成功 | 失败 | 缓存数据被破坏，但未报告运行错误 |

五种访问配置的关键输出如下，便于对照不同目标区域的运行现象：

```text
# 静态UB合法范围
[Case3] region=static_ub, access_offset_bytes=4092
[Case3] precision_check=PASSED

# 未被占用的普通UB空间
[Case3] region=unallocated_ub, access_offset_bytes=63488
[Case3] precision_check=PASSED

# 6KB VF预留范围
[Case3] region=vf_reserved, access_offset_bytes=125952
[Case3] precision_check=PASSED

# 2KB Ascend C API预留范围
[Case3] region=asc_reserved, access_offset_bytes=130048
[Case3] precision_check=PASSED

# Data Cache
[Case3] region=data_cache, access_offset_bytes=131072
[Case3] corrupted_output: expected=0xc0030000, actual=0xdeadbfde
[Case3] precision_check=FAILED
```

**分析**：

精度校验通过只能说明本次运行没有观察到精度异常，不能说明未被占用的普通UB空间或预留空间可以访问。[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制)指出，访问超出已申请的静态UB或动态UB范围后，不一定立即表现为内存分配失败，但可能破坏Data Cache并导致计算结果不稳定或精度异常。因此，表中结果只用于展示本次运行现象，不能作为越界访问合法或目标区域未被使用的依据。

**结论**：

Case 3中只有静态UB合法范围场景不越界，其余四个场景均会发生静态数组越界。越界访问未必在当前运行中触发可见异常，但超出静态数组声明范围的访问仍然属于错误。

## 编译运行

在本样例根目录下执行如下步骤，编译并运行样例。

- 配置环境变量。

    请根据当前环境中CANN开发套件包的[安装方式](../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

  > **说明**：`${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 编译并运行样例。

    ```bash
    SCENARIO_NUM=1                              # 选择Case，可选0、1、2、3
    mkdir -p build && cd build                  # 创建并进入build目录
    cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
      -DSCENARIO_NUM=${SCENARIO_NUM} ..         # 配置工程，可按需追加Case专用选项
    make -j                                     # 编译工程
    ./ub_usage_limit                            # 运行样例
    ```

**编译选项说明**：

不同场景请使用独立的build目录，避免上一次构建的CMake选项影响当前场景。本文中的默认配置指不添加关闭预留空间的编译选项。样例已按Case拆分为不同Ascend C文件；除`SCENARIO_NUM`外，其余选项仅用于在单个Case内切换对照场景。Case 1通过`CASE1_VF_MODE`和`CASE1_RESERVED_MODE`选择验证场景，CMake会自动计算对应UB大小。

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
| `SCENARIO_NUM` | `0`~`3` | Case编号，默认值为`1`。CMake根据该选项只编译对应Case的Ascend C文件 |
| `CASE0_ENABLE_FIX` | `ON`、`OFF` | Case 0是否检查静态数组运行时下标，默认值为`OFF` |
| `CASE1_VF_MODE` | `none`、`simd`、`simt` | Case 1的VF调用路径，默认值为`simd` |
| `CASE1_RESERVED_MODE` | `default`、`disable_vf`、`disable_all` | Case 1的预留配置，默认值为`default`。`disable_vf`添加编译选项`--cce-disable-vf-stack-reserved-ubuf`，`disable_all`同时添加`--cce-disable-vf-stack-reserved-ubuf`和`--cce-disable-asc-reserved-ubuf` |
| `CASE1_OVER_LIMIT` | `ON`、`OFF` | Case 1是否在可用UB上限基础上多申请4B，默认值为`OFF` |
| `CASE3_ACCESS_REGION` | `0`~`4` | Case 3访问区域，默认值为`0`。`0`：静态UB；`1`：未被占用的普通UB空间；`2`：6KB VF预留；`3`：2KB Ascend C API预留；`4`：Data Cache |

Case 0默认运行UB越界写入Data Cache的问题场景：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0 -DCASE0_ENABLE_FIX=OFF ..
make -j
./ub_usage_limit
```

增加`CASE0_ENABLE_FIX=ON`可启用下标检查，编译并运行修复场景：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=0 -DCASE0_ENABLE_FIX=ON ..
make -j
./ub_usage_limit
```

Case 1各场景对应的编译选项组合如下：

| VF调用 | 预留配置 | 追加的CMake选项 |
| --- | --- | --- |
| 不调用VF | 默认配置 | `-DCASE1_VF_MODE=none` |
| 仅调用SIMD VF | 默认配置 | `-DCASE1_VF_MODE=simd -DCASE1_RESERVED_MODE=default` |
| 仅调用SIMD VF | 关闭VF预留 | `-DCASE1_VF_MODE=simd -DCASE1_RESERVED_MODE=disable_vf` |
| 仅调用SIMD VF | 关闭全部预留 | `-DCASE1_VF_MODE=simd -DCASE1_RESERVED_MODE=disable_all` |
| 仅调用SIMT VF | 默认配置 | `-DCASE1_VF_MODE=simt -DCASE1_RESERVED_MODE=default` |
| 仅调用SIMT VF | 关闭VF预留 | `-DCASE1_VF_MODE=simt -DCASE1_RESERVED_MODE=disable_vf` |
| 仅调用SIMT VF | 关闭全部预留 | `-DCASE1_VF_MODE=simt -DCASE1_RESERVED_MODE=disable_all` |

下面以“仅调用SIMT VF，并关闭全部预留”为例，完整编译运行命令如下：

```bash
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DSCENARIO_NUM=1 \
  -DCASE1_VF_MODE=simt \
  -DCASE1_RESERVED_MODE=disable_all ..
make -j
./ub_usage_limit
```

**执行结果示例**：

以下展示各Case的关键输出。Case 1和Case 3包含多个场景，代码块中用注释标明对应场景。

- Case 0包含问题场景和修复场景，关键输出如下：

    ```text
    # 问题场景：CASE0_ENABLE_FIX=OFF，越界写入Data Cache
    [Case0] mode=baseline, access_byte_offset=229376
    [Case0] corrupted_output: expected=0xc0000000, actual=0xdeadbeef
    [Case0] precision_check=FAILED (fault reproduced)

    # 修复场景：CASE0_ENABLE_FIX=ON，增加运行时下标检查
    [Case0] mode=fixed, access_byte_offset=229376
    [Case0] precision_check=PASSED (out-of-bounds write blocked)
    ```

- Case 1按可用UB上限申请`static_ub`，各场景关键输出如下：

    ```text
    # 场景1：不调用VF，可用UB上限为262144B
    [Case1] mode=without SIMD VF, ub_limit_bytes=262144
    [Case1] result=PASSED

    # 场景2：仅调用SIMD VF，默认配置，可用UB上限为253952B
    [Case1] mode=with SIMD VF, ub_limit_bytes=253952
    [Case1] result=PASSED

    # 场景2：仅调用SIMD VF，关闭VF预留，可用UB上限为260096B
    [Case1] mode=with SIMD VF, ub_limit_bytes=260096
    [Case1] result=PASSED

    # 场景2：仅调用SIMD VF，关闭全部预留，可用UB上限为262144B
    [Case1] mode=with SIMD VF, ub_limit_bytes=262144
    [Case1] result=PASSED

    # 场景3：仅调用SIMT VF，默认配置，可用UB上限为221184B
    [Case1] mode=with SIMT VF, ub_limit_bytes=221184
    [Case1] result=PASSED

    # 场景3：仅调用SIMT VF，关闭VF预留，可用UB上限为227328B
    [Case1] mode=with SIMT VF, ub_limit_bytes=227328
    [Case1] result=PASSED

    # 场景3：仅调用SIMT VF，关闭全部预留，可用UB上限为229376B
    [Case1] mode=with SIMT VF, ub_limit_bytes=229376
    [Case1] result=PASSED
    ```

    追加`-DCASE1_OVER_LIMIT=ON`后，CMake会将`static_ub`在可用上限基础上多申请4B。默认配置下，Kernel启动失败：

    ```text
    # 仅调用SIMD VF：253952B + 4B
    [Case1] mode=with SIMD VF, ub_limit_bytes=253956
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    [Case1] launch_ret=107000, sync_ret=107000
    [Case1] result=FAILED

    # 仅调用SIMT VF：221184B + 4B
    [Case1] mode=with SIMT VF, ub_limit_bytes=221184, configured_ub_bytes=221188
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    [Case1] launch_ret=107000, sync_ret=107000
    [Case1] result=FAILED
    ```

- Case 2关键输出如下：

    ```text
    [Case2] static_total_bytes=7168
    [Case2] main_static_ub=0x0, simd_static_ub=0x400, simt_static_ub=0xc00
    [Case2] main_dynamic_ub=0x1c00, simd_dynamic_ub=0x1c00, simt_dynamic_ub=0x1c00
    [Case2] result=PASSED
    ```

- Case 3访问不同UB区域，关键输出如下：

    ```text
    # 静态UB合法范围
    [Case3] region=static_ub, access_offset_bytes=4092
    [Case3] precision_check=PASSED

    # 未申请的普通UB范围
    [Case3] region=unallocated_ub, access_offset_bytes=63488
    [Case3] precision_check=PASSED

    # 6KB VF预留范围
    [Case3] region=vf_reserved, access_offset_bytes=125952
    [Case3] precision_check=PASSED

    # 2KB Ascend C API预留范围
    [Case3] region=asc_reserved, access_offset_bytes=130048
    [Case3] precision_check=PASSED

    # Data Cache
    [Case3] region=data_cache, access_offset_bytes=131072
    [Case3] corrupted_output: expected=0xc0030000, actual=0xdeadbfde
    [Case3] precision_check=FAILED
    ```
