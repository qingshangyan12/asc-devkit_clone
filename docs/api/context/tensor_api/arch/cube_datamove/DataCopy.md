# DataCopy>

## 产品支持情况>

|产品|是否支持|
|--|:-:|
|Ascend 950PR/Ascend 950DT|√|
|Atlas A3 训练系列产品/Atlas A3 推理系列产品|x|
|Atlas A2 训练系列产品/Atlas A2 推理系列产品|x|
|Atlas 200I/500 A2 推理产品|x|
|Atlas 推理系列产品AI Core|x|
|Atlas 推理系列产品Vector Core|x|
|Atlas 训练系列产品|x|
|Atlas 200/300/500 推理产品|x|


## 功能说明>

DataCopy提供不同存储位置间数据搬运功能，支持Global Memory到L1 Buffer和L1 Buffer到BiasTable/Fixpipe Buffer的数据搬运，同时支持随路数据格式转换。

## 函数原型>

```cpp
template <const DataCopyTrait& trait = DEFAULT_DATA_COPY_TRAIT, typename T, typename U>
__aicore__ inline typename Std::enable_if<VerifyingDataCopyTemplate<T, U>, void>::type
DataCopy(const T& dst, const U& src)

template <const DataCopyTrait& trait = DEFAULT_DATA_COPY_TRAIT, typename T, typename U, typename Coord>
__aicore__ inline typename Std::enable_if<VerifyingDataCopyTemplateWithCoord<T, U, Coord>, void>::type
DataCopy(const T& dst, const U& src, const Coord& coord)
```


## 参数说明>

**表 1**  模板参数说明

|参数名|描述|
|--|--|
|DataCopyTrait|预留参数，保持默认值即可。|
|T|目的操作数的数据类型，通过MakeTensor构造的[LocalTensor类型](../../tensor/Tensor.md)，存储位置支持GM和L1，数据格式支持ND、DN和Nz。|
|U|源操作数的数据类型，通过MakeTensor构造的[LocalTensor类型](../../tensor/Tensor.md)，存储位置支持L1、L0A、L0B、BIAS和FIXBUF，数据格式支持ND、DN、Nz、Zn和Zz。|
|Coord|坐标偏移的数据类型，通过MakeCoord构造的[Coord类型](../../tensor/Coord.md)。| 


**表 2**  参数说明

|参数名|输入/输出|含义|
|--|--|--|
|dst|输出|目的操作数。|
|src|输入|源操作数。|
|coord|输入|源操作数上的偏移坐标。|

## 返回值说明>

VerifyingDataCopyTemplate/VerifyingDataCopyTemplateWithCoord表达式为DataCopy模板参数校验表达式，执行效果如下：

-   校验表达式成立，表明输入参数符合DataCopy接口入参数据类型约束，DataCopy函数表达式返回值为void。。
-   校验表达式不成立，表明输入参数不符合DataCopy接口入参数据类型约束。上述四个模板参数表达式均不通过时，DataCopy函数声明失效，继而会导致编译错误。

## 约束说明>
-   起始地址对齐约束：L1上要求32字节对齐，L0A/L0B上要求512字节对齐   
-   Tensor Layout相关约束：
    -   Shape、Stride只支持四维，针对不同的物理存储位置，四个维度的配置均有不同的约束，部分维度为固定值，不可配置。详见[层次化表达法](../../tensor/Layout和层次化表述法.md)。
    -   Shape、Stride具体维度的数据，仅支持基础整数类型和Std::Int类型。


## 数据通路说明>

下文的数据通路均通过逻辑位置Hardware来表达，并注明了对应的物理通路。Hardware与物理内存的映射关系见[表1]。

