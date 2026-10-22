# asc_pack_datablock

## 产品支持情况

<!-- npu="960" id8 -->
- Ascend 960PR&960DT系列产品：支持
<!-- end id8 -->
<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"c_api/reg_compute/compute/reg_permute_sel.h"`。

按照DataBlock粒度对矢量数据寄存器中的数据进行打包，源操作数和目的操作数的数据类型一致。每个DataBlock长度为32B。

本接口为Reg矢量计算接口，仅在AIV上生效，仅支持Ascend 960PR&960DT系列产品的`dav-9201`架构。

## 函数原型

```cpp
// 通过引用参数输出结果（占位符形式）
__simd_callee__ inline void asc_pack_datablock(vector_<dtype>& dst,
                                               vector_<dtype> src)

// 通过函数返回值返回结果（占位符形式）
__simd_callee__ inline vector_<dtype> asc_pack_datablock(vector_<dtype> src)
```

### dtype支持数据类型

| dtype |
| --- |
| uint16_t |
| int16_t |
| half |
| uint32_t |
| int32_t |
| float |
| uint64_t |
| int64_t |

### 函数原型典型示例

```cpp
// 按DataBlock粒度打包uint16_t类型数据。
__simd_callee__ inline void asc_pack_datablock(vector_uint16_t& dst,
                                               vector_uint16_t src)

// 通过函数返回值返回打包结果。
__simd_callee__ inline vector_float asc_pack_datablock(vector_float src)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --------- | ----- | ---------------- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |

矢量数据寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

- 通过引用参数输出结果的函数原型无返回值，打包结果写入`dst`。
- 通过函数返回值返回结果的函数原型返回打包结果，类型与`src`一致。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 仅支持Ascend 960PR&960DT系列产品的`dav-9201`架构。
- 源操作数和目的操作数的数据类型必须一致。

## 调用示例

将代码保存为`example.asc`后，可通过`bisheng`命令编译运行，其中`--npu-arch`参数需指定为`dav-9201`。

```bash
bisheng example.asc -o main --npu-arch=dav-9201 && ./main
```

```cpp
__simd_vf__ inline void pack_datablock_vf(__ubuf__ uint16_t* dst_addr, __ubuf__ uint16_t* src_addr)
{
    vector_uint16_t src;
    vector_uint16_t dst;
    vector_bool mask = asc_create_mask_b16(PAT_ALL);
    asc_loadalign(src, src_addr);
    asc_pack_datablock(dst, src);
    asc_storealign(dst_addr, dst, mask);
}
```
