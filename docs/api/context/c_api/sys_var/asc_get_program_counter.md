# asc_get_program_counter

## AI处理器支持情况

| AI处理器类型 | 是否支持 |
| :-----------| :------: |
| Ascend 910C |    √    |
| Ascend 910B |    √    |

## 功能说明

获取程序计数器的指针，程序计数器用于记录当前程序执行的位置。

## 函数原型

```cpp
__aicore__ inline int64_t asc_get_program_counter()
```

## 参数说明

无

## 返回值说明

返回int64_t类型的程序计数器指针。

## 流水类型

PIPE_TYPE_S

## 约束说明

无

## 调用示例

```cpp
asc_get_program_counter();
```