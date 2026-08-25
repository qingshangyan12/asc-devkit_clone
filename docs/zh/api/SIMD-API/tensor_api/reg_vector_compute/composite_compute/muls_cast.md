# muls_cast

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/tensor.h"`。

命名空间：`asc::te::experimental`。

将`float`输入与标量逐元素相乘，并将结果转换为`half`。转换结果写入返回寄存器的偶数`half`位置。

## 函数原型

```cpp
__simd_callee__ inline decltype(auto) muls_cast(
    const reg_tensor<float>& src, const float& scalar)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | `reg_tensor<float>`输入，同时提供执行mask。 |
| `scalar` | 输入 | `float`标量乘数。 |

## 返回值说明

返回`reg_tensor<half>`。每个有效`float`输入对应一个位于偶数位置的`half`结果，结果mask等于`src.mask`。

## 约束说明

- 返回值采用偶数位置布局，不能按连续`half`数据解释。

## 调用示例

```cpp
auto src = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());
auto result = asc::te::experimental::muls_cast(src, 1.5f);
```
