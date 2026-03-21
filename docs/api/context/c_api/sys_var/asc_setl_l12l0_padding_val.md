# asc_setl_l12l0_padding_val

## 产品支持情况

| 产品 | 是否支持 |
| :-----------| :------: |
| Ascend 950PR/Ascend 950DT |    √     |

## 功能说明

对PADDING_B寄存器中的值进行设置。

## 函数原型

```cpp
__aicore__ inline void asc_setl_l12l0_padding_val(uint64_t config)
```

## 参数说明

|参数名|输入/输出| 描述        |
| :------ | :---  |:----------|
|config   |输入   | 待设置的寄存器值。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例
```cpp
uint64_t config = 123456789;
asc_set_gm2l1_nz_para(config);
```