# all_mask<a name="ZH-CN_TOPIC_0000002000000022"></a>

## 产品支持情况<a name="section1550532418810"></a>

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

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"tensor_api/arch/vector/experimental/mask_reg_compute.h"`，命名空间为 `asc::te::experimental`。

创建与 `DataType` 元素宽度匹配的全有效谓词，等价于 `make_mask<mask_pattern::all, DataType>()`。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <typename DataType>
__simd_callee__ inline reg_tensor<bool> all_mask();
```

## 参数说明<a name="section622mcpsimp"></a>

| 参数名 | 描述 |
| --- | --- |
| `DataType` | mask所服务的数据元素类型。 |

## 数据类型<a name="section630mcpsimp"></a>

支持b8、b16和b32元素类型。

## 返回值说明<a name="section640mcpsimp"></a>

返回 `reg_tensor<bool>`，其 `reg` 成员保存全有效谓词。

## 约束说明<a name="section650mcpsimp"></a>

返回对象只保证 `reg` 成员有效，不应读取其 `mask` 成员。

## 调用示例<a name="section660mcpsimp"></a>

```cpp
auto fullMask = asc::te::experimental::all_mask<half>();
auto value = src.load(asc::te::make_coord(0)).with_mask(fullMask);
```
