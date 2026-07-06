# 使用高阶API时配套的Tiling实现

本文介绍aclnn算子工程中调用AscendC高阶API时，Host侧Tiling需要额外完成的工作。阅读本文前，建议先了解[Host侧Tiling实现基本流程](./基本流程.md)。

在基础API实现中，`TilingData`通常只保存开发者自己定义的字段，例如`totalLength`、`tileNum`。调用部分高阶API时，Kernel侧还需要一组由高阶API定义的Tiling参数，例如`SoftMaxTiling`、`PadTiling`、`TCubeTiling`。这组参数由Host侧配套Tiling接口计算，再随算子自己的`TilingData`一起传到Kernel侧。

整体流程可以概括为：

```text
Host侧读取shape、dtype、平台信息
  ↓
调用高阶API配套的Tiling接口
  ↓
把高阶API的Tiling结构写入算子TilingData
  ↓
Kernel侧读取TilingData
  ↓
把其中的高阶APITiling字段传给高阶API
```

## 1.判断是否需要配套Tiling

部分高阶API需要在Host侧生成一份完整的高阶APITiling结构。写Tiling前先看对应高阶API文档中的Kernel接口参数和Tiling接口说明，重点确认两类信息：

- Kernel侧接口是否要求传入`SoftMaxTiling`、`PadTiling`、`TopkTiling`、`TCubeTiling`等结构体。
- Tiling侧是否提供`XxxTilingFunc`、`GetXxxTilingInfo`、`MultiCoreMatmulTiling::GetTiling`等接口。

如果Kernel侧高阶API只需要临时空间大小，Host侧可以把临时空间大小作为普通字段写入`TilingData`。如果Kernel侧高阶API需要专用Tiling结构，Host侧需要按本文流程把该结构嵌入算子`TilingData`。

常见场景如下：

| 高阶API类型 | Host侧常见工作 | Kernel侧常见使用方式 |
|---|---|---|
| SoftMax/SoftMaxGrad | 计算临时空间大小，调用`SoftMaxTilingFunc`或`SoftMaxGradTilingFunc` | 将`SoftMaxTiling`传给SoftMax类接口 |
| Pad/UnPad/Transpose | 计算最小和最大临时空间，调用对应Tiling函数 | 将`PadTiling`、`UnPadTiling`或`ConfusionTransposeTiling`传给接口 |
| Sort/TopK | 计算临时空间和排序相关Tiling参数 | 将排序Tiling结构传给排序接口 |
| Matmul | 通过`MultiCoreMatmulTiling`生成`TCubeTiling` | 将`TCubeTiling`传给Matmul对象或拷贝到Kernel侧使用 |

## 2.定义包含高阶API字段的TilingData

使用高阶API专用Tiling结构时，`TilingData`需要使用宏定义方式。原因是高阶API的Tiling结构由框架提供，需要通过`TILING_DATA_FIELD_DEF_STRUCT`嵌入到算子自己的`TilingData`中。

以SoftMax为例，`TilingData`可以按下面方式定义：

```cpp
#include "register/tilingdata_base.h"
#include "lib/softmax/softmax_tiling.h"

namespace optiling {
BEGIN_TILING_DATA_DEF(SoftmaxCustomTilingData)
    TILING_DATA_FIELD_DEF(uint32_t, totalLength);
    TILING_DATA_FIELD_DEF(uint32_t, rowsPerCore);
    TILING_DATA_FIELD_DEF(uint32_t, tailRows);
    TILING_DATA_FIELD_DEF_STRUCT(SoftMaxTiling, softmaxTilingData);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(SoftmaxCustom, SoftmaxCustomTilingData)
} // namespace optiling
```

这段定义里有两类字段：

- 算子自己的字段：`totalLength`、`rowsPerCore`、`tailRows`，用于描述本算子的分核和数据范围。
- 高阶API字段：`softmaxTilingData`，用于保存SoftMax接口内部需要的Tiling参数。

字段设计时只需要把Kernel侧会读取的信息放进去。高阶API结构体内部字段通常由配套Tiling函数填写，开发者只需要保存并传递。

## 3.在Host侧调用高阶APITiling接口

Host侧Tiling函数需要先完成本算子的分核策略，再根据每个Kernel分支实际处理的shape调用高阶API配套Tiling接口。

下面以SoftMax为例，展示Host侧Tiling函数中最核心的几步：

