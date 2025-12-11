# asc_set_atomic_min

## AI处理器支持情况

|AI处理器类型   | 是否支持 |
| ------------|:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

设置计算结果是否以原子比较的方式传输到GM。在拷贝前，将待传输内容与GM中已有内容进行比较，并将最小值写入GM。

## 函数原型

```cpp
__aicore__ inline void asc_set_atomic_min()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_TYPE_S

## 约束说明

使用结束后，建议通过[asc_set_atomic_none](./asc_set_atomic_none())关闭原子最小操作，以免影响后续相关指令功能。

## 调用示例

```cpp
// dst为目标数据地址，dst0, dst1为源数据地址，total_length指参与计算的数据长度
asc_copy_ub2gm(dst, dst0, total_length);
asc_sync(PIPE_TYPE_MTE3);
asc_set_atomic_min();
asc_copy_ub2gm(dst, dst1, total_length);
asc_set_atomic_none();
```
