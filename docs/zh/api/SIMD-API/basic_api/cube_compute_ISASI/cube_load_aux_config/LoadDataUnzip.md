# LoadDataUnzip<a name="ZH-CN_TOPIC_0000001913015096"></a>

## 产品支持情况<a name="section1550532418810"></a>

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMD-API/basic_api/cube_compute_ISASI/cube_load_aux_config/LoadDataUnzip_res.md#id1 -->

## 功能说明<a name="section618mcpsimp"></a>

将GM上的数据解压并搬运到L1 Buffer（A1/B1）或L0B Buffer（B2）上。执行该API前需要执行[LoadUnzipIndex](LoadUnzipIndex.md)加载压缩索引表。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <typename T>
__aicore__ inline void LoadDataUnzip(const LocalTensor<T>& dst, const GlobalTensor<T>& src)
```

**裸指针形（与上面的 tensor 形逐项对应）：**

```cpp
template <typename DstPtr, typename SrcPtr, typename Std::enable_if<CubePtrOverload::AnyPtr<DstPtr, SrcPtr>::value, bool>::type = true>
__aicore__ inline void LoadDataUnzip(DstPtr dst, SrcPtr src)
```

> 上述为裸指针形，参数含义与上面同名的 tensor 形逐项对应，区别仅在于实参直接传地址而不是 tensor；操作数的地址空间由指针实参本身区分。

## 参数说明<a name="section622mcpsimp"></a>

**表1** 参数说明

| 参数名称 | 输入/输出 | 含义 |
| ------ | ------ | ------ |
| dst | 输出 | 目的操作数，类型为LocalTensor，支持的TPosition为A1/B1/B2。<br>LocalTensor的起始地址需要保证：<br>&nbsp;&nbsp;&bull; TPosition为A1/B1时，32字节对齐。<br>&nbsp;&nbsp;&bull; TPosition为B2时，512B对齐。 |
| src | 输入 | 源操作数，类型为GlobalTensor。数据类型需要与dst保持一致。 |

裸指针形的 `dst`、`src` 分别与上表同名 tensor 形参语义一致，实参为对应地址空间的裸指针。

## 数据类型

支持的数据类型为：int8_t。

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- 操作数地址对齐要求请参见[通用地址对齐约束](../../../general_description_and_constraints.md#section796754519912)。
- 裸指针形下，各操作数的地址空间须与原 tensor 形一致：`dst` 为 `__cbuf__`（A1/B1）、`__ca__`（L0A Buffer）或 `__cb__`（L0B Buffer），`src` 为 `__gm__`（GM）。元素类型由指针实参推导。

## 调用示例<a name="section6461234123118"></a>

该调用示例支持的运行平台为Atlas推理系列产品AI Core。

```cpp
uint32_t srcLen = 896, dstLen = 1024, numOfIndexTabEntry = 1;
AscendC::LocalTensor<int8_t> weightB1 = inQueueB1.AllocTensor<int8_t>();
AscendC::LoadUnzipIndex(indexGlobal, numOfIndexTabEntry); // 加载索引数据，加载GM上的压缩索引表到内部寄存器
AscendC::LoadDataUnzip(weightB1, weGlobal); // 根据内部寄存器里的索引表加载数据
```
