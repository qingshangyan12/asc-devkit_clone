# data_copy_l0c2l1样例

## 概述

本样例演示如何使用Ascend C C API，通过`asc_copy_l0c2l1`将矩阵乘计算结果从L0C Buffer搬运到L1 Buffer，并在搬运过程中完成标量/Vector量化、Cast转换等随路能力。输入矩阵A和B均为ND格式，先从GM搬运到L1 Buffer并转换为Nz格式，再经L0A Buffer和L0B Buffer完成两次K轴分块矩阵乘。

由于Ascend 950PR/Ascend 950DT（dav-3510）上L1 Buffer的数据无法直接搬运到GM，本样例参考基础API的`fixpipe_l0c2l1`样例，通过第二次矩阵乘（C × 单位矩阵D）将L1 Buffer上的量化结果经L0A Buffer搬回到L0C Buffer，再通过`asc_copy_l0c2gm`搬运到GM（Global Memory）以便主机侧验证。

本样例适用于Ascend 950PR/Ascend 950DT（`dav-3510`），可在NPU运行模式或NPU仿真模式下执行，不提供CPU域调试模式。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
data_copy_l0c2l1
├── scripts
│   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   └── verify_result.py           // 输出数据与真值数据精度校验脚本
├── data_utils.h                   // 数据读入写出函数
├── CMakeLists.txt                 // CMake编译文件
├── data_copy_l0c2l1.asc           // C API样例实现及调用代码
├── README.md                      // 中文样例说明文档
└── README_en.md                   // 英文样例说明文档
```

## 场景详细说明

通过编译参数`SCENARIO_NUM`选择场景。所有场景的矩阵乘规格均为`[M, K, N] = [128, 128, 128]`，其中K轴拆分为两个`[128, 64]`分块，内核函数名为`data_copy_l0c2l1`。

| SCENARIO_NUM | A/B类型 | L0C类型 | 输出类型与格式 | 量化模式 | 第二次Mmad类型 |
|---|---|---|---|---|---|
| 1 | half | float | int8_t，Nz | 标量`QF322B8_PRE` | int8×int8→int32 |
| 2 | half | float | int8_t，Nz | Vector `VQF322B8_PRE` | int8×int8→int32 |
| 3 | int8_t | int32_t | int8_t，Nz | 标量`REQ8` | int8×int8→int32 |
| 4 | int8_t | int32_t | int8_t，Nz | Vector `VREQ8` | int8×int8→int32 |
| 5 | half | float | half，Nz | Cast `F322F16` | half×half→float |
| 6 | half | float | half，Nz | Vector `VQF322F16_PRE` | half×half→float |
| 7 | int8_t | int32_t | half，Nz | 标量`DEQF16`（int32→half反量化） | half×half→float |
| 8 | int8_t | int32_t | half，Nz | Vector `VDEQF16`（int32→half反量化） | half×half→float |
| 9 | half | float | bfloat16_t，Nz | Cast `F322BF16` | bf16×bf16→float |
| 10 | half | float | bfloat16_t，Nz | Vector `VQF322BF16_PRE` | bf16×bf16→float |
| 11 | int8_t | int32_t | int4b_t，Nz | Vector `VREQ4` | int4b×int4b→int32 |
| 12 | int8_t | int32_t | bfloat16_t，Nz | Vector `VQS322BF16_PRE` | bf16×bf16→float |
| 13 | half | float | int4b_t，Nz | Vector `VQF322S4_PRE` | int4b×int4b→int32 |
| 14 | half | float | fp8_e4m3fn_t，Nz | Vector `VQF322FP8_PRE` | fp8×fp8→float |
| 15 | half | float | hifloat8_t，Nz | Vector `VQF322HIF8_PRE` | hif8×hif8→float |
| 16 | half | float | hifloat8_t，Nz | Vector `VQF322HIF8_PRE_HYBRID` | hif8×hif8→float |
| 17 | half | float | float，Nz | Vector `VQF322F32_PRE` | float×float→float |

> **说明：** 场景9-17需要CANN版本支持对应的`asc_copy_l0c2l1`、`asc_copy_l12l0a`、`asc_copy_l12l0b_trans`和`asc_mmad`函数重载（int4b_t、bfloat16_t、fp8_e4m3fn_t、hifloat8_t、float类型）。

`SCENARIO_NUM`由CMake作为编译期宏传入，内核通过`if constexpr`选择对应场景。切换场景后，需要重新编译。

设备侧数据流如下：

1. **第一次矩阵乘**：每个K轴分块按照`GM→L1 Buffer（MTE2）→L0A Buffer/L0B Buffer（MTE1）→Mmad（M）`执行，结果存放在L0C Buffer。
2. **L0C→L1搬运**（`asc_copy_l0c2l1`，PIPE_FIX）：在Mmad完成后，将L0C Buffer中的计算结果搬运到L1 Buffer，并在搬运过程中完成量化/Cast转换。对于Vector量化场景，量化参数先从GM搬运到L1 Buffer，再搬运到Fixpipe Buffer并配置地址。
3. **第二次矩阵乘**（验证路径）：将L1 Buffer上的量化结果搬回到L0A Buffer（PIPE_MTE1），同时从GM加载单位矩阵D到L1 Buffer再到L0B Buffer，执行`Mmad`（C × D = C），结果存放在L0C Buffer。
4. **L0C→GM搬运**（`asc_copy_l0c2gm`，PIPE_FIX）：将第二次矩阵乘的结果以NoQuant模式和NZ2ND格式搬运到GM。

核函数结束时调用`asc_sync_pipe(PIPE_ALL)`，确保全部流水完成。

## 编译运行

在本样例根目录执行以下步骤。

- 配置环境变量

  请根据CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录；root用户默认为`/usr/local/Ascend`，非root用户默认为`${HOME}/Ascend`。

- NPU运行

  ```bash
  cd ..
  rm -rf build
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # 编译工程，默认NPU模式
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # 生成测试输入数据和真值数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM output/output.bin ./output/golden.bin  # 验证输出结果是否正确
  ```

- NPU仿真

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU仿真模式
  ```

  切换运行模式前需清理CMake缓存，例如在`build`目录执行`rm CMakeCache.txt`后重新配置。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构，对应Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1-17 | 场景编号 |
  | `SATURATION_MODE` | 0、1（默认） | 饱和模式：1=饱和（CTRL[48]=0，inf→±MAX，nan→0），0=非饱和（CTRL[48]=1，inf/nan保持） |

