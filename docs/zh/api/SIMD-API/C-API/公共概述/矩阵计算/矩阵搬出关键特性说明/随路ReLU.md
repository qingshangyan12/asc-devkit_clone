# 随路ReLU<a name="ZH-CN_TOPIC_0000002538231206"></a>

## 特性说明

矩阵计算的搬出过程中支持随路ReLU能力。`asc_copy_l0c2gm`的`relu_pre`可设置为1开启Normal ReLU，结合`clip_relu_pre`可进一步开启Clip ReLU能力。

- 参数配置为0时，即不开启随路ReLU能力。

- 参数配置为1时，即Normal ReLU，无需配置额外寄存器，对输出数据执行激活处理：`y = max(0, x)`。

- 开启Normal ReLU（`relu_pre=1`）并同时配置`clip_relu_pre=1`时，即Normal ReLU + Clip ReLU，先执行`y = max(0, x)`，再执行`y = min(clip_relu_max_val, y)`。参数组合与目标架构约束详见[asc_copy_l0c2gm](../../../cube_datamove/asc_copy_l0c2gm/asc_copy_l0c2gm_arch_3510.md)。

与随路量化组合使用的详细信息请参考[随路量化与随路ReLU场景组合](随路量化与随路ReLU场景组合.md)。