```cpp
static ge::graphStatus TilingFunc(gert::TilingContext* context)
{
    SoftmaxCustomTilingData tiling;

    const gert::Shape& inputShape = context->GetInputShape(0)->GetOriginShape();
    const uint32_t totalRows = static_cast<uint32_t>(inputShape.GetDim(0));
    const uint32_t reduceLen = static_cast<uint32_t>(inputShape.GetDim(1));
    const uint32_t numBlocks = 8;

    const uint32_t rowsPerCore = (totalRows + numBlocks - 1) / numBlocks;
    const uint32_t mainRows = rowsPerCore;
    const uint32_t tailRows = totalRows - rowsPerCore * (numBlocks - 1);

    tiling.set_totalLength(static_cast<uint32_t>(inputShape.GetShapeSize()));
    tiling.set_rowsPerCore(rowsPerCore);
    tiling.set_tailRows(tailRows);
    context->SetBlockDim(numBlocks);

    ge::Shape softmaxComputeShape({mainRows, reduceLen});
    const uint32_t localWorkspaceSize =
        AscendC::GetSoftMaxMinTmpSize(softmaxComputeShape, sizeof(float), true);
    AscendC::SoftMaxTilingFunc(
        softmaxComputeShape, sizeof(float), localWorkspaceSize, tiling.softmaxTilingData);

    auto workspaceSizes = context->GetWorkspaceSizes(1);
    workspaceSizes[0] = 0;
    tiling.SaveToBuffer(context->GetRawTilingData()->GetData(),
        context->GetRawTilingData()->GetCapacity());
    context->GetRawTilingData()->SetDataSize(tiling.GetDataSize());
    context->SetTilingKey(1);
    return ge::GRAPH_SUCCESS;
}
```

这段代码可以按四步理解：

1. 读取输入shape，确定本算子的分核方式。
2. 根据当前分支的计算shape构造`ge::Shape`。
3. 调用高阶API的临时空间查询接口和Tiling计算接口。
4. 将完整`TilingData`保存到框架提供的RawTilingData缓冲区。

使用宏定义方式的`TilingData`时，Host侧通常通过局部对象写入字段，再调用`SaveToBuffer`和`SetDataSize`完成序列化。这个写法和标准C++结构体方式下的`context->GetTilingData<T>()`不同。

## 4.临时空间大小怎么处理

很多高阶API会提供最小临时空间和最大临时空间查询接口，例如`GetSoftMaxMinTmpSize`、`GetPadMaxMinTmpSize`。这类接口的返回值用于帮助开发者选择Kernel侧可用于高阶API计算的LocalMemory空间。

可以按下面原则处理：

- 最小临时空间用于保证功能正确。传入配套Tiling函数的空间大小需要满足高阶API文档要求。
- 最大临时空间通常用于性能参考。实际可用空间还要结合本算子自己的队列、Buffer和其他中间数据决定。
- 如果算子还需要GM侧workspace，通过`context->GetWorkspaceSizes(1)`设置workspace大小。LocalMemory里的临时空间大小和GM侧workspace大小需要分开理解。

Pad这类接口通常会同时返回最大值和最小值：

```cpp
uint32_t maxValue = 0;
uint32_t minValue = 0;
AscendC::GetPadMaxMinTmpSize(srcShape, sizeof(float), maxValue, minValue);

const uint32_t localWorkspaceSize = minValue;
AscendC::PadTilingFunc(srcShape, oriSrcShape, localWorkspaceSize,
    sizeof(float), tiling.padTilingData);
```

调试初期可以先使用`minValue`验证功能。性能调优阶段再结合Kernel侧LocalMemory使用情况，选择更合适的临时空间大小。

## 5.Matmul类高阶API的Tiling写法

Matmul类高阶API通常通过`MultiCoreMatmulTiling`生成`TCubeTiling`。这类Tiling除了描述单核计算块，还会包含多核切分、矩阵shape、format、dtype、bias等信息。

Host侧核心流程如下：

