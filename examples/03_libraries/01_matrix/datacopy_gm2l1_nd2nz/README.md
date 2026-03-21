# DataCopy GM2L1 ND2NZ样例
## 概述
本样例基于基础API DataCopy实现Global Memory到L1 Buffer的数据搬运，并在搬运过程中实现ND2NZ格式转换，数据通路为：GM->A1、B1。

## 支持的产品
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍
```
├── datacopy_gm2l1_nd2nz
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   └── datacopy_gm2l1_nd2nz.asc    // Ascend C算子实现 & 调用样例
```

## 算子描述
本样例实现GM到L1的数据搬运，并使用ND2NZ格式转换。ND2NZ结构体参数定义如下：

| 参数名称 | 含义 |
|:--------:|:------:|
| ndNum | 源矩阵中ND矩阵的数量，取值范围：ndNum∈[0, 4095]<br>注：ndNum = 0表示不执行搬运，即空操作。 |
| nValue | 源矩阵中ND矩阵的行数，取值范围：nValue∈[0, 16384]<br>注：nValue = 0表示不执行搬运，即空操作。  |
| dValue | 源矩阵中ND矩阵的列数，取值范围：nValue∈[0, 65535]，当dValue*sizeof(T)不满足32B对齐时，在目的矩阵中会补0对齐到32B<br>注：dValue = 0表示不执行搬运，即空操作。  |
| srcNdMatrixStride | 源矩阵相邻ND矩阵起始地址间的偏移，取值范围：srcNdMatrixStride∈[0, 65535]，单位为元素个数。 <br>ndNum等于1时，srcNdMatrixStride无意义，设置为0即可。 ndNum不等于1时，srcNdMatrixStride为0时，表示重复搬第一个ND矩阵。|
| srcDValue | 源矩阵一行中包含的元素，取值范围：srcDValue∈[1, 65535]，单位为元素个数 |
| dstNzC0Stride | ND转换为NZ格式后，目的NZ矩阵中相邻Z矩阵起始地址的偏移，取值范围dstNzC0Stride∈[1, 16384]，单位为C0_SIZE（32B）个数 |
| dstNzNStride | 目的矩阵中NZ矩阵相邻行起始地址的偏移，取值范围：dstNzNStride∈[1, 16384]，单位为C0_SIZE（32B）个数 |
| dstNzMatrixStride | 目的矩阵中相邻NZ矩阵起始地址的偏移，取值范围：dstNzMatrixStride∈[0, 65535]，单位为元素个数 |
  

本样例使用Ascend C的DataCopy接口实现数据搬运，主要步骤如下：
  1. 使用DataCopy接口将数据从GM搬运到L1，同时进行ND到NZ的格式转换
  2. 使用SetFlag和WaitFlag进行流水线同步
  3. 将转换后的NZ格式数据直接从L1搬运回GM

本样例基于操作数数据类型和dValue是否对齐到32B等特性，支持以下7种场景：

| 场景编号<br>SCENARIONUM | 数据类型 | ndNum | nValue | dValue | dValue对齐 | 特点 |
|:--------:|:------:|:--------:|:--------:|:--------:|:--------:|:--------:|
| 1 | half | 1 | 16 | 32 | 是 | 单矩阵，对齐 |
| 2 | float | 1 | 16 | 32 | 是 | 单矩阵，对齐 |
| 3 | int8_t | 1 | 16 | 32 | 是 | 单矩阵，对齐 |
| 4 | half | 1 | 16 | 23 | 否 | 单矩阵，非对齐 |
| 5 | float | 1 | 16 | 23 | 否 | 单矩阵，非对齐 |
| 6 | int8_t | 1 | 16 | 23 | 否 | 单矩阵，非对齐 |
| 7 | half | 2 | 8 | 23 | 否 | 多矩阵，非连续 |

### 场景参数详解

<details>
<summary>场景1-3：单矩阵，dValue对齐到32B</summary>

```cpp
// 16x32矩阵，dValue=32对齐到32B
intriParams.ndNum = 1;
intriParams.nValue = 16;
intriParams.dValue = 32;
intriParams.srcNdMatrixStride = 0;
intriParams.srcDValue = 32;
intriParams.dstNzC0Stride = 16;
intriParams.dstNzNStride = 1;
intriParams.dstNzMatrixStride = 0;
```
</details>

<details>
<summary>场景4-6：单矩阵，dValue未对齐到32B</summary>

```cpp
// 16x23矩阵，dValue=23未对齐到32B，搬运时自动补零
intriParams.ndNum = 1;
intriParams.nValue = 16;
intriParams.dValue = 23;
intriParams.srcNdMatrixStride = 0;
intriParams.srcDValue = 32;
intriParams.dstNzC0Stride = 16;
intriParams.dstNzNStride = 1;
intriParams.dstNzMatrixStride = 0;
```
</details>

<details>
<summary>场景7：多矩阵，非连续搬运</summary>

```cpp
// 2个8x23矩阵，源数据非连续存储
intriParams.ndNum = 2;
intriParams.nValue = 8;
intriParams.dValue = 23;
intriParams.srcNdMatrixStride = 384;  // 源矩阵间隔384元素
intriParams.srcDValue = 32;
intriParams.dstNzC0Stride = 37;
intriParams.dstNzNStride = 2;
intriParams.dstNzMatrixStride = 320;  // 目标矩阵间隔320元素
```

输入数据布局：20x32=640元素
- 矩阵1：第0-255元素（8行×32列）
- 间隔：第256-383元素（不搬运）
- 矩阵2：第384-639元素（8行×32列）
</details>

## 编译运行
在本样例根目录下执行如下步骤，编译并执行算子。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  SCENARIONUM=7
  mkdir -p build && cd build;      # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIONUM;make -j;   # 编译工程
  python3 ../scripts/gen_data.py -scenario_num=$SCENARIONUM  # 生成测试输入数据
  ./demo                           # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py -scenario_num=$SCENARIONUM output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```