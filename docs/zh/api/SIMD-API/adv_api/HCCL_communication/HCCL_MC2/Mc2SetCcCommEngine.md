# Mc2SetCcCommEngine

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas训练系列产品：不支持
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/adv_api/HCCL_communication/HCCL_MC2/Mc2SetCcCommEngine_res.md#id1 -->

## 功能说明

设置MC2通信任务使用的通信引擎，并自动更新参数对象中的launch版本。

## 函数原型

```
extern Mc2Result __attribute__((visibility("default"))) Mc2SetCcCommEngine(void* ccArgs, uint8_t commEngine)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| ccArgs | 输入 | [Mc2GetCcArgs](Mc2GetCcArgs.md)返回的MC2通信参数对象指针。 |
| commEngine | 输入 | 通信引擎。取值如下：2表示AICPU_TS；5表示CCU_MS；6表示CCU_SCHED。 |

## 返回值说明

-   0表示设置成功。
-   非0表示设置失败，例如参数为空指针或通信引擎不支持。

## 约束说明

-   AICPU路径使用AICPU_TS。
-   CCU路径使用CCU_MS或CCU_SCHED，二者均会走CCU资源申请路径，实际使用时建议优先使用CCU_SCHED。
-   当前仅支持上述三个通信引擎取值，其他取值返回错误。

## 调用示例

```
constexpr uint8_t kCcuSchedEngine = 6U; // CCU_SCHED
Mc2Result ret = Mc2SetCcCommEngine(ccArgs, kCcuSchedEngine);
```

AICPU路径示例：

```
constexpr uint8_t kAicpuTsEngine = 2U; // AICPU_TS
Mc2Result ret = Mc2SetCcCommEngine(ccArgs, kAicpuTsEngine);
```
