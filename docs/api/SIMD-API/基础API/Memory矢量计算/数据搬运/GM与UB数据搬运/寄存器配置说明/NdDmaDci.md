# NdDmaDci<a name="ZH-CN_TOPIC_0000002414107834"></a>

## 产品支持情况<a name="section1550532418810"></a>

| 产品 | 是否支持  |
| :----------------------- | :------: |
| <cann-filter npu-type="950"><term>Ascend 950PR/Ascend 950DT</term> | √</cann-filter> |
| <cann-filter npu-type="A3"><term>Atlas A3 训练系列产品/Atlas A3 推理系列产品</term> | x</cann-filter> |
| <cann-filter npu-type="910b"><term>Atlas A2 训练系列产品/Atlas A2 推理系列产品</term> | x</cann-filter> |
| <cann-filter npu-type="310b"><term>Atlas 200I/500 A2 推理产品</term> | x</cann-filter> |
| <cann-filter npu-type="310p"><term>Atlas 推理系列产品AI Core</term> | x</cann-filter> |
| <cann-filter npu-type="310p"><term>Atlas 推理系列产品Vector Core</term> | x</cann-filter> |
| <cann-filter npu-type="910"><term>Atlas 训练系列产品</term> | x</cann-filter> |

## 功能说明<a name="section618mcpsimp"></a>

头文件路径为：`"basic_api/kernel_operator_data_copy_intf.h"`。

刷新NDDMA DataCache。在多核场景下，当多个AI Core读写同一块GM（Global Memory）地址时，NDDMA DataCache中可能缓存了旧数据，导致部分核读取到错误的数据。因此，在使用DataCopy接口进行多维数据搬运前，需要调用NdDmaDci接口刷新NDDMA DataCache，确保缓存中的数据为最新状态。

## 函数原型<a name="section620mcpsimp"></a>

```cpp
__aicore__ inline void NdDmaDci()
```

## 参数说明<a name="section622mcpsimp"></a>

无

## 返回值说明<a name="section640mcpsimp"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

- 该接口仅在使用DataCopy多维数据搬运接口前调用，用于刷新NDDMA DataCache。

## 调用示例<a name="section1227835243314"></a>

完整算子用例请参考：[多维数据搬运样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/00_data_movement/data_copy_gm2ub_nddma)

以2D Padding场景为例，将GM上shape为[2, 8]的数据搬运至UB并Padding为[4, 16]，左Padding 3，上Padding 1，右Padding 5，下Padding 1，Padding值填充为0。在调用DataCopy多维数据搬运接口前，需要先调用NdDmaDci刷新NDDMA DataCache。

```cpp
// T：搬运数据的类型
// xGm：保存DataCopy搬入数据
// xLocal：保存DataCopy搬出数据

// 2D Padding场景
// xGmShape：[2, 8]，搬运8列2行数据，左Padding 3，上Padding 1，右Padding 5，下Padding 1，xLocalShape：[4, 16]
AscendC::NdDmaLoopInfo<2> loopInfo{{1, 8}, {1, 16}, {8, 2}, {3, 1}, {5, 1}}; 
AscendC::NdDmaParams<T, 2> params{loopInfo, 0}; // padding的值为0
AscendC::NdDmaDci(); // 刷新NDDMA DataCache，保证多核场景下数据一致性
static constexpr AscendC::NdDmaConfig dmaConfig; // 使用默认参数，也可以不传 AscendC::DataCopy<T, 2, dmaConfig>(xLocal, xGm, params);
```

结果示例如下：

```cpp
输入数据（xGm）:
[[ 1. 2. 3. 4. 5. 6. 7. 8.]
[ 9. 10. 11. 12. 13. 14. 15. 16.]]
搬运至Local的数据（xLocal）:
[[ 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0.]
[ 0. 0. 0. 1. 2. 3. 4. 5. 6. 7. 8. 0. 0. 0. 0. 0.]
[ 0. 0. 0. 9. 10. 11. 12. 13. 14. 15. 16. 0. 0. 0. 0. 0.]
[ 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0. 0.]]
```
