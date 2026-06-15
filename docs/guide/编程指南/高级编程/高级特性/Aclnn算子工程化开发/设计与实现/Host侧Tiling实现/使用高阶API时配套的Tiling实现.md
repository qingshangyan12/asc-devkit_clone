# 使用高阶 API 时配套的 Tiling 实现

> 类型：扩展

本文档以 MatmulCustom（多核矩阵乘法）算子为例，展示需要高阶 Tiling API（如 MultiCoreMatmulTiling）时的实现方式。该场景下 TilingData **必须使用宏定义方式注册**，以便通过 `TILING_DATA_FIELD_DEF_STRUCT` 嵌套框架预定义的结构体（TCubeTiling）。

## 算子概述

MatmulCustom 算子实现多核矩阵乘法：C = A × B + Bias。

- **输入**：矩阵 A、矩阵 B（可选 Bias）
- **输出**：矩阵 C
- **特点**：使用高阶 API 自动计算 M/N/K 维度的切分策略，支持多核并行
- **适用场景**：Matmul、Conv 等复杂算子的 Tiling 实现

## 高阶 API 的概念

AscendC 框架提供了一系列高阶 Tiling API，用于简化常见算子类型（如 Matmul、Conv 等）的 Tiling 计算。这些 API 封装了复杂的数据切分逻辑，开发者只需设置算子的 Shape、数据类型等基本信息，API 会自动计算出最优的 Tiling 参数。

常见的高阶 Tiling API 类包括：

| 高阶 API 类 | 适用场景 | 说明 |
|------------|---------|------|
| MultiCoreMatmulTiling | Matmul 类算子 | 支持多核 Matmul 切分，自动计算 M/N/K 维度的切分策略 |
| TCubeTiling | Cube 计算相关算子 | 底层 Tiling 数据结构，通常作为其他高阶 API 的输出载体 |
| SingleCoreMatmulTiling | 单核 Matmul 场景 | 适用于小规模 Matmul 的单核计算场景 |

这些高阶 API 类来自 AscendC 框架的头文件（如 `matmul_tiling.h`），通过 `platform_ascendc::PlatformAscendC` 对象进行初始化，并与平台信息绑定。

高阶 API 计算出的 Tiling 数据（如 `TCubeTiling` 结构体）需要嵌入到开发者自定义的 TilingData 结构体中，通过序列化传递到 kernel 侧。kernel 侧再通过高阶 API 提供的宏（如 `REGIST_MATMUL_OBJ`）初始化计算对象。

## 算子原型定义

在 `op_host/matmul_custom.cpp` 中定义算子原型：

```cpp
namespace ops {
class MatmulCustom : public OpDef {
public:
    MatmulCustom(const char* name) : OpDef(name)
    {
        this->Input("a")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT16})
            .Format({ge::FORMAT_ND});
        this->Input("b")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT16})
            .Format({ge::FORMAT_ND});
        this->Input("bias")
            .ParamType(OPTIONAL)
            .DataType({ge::DT_FLOAT})
            .Format({ge::FORMAT_ND});
        this->Output("c")
            .ParamType(REQUIRED)
            .DataType({ge::DT_FLOAT})
            .Format({ge::FORMAT_ND});
        this->SetInferShape(ge::InferShape);
        this->SetInferDataType(ge::InferDataType);
        this->AICore()
            .SetTiling(optiling::TilingFunc);
        this->AICore().AddConfig("ascendxxx");
    }
};
OP_ADD(MatmulCustom);
} // namespace ops
```

关于原型定义的详细说明，请参考[算子原型定义](../算子原型定义.md)。

## 步骤一：定义 Tiling 结构体

Tiling 结构体需要嵌套高阶 API 预定义的结构体（如 `TCubeTiling`）作为成员变量。由于 `TILING_DATA_FIELD_DEF_STRUCT` 宏只能在宏定义方式使用，该场景下 TilingData 必须使用宏定义方式注册。

