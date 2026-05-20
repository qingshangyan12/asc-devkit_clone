# Matmul和LeakyRelu融合样例

## 概述

本样例采用高阶API实现Matmul与LeakyRelu激活函数融合计算，实现矩阵运算单元和向量运算单元的融合计算。

## 支持的产品

- Ascend 950PR/Ascend 950DT
- Atlas A3 训练系列产品/Atlas A3 推理系列产品
- Atlas A2 训练系列产品/Atlas A2 推理系列产品

## 目录结构介绍

```
├── matmul_leakyrelu_high_level_api
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── data_utils.h            // 数据读入写出函数
│   ├── matmul_leakyrelu.asc    // Ascend C样例实现 & 调用样例
│   └── scripts
│       ├── gen_data.py         // 输入数据和真值数据生成脚本文件
│       └── verify_result.py    // 真值对比文件
```

## 样例描述

- 样例功能：
  本样例使用Ascend C Matmul高阶API实现矩阵乘、Bias加法和LeakyRelu激活函数的融合计算。Matmul高阶API封装了矩阵计算过程中的数据搬运、Cube计算调度、基础流水同步等细节，开发者主要需要完成矩阵规格配置、tiling生成、输入Tensor设置、Vector侧融合计算和结果写回。

  MatmulLeakyRelu的计算公式如下：
  ```
  C = A * B + Bias
  C = C > 0 ? C : C * 0.001
  ```
  其中，A矩阵形状为`[M, K]`，B矩阵形状为`[K, N]`，Bias形状为`[N]`，输出C矩阵形状为`[M, N]`。矩阵乘结果会先加上Bias，再对每个输出元素执行LeakyRelu激活。

