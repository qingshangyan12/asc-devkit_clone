# Reg矢量计算C API编程<a name="ZH-CN_TOPIC_REG_COMPUTE_CAPI"></a>

本文介绍如何在AscendC中使用Reg矢量计算C API编写寄存器级向量计算代码。Reg矢量计算C API以`vector_*`类型表示矢量数据寄存器，以`vector_bool`表示掩码寄存器，以`iter_reg`表示地址寄存器，并通过`asc_*`函数完成UB和寄存器之间的数据搬运、寄存器内计算、掩码生成、地址更新和同步控制。

Reg矢量计算C API适用于开发者需要显式组织`Load -> Compute -> Store`流程的高性能场景。数据先从GM搬运到UB，再由C API搬入寄存器，在寄存器中完成计算后写回UB。

## 编程模型<a name="section_capi_model"></a>

Reg矢量计算C API主要在`__simd_vf__`函数内使用。调用方负责准备UB地址、计算处理规模并通过`asc_vf_call`进入VF函数；VF函数内部声明寄存器变量，调用`asc_*`接口完成寄存器级向量计算。

数据路径如下：

```text
Global Memory (GM)
        ^
        |
        | DataCopy
        v
Unified Buffer (UB)
        ^
        |
        | asc_load / asc_loadalign / asc_store / asc_storealign
        v
VF Reg: vector_* / vector_bool / iter_reg
        ^
        |
        | asc_add / asc_mul / asc_madd / asc_select / ...
        v
Vector Unit
```

`__simd_vf__`函数通常遵循以下结构：

```text
1. 声明vector_*、vector_bool、iter_reg等寄存器变量。
2. 根据数据类型创建或更新mask。
3. 在循环内从UB搬入一个VL的数据到寄存器。
4. 调用asc_*计算接口在寄存器内完成计算。
5. 将结果从寄存器写回UB。
```

## C API类别<a name="section_capi_categories"></a>

Reg矢量计算C API位于`docs/api/c_api/reg`目录下，可按功能分为以下类别。

| 类别 | 代表接口 | 作用 |
| --- | --- | --- |
| 数据类型 | `vector_half`、`vector_float`、`vector_bool`、`iter_reg` | 描述矢量数据寄存器、掩码寄存器、地址寄存器等对象 |
| 对齐搬入 | `asc_loadalign`、`asc_loadalign_brc`、`asc_loadalign_deintlv` | 从32B对齐UB地址搬入矢量数据寄存器 |
| 通用搬入 | `asc_load` | 从UB地址搬入矢量数据寄存器，不区分是否对齐 |
| 非对齐搬入 | `asc_loadunalign_pre`、`asc_loadunalign`、`asc_loadunalign_postupdate` | 从非32B对齐UB地址连续搬入数据 |
| 对齐搬出 | `asc_storealign`、`asc_storealign_pack`、`asc_storealign_intlv` | 将寄存器数据搬出到32B对齐UB地址 |
| 通用搬出 | `asc_store` | 将寄存器数据搬出到UB地址，不区分是否对齐 |
| 非对齐搬出 | `asc_storeunalign`、`asc_storeunalign_postupdate`、`asc_storeunalign_post` | 将寄存器数据连续搬出到非32B对齐UB地址 |
| 掩码操作 | `asc_create_mask_b8/b16/b32`、`asc_update_mask_b8/b16/b32` | 生成整VL或尾块mask |
| 地址寄存器 | `asc_create_iter_reg_b8/b16/b32` | 创建用于地址偏移的`iter_reg` |
| 算术计算 | `asc_add`、`asc_sub`、`asc_mul`、`asc_div`、`asc_madd` | 执行逐元素算术计算 |
| 标量计算 | `asc_add_scalar`、`asc_mul_scalar`、`asc_max_scalar`、`asc_min_scalar` | 矢量寄存器与标量进行计算 |
| 比较选择 | `asc_eq`、`asc_gt`、`asc_le`、`asc_select` | 生成比较结果或按mask选择数据 |
| 逻辑运算 | `asc_and`、`asc_or`、`asc_xor`、`asc_not` | 对寄存器或掩码执行逻辑运算 |
| 类型转换 | `asc_half2float`、`asc_float2half`、`asc_int322float`等 | 在不同矢量数据类型之间转换 |
| 数据重排 | `asc_pack`、`asc_unpack`、`asc_squeeze`、`asc_unsqueeze`、`asc_intlv`、`asc_deintlv` | 完成打包、解包、交织、解交织等操作 |
| 归约计算 | `asc_reduce_sum`、`asc_reduce_max`、`asc_pair_reduce_sum` | 在寄存器内执行归约类计算 |
| 同步控制 | `asc_mem_bar` | 控制Reg搬运和计算之间的同步关系 |

