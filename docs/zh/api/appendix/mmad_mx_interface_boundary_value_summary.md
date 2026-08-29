# MmadMx接口边界值汇总

本节汇总介绍了MmadMx计算接口在边界值输入下的输出结果，数据类型见基础API下的[MmadMx](../SIMD-API/basic_api/cube_compute_ISASI/mmad_compute/MmadMx.md#数据类型)，以下内容针对如下型号生效：

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT
<!-- end id1 -->

## 边界值说明
- fp4数据类型没有+inf/-inf/nan定义，从其他数据类型的+inf/-inf/nan转换为fp4数据类型会被直接转换为边界值或0参与后续计算。值转换情况如下表所示：
    
    **表1**  fp4类型边界值转换结果

    |  | nan | -inf | inf |
    | --- | --- | --- | --- |
    | fp4_e1m2 | 0 | -1.75 | 1.75 |
    | fp4_e2m1 | 0 | -6 | 6 |

- fp8_e4m3fn_t数据类型只有nan定义，没有inf、-inf的定义，从其他数据类型的inf、-inf值转成fp8_e4m3fn_t数据类型时会被转换成nan。

## 边界值汇总

**表2** MmadMx接口边界值汇总

| C矩阵的数据类型 | MmadMx子阶段 | A矩阵的元素取值 | B矩阵的元素取值 | dst的元素取值<br>（INF/NAN模式） | dst的元素取值<br>（饱和模式） |
| --- | --- | --- | --- | --- | --- |
| float | 矩阵乘法阶段 | +inf | norm(> 0.0)/+inf | +inf | MAX（0x7F800000） |
| float | 矩阵乘法阶段 | +inf | norm(< 0.0)/-inf | -inf | MIN（0xFF800000） |
| float | 矩阵乘法阶段 | -inf | norm(> 0.0)/+inf | -inf | MIN（0xFF800000） |
| float | 矩阵乘法阶段 | -inf | norm(< 0.0)/-inf | +inf | MAX（0x7F800000） |
| float | 矩阵乘法阶段 | +inf/-inf | 0.0 | nan | 0.0 |
| float | 矩阵乘法阶段 | norm(> 0.0) | +inf | +inf | MAX（0x7F800000） |
| float | 矩阵乘法阶段 | norm(> 0.0) | -inf | -inf | MIN（0xFF800000） |
| float | 矩阵乘法阶段 | norm(< 0.0) | +inf | -inf | MIN（0xFF800000） |
| float | 矩阵乘法阶段 | norm(< 0.0) | -inf | +inf | MAX（0x7F800000） |
| float | 矩阵乘法阶段 | 0.0 | +inf/-inf | nan | 0.0 |
| float | 矩阵乘法阶段 | nan | 任意输入 | nan | nan值会在进行乘法计算前被舍入为0 |
| float | 矩阵乘法阶段 | 任意输入 | nan | nan | nan值会在进行乘法计算前被舍入为0 |
| float | 矩阵乘法阶段 | 任意输入 | 任意输入 | 结果溢出情况下：<br>正向溢出为inf，<br>负向溢出为-inf | MAX（0x7F800000）/MIN（0xFF800000） |
| float | 加法阶段 | +inf | -inf | -inf | MIN（0xFF800000） |
| float | 加法阶段 | +inf | +inf/norm | +inf | MAX（0x7F800000） |
| float | 加法阶段 | -inf | +inf | +inf | MAX（0x7F800000） |
| float | 加法阶段 | -inf | -inf/norm | -inf | MIN（0xFF800000） |
| float | 加法阶段 | nan | 任意输入 | nan | nan值会在进行加法计算前被舍入为0 |
| float | 加法阶段 | 任意输入 | nan | nan | nan值会在进行加法计算前被舍入为0 |

注：

MmadMx计算应避免nan输入，否则可能会产生执行报错。
