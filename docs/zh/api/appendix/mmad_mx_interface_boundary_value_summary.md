# Mmad_Mx接口边界值汇总

本节汇总介绍了Mmad计算接口在边界值输入下的输出结果，数据类型见基础API下的[MmadMx](../SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/MmadMx.md)，以下内容针对如下型号生效：

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT
<!-- end id1 -->

## 左右矩阵为fp4数据类型场景

fp4数据类型并没有+inf/-inf/nan定义，从其他数据类型的+inf/-inf/nan转换为该数据类型会被直接饱和为边界值或0参与后续计算。
饱和后的结果如下表所示。

**表1**  fp4类型特殊值/边界值饱和结果说明

|  | nan | -inf | inf |
| --- | --- | --- | --- |
| fp4_e1m2 | 0 | -1.75 | 1.75 |
| fp4_e2m1 | 0 | -6 | 6 |

bias矩阵中包含+inf/-inf/nan时，计算结果仍会受到非饱和/饱和模式的控制。计算结果说明如下表：

**表2**  bias矩阵中含有边界值时加法计算结果说明

|  | -inf | inf | nan |
| --- | --- | --- | --- |
| 饱和模式 | -MAX | MAX | 在进行加法计算前被饱和为0，保持乘法计算结果值 |
| 非饱和模式 | -inf | inf | nan |

## 左右矩阵为fp8数据类型的场景

fp8_e4m3fn_t数据类型只有nan定义，没有inf、-inf的定义，从其他数据类型的inf、-inf值强转成fp8_e4m3fn_t时会被转成nan

| C矩阵的数据类型 | Mmad子阶段 | A矩阵的元素取值 | B矩阵的元素取值 | dst的元素取值<br>（INF/NAN模式） | dst的元素取值<br>（饱和模式） |
| --- | --- | --- | --- | --- | --- |
| float | 乘法阶段 | +inf | norm(> 0.0)/+inf | +inf | 在进行计算前会先进行一次饱和舍入 |
| float | 乘法阶段 | +inf | norm(< 0.0)/-inf | -inf | 饱和舍入为float类型能表示的最小值 |
| float | 乘法阶段 | -inf | norm(> 0.0)/+inf | -inf | 饱和舍入为float类型能表示的最小值 |
| float | 乘法阶段 | -inf | norm(< 0.0)/-inf | +inf | 饱和舍入为float类型能表示的最大值 |
| float | 乘法阶段 | +inf/-inf | 0.0 | nan | 舍入为0 |
| float | 乘法阶段 | norm(> 0.0) | +inf | +inf | 饱和舍入为float类型能表示的最大值 |
| float | 乘法阶段 | norm(> 0.0) | -inf | -inf | 饱和舍入为float类型能表示的最小值 |
| float | 乘法阶段 | norm(< 0.0) | +inf | -inf | 饱和舍入为float类型能表示的最小值 |
| float | 乘法阶段 | norm(< 0.0) | -inf | +inf | 饱和舍入为float类型能表示的最大值 |
| float | 乘法阶段 | 0.0 | +inf/-inf | nan | 舍入为0 |
| float | 乘法阶段 | nan | 任意输入 | nan | 舍入为0 |
| float | 乘法阶段 | 任意输入 | nan | nan | 舍入为0 |
| float | 乘法阶段 | 任意输入 | 任意输入 | 结果溢出情况下，正向溢出为inf，负向溢出为-inf | 饱和舍入为float类型能表示的最大值/最小值 |