**表 3**  GM-\>L1 Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor<TensorTrait>。|
|模板参数U|数据类型为GlobalTens or<TensorTrait>。|
|目的操作数TPosition取值范围|TPosition::A1、TPosition::B1、TPosition::C1|
|源操作数TPosition取值范围|TPosition::GM|
|目的操作数的数据类型|bfloat16_t、 half、 float、 int16_t、 int32_t、 int8_t、 uint16_t、 uint32_t、 uint8_t|
|源操作数的数据类型|bfloat16_t、 half、 float、 int16_t、 int32_t、 int8_t、 uint16_t、 uint32_t、 uint8_t|
|源操作数和目的操作数基础数据类型是否要求一致|是|
|源操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|目的操作数数据对齐要求|起始地址和数据长度要求32字节对齐。|
|数据排布要求|NZ Layout -> NZ Layout : 源操作数和目的操作数均支持连续和非连续排布。ND Layout -> ND Layout : 源操作数和目的操作数均支持连续和非连续排布。RowMajor Layout -> NZ Layout ：源操作数和目的操作数仅支持连续排布。ColumnMajor Layout ->  NZ Layout：源操作数和目的操作数仅支持连续排布。NZ Layout内层矩阵的Shape大小是16 * (32 / sizeof(T))。非连续搬运时，外层矩阵列方向要保证连续，行方向可以跳跃。即：// StrideColumn1应满足：StrideColumn1 = StrideRow1 * ShapeRow1|


**表 4**  L1 Buffer -\> BiasTable Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor，数据格式为ND。|
|模板参数U|数据类型为LocalTensor，数据格式为ND。|
|目的操作数Hardware要求|Hardware::BIAS|
|源操作数Hardware要求|Hardware::L1|
|目的操作数的数据类型|half、bfloat16_t、int32_t、float|
|源操作数的数据类型|half、bfloat16_t、int32_t、float|
|目的操作数和源操作数数据类型组合要求|源数据类型为half，目的数据为float场景可以不一致，此外必须一致 |
|目的操作数数据对齐要求|起始地址要求64字节对齐。|
|源操作数数据对齐要求|起始地址要求32字节对齐。| 
|数据格式要求|源操作数和目的操作数数据格式均为ND。| 
|搬运数据量要求|每次最多搬运4KB数据|


**表 5**  L1 Buffer -\> Fixpipe Buffer通路使用说明

|项目|内容|
|--|--|
|模板参数T|数据类型为LocalTensor，数据格式为ND。|
|模板参数U|数据类型为LocalTensor，数据格式为ND。|
|目的操作数Hardware要求|Hardware::FIXBUF|
|源操作数Hardware要求|Hardware::L1|
|目的操作数的数据类型|uint64_t|
|源操作数的数据类型|uint64_t|
|目的操作数和源操作数数据类型要求|类型一致 |
|目的操作数数据对齐要求|起始地址要求128 字节对齐。|
|源操作数数据对齐要求|起始地址要求32字节对齐。| 
|数据格式要求|源操作数和目的操作数数据格式均为ND。| 
|搬运数据量要求|每次最多搬运4KB数据|

## 调用示例>

