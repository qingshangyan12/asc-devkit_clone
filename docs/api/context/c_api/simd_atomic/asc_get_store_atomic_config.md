# asc_get_store_atomic_config

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

## 功能说明

获取原子操作使能位与原子操作类型的值。

- 原子操作使能位对应获取值的bit[2:0]。

- 原子操作类型对应获取值的bit[4:3]

## 函数原型

```c++
__aicore__ inline int64_t asc_get_store_atomic_config()
```

## 参数说明

无

## 返回值说明

获取原子操作使能位与原子操作类型的值，数据类型为int64_t。

- 原子操作使能位取值如下：
  
  0：无原子操作。

  1：使能原子操作，进行原子操作的数据类型为float。
  
  2：使能原子操作，进行原子操作的数据类型为half。
  
  3：使能原子操作，进行原子操作的数据类型为int16_t。
  
  4：使能原子操作，进行原子操作的数据类型为int32_t。
  
  5：使能原子操作，进行原子操作的数据类型为int8_t。
  
  6：使能原子操作，进行原子操作的数据类型为bfloat16_t。

  其余值无具体含义。

- 原子操作类型取值如下：

  0：求和操作。

  其余值无具体含义。

## 流水类型

PIPE_TYPE_S

## 约束说明

无

## 调用示例

```c++
int64_t value = asc_get_store_atomic_config();
uint16_t atomic_type = static_cast<uint64_t>(value) & 0x7;    // 原子操作使能位
uint16_t atomic_op = (static_cast<uint64_t>(value) >> 4) & 0x3;    // 原子操作类型
```
