# SatMode<a name="ZH-CN_TOPIC_0000001956862301"></a>

<!-- md-trans-meta sourceCommit=6b81d2e6322744fc9adc8665d94bfafab36b3550 translatedAt=2026-09-23T15:36:11.845Z pushedAt=2026-10-26T07:41:44.436Z nl=2 -->

| Parameter | Description |
| :-- | :-- | 
| SatMode | Used to set the saturation and non-saturation modes for Cast type conversion.<br>enum class SatMode { <br>    UNKNOWN = -1, <br>    NO_SAT, <br>    SAT <br>};<br>&bull;&nbsp;&nbsp;UNKNOWN: Select this mode when the conversion type does not support SatMode.<br>&bull;&nbsp;&nbsp;NO_SAT: Non-saturation mode.<br>&bull;&nbsp;&nbsp;SAT: Saturation mode. |

```cpp
enum class SatMode {
    UNKNOWN = -1,
    NO_SAT,
    SAT
};
```
