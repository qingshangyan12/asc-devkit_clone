# SIMD与SIMT混合编程UB越界定位样例

## 概述

本样例以SIMD与SIMT混合编程场景下UB访问越界导致输出精度异常为例，介绍如何结合UB容量、内存布局和实际访问范围定位并修复UB访问越界。样例随后通过多个Case说明不同VF配置下的UB可用上限、静态UB和动态UB的布局关系，以及越界访问不同UB区域时的运行和精度结果。

本样例主要内容如下表所示。

| 内容类别 | 对应Case | 主要内容 |
| --- | --- | --- |
| 问题定位 | Case 0 | 从输出精度异常出发，定位复杂工作区偏移导致的UB越界写入，并给出修复方式 |
| UB容量边界 | Case 1 | 说明不同VF调用、预留空间和Data Cache配置下的UB可用上限 |
| UB布局关系 | Case 2 | 说明MainScalar、SIMD VF和SIMT VF中声明的静态UB如何共同占用静态内存区域，以及动态UB共用方式 |
| 越界访问结果 | Case 3 | 说明越过已申请内存范围写入后续UB区域时的运行和精度结果 |

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── simd_simt_ub_usage_limit
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── ub_usage_limit_common.h// 公共声明和工具函数
│   ├── case0_ub_oob.asc       // Case 0样例实现
│   ├── case1_ub_limit.asc     // Case 1样例实现
│   ├── case2_ub_layout.asc    // Case 2样例实现
│   ├── case3_oob_result.asc   // Case 3样例实现
│   ├── figures                // README引用的示意图
│   ├── README.md              // 中文说明
│   └── README_en.md           // 英文说明
```

## 样例描述

本样例包含4个Case，围绕UB访问越界定位、容量边界和内存布局展开。

**Case说明**：

| Case | 场景 | 主要内容 |
| --- | --- | --- |
| Case 0 | 复杂工作区偏移导致静态UB访问越界 | 展现精度问题，并根据UB布局和访问偏移定位、修复越界写 |
| Case 1 | 不同VF场景下的UB可用上限 | 确定不同VF调用、预留空间和Data Cache配置下的UB可用上限 |
| Case 2 | 不同执行位置的静态UB和动态UB布局 | 说明MainScalar、SIMD VF和SIMT VF中的静态UB和动态UB布局关系 |
| Case 3 | 静态UB数组越界写入不同区域 | 说明越过已申请内存范围写入后续UB区域时的运行及精度结果 |

**样例规格**：

<table>
<tr><td align="center">样例类型（OpType）</td><td colspan="4" align="center">SimdSimtUbUsageLimit</td></tr>
<tr><td rowspan="2" align="center">样例输入</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0、Case 3</td><td align="center">input</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td rowspan="2" align="center">样例输出</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0、Case 3</td><td align="center">output</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">核函数名</td><td colspan="4" align="center">case0_static_ub_oob_kernel/case3_static_ub_oob_kernel</td></tr>
</table>

Case 1和Case 2用于验证UB容量和布局，不涉及实际算子输入输出，因此不在样例规格表中单独列出。

## 样例实现

本节按照“问题展现与修复、UB可用上限确认、内存布局说明、越界结果分析”的顺序介绍各Case。

### Case 0：复杂工作区偏移导致静态UB访问越界

**样例目标**：

展现实际算子中由于工作区偏移计算复杂，Kernel未报告运行错误但输出精度校验失败的问题，并通过检查UB容量、内存布局和实际访问范围定位原因，验证修复方法。

**核心实现**：

Case 0实现一个简化的分块数据校准流程：Kernel将静态UB工作区分成4块循环复用，每个Tile使用其中一块保存`sample`、`bias`和校准中间结果；SIMT VF先把GM中的样本数据搬入当前Tile对应的工作区分块，并计算对应的`bias`；MainScalar随后根据`tile_id`和`lane_id`计算`bias`区域的写入偏移；最后SIMT VF继续读取GM样本和工作区中的`bias`，生成校准输出。

```text
calibrated_output[i] = sample[i] + bias[i]
```

Kernel申请216KB静态UB作为工作区。不同阶段通过固定基址、`tile_id`和`lane_id`计算各自访问的工作区偏移。问题版本在计算工作区分块大小时，误把8KB预留空间计入可用工作区，导致MainScalar写入`bias`区域时访问越过静态UB声明范围。

关键代码如下：

```cpp
__ubuf__ uint32_t workspace[216 * 1024 / sizeof(uint32_t)];