## 命名方式<a name="section_capi_naming"></a>

Reg矢量计算C API的命名有固定规律。

| 命名元素 | 含义 | 示例 |
| --- | --- | --- |
| `asc_`前缀 | AscendC C API接口统一前缀 | `asc_add`、`asc_loadalign` |
| 操作名 | 表示具体操作 | `add`、`mul`、`storealign`、`loadunalign` |
| 数据位宽后缀 | 表示按b8、b16或b32生成mask或地址寄存器 | `asc_create_mask_b16`、`asc_create_iter_reg_b32` |
| 模式后缀 | 表示搬运或计算模式 | `asc_loadalign_brc`、`asc_storealign_pack` |
| `_scalar`后缀 | 表示矢量寄存器与标量计算 | `asc_add_scalar`、`asc_gt_scalar` |
| `_pre`后缀 | 表示非对齐搬入前初始化 | `asc_loadunalign_pre` |
| `_post`后缀 | 表示非对齐搬出后处理 | `asc_storeunalign_post` |
| `_postupdate`后缀 | 表示接口执行后自动更新地址 | `asc_loadunalign_postupdate`、`asc_storeunalign_postupdate` |

官方接口名以文档定义为准，例如`asc_loadalign`、`asc_storealign`是固定接口名。本文所有自定义函数名和变量名均使用snake_case，例如`add_half_vf`、`dst_addr`、`one_repeat_size`。

## 寄存器类型<a name="section_capi_registers"></a>

### 矢量数据寄存器

矢量数据寄存器用于保存一个VL宽度的矢量数据。在Ascend 950PR/Ascend 950DT版本中，VL为256B。例如`vector_float`可保存`256B / sizeof(float) = 64`个`float`元素。

| 位宽 | 常见矢量数据类型 |
| --- | --- |
| b8 | `vector_int8_t`、`vector_uint8_t`、`vector_int4x2_t`、`vector_hifloat8_t`、`vector_fp8_e4m3fn_t`、`vector_fp8_e5m2_t` |
| b16 | `vector_int16_t`、`vector_uint16_t`、`vector_half`、`vector_bfloat16_t` |
| b32 | `vector_int32_t`、`vector_uint32_t`、`vector_float` |
| b64 | `vector_int64_t`、`vector_uint64_t` |

### 掩码寄存器

`vector_bool`用于控制哪些元素参与本轮计算。创建整VL mask时使用`asc_create_mask_b8/b16/b32`；处理尾块时使用`asc_update_mask_b8/b16/b32`。

```cpp
vector_bool all_mask = asc_create_mask_b16(PAT_ALL);
uint32_t remain_count = 96;
vector_bool tail_mask = asc_update_mask_b16(remain_count);
```

### 地址寄存器

`iter_reg`用于保存地址偏移量，可通过`asc_create_iter_reg_b8/b16/b32`创建。后续可传入支持`iter_reg`参数的搬运接口。

```cpp
iter_reg addr_reg = asc_create_iter_reg_b16(128);
```

### 非对齐寄存器

