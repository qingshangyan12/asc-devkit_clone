# SIMD与SIMT混合编程UB越界定位样例

## 概述

本样例以SIMD与SIMT混合编程场景下静态UB访问越界导致输出精度异常为例，介绍如何结合UB容量、内存布局和实际访问范围定位并修复静态UB访问越界。样例随后通过多个Case说明不同VF配置下的UB可用上限、静态UB和动态UB的布局关系，以及越界访问不同UB区域时的运行和精度结果。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── simd_simt_ub_usage_limit
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── ub_usage_limit.asc     // Ascend C样例实现
│   ├── figures                // README引用的示意图
│   ├── README.md              // 中文说明
│   └── README_en.md           // 英文说明
```

## 样例描述

本样例包含5个Case。Case 0展现、定位并修复静态UB越界写入Data Cache导致的精度问题；Case 1确定不包含SIMT VF时三种配置下的UB可用上限；Case 2确定包含SIMT VF时静态UB数组和动态UB数组各自的可用上限；Case 3说明静态UB的累加方式和动态UB的共用方式；Case 4说明越界写入不同UB区域时是否出现运行错误或精度错误。

| Case | 场景 | 主要内容 |
| --- | --- | --- |
| Case 0 | 静态UB越界写入Data Cache | 展现精度问题，并根据UB布局定位和修复越界写 |
| Case 1 | 不包含SIMT VF时的UB可用上限 | 确定不调用SIMD VF、默认调用SIMD VF和关闭VF栈预留三种配置各自的UB可用上限 |
| Case 2 | 包含SIMT VF时的UB可用上限 | 确定静态UB数组和动态UB数组各自的可用上限 |
| Case 3 | MainScalar、SIMD VF和SIMT VF混合使用UB | 说明不同执行位置的静态UB和动态UB布局关系 |
| Case 4 | 小静态UB越界写入不同UB区域 | 说明访问未申请UB、预留区和Data Cache时的运行及精度结果 |

> **UB访问越界说明**
>
> UB申请总量未超过容量上限，并不代表每次访问都合法。静态数组只能访问其声明范围，动态UB只能访问Kernel启动时配置的范围。静态数组或动态UB访问越界后，可能落入未申请UB、VF栈预留、Ascend C API预留或Data Cache。UB访问越界不一定立即触发运行错误，需要结合地址范围、运行日志和输出精度进行定位。

**样例规格**：

<table>
<tr><td align="center">样例类型（OpType）</td><td colspan="4" align="center">SimdSimtUbUsageLimit</td></tr>
<tr><td rowspan="3" align="center">样例输入</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0、Case 4</td><td align="center">input</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">Case 1～Case 3</td><td colspan="3" align="center">无GM输入，测试数据由Kernel内部生成</td></tr>
<tr><td rowspan="3" align="center">样例输出</td><td align="center">Case</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
<tr><td align="center">Case 0、Case 4</td><td align="center">output</td><td align="center">[1024]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">Case 1～Case 3</td><td align="center">result</td><td align="center">[8]</td><td align="center">uint32_t</td></tr>
<tr><td align="center">核函数名</td><td colspan="4" align="center">Case0StaticUbOobKernel/Case1StaticUbLimitKernel/SimtUbLimitKernel/MixedUbKernel/Case4StaticUbOobKernel</td></tr>
</table>

## 样例实现

本节按照“问题展现与修复、UB可用上限确认、内存布局说明、越界结果分析”的顺序介绍各Case。

### Case 0：静态UB访问越界及问题定位

**样例目标**：

展现逐元素计算中Kernel未报告运行错误但输出精度校验失败的问题，并通过检查UB容量、内存布局和实际访问范围定位原因，验证修复方法。

**核心实现**：

Case 0对GM中的输入数据逐元素加一，计算公式如下：

```text
output[i] = input[i] + 1
```

Kernel启动1024个SIMT线程执行逐元素加一计算，并申请216KB静态UB。两次计算之间，MainScalar使用运行时下标写入静态数组；Host侧将第二次计算结果与Golden数据进行比较。

关键代码如下：

```cpp
__ubuf__ uint32_t static_ub[CASE0_STATIC_WORDS];