asc_vf_call<simt_case0_prepare_calibration_workspace>(dim3(256), sample, workspace, tile_id);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
asc_vf_call<simt_case0_apply_bias>(dim3(128), workspace, tile_id);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);

uint32_t block_id = tile_id & 3;
uint32_t bias_word_offset = block_id * (56 * 1024 / sizeof(uint32_t)) +
                            (56 * 1024 / sizeof(uint32_t)) + lane_id;
workspace[bias_word_offset] = CORRUPTED_VALUE;

asc_sync_notify(PIPE_S, PIPE_V, EVENT_ID0);
asc_sync_wait(PIPE_S, PIPE_V, EVENT_ID0);
asc_vf_call<simt_case0_store_calibrated_output>(dim3(THREAD_COUNT), sample, workspace, output,
                                                BUFFER_WORDS, tile_id);
```

**执行结果**：

复杂偏移计算得到的写入位置越过静态UB并落入Data Cache后，Kernel未报告运行错误，但输出精度校验失败。

关键输出：

```text
[Case0] mode=fault, faulty_byte_offset=229376
[Case0] first_mismatch_index=0, expected=0xc0000000, actual=0xdeadbeef
[Case0] precision_check=FAILED (fault reproduced)
```

**问题定位**：

1. 先确认现象。问题版本没有出现Kernel启动失败或同步失败，异常集中在输出数据不符合预期，因此需要继续回查Kernel内部的UB访问范围。
2. 统计UB占用。Case 0使用默认编译配置，申请216KB静态UB，不配置动态UB。其余空间包括8KB预留空间和32KB Data Cache。Data Cache是SIMT专有空间，用于缓存SIMT线程访问GM的数据，空间范围为32KB～128KB。

    ```text
    低地址 0
    ├── 静态UB       216KB
    ├── 预留空间       8KB
    └── Data Cache    32KB
    高地址 256KB
    ```

3. 计算Data Cache位置。Data Cache大小为`min(256KB - 216KB - 0KB - 8KB, 128KB) = 32KB`，因此起始地址为`256KB - 32KB = 224KB`。
4. 回查工作区偏移计算。代码将`216KB静态UB + 8KB预留空间`误作为可用工作区，并按4个工作区分块切分，每个分块大小为`(216KB + 8KB) / 4 = 56KB`。当`tile_id = 3`、`lane_id = 0`时，目标字节偏移为：

    ```text
    block_id = tile_id & 3 = 3
    bias_word_offset = block_id * 56KB / 4B + 56KB / 4B + lane_id
                     = 57344
    byte_offset = bias_word_offset * 4B = 229376B = 224KB
    ```

    224KB字节偏移已经超出216KB静态工作区声明范围，并正好位于Data Cache起始位置。
5. 核对异常数据。前置SIMT VF阶段读取GM样本后，数据进入Data Cache；越界写随后破坏缓存数据。后续SIMT VF继续读取相同样本并结合工作区中的`bias`生成校准输出，错误输出为`0xDEADBEEF`，与越界写入值一致。结合错误偏移落点和异常值，可将问题定位为越界写入Data Cache导致的精度异常。

**解决方案**：

- 在写入工作区前检查计算出的实际访问范围。仅当`bias_word_offset`小于静态工作区元素个数`CASE0_WORKSPACE_WORDS`时执行写入，避免访问超出`workspace`的声明范围。

    ```cpp
    if (bias_word_offset < CASE0_WORKSPACE_WORDS) {
        workspace[bias_word_offset] = CORRUPTED_VALUE;
    }
    ```

- 增加下标检查后，越界写入被阻止，输出精度校验通过：

    ```text
    [Case0] mode=fixed, faulty_byte_offset=229376
    [Case0] precision_check=PASSED (out-of-bounds write blocked)
    ```

**结论**：

修复模式下输出精度校验通过。UB越界不一定触发Kernel启动或运行错误，当错误访问落入Data Cache时，可能破坏SIMT访问GM时缓存的数据，并最终表现为精度异常。因此，定位和修复UB访问越界问题时，不仅要检查UB申请总量，还要检查由工作区分块计算出的实际访问范围。

Case 0说明了复杂工作区偏移导致的UB访问越界可能表现为精度异常。进一步定位UB访问越界问题，还需要明确混合编程场景下UB的容量和布局：MainScalar、SIMD VF和SIMT VF均可能声明静态UB或动态UB，不同位置声明的静态UB共同占用静态内存区域，动态UB共用Kernel启动时配置的空间；是否调用VF还会影响预留空间和UB可用上限。下面通过Case 1～3分别说明不同VF配置下的UB可用上限、静态UB与动态UB的布局关系，以及越界写入不同区域时的结果。

### Case 1：不同VF场景下的UB可用上限

**样例目标**：

确定不同VF调用和预留配置下的UB可用上限。

**核心实现**：

本Case包含不调用VF、调用SIMD VF、调用SIMT VF三个场景。存在VF调用时，分别验证默认配置、关闭VF预留和关闭全部预留时的UB可用上限。

单个Vector Core共有256KB UB。存在VF调用时，默认配置下有8KB预留空间（6KB VF预留和2KB Ascend C API预留）；不调用VF时不按该预留计算。根据[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制)，Data Cache是SIMT专有空间，用于缓存SIMT线程访问GM的数据，空间范围为32KB～128KB。SIMD与SIMT混合编程场景下，需要从UB中划分Data Cache；因此，确定SIMT VF场景下的UB可用上限时，应按“至少保留32KB Data Cache”反推静态UB与动态UB可占用的最大空间。Data Cache实际大小由UB总量扣除静态内存、动态内存和预留空间后的剩余空间计算。

| 场景 | VF调用 | Data Cache约束 | 默认配置（预留8KB） | 关闭VF预留（预留2KB） | 关闭全部预留（预留0KB） |
| --- | --- | ---: | ---: | ---: | ---: |
| 1 | 不调用VF | 无 | 256KB | - | - |
| 2 | SIMD VF | 无 | 248KB | 254KB | 256KB |
| 3 | SIMT VF | 至少32KB | 216KB | 222KB | 224KB |

本Case声明大小为`UB_LIMIT_BYTES`的静态UB数组`ub_buffer`，用它验证当前配置下这段UB是否可用。根据场景不同，`ub_buffer`由MainScalar、SIMD VF或SIMT VF写入。Kernel将数组中的两个元素写回GM，使Host侧可以确认上限配置下的写入结果正常：

```cpp
__ubuf__ uint32_t ub_buffer[UB_LIMIT_WORDS];

