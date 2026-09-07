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
<!-- npu="x90" id8 -->
- Kirin X90：不支持
<!-- end id8 -->
<!-- npu="9030" id9 -->
- Kirin 9030：不支持
<!-- end id9 -->

## 功能说明

判断当前选择出来的算法资源是否足够，不满足时返回错误。

## 函数原型

```
extern HcclResult __attribute__((visibility("default"))) CheckOpResSufficient(
    HcclComm comm, void* stream, void* mc2Tiling)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| comm | 输入 | HCCL 的通信域句柄，标识一个通信域（communicator），即一组参与集合通信的进程/rank 的上下文。它承载了 rank 数量、本端 rankId、通信域名、拓扑信息、已申请的 CCL buffer/CcuIns/channel/engine ctx 等资源。 |
| stream | 输入 | aclrtStream 运行时流。非空校验通过后，CPU/AICPU_TS 引擎场景下会使用该流调用 HcclThreadAcquireWithStream 绑定线程与流。 |
| mc2Tiling | 输入 | MC2 编译器产出的总 tiling 头。 |

## 返回值说明

返回HcclResult类型的值：

-   HCCL_SUCCESS（0）：资源充足。
-   HCCL_E_RES_NOT_SUFFICIENT（1043）：算法已选，但资源不足。
-   HCCL_E_PARA（1）：参数格式非法、tiling版本非法、数量非法等。
-   HCCL_E_PTR（2）：空指针错误。
-   HCCL_E_NOT_SUPPORT（5）：通信类型、或者算法路径不支持。
-   HCCL_E_ALG_NOT_SUPPORTED（1042）：算法未注册。

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