asc_vf_call<SimtCase0AddOne>(dim3(THREAD_COUNT), input, output, BUFFER_WORDS);
WaitVectorToScalar();

__ubuf__ volatile uint32_t* static_data = static_ub;
static_data[faulty_index] = CORRUPTED_VALUE;

WaitScalarToVector();
asc_vf_call<SimtCase0AddOne>(dim3(THREAD_COUNT), input, output, BUFFER_WORDS);
```

**执行结果**：

问题模式设置`CASE0_ENABLE_FIX=OFF`，用于展现输出精度校验失败的问题。

关键输出：

```text
[Case0] mode=fault, static_bytes=221184, dynamic_bytes=0, reserved_bytes=8192, dcache_bytes=32768, faulty_byte_offset=229376
[Case0] mismatch_count=1
[Case0] first_mismatch_index=896, expected=0xc0000381, actual=0xdeadbef0
[Case0] precision_check=FAILED (fault reproduced)
```

**问题定位**：

1. 检查Kernel启动和同步结果。Host侧仅在Kernel启动和同步成功后执行精度校验。本次运行进入精度校验，且`mismatch_count=1`，说明Kernel正常结束但输出存在精度错误。
2. 统计UB占用。Case 0使用默认编译配置，申请216KB静态UB，不配置动态UB。其余空间包括8KB预留空间（默认编译配置下，Kernel包含VF调用时会预留该空间）和32KB Data Cache（包含SIMT VF时至少需要32KB，用于缓存SIMT线程访问GM的数据）：

    ```text
    低地址 0
    ├── 静态UB       216KB
    ├── 预留空间       8KB
    └── Data Cache    32KB
    高地址 256KB
    ```

3. 计算Data Cache位置。Data Cache大小为`min(256KB - 216KB - 0KB - 8KB, 128KB) = 32KB`，因此起始地址为`256KB - 32KB = 224KB`。
4. 回查UB访问代码。问题写操作以`static_ub`首地址为基址，实际字节偏移为`faulty_index * sizeof(uint32_t) = 229376B = 224KB`：

    ```cpp
    __ubuf__ uint32_t static_ub[216 * 1024 / sizeof(uint32_t)];
    uint32_t faulty_index = (216 * 1024 + 8 * 1024) / sizeof(uint32_t);
    __ubuf__ volatile uint32_t* static_data = static_ub;
    static_data[faulty_index] = 0xDEADBEEF;
    ```

    224KB字节偏移已经超出216KB静态数组的声明范围，并正好位于Data Cache起始位置。
5. 核对异常数据。第一次SIMT VF读取GM输入后，数据进入Data Cache；越界写随后破坏缓存数据。第二次SIMT VF再次读取相同输入并执行加一，错误输出为`0xDEADBEF0`，正好是越界写入值`0xDEADBEEF`加一的结果。由此可以确认精度错误由越界写入Data Cache导致。样例只校验是否存在不匹配元素，不依赖固定的错误位置。

**解决方案**：

- 在通过运行时下标写入静态数组前检查下标范围。仅当`faulty_index`小于静态数组元素个数`CASE0_STATIC_WORDS`时执行写入，避免访问超出`static_ub`的声明范围。

    ```cpp
    if (faulty_index < CASE0_STATIC_WORDS) {
        static_data[faulty_index] = CORRUPTED_VALUE;
    }
    ```

- 设置`CASE0_ENABLE_FIX=ON`启用下标检查。重新编译并运行Case 0，输出精度校验通过：

    ```text
    [Case0] mode=fixed, static_bytes=221184, dynamic_bytes=0, reserved_bytes=8192, dcache_bytes=32768, faulty_byte_offset=229376
    [Case0] mismatch_count=0
    [Case0] precision_check=PASSED (out-of-bounds write blocked)
    ```

**结论**：

修复模式下输出精度校验通过。UB越界不一定触发Kernel启动或运行错误，当错误访问落入Data Cache时，可能破坏SIMT访问GM时缓存的数据，并最终表现为精度异常。因此，定位和修复UB访问越界问题时，不仅要检查UB申请总量，还要检查每次访问使用的实际下标是否位于被访问数组的声明范围内。

Case 0说明了UB访问越界可能表现为精度异常。进一步定位UB访问越界问题，还需要明确混合编程场景下UB的容量和布局：MainScalar、SIMD VF和SIMT VF均可能声明静态UB或动态UB，不同位置的静态UB需要累加，动态UB共用Kernel启动时配置的空间；是否调用VF还会影响预留空间和UB可用上限。下面通过Case 1～4分别说明不同VF配置下的UB可用上限、静态UB与动态UB的布局关系，以及越界写入不同区域时的结果。

### Case 1：不包含SIMT VF时的UB可用上限

**样例目标**：

确定不包含SIMT VF时，不调用SIMD VF、使用默认配置调用SIMD VF，以及调用SIMD VF并关闭VF栈预留三种配置下的UB可用上限。

**核心实现**：

Case 1包含三种编译配置：不调用SIMD VF、使用默认配置调用SIMD VF，以及调用SIMD VF并关闭VF栈预留。样例逐步调整静态数组`static_ub`的大小，以“当前配置运行成功，增加4B后Kernel启动失败”确定边界。静态数组仅用于验证容量，最终结论是三种配置下的UB可用上限。

Kernel声明一段大小可配置的`static_ub`和一段固定的32B静态结果区。根据配置，由MainScalar或SIMD VF写满`static_ub`，随后读取首尾元素并返回Host侧校验：

```cpp
__ubuf__ uint32_t static_ub[BOUNDARY_STATIC_WORDS];
__ubuf__ uint32_t result[8];

