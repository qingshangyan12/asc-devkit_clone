# asc_set_l0c_copy_config

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持

## 功能说明

配置L0C搬出过程的ReLU预处理地址、量化预处理地址和unit flag清除开关。

## 函数原型

```cpp
__aicore__ inline void asc_set_l0c_copy_config(
    uint64_t relu_pre_addr, uint64_t quant_pre_addr, bool is_clean_unit_flag)
```

## 参数说明

|参数名|输入/输出|描述|
|---|---|---|
|relu_pre_addr|输入|ReLU预处理参数地址。|
|quant_pre_addr|输入|量化预处理参数地址。|
|is_clean_unit_flag|输入|是否清除unit flag。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 在调用使用随路ReLU预处理、量化预处理或UnitFlag的L0C搬出接口前完成配置。
- `relu_pre_addr`和`quant_pre_addr`为相应预处理参数的地址；未使用对应功能时可设置为0。

## 调用示例

```cpp
asc_set_l0c_copy_config(0, 0, true);
```
