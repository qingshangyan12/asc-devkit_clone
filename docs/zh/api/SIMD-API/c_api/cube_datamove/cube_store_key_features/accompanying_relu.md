# 随路ReLU<a name="ZH-CN_TOPIC_0000002538231206"></a>

## 特性说明

C API通过L0C搬出接口的激活参数控制随路激活。3510使用`asc_relu_pre_mode`枚举，2201现有搬出重载仍使用整数`relu_pre`。相比只描述Normal ReLU的场景，需要同时区分Scalar、Vector和Clip控制。

| 3510枚举 | 编码 | 作用及配置 |
| --- | --- | --- |
| `NONE` | 0 | 不启用激活 |
| `NORMAL` | 1 | Normal ReLU，负半轴系数为0 |
| `SCALAR` | 2 | 全矩阵共享激活系数，使用[asc_set_l0c_copy_relu_alpha](../asc_set_l0c_copy_relu_alpha.md)配置 |
| `VECTOR` | 3 | 每个N通道独立激活系数，先将4B系数搬入激活区，再用[asc_set_l0c_copy_config](../asc_set_l0c_copy_config.md)配置`relu_pre_addr` |

不带量化时，Normal ReLU的数学效果为`y=max(0,x)`。Scalar/Vector模式控制负半轴系数，用于参数化激活；不能将2、3当成布尔true，或当成普通ReLU。量化和激活同时开启时，正负半轴分别使用M1/M2，见[随路量化与随路ReLU场景组合](accompanying_quantization_and_relu_scenario_combination.md)。

3510还提供`enable_clip_relu_pre`作为独立的Clip开关。Clip需搭配Normal ReLU和量化模式，但当前公开C API中未找到Clip上限的专用配置接口，不能把该开关描述为已经具备任意上限配置的完整流程。不要照搬Basic API的`SetFixPipeClipRelu`调用；本仓库3510的Basic API该实现会报告不支持。

## 特性约束

- [asc_set_l0c_copy_relu_alpha](../asc_set_l0c_copy_relu_alpha.md)与[asc_set_l0c_copy_lrelu_alpha](../asc_set_l0c_copy_lrelu_alpha.md)均用于Scalar激活系数，底层分别调用`set_relu_alpha`和`set_lrelu_alpha`；二者都不能当作Clip上限配置接口。本文Scalar示例采用前者。
- Scalar系数仅保留float位模式的高19位，即(1,8,10)格式；系数应满足所选接口对有限值的要求。
- Vector激活按源矩阵N列索引，即使输出NZ2DN仍按原通道对应。3510激活区2KB，每个系数4B，起始索引单位为64B；它与量化区4KB独立寻址。
- UB双目标不支持其他随路量化/激活功能组合，不能同时启用本页的额外激活模式。
- 2201没有3510的枚举搬出重载和Clip开关；使用现有整数`relu_pre`，0/1/2/3分别对应不激活、Normal、Scalar、Vector。Scalar系数使用[asc_set_l0c2gm_lrelu_alpha](../asc_set_l0c2gm_lrelu_alpha.md)的half或float引用入参；Vector系数先搬入1KB激活区，再使用`asc_set_l0c2gm_config`设置地址索引。3510优先使用本页的`asc_set_l0c_copy_*`新接口。

## 使用示例

以下为3510的float→float Normal ReLU片段，源数据已就绪，输出16×16 ND矩阵：

```cpp
asc_set_l0c_copy_nz_para(1, 0, 0);
asc_copy_l0c2gm(dst, src, 16, 16, 16, 16,
    asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM, asc_unit_flag_mode::DISABLE,
    asc_quant_mode::NoQuant, asc_relu_pre_mode::NORMAL,
    false, true, false, false);
```

寄存器配置会持续生效；切换到后续算子时显式选择所需激活模式和系数，不依赖前次调用留下的状态。

2201的Scalar激活配置片段如下；该架构尚无同能力的`asc_set_l0c_copy_relu_alpha`实现，因此使用其现有公开接口：

```cpp
#if __NPU_ARCH__ == 2201
float alpha = 0.5f;
asc_set_l0c2gm_lrelu_alpha(alpha);
// 后续调用2201的asc_copy_l0c2gm/asc_copy_l0c2l1，relu_pre设为2。
#endif
```