#if defined(UB_BOUNDARY_NO_SIMD_VF)
for (uint32_t index = 0; index < BOUNDARY_STATIC_WORDS; ++index) {
    static_ub[index] = BOUNDARY_MARKER + index;
}
#else
asc_vf_call<SimdFillBoundaryUb>(static_ub, BOUNDARY_STATIC_WORDS);
WaitVectorToScalar();
#endif

result[2] = static_ub[0];
result[3] = static_ub[BOUNDARY_STATIC_WORDS - 1];
```

`UB_BOUNDARY_STATIC_BYTES`控制第一段静态数组的字节数，配置值必须是4B的整数倍。

#### 场景1：不调用SIMD VF

通过`UB_BOUNDARY_NO_SIMD_VF=ON`关闭SIMD VF调用。不调用SIMD VF时，当前Kernel的UB可用上限为256KB，扣除固定的32B结果区后，`static_ub`上限为262112B。

```text
static_ub上限 = 256KB - 32B = 262112B
```

**执行结果**：

| `static_ub`大小 | UB总占用 | 结果 |
| ---: | ---: | --- |
| 262112B（256KB - 32B） | 256KB | 通过 |
| 262116B | 256KB + 4B | Kernel启动失败 |

```text
# static_ub为262112B
[Case1] mode=without SIMD VF, static_bytes=262112
[Case1] launch_ret=0, sync_ret=0, result=PASSED

# static_ub为262116B
[Case1] mode=without SIMD VF, static_bytes=262116
[Case1] launch_ret=107000, sync_ret=107000, result=FAILED
```

#### 场景2：调用SIMD VF，使用默认编译配置

使用默认编译配置调用SIMD VF。根据[3510架构规格](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description)，6KB VF栈预留和2KB Ascend C API预留不计入可用UB，因此`static_ub`与固定的32B结果区合计不能超过248KB：

```text
可用UB上限 = 256KB - 8KB = 248KB
static_ub上限 = 248KB - 32B = 253920B
```

**执行结果**：

| `static_ub`大小 | UB总占用 | 结果 |
| ---: | ---: | --- |
| 253920B（248KB - 32B） | 248KB | 通过 |
| 253924B | 248KB + 4B | Kernel启动失败 |

```text
# static_ub为253920B
[Case1] mode=with SIMD VF, static_bytes=253920
[Case1] launch_ret=0, sync_ret=0, result=PASSED

