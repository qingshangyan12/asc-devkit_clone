# asc_icache_preload

## AI处理器支持情况

| AI处理器类型 | 是否支持  |
| :-----------| :------: |
| Ascend 910C |    √     |
| Ascend 910B |    √     |

## 功能说明

从指令所在DDR地址（可以为UB或GM）预加载数据到对应的cacheline中。

## 函数原型

```cpp
__aicore__ inline void asc_icache_preload(__gm__ uint64_t *address, int16_t offset);

__aicore__ inline void asc_icache_preload(uint64_t *address, int16_t offset);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| address | 输入| 加载数据的地址。 |
| offset | 输入 | 加载数据的长度。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
asc_icache_preload();
```
