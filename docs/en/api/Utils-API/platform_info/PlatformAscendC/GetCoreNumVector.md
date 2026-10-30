# GetCoreNumVector<a name="ZH-CN_TOPIC_0000002114097977"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:06:32.426Z pushedAt=2026-10-23T07:01:40.538Z nl=2 -->

## Description<a name="zh-cn_topic_0000001817714666_zh-cn_topic_0000001442758437_section36583473819"></a>

Obtains the number of Vector Cores independent of the hardware platform.

This API is valid only for Atlas inference products. It returns 0 on all other hardware platform models.

## Prototype<a name="zh-cn_topic_0000001817714666_zh-cn_topic_0000001442758437_section13230182415108"></a>

```
uint32_t GetCoreNumVector(void) const
```

## Parameters<a name="zh-cn_topic_0000001817714666_zh-cn_topic_0000001442758437_section189014013619"></a>

None

## Return Value<a name="zh-cn_topic_0000001817714666_zh-cn_topic_0000001442758437_section25791320141317"></a>

Returns the number of Vector Cores on the hardware platform.

## Constraints<a name="zh-cn_topic_0000001817714666_zh-cn_topic_0000001442758437_section19165124931511"></a>

<!-- npu="910" id1 -->
Atlas training products do not support this API and return 0.
<!-- end id1 -->
<!-- npu="310p" id2 -->
Atlas inference products support this API and return the number of Vector Cores on the hardware platform.
<!-- end id2 -->
<!-- npu="910b" id3 -->
Atlas A2 products do not support this API and return 0.
<!-- end id3 -->
<!-- npu="A3" id4 -->
Atlas A3 products do not support this API and return 0.
<!-- end id4 -->
<!-- npu="310b" id5 -->
Atlas 200I/500 A2 inference products do not support this API and return 0.
<!-- end id5 -->
<!-- npu="950" id6 -->
Ascend 950PR&950DT products do not support this API and return 0.
<!-- end id6 -->

## Examples<a name="zh-cn_topic_0000001817714666_zh-cn_topic_0000001442758437_section320753512363"></a>

### Custom Operator Project

```cpp
ge::graphStatus TilingXXX(gert::TilingContext* context) {
    auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
    auto aivCoreNum = ascendcPlatform.GetCoreNumAiv();
    auto vectorCoreNum = ascendcPlatform.GetCoreNumVector();
    auto allVecCoreNums = aivCoreNum + vectorCoreNum;
    // Split by allVecCoreNums.
    return ret;
}
```

### Kernel Direct Invocation

```cpp
void GetInfoFun()
{
    auto* ascendcPlatform = platform_ascendc::PlatformAscendCManager::GetInstance();
    if (ascendcPlatform == nullptr) {
        return;
    }
    auto aivCoreNum = ascendcPlatform->GetCoreNumAiv();
    auto vectorCoreNum = ascendcPlatform->GetCoreNumVector();
    auto allVecCoreNums = aivCoreNum + vectorCoreNum;
    // ... Design the splitting strategy based on allVecCoreNums.
}
```