#if defined(UB_LIMIT_WITH_SIMT_VF)
// 场景3：调用SIMT VF写入ub_buffer。
asc_vf_call<simt_fill_ub_limit>(dim3(THREAD_COUNT), ub_buffer, UB_LIMIT_WORDS, CASE1_MARKER);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
#elif defined(UB_LIMIT_NO_SIMD_VF)
// 场景1：不调用VF，由MainScalar写入ub_buffer。
for (uint32_t index = 0; index < UB_LIMIT_WORDS; ++index) {
    ub_buffer[index] = CASE1_MARKER + index;
}
#else
// 场景2：调用SIMD VF写入ub_buffer。
asc_vf_call<simd_fill_ub_limit>(ub_buffer, UB_LIMIT_WORDS);
asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
#endif

// 写回两个元素，使Host侧可以确认写入结果。
output[0] = ub_buffer[0];
output[1] = ub_buffer[UB_LIMIT_WORDS - 1];
```

**执行结果**：

本Case先按上表中的可用UB上限申请`ub_buffer`，再将`ub_buffer`多申请一个`uint32_t`元素（4B）。前者Kernel启动和运行均成功；后者仍可完成编译，但Kernel启动失败，属于运行阶段错误。

| 场景 | VF调用 | 上限配置结果 | 上限基础上多申请4B后的结果 |
| --- | --- | --- | --- |
| 1 | 不调用VF | 运行通过 | Kernel启动失败 |
| 2 | SIMD VF | 各配置均运行通过 | 各配置均在Kernel启动时失败 |
| 3 | SIMT VF | 各配置均运行通过 | 各配置均在Kernel启动时失败 |

存在VF调用时，默认配置下6KB VF预留和2KB Ascend C API预留均不计入可用UB。`--cce-disable-vf-stack-reserved-ubuf`释放6KB VF预留；开启后，Kernel不应依赖该预留空间。`--cce-disable-asc-reserved-ubuf`释放另外2KB Ascend C API预留；开启后，依赖该预留空间的[部分Ascend C API](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMD编程/基于Tensor的CPP编程/静态Tensor编程.md#section_reserved_ubuf_api)不可使用，调用时会产生编译错误。两个选项的具体限制请参考[3510架构规格](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description)。

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
__ubuf__ uint32_t main_static_ub[CASE2_MAIN_STATIC_WORDS];
// SIMD VF
extern __ubuf__ uint32_t simd_dynamic_ub[];
__ubuf__ uint32_t simd_static_ub[CASE2_SIMD_STATIC_WORDS];
// SIMT VF
extern __ubuf__ uint32_t simt_dynamic_ub[];
__ubuf__ uint32_t simt_static_ub[CASE2_SIMT_STATIC_WORDS];
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

从一块静态UB出发，分别访问静态数组合法范围、未申请UB、预留区和Data Cache，判断访问是否越界，并观察是否出现运行错误或精度错误。

**核心实现**：

Case 3使用默认编译配置，仅申请静态UB，不配置动态UB。在该配置下，Data Cache达到128KB上限，并位于UB高地址端。根据[3510架构规格](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description)和[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md)，Data Cache之前还包含8KB预留空间。Case 3的UB布局计算如下：

```text
Data Cache大小 = min(256KB - 4KB - 0KB - 8KB, 128KB) = 128KB
Data Cache起点 = 256KB - 128KB = 128KB
预留空间起点 = 128KB - 8KB = 120KB
未申请UB大小 = 120KB - 4KB = 116KB
```

![Case 3 UB地址布局与访问位置](./figures/case3_ub_layout.png "Case 3 UB地址布局与访问位置")

`static_ub`的合法访问范围为前4KB。样例为五个目标区域分别设置访问偏移，Host侧将字节偏移换算为下标并传入Kernel。Kernel通过`volatile`指针和运行时下标写入选定的目标区域：

| 目标区域 | 相对静态数组首地址的访问偏移 |
| --- | ---: |
| 静态UB合法范围的最后一个`uint32_t`元素 | 4092B |
| 未申请的普通UB范围中部 | 63488B |
| 6KB VF预留范围中部 | 125952B |
| 2KB Ascend C API预留范围中部 | 130048B |
| Data Cache起始位置 | 131072B |

Kernel首先调用SIMT VF读取GM输入并写出逐元素加一结果，使GM输入进入Data Cache。随后，MainScalar以4KB静态数组的首地址为基址，通过运行时下标写入目标区域；最后再次调用同一个SIMT VF读取GM并执行计算。Host侧检查Kernel启动和同步结果，并将第二次计算结果与Golden数据比较，判断输出精度是否受到影响：

```cpp
__ubuf__ uint32_t static_ub[1024];
__ubuf__ volatile uint32_t* static_data = static_ub;
static_data[access_index] = 0xDEADBEEF;
```

**执行结果**：

五种访问配置的执行结果如下：

| 目标区域 | 启动/同步结果 | 精度结果 | 结果说明 |
| --- | --- | --- | --- |
| 静态UB合法范围 | 成功 | 通过 | 合法访问 |
| 未申请的普通UB范围 | 成功 | 通过 | 已越界，但本次运行无可见异常 |
| 6KB VF预留范围 | 成功 | 通过 | 已越界，本次运行未出现可见异常 |
| 2KB Ascend C API预留范围 | 成功 | 通过 | 已越界，本次运行未出现可见异常 |
| Data Cache | 成功 | 失败 | 缓存数据被破坏，但未报告运行错误 |

五种访问配置的关键输出如下，便于对照不同目标区域的运行现象：

```text
# 静态UB合法范围
[Case3] region=static_ub, access_offset_bytes=4092
[Case3] precision_check=PASSED
[Case3] runtime_check=PASSED

