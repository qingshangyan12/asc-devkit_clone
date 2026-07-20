# 使用高阶API时配套的Tiling实现

Matmul、Conv等高阶API通常提供配套的Host侧Tiling类，用于根据shape、dtype和平台信息计算切分参数。开发者需要把高阶Tiling类的计算结果传到Kernel侧，并使用同一份数据初始化高阶API对象。

本文构造一个简化的aclnn工程化算子MatmulCustom作为示例。该算子只计算`C = A * B`，用于说明`MultiCoreMatmulTiling`和`TCubeTiling`在算子工程中的接入流程，不包含Bias、激活函数或多分支逻辑。

## 1. 高阶API Tiling的数据流

配套流程如下：

```text
输入shape、dtype、平台信息
    -> Host侧高阶Tiling类计算
    -> 写入算子TilingData
    -> 传递到Kernel侧
    -> 初始化高阶API对象并计算
```

高阶Tiling类负责计算API内部使用的多核切分和核内切分参数，算子TilingData负责把计算结果传到Kernel侧。

## 2. 定义包含TCubeTiling的TilingData

高阶API的Tiling结构体可以通过标准C++方式或宏定义方式组织。本例为了展示`TILING_DATA_FIELD_DEF_STRUCT`的使用方法，选择宏定义方式嵌套`TCubeTiling`：

```cpp
#ifndef MATMUL_CUSTOM_TILING_H
#define MATMUL_CUSTOM_TILING_H
#include "register/tilingdata_base.h"
#include "tiling/tiling_api.h"

namespace optiling {
BEGIN_TILING_DATA_DEF(MatmulCustomTilingData)
    TILING_DATA_FIELD_DEF_STRUCT(TCubeTiling, cubeTilingData);
END_TILING_DATA_DEF;

REGISTER_TILING_DATA_CLASS(MatmulCustom, MatmulCustomTilingData)
} // namespace optiling
#endif // MATMUL_CUSTOM_TILING_H
```

`TILING_DATA_FIELD_DEF_STRUCT`只能嵌套由同类宏定义的Tiling结构体，因此选择该宏时，外层`MatmulCustomTilingData`也需要使用宏定义方式。使用标准C++方式时，可以在普通结构体中直接声明`AscendC::tiling::TCubeTiling`字段，不使用上述宏。

宏定义方式会生成`SaveToBuffer()`和`GetDataSize()`等接口，Host侧计算完成后需要显式序列化TilingData。

## 3. 在TilingFunc中调用高阶Tiling类

本例固定A为`[512, 128]`、B为`[128, 512]`，输出C为`[512, 512]`。A、B使用`float16`，C使用`float`，矩阵均为ND格式。

```cpp
#include "../../op_kernel/matmul_custom/matmul_custom_tiling.h"
#include "register/op_def_registry.h"
#include "tiling/tiling_api.h"

namespace optiling {
static ge::graphStatus TilingFunc(gert::TilingContext* context)
{
    constexpr int32_t M = 512;
    constexpr int32_t N = 512;
    constexpr int32_t K = 128;
    constexpr uint32_t NUM_BLOCKS = 2;

    auto platform =
        platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
    matmul_tiling::MultiCoreMatmulTiling matmulTiling(platform);

    matmulTiling.SetDim(NUM_BLOCKS);
    matmulTiling.SetAType(
        matmul_tiling::TPosition::GM,
        matmul_tiling::CubeFormat::ND,
        matmul_tiling::DataType::DT_FLOAT16,
        false);
    matmulTiling.SetBType(
        matmul_tiling::TPosition::GM,
        matmul_tiling::CubeFormat::ND,
        matmul_tiling::DataType::DT_FLOAT16,
        false);
    matmulTiling.SetCType(
        matmul_tiling::TPosition::GM,
        matmul_tiling::CubeFormat::ND,
        matmul_tiling::DataType::DT_FLOAT);
    matmulTiling.SetOrgShape(M, N, K);
    matmulTiling.SetShape(M, N, K);
    matmulTiling.EnableBias(false);
    matmulTiling.SetBufferSpace(-1, -1, -1);

    MatmulCustomTilingData tiling;
    if (matmulTiling.GetTiling(tiling.cubeTilingData) == -1) {
        return ge::GRAPH_FAILED;
    }
    int32_t usedCoreNum = tiling.cubeTilingData.get_usedCoreNum();
    if (usedCoreNum <= 0) {
        return ge::GRAPH_FAILED;
    }

    tiling.SaveToBuffer(context->GetRawTilingData()->GetData(),
                        context->GetRawTilingData()->GetCapacity());
    context->GetRawTilingData()->SetDataSize(tiling.GetDataSize());

    context->SetBlockDim(static_cast<uint32_t>(usedCoreNum));

    size_t userWorkspaceSize = 0;
    size_t systemWorkspaceSize =
        static_cast<size_t>(platform.GetLibApiWorkSpaceSize());
    size_t* currentWorkspace = context->GetWorkspaceSizes(1);
    currentWorkspace[0] = systemWorkspaceSize + userWorkspaceSize;
    return ge::GRAPH_SUCCESS;
}
} // namespace optiling
```

这段代码包含五个主要步骤：