`vector_load_unalign`和`vector_store_unalign`用于连续非对齐搬运场景，作为跨32B对齐边界数据的临时缓存。

```cpp
vector_load_unalign load_ureg;
vector_store_unalign store_ureg;
```

## 编程示例<a name="section_capi_usage"></a>

Reg矢量计算C API的基本使用流程开发与[Reg矢量计算编程]步骤相同，接口可以同等语义替换如下。

```cpp
__simd_vf__ inline void add_half_vf(__ubuf__ half* dst_addr,
                                    __ubuf__ half* src0_addr,
                                    __ubuf__ half* src1_addr,
                                    uint32_t one_repeat_size,
                                    uint16_t repeat_times)
{
    vector_half src0_reg;
    vector_half src1_reg;
    vector_half dst_reg;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);

    for (uint16_t i = 0; i < repeat_times; ++i) {
        uint32_t offset = i * one_repeat_size;
        asc_load(src0_reg, src0_addr + offset);
        asc_load(src1_reg, src1_addr + offset);
        asc_add(dst_reg, src0_reg, src1_reg, mask);
        asc_store(dst_addr + offset, dst_reg);
    }
}
```

这段代码包含四类典型动作：

1. 声明`vector_half`作为源和目的矢量数据寄存器。
2. 使用`asc_create_mask_b16(PAT_ALL)`创建b16数据类型的全有效mask。
3. 使用`asc_loadalign`从UB搬入源数据。
4. 使用`asc_add`完成寄存器内加法，并使用`asc_storealign`写回UB。

## 特性描述

### 通用搬运<a name="section_capi_normal_move"></a>

`asc_load`和`asc_store`是不区分是否32B对齐的通用搬运接口。它们适合需要统一处理对齐和非对齐地址的场景；如果已知地址满足对齐要求，并且追求更高性能，优先选择`asc_loadalign`和`asc_storealign`。

```cpp
__simd_vf__ inline void copy_vf(__ubuf__ float* dst_addr,
                                __ubuf__ float* src_addr,
                                uint32_t one_repeat_size,
                                uint16_t repeat_times)
{
    vector_float src_reg;

    for (uint16_t i = 0; i < repeat_times; ++i) {
        uint32_t offset = i * one_repeat_size;
        asc_load(src_reg, src_addr + offset);
        asc_store(dst_addr + offset, src_reg);
    }
}
```

`asc_store`还支持传入`count`参数，用于搬出不超过一个VL的元素数量。`count`的取值范围为`[1, VL / sizeof(dst)]`。

#### 对齐搬运<a name="section_capi_aligned_move"></a>

当UB地址满足32B对齐要求时，优先使用对齐搬运接口。

```cpp
__simd_vf__ inline void copy_align_vf(__ubuf__ half* dst_addr,
                                      __ubuf__ half* src_addr,
                                      uint32_t one_repeat_size,
                                      uint16_t repeat_times)
{
    vector_half src_reg;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);

    for (uint16_t i = 0; i < repeat_times; ++i) {
        uint32_t offset = i * one_repeat_size;
        asc_loadalign(src_reg, src_addr + offset);
        asc_storealign(dst_addr + offset, src_reg, mask);
    }
}
```

常见对齐搬运接口包括：

| 接口 | 说明 |
| --- | --- |
| `asc_loadalign` | NORM模式，从对齐UB地址搬入VL数据 |
| `asc_loadalign_brc` | 搬入一个元素并广播到寄存器所有位置 |
| `asc_loadalign_deintlv` | 从UB读取交织数据并拆分到两个寄存器 |
| `asc_storealign` | NORM模式，将寄存器数据搬出到对齐UB地址 |
| `asc_storealign_pack` | 按mask压缩后搬出 |
| `asc_storealign_intlv` | 将两个寄存器交织搬出到UB |

普通连续读写场景优先使用`asc_loadalign`和`asc_storealign`。