```cpp
#ifndef MATMUL_CUSTOM_TILING_H
#define MATMUL_CUSTOM_TILING_H
#include "register/tilingdata_base.h"
#include "tiling/tiling_api.h"  // for TCubeTiling

namespace optiling {
BEGIN_TILING_DATA_DEF(MatmulCustomTilingData)  // 声明tiling结构名字
    TILING_DATA_FIELD_DEF(uint64_t, localMemSize);
    TILING_DATA_FIELD_DEF_STRUCT(TCubeTiling, cubeTilingData);   // 引用高阶API的tiling结构体
END_TILING_DATA_DEF;
REGISTER_TILING_DATA_CLASS(MatmulCustom, MatmulCustomTilingData)  // tiling结构注册给算子
} // namespace optiling
#endif // MATMUL_CUSTOM_TILING_H
```

**宏定义方式各宏的含义**：

| 宏名称 | 作用 |
|--------|------|
| `BEGIN_TILING_DATA_DEF(结构体名)` | 开始定义 Tiling 结构体 |
| `TILING_DATA_FIELD_DEF(类型, 字段名)` | 定义基本类型字段（如 uint32_t、float 等） |
| `TILING_DATA_FIELD_DEF_STRUCT(结构体类型, 字段名)` | 定义结构体类型字段（用于嵌套框架预定义的结构体，如 TCubeTiling） |
| `END_TILING_DATA_DEF` | 结束 Tiling 结构体定义 |
| `REGISTER_TILING_DATA_CLASS(算子名, 结构体名)` | 将 Tiling 结构体注册给指定算子 |

## 步骤二：实现 TilingFunc 函数

TilingFunc 中需要完成以下步骤：

1. **初始化高阶 API 对象**：使用 PlatformAscendC 初始化 MultiCoreMatmulTiling 等对象。
2. **设置参数**：通过 SetShape、SetDim、SetAType 等方法设置算子的基本信息。
3. **获取 Tiling 数据**：调用 GetTiling 获取计算结果并写入 TilingData。
4. **SaveToBuffer 序列化**：将 TilingData 序列化到 context。
5. **SetBlockDim 设置核数**：设置参与计算的核数。
6. **GetWorkspaceSizes 设置 workspace**：配置 workspace 大小。

以下示例展示完整的 TilingFunc 实现。实际算子中应从 `context->GetInputShape()` 动态获取 M/N/K（推荐方式）：

```cpp
#include "../op_kernel/matmul_custom_tiling.h"

namespace optiling {
static ge::graphStatus TilingFunc(gert::TilingContext *context)
{
    // 1. 从 context 动态获取 M/N/K（推荐方式）
    auto inputShapeA = context->GetInputShape(0)->GetOriginShape();  // A: [M, K]
    auto inputShapeB = context->GetInputShape(1)->GetOriginShape();  // B: [K, N]
    int32_t M = static_cast<int32_t>(inputShapeA.GetDim(0));
    int32_t K = static_cast<int32_t>(inputShapeA.GetDim(1));
    int32_t N = static_cast<int32_t>(inputShapeB.GetDim(1));
    int32_t baseM = 128;
    int32_t baseN = 128;

    // 2. 初始化高阶API对象
    auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
    MultiCoreMatmulTiling cubeTiling(ascendcPlatform);

    // 3. 设置参数
    cubeTiling.SetDim(2);
    cubeTiling.SetAType(TPosition::GM, CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
    cubeTiling.SetBType(TPosition::GM, CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT16);
    cubeTiling.SetCType(TPosition::LCM, CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
    cubeTiling.SetBiasType(TPosition::GM, CubeFormat::ND, matmul_tiling::DataType::DT_FLOAT);
    cubeTiling.SetShape(M, N, K);
    cubeTiling.SetOrgShape(M, N, K);
    cubeTiling.SetFixSplit(baseM, baseN, -1);  // -1 表示由框架自动推导 K 维度的切分策略
    cubeTiling.SetBias(true);
    cubeTiling.SetBufferSpace(-1, -1, -1);  // -1 表示由框架自动推导各 buffer 空间大小

    // 4. 获取Tiling数据
    MatmulCustomTilingData tiling;
    if (cubeTiling.GetTiling(tiling.cubeTilingData) == -1) {
        return ge::GRAPH_FAILED;
    }

    // 5. 设置其他自定义字段
    uint64_t localMemSize;
    ascendcPlatform.GetCoreMemSize(platform_ascendc::CoreMemType::UB, localMemSize);
    tiling.set_localMemSize(localMemSize);

    // 6. SaveToBuffer 序列化（宏定义方式需要手动序列化，标准 C++ 方式由框架自动处理）
    tiling.SaveToBuffer(context->GetRawTilingData()->GetData(), context->GetRawTilingData()->GetCapacity());
    context->GetRawTilingData()->SetDataSize(tiling.GetDataSize());

    // 7. SetBlockDim设置核数
    context->SetBlockDim(2);

    // 8. GetWorkspaceSizes设置workspace
    size_t *currentWorkspace = context->GetWorkspaceSizes(1);
    currentWorkspace[0] = 0;

    return ge::GRAPH_SUCCESS;
}
} // namespace optiling
```