```cpp
...
    using namespace AscendC::Te;

    constexpr uint32_t m = 64;
    constexpr uint32_t n = 64;
    constexpr uint32_t k = 64;
    using _1 = AscendC::Std::Int<1>;

    // 创建GM上tensor的layout
    auto layoutA = MakeLayout(MakeShape(m, k), MakeStride(k, _1{}));
    auto layoutB = MakeLayout(MakeShape(k, n), MakeStride(n, _1{}));
    auto layoutC = MakeNDLayout<int8_t>(m, n);
    auto layoutQuant = MakeLayout(MakeShape(_1{}, n), MakeStride(n, _1{}));
    auto layoutBias = MakeLayout(MakeShape(_1{}, n), MakeStride(n, _1{}));

    // 创建GM上tensor的迭代器
    auto gmPtrA = MakeGMmemPtr(gm_x);
    auto gmPtrB = MakeGMmemPtr(gm_y);
    auto gmPtrC = MakeGMmemPtr(gm_z);
    auto gmPtrQuant = MakeGMmemPtr(gm_quant);
    auto gmPtrBias = MakeGMmemPtr(gm_bias);

    // 创建GM上tensor
    auto globalA = MakeTensor(gmPtrA, layoutA);
    auto globalB = MakeTensor(gmPtrB, layoutB);
    auto globalC = MakeTensor(gmPtrC, layoutC);
    auto globalQuant = MakeLayout(MakeShape(_1{}, n), MakeStride(n, _1{}));
    auto globalBias = MakeLayout(MakeShape(_1{}, n), MakeStride(n, _1{}));

    // 创建L1上tensor的layout
    auto l1ALayout = MakeNzLayout<half>(m, k);
    auto l1BLayout = MakeNzLayout<half>(k, n);
    auto l1QuantLayout = MakeNDLayout<uint64_t>(_1{}, n);
    auto l1BiasLayout = MakeNDLayout<uint64_t>(_1{}, n);

    // 创建L0上tensor的layout
    auto l0ALayout = MakeNzLayout<half>(m, k);
    auto l0BLayout = MakeZnLayout<half>(k, n);
    auto l0CLayout = MakeL0CLayout(m, n);

     // 创建BT上tensor的layout
    auto btLayout = MakeNDLayout<uint64_t>(_1{}, n);

    constexpr int32_t  L1_MAX = 128*128;
    constexpr int32_t  L0_MAX = 128*128;
    constexpr int32_t  L0C_MAX = 128*128;

    // 数组方式静态分配内存
    __cbuf__ half l1AAddr[L1_MAX];
    __cbuf__ half l1BAddr[L1_MAX];
    __cbuf__ uint64_t l1QuantAddr[128];
    __cbuf__ float l1BiasAddr[128];

    __ca__ half l0AAddr[L0_MAX];
    __cb__ half l0BAddr[L0_MAX];
    __cc__ float l0CAddr[L0C_MAX];
    __biasbuf__ float btAddr[L0_MAX];

    // 创建L1上tensor的迭代器
    auto l1APtr = MakeL1memPtr(l1AAddr);
    auto l1BPtr = MakeL1memPtr(l1BAddr);
    auto l1QuantPtr = MakeL1memPtr(l1QuantAddr);
    auto l1BiasPtr = MakeL1memPtr(l1BiasAddr);

    // 创建L0上tensor的迭代器
    auto l0APtr = MakeL0AmemPtr(l0AAddr);
    auto l0BPtr = MakeL0BmemPtr(l0BAddr);
    auto l0CPtr = MakeL0CmemPtr(l0CAddr);
    // 创建BT上tensor的迭代器
    auto biasPtr = MakeBiasmemPtr(btAddr);

    // 创建L1上tensor
    auto l1ATensor = MakeTensor(l1APtr, l1ALayout);
    auto l1BTensor = MakeTensor(l1BPtr, l1BLayout);
    auto l1QuantTensor = MakeTensor(l1QuantPtr, l1QuantLayout);
    auto l1BiasTensor = MakeTensor(l1BiasPtr, l1BiasLayout);

    // 创建L0上tensor
    auto l0ATensor = MakeTensor(l0APtr, l0ALayout);
    auto l0BTensor = MakeTensor(l0BPtr, l0BLayout);
    auto l0CTensor = MakeTensor(l0CPtr, l0CLayout);
    // 创建BT上tensor
    auto biasTensor = MakeTensor(biasPtr, btLayout);

    
    auto atomCopyGM2L1 = MakeCopy(CopyGM2L1{}, DataCopyTraitDefault{});
    // GM->L1的数据搬运
    atomCopyGM2L1.Call(l1ATensor, globalA);
    atomCopyGM2L1.Call(l1BTensor, globalB);
    atomCopyGM2L1.Call(l1QuantTensor, globalQuant);
    atomCopyGM2L1.Call(l1BiasTensor, globalBias);

    set_flag(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    wait_flag(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    

    auto atmoCopyL12L0 = MakeCopy(CopyL12L0{}, LoadDataTraitDefault{});
    // L1->L0A的数据搬运
    atmoCopyL12L0.Call(l0ATensor, l1ATensor);
    // L1->L0B的数据搬运
    atmoCopyL12L0.Call(l0BTensor, l1BTensor);

    auto atmoCopyL12BT = MakeCopy(CopyL12BT{}, LoadDataTraitDefault{});
    // L1->BT的数据搬运
    atmoCopyL12L0.Call(biasTensor, l1BiasTensor);
    
    set_flag(PIPE_MTE1, PIPE_M, EVENT_ID0);
    wait_flag(PIPE_MTE1, PIPE_M, EVENT_ID0);

    // 执行Mad运算
    MmadParams para;
    para.m = m;
    para.n = n;
    para.k = k;
    para.cmatrixInitVal = true;
    Mad(MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}, l0CTensor, l0ATensor, l0BTensor, para);

    set_flag(PIPE_M, PIPE_FIX, EVENT_ID0);
    wait_flag(PIPE_M, PIPE_FIX, EVENT_ID0);    

    // L0C->GM的数据拷贝
    auto atomCopyL0C2GM = MakeCopy(CopyL0C2GM{}, FixpipeTraitDefault{});
    atomCopyL0C2GM.Call(globalC, l0CTensor, l1QuantTensor);
...
```

