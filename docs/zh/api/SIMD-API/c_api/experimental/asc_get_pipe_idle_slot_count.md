# asc_get_pipe_idle_slot_count

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持。
<!-- end id1 -->

## 头文件/库文件

头文件路径为：`"c_api/experimental/asc_get_pipe_idle_slot_count.h"`。

## 功能说明

获取指定流水线对应发射队列（Issue Queue）当前的空闲槽位数量。空闲槽位表示发射队列还能接收的待发射指令数量，可用于观察队列占用情况。

本组接口包含以下六个函数，各函数查询的流水线和适用Core类型如下。

**表 1**  接口与流水线对应关系

| 函数 | 查询的流水线 | 适用Core类型 |
| --- | --- | --- |
| `asc_get_pipe_mte2_idle_slot_count` | PIPE_MTE2 | AIC、AIV |
| `asc_get_pipe_mte3_idle_slot_count` | PIPE_MTE3 | AIC、AIV |
| `asc_get_pipe_v_idle_slot_count` | PIPE_V | AIV |
| `asc_get_pipe_m_idle_slot_count` | PIPE_M | AIC |
| `asc_get_pipe_mte1_idle_slot_count` | PIPE_MTE1 | AIC |
| `asc_get_pipe_fix_idle_slot_count` | PIPE_FIX | AIC |

## 函数原型

- 获取PIPE_MTE2发射队列的空闲槽位数量。

  ```cpp
  __aicore__ inline uint8_t asc_get_pipe_mte2_idle_slot_count()
  ```

- 获取PIPE_MTE3发射队列的空闲槽位数量。

  ```cpp
  __aicore__ inline uint8_t asc_get_pipe_mte3_idle_slot_count()
  ```

- 获取PIPE_V发射队列的空闲槽位数量。

  ```cpp
  __aicore__ inline uint8_t asc_get_pipe_v_idle_slot_count()
  ```

- 获取PIPE_M发射队列的空闲槽位数量。

  ```cpp
  __aicore__ inline uint8_t asc_get_pipe_m_idle_slot_count()
  ```

- 获取PIPE_MTE1发射队列的空闲槽位数量。

  ```cpp
  __aicore__ inline uint8_t asc_get_pipe_mte1_idle_slot_count()
  ```

- 获取PIPE_FIX发射队列的空闲槽位数量。

  ```cpp
  __aicore__ inline uint8_t asc_get_pipe_fix_idle_slot_count()
  ```

## 参数说明

无

## 返回值说明

返回对应发射队列当前的空闲槽位数量，数据类型为`uint8_t`。

## 流水类型

PIPE_S。

## 约束说明

- 本组接口为实验接口，接口名称、功能和行为可能在后续版本中变更或删除。使用前需要通过编译选项`-DCANN_ASC_USE_EXPERIMENTAL`开启实验特性。
- 使用本组接口前，需要通过`#include "c_api/experimental/asc_get_pipe_idle_slot_count.h"`包含头文件。
- 返回值仅表示调用接口时发射队列的瞬时状态，可能随指令发射和执行发生变化。
- 本组接口不提供流水同步语义，不能替代同步接口判断指定流水线中的指令是否执行完成。
- 各接口仅支持在功能说明表格所列的Core类型上调用。在不支持的Core类型上调用时返回0，此时无法区分发射队列没有空闲槽位和当前Core类型不支持该接口两种情况。

## 调用示例

以下示例查询PIPE_MTE2发射队列是否存在空闲槽位。返回值大于0表示至少还有一个空闲槽位，PIPE_MTE2同时适用于AIC和AIV。

```cpp
#include "c_api/experimental/asc_get_pipe_idle_slot_count.h"

__aicore__ inline bool has_mte2_idle_slot()
{
    return asc_get_pipe_mte2_idle_slot_count() > 0;
}
```