# static_ub为253924B
[Case1] mode=with SIMD VF, static_bytes=253924
[Case1] launch_ret=107000, sync_ret=107000, result=FAILED
```

#### 场景3：调用SIMD VF并关闭VF栈预留空间

调用SIMD VF，并通过`UB_BOUNDARY_DISABLE_RESERVED=ON`关闭6KB VF栈预留。2KB Ascend C API预留仍然存在，因此`static_ub`与固定的32B结果区合计不能超过254KB。

```text
可用UB上限 = 256KB - 2KB = 254KB
static_ub上限 = 254KB - 32B = 260064B
```

**执行结果**：

| `static_ub`大小 | UB总占用 | 结果 |
| ---: | ---: | --- |
| 260064B（254KB - 32B） | 254KB | 通过 |
| 260068B | 254KB + 4B | Kernel启动失败 |

```text
# static_ub为260064B
[Case1] mode=with SIMD VF, static_bytes=260064
[Case1] launch_ret=0, sync_ret=0, result=PASSED

# static_ub为260068B
[Case1] mode=with SIMD VF, static_bytes=260068
[Case1] launch_ret=107000, sync_ret=107000, result=FAILED
```

三种场景均按4B粒度完成边界验证，即表中上限值运行成功，增加4B后Kernel启动失败。本Case使用静态数组验证边界，不用于比较静态UB和动态UB的容量差异。

**分析**：

默认配置下调用SIMD VF时，6KB VF栈预留和2KB Ascend C API预留均不计入可用UB。`--cce-disable-vf-stack-reserved-ubuf`只释放6KB VF栈预留，不会释放2KB Ascend C API预留。根据[3510架构规格](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description)，开启该选项后，编译器无法再使用这6KB空间缓存寄存器溢出数据，因此需要确保不会发生寄存器溢出。

**结论**：

对于本Case的当前Kernel，不调用SIMD VF时可用UB为256KB；按默认配置调用SIMD VF时可用UB为248KB；调用SIMD VF并关闭6KB VF栈预留后可用UB为254KB。实际开发中不能仅根据编译选项推断可用空间，还需要结合Kernel中的全部UB声明和VF实现进行确认。

### Case 2：包含SIMT VF时的UB可用上限

**样例目标**：

确定包含SIMT VF时，当前Kernel中的静态UB数组和动态UB数组各自的可用上限，并确认两种声明方式的可用上限是否一致。

**核心实现**：

Case 2使用默认编译配置，不关闭8KB预留空间。根据[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制)，单个Vector Core共有256KB UB，包含SIMT VF时还需要保留8KB预留空间和至少32KB Data Cache，因此静态UB与动态UB合计最多为216KB：

```text
静态UB与动态UB合计上限 = 256KB - 8KB - 32KB = 216KB
```

当前Kernel还固定包含32B结果区，因此可配置的UB数组上限为：

```text
UB数组上限 = 216KB - 32B = 221152B
```

216KB是本Case默认编译配置下的上限；调整影响预留空间的编译选项后，需要根据实际保留的空间重新计算，不能继续使用216KB作为上限。

`CASE2_UB_BYTES`用于配置UB数组大小，`CASE2_USE_STATIC_UB=OFF`表示使用动态UB，`CASE2_USE_STATIC_UB=ON`表示使用静态UB。动态模式通过Kernel启动参数配置动态UB大小，静态模式不配置动态UB。两种模式仅UB声明方式不同：SIMT VF均写满整个配置范围，MainScalar读取首尾元素并由Host侧校验，确保最后一个`uint32_t`元素被实际访问。关键代码如下：

```cpp
#if defined(CASE2_USE_STATIC_UB)
__ubuf__ uint32_t case2_ub[CASE2_UB_WORDS];
#else
extern __ubuf__ uint32_t simt_dynamic_ub[];
__ubuf__ uint32_t* case2_ub = simt_dynamic_ub;
#endif
__ubuf__ uint32_t result[RESULT_WORDS_PER_CASE];

