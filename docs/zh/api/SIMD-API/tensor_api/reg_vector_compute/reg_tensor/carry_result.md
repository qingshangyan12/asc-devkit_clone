# carry_result

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

头文件路径为：`"tensor_api/experimental/arch/vector/reg_tensor.h"`，命名空间为`asc::te::experimental`。

保存算术结果和对应的进位标志，用作`addc`和`subc`的双输出返回类型。

## 定义原型

```cpp
template <typename DataType>
struct carry_result {
    reg_tensor<DataType> value;
    reg_tensor<bool> carry;
};
```

## 参数说明

| 参数名 | 描述 |
| --- | --- |
| `DataType` | `value`成员的元素类型。 |

## 成员说明

| 成员名 | 描述 |
| --- | --- |
| `value` | 算术结果，类型为`reg_tensor<DataType>`。 |
| `carry` | 进位或无借位标志，类型为`reg_tensor<bool>`。 |

## 调用示例

```cpp
auto result = asc::te::experimental::addc(src0, src1);
auto value = result.value;
auto carry = result.carry;
```
