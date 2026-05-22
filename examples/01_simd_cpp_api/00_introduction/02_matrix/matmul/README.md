# 基于静态Tensor编程实现Matmul计算

## 概述

本样例基于静态Tensor编程范式实现多核矩阵乘计算。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── matmul
│   ├── scripts
│   │   ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│   │   └── verify_result.py    // 真值对比文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   └── matmul.asc              // Ascend C样例实现 & 调用样例
```

## 样例描述

- 样例功能：  
  Matmul计算公式：
  $$
  C = A * B
  $$
- 样例规格：  
  本样例参数M = 128, N = 128, K = 128，调用2个核完成计算，输入规格如下表所示：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">Matmul</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">mmad_custom</td></tr>
  </table>

- 样例实现：
  - **实现流程**
    | 阶段 | 数据流动 | 主要计算接口 | 说明 |
    |------|----------|----------|------|
    | Stage 1<br>初始化与分核 | — | `InitSocState` | 初始化片上系统状态；定义`GlobalTensor`/`LocalTensor`指向GM和各层级Buffer；</br>获取当前核索引，每个核处理M维的一半（localM=M/2），实现2核并行 |
    | Stage 2<br>Memory数据搬运 | GM → L1 | `DataCopy` | 将矩阵A和B从GM搬运到L1，同时完成ND→NZ格式转换。A每个核搬运localM×K，B搬运完整K×N（核间共享）。</br>之后`PipeBarrier<PIPE_ALL>()`同步 |
    | Stage 3<br>矩阵数据搬运 | L1 → L0A/L0B | `LoadData` | 将L1中NZ格式数据搬运到Cube输入Buffer L0A/L0B。</br> dav-2201使用`LoadData2DParams`，dav-3510使用`LoadData2DParamsV2`。</br>之后`PipeBarrier<PIPE_ALL>()`同步 |
    | Stage 4<br>Cube矩阵乘 | L0A×L0B→L0C | `Mmad` | Cube单元执行矩阵乘：C\[localM\]\[N\] = A\[localM\]\[K\] × B\[K\]\[N\]，结果写入L0C。</br>之后`PipeBarrier<PIPE_ALL>()`同步 |
    | Stage 5<br>计算结果矩阵数据搬运 | L0C → GM | `Fixpipe` | 将L0C中的float32结果转为half后搬回GM对应核的输出区域。</br>之后`PipeBarrier<PIPE_ALL>()`同步 |

  - **接口参数说明**

    以下结构体均以花括号`{}`方式传参，各字段含义如下（字段顺序与API文档保持一致，实际struct声明中部分字段顺序可能不同）：

    **`AscendC::Nd2NzParams`** — `DataCopy`接口使用，描述ND→NZ格式转换参数：
    ```cpp
    struct Nd2NzParams {
        int32_t  ndNum;              // 传输ND矩阵的数目，[0, 4095]
        uint16_t nValue;             // ND矩阵的行数，[0, 16384]
        int32_t  dValue;             // ND矩阵的列数，[0, 65535]
        int32_t  srcNdMatrixStride;  // 相邻ND矩阵起始地址偏移，单位：元素，[0, 65535]
        int32_t  srcDValue;          // 同一ND矩阵相邻行偏移，单位：元素，[1, 65535]
        uint16_t dstNzC0Stride;      // 目的NZ中同源行转换后多行相邻偏移，单位：C0_SIZE(32B)，[1, 16384]
        uint16_t dstNzNStride;       // 目的NZ中Z型矩阵相邻行偏移，单位：C0_SIZE(32B)，[1, 16384]
        int32_t  dstNzMatrixStride;  // 目的NZ中相邻NZ矩阵起始地址偏移，单位：元素，[1, 65535]
    };
    ```
    例如搬运A矩阵时`{1, localM, K, 0, K, localM, 1, 0}`，将localM×K的ND数据转为NZ格式。

    **`AscendC::LoadData2DParams`** — `LoadData`接口使用（dav-2201架构）：
    ```cpp
    struct LoadData2DParams {
        int32_t  startIndex;   // 分形矩阵ID（0为第1个），单位：512B，[0, 65535]
        int32_t  repeatTimes;  // 迭代次数，每个迭代处理512B，[1, 255]
        int32_t  srcStride;    // 相邻迭代源分形起始地址间隔，单位：512B，[0, 65535]
        int32_t  sid;          // 预留，配置为0
        int32_t  dstGap;       // 目的端相邻迭代分形间隔，单位：512B，[0, 65535]
        bool     ifTranspose;  // 是否转置每个分形，默认false
        bool     addrMode;     // 地址更新方式，false=递增，true=递减，默认false
    };
    ```
    例如搬运A矩阵时`{0, 8, 4, 0, 0, false, 0}`，从第0个分形开始，重复8次，源步长4 blocks。B矩阵搬运时`ifTranspose=true`使能转置。

    **`AscendC::LoadData2DParamsV2`** — `LoadData`接口使用（dav-3510架构）：
    ```cpp
    struct LoadData2DParamsV2 {
        int32_t  mStartPosition;  // 源矩阵M轴起始位置，单位：16个元素
        int32_t  kStartPosition;  // 源矩阵K轴起始位置，单位：32B
        uint16_t mStep;           // M轴搬运长度，单位：16 element，[0, 255]
        int32_t  kStep;           // K轴搬运长度，单位：32B，[0, 255]
        int32_t  srcStride;       // 源K方向相邻分形间隔，单位：512B
        int32_t  dstStride;       // 目的K方向相邻分形间隔，单位：512B
        bool     ifTranspose;     // 是否转置，默认false
        int32_t  sid;             // 预留，配置为0
    };
    ```
    例如搬运A矩阵时`{0, 0, 4, 8, 4, 4, false, 0}`，从M/K起点搬运4×8个分形到L0A。B矩阵时`ifTranspose=true`使能转置。

    **`AscendC::MmadParams`** — `Mmad`接口使用，描述矩阵乘参数：
    ```cpp
    struct MmadParams {
        uint16_t m;               // 左矩阵Height（M维），[0, 4095]
        uint16_t n;               // 右矩阵Width（N维），[0, 4095]
        uint16_t k;               // 左矩阵Width/右矩阵Height（K维），[0, 4095]
        bool     cmatrixInitVal;  // C矩阵初始值是否为0，默认true
        bool     cmatrixSource;   // C矩阵初始值来源，false=CO1，true=C2，默认false
        bool     isBias;          // （已废弃）是否累加，false=C=A*B，true=C+=A*B，默认false
        bool     disableGemv;     // M=1时控制是否开启GEMV，默认false
        uint16_t unitFlag;        // Mmad与Fixpipe细粒度并行控制，默认0
        bool     kDirectionAlign; // 是否需要对齐，默认false
    };
    ```
    例如`{localM, N, K, false, false, true}`，其余参数默认，结果为C+=A*B（从CO1初始值累加）。

    **`AscendC::FixpipeParamsV220`** — `Fixpipe`接口使用，描述数据搬移和精度转换参数：
    ```cpp
    struct FixpipeParamsV220 {
        int32_t      nSize;          // 源NZ矩阵N方向大小，[1, 4095]
        uint16_t     mSize;          // 源NZ矩阵M方向大小（NZ2ND时[1, 8192]）
        uint16_t     srcStride;      // 源NZ相邻Z排布起始偏移，单位：C0_Size，[0, 65535]
        int32_t      dstStride;      // NZ2ND时目的ND矩阵每行元素数，单位：element
        bool         reluEn;         // 是否使能ReLU
        QuantMode_t  quantPre;       // 量化模式，F322F16表示float→half (CAST_RINT)
        uint64_t     deqScalar;      // scalar量化参数，单个scale值
        int32_t      ndNum;          // 源NZ矩阵数目，[1, 65535]
        int32_t      srcNdStride;    // 不同NZ矩阵起始地址间隔，单位：16×C0_Size，[1, 512]
        int32_t      dstNdStride;    // 目的相邻ND矩阵偏移，单位：element，[1, 65535]
        int32_t      unitFlag;       // Mmad与Fixpipe并行控制
        bool         isChannelSplit; // 是否通道拆分，默认false
    };
    ```
    例如`{N, localM, localM, N, false, F322F16, 0, 1, 0, 0, 0}`，将L0C中localM×N的float32数据转为half写入GM。

  - **调用实现**  
    使用内核调用符`<<<>>>`调用核函数：
    ```cpp
    mmad_custom<<<numBlocks, nullptr, stream>>>(aDevice, bDevice, cDevice);
    ```
    其中`numBlocks = 2`表示启动2个核。主程序还负责ACL初始化（`aclInit`）、Device内存申请与释放、Host数据文件读取、结果数据写回等功能

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。
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
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
