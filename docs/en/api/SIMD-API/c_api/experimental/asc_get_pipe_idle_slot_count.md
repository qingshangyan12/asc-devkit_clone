# Instruction Issue Queue Idle Slot Query

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:58:23.003Z pushedAt=2026-10-28T12:55:49.534Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/experimental/asc_get_pipe_idle_slot_count.h"`.

This API is an experimental API. It may be adjusted or improved in later versions, and backward compatibility is not guaranteed. You are advised to keep track of updates in later versions.

This API is used to obtain the current number of idle slots in the issue queue corresponding to a specified pipeline. An idle slot indicates the number of instructions to be issued that the issue queue can still accept, and can be used to observe queue occupancy. The API includes the following six functions. The pipeline queried by each function, the applicable AI Core, and the maximum idle slot count are as follows:

<a id="table1"></a>
**Table 1**  API, Pipeline, and Maximum Value Correspondence

| Function | Queried Pipeline | Applicable AI Core | Maximum Idle Slot Count |
| --- | --- | --- | --- |
| `asc_get_pipe_mte2_idle_slot_count` | PIPE_MTE2 | AIC, AIV | AIC: 16; AIV: 16 |
| `asc_get_pipe_mte3_idle_slot_count` | PIPE_MTE3 | AIV | 16 |
| `asc_get_pipe_v_idle_slot_count` | PIPE_V | AIV | 32 |
| `asc_get_pipe_m_idle_slot_count` | PIPE_M | AIC | 16 |
| `asc_get_pipe_mte1_idle_slot_count` | PIPE_MTE1 | AIC | 32 |
| `asc_get_pipe_fix_idle_slot_count` | PIPE_FIX | AIC | 32 |

## Prototype

  ```cpp
  // Obtain the number of idle slots in the PIPE_MTE2 issue queue.
  __aicore__ inline uint8_t asc_get_pipe_mte2_idle_slot_count()
  // Obtain the number of idle slots in the PIPE_MTE3 issue queue.
  __aicore__ inline uint8_t asc_get_pipe_mte3_idle_slot_count()
  // Obtain the number of idle slots in the PIPE_V issue queue.
  __aicore__ inline uint8_t asc_get_pipe_v_idle_slot_count()
  // Obtain the number of idle slots in the PIPE_M issue queue.
  __aicore__ inline uint8_t asc_get_pipe_m_idle_slot_count()
  // Obtain the number of idle slots in the PIPE_MTE1 issue queue.
  __aicore__ inline uint8_t asc_get_pipe_mte1_idle_slot_count()
  // Obtain the number of idle slots in the PIPE_FIX issue queue.
  __aicore__ inline uint8_t asc_get_pipe_fix_idle_slot_count()
  ```

## Parameters

None

## Return Value

Returns the current number of idle slots in the corresponding issue queue, with the data type `uint8_t`.

## Pipeline Type

PIPE_S.

## Constraints

- Before use, add the compilation option `-DCANN_ASC_USE_EXPERIMENTAL` to enable the experimental feature.
- Each API supports only the AI Core types listed in [Table 1 API, pipeline, and maximum value mapping](#table1). When called on an unsupported AI Core type, the API returns 0. In this case, it is impossible to distinguish between a situation where the issue queue has no idle slots and a situation where the current AI Core type does not support the API.

## Examples

The following example checks whether the PIPE_MTE2 issue queue has idle slots. When the number of idle slots is greater than 0, `has_mte2_idle_slot` returns `true`; otherwise, it returns `false`.

```cpp
#include "c_api/experimental/asc_get_pipe_idle_slot_count.h"

__aicore__ inline bool has_mte2_idle_slot()
{
    return asc_get_pipe_mte2_idle_slot_count() > 0;
}
```
