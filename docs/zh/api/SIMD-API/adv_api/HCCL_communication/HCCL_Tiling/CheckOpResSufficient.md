# CheckOpResSufficient

## 功能说明

判断当前选择出来的算法资源是否足够，不满足时返回错误。

## 函数原型

```
HcclResult __attribute__((visibility("default"))) CheckOpResSufficient(HcclComm comm, void* stream, void* mc2Tiling)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| comm | 输入 | HCCL的通信域句柄，标识一个通信域（communicator），即一组参与集合通信的进程/rank的上下文。它承载了rank数量、本端rankId、通信域名、拓扑信息、已申请的CCL buffer/CcuIns/channel/engine ctx等资源。 |
| stream | 输入 | aclrtStream运行时流。非空校验通过后，CPU/AICPU_TS引擎时会使用它调用HcclThreadAcquireWithStream绑定线程与流。 |
| mc2Tiling | 输入 | MC2编译器产出的总tiling头。 |

## 返回值说明

返回HcclResult类型的值：

-   0（HCCL_SUCCESS）表示资源充足。
-   非0表示报错，其中资源不足时返回HCCL_E_RES_NOT_SUFFICIENT。

## 约束说明

-   只支持ccu通信引擎。
-   仅支持rank数量大于1的场景。

## 调用示例

```
extern "C" void NnopbaseGetTilingData(void *exector, void **tilingData, uint64_t *dataLen);
extern "C" HcclResult __attribute__((visibility("default"))) CheckOpResSufficient(
    HcclComm comm, void* stream, void* mc2Tiling);

int funcXXX() {
    // ...
    void *mc2Tiling = nullptr;
    uint64_t tilingLen = 0;
    NnopbaseGetTilingData(exector, &mc2Tiling, &tilingLen);
    HcclResult checkRet = CheckOpResSufficient(args.hcclcomm, args.stream, mc2Tiling);
    if (checkRet != HCCL_SUCCESS) {
        // 资源不足，错误处理
        ...
    }
    return 0;
}
```
