# asc_lt

## AI处理器支持情况

| AI处理器类型         | 是否支持 |
| :-----------------------| :-----:|
| <term>Ascend 910C</term> | √ |
| <term>Ascend 910B</term> | √ |

### 功能说明

按元素判断src0 < src1是否成立，若成立则输出结果为1，否则为0。每个元素的比较结果占1个bit。
当结果输出到寄存器中时，可在计算完成后调用[asc_get_cmp_mask](asc_get_cmp_mask.md)获取比较结果。

### 函数原型


- 高维切分计算

  ```cpp
  //结果存入寄存器
  __aicore__ inline void asc_lt(__ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
  __aicore__ inline void asc_lt(__ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
  // 结果输出到目标地址中
  __aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
  __aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
  ```

- 同步计算

  ```cpp
  //结果存入寄存器
  __aicore__ inline void asc_lt_sync(__ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
  __aicore__ inline void asc_lt_sync(__ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
  // 结果输出到目标地址中
  __aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, const asc_binary_config& config)
  __aicore__ inline void asc_lt(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, const asc_binary_config& config)
  ```

### 参数说明

| 参数名  | 输入/输出 | 描述 |
| ------------ | ------------ | ------------ |
| dst | 输出 | 目的操作数（矢量）的起始地址。 |
| src0 | 输入 | 源操作数（矢量）的起始地址。 |
| src1 | 输入 | 源操作数（矢量）的起始地址。 |
| config | 输入 | 在高维切分计算场景下使用的计算配置参数, 详细说明请参考[asc_binary_config](../struct/asc_binary_config.md)。|

### 返回值说明

无

### 流水类型

PIPE_TYPE_V

### 约束说明

- 操作数地址重叠约束请参考[通用地址重叠约束](../general_instruction.md#通用地址重叠约束)。
- dst、src的起始地址需要32字节对齐。
- dst或输出到寄存器中的结果均按照小端顺序排序成二进制结果，其中的每个bit对应src中相应位置的元素的比较结果。
- 当结果输出到寄存器中时，固定处理256B的数据，config中的repeat字段不生效。

### 调用示例

```cpp
// 结果输出到寄存器中，固定为128个元素
uint64_t offset = 0;
__ubuf__ half* src0 = (__ubuf__ half*)asc_get_phy_buf_addr(offset);
offset += 128 * sizeof(half);
__ubuf__ half* src1 = (__ubuf__ half*)asc_get_phy_buf_addr(offset);
offset += 128 * sizeof(half);
__ubuf__ void* dst = (__ubuf__ void*)asc_get_phy_buf_addr(offset);
…… // 数据搬运及同步操作
asc_lt(src0, src1, CAPI_DEFAULT_BINARY_CFG);
…… // 同步操作
asc_get_cmp_mask(dst); // 读取结果
```



