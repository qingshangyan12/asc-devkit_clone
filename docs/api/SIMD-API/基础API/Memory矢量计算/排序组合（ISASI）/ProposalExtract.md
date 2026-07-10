# ProposalExtract<a name="ZH-CN_TOPIC_0000001492897985"></a>

## 产品支持情况<a name="section1550532418810"></a>

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：不支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：支持
<!-- end id7 -->

## 功能说明<a name="section618mcpsimp"></a>

与ProposalConcat功能相反，从Region Proposals内将相应位置的单个元素抽取后重排，每次迭代处理16个Region Proposals，抽取16个元素后连续排列。

## 函数原型<a name="section620mcpsimp"></a>

```
template <typename T>
__aicore__ inline void ProposalExtract(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const int32_t modeNumber)
```

## 参数说明<a name="section622mcpsimp"></a>

**表1**  模板参数说明

<a name="table4835205712588"></a>
- T：不支持

**表2**  参数说明

<a name="table8955841508"></a>
- dst：不支持
- src：不支持
- repeatTime：不支持
- modeNumber：不支持

## 返回值说明<a name="section1719311422244"></a>

无

## 约束说明<a name="section633mcpsimp"></a>

-   用户需保证src中存储的proposal数目大于等于实际所需数目，否则会存在tensor越界错误。
-   用户需保证dst中存储的元素大于等于实际所需数目，否则会存在tensor越界错误。
-   操作数地址对齐要求请参见[通用地址对齐约束](../../../通用说明和约束.md#section796754519912)。

## 调用示例<a name="section642mcpsimp"></a>

详细示例请参考[RegionProposalSort样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute/region_proposal_sort)。

-   接口使用样例

    ```
    // repeatTime = 2, modeNumber = 4, 把32个Region Proposal中的score域元素抽取出来排列成32个连续元素
    AscendC::ProposalExtract(dstLocal, srcLocal, 2, 4);
    ```

    ```
    示例结果 
    输入数据(src_gm):
    因为moodel=4，第一个元素33.3的起始位置是4。每个Region Proposal占用连续8个half/float类型的元素。这里使用的类型是half。后续被抽取的每个元素间隔8个元素。repeat为2，每次迭代完成16个元素，共计32个元素
    [ 0.      0.      0.      0. 
      33.3    0.      0.      0.      0.      0.      0.      0.
      67.56   0.      0.      0.      0.      0.      0.      0.
      68.5    0.      0.      0.      0.      0.      0.      0.
      -11.914 0.      0.      0.      0.      0.      0.      0.
      25.19   0.      0.      0.      0.      0.      0.      0.
      -72.8   0.      0.      0.      0.      0.      0.      0.
      11.79   0.      0.      0.      0.      0.      0.      0.
      -49.47  0.      0.      0.      0.      0.      0.      0.
      49.44   0.      0.      0.      0.      0.      0.      0.
      84.4    0.      0.      0.      0.      0.      0.      0.
      -14.36  0.      0.      0.      0.      0.      0.      0.
      45.97   0.      0.      0.      0.      0.      0.      0.
      52.47   0.      0.      0.      0.      0.      0.      0.
      -5.387  0.      0.      0.      0.      0.      0.      0.
      -13.12  0.      0.      0.      0.      0.      0.      0.
      -88.9   0.      0.      0.      0.      0.      0.      0.
      54.     0.      0.      0.      0.      0.      0.      0.
      -51.62  0.      0.      0.      0.      0.      0.      0.
     -20.67   0.      0.      0.      0.      0.      0.      0.
     59.56    0.      0.      0.      0.      0.      0.      0.
     35.72    0.      0.      0.      0.      0.      0.      0.
     -6.12    0.      0.      0.      0.      0.      0.      0.
     -39.4    0.      0.      0.      0.      0.      0.      0.
     -11.46   0.      0.      0.      0.      0.      0.      0.
     -7.066   0.      0.      0.      0.      0.      0.      0.
     30.23    0.      0.      0.      0.      0.      0.      0.
     -11.18   0.      0.      0.      0.      0.      0.      0.
     -35.84   0.      0.      0.      0.      0.      0.      0.
     -40.88   0.      0.      0.      0.      0.      0.      0.
     60.9     0.      0.      0.      0.      0.      0.      0.
     -73.3    0.      0.      0.      0.      0.      0.      0.
     38.47    0.      0.      0. 
     ]
    输出数据(dst_gm):
    [ 33.3    67.56   68.5   -11.914  25.19  -72.8    11.79  -49.47   49.44
      84.4   -14.36   45.97   52.47   -5.387 -13.12  -88.9    54.    -51.62
     -20.67   59.56   35.72   -6.12  -39.4   -11.46   -7.066  30.23  -11.18
     -35.84  -40.88   60.9   -73.3    38.47 ]
    ```
