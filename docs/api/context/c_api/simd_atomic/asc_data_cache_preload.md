# asc_data_cache_preload

## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

从源地址所在的特定GM地址预加载数据到Data Cache中。

## 函数原型

```cpp
__aicore__ inline void asc_data_cache_preload(uint64_t* address, int16_t offset)
__aicore__ inline void asc_data_cache_preload(uint64_t* address, int64_t offset)
```

## 参数说明

表1 参数说明

|参数名|输入/输出|描述|
|------------|------------|-----------|
| address     | 输入     | 源操作数的起始地址。   |
| offset     | 输入     | 在源操作数上偏移offset大小开始加载数据，单位为Byte。|

## 返回值说明

无

## 流水类型

PIPE_TYPE_S

## 约束说明

频繁调用此接口可能会导致保留站拥塞，此时，该接口将被是为NOP指令，阻塞Scalar流水。因此不建议频繁调用此接口。

## 调用示例

```cpp
uint64_t* addr;
int64_t offset = 0;
asc_data_cache_preload(addr, offset);
```