## 步骤三：Kernel 侧使用高阶 API Tiling 数据

在 kernel 侧，需要通过高阶 API 提供的宏来初始化计算对象，并使用 Tiling 数据中的参数。

```cpp
#include "kernel_operator.h"
#include "matmul_custom_tiling.h"

extern "C" __global__ __aicore__ void matmul_custom(GM_ADDR a, GM_ADDR b, GM_ADDR bias, GM_ADDR c,
                                                     GM_ADDR workspace, GM_ADDR tiling)
{
    GET_TILING_DATA(tilingData, tiling);

    // 使用高阶API的Tiling数据初始化计算对象
    MatmulKernel<half, half, float, float> matmulKernel;
    AscendC::TPipe pipe;
    REGIST_MATMUL_OBJ(&pipe, GetSysWorkSpacePtr(), matmulKernel.matmulObj, &tilingData.cubeTilingData);
    matmulKernel.Init(a, b, bias, c, workspace, tilingData.localMemSize, tilingData.cubeTilingData);

    // 执行计算
    matmulKernel.Process();
}
```

## 高阶 API 常用设置方法说明

| 方法 | 说明 |
|------|------|
| SetDim(int32_t dim) | 设置参与计算的维度（如 2 表示 2D 切分） |
| SetAType / SetBType / SetCType | 设置 A/B/C 矩阵的位置（GM/LCM 等）、格式（ND 等）和数据类型 |
| SetBiasType | 设置 Bias 矩阵的位置、格式和数据类型 |
| SetShape(int32_t M, int32_t N, int32_t K) | 设置 Matmul 的 M/N/K 维度大小 |
| SetOrgShape(int32_t M, int32_t N, int32_t K) | 设置原始 Matmul 的 M/N/K 维度大小（用于 padding 场景） |
| SetFixSplit(int32_t baseM, int32_t baseN, int32_t baseK) | 设置固定的切分策略，-1 表示自动推导 |
| SetBias(bool hasBias) | 设置是否包含 Bias |
| SetBufferSpace(int32_t a, int32_t b, int32_t c) | 设置 Buffer 空间限制，-1 表示自动推导 |
| GetTiling(TCubeTiling &tiling) | 获取计算后的 Tiling 数据，返回 -1 表示失败 |

## 编译与部署

```bash
./build.sh
./custom_opp_<target os>_<target architecture>.run
```

详细说明请参考[编译与部署基本流程](../../编译与部署/基本流程.md)。


## 相关文档

- [算子原型定义](../算子原型定义.md) — 输入输出属性定义
- [基本流程](./基本流程.md) — 基础算子的 Tiling 实现（标准 C++ 方式）
- [通过TilingData传递属性信息](./通过TilingData传递属性信息.md) — 属性传递的原理和端到端示例
- [多分支策略](../多分支策略.md) — TilingKey 和 Kernel 模板的详细说明
