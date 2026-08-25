# asc_dcci_entire_ub样例

## 概述

本样例验证[asc_dcci_entire_ub](../../../../../docs/zh/api/SIMD-API/c_api/cache_ctrl/asc_dcci.md)在`dav-3510`架构下刷新UB对应DCache的功能。核函数使用`__vector__ __global__`声明，仅在Vector Core上执行。

## 支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持，验证环境为CANN9.2.0-beta.1和NPU运行模式。
<!-- end id1 -->

## 测试设计

样例使用同一个AIV核函数操作两个相隔128字节的UB位置，执行过程如下：

1. Vector单元将两个位置写为旧值`11`，并同步到Scalar流水。
2. 读取并保存CTRL寄存器原值，要求设置前CTRL[49]为`1'b0`。
3. 将CTRL[49]设置为`1'b1`并读回确认，使Scalar通过DCache读取UB。
4. Scalar读取两个位置，将旧值装入DCache。
5. Vector单元将UB中的两个位置覆盖为新值`101`，并同步到Scalar流水。
6. 调用接口前，Scalar再次读取两个位置，预期仍从DCache读到旧值`11`。
7. 调用`asc_dcci_entire_ub()`，随后调用`asc_sync_data_barrier(mem_dsb_t::DSB_UB)`。
8. Scalar再次读取两个位置，预期读到UB中的新值`101`，最后恢复CTRL寄存器原值。

该设计要求接口调用前必须观察到旧值。如果调用前已经读到新值，则无法证明UB与DCache存在不一致，样例会返回失败，不会将其视为接口通过。

## 预期结果

| 观测项 | 期望值 | 判断目的 |
| --- | --- | --- |
| `original_ctrl_bit49` | 0 | 确认调用`asc_set_ctrl`前CTRL[49]未开启。 |
| `enabled_ctrl_bit49` | 1 | 确认调用`asc_set_ctrl`后UB对DCache可缓存。 |
| `warm_line0`、`warm_line1` | 11、11 | 确认两个位置已将旧值装入DCache。 |
| `before_line0`、`before_line1` | 11、11 | 确认Vector覆盖UB后，DCache中仍为旧值。 |
| `after_line0`、`after_line1` | 101、101 | 确认整个UB DCCI和DSB后读取到新值。 |

## CANN软链接前提

`test.sh`只检查软链接，不修改CANN安装目录。执行前要求以下链接已经存在：

```text
/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/include/c_api
  -> /home/g00954335/asc-devkit_GRJ/include/c_api
/usr/local/Ascend/cann-9.2.0-beta.1/x86_64-linux/asc/impl/c_api
  -> /home/g00954335/asc-devkit_GRJ/impl/c_api
```

可以通过环境变量`ASCEND_CANN_PACKAGE_PATH`指定其他CANN安装路径，但链接目标仍必须是当前仓库对应的`include/c_api`和`impl/c_api`目录。

## 编译运行

在本样例目录执行：

```bash
bash test.sh
```

脚本会校验软链接、清理本样例的`build`目录、配置`dav-3510`NPU工程、编译并运行。仅当八个观测值全部符合预期时，脚本才输出`test pass!`并返回0。

## 验证证据与合入结论

| 检查项 | 结果 |
| --- | --- |
| 原始CANN9.2.0-beta.1编译对照 | 失败，编译器报告`asc_dcci_entire_ub`未声明，符合包内未支持3510的现状。 |
| 3510 DCCI单元测试 | 同文件6个DCCI用例全部通过，新增用例确认参数为UB零地址、`ENTIRE_DATA_CACHE`和`CACHELINE_UB`。 |
| 链接本仓后的样例编译 | 通过，目标架构为`dav-3510`。 |
| NPU功能验证 | 补充CTRL[49]观测后干净构建与运行通过，设置前为`0`，设置后为`1`，接口调用前为`11、11`，调用后为`101、101`。 |

判断依据：接口映射正确、样例可以编译、调用前后差异能够证明接口生效、重复运行结果稳定，并且相邻DCCI接口回归通过。因此，本接口可以合入。
