# Mmad样例
## 概述
本样例介绍了在int4b_t / int8_t/ bfloat16 / float 四种输入数据类型下的矩阵乘法，说明如何通过Mmad指令实现矩阵乘法计算（C = A x B + Bias）。

## 支持的产品
- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── mmad
│   ├── img                         // 本文中的插图文件
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── mmad.asc                    // Ascend C算子实现 & 调用样例
```

## 样例描述
一次完整的矩阵乘法涉及的数据搬运过程包括：GM-->L1、L1-->L0A/L0B、L1-->BT（Bias）、L0C-->GM，其中不同存储单元的数据排布格式，如下表1所示：  

<table border="2" align="center">
<caption>表1：不同存储单元的数据排布格式</caption>
  <tr>
    <td >存储单元</td>
    <td>数据排布格式</td>
  </tr>
  <tr>
    <td>GM</td>
    <td>输入A、B矩阵和输出C矩阵为ND排列。</td>
  </tr>
  <tr>
    <td>L1</td>
    <td>A、B矩阵为NZ排列。</td>
  </tr>
  <tr>
    <td>L0A</td>
    <td>Ascend 950PR/Ascend 950DT产品，A矩阵为NZ排列；<br>Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品，A矩阵为ZZ排列。</td>
  </tr>
  <tr>
    <td>L0B</td>
    <td>B矩阵为ZN排列。</td>
  </tr>
  <tr>
    <td>BT</td>
    <td>Bias为shape时[N]的一维Tensor。</td>
  </tr>
  <tr>
    <td>L0C</td>
    <td>C矩阵为NZ排列。</td>
  </tr>
</table>

通常的矩阵乘法计算公式：C = A × B + Bias，其中A、B、Bias、C矩阵的需要满足的shape分别为[M,K]、[K,N]、[N]和[M,N]。当Mmad计算结果为float类型时，输入的Bias数据类型支持bfloat16（仅Ascend 950PR/Ascend 950DT支持）、half、float三种，但C2的数据类型需要与L0C类型一致。因此，当输入的Bias类型为bfloat16或half时，可以在C1搬运至C2时，使用src和dst数据类型不一致的搬运，具体使用方法与类型一致时一样，只是数据量的区别。

下文将介绍，A、B矩阵在完整矩阵乘法流程的数据排布、对齐要求、所用指令以及如何配置相应参数。程序中以参数scenarioNum来代表上述4种场景，scenarioNum不同取值对应的含义，如下表2所示：

<table border="2" align="center">
<caption>表2：scenarioNum不同取值的含义</caption>
  <tr>
    <td >scenarioNum</td>
    <td>输入数据类型</td>
    <td>输出数据类型</td>
    <td>A矩阵</td>
    <td>B矩阵</td>
    <td>Bias</td>
  </tr>
  <tr>
    <td>1</td>
    <td>int4b_t</td>
    <td>int32_t</td>
    <td>不转置</td>
    <td>转置</td>
    <td>不启用Bias</td>
  </tr>
  <tr>
    <td>2</td>
    <td>int8_t</td>
    <td>int32_t</td>
    <td>不转置</td>
    <td>不转置</td>
    <td>带Bias且不传入biasTensor，C矩阵的初始值来源于C2</td>
  </tr>
  <tr>
    <td>3</td>
    <td>bfloat16</td>
    <td>bfloat16</td>
    <td>不转置</td>
    <td>转置</td>
    <td>不带Bias，C矩阵累加来源于CO1初始值</td>
  </tr>
  <tr>
    <td>4</td>
    <td>float</td>
    <td>float</td>
    <td>转置</td>
    <td>转置</td>
    <td>带Bias且传入biasTensor的场景。</td>
  </tr>
</table>

### 场景详细说明
  本样例通过编译参数 `SCENARIO_NUM` 选择不同的输出场景，所有场景基于相同的矩阵乘规格：[M, N, K] = [30, 40, 70]，核函数名为 `mmad_custom`。

**场景1 int4b_t输入，int32_t输出，A不转置，B转置，不带Bias**

- 输入：A不转置 [30, 70] int4b_t类型，ND格式；B转置 [40, 70] int4b_t类型，ND格式；不带Bias
- 输出：C [30, 40] int32_t类型，ND格式
- 实现：使用`Mmad`实现矩阵乘法运算，通过参数：`mmadParams.cmatrixInitVal = true`，设置C01初始值为0
- 说明：该场景仅支持Atlas A3 训练系列产品/Atlas A3 推理系列产品/Atlas A2 训练系列产品/Atlas A2 推理系列产品，且不支持使用传入biasTensor的方式加Bias（场景4）。

**场景2 int8_t输入，int32_t输出，A不转置，B不转置，带Bias**
- 输入：A不转置 [30, 70] int8_t类型，ND格式；B不转置 [70, 40] int8_t类型，ND格式；Bias [40] int32_t类型
- 输出：C [30, 40] int32_t类型，ND格式
- 实现：使用`Mmad`实现矩阵乘法运算，不传入biasTensor通过参数：`mmadParams.cmatrixInitVal = false、mmadParams.cmatrixSource = true`，设置C01初始值来源于C2
- 说明：输入B不转置，数据从L1（NZ）搬运到L0B（ZN）时，需要转置。b8类型转置需要两个(16, 32)分形合并成一个(32, 32)进行转置。如图1所示，由于B转置过程N轴向2 * 16对齐，填充了一个全部是无效数据的32 * 16的分形，Mmad计算时要设置：`mmadParams.n = AlignUp(N, BLOCK_CUBE * fractalNum)`，避免cube单元数据读入错误。
<p align="center">
  <img src="img/mmad_s8_L0B_转置.png" width="700">
</p>

**场景3 bfloat16输入，float输出，A不转置，B转置，不带Bias，累加CO1初始值**
- 输入：A不转置 [30, 70] bfloat16类型，ND格式；B转置 [40, 70] bfloat16类型，ND格式；不带Bias，累加CO1初始值
- 输出：C [30, 40] float类型，ND格式
- 实现：使用`Mmad`实现矩阵乘法运算，通过参数：`mmadParams.cmatrixInitVal = false、mmadParams.cmatrixSource = false`，设置计算结果累加来源于C01的初始值
- 说明：该场景进行两次Mmad计算，前次计算位于结果CO1作为，下一次计算的累加C01的初始值，最终累加两次Mmad计算结果。

**场景4 float输入，float输出，A转置，B转置，带Bias**
- 输入：A转置 [70, 30] float类型，ND格式；B转置 [40, 70] float类型，ND格式；Bias [40] float类型
- 输出：C [30, 40] float类型，ND格式
- 实现：使用`Mmad`实现矩阵乘法运算，传入biasTensor，该场景下`mmadParams.cmatrixSource`参数无效
- 说明：A转置，b32类型需要将2个(8, 16)的分型拼成1个(16, 16)进行转置。只有当L1上排布为ZZ时，LoadDataWithTranspose才能读2个连续的小分型，如图2所示，满足转置要求，可使用LoadDataWithTranspose指令实现L1->L0A转置搬运。该场景下K轴实际向8 * 2对齐，与Mmad指令默认不一致，又因为K为A、B矩阵公共轴，也不能像场景2那样设置：`mmadParams.k = AlignUp(K, c0Size * fractalNum)`。此时需要引入`mmadParams.kDirectionAlign`来解决，该参数仅在cube单元读取L0A数据且数据类型为float时生效。当A矩阵转置时，该参数需要设置为真，此时A矩阵在L0A上K轴向16对齐，矩阵计算单元从L0A读取数据会跳过填充的无效数据，其余场景下该参数不生效，此时L0B上B矩阵在K仍向8对齐。如下图所示，图2为Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品，图3为Ascend 950PR/Ascend 950DT。
<p align="center">
  <img src="img/mmad_f32_L0A转置_A3.png" width="900">
</p>
<p align="center">
  <img src="img/mmad_f32_L0A转置_A5.png" width="900">
</p>

### 矩阵乘法（Mmad）

下面将介绍如何配置
[Mmad](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0249.html)指令的MmadParams结构体的成员，各个成员变量的具体含义这里不再赘述。

需要注意的是当Mmad指令执行时，矩阵计算单元会从L0A/L0B连续读入多个分形参与矩阵乘计算，读入分形的数量根据MmadParams结构体的成员变量m、n、k的取值以及Mmad指令对L0A/L0B上A矩阵和B矩阵各个轴的对齐要求来计算的。以输入为b16类型为例：Mmad指令是按照A矩阵分形为[16,16]、B矩阵分形为[16,16]来连续读入分形的，也就是说矩阵计算单元从L0A/L0B连续读入的分形总数目分别为：2x5=10、5x3=15，写入到L0C的分形总数为2x3=6。如下图所示，图2表示Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品，图3表示Ascend 950PR/Ascend 950DT，两者在L0A上的数据排布不一致，前者为ZZ，后者则是NZ。
<p align="center">
  <img src="img/mmad_f16_A3.png" width="900">
</p>
<p align="center">
  <img src="img/mmad_f16_A5.png" width="900">
</p>

Mmad计算中包含了补齐的无效数据，需要配合Fixpipe指令在L0C搬出到GM过程中，排除参数Mmad计算过程中填充的无效数据。其中`fixpipeParams.srcStride`参数的取值是L0C上C矩阵M轴向16对齐后的长度，即：AlignUp(M, BLOCK_CUBE)，其含义是源NZ矩阵中相邻Z排布的起始地址偏移。


## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
  - 默认路径，root用户安装CANN软件包
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
  
- 样例执行
  ```bash
  SCENARIO_NUM=1 M=30 K=70 N=50
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DRUN_MODE=npu -DNPU_ARCH=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM -DM_SIZE=$M -DK_SIZE=$K -DN_SIZE=$N;make -j;    # 编译工程
  python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM -m=$M -k=$K -n=$N   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenario_num=$SCENARIO_NUM output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  - 编译选项说明  
    | 选项 | 可选值 | 说明 |
    |------|--------|------|
    | `RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU仿真 |
    | `NPU_ARCH` | `dav-2201`（默认）、`dav-3510` | NPU 架构，dav-2201 对应 Atlas A2/A3 系列，dav-3510 对应 Ascend 950PR/Ascend 950DT |
    | `SCENARIO_NUM` | 1，2，3，4 | 对应本文场景详细说明中的4种场景。 |
    | `M` | 30 | 对应Mmad计算时的M轴大小 |
    | `K` | 70 | 对应Mmad计算时的K轴大小 |
    | `N` | 40 | 对应Mmad计算时的N轴大小 |
    
  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```