asc_vf_call<SimtFillCase2Ub>(dim3(THREAD_COUNT), case2_ub, CASE2_UB_WORDS, CASE2_MARKER);
WaitVectorToScalar();
result[3] = case2_ub[0];
result[4] = case2_ub[CASE2_UB_WORDS - 1];
```

**执行结果**：

静态和动态两种模式在221152B（216KB - 32B）均运行通过：

```text
[Case2] ub_type=dynamic, max_ub_bytes=221152, configured_ub_bytes=221152
Case 2 (SIMT VF usable UB): PASSED

[Case2] ub_type=static, max_ub_bytes=221152, configured_ub_bytes=221152
Case 2 (SIMT VF usable UB): PASSED
```

两种模式均增加一个`uint32_t`元素，将UB数组大小调整为221156B（与32B结果区合计超过216KB上限4B），此时Kernel启动失败：

```text
[Case2] ub_type=dynamic, max_ub_bytes=221152, configured_ub_bytes=221156
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 2 (SIMT VF usable UB): FAILED

[Case2] ub_type=static, max_ub_bytes=221152, configured_ub_bytes=221156
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 2 (SIMT VF usable UB): FAILED
```

**结论**：

在本Case的默认编译配置下，当前Kernel的静态UB与动态UB合计上限为216KB。扣除固定的32B结果区后，静态UB数组和动态UB数组的上限均为221152B（216KB - 32B）。

### Case 3：不同执行位置的静态UB和动态UB布局

**样例目标**：

说明MainScalar、SIMD VF和SIMT VF中的静态UB需要累加计入总量；同时说明三处名称不同的动态UB声明具有相同首地址，共用Kernel启动时配置的动态UB空间。

**核心实现**：

Case 3与Case 2使用相同的默认编译配置，因此静态UB与动态UB合计上限仍为216KB。Case 3分别在MainScalar、SIMD VF和SIMT VF中声明静态UB。MainScalar声明1KB的`main_static_ub`和三个32B结果数组，SIMD VF声明2KB静态UB，SIMT VF声明4KB静态UB。不同执行位置的静态UB分别占用空间，需要累加计入当前Kernel的静态UB总量：

```text
静态UB总量 = 1KB + 96B + 2KB + 4KB = 7264B
动态UB上限 = 216KB - 7264B = 213920B
```

三个执行位置均访问各自静态数组的末元素，避免静态数组声明被编译器优化。动态UB的空间大小仅在Kernel启动时通过`CASE3_DYNAMIC_BYTES`配置一次。以下代码展示三个执行位置中的静态UB和动态UB声明：

```cpp
// MainScalar
extern __ubuf__ uint32_t main_dynamic_ub[];
__ubuf__ uint32_t main_static_ub[MIXED_MAIN_STATIC_WORDS];
// SIMD VF
extern __ubuf__ uint32_t simd_dynamic_ub[];
__ubuf__ uint32_t simd_static_ub[MIXED_SIMD_STATIC_WORDS];
// SIMT VF
extern __ubuf__ uint32_t simt_dynamic_ub[];
__ubuf__ uint32_t simt_static_ub[MIXED_SIMT_STATIC_WORDS];
```

三个执行位置分别将动态UB数组的首地址转换为整数并写入结果区，Host侧打印并比较三个地址，用于验证这些数组是否从同一地址开始。样例还通过首元素依次写入`0xC0030001`、`0xC0030002`和`0xC0030003`，并在三个执行位置读取动态区末元素，用于验证跨执行位置读写和整个配置范围。打印的地址属于AICore UB地址空间，仅用于比较三者是否相等。

**执行结果**：

根据UB占用计算得到的动态UB上限为213920B（216KB - 7264B静态UB），按此大小配置时运行通过：

```text
[Case3] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213920
[Case3] main_dynamic_ub=0x1c60, simd_dynamic_ub=0x1c60, simt_dynamic_ub=0x1c60
Case 3 usage: static_total_bytes=7264, dynamic_bytes=213920, same_start_address=YES
Case 3 (mixed UB): PASSED
```

增加一个`uint32_t`元素，将动态UB大小调整为213924B（静态UB与动态UB合计超过216KB上限4B）后，Kernel启动失败：

```text
[Case3] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213924
[ERROR] [Bisheng] Launch kernel failure! ret 107000
Case 3 (mixed UB): FAILED
```

**结论**：

在本Case的默认编译配置下，当前Kernel中的7264B静态UB从216KB可用UB空间中统一扣除，动态UB可用上限为213920B（216KB - 7264B）。三个执行位置声明的动态UB名称不同，但首地址相同，共用Host侧在Kernel启动时配置的动态UB空间。

### Case 4：小静态UB越界写入不同UB区域

**样例目标**：

从一块4KB静态UB出发，分别访问静态数组合法范围、未申请UB、6KB VF栈预留、2KB Ascend C API预留和Data Cache，说明各访问是否越界、是否报告运行错误以及是否出现精度错误。

**核心实现**：

Case 4使用默认编译配置，仅申请4KB静态UB，不配置动态UB。由于静态UB与动态UB占用较小，Data Cache达到128KB上限，并位于UB高地址端。根据[3510架构规格](../../../../docs/zh/guide/编程指南/高级编程/硬件实现/架构规格/NPU架构版本3510.md#compile-options-description)和[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md)，Data Cache之前还包含8KB预留空间，其中6KB用于VF栈预留，2KB用于Ascend C API预留。Case 4的UB布局计算如下：

```text
Data Cache大小 = min(256KB - 4KB - 0KB - 8KB, 128KB) = 128KB
Data Cache起点 = 256KB - 128KB = 128KB
预留空间起点 = 128KB - 8KB = 120KB
未申请UB大小 = 120KB - 4KB = 116KB
```

![Case 4 UB地址布局与访问位置](./figures/case4_ub_layout.png "Case 4 UB地址布局与访问位置")

`static_ub`的合法访问范围为前4KB。`CASE4_ACCESS_REGION`在编译期确定目标区域和目标区域的字节偏移，Host侧将字节偏移换算为下标并传入Kernel。Kernel通过`volatile`指针和运行时下标写入选定的目标区域：

| 配置值 | 目标区域 | 相对静态数组首地址的访问偏移 |
| ---: | --- | ---: |
| `0` | 静态UB合法范围的最后一个`uint32_t`元素 | 4092B |
| `1` | 未申请的普通UB范围中部 | 63488B |
| `2` | 6KB VF栈预留区中部 | 125952B |
| `3` | 2KB Ascend C API预留区中部 | 130048B |
| `4` | Data Cache起始位置 | 131072B |

Kernel首先调用SIMT VF读取GM输入并写出逐元素加一结果，使GM输入进入Data Cache。随后，MainScalar以4KB静态数组的首地址为基址，通过运行时下标写入目标区域；最后再次调用同一个SIMT VF读取GM并执行计算。Host侧检查Kernel启动和同步结果，并将第二次计算结果与Golden数据比较，判断输出精度是否受到影响：

```cpp
__ubuf__ uint32_t static_ub[1024];
__ubuf__ volatile uint32_t* static_data = static_ub;
static_data[access_index] = 0xDEADBEEF;
```

通过`CASE4_ACCESS_REGION`选择目标区域，例如设置为`4`时访问Data Cache起始位置。

**执行结果**：

五种访问配置的执行结果如下：

| 目标区域 | 启动/同步结果 | 精度结果 | 结果说明 |
| --- | --- | --- | --- |
| 静态UB合法范围 | 成功 | 通过 | 合法访问 |
| 未申请的普通UB范围 | 成功 | 通过 | 已越界，但本次运行无可见异常 |
| 6KB VF栈预留 | 成功 | 通过 | 已越界，本次运行未出现可见异常 |
| 2KB Ascend C API预留 | 成功 | 通过 | 已越界，本次运行未出现可见异常 |
| Data Cache | 成功 | 失败，1个元素不匹配 | 缓存数据被破坏，但未报告运行错误 |

五种访问配置的关键输出如下。`launch_ret=0`和`sync_ret=0`表示Kernel启动和同步均未报告错误；`precision_check`表示输出是否与Golden数据一致：

```text
# 静态UB合法范围
[Case4] region=static_ub, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=4092
[Case4] mismatch_count=0
[Case4] precision_check=PASSED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED

