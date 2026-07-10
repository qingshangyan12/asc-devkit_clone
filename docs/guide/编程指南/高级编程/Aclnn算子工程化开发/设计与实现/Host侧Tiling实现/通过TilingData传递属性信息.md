# 通过TilingData传递属性信息

本文介绍aclnn算子工程中如何在Host侧Tiling读取算子属性，并通过`TilingData`传递给Kernel侧。阅读本文前，建议先了解[算子原型定义](../算子原型定义.md)和[Host侧Tiling实现基本流程](./基本流程.md)。

算子属性来自算子原型中的`Attr(...)`定义。调用aclnn接口时，属性由调用方传入，框架在执行Tiling函数时把属性放到`TilingContext`中。Host侧Tiling可以读取这些属性，并按Kernel侧需要写入`TilingData`。

整体流程如下：

```text
算子原型定义属性
  ↓
aclnn接口调用时传入属性值
  ↓
Host侧Tiling从TilingContext读取属性
  ↓
写入TilingData字段
  ↓
Kernel侧读取TilingData并参与计算
```

## 1.什么时候需要通过TilingData传递属性

属性是否需要传到Kernel侧，取决于属性会不会影响Kernel侧计算。

常见场景如下：

| 属性用途 | 处理方式 | 示例 |
|---|---|---|
| 直接参与Kernel计算 | 把属性值写入`TilingData` | `negative_slope`参与LeakyRelu计算 |
| 影响Host侧切分策略 | Host侧使用属性计算切分参数，把计算结果写入`TilingData` | `reduceDim`用于计算归约轴长度 |
| 只影响shape推导或dtype推导 | 通常无需写入`TilingData` | `keepDim`只影响输出shape |
| 用于选择Kernel分支 | 属性可参与`TilingKey`或Kernel模板参数选择 | `mode`选择不同计算路径 |

写文档和代码时建议先说清楚：Kernel侧需要的是属性原值，还是Host侧根据属性计算出来的结果。这样读者更容易判断`TilingData`字段该怎么设计。

## 2.在算子原型中定义属性

属性先在算子原型中定义。以LeakyRelu为例，`negative_slope`是可选Float属性，默认值为`0.0f`：

```cpp
namespace ops {
class LeakyReluCustom : public OpDef {
public:
    LeakyReluCustom(const char* name) : OpDef(name)
    {
        this->Input("x").ParamType(REQUIRED).DataType({ge::DT_FLOAT}).Format({ge::FORMAT_ND});
        this->Output("y").ParamType(REQUIRED).DataType({ge::DT_FLOAT}).Format({ge::FORMAT_ND});
        this->Attr("negative_slope").AttrType(OPTIONAL).Float(0.0f);

        this->SetInferShape(ge::InferShape).SetInferDataType(ge::InferDataType);
        this->AICore().SetTiling(optiling::TilingFunc).AddConfig("ascend910b");
    }
};
OP_ADD(LeakyReluCustom);
} // namespace ops
```

属性在`Attr(...)`中的定义顺序，会影响Host侧通过索引读取属性的顺序。上例只有一个属性，因此Host侧读取时使用索引`0`。

## 3.在TilingData中增加属性字段

Kernel侧需要使用`negative_slope`，因此在`TilingData`中增加字段保存该属性值：

```cpp
#ifndef LEAKY_RELU_CUSTOM_TILING_H
#define LEAKY_RELU_CUSTOM_TILING_H

#include <cstdint>

struct LeakyReluCustomTilingData {
    uint32_t totalLength;
    uint32_t tileNum;
    float negativeSlope;
};

#endif // LEAKY_RELU_CUSTOM_TILING_H
```

字段设计时可以按Kernel侧使用方式命名。原型中的属性名是`negative_slope`，`TilingData`中的字段名可以使用C++风格的`negativeSlope`，只要Host侧写入和Kernel侧读取保持一致。

## 4.在Host侧Tiling读取属性

Host侧通过`context->GetAttrs()`获取属性集合，再通过`GetAttrPointer<T>(index)`读取指定属性。下面示例读取`negative_slope`，并写入`TilingData`：

```cpp
namespace optiling {
constexpr uint32_t NUM_BLOCKS = 8;
constexpr uint32_t TILE_NUM = 16;

static ge::graphStatus TilingFunc(gert::TilingContext* context)
{
    LeakyReluCustomTilingData* tiling =
        context->GetTilingData<LeakyReluCustomTilingData>();
    const uint32_t totalLength =
        context->GetInputShape(0)->GetOriginShape().GetShapeSize();

    const gert::RuntimeAttrs* attrs = context->GetAttrs();
    const float* negativeSlope = attrs->GetAttrPointer<float>(0);

    context->SetBlockDim(NUM_BLOCKS);
    tiling->totalLength = totalLength;
    tiling->tileNum = TILE_NUM;
    tiling->negativeSlope = *negativeSlope;

    size_t* workspaceSizes = context->GetWorkspaceSizes(1);
    workspaceSizes[0] = 0;
    return ge::GRAPH_SUCCESS;
}
} // namespace optiling
```

这段代码完成三件事：

1. 读取输入shape，计算`totalLength`。
2. 读取第0个属性`negative_slope`。
3. 把Kernel侧需要的属性值写入`TilingData`。

`GetAttrPointer<T>(index)`中的`T`需要和属性定义类型匹配。`Float`属性按`float`读取，`Bool`属性按`bool`读取，`Int`属性通常按`int64_t`读取后再按Kernel需要转换为`uint32_t`、`int32_t`等类型。

## 5.传递属性原值还是派生参数

