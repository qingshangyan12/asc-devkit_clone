# GEMV<a name="ZH-CN_TOPIC_0000002538231187"></a>

**特性说明：**

Gemv的核心功能体现为：当矩阵A的维度M取值为1时，接口会自动启用Gemv功能，该操作退化为1×K维度的行向量与K×N维度矩阵之间的乘法运算。

<!-- npu="950" id1 -->
针对Ascend 950PR/Ascend 950DT产品，可以通过设置`asc_mmad`的`disable_gemv`参数为true，将该功能关闭。
<!-- end id1 -->

**特性约束：**

1. 1×K矩阵A需满足512B地址对齐，K个数据连续存储；以half类型为例，当K=256时，软件侧可视作16\*16分块，配置`left_height=1`后硬件解析为1\*256向量，可通过`asc_copy_l12l0a`将256个half数据从L1 Buffer搬至L0A Buffer。

2. C矩阵是一个（1\*N）的向量，当1×N向量被划分为多个1×16子向量时，每个子向量在L0C Buffer中占用1024B。但实际有效数据仅占最低32B或64B。例如，当N=50时，共划分4个1\*16子向量，占用4\*512B = 2048B，其中有效数据仅为4\*32B = 128B。

**图1** GEMV模式，矩阵乘示意图

![GEMV模式-矩阵乘示意图](../../../../figures/mmad_gemv.png "GEMV模式-矩阵乘示意图")

**使用优势：**

M=1时自动开启GEMV模式，则矩阵乘法将M方向作为非对齐场景进行处理。GEMV模式相较于非对齐处理方式，搬运数据量更少，性能更好。下面以M=1，K=256，N=32，左右矩阵数据类型为half的矩阵乘示例说明。

- GEMV模式

    将A矩阵从L1 Buffer搬运到L0A Buffer时，1\*256的向量被当作16\*16的矩阵进行处理，调用`asc_copy_l12l0a`一次完成16\*16分形大小的矩阵搬运。B矩阵的搬运以及矩阵乘计算与常规场景相同，如下图所示。

    **图2** GEMV模式M=1的矩阵乘计算示意图

    ![GEMV模式M=1的矩阵乘计算示意图](../../../../figures/mmad_gemv_compare.png "GEMV模式M=1的矩阵乘计算示意图")

- 非GEMV模式

    将A矩阵从L1 Buffer搬运到L0A Buffer时，1\*256的向量被当作非对齐矩阵数据进行处理，将M方向对齐到32字节后进行搬运。调用`asc_copy_l12l0a`每次搬运16\*16分形大小的矩阵，一共搬运K/16=16次，导致搬运数据量增加，性能相较于GEMV模式差，如下图所示。

    **图3** 非GEMV模式M=1的矩阵乘计算示意图

    ![非GEMV模式M=1的矩阵乘计算示意图](../../../../figures/mmad_nongemv_compare.png "非GEMV模式M=1的矩阵乘计算示意图")

**使用示例：**

**表1** 示例配置说明

| 矩阵 | 维度大小 | 数据类型 |
| --- | --- | --- |
| A | 1 * 4096 | half |
| B | 4096 * 256 | half |
| C | 1 * 256 | float |

A矩阵为1\*4096的向量，从软件侧可以视为64\*64的（针对Ascend 950PR/Ascend 950DT产品为NZ分形）数据，通过`asc_copy_l12l0a`搬运到L0A Buffer上；

C矩阵为1\* 256的向量，共可以划分为16个1 \* 16的子向量，占用16 \* 1024B = 16384B\(f162f32\)，其中有效数据仅仅为16 \* 64B = 1024B；

**图4** GEMV模式矩阵乘示意图

![GEMV模式-矩阵乘示意图-demo](../../../../figures/mmad_gemv_demo.png "GEMV模式-矩阵乘示意图-demo")

`asc_mmad`的完整原型、参数约束和调用示例请参考[asc_mmad](../asc_mmad.md)。

```cpp
// left_height设置为1，自动开启GEMV能力。
uint16_t left_height = 1;
uint8_t unit_flag = 0;
bool disable_gemv = false;
bool c_matrix_source = false;
bool c_matrix_init_val = true;
asc_mmad(c, a, b, left_height, k, n, unit_flag, disable_gemv,
    c_matrix_source, c_matrix_init_val);
```

<!-- npu="950" id2 -->
针对Ascend 950PR/Ascend 950DT产品，可以将`disable_gemv`设置为`true`关闭GEMV，示例如下：

```cpp
uint16_t left_height = 1;
uint8_t unit_flag = 0;
bool disable_gemv = true;
bool c_matrix_source = false;
bool c_matrix_init_val = true;
asc_mmad(c, a, b, left_height, k, n, unit_flag, disable_gemv,
    c_matrix_source, c_matrix_init_val);
```
<!-- end id2 -->