# 未申请的普通UB范围
[Case4] region=unallocated_ub, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=63488
[Case4] mismatch_count=0
[Case4] precision_check=PASSED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED

# 6KB VF栈预留区
[Case4] region=vf_stack_6kb, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=125952
[Case4] mismatch_count=0
[Case4] precision_check=PASSED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED

# 2KB Ascend C API预留区
[Case4] region=asc_reserved_2kb, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=130048
[Case4] mismatch_count=0
[Case4] precision_check=PASSED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED

# Data Cache
[Case4] region=data_cache, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=131072
[Case4] mismatch_count=1
[Case4] first_mismatch_index=896, expected=0xc0040381, actual=0xdeadbef0
[Case4] precision_check=FAILED
[Case4] launch_ret=0, sync_ret=0, observation=COMPLETED
```

**分析**：

精度校验通过不表示未申请UB或预留区可以访问。[SIMT内存层级](../../../../docs/zh/guide/编程指南/编程模型/AI-Core-SIMT编程/内存层级.md#共享内存大小的限制)说明，越过已申请的静态UB或动态UB范围后，未必立即表现为内存分配失败，但可能破坏Data Cache并导致计算结果不稳定或精度异常。因此，表中结果仅表示本次运行现象，不能据此判断越界访问合法，也不能据此判断目标区域未被使用。

**结论**：

Case 4中只有`CASE4_ACCESS_REGION=0`对应合法访问，配置为`1~4`时均会发生静态数组越界。即使Kernel正常结束且本次精度校验通过，超出静态数组声明范围的访问仍然属于错误。

## UB容量计算规则

在包含SIMT VF的默认编译模式下，单个Vector Core的256KB UB由静态UB、动态UB、8KB预留空间和Data Cache组成。8KB预留空间进一步划分为6KB VF栈预留和2KB Ascend C API预留。两部分用途不同，均不在已申请的静态UB或动态UB范围内：

```text
Data Cache = min(256KB - 静态UB - 动态UB - 8KB, 128KB)
8KB预留空间 = 6KB VF栈预留 + 2KB Ascend C API预留
```

Data Cache最小为32KB，因此静态UB与动态UB的合计容量需要满足以下约束：

```text
静态UB + 动态UB <= 216KB
实际动态UB访问范围 <= Kernel启动参数配置的动态UB大小
```

统计静态UB时，需要包含MainScalar、SIMD VF和SIMT VF中的全部声明。实际访问必须同时满足UB总容量限制和各数组的具体访问边界。

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
    SCENARIO_NUM=1                              # 选择Case，可选0、1、2、3、4
    mkdir -p build && cd build                  # 创建并进入build目录
    cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
      -DSCENARIO_NUM=${SCENARIO_NUM} ..         # 配置工程，可按需追加Case专用选项
    make -j                                     # 编译工程
    ./ub_usage_limit                            # 运行样例
    ```

