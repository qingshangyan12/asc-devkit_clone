# asc_fill_l1

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

将value的二进制值赋值给dst，对L1 Buffer的Local Memory进行初始化。

## 函数原型

- 常规计算

    ```cpp
    __aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
    ```

- 同步计算

    ```cpp
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, half value,const asc_fill_value_config& config)
    __aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
    ```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| dst | 输出 | 目的操作数（矩阵）的起始地址。 |
| value | 输入 | 源操作数（标量）。 |
| config | 输入 | 使用的初始化相关参数，详细说明请参考[asc_fill_value_config](../struct/asc_fill_value_config.md)。 |

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../general_description_and_constraints.md#通用地址重叠约束)。
- 若dst和value的数据类型不相同，则需要根据dst的数据类型对value的值进行转换，具体请参考调用示例。

<!-- npu="950" id8 -->
### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需32字节对齐（L1 Buffer对齐要求），否则触发地址对齐异常。
- 如果需要执行多条本接口指令，且目的地址存在重叠，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移+填充总大小不可越界，否则触发地址溢出异常。填充总大小=`repeat× (blk_num+dst_gap) ×32字节`。
- value dtype需与dst dtype匹配（按函数原型节dtype组合表），否则触发异常。
- `config`的`reserved1`、`reserved2`、`reserved3`为保留位，填0即可，传入非0值不生效。
- repeat或blk_num为0时不执行填充，本接口被视为NOP（空操作）。
- repeat传入>2^15−1的值会截断处理（如2^15→0、2^15+1→1）。截断后实际生效值偏小，导致实际重复次数小于预期，循环提前结束，部分32字节块未被填充。截断后若值为0，本接口被视为NOP（空操作）。
- blk_num传入>2^15−1的值会截断处理（如2^15→0、2^15+1→1）。截断后实际生效值偏小，导致该次重复内实际填充的32字节块数小于预期，部分位置未被填充。截断后若值为0，本接口被视为NOP（空操作）。
- dst_gap传入>2^15−1的值会截断处理（如2^15→0、2^15+1→1）。截断后实际生效值偏小，导致相邻两次重复之间的间隔小于预期，后一次重复写到错误位置，与前一次重复数据重叠或错位。

<!-- end id8 -->

## 调用示例

```cpp
constexpr uint64_t total_length = 128;    // total_length指参与搬运的数据总长度
asc_fill_value_config config;

// dst和value数据类型不相同
__cbuf__ float dst0[total_length];
uint32_t value0 = 0x3F800000;  // float类型的1的二进制表示为0x3F800000
config.repeat = 1;
config.blk_num = total_length * sizeof(float) / 32;
config.dst_gap = 0;
asc_fill_l1(dst0, value0, config);    // 将dst0中128个float类型的元素初始化为1

// dst和value数据类型相同
__cbuf__ half dst1[total_length];
half value1 = 1;  // 不用做二进制转换，直接填1即可
config.repeat = 1;
config.blk_num = total_length * sizeof(half) / 32;
config.dst_gap = 0;
asc_fill_l1(dst1, value1, config);    // 将dst1中128个half类型的元素初始化为1
```
