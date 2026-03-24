# Mmad

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Ascend 950PR/Ascend 950DT|√|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品|x|
| Atlas A2 训练系列产品/Atlas A2 推理系列产品|x|
| Atlas 200I/500 A2 推理产品|x|
| Atlas 推理系列产品AI Core|x|
| Atlas 推理系列产品Vector Core|x|
| Atlas 训练系列产品|x|
| Atlas 200/300/500 推理产品|x|

## 功能说明

完成矩阵乘加（C += A *B 或 C = A* B + Bias）操作。

## 函数原型

```cpp
template <const MmadTrait& trait = DEFAULT_MMAD_TRAIT, typename T, typename U, typename S, typename Params>
__aicore__ inline typename Std::enable_if<VerifyingMmadTemplate<T, U, S>, void>::type 
Mmad(const T& dst, const U& fm, const S& filter, const Params& params)

template <const MmadTrait& trait = DEFAULT_MMAD_TRAIT, typename T, typename U, typename S, typename V, typename Params>
__aicore__ inline typename Std::enable_if<VerifyingMmadWithBiasTemplate<T, U, S, V>, void>::type 
Mmad(const T& dst, const U& fm, const S& filter, const V& bias, const Params& params)
```

## 参数说明

**表 1** 模板参数说明

| 参数名 | 描述 |
| :--- | :--- |
| trait | 矩阵乘加特征，默认为 `DEFAULT_MMAD_TRAIT`。 |
| T | 目的操作数的数据类型。 |
| U | 左矩阵的数据类型。 |
| S | 右矩阵的数据类型。 |
| V | Bias的数据类型。 |
| Params | params的类型 |

**表2** 参数说明

| 参数名 | 类型 | 说明 |
|--------|------|------|
| dst | LocalTensor | 输出矩阵 C，位于 L0C（`__cc__`） |
| fm | LocalTensor | 左矩阵 A，位于 L0A（`__ca__`） |
| filter | LocalTensor | 右矩阵 B，位于 L0B（`__cb__`） |
| bias | LocalTensor | 可选，偏置向量，位于 L0C（`__cbuf__`）或BiasTable Buff（`__biasbuf__`），带偏置时传入 |
| params | MmadParams | 矩阵乘加参数结构体 |

**表3** MmadParams参数说明

| 参数名  | 类型 | 说明 |
|------|------|------|
| m | uint16_t | 左矩阵Height，取值范围：m∈[0, 4095] 。默认值为0。 |
| n | uint16_t | 右矩阵Width，取值范围：n∈[0, 4095] 。默认值为0。 |
| k | uint16_t | 左矩阵Width、右矩阵Height，取值范围：k∈[0, 4095] 。默认值为0。 |
| unitFlag | uint8_t |unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，使能该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于在L0C Buffer累加的场景。取值说明如下：<br>0：保留值；<br>2：使能unitFlag，硬件执行完指令之后，不会关闭unitFlag功能；<br>3：使能unitFlag，硬件执行完指令之后，会将unitFlag功能关闭。|
| cmatrixInitVal | bool | 配置C矩阵初始值是否为0。默认值false。<br>true: C矩阵初始值为0；<br>false: C矩阵初始值通过cmatrixSource参数进行配置。|

**表4** MmadTrait参数说明

| 参数名 | 类型 |说明 |
|------|------|------|
| fmOffset | int32_t | 特征图偏移。默认值0。  |
| kDirectionAlign | bool | K 方向对齐方式。默认值false。  |
| cmatrixSource | bool | 配置C矩阵初始值是否来源于BiasTable Buff。默认值为false。<br> false：来源于L0C；<br> true：来源于C2。|
| disableGemv | bool | 是否禁用 GEMV 优化。默认值false。 |
| mmadType | MmadType | Mmad类型。默认值NORMAL。<br> NORMAL：普通Mmad；<br>MX：带Scale的Mmad。|

## 数据类型支持

**表 5**  dst、fm、filter、bias支持的精度类型组合

| 左矩阵 (fm type) | 右矩阵 (filter type) | bias type | 结果矩阵 (dst type) |
| :--- | :--- | :--- | :--- |
| int8_t | int8_t | int32_t | int32_t |
| half | half | float | float |
| float | float | float | float |
| bfloat16_t | bfloat16_t | float | float |
| fp8_e4m3fn_t | fp8_e4m3fn_t | float | float |
| fp8_e4m3fn_t | fp8_e5m2_t | float | float |
| fp8_e5m2_t | fp8_e4m3fn_t | float | float |
| fp8_e5m2_t | fp8_e5m2_t | float | float |
| hifloat8_t | hifloat8_t | float | float |

## 返回值说明

VerifyingMmadTemplate/VerifyingMmadWithBiasTemplate表达式为模板参数校验表达式，执行效果如下：

- 校验表达式成立，表明输入参数符合Mmad接口入参数据类型约束，Mad函数表达式返回值为void。
- 校验表达式不成立，表明输入参数不符合Mmad接口入参数据类型约束。上述四个模板参数表达式均不通过时，Mad函数声明失效，继而会导致编译错误。

## 约束说明

- 源操作数 A 必须位于 L0A（`__ca__`），B 必须位于 L0B（`__cb__`）
- 目的操作数 C 必须位于 L0C（`__cc__`）
- 当m、n、k中的任意一个值为0时，该指令不会被执行。

## 调用示例

```cpp
using namespace AscendC::Te;


constexpr int32_t  L0_MAX = 128*128*2;
constexpr int32_t  L0C_MAX = 128*128;

size_t m = 128;
size_t n = 64;
size_t k = 64;

auto l0ALayout = MakeZzLayout<half>(m, k);
auto l0BLayout = MakeZnLayout<half>(k, n);
auto l0CLayout = MakeL0CLayout(m, n);
auto biasLayout = MakeNDCLayout<float>(1, n);

__ca__ half l0AAddr[L0_MAX];
__cb__ half l0BAddr[L0_MAX];
__cc__ float l0CAddr[L0C_MAX];
__biasbuf__ float biasAddr[L0C_MAX];

auto l0APtr = MakeL0AmemPtr(l0AAddr);
auto l0BPtr = MakeL0BmemPtr(l0BAddr);
auto l0CPtr = MakeL0CmemPtr(l0CAddr);
auto biasPtr = MakeBiasmemPtr(biasAddr);

auto l0ATensor = MakeTensor(l0APtr, l0ALayout);
auto l0BTensor = MakeTensor(l0BPtr, l0BLayout);
auto l0CTensor = MakeTensor(l0CPtr, l0CLayout);
auto biasTensor = MakeTensor(biasPtr, biasLayout);

MmadParams para;
para.m = m;                  
para.n = n;                  
para.k = k;                  
para.cmatrixInitVal = true;  


// 下面接口为3种调用方式的示例
// 调用方式1
Mad(MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}, L0CTensor, L0ATensor, L0BTensor, para);
// 调用方式2
MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}.Call(L0CTensor, L0ATensor, L0BTensor, para);
// 调用方式3
auto atomMad = MakeMad(MmadOperation{}, MmadTraitDefault{});
atomMad.Call(L0CTensor, L0ATensor, L0BTensor, para);

// 带Bias的调用方式
Mad(MmadAtom<MmadTraits<MmadOperation, MmadTraitDefault>>{}, L0CTensor, L0ATensor, L0BTensor, biasTensor, para);

```

完整样例请参考[TensorAPI样例代码](../../../../../examples/02_features/05_tensor_api)。