有些属性本身不直接参与Kernel计算，只用于Host侧推导Kernel需要的参数。以归约类算子为例，`reduceDim`表示按哪个维度归约。Kernel侧通常更关心归约轴长度，因此Host侧可以读取`reduceDim`后计算`reduceAxisLen`，再把`reduceAxisLen`写入`TilingData`。

示例代码如下：

```cpp
struct ReduceMaxTilingData {
    uint32_t reduceAxisLen;
    uint32_t totalLength;
};

static ge::graphStatus TilingFunc(gert::TilingContext* context)
{
    ReduceMaxTilingData* tiling = context->GetTilingData<ReduceMaxTilingData>();

    const gert::RuntimeAttrs* attrs = context->GetAttrs();
    const int64_t* reduceDim = attrs->GetAttrPointer<int64_t>(0);

    const gert::Shape& xShape = context->GetInputShape(0)->GetOriginShape();
    const uint32_t reduceAxisLen =
        static_cast<uint32_t>(xShape.GetDim(static_cast<size_t>(*reduceDim)));

    tiling->reduceAxisLen = reduceAxisLen;
    tiling->totalLength = static_cast<uint32_t>(xShape.GetShapeSize());
    context->SetBlockDim(8);

    size_t* workspaceSizes = context->GetWorkspaceSizes(1);
    workspaceSizes[0] = 0;
    return ge::GRAPH_SUCCESS;
}
```

这种写法把属性转换成Kernel侧真正需要的参数，Kernel侧读取逻辑更直接。属性原值仍然可以写入`TilingData`，适合Kernel侧需要根据原始属性继续分支判断的场景。

## 6.Kernel侧读取属性字段

Kernel侧通过`GET_TILING_DATA`读取`TilingData`，再把属性字段传入计算类或直接参与计算。以LeakyRelu为例：

```cpp
extern "C" __global__ __aicore__ void leaky_relu_custom(GM_ADDR x, GM_ADDR y,
    GM_ADDR workspace, GM_ADDR tiling)
{
    REGISTER_TILING_DEFAULT(LeakyReluCustomTilingData);
    GET_TILING_DATA(tilingData, tiling);

    KernelLeakyRelu op;
    op.Init(x, y, tilingData.totalLength, tilingData.tileNum,
        tilingData.negativeSlope);
    op.Process();
}
```

Kernel侧不再访问`TilingContext`。属性需要在Kernel侧使用时，要在Host侧Tiling阶段提前写入`TilingData`。

## 7.列表和字符串属性怎么处理

`TilingData`适合保存简单、定长、Kernel侧可直接解析的数据。列表和字符串属性需要先转换成Kernel侧容易使用的形式。

常见处理方式如下：

| 属性类型 | 建议写入`TilingData`的形式 | 说明 |
|---|---|---|
| `ListInt` | 定长数组加长度字段 | 适合轴列表、shape类参数 |
| `ListFloat` | 定长数组加长度字段 | 适合少量系数参数 |
| `String` | 枚举值或整数模式 | Host侧把字符串映射为`mode` |
| `ListListInt` | 展平成定长数组，再补充每段长度 | 适合小规模二维配置 |

例如字符串属性`mode`可以在Host侧转换成整数：

```cpp
enum class ComputeMode : uint32_t {
    FAST = 0,
    ACCURATE = 1,
};

const char* mode = attrs->GetAttrPointer<char>(0);
if (std::string(mode) == "accurate") {
    tiling->mode = static_cast<uint32_t>(ComputeMode::ACCURATE);
} else {
    tiling->mode = static_cast<uint32_t>(ComputeMode::FAST);
}
```

列表属性写入定长数组时，需要同时记录有效长度。列表属性的读取方式以当前属性类型接口说明为准，写入`TilingData`时可以整理成下面这种形式：

```cpp
struct SliceTilingData {
    uint32_t axisNum;
    int64_t axes[8];
};

constexpr uint32_t MAX_AXES_NUM = 8;
tiling->axisNum = axisNum;
for (uint32_t i = 0; i < tiling->axisNum && i < MAX_AXES_NUM; ++i) {
    tiling->axes[i] = axes[i];
}
```

实际工程中还需要根据算子原型约束检查列表长度。超过`TilingData`定长数组容量时，Host侧Tiling需要返回失败，或在原型校验阶段限制属性取值范围。

## 8.使用检查点

通过`TilingData`传递属性时，建议按下面几项检查：

- 属性已在算子原型中通过`Attr(...)`定义，必选和可选属性设置清晰。
- Host侧`GetAttrPointer<T>(index)`中的索引和`Attr(...)`定义顺序一致。
- `GetAttrPointer<T>(index)`中的类型`T`和属性定义类型匹配。
- 写入`TilingData`的是Kernel侧真正需要的字段，属性原值和派生参数不要混用。
- `TilingData`字段已在Host侧显式赋值，Kernel侧不会读取未初始化字段。
- 列表和字符串属性已转换成Kernel侧容易解析的定长数据或枚举值。
- 属性影响Kernel分支时，已同步设置`TilingKey`或Kernel模板参数。

## 9.相关文档

- [算子原型定义](../算子原型定义.md)：了解如何声明属性类型、必选属性和可选属性。
- [Host侧Tiling实现基本流程](./基本流程.md)：了解`TilingData`、`numBlocks`、workspace大小和`TilingKey`的基本写法。
- [多分支策略](../多分支策略.md)：了解属性如何参与`TilingKey`或Kernel模板参数选择。
- [Attr](../../../../../../api/Utils-API/原型注册与管理/OpDef/Attr.md)：查看属性注册接口说明。
- [OpAttrDef](../../../../../../api/Utils-API/原型注册与管理/OpAttrDef/OpAttrDef-272.md)：查看属性类型和默认值配置方式。