**编译选项说明**：

| 选项 | 可选值 | 说明 |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
| `SCENARIO_NUM` | `0`~`4` | Case编号，默认值为`1` |
| `CASE0_ENABLE_FIX` | `ON`、`OFF` | Case 0是否检查静态数组运行时下标，默认值为`OFF` |
| `UB_BOUNDARY_STATIC_BYTES` | 4B的正整数倍 | Case 1静态UB数组大小，默认值为`253920`，单位为B |
| `UB_BOUNDARY_NO_SIMD_VF` | `ON`、`OFF` | Case 1是否不调用SIMD VF，默认值为`OFF` |
| `UB_BOUNDARY_DISABLE_RESERVED` | `ON`、`OFF` | Case 1是否传递`--cce-disable-vf-stack-reserved-ubuf`，默认值为`OFF` |
| `CASE2_UB_BYTES` | 4B的正整数倍 | Case 2静态或动态UB数组大小，默认值为`221152`，单位为B |
| `CASE2_USE_STATIC_UB` | `ON`、`OFF` | Case 2是否使用静态UB数组，默认值为`OFF`，即使用动态UB数组 |
| `CASE3_DYNAMIC_BYTES` | 4B的正整数倍 | Case 3动态UB大小，默认值为`213920`，单位为B |
| `CASE4_ACCESS_REGION` | `0`~`4` | Case 4访问区域，默认值为`0`。`0`：静态UB；`1`：未申请UB；`2`：6KB VF栈预留；`3`：2KB Ascend C API预留；`4`：Data Cache |