# 未申请的普通UB范围
[Case3] region=unallocated_ub, access_offset_bytes=63488
[Case3] precision_check=PASSED
[Case3] runtime_check=PASSED

# 6KB VF预留范围
[Case3] region=vf_reserved, access_offset_bytes=125952
[Case3] precision_check=PASSED
[Case3] runtime_check=PASSED

# 2KB Ascend C API预留范围
[Case3] region=asc_reserved, access_offset_bytes=130048
[Case3] precision_check=PASSED
[Case3] runtime_check=PASSED

# Data Cache
[Case3] region=data_cache, access_offset_bytes=131072
[Case3] first_mismatch_index=896, expected=0xc0030381, actual=0xdeadbef0
[Case3] precision_check=FAILED
[Case3] runtime_check=PASSED
```

**分析**：

精度校验通过不代表未申请UB或预留区可以访问。[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制)指出，访问超出已申请的静态UB或动态UB范围后，不一定立即表现为内存分配失败，但可能破坏Data Cache并导致计算结果不稳定或精度异常。因此，表中结果只用于展示本次运行现象，不能作为越界访问合法或目标区域未被使用的依据。

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

不同场景请使用独立的build目录，避免上一次构建的CMake选项影响当前场景。

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
| `SCENARIO_NUM` | `0`~`3` | Case编号，默认值为`1`。CMake根据该选项只编译对应Case的Ascend C文件 |
| `CASE0_ENABLE_FIX` | `ON`、`OFF` | Case 0是否检查静态数组运行时下标，默认值为`OFF` |
| `UB_LIMIT_BYTES` | 4B的正整数倍 | Case 1的静态UB数组大小，未配置时调用SIMD VF取`253952`，调用SIMT VF取`221184`，单位为B |
| `UB_LIMIT_WITH_SIMT_VF` | `ON`、`OFF` | Case 1是否包含SIMT VF，默认值为`OFF` |
| `UB_LIMIT_NO_SIMD_VF` | `ON`、`OFF` | 不包含SIMT VF时是否不调用SIMD VF，默认值为`OFF` |
| `UB_LIMIT_DISABLE_VF_RESERVED` | `ON`、`OFF` | 是否传递`--cce-disable-vf-stack-reserved-ubuf`，默认值为`OFF` |
| `UB_LIMIT_DISABLE_ASC_RESERVED` | `ON`、`OFF` | 是否传递`--cce-disable-asc-reserved-ubuf`，默认值为`OFF` |
| `CASE3_ACCESS_REGION` | `0`~`4` | Case 3访问区域，默认值为`0`。`0`：静态UB；`1`：未申请UB；`2`：6KB VF预留；`3`：2KB Ascend C API预留；`4`：Data Cache |

Case 1的基础场景和编译选项组合如下：

| 场景 | 追加的CMake选项 |
| --- | --- |
| 场景1：不调用VF | `-DUB_LIMIT_NO_SIMD_VF=ON -DUB_LIMIT_BYTES=262144` |
| 场景2：SIMD VF | `-DUB_LIMIT_BYTES=253952` |
| 场景3：SIMT VF | `-DUB_LIMIT_WITH_SIMT_VF=ON -DUB_LIMIT_BYTES=221184` |
| SIMD VF，关闭VF预留 | `-DUB_LIMIT_DISABLE_VF_RESERVED=ON -DUB_LIMIT_BYTES=260096` |
| SIMD VF，关闭全部预留 | `-DUB_LIMIT_DISABLE_VF_RESERVED=ON -DUB_LIMIT_DISABLE_ASC_RESERVED=ON -DUB_LIMIT_BYTES=262144` |
| SIMT VF，关闭VF预留 | `-DUB_LIMIT_WITH_SIMT_VF=ON -DUB_LIMIT_DISABLE_VF_RESERVED=ON -DUB_LIMIT_BYTES=227328` |
| SIMT VF，关闭全部预留 | `-DUB_LIMIT_WITH_SIMT_VF=ON -DUB_LIMIT_DISABLE_VF_RESERVED=ON -DUB_LIMIT_DISABLE_ASC_RESERVED=ON -DUB_LIMIT_BYTES=229376` |

Case 0默认运行静态UB越界写入Data Cache的问题场景：

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

**执行结果示例**：

- Case 0使用默认的`CASE0_ENABLE_FIX=OFF`，关键输出如下：

    ```text
    [Case0] precision_check=FAILED (fault reproduced)
    # 精度问题已出现
    ```

- Case 1默认调用SIMD VF，`ub_buffer`大小为253952B，关键输出如下：

    ```text
    [Case1] mode=with SIMD VF, ub_limit_bytes=253952
    [Case1] result=PASSED
    ```

- Case 1调用SIMT VF并使用221184B静态UB时，关键输出如下：

    ```text
    [Case1] mode=with SIMT VF, ub_limit_bytes=221184
    [Case1] result=PASSED
    ```

- Case 2关键输出如下：

    ```text
    [Case2] static_total_bytes=7168
    [Case2] main_static_ub=0x0, simd_static_ub=0x400, simt_static_ub=0xc00
    [Case2] main_dynamic_ub=0x1c00, simd_dynamic_ub=0x1c00, simt_dynamic_ub=0x1c00
    [Case2] result=PASSED
    ```

- Case 3使用默认的`CASE3_ACCESS_REGION=0`，关键输出如下：

    ```text
    [Case3] region=static_ub, access_offset_bytes=4092
    [Case3] precision_check=PASSED
    [Case3] runtime_check=PASSED
    ```

**边界与异常结果示例**：

- Case 1同时关闭6KB VF预留和2KB Ascend C API预留后，`ub_buffer`为262144B，占用256KB，运行通过：

    ```text
    [Case1] mode=with SIMD VF, ub_limit_bytes=262144
    [Case1] result=PASSED
    ```

- Case 1使用默认SIMD VF配置，将`ub_buffer`从253952B增加至253956B后，UB总占用超过248KB上限4B，Kernel启动失败：

    ```text
    [Case1] mode=with SIMD VF, ub_limit_bytes=253956
    [Case1] launch_ret=107000, sync_ret=107000
    [Case1] result=FAILED
    ```

- Case 1调用SIMT VF时，将静态UB数组从221184B增加至221188B后，UB总占用超过216KB上限4B，Kernel启动失败：

    ```text
    [Case1] mode=with SIMT VF, ub_limit_bytes=221184, configured_ub_bytes=221188
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    [Case1] launch_ret=107000, sync_ret=107000
    [Case1] result=FAILED
    ```

- Case 3设置`CASE3_ACCESS_REGION=4`，越界写入Data Cache。Kernel启动和同步均成功，但输出出现精度错误：

    ```text
    [Case3] region=data_cache, access_offset_bytes=131072
    [Case3] precision_check=FAILED
    [Case3] runtime_check=PASSED
    ```