- 执行结果

  输出与真值比较通过时，打印如下信息：

  ```bash
  test pass!
  ```

## 边界值测试

边界值测试通过向输入矩阵A注入inf/-inf/nan（配合B矩阵对应行设为1.0），使L0C = A×B中产生inf/-inf/nan，验证各输出类型在饱和模式和非饱和模式下的inf/nan处理是否符合预期。对于int8_t输入场景，使用极端值（A某行设为-128、其余设为127）验证整数溢出饱和行为。

全部17个场景×2种模式一键测试：

```bash
bash scripts/test_boundary.sh
```

各量化模式对边界值的预期行为（饱和模式，CTRL[48]=0）：

| 输出类型 | +inf | -inf | nan | 说明 |
|---------|------|------|-----|------|
| int8 | 饱和为127 | 饱和为-128或0 | 饱和为0 | 整数类型始终饱和 |
| int4b | 饱和为7 | 饱和为-8或0 | 饱和为0 | 同上 |
| half | 饱和为65504 | 饱和为-65504 | 饱和为0 | CTRL[48]=0控制 |
| bfloat16 | 饱和为max | 饱和为min | 饱和为0 | CTRL[48]=0控制 |
| fp8_e4m3fn | 饱和为448 | 饱和为-448 | 饱和为0（CTRL[50]=0） | CTRL[48]=0+CTRL[50]=0 |
| hifloat8 | 111（HiF8+inf编码） | 239（HiF8-inf编码） | 饱和为0 | CTRL[48]=0控制 |
| float | 保持inf | 保持-inf | 保持nan | CTRL[48]不支持float，始终保持 |

非饱和模式（CTRL[48]=1）预期行为：

| 输出类型 | +inf | -inf | nan | 说明 |
|---------|------|------|-----|------|
| int8 | 同饱和模式 | 同饱和模式 | 同饱和模式 | 整数类型始终饱和 |
| int4b | 同饱和模式 | 同饱和模式 | 同饱和模式 | 同上 |
| half | 保持inf | 保持-inf | 保持nan | float16可表示 |
| bfloat16 | 保持inf | 保持-inf | 保持nan | bfloat16可表示 |
| fp8_e4m3fn | 输出nan | 输出nan | 输出nan | inf或超范围值→nan |
| hifloat8 | 111（HiF8+inf编码） | 239（HiF8-inf编码） | 128（HiF8NAN编码） | inf/nan保持对应编码 |
| float | 保持inf | 保持-inf | 保持nan | CTRL[48]不支持float，始终保持 |

### 已知限制

场景17在边界值测试中跳过，属于硬件限制，非 `asc_copy_l0c2l1` API 本身问题：

| 场景 | 模式 | 原因 |
|------|------|------|
| 17 | 两种模式 | float×float→float Mmad 在 L0C 包含 inf/-inf/nan 时触发硬件运行时错误（`aclrtSynchronizeStream` 返回 507015），可能是 dav-3510 的 CANN 限制 |

注：场景5、6、9、10的非饱和模式存在验证路径中第二次Mmad的inf×0=nan问题（IEEE 754: `inf * 0.0 = nan`，累加后整行变为nan）。golden数据已适配此行为，非饱和模式下inf/-inf/nan位置的golden值设为nan，测试通过。L1 Buffer上的量化结果实际保留了inf（符合API边界值预期），仅经第二次Mmad后被破坏为nan。
