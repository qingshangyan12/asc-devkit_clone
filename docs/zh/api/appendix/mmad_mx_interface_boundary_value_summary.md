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
