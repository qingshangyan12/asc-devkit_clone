# CheckOpResSufficient

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

检查MC2算子选定的通信算法所需的资源是否充足。

## 函数原型

```
extern HcclResult __attribute__((visibility("default"))) CheckOpResSufficient(
    HcclComm comm, void* stream, void* mc2Tiling)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| comm | 输入 | 初始化后的通信域指针。<br>HcclComm类型的定义可参见[HcclComm](https://gitcode.com/cann/hcomm/blob/master/docs/zh/api_ref/comm_mgr_c/data_type_definition/HcclComm.md)。 |
| stream | 输入 | stream类型为aclrtStream，用于维护一些异步操作的执行顺序，确保按照应用程序中的代码调用顺序在Device上执行。stream创建等管理接口请参考[《Runtime运行时API》](https://hiascend.com/document/redirect/CannCommunityRuntimeApi)。 |
| mc2Tiling | 输入 | MC2算子的tiling配置数据，通过[GetTiling](GetTiling.md)接口组装到算子TilingData结构体中。 |

## 返回值说明

返回HcclResult类型的值：

**表2**  返回值说明

| 返回值 | 描述 |
| --- | --- |
| HCCL_SUCCESS | 资源充足。 |
| HCCL_E_PARA | 参数格式非法，tiling版本、数量非法等。 |
| HCCL_E_PTR | 空指针错误。 |
| HCCL_E_NOT_SUPPORT | 通信类型、或者算法路径不支持。 |
| HCCL_E_ALG_NOT_SUPPORTED | 算法未注册。 |
| HCCL_E_RES_NOT_SUFFICIENT | 算法已选，但资源不足。 |

## 约束说明

-   仅支持ccu通信引擎。
-   仅支持rank数量大于1的场景。

## 调用示例

```
#include "adv_api/hccl/hccl_mc2.h"

extern "C" void NnopbaseGetTilingData(void *executor, void **tilingData, uint64_t *dataLen);

struct Args {
    uint32_t rankId;
    HcclComm hcclcomm;
    aclrtStream stream;
    aclrtContext context;
};

int func(Args &args) {
    ...
    aclOpExecutor *executor = nullptr;
    void *mc2Tiling = nullptr;
    uint64_t tilingLen = 0;
    NnopbaseGetTilingData(executor, &mc2Tiling, &tilingLen);
    ...
    HcclResult checkRet = CheckOpResSufficient(args.hcclcomm, args.stream, mc2Tiling);
    if (checkRet == HCCL_E_RES_NOT_SUFFICIENT) {
        // 资源不足处理
        ...
    } else {
        // 其他错误，错误处理
        ...
    }
    return 0;
}
```