#### 非对齐搬运<a name="section_capi_unaligned_move"></a>

非对齐搬运用于UB起始地址不是32B对齐的连续读写场景。读取非对齐地址前，先使用`asc_loadunalign_pre`初始化`vector_load_unalign`，再使用`asc_loadunalign`或`asc_loadunalign_postupdate`搬入数据。写非对齐地址时，先使用`asc_storeunalign`或`asc_storeunalign_postupdate`连续搬出，再使用`asc_storeunalign_post`或`asc_storeunalign_post_postupdate`处理尾部缓存。

推荐把搬入初始化和搬出后处理放到循环外。

```cpp
__simd_vf__ inline void neg_unalign_vf(__ubuf__ half* dst_addr,
                                       __ubuf__ half* src_addr,
                                       uint32_t one_repeat_size,
                                       uint16_t repeat_times)
{
    vector_half src_reg;
    vector_half dst_reg;
    vector_load_unalign load_ureg;
    vector_store_unalign store_ureg;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);

    asc_loadunalign_pre(load_ureg, src_addr);
    for (uint16_t i = 0; i < repeat_times; ++i) {
        asc_loadunalign_postupdate(src_reg, load_ureg, src_addr, one_repeat_size);
        asc_neg(dst_reg, src_reg, mask);
        asc_storeunalign_postupdate(dst_addr, store_ureg, dst_reg, one_repeat_size);
    }
    asc_storeunalign_post_postupdate(dst_addr, store_ureg, 0);
}
```

使用`uint32_t`作为连续非对齐搬出步长时，`asc_storeunalign_postupdate`应与`asc_storeunalign_post_postupdate`配合使用；使用`iter_reg`作为偏移时，应与`asc_storeunalign_post`配合使用。

#### PostUpdate<a name="section_capi_postupdate"></a>

PostUpdate表示搬运接口在完成本次访问后自动更新UB地址指针或地址偏移，适合连续访问场景。它可以减少循环内显式计算`addr + i * stride`的标量指令数量。

常见PostUpdate接口如下：

| 接口 | 用途 |
| --- | --- |
| `asc_loadalign_postupdate` | 从UB对齐搬入`vector_bool`等数据，并更新源地址 |
| `asc_loadunalign_postupdate` | 从非对齐UB地址连续搬入，并更新源地址或偏移 |
| `asc_storeunalign_postupdate` | 向非对齐UB地址连续搬出，并更新目的地址或偏移 |

示例：

```cpp
__simd_vf__ inline void load_unalign_postupdate_vf(__ubuf__ int8_t* src_addr,
                                                   uint32_t one_repeat_size,
                                                   uint16_t repeat_times)
{
    vector_int8_t src_reg;
    vector_load_unalign load_ureg;

    asc_loadunalign_pre(load_ureg, src_addr);
    for (uint16_t i = 0; i < repeat_times; ++i) {
        asc_loadunalign_postupdate(src_reg, load_ureg, src_addr, one_repeat_size);
        // 使用src_reg参与后续计算
    }
}
```

#### iter_reg地址寄存器<a name="section_capi_iter_reg"></a>

`iter_reg`用于保存地址偏移量，通过`asc_create_iter_reg_b8/b16/b32`创建。后缀中的`b8`、`b16`、`b32`表示按对应数据位宽解释偏移。

```cpp
__simd_vf__ inline void add_with_iter_reg_vf(__ubuf__ half* dst_addr,
                                             __ubuf__ half* src0_addr,
                                             __ubuf__ half* src1_addr,
                                             uint32_t one_repeat_size,
                                             uint16_t repeat_times)
{
    vector_half src0_reg;
    vector_half src1_reg;
    vector_half dst_reg;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    iter_reg addr_reg = asc_create_iter_reg_b16(one_repeat_size);

    for (uint16_t i = 0; i < repeat_times; ++i) {
        asc_loadalign(src0_reg, src0_addr, addr_reg);
        asc_loadalign(src1_reg, src1_addr, addr_reg);
        asc_add(dst_reg, src0_reg, src1_reg, mask);
        asc_storealign(dst_addr, dst_reg, addr_reg, mask);
    }
}
```

