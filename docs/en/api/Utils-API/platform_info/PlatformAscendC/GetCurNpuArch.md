# GetCurNpuArch<a name="ZH-CN_TOPIC_0000002543243123"></a>

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:07:37.111Z pushedAt=2026-10-23T07:01:40.539Z nl=2 -->

## Description<a name="zh-cn_topic_0000001664705472_zh-cn_topic_0000001442758437_section36583473819"></a>

Obtains the chip architecture version number of the current hardware platform.

## Prototype<a name="zh-cn_topic_0000001664705472_zh-cn_topic_0000001442758437_section13230182415108"></a>

```
NpuArch GetCurNpuArch(void) const
```

## Parameters<a name="zh-cn_topic_0000001664705472_zh-cn_topic_0000001442758437_section189014013619"></a>

None

## Return Value<a name="zh-cn_topic_0000001664705472_zh-cn_topic_0000001442758437_section25791320141317"></a>

Returns the enumeration class of the current hardware platform architecture number. For the mapping between this enumeration class and AI processor models, see the soc_spec.h header file in the include/platform directory under the CANN software installation path.

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: DAV\_3510
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: DAV\_2201
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: DAV\_2201
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: DAV\_3002
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: DAV\_2002
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: DAV\_2002
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: DAV\_1001
<!-- end id7 -->

## Constraints<a name="zh-cn_topic_0000001664705472_zh-cn_topic_0000001442758437_section19165124931511"></a>

None

## Examples<a name="zh-cn_topic_0000001664705472_zh-cn_topic_0000001442758437_section320753512363"></a>

### Custom Operator Project

```cpp
ge::graphStatus TilingXXX(gert::TilingContext* context) {
    auto ascendcPlatform = platform_ascendc::PlatformAscendC(context->GetPlatformInfo());
    auto npuArch = ascendcPlatform.GetCurNpuArch();
    // Design the tiling strategy based on the obtained version model.
    // Replace DAV_XXX with the actual architecture number.
    if (socVersion == NpuArch::DAV_XXXX) {
        // ...
    }
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
    auto npuArch = ascendcPlatform->GetCurNpuArch();
    // ... Design the tiling strategy based on npuArch.
}
```
