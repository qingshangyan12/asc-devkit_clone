# GEMV<a name="ZH-CN_TOPIC_0000002538231187"></a>

**特性说明：**

Gemv 的核心功能体现为：当矩阵 A 的维度 M 取值为 1 时，接口会自动启用 Gemv 功能，该操作退化为 1×K 维度的行向量与 K×N 维度矩阵之间的乘法运算。

<cann-filter npu-type="950">

针对Ascend 950PR/Ascend 950DT产品，可以通过设置MmadParams的disableGemv参数为true，将该功能关闭。

</cann-filter>

**特性约束：**

1. 1×K 矩阵 A 需满足 512B 地址对齐，K 个数据连续存储；以half 类型为例，当 K=256 时，软件侧可视作 16×16 分块，配置 m=1 后硬件解析为 1×256 向量，可通过 LoadData 接口将 256 个 half 数据从 L1 搬至 L0A。

2. C矩阵是一个\(1 \* N\)的向量, 当 1×N 向量被划分为多个 1×16 子向量时，每个子向量在 L0C 中占用1024B（u8/s8/f162f32）。但实际有效数据仅占最低 32B 或 64B。例如，当 N=50 时，共划分 4 个 1×16 子向量，占用 4×512B = 2048B（f162f16），其中有效数据仅为 4×32B = 128B。

**图 1**  GEMV模式，矩阵乘示意图
![](../../../../../figures/mmad_gemv.png "GEMV模式-矩阵乘示意图")

**使用优势：**

M=1时自动开启GEMV模式，则矩阵乘法将M方向作为非对齐场景进行处理。GEMV模式相较于非对齐处理方式，搬运数据量更少，性能更好。下面以M=1，K=256，N=32，左右矩阵数据类型为half的矩阵乘示例说明。

-   GEMV模式

    将A矩阵从L1 Buffer\(TPosition:A1\)搬运到L0A Buffer\(TPosition:A2\)时，1\*256的向量被当作16\*16的矩阵进行处理，调用[LoadData](矩阵数据搬入至L0-Buffer.md#ZH-CN_TOPIC_0000002538071218)接口一次完成16\*16分形大小的矩阵搬运。B矩阵的搬运以及矩阵乘计算跟基础场景相同，如下图所示。

    **图 2**  GEMV模式M==1的矩阵乘计算示意图
    ![](../../../../../figures/mmad_gemv_compare.png "GEMV模式M-1的矩阵乘计算示意图")

-   非GEMV模式

    将A矩阵从L1 Buffer\(TPosition:A1\)搬运到L0A Buffer\(TPosition:A2\)时，1\*256的向量被当作非对齐矩阵数据进行处理，将M方向对齐到32字节后进行搬运。调用[LoadData](矩阵数据搬入至L0-Buffer.md#ZH-CN_TOPIC_0000002538071218)接口每次搬运16\*16分形大小的矩阵，一共搬运K/16=16次，导致搬运数据量增加，性能相较于GEMV模式差，如下图所示。
    **图 3**  非GEMV模式M!=1的矩阵乘计算示意图
![](../../../../../figures/mmad_nongemv_compare.png "非GEMV模式M-1的矩阵乘计算示意图")

**使用示例:**

**表 1**  示例配置说明

| 矩阵 | 维度大小 | 数据类型 |
| --- | --- | --- |
| A | 1 * 4096 | half |
| B | 4096 * 256 | half |
| C | 1 * 256 | float |

A 矩阵为 1\* 4096的向量，从软件侧可以视为64\*64的（针对Ascend 950PR/Ascend 950DT产品为NZ分型）数据，通过LoadData搬运到L0A上；

C 矩阵为 1\* 256的向量，共可以划分为16 个1 \* 16的子向量， 占用16 \* 1024B = 16384B\(f162f32\), 其中有效数据仅仅为 16 \* 64B = 1024B;

**图 4**  GEMV模式矩阵乘示意图
![](../../../../../figures/mmad_gemv_demo.png "GEMV模式-矩阵乘示意图-demo")

GEMV功能的完整示例：[GEMV样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/02_features/03_basic_api/01_matrix_compute/mmad_gemv)。

```cpp
AscendC::MmadParams mmadParams;
// 设置m = 1自动使能GEMV能力
mmadParams.m = 1;
mmadParams.n = n;
mmadParams.k = k;
AscendC::Mmad(c, a, b, mmadParams);
```

<cann-filter npu-type="950">

针对Ascend 950PR/Ascend 950DT产品，可以通过设置MmadParams的disableGemv参数为true，将该功能关闭，示例如下

```cpp
AscendC::MmadParams mmadParams;
// 设置m = 1自动使能GEMV能力
mmadParams.m = 1;
mmadParams.n = n;
mmadParams.k = k;
// 关闭GEMV
mmadParams.disableGemv = true;
AscendC::Mmad(c, a, b, mmadParams);
```
</cann-filter>
