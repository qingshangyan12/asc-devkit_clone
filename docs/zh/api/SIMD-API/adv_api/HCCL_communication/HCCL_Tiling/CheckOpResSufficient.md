# CheckOpResSufficient

## 功能说明

根据输入的Tiling信息，检查指定的通信算法所需资源是否充足，返回检查结果，资源不足时返回HCCL_E_RES_NOT_SUFFICIENT。

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
| mc2Tiling | 输入 | MC2算子的Tiling信息，通过[GetTiling](GetTiling.md)接口组装到算子TilingData结构体中。 |

## 返回值说明

返回HcclResult类型的值，具体取值参考下表。

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
