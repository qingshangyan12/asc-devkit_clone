# mull

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

命名空间：`asc::te::experimental`。

逐元素执行32位整数宽乘，并返回乘积的低32位和高32位。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_pair<T> mull(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`int32_t`、`uint32_t`。 |
| `src0` | 输入 | 第一个乘数，同时提供执行mask。 |
| `src1` | 输入 | 第二个乘数。 |

## 返回值说明

返回`reg_pair<T>`。`first`保存乘积低32位，`second`保存乘积高32位，二者的mask均等于`src0.mask`。

## 约束说明

- 两个输入寄存器的元素类型必须一致。

## 调用示例

```cpp
auto mask = asc::te::all_mask<uint32_t>();
auto src0 = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
auto src1 = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);
auto [low, high] = asc::te::experimental::mull(src0, src1);
```