- 样例规格：
  本样例参数`M = 512, N = 16, K = 256`。代码中A矩阵K轴使用`Ka`表示，B矩阵K轴使用`Kb`表示，本样例`Ka = Kb = K = 256`。输入A、B、Bias和输出C均为`float`类型、`ND`格式。输入输出规格如下表所示：
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">MatmulLeakyRelu</td></tr>
  <tr><td rowspan="4" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">A</td><td align="center">[M, K]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">B</td><td align="center">[K, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">Bias</td><td align="center">[N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">C</td><td align="center">[M, N]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">matmul_leakyrelu_custom</td></tr>
  </table>

  样例采用`__mix__(1, 2)`声明核函数，表示核函数按Cube:Vector为1:2的比例执行。Host侧以`numBlocks = 2`启动核函数，因此运行时包含2个AI Core组合，每个组合包含1个Cube核和2个Vector核，即实际启动2个Cube核和4个Vector核。当前tiling只按M轴分核，N轴和K轴不分核，每个AIV逻辑核负责输出矩阵C中`singleCoreM = 128`行、N方向完整16列的数据。

- 样例实现：
  - Tiling生成流程
    - `GenerateTiling`中固定设置矩阵规格：`M = 512`、`N = 16`、`K = 256`。
    - 创建`matmul_tiling::MultiCoreMatmulTiling`对象`tilingApi`，用于生成多核Matmul所需的tiling参数。
    - `SetDim(4)`表示tiling侧配置4个AIV逻辑核参与Matmul计算发起。分离架构MIX场景下，Matmul高阶API由AIV侧发起，调用`Iterate`时由AIV通知AIC执行矩阵计算。
    - Host侧kernel launch的`numBlocks`表示实际启动的AI Core组合数量。本样例使用`__mix__(1, 2)`，每个组合包含1个Cube核和2个Vector核，因此`SetDim(4)`对应`numBlocks = 2`。
    - `SetAType`、`SetBType`、`SetCType`、`SetBiasType`分别设置A、B、C、Bias的数据来源位置、数据格式和数据类型。本样例A/B/Bias位于GM，C类型设置为`VECIN`，表示Matmul中间结果输出到Vector侧可读取的位置，便于后续执行LeakyRelu激活。
    - `SetOrgShape(M, N, K)`设置原始完整矩阵形状，`SetShape(M, N, K)`设置本次实际参与Matmul计算的`m/n/k`。本样例计算完整矩阵乘，因此两者都设置为`M/N/K`。
    - `EnableBias(true)`表示Matmul计算带Bias输入。
    - `SetFixSplit(64, 16, 256)`固定Matmul基础分块大小，使Kernel侧按`baseM * baseN = 64 * 16`处理Matmul输出分块并执行后续Vector计算。
    - `SetBufferSpace(-1, -1, -1)`设置Matmul可使用的L1/L0C/UB空间大小。传入`-1`表示使用当前AI处理器对应buffer的默认大小。
    - `GetTiling(tilingData)`生成最终tiling结果。如果返回`-1`表示tiling计算失败，tiling结果不可继续使用。

  - Kernel侧整体思路
    - `matmul_leakyrelu_custom`是一个`__global__ __mix__(1, 2)`核函数，运行时同时包含Cube侧和Vector侧计算逻辑。
    - Kernel入参中的A、B、Bias、C均使用`__gm__ uint8_t*`表示GM上的字节地址，Kernel内部通过模板参数`T`转换为`GlobalTensor<T>`使用。本样例调用`matmul_leakyrelu_custom<float>`，因此实际计算类型为`float`。
    - workspace参数使用`__kfc_workspace__`修饰，表示需要使用系统workspace。Kernel侧通过`GetSysWorkSpacePtr()`将系统workspace传递给`REGIST_MATMUL_OBJ`。
    - 创建`GlobalTensor`对象`aGlobal`、`bGlobal`、`cGlobal`、`biasGlobal`，分别表示GM中的A、B、C、Bias数据。`GlobalTensor`只描述GM上的地址和元素个数，真正的数据搬运、L1/L0切分由Matmul高阶API结合tiling完成。
    - 当前样例只按M轴分核。`SetDim(4)`生成4个AIV逻辑核的Matmul分核参数；Host侧`numBlocks = 2`启动2个AI Core组合，每个组合包含1个Cube核和2个Vector核。
    - Kernel侧通过`GetBlockIdx()`获取当前AI Core组合编号，用于计算A矩阵和C矩阵在M方向的起始偏移。Matmul高阶API内部负责处理当前组合中AIV发起、AIC执行以及AIV获取结果的协同过程。
    - A矩阵和C矩阵需要按照逻辑块编号在M方向偏移，B矩阵和Bias在各个逻辑块上复用起始地址。
    - 创建`TQue<AscendC::TPosition::VECOUT, 1>`管理Vector侧LeakyRelu输出缓存，并通过`pipe.InitBuffer`为单个`baseM * baseN`输出分块申请片上缓存。
    - 创建Matmul高阶API对象`matmulObj`。`MatmulType`模板参数描述A/B/C/Bias在Kernel侧的位置、格式和数据类型，其中C使用`VECIN`，表示Matmul结果输出到Vector侧可读取的位置。
    - 通过`REGIST_MATMUL_OBJ(&pipe, GetSysWorkSpacePtr(), matmulObj, &tiling)`注册Matmul对象。注册时会把`TPipe`、系统workspace、Matmul对象和对应`TCubeTiling`交给Matmul高阶API内部使用。
    - 调用`SetTensorA`、`SetTensorB`、`SetBias`绑定当前逻辑块需要读取的A、B、Bias输入。

  - Matmul与LeakyRelu融合流程
    - `while (matmulObj.template Iterate<true>())`通过单次`Iterate`叠加循环的方式完成当前逻辑块负责的多个Matmul输出分块。每次`Iterate<true>()`完成一个`baseM * baseN`输出分块的Matmul计算。
    - `reluOutQueue.AllocTensor<T>()`从Vector输出队列申请LocalTensor缓存，用于承接Matmul输出分块。
    - `matmulObj.template GetTensorC<true>(reluOutLocal, false, true)`从Matmul对象获取当前分块的C中间结果。由于C类型设置为`VECIN`，该中间结果可以直接作为Vector侧LeakyRelu输入。
    - `AscendC::LeakyRelu(reluOutLocal, reluOutLocal, (T)0.001, tiling.baseM * tiling.baseN)`对当前分块执行逐元素LeakyRelu激活，并将结果原地写回`reluOutLocal`。
    - `reluOutQueue.EnQue`和`reluOutQueue.DeQue`用于建立Vector计算和搬出之间的数据依赖。
    - `AscendC::DataCopy`将激活后的`baseM * baseN`分块搬运到GM输出矩阵C的对应位置。
    - `matmulObj.End()`通知Matmul对象当前核计算结束，释放Matmul高阶API内部状态。

  - 分核和地址偏移说明
    - 本样例`SetDim(4)`按4个AIV逻辑核生成tiling，tiling结果按M轴切分：`M = 512`、`singleCoreM = 128`，因此输出C矩阵被分成4个M轴分片。
    - Host侧`numBlocks = 2`启动2个AI Core组合。由于`__mix__(1, 2)`表示每个组合包含2个Vector核，2个组合共包含4个Vector核，与`SetDim(4)`配置的AIV逻辑核数对应。
    - 第0个AIV逻辑核处理C矩阵第0到127行，第1个AIV逻辑核处理第128到255行，第2个AIV逻辑核处理第256到383行，第3个AIV逻辑核处理第384到511行。
    - A矩阵形状为`[512, 256]`，每一行长度为`tiling.Ka = 256`，所以A矩阵偏移为`blockIdx * tiling.singleCoreM * tiling.Ka`。
    - B矩阵形状为`[256, 16]`，对应代码中的`[Kb, N]`。不同逻辑块负责C矩阵的不同行，但每个输出元素都需要完整K轴累加，因此各个逻辑块都从`bGlobal[0]`读取完整B矩阵。
    - Bias形状为`[16]`，对应N方向的16列，各个逻辑块复用同一份Bias数据。
    - C矩阵形状为`[512, 16]`，每一行长度为`tiling.N = 16`，所以C矩阵逻辑块起始偏移为`blockIdx * tiling.singleCoreM * tiling.N`。
    - 每个逻辑块内部包含`singleCoreM / baseM = 2`个`baseM * baseN`输出分块。`computeRound`表示当前逻辑块内的分块轮次，`startOffset`根据`computeRound`计算当前分块在该逻辑块C输出区域内的写回偏移。
    - 由于M轴正好被4个逻辑块均分，且N轴等于`baseN`，代码中没有尾块处理逻辑。

  - 调用实现
    使用内核调用符`<<<>>>`调用核函数。调用时`numBlocks`固定为2，表示启动2个AI Core组合；运行时参数依次传入Device侧A矩阵地址、B矩阵地址、Bias地址、C矩阵地址、workspace地址和Host侧生成的tiling数据。

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
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU 架构：dav-2201 对应 Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品，dav-3510 对应 Ascend 950PR/Ascend 950DT |

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
