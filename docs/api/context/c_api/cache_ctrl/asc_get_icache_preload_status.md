# asc_get_icache_preload_status

## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

获取ICache的Preload的状态。

## 函数原型

```c++
__aicore__ inline int64_t asc_get_icache_preload_status()
```

## 参数说明

无

## 返回值说明

int64_t类型，0表示空闲，1表示忙。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```c++
int64_t status = asc_get_icache_preload_status();
```
