# asc_set_deq_scale

## AI处理器支持情况

|AI处理器类型|是否支持|
| :------------ | :------------: |
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

## 功能说明

设置DEQSCALE（量化参数）寄存器的值。

## 函数原型

  ```cpp
    __aicore__ inline void asc_set_deq_scale(const asc_deq_scale_config& config)
    __aicore__ inline void asc_set_deq_scale(__ubuf__ uint64_t* config)
  ```

## 参数说明

|参数名|输入/输出|描述|
| ------------ | ------------ | ------------ |
|config|输入|量化参数。<br/> - asc_deq_scale_config参数：用于非向量模式的量化。详细说明请参考[asc_deq_scale_config](../struct/asc_deq_scale_config.md)。<br/> - uint64_t*参数：量化系数的地址。|

## 返回值说明

无

## 流水类型

PIPE_V

## 约束说明

无

## 调用示例

```cpp
uint32_t src_size = 256;
__ubuf__ half src[src_size];
__ubuf__ half dst[src_size];

asc_deq_scale_config config;
config.scale = 1;
config.offset = 0;
config.sign_mode = 1;
asc_set_deq_scale(config);
```