规则连续访问可以直接使用指针加偏移或PostUpdate；当接口形态明确支持`iter_reg`，或地址偏移复用能减少循环内标量计算时，再使用`iter_reg`。

#### 掩码与尾块<a name="section_capi_mask_tail"></a>

C API计算接口通常通过`vector_bool mask`控制有效元素。`asc_create_mask_b8/b16/b32(PAT_ALL)`适用于整VL都有效的场景；`asc_update_mask_b8/b16/b32`适用于尾块处理。

`asc_update_mask_*`接收剩余元素数的引用参数，生成当前VL对应的mask，并更新剩余元素数。

```cpp
__simd_vf__ inline void add_tail_vf(__ubuf__ half* dst_addr,
                                    __ubuf__ half* src0_addr,
                                    __ubuf__ half* src1_addr,
                                    uint32_t count,
                                    uint32_t one_repeat_size,
                                    uint16_t repeat_times)
{
    vector_half src0_reg;
    vector_half src1_reg;
    vector_half dst_reg;
    uint32_t remain_count = count;

    for (uint16_t i = 0; i < repeat_times; ++i) {
        vector_bool mask = asc_update_mask_b16(remain_count);
        uint32_t offset = i * one_repeat_size;
        asc_loadalign(src0_reg, src0_addr + offset);
        asc_loadalign(src1_reg, src1_addr + offset);
        asc_add(dst_reg, src0_reg, src1_reg, mask);
        asc_storealign(dst_addr + offset, dst_reg, mask);
    }
}
```

多数C API计算接口的mask行为为ZEROING模式：mask未选中的元素在输出中置零。若尾块后的无效lane仍会被后续计算消费，应明确这些lane是否会影响结果。

#### 寄存器内计算<a name="section_capi_compute"></a>

寄存器内计算接口以`asc_*`命名，通常以目的寄存器作为第一个参数，源寄存器和mask作为后续参数。

```cpp
vector_half src0_reg;
vector_half src1_reg;
vector_half dst_reg;
vector_bool mask = asc_create_mask_b16(PAT_ALL);

asc_add(dst_reg, src0_reg, src1_reg, mask);
asc_mul(dst_reg, dst_reg, src1_reg, mask);
```

常见计算接口包括：

| 类型 | 接口示例 |
| --- | --- |
| 算术 | `asc_add`、`asc_sub`、`asc_mul`、`asc_div`、`asc_madd` |
| 标量 | `asc_add_scalar`、`asc_mul_scalar`、`asc_max_scalar`、`asc_min_scalar` |
| 比较 | `asc_eq`、`asc_ne`、`asc_gt`、`asc_ge`、`asc_lt`、`asc_le` |
| 逻辑 | `asc_and`、`asc_or`、`asc_xor`、`asc_not` |
| 选择 | `asc_select` |
| 填充 | `asc_duplicate`、`asc_duplicate_scalar` |
| 归约 | `asc_reduce_sum`、`asc_reduce_max`、`asc_reduce_min` |
| 转换 | `asc_half2float`、`asc_float2half`、`asc_int322float`等 |

#### 手工VF融合<a name="section_capi_vf_fuse"></a>

手工VF融合指开发者把一段连续计算链路写在同一个`__simd_vf__`函数和同一个循环中，使中间结果保留在寄存器中继续被后续指令消费，减少写回UB再读回寄存器的开销。

例如计算`dst = (a + b) * c + d`，推荐写成一个VF：

