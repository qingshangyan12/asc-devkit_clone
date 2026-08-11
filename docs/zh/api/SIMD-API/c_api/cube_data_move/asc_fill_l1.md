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

将一个标量填充值写入L1 Buffer的目的地址，并将填充值广播到多个32字节分形块中，可用于L1 Buffer的内存初始化。需要通过[asc_fill_value_config](../struct/asc_fill_value_config.md)配置的重复次数、单次分形块数、相邻重复之间的间隔。

本接口仅在AIC上执行有效。

## 函数原型

```cpp
__aicore__ inline void asc_fill_l1(__cbuf__ <dst_dtype>* dst,
                                   <value_dtype> value,
                                   const asc_fill_value_config& config)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ <dst_dtype>* dst,
                                        <value_dtype> value,
                                        const asc_fill_value_config& config)
```

### dtype支持的数据类型

dst dtype与value dtype支持以下组合：

- `dst_dtype`为`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`或`float`时，`value_dtype`支持`half`、`uint32_t`。
- `dst_dtype`为`bfloat16_t`时，`value_dtype`支持`half`、`bfloat16_t`、`uint32_t`。

### 函数原型典型示例

```cpp
// 示例：使用half类型的值填充half类型的L1 Buffer。
__aicore__ inline void asc_fill_l1(__cbuf__ half* dst,
                                   half value,
                                   const asc_fill_value_config& config)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `dst` | 输出 | 目的操作数，目的矩阵的起始地址，存储位置为L1 Buffer，目的地址需32字节对齐。 |
| `value` | 输入 | 源操作数，标量填充值。|
| `config` | 输入 | 填充配置的结构体[asc_fill_value_config](../struct/asc_fill_value_config.md)，结构体说明见[表2](#table2)。 |

**表2** config字段说明 <a id="table2"></a>

| 字段名 | 含义 |
|---|---|
| `repeat` | 重复次数，即填充值按`blk_num`个32字节块写入L1 Buffer后，再以`dst_gap`为间隔重复写入的总次数。取值范围：[1，32767]。 |
| `blk_num` | 单次重复写入的32字节块数，即一次重复内连续填充的32字节块个数。取值范围：[1，32767]。 |
| `dst_gap` | 相邻两次重复之间的间隔，单位32字节块。两次重复起始地址偏移 = `blk_num + dst_gap`（单位32字节）。取值范围：[0，32767]。 |

如下图所示，将数据空间内最后三行初始化为0，配置如下参数：value=0表示初始化值为0，repeat=5表示按照行方向迭代5次，blk_num=3表示每次repeat对列方向的3块32Byte数据块清0，dst_gap=5表示相邻repeat之间间隔5个数据块。

**图1** L1 Buffer中Fill示意图<a name="zh-cn_topic_capi_fill_l1"></a>  
![](../../../figures/Fill_L1.png "L1 Buffer中Fill示意图")

## 返回值说明

无

## 流水类型

PIPE_MTE2

## 约束说明

- 若dst和value的数据类型不相同，则需要根据dst的数据类型对value的值进行转换，具体请参考调用示例。

<!-- npu="950" id8 -->
### 通用约束

- 本接口非AIC调用直接返回。
- dst起始地址需32字节对齐（L1 Buffer对齐要求），否则触发地址对齐异常。
- 如果本指令与其他指令存在目的地址重叠，需要插入同步指令（[asc_sync_notify](../sync/asc_sync_notify.md)和[asc_sync_wait](../sync/asc_sync_wait.md)），保证多个指令串行化，防止出现异常数据。
- L1 Buffer容量上限：L1 Buffer总容量512KB，dst偏移量与填充总大小之和不可越界，否则触发地址溢出异常。填充总大小=`repeat× (blk_num+dst_gap) ×32字节`。
- value dtype需与dst dtype匹配（按函数原型节dtype组合表），若dst和value的数据类型不相同，则需要根据dst的数据类型对value的值进行转换，具体请参考调用示例。

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
