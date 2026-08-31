# reg_tensor::with_mask<a name="ZH-CN_TOPIC_0000002000000016"></a>

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

头文件路径为：`"tensor_api/arch/vector/experimental/reg_tensor.h"`，命名空间为`asc::te::experimental`。

将输入的`reg_tensor<bool>`寄存器`maskReg.reg`绑定到当前`reg_tensor<DataType>`的`mask`成员，不修改数据成员`reg`。可与`local_tensor::load`链式调用。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
__simd_callee__ inline reg_tensor& with_mask(
    const reg_tensor<bool>& maskReg);
```

## 参数说明<a name="section622mcpsimp"></a>

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `maskReg` | 输入 | `reg_tensor<bool>`类型的寄存器，必须保证其`reg`成员已初始化。 |

## 返回值说明<a name="section630mcpsimp"></a>

返回当前`reg_tensor`的引用。

## 约束说明<a name="section640mcpsimp"></a>

`with_mask`只复制掩码寄存器，不校验掩码寄存器与数据元素宽度是否一致。

## 调用示例<a name="section650mcpsimp"></a>

```cpp
auto value = src.load(asc::te::make_coord(0))
                .with_mask(asc::te::experimental::all_mask<float>());
dst.store(asc::te::make_coord(0), value);
```