```cpp
__simd_vf__ inline void fused_compute_vf(__ubuf__ half* dst_addr,
                                         __ubuf__ half* a_addr,
                                         __ubuf__ half* b_addr,
                                         __ubuf__ half* c_addr,
                                         __ubuf__ half* d_addr,
                                         uint32_t one_repeat_size,
                                         uint16_t repeat_times)
{
    vector_half a_reg;
    vector_half b_reg;
    vector_half c_reg;
    vector_half d_reg;
    vector_half tmp_reg;
    vector_half dst_reg;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);

    for (uint16_t i = 0; i < repeat_times; ++i) {
        uint32_t offset = i * one_repeat_size;
        asc_loadalign(a_reg, a_addr + offset);
        asc_loadalign(b_reg, b_addr + offset);
        asc_loadalign(c_reg, c_addr + offset);
        asc_loadalign(d_reg, d_addr + offset);

        asc_add(tmp_reg, a_reg, b_reg, mask);
        asc_mul(tmp_reg, tmp_reg, c_reg, mask);
        asc_add(dst_reg, tmp_reg, d_reg, mask);

        asc_storealign(dst_addr + offset, dst_reg, mask);
    }
}
```

融合时需要控制寄存器数量。融合粒度过大可能增加`vector_*`、`vector_bool`、`vector_load_unalign`和`vector_store_unalign`数量，导致寄存器压力升高。

#### 指令双发<a name="section_capi_dual_issue"></a>

指令双发依赖硬件同时发射和执行多条互不依赖指令的能力。编写VF时，应尽量让执行队列中存在足够多的独立指令。例如先搬入多个独立源寄存器，再组织计算，避免连续形成很长的数据依赖链。

```cpp
for (uint16_t i = 0; i < repeat_times; ++i) {
    uint32_t offset = i * one_repeat_size;
    asc_loadalign(src0_reg, src0_addr + offset);
    asc_loadalign(src1_reg, src1_addr + offset);
    asc_loadalign(src2_reg, src2_addr + offset);

    asc_add(tmp0_reg, src0_reg, src1_reg, mask);
    asc_mul(tmp1_reg, src2_reg, src1_reg, mask);
    asc_add(dst_reg, tmp0_reg, tmp1_reg, mask);

    asc_storealign(dst_addr + offset, dst_reg, mask);
}
```

如果融合后依赖链很长，可以考虑拆分计算阶段，或适度展开循环，在减少UB搬运和保持指令并行之间做权衡。

#### 硬件循环<a name="section_capi_hardware_loop"></a>

VF中的循环如果满足硬件识别规范，编译器可以将其转换为Hardware Loop，减少软件循环中的计数、比较和跳转开销。

常见编码要求如下：

- 循环迭代变量使用`uint16_t`类型。
- 循环起始值从0开始，步长为1。
- 循环边界在执行过程中不被修改。
- 循环体内避免`if/else`、三目运算符等运行时跳转控制流；可用`if constexpr`处理编译期分支。
- 尾块可用`asc_update_mask_*`处理，避免在循环内引入复杂分支。

典型写法如下：

```cpp
for (uint16_t i = 0; i < repeat_times; ++i) {
    uint32_t offset = i * one_repeat_size;
    asc_loadalign(src0_reg, src0_addr + offset);
    asc_loadalign(src1_reg, src1_addr + offset);
    asc_add(dst_reg, src0_reg, src1_reg, mask);
    asc_storealign(dst_addr + offset, dst_reg, mask);
}
```

## 编写建议<a name="section_capi_suggestions"></a>

- 整VL场景使用`asc_create_mask_*`，尾块场景使用`asc_update_mask_*`。
- 已知UB地址32B对齐时优先使用`asc_loadalign`和`asc_storealign`。
- 连续非对齐访问优先使用`asc_loadunalign_postupdate`和`asc_storeunalign_postupdate`，并把初始化和后处理移出循环。
- 中间结果优先保留在`vector_*`寄存器中继续计算，减少UB往返读写。
- 控制单个VF内寄存器数量，避免过度融合造成寄存器压力。
- 保持循环结构简单，优先写成可识别为Hardware Loop的形式。