**执行结果示例**：

- Case 0使用默认的`CASE0_ENABLE_FIX=OFF`，关键输出如下：

    ```text
    [Case0] mismatch_count=1
    [Case0] precision_check=FAILED (fault reproduced)
    # 精度问题已出现
    ```

- Case 1默认调用SIMD VF，`static_ub`大小为253920B，关键输出如下：

    ```text
    [Case1] mode=with SIMD VF, static_bytes=253920
    [Case1] launch_ret=0, sync_ret=0, result=PASSED
    ```

- Case 2默认使用221152B动态UB，关键输出如下：

    ```text
    [Case2] ub_type=dynamic, max_ub_bytes=221152, configured_ub_bytes=221152
    Case 2 (SIMT VF usable UB): PASSED
    ```

- Case 3默认配置213920B动态UB，关键输出如下：

    ```text
    [Case3] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213920
    [Case3] main_dynamic_ub=0x1c60, simd_dynamic_ub=0x1c60, simt_dynamic_ub=0x1c60
    Case 3 usage: static_total_bytes=7264, dynamic_bytes=213920, same_start_address=YES
    Case 3 (mixed UB): PASSED
    ```

- Case 4使用默认的`CASE4_ACCESS_REGION=0`，关键输出如下：

    ```text
    [Case4] region=static_ub, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=4092
    [Case4] mismatch_count=0
    [Case4] precision_check=PASSED
    [Case4] launch_ret=0, sync_ret=0, observation=COMPLETED
    ```

**边界与异常结果示例**：

- Case 1使用默认SIMD VF配置，将`static_ub`从253920B增加至253924B后，UB总占用超过248KB上限4B，Kernel启动失败：

    ```text
    [Case1] mode=with SIMD VF, static_bytes=253924
    [Case1] launch_ret=107000, sync_ret=107000, result=FAILED
    ```

- Case 2将动态UB从221152B增加至221156B后，UB总占用超过216KB上限4B，Kernel启动失败：

    ```text
    [Case2] ub_type=dynamic, max_ub_bytes=221152, configured_ub_bytes=221156
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    Case 2 (SIMT VF usable UB): FAILED
    ```

- Case 3将动态UB从213920B增加至213924B后，UB总占用超过216KB上限4B，Kernel启动失败：

    ```text
    [Case3] static_total_bytes=7264, max_dynamic_bytes=213920, configured_dynamic_bytes=213924
    [ERROR] [Bisheng] Launch kernel failure! ret 107000
    Case 3 (mixed UB): FAILED
    ```

- Case 4设置`CASE4_ACCESS_REGION=4`，越界写入Data Cache。Kernel启动和同步均成功，但输出出现精度错误：

    ```text
    [Case4] region=data_cache, static_bytes=4096, dynamic_bytes=0, access_offset_bytes=131072
    [Case4] mismatch_count=1
    [Case4] precision_check=FAILED
    [Case4] launch_ret=0, sync_ret=0, observation=COMPLETED
    ```