1. 使用平台信息初始化`PlatformAscendC`和`MultiCoreMatmulTiling`。
2. 配置A、B、C的存储位置、format和dtype。
3. 配置矩阵shape、核数、Bias开关和buffer空间。
4. 调用`GetTiling`生成`TCubeTiling`，并序列化到框架提供的buffer。
5. 根据Tiling结果设置Kernel启动Block数量，并设置workspace大小。

示例使用固定shape以突出高阶Tiling接口。支持动态shape时，应从`TilingContext`读取`M`、`N`、`K`，并在调用`GetTiling`前校验rank、K维相等关系以及整数转换范围。

### 3.1 设置workspace

Matmul高阶API可能需要系统workspace。Host侧通过`GetLibApiWorkSpaceSize()`获取系统workspace大小，并与算子自定义workspace相加：

```cpp
size_t userWorkspaceSize = 0;
size_t systemWorkspaceSize =
    static_cast<size_t>(platform.GetLibApiWorkSpaceSize());
size_t* currentWorkspace = context->GetWorkspaceSizes(1);
currentWorkspace[0] = systemWorkspaceSize + userWorkspaceSize;
```

即使算子没有自定义临时数据，也需要检查高阶API是否使用系统workspace，不能直接把workspace设为0。

### 3.2 校验输入条件

实际算子还需要在调用`GetTiling`前校验：

- A、B是否为二维矩阵。
- A的K维与B的K维是否相等。
- 输入dtype和format是否与`SetAType`、`SetBType`配置一致。
- 输出shape、dtype和format是否与`SetCType`配置一致。
- `M`、`N`、`K`转换到`int32_t`前是否超出范围。

校验失败时返回`ge::GRAPH_FAILED`，不要继续使用无效shape计算Tiling。

## 4. Kernel侧使用TCubeTiling

Kernel入口解析TilingData，并用其中的`TCubeTiling`注册Matmul对象：

```cpp
#include "kernel_operator.h"
#define ASCENDC_CUBE_ONLY
#include "lib/matmul_intf.h"
#include "matmul_custom_tiling.h"

using namespace matmul;

extern "C" __global__ __aicore__ void matmul_custom(
    GM_ADDR a, GM_ADDR b, GM_ADDR c,
    GM_ADDR workspace, GM_ADDR tiling)
{
    GET_TILING_DATA(tilingData, tiling);

    AscendC::TPipe pipe;
    AscendC::GlobalTensor<half> aGlobal;
    AscendC::GlobalTensor<half> bGlobal;
    AscendC::GlobalTensor<float> cGlobal;
    aGlobal.SetGlobalBuffer(
        reinterpret_cast<__gm__ half*>(a),
        tilingData.cubeTilingData.M * tilingData.cubeTilingData.Ka);
    bGlobal.SetGlobalBuffer(
        reinterpret_cast<__gm__ half*>(b),
        tilingData.cubeTilingData.Kb * tilingData.cubeTilingData.N);
    cGlobal.SetGlobalBuffer(
        reinterpret_cast<__gm__ float*>(c),
        tilingData.cubeTilingData.M * tilingData.cubeTilingData.N);

    Matmul<
        MatmulType<AscendC::TPosition::GM, CubeFormat::ND, half>,
        MatmulType<AscendC::TPosition::GM, CubeFormat::ND, half>,
        MatmulType<AscendC::TPosition::GM, CubeFormat::ND, float>> mm;

    REGIST_MATMUL_OBJ(
        &pipe, GetSysWorkSpacePtr(), mm,
        &tilingData.cubeTilingData);

    mm.SetOrgShape(
        tilingData.cubeTilingData.M,
        tilingData.cubeTilingData.N,
        tilingData.cubeTilingData.Ka,
        tilingData.cubeTilingData.Kb);

    uint64_t blockIdx = static_cast<uint64_t>(AscendC::GetBlockIdx());
    uint64_t aOffset =
        blockIdx * tilingData.cubeTilingData.singleCoreM *
        tilingData.cubeTilingData.Ka;
    uint64_t cOffset =
        blockIdx * tilingData.cubeTilingData.singleCoreM *
        tilingData.cubeTilingData.N;
    mm.SetTensorA(aGlobal[aOffset], false);
    mm.SetTensorB(bGlobal[0], false);
    mm.IterateAll(cGlobal[cOffset]);
    mm.End();
}
```

Host侧`SetAType`、`SetBType`和`SetCType`配置的位置、format、dtype，需要与Kernel侧的`MatmulType`模板参数保持一致。本例按M轴分核：不同Block读取A的不同行，复用完整B矩阵，并把结果写入C的对应行。

## 5. 常用配置接口

| 接口 | 作用 |
|---|---|
| `SetDim` | 设置高阶Tiling计算可使用的核数 |
| `SetAType`、`SetBType`、`SetCType` | 设置矩阵位置、format、dtype和转置信息 |
| `SetShape` | 设置本次计算的`M`、`N`、`K` |
| `SetOrgShape` | 设置原始shape，用于存在padding等场景 |
| `EnableBias` | 设置Matmul计算是否包含Bias |
| `SetFixSplit` | 设置固定切分参数 |
| `SetBufferSpace` | 设置buffer空间约束 |
| `GetTiling` | 计算并输出`TCubeTiling`，失败时返回`-1` |

不同高阶API的必选配置、支持dtype和workspace要求不同。本文只展示工程接入方式，具体参数以对应API接口约束为准。

## 6. 相关文档

- [Host侧Tiling实现](./基本流程.md)：普通TilingData和Tiling函数的基本流程。
