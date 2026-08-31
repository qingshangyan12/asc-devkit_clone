# make_mask<a name="ZH-CN_TOPIC_0000002000000021"></a>

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

头文件路径为：`"tensor_api/experimental/arch/vector/mask_reg_compute.h"`，接口命名空间为`asc::te::experimental`。

根据模式和数据元素宽度创建掩码寄存器。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <mask_pattern Pattern = mask_pattern::all, typename DataType>
__simd_callee__ inline reg_tensor<bool> make_mask();
```

## 参数说明<a name="section622mcpsimp"></a>

| 参数名 | 描述 |
| --- | --- |
| `Pattern` | 创建掩码寄存器的模式，enum class类型。<br><code>enum class mask_pattern {<br>    all,      // 所有元素设置为有效数据<br>    vl1,      // 最低1个元素设置为有效数据<br>    vl2,      // 最低2个元素设置为有效数据<br>    vl3,      // 最低3个元素设置为有效数据<br>    vl4,      // 最低4个元素设置为有效数据<br>    vl8,      // 最低8个元素设置为有效数据<br>    vl16,     // 最低16个元素设置为有效数据<br>    vl32,     // 最低32个元素设置为有效数据<br>    vl64,     // 最低64个元素设置为有效数据<br>    vl128,    // 最低128个元素设置为有效数据<br>    every3,   // 3的倍数设置为有效数据<br>    every4,   // 4的倍数设置为有效数据<br>    half,     // 最低一半元素设置为有效数据<br>    quarter,  // 最低四分之一元素设置为有效数据<br>    none      // 所有元素设置为无效数据<br>};</code> |
| `DataType` | 掩码寄存器所服务的数据元素类型，其字节数决定掩码粒度。 |

## 数据类型<a name="section630mcpsimp"></a>

支持1Byte、2Byte和4Byte元素类型，分别使用b8、b16和b32 mask指令。不支持8Byte及其他字节数的类型。

## 返回值说明<a name="section640mcpsimp"></a>

返回`reg_tensor<bool>`，其`reg`成员保存生成的掩码。

## 约束说明<a name="section650mcpsimp"></a>

掩码寄存器数量上限为8。超出限制上限的寄存器数据会写入预留的8KUB内存中，可能会引起性能劣化。编译器会自动复用生命周期结束的寄存器和预留内存，若寄存器与预留内存均存在可用空间，将优先复用寄存器。

## 调用示例<a name="section660mcpsimp"></a>

```cpp
auto mask = asc::te::experimental::make_mask<asc::te::experimental::mask_pattern::every3, float>();
auto value = src.load(asc::te::make_coord(0)).with_mask(mask);
```
