# Mmad样例
## 概述
本样例介绍了在int4b_t / int8_t/ half / float 四种输入数据类型下的矩阵乘法，说明如何通过mmad指令实现矩阵乘法计算（C = A x B + Bias）。

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

## 算子描述
一次完整的矩阵乘法涉及的数据搬运过程包括：GM-->L1、L1-->L0A/L0B、L1-->BT（Bias）、L0C-->GM，其中不同存储单元的数据排布格式，如下表1所示：  

<table border="2" align="center">
<caption>表1：不同存储单元的数据排布格式</caption>
  <tr>
    <td >存储单元</td>
    <td>数据排布格式</td>
  </tr>
  <tr>
    <td>GM</td>
    <td>A矩阵为ND排列；B矩阵为DN排列；C矩阵为ND。</td>
  </tr>
  <tr>
    <td>L1</td>
    <td>A矩阵为NZ排列；B矩阵为ZN排列。</td>
  </tr>
  <tr>
    <td>L0A</td>
    <td>Atlas A3 / A2 系列产品的A矩阵为ZZ排列；Ascend 950PR/Ascend 950DT产品的A矩阵为NZ排列。</td>
  </tr>
  <tr>
    <td>L0B</td>
    <td>B矩阵为ZN排列。</td>
  </tr>
  <tr>
    <td>L0C</td>
    <td>C矩阵为NZ排列。</td>
  </tr>
</table>

通常的矩阵乘法计算公式：C = A × B + Bias，其中A、B、C矩阵的需要满足的shape分别为[M,K]、[K,N]和[M,N]。当输入数据类型分别取int8_t/ half / float时，可以通过布尔变量IS_BIAS控制是否启用Bias，当输入为int4b_t时，不支持启用Bias。

下文将介绍，A、B矩阵在完整矩阵乘法流程的数据排布、对齐要求、所用指令以及如何配置相应参数。程序中以参数scenarioNum来代表上述4种场景，scenarioNum不同取值对应的含义，如下表2所示：

<table border="2" align="center">
<caption>表2：scenarioNum不同取值的含义</caption>
  <tr>
    <td >scenarioNum</td>
    <td>输入数据类型</td>
    <td>输出数据类型</td>
    <td>是否支持启用Bias</td>
  </tr>
  <tr>
    <td>1</td>
    <td>int4b_t</td>
    <td>int32_t</td>
    <td>不支持</td>
  </tr>
  <tr>
    <td>2</td>
    <td>int8_t</td>
    <td>int32_t</td>
    <td>支持</td>
  </tr>
  <tr>
    <td>3</td>
    <td>bfloat16</td>
    <td>bfloat16</td>
    <td>支持</td>
  </tr>
  <tr>
    <td>4</td>
    <td>float</td>
    <td>float</td>
    <td>支持</td>
  </tr>
</table>

为了方便描述，在此对后续常用概念给出定义，如下表3所示：
<table border="2" align="center">
<caption>表3：常用概念定义</caption>
  <tr>
    <td>概念</td>
    <td>定义</td>
  </tr>
  <tr>
    <td>fractalSize</td>
    <td>A矩阵或B矩阵的一个分形中包含的元素个数，当输入数据类型分别为int4b_t/int8_t/half/flaot时，该变量取值分别为1024、512、256、128。</td>
  </tr>
  <tr>
    <td>CeilDivision</td>
    <td>向上取整除法，一般用于求解向上对齐后的循环次数。</td>
  </tr>
  <tr>
    <td>CeilAlign</td>
    <td>向上对齐操作，例如mAlign=CeilAlign(30, 16)=32，表示将M轴向16对齐，对齐后M轴长度为32。</td>
  </tr>
</table>

### 矩阵乘法（Mmad）

下面将介绍如何配置
[Mmad](https://www.hiascend.com/document/detail/zh/canncommercial/850/API/ascendcopapi/atlasascendc_api_07_0249.html)指令的MmadParams结构体的成员，各个成员变量的具体含义这里不再赘述。

需要注意的是当Mmad指令执行时，矩阵计算单元会从L0A/L0B连续读入多个分形参与矩阵乘计算，读入分形的数量根据MmadParams结构体的成员变量m、n、k的取值以及Mmad指令对L0A/L0B上A矩阵和B矩阵各个轴的对齐要求来计算的。以输入为int8_t为例：Mmad指令是按照A矩阵分形为[16,32]、B矩阵分形为[32,16]来连续读入分形的，也就是说矩阵计算单元从L0A/L0B连续读入的分形总数目分别为：CeilDivision(m,16)*CeilDivision(k,32)、CeilDivision(k,32)*CeilDivision(n,16)，写入到L0C的分形总数为CeilDivision(m,16)*CeilDivision(n,16)。如下图所示：
<p align="center">
  <img src="img/mmad_s8_L0A.png" width="600">
</p>
<p align="center">
  <img src="img/mmad_s8_L0B.png" width="400">
</p>
<p align="center">
  <img src="img/mmad_s8_L0C.png" width="600">
</p>
显然矩阵计算中包含了补齐的无效数据，通过配置Fixpipe指令参数：fixpipeParams.mSize = m、fixpipeParams.nSize = n、fixpipeParams.srcStride = mAlign，在L0C-->GM计算结果搬出时，去除无效数据。


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
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake ..;make -j;                # 编译工程
  python3 ../scripts/gen_data.py   # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```