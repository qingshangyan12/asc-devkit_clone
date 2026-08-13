# data_copy_l0c2gm样例

## 概述

本样例演示如何使用Ascend C C API，通过Fixpipe将矩阵乘计算结果从L0C Buffer搬运到GM（Global Memory），并在搬运过程中完成标量/Vector量化、ReLU激活和NZ2ND格式转换。输入矩阵A和B均为ND格式，先从GM搬运到L1 Buffer并转换为Nz格式，再经L0A Buffer和L0B Buffer完成两次K轴分块矩阵乘。

本样例适用于Ascend 950PR/Ascend 950DT（`dav-3510`），可在NPU运行模式或NPU仿真模式下执行，不提供CPU域调试模式。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
data_copy_l0c2gm
├── scripts
│   ├── gen_data.py                // 输入数据和真值数据生成脚本
│   └── verify_result.py           // 输出数据与真值数据精度校验脚本
├── data_utils.h                   // 数据读入写出函数
├── CMakeLists.txt                 // CMake编译文件
├── data_copy_l0c2gm.asc           // C API样例实现及调用代码
├── README.md                      // 中文样例说明文档
└── README_en.md                   // 英文样例说明文档
```

## 场景详细说明

通过编译参数`SCENARIO_NUM`选择场景。所有场景的矩阵乘规格均为`[M, K, N] = [128, 128, 256]`，其中K轴拆分为两个`[128, 64]`分块，内核函数名为`data_copy_l0c2gm`。

| SCENARIO_NUM | A/B类型 | L0C类型 | 输出类型与格式 | 量化模式 | ReLU | NZ2ND |
|---|---|---|---|---|---|---|
| 1 | int8_t | int32_t | half，ND | 标量`DEQF16` | 是 | 是 |
| 2 | int8_t | int32_t | half，Nz | Vector `VDEQF16` | 否 | 否 |
| 3 | half | float | int8_t，Nz | 标量`QF322B8_PRE` | 否 | 否 |
| 4 | half | float | int8_t，ND | Vector `VQF322B8_PRE` | 是 | 是 |
| 5 | int8_t | int32_t | int8_t，ND | 标量`REQ8` | 否 | 是 |
| 6 | int8_t | int32_t | int8_t，Nz | Vector `VREQ8` | 是 | 否 |
| 7 | int8_t | int32_t | int4b_t，Nz | Vector `VREQ4` | 是 | 否 |
| 8 | int8_t | int32_t | bfloat16_t，Nz | Vector `VQS322BF16_PRE` | 否 | 否 |
| 9 | half | float | half，Nz | Vector `VQF322F16_PRE` | 否 | 否 |
| 10 | half | float | bfloat16_t，Nz | Vector `VQF322BF16_PRE` | 否 | 否 |
| 11 | half | float | int4b_t，Nz | Vector `VQF322S4_PRE` | 否 | 否 |
| 12 | half | float | fp8_e4m3fn_t，Nz | Vector `VQF322FP8_PRE` | 否 | 否 |
| 13 | half | float | hifloat8_t，Nz | Vector `VQF322HIF8_PRE` | 否 | 否 |
| 14 | half | float | hifloat8_t，Nz | Vector `VQF322HIF8_PRE_HYBRID` | 否 | 否 |
| 15 | half | float | float，Nz | Vector `VQF322F32_PRE` | 否 | 否 |
| 16 | half | float | half，Nz | Cast `F322F16` | 否 | 否 |
| 17 | half | float | bfloat16_t，Nz | Cast `F322BF16` | 否 | 否 |

`SCENARIO_NUM`由CMake作为编译期宏传入，内核通过`if constexpr`选择对应场景。切换场景后，需要重新编译。

设备侧数据流如下：每个K轴分块按照`GM→L1 Buffer（MTE2）→L0A Buffer/L0B Buffer（MTE1）→Mmad（M）`执行；`MTE1→MTE2`和`M→MTE1`仅用于保护下一轮分块对L1 Buffer、L0A Buffer和L0B Buffer的复用。最终建立`M→FIX`依赖，使Fixpipe在Mmad完成后将L0C Buffer中的计算结果搬运到GM（Global Memory）；核函数结束时调用`asc_sync_pipe(PIPE_ALL)`，确保全部流水完成。

**场景1：int8输入、标量反量化并输出ND half**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`int8_t`
- 输出：C `[128, 256]`，ND格式`half`
- 实现：调用`asc_set_l0c_copy_prequant`配置标量反量化参数，使用`DEQF16`模式；启用ReLU和NZ2ND

**场景2：int8输入、Vector反量化并输出Nz half**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`int8_t`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`half`
- 实现：使用`VDEQF16`模式，每列使用一个Vector量化参数

**场景3：half输入、标量量化并输出Nz int8_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`
- 输出：C `[128, 256]`，Nz格式`int8_t`
- 实现：调用`asc_set_l0c_copy_prequant`配置标量量化参数，使用`QF322B8_PRE`模式

**场景4：half输入、Vector量化并输出ND int8_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，ND格式`int8_t`
- 实现：使用`VQF322B8_PRE`模式；启用ReLU和NZ2ND

**场景5：int8输入、标量量化并输出ND int8_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`int8_t`
- 输出：C `[128, 256]`，ND格式`int8_t`
- 实现：调用`asc_set_l0c_copy_prequant`配置标量量化参数，使用`REQ8`模式并启用NZ2ND

**场景6：int8输入、Vector量化并输出Nz int8_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`int8_t`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`int8_t`
- 实现：使用`VREQ8`模式并启用ReLU

**场景7：int8输入、Vector量化并输出Nz int4b_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`int8_t`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`int4b_t`（每字节打包2个int4值）
- 实现：使用`VREQ4`模式（int32_t量化成int4b_t），启用ReLU

**场景8：int8输入、Vector量化并输出Nz bfloat16_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`int8_t`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`bfloat16_t`
- 实现：使用`VQS322BF16_PRE`模式（int32_t量化成bfloat16_t）

**场景9：half输入、Vector量化并输出Nz half**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`half`
- 实现：使用`VQF322F16_PRE`模式（float量化成half）

**场景10：half输入、Vector量化并输出Nz bfloat16_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`bfloat16_t`
- 实现：使用`VQF322BF16_PRE`模式（float量化成bfloat16_t）

**场景11：half输入、Vector量化并输出Nz int4b_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`int4b_t`（每字节打包2个int4值）
- 实现：使用`VQF322S4_PRE`模式（float量化成int4b_t）

**场景12：half输入、Vector量化并输出Nz fp8_e4m3fn_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`fp8_e4m3fn_t`
- 实现：使用`VQF322FP8_PRE`模式（float量化成fp8_e4m3fn_t）

**场景13：half输入、Vector量化并输出Nz hifloat8_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`hifloat8_t`
- 实现：使用`VQF322HIF8_PRE`模式（float量化成hifloat8_t，Half to Away Round）

**场景14：half输入、Vector量化并输出Nz hifloat8_t（Hybrid Round）**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`hifloat8_t`
- 实现：使用`VQF322HIF8_PRE_HYBRID`模式（float量化成hifloat8_t，Hybrid Round）

**场景15：half输入、Vector量化并输出Nz float**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`；量化参数`[256] uint64_t`
- 输出：C `[128, 256]`，Nz格式`float`
- 实现：使用`VQF322F32_PRE`模式（float量化成float），精度可以达到双千分之一，无法达到双万分之一。如果有双万分之一的精度要求，建议使用AscendDeQuant高阶API

**场景16：half输入、Cast转换并输出Nz half**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`
- 输出：C `[128, 256]`，Nz格式`half`
- 实现：使用`F322F16`模式（float cast成half），cast mode为CAST_RINT模式（round to nearest even）

**场景17：half输入、Cast转换并输出Nz bfloat16_t**

- 输入：A `[128, 128]`、B `[128, 256]`，均为ND格式`half`
- 输出：C `[128, 256]`，Nz格式`bfloat16_t`
- 实现：使用`F322BF16`模式（float cast成bfloat16_t），cast mode为CAST_RINT模式（round to nearest even）

对于场景2、4、6、7、8、9、10、11、12、13、14和15，量化参数先通过`asc_copy_gm2l1`从GM搬运到L1 Buffer，再通过`asc_copy_l12fb`搬运到Fixpipe Buffer，最后调用`asc_set_l0c2gm_config`配置Vector量化参数地址。`asc_set_l0c2gm_config`的参数地址以128B为单位，因此参数文件需按128B向上对齐，不足部分补0。本样例的`[256] uint64_t`参数共2048B，已满足要求。量化参数从L1 Buffer搬运至Fixpipe Buffer时，需要搬运2048B数据，数据搬运单位为64B，对应`asc_copy_l12fb`的`len_burst`配置为32。量化参数从L1 Buffer搬运至Fixpipe Buffer后，通过`asc_sync_pipe(PIPE_FIX)`确保参数和配置生效，随后通过Fixpipe将L0C Buffer中的计算结果搬运到GM（Global Memory）。

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
SCENARIO_NUM=6
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

- 执行结果

  输出与真值比较通过时，打印如下信息：

  ```bash
  test pass!
  ```