```cpp
static ge::graphStatus TilingFunc(gert::TilingContext* context)
{
    auto ascendcPlatform =
        platform_ascendc::PlatformAscendC(context->GetPlatformInfo());

    const int32_t m = 1024;
    const int32_t n = 640;
    const int32_t k = 256;
    const int32_t baseM = 128;
    const int32_t baseN = 128;

    matmul_tiling::MultiCoreMatmulTiling cubeTiling(ascendcPlatform);
    cubeTiling.SetDim(2);
    cubeTiling.SetAType(TPosition::GM, CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
    cubeTiling.SetBType(TPosition::GM, CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
    cubeTiling.SetCType(TPosition::GM, CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
    cubeTiling.SetShape(m, n, k);
    cubeTiling.SetOrgShape(m, n, k);
    cubeTiling.SetFixSplit(baseM, baseN, -1);
    cubeTiling.SetBufferSpace(-1, -1, -1);

    MatmulCustomTilingData tiling;
    if (cubeTiling.GetTiling(tiling.cubeTilingData) == -1) {
        return ge::GRAPH_FAILED;
    }

    context->SetBlockDim(tiling.cubeTilingData.usedCoreNum);
    tiling.SaveToBuffer(context->GetRawTilingData()->GetData(),
        context->GetRawTilingData()->GetCapacity());
    context->GetRawTilingData()->SetDataSize(tiling.GetDataSize());
    context->SetTilingKey(1);
    return ge::GRAPH_SUCCESS;
}
```

对应的`TilingData`定义中，需要嵌入`TCubeTiling`：

```cpp
BEGIN_TILING_DATA_DEF(MatmulCustomTilingData)
    TILING_DATA_FIELD_DEF_STRUCT(TCubeTiling, cubeTilingData);
END_TILING_DATA_DEF;
```

Matmul场景中，`numBlocks`通常可以由`TCubeTiling`中的多核切分结果决定。例如样例中常见写法是使用`usedCoreNum`设置启动核数。实际工程中还需要结合AIC/AIV启动方式和算子自己的并行策略确认。

## 6.Kernel侧如何使用

Kernel侧读取`TilingData`后，把高阶API的Tiling字段继续传给高阶API即可。以SoftMax为例：

```cpp
extern "C" __global__ __aicore__ void softmax_custom(GM_ADDR x, GM_ADDR y,
    GM_ADDR workspace, GM_ADDR tiling)
{
    GET_TILING_DATA(tilingData, tiling);

    KernelSoftmax op;
    op.Init(x, y, tilingData.totalLength, tilingData.rowsPerCore,
        tilingData.tailRows, tilingData.softmaxTilingData);
    op.Process();
}
```

Kernel侧需要关注两点：

- 读取的字段名称要和Host侧`TilingData`定义一致。
- 传给高阶API的Tiling字段要和Host侧调用的配套Tiling函数一致。

如果主核和尾核使用不同计算shape，Host侧可以只保存主核通用Tiling，尾核在Kernel侧根据尾部shape重新计算；也可以在`TilingData`中保存两份高阶APITiling。选择哪种方式取决于高阶API是否支持Kernel侧重新计算，以及尾核路径的性能要求。

## 7.使用检查点

实现高阶API配套Tiling后，建议按下面几项检查：

- 高阶API文档中要求的Tiling结构已通过`TILING_DATA_FIELD_DEF_STRUCT`加入算子`TilingData`。
- Host侧传给高阶APITiling函数的shape、dtype大小、format和Kernel侧实际处理的数据一致。
- 临时空间大小满足高阶API文档约束，最小空间用于功能验证，性能调优时再评估更大空间。
- 使用宏定义方式的`TilingData`时，已调用`SaveToBuffer`和`SetDataSize`。
- `context->SetBlockDim(...)`设置的`numBlocks`和Kernel侧分核逻辑一致。
- Kernel侧传入高阶API的Tiling字段来源清晰，主核和尾核策略一致。
- 需要不同分支时，已结合[多分支策略](../多分支策略.md)设置`TilingKey`或Kernel模板参数。

## 8.相关文档

- [Host侧Tiling实现基本流程](./基本流程.md)：了解`TilingData`、`numBlocks`、workspace大小和`TilingKey`的基本写法。
- [Kernel侧算子实现](../Kernel侧算子实现/基本流程.md)：了解Kernel侧如何读取`TilingData`并组织计算流程。
- [SoftMax/SimpleSoftMaxTiling](../../../../../../api/SIMD-API/高阶API/激活函数/SoftMax接口/SoftMax-SimpleSoftMax-Tiling.md)：了解SoftMax类高阶API的Tiling接口参数。
- [PadTiling](../../../../../../api/SIMD-API/高阶API/张量变换/Pad-Tiling.md)：了解Pad高阶API的Tiling接口参数。
- [TransposeTiling](../../../../../../api/SIMD-API/高阶API/张量变换/Transpose-Tiling.md)：了解Transpose高阶API的Tiling接口参数。
