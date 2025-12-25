
# asc_set_fp32_mode

## AI处理器支持情况

| AI处理器类型     | 是否支持 |
| ----------- |:----:|
| Ascend 910C | √    |
| Ascend 910B | √    |

### 功能说明

用于设置Mmad计算开启FP32模式，开启该模式后L0A Buffer/L0B Buffer中的FP32数据在参与Mmad计算之前不做舍入处理。


### 函数原型



 ```cpp
   __aicore__ inline void asc_set_fp32_mode()
```


### 返回值说明

无

### 流水类型

PIPE_TYPE_S

### 约束说明

无

### 调用示例

```cpp
asc_set_fp32_mode();
```