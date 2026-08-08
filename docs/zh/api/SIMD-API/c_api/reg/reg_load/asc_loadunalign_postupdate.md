# asc_loadunalign_postupdate

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

将Unified Buffer（UB）中起始地址任意字节对齐的连续数据搬入矢量数据寄存器，搬入数据量为VL。搬运完成后启用Post Update模式，自动更新源地址指针或地址寄存器记录的地址偏移量。本接口是非对齐搬入的第二阶段，需要先调用[asc_loadunalign_pre](asc_loadunalign_pre.md)初始化非对齐寄存器[vector_load_unalign](../reg_data_types/data_type_definition.md#非对齐寄存器)。本接口提供两种功能模式：

- Post Update搬入：从`src1`起始地址搬运数据，搬运完成后启用Post Update模式，自动将传入的源地址指针向后偏移`count * sizeof(dtype)`字节，无需手动偏移地址。

- 带地址寄存器偏移Post Update搬入：`src1`为固定的基地址指针，接口不会修改该指针。实际搬入地址为`src1`的底层UB地址与`offset`记录的实际字节偏移之和。接口先使用当前`offset`搬入数据，搬入完成后再将`offset`记录的地址偏移量增加`inc`个元素。该模式需要与[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)配合使用。

`asc_update_addr_reg`的参数和本接口的`inc`分别控制不同循环迭代之间和同一循环迭代之内的地址偏移：

**不同循环迭代之间**：以一维循环和b8模式为例，`asc_update_addr_reg_b8`的`offset`参数表示相邻循环迭代之间的迭代偏移量。第`i`轮生成的地址寄存器中，记录的起始偏移量为`i * offset`。进入第`i + 1`轮并再次调用`asc_update_addr_reg_b8(offset)`时，地址寄存器会根据新的循环索引生成`(i + 1) * offset`，不会从上一轮经`inc`更新后的值继续累加。

**同一循环迭代之内**：`inc`表示本次搬入完成后对`offset`记录的地址偏移量追加的元素偏移，用于为本轮的下一次带地址寄存器偏移搬入准备地址。本轮最后一次调用之后若不再使用该地址寄存器，可将`inc`设置为0。下一轮的起始偏移仍由`asc_update_addr_reg`生成。

例如，每轮只调用一次本接口且每轮搬入一个VL时，将`asc_update_addr_reg`的迭代偏移量设置为一个VL，将`inc`设置为0。第`i`轮从基地址加`i`个VL的位置搬入，下一轮从基地址加`i + 1`个VL的位置搬入。每轮连续调用两次本接口时，将迭代偏移量设置为两个VL，将第一次调用的`inc`设置为一个VL，并将第二次调用的`inc`设置为0。第`i`轮依次从基地址加`2 * i`个VL和基地址加`2 * i + 1`个VL的位置搬入，下一轮从基地址加`2 * (i + 1)`个VL的位置开始。具体代码请参考[调用示例](#调用示例)。

本接口仅在AIV上执行有效，在AIC上调用为空操作。

## 函数原型

### Post Update搬入

```c
__simd_callee__ inline void asc_loadunalign_postupdate(vector_<dtype>& dst,
                                                       vector_load_unalign& src0,
                                                       __ubuf__ <dtype>*& src1,
                                                       uint32_t count)
```

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

```c
__simd_callee__ inline void asc_loadunalign_postupdate(vector_int8_t& dst,
                                                       vector_load_unalign& src0,
                                                       __ubuf__ int8_t*& src1,
                                                       uint32_t count)
```

### 带地址寄存器偏移Post Update搬入

```c
__simd_callee__ inline void asc_loadunalign_postupdate(vector_<dtype>& dst,
                                                       vector_load_unalign& src0,
                                                       __ubuf__ <dtype>* src1,
                                                       addr_reg& offset,
                                                       uint32_t inc)
```

dtype支持的数据类型为`int4b_t`、`int8_t`、`uint8_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`、`hifloat8_t`、`fp8_e8m0_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`。

```c
__simd_callee__ inline void asc_loadunalign_postupdate(vector_int8_t& dst,
                                                       vector_load_unalign& src0,
                                                       __ubuf__ int8_t* src1,
                                                       addr_reg& offset,
                                                       uint32_t inc)
```

## 参数说明

**表1** 参数说明（Post Update搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype须与src1一致，搬入VL长度数据。 |
| src0 | 输入 | 非对齐寄存器。保存非32字节对齐的数据，该寄存器大小为32字节，须通过[asc_loadunalign_pre](asc_loadunalign_pre.md)预先初始化。 |
| src1 | 输入/输出 | 源UB地址，实际读取地址无需32字节对齐。 |
| count | 输入 | Post Update步长，单位为元素个数。接口使用当前`src1`地址完成搬入后，将`src1`指针增加`count * sizeof(dtype)`字节。 |

**表2** 参数说明（带地址寄存器偏移Post Update搬入）

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| dst | 输出 | 目的矢量数据寄存器。dtype须与src1一致，搬入VL长度数据。 |
| src0 | 输入 | 非对齐寄存器。保存非32字节对齐的数据，该寄存器大小为32字节，须通过[asc_loadunalign_pre](asc_loadunalign_pre.md)预先初始化。 |
| src1 | 输入 | 源UB地址，实际读取地址无需32字节对齐。 |
| offset | 输入 | 地址寄存器，类型为`addr_reg`，须通过[asc_update_addr_reg](../reg_data_types/asc_update_addr_reg.md)生成。该寄存器记录的实际字节偏移由创建地址寄存器时选择的位宽模式决定：b8、b16、b32模式分别以1字节、2字节、4字节为偏移单位。 |
| inc | 输入 | Post Update步长，单位为元素个数。接口使用当前`offset`完成搬入后，将`offset`记录的地址偏移量增加`inc * sizeof(dtype)`字节。 |

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

- 本接口仅在AIV上生效，非AIV调用直接返回。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- src1与dst的dtype必须完全一致，dtype不一致触发异常。
- `src1`起始地址或`src1`与`offset`确定的实际访问地址无需32字节对齐，但实际访问范围须在UB地址空间内，否则会报错。
- UB容量上限为256KB，用户可用容量随编译选项与编程场景变化（默认预留6KB SIMD VF栈 + 2KB Ascend C预留，可用248KB；SIMD+SIMT混编时再划分32KB～128KB作Data Cache，可用容量进一步减少）。UB地址偏移后不可超过实际可用容量，否则会报错。
- 如果本指令与其他指令存在UB地址重叠，需要插入同步指令[asc_mem_bar](../sync_control/asc_mem_bar.md)，保证多个指令串行化，防止出现异常数据。
- 针对带地址寄存器偏移搬入：必须先调用`asc_update_addr_reg`接口。
- 该接口必须与[asc_loadunalign_pre](asc_loadunalign_pre.md)配合使用。

## 调用示例

### Post Update搬入

下面以`int8_t`为例，通过Post Update模式将数据从非对齐源地址连续搬运到非对齐目的地址。`count`表示相邻两次搬运起始地址之间的元素间隔；连续搬运完整VL时，`count`应设置为一个VL包含的`int8_t`元素个数。搬入侧的非对齐寄存器只需在循环前初始化一次，循环内每次调用`asc_loadunalign_postupdate`后，`src_addr`都会自动增加`count`个元素。搬出侧使用[asc_storeunalign_postupdate](../reg_store/asc_storeunalign_postupdate.md)，每次搬出后`dst_addr`同样增加`count`个元素；该接口会在非对齐寄存器中缓存尾部数据，因此循环结束后必须调用[asc_storeunalign_post_postupdate](../reg_store/asc_storeunalign_post_postupdate.md)完成尾部写回。

```cpp
__simd_vf__ inline void load_store_postupdate(
    __ubuf__ int8_t* dst_base, __ubuf__ int8_t* src_base,
    uint32_t count, uint16_t repeat_times)
{
    vector_int8_t data;
    vector_load_unalign load_unalign_reg;
    vector_store_unalign store_unalign_reg;
    __ubuf__ int8_t* src_addr = src_base;
    __ubuf__ int8_t* dst_addr = dst_base;
    asc_loadunalign_pre(load_unalign_reg, src_addr);
    for (uint16_t i = 0; i < repeat_times; ++i) {
        asc_loadunalign_postupdate(data, load_unalign_reg, src_addr, count);
        asc_storeunalign_postupdate(dst_addr, store_unalign_reg, data, count);
    }
    asc_storeunalign_post_postupdate(dst_addr, store_unalign_reg, 0);
}
```

第`i`轮循环使用更新前的`src_addr`搬入数据，实际搬入地址为`src_base + i * count`；数据搬出到`dst_base + i * count`。本轮搬运完成后，`src_addr`和`dst_addr`分别更新为对应基地址加`(i + 1) * count`个元素。循环结束后，两个地址相对各自基地址均增加`repeat_times * count`个元素。

### 每轮调用一次带地址寄存器偏移Post Update搬入

下面以`int8_t`为例，每轮循环搬运一个VL。`vl_elements`表示一个VL包含的`int8_t`元素个数，同时作为`asc_update_addr_reg_b8`的迭代偏移量。每轮仅调用一次`asc_loadunalign_postupdate`，因此`inc`设置为0。搬入的数据通过`asc_storeunalign_postupdate`连续搬出到目的地址，并在循环结束后完成尾部写回。

```cpp
__simd_vf__ inline void load_store_one_vl_per_loop(
    __ubuf__ int8_t* dst_base, __ubuf__ int8_t* src_addr,
    uint32_t vl_elements, uint16_t repeat_times)
{
    vector_int8_t data;
    vector_load_unalign load_unalign_reg;
    vector_store_unalign store_unalign_reg;
    __ubuf__ int8_t* dst_addr = dst_base;
    addr_reg offset;
    for (uint16_t i = 0; i < repeat_times; ++i) {
        offset = asc_update_addr_reg_b8(vl_elements);
        asc_loadunalign_pre(load_unalign_reg, src_addr, offset);
        asc_loadunalign_postupdate(data, load_unalign_reg, src_addr, offset, 0);
        asc_storeunalign_postupdate(dst_addr, store_unalign_reg, data, vl_elements);
    }
    asc_storeunalign_post_postupdate(dst_addr, store_unalign_reg, 0);
}
```

第`i`轮循环的搬入地址为`src_addr + i * vl_elements`，下一轮由`asc_update_addr_reg_b8`根据迭代偏移量生成新的起始偏移。搬出地址为`dst_base + i * vl_elements`，每轮搬出后由`asc_storeunalign_postupdate`自动推进一个VL。

### 每轮调用两次带地址寄存器偏移Post Update搬入

下面以`int8_t`为例，每轮循环连续搬运两个VL。`asc_update_addr_reg_b8`的迭代偏移量为`2 * vl_elements`。第一次调用将`inc`设置为`vl_elements`，使`offset`在搬运完成后增加一个VL；第二次调用使用更新后的`offset`搬运同一轮内的第二个VL。第二次调用后不再使用当前`offset`，因此其`inc`设置为0。两次搬入的数据分别搬出，目的地址随每次搬出自动推进一个VL；循环结束后再统一完成尾部写回。

```cpp
__simd_vf__ inline void load_store_two_vl_per_loop(
    __ubuf__ int8_t* dst_base, __ubuf__ int8_t* src_addr,
    uint32_t vl_elements, uint16_t repeat_times)
{
    vector_int8_t data0;
    vector_int8_t data1;
    vector_load_unalign load_unalign_reg;
    vector_store_unalign store_unalign_reg;
    __ubuf__ int8_t* dst_addr = dst_base;
    addr_reg offset;
    for (uint16_t i = 0; i < repeat_times; ++i) {
        offset = asc_update_addr_reg_b8(2 * vl_elements);
        asc_loadunalign_pre(load_unalign_reg, src_addr, offset);
        asc_loadunalign_postupdate(data0, load_unalign_reg, src_addr, offset, vl_elements);
        asc_storeunalign_postupdate(dst_addr, store_unalign_reg, data0, vl_elements);
        asc_loadunalign_postupdate(data1, load_unalign_reg, src_addr, offset, 0);
        asc_storeunalign_postupdate(dst_addr, store_unalign_reg, data1, vl_elements);
    }
    asc_storeunalign_post_postupdate(dst_addr, store_unalign_reg, 0);
}
```

第`i`轮循环的两次搬入地址依次为`src_addr + i * 2 * vl_elements`和`src_addr + i * 2 * vl_elements + vl_elements`，对应搬出地址依次为`dst_base + i * 2 * vl_elements`和`dst_base + i * 2 * vl_elements + vl_elements`。进入下一轮循环后，`asc_update_addr_reg_b8`重新生成该轮的搬入起始偏移；`dst_addr`则已由两次搬出自动推进两个VL。
