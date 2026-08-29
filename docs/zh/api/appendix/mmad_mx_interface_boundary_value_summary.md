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
- MmadMx计算应避免nan输入，否则可能会产生执行报错。

## 边界值汇总

### INF/NAN模式边界值汇总

**表2** MmadMx接口边界值汇总

| C矩阵的数据类型 | MmadMx子阶段 | A矩阵的元素取值 | B矩阵的元素取值 | dst的元素取值<br>（INF/NAN模式） |
| --- | --- | --- | --- | --- |
| float | 矩阵乘法阶段 | +inf | norm(> 0.0)/+inf | +inf |
| float | 矩阵乘法阶段 | +inf | norm(< 0.0)/-inf | -inf |
| float | 矩阵乘法阶段 | -inf | norm(> 0.0)/+inf | -inf |
| float | 矩阵乘法阶段 | -inf | norm(< 0.0)/-inf | +inf |
| float | 矩阵乘法阶段 | +inf/-inf | 0.0 | nan |
| float | 矩阵乘法阶段 | norm(> 0.0) | +inf | +inf |
| float | 矩阵乘法阶段 | norm(> 0.0) | -inf | -inf |
| float | 矩阵乘法阶段 | norm(< 0.0) | +inf | -inf |
| float | 矩阵乘法阶段 | norm(< 0.0) | -inf | +inf |
| float | 矩阵乘法阶段 | 0.0 | +inf/-inf | nan |
| float | 矩阵乘法阶段 | nan | 任意输入 | nan |
| float | 矩阵乘法阶段 | 任意输入 | nan | nan |
| float | 矩阵乘法阶段 | 任意输入 | 任意输入 | 结果溢出情况下：<br>正向溢出为inf，<br>负向溢出为-inf |
| float | 加法阶段 | +inf | -inf | -inf |
| float | 加法阶段 | +inf | +inf/norm | +inf |
| float | 加法阶段 | -inf | +inf | +inf |
| float | 加法阶段 | -inf | -inf/norm | -inf |
| float | 加法阶段 | nan | 任意输入 | nan |
| float | 加法阶段 | 任意输入 | nan | nan |

### 饱和模式边界值汇总

- 在进行量化计算前会对边界值进行一次舍入，随后以数值形式进行MmadMx计算。
    - nan被转换为0.0。
    - 四种入参类型中只有fp8_e5m2数据类型具有inf/-inf定义，会被转换为57344.0/-57344.0。
- MmadMx计算后结果溢出的情况下结果根据正负分别舍入为MAX（0x7F800000）/MIN（0xFF800000）
- 矩阵乘结果与bias矩阵相加的加法阶段前，bias矩阵中的nan会被舍入为0。
