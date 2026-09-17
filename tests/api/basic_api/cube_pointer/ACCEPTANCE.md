# 验收记录（Ascend 950 / CANN 9.2）

本文件随代码仓一起提供，便于接手者在本机复现，不依赖任何仓库外的临时目录。
记录时间：2026-09-17。

## 环境

| 项 | 值 |
| --- | --- |
| SoC | Ascend950PR_9579（`__NPU_ARCH__ == 3510`） |
| CANN | 9.2.0-beta.1（`source /usr/local/Ascend/cann-9.2.0-beta.1/cann/set_env.sh`） |
| 驱动 | 25.7.rc1 |
| 编译 | bisheng，`--npu-arch=dav-3510` |

任务书规定 CANN 9.0–9.1。本记录使用的是 9.2.0-beta.1，**不能替代**任务书规定的
9.0–9.1 验证。

## 已通过的验收项

### 1. Tensor / 指针 / 混合三种调用方式数值一致（18/18）

命令：

```bash
bash tests/api/basic_api/cube_pointer/run.sh <output>
```

覆盖 fp16、bf16 两种输入，方阵边长 16/32/64，`CUBE_POINTER_MODE` 0（Tensor）、
1（纯指针）、2（Tensor+指针混合），共 18 个实例，全部在真实 950 上执行。

结果：18/18 `pass=true`，`matched == total`，`max_abs_error == 0`（逐元素与独立
CPU 双精度 golden 完全一致）。三种调用方式在同一 shape 下输出相同。

单次事件耗时（`median_us` / `p95_us`）示例：fp16 32×32 三种模式分别为
1.907/2.030、1.725/2.225、1.903/2.112；bf16 64×64 为 1.913/2.068、1.931/2.286、
2.992/3.343。任务书第 3.3 节明确「无额外性能指标与标杆时延要求」，此处仅作记录。

### 2. 非法操作数编译期拒绝（5/5）

命令：

```bash
bash tests/api/basic_api/cube_pointer/negative.sh <output>
```

5 个用例全部被拒绝，且报错落在地址空间约束、只写约束或类型约束上：

| 用例 | 拒绝原因 |
| --- | --- |
| 1 | `__ubuf__ half*` 传给 L0A 参数，地址空间断言失败 |
| 2 | `const float` 目标，`CUBE destination must be writable` |
| 3 | 普通 `float*`，无 `CubeOperandTraits` 特化 |
| 4 | `__ubuf__ float*` 传给 L0C 参数，地址空间断言失败 |
| 5 | `__ubuf__ half*` 作为 LoadData 目标，目标空间约束失败 |

### 3. 矩阵编译探针

`matrix_compile.asc` 编入 `cube_pointer_compile_probe`，在 Cube 与 Vector 核上
实例化 950 适用的指针重载：LoadData、Mmad、MmadMx、Fixpipe、DumpTensor、
DumpAccChkPoint、SetAddrWithOffset、SyncAll、IBSet/IBWait、Determine、
PopStackBuffer。构建通过。

### 4. 双核同步（Tensor / 纯指针 / 两种混合，共 4 种模式）

```bash
cmake -S tests/api/basic_api/cube_pointer -B <out> -DCUBE_SYNC_TEST=ON
cmake --build <out> -j4
timeout 60 <out>/sync_test
```

四种操作数模式下 `SyncAll` 两轮与 `IBSet/IBWait` 一次握手均通过，两核计数器
`2,2`。

## 尚未通过或尚未验证的项

1. **CANN 9.0–9.1 未验证**。任务书指定 9.0–9.1，本机只有 9.2.0-beta.1 与
   9.0.0-beta.2 两套环境，后者无法编译当前 `master`。
2. **dav-2201 指针重载未做编译验证**。`MmadWithSparse` / `LoadDataWithSparse`
   指针重载与原始 Tensor 版本一样受 `#if __NPU_ARCH__ == 2201` 约束；本容器用
   `--npu-arch=dav-2201` 编译 `master` 会在既有头文件
   `impl/basic_api/utils/kernel_utils_struct_norm_sort.h` 的 `__callee__`
   属性处失败，属于该分支在 A2 架构上的既有编译限制，与本扩展无关。
3. **2.4 清单逐行覆盖未完成**。清单 60 行中部分行（SPM 系列、部分 Fixpipe
   参数族）尚无独立用例。
4. **附件回归的部分结论依赖对附件源码的修正**，且 `mmad_unitflag` 对附件原始
   golden 的失败未消除（详见 `docs/zh/cube_pointer_handoff.md`）。该失败不得
   计入通过。
