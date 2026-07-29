# DataCopy ub2l1样例

## 概述

本样例在Mmad矩阵乘场景下，基于DataCopy实现UB（Unified Buffer）到L1（L1 Buffer）的数据搬运，覆盖连续搬运和随路ND2NZ搬运两种场景。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## 目录结构介绍

```
├── data_copy_ub2l1
│   ├── scripts
│   │   ├── gen_data.py                   // 输入数据和真值数据生成脚本
│   ├── CMakeLists.txt                    // 编译工程文件
│   ├── data_utils.h                      // 数据读入写出函数
│   ├── data_copy_ub2l1.asc               // AscendC样例实现 & 调用样例
│   └── README.md                         // 样例说明文档
```

## 样例描述

- 样例功能：  
  将数据从UB（Unified Buffer）搬运到L1（L1 Buffer），然后进行Mmad矩阵乘计算，最后通过Fixpipe搬出到GM（Global Memory）。

- 场景说明：  
  通过编译选项 `SCENARIO_NUM` 切换两种搬运场景：

  | 场景 | SCENARIO_NUM | M-N-K | 搬运接口 | 输入数据格式 | 说明 |
  |------|-------------|-------|---------|------------|------|
  | 连续搬运 | 1 | 128-128-128（对齐） | `DataCopy(dst, src, count)` | NZ | UB→L1 数据内容不变，输入需预先转为NZ格式 |
  | 随路ND2NZ搬运 | 2 | 120-120-120（非对齐，对齐后为128-128-128） | `DataCopyPad` + `DataCopy(dst, src, DataCopyParams)` + `DataCopy(l1Dst, ubNZ, count)` | ND | 先用DataCopyPad将GM的ND数据搬运至UB并完成width方向padding，再在UB内逐C0列块搬运完成ND→NZ转换同时完成height方向padding，最后连续搬运至L1 |

  接口资料参考[UBToL1连续数据搬运](../../../../../docs/zh/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬入/DataCopy（UBToL1连续数据搬运）.md)和[UBToL1随路转换-ND2NZ搬运](../../../../../docs/zh/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬入/DataCopy（UBToL1随路转换-ND2NZ搬运）.md)。

- 关于场景2实现方案的说明：
  
  场景2未直接使用 `DataCopy(dst, src, Nd2NzParams)` 接口，而是在UB内完成ND→NZ转换，再用连续搬运 `DataCopy(l1Dst, ubNZ, count)` 搬至L1。因为 `DataCopy(dst, src, Nd2NzParams)` 接口为软件仿真实现，硬件本身不支持该能力，如果有业务场景需要，推荐用户根据自身业务场景设计UB→L1的ND2NZ搬运方案。

  场景2采用非对齐shape（M=N=K=120，对齐后为128），在UB上完成ND2NZ转换的同时完成矩阵对齐（padding），具体分为两个对齐维度：

  1. **Width方向对齐**：GM中的ND数据行宽为120（非16的倍数），UB内无法直接按32字节块进行strided搬运。因此GM→UB阶段使用 `DataCopyPad` 接口，将每行搬运时在右侧padding至128（16对齐），使UB中每行列宽为128，行步长32字节对齐。padding值设为0。

  2. **Height方向对齐**：UB中padded ND数据仅有120行，而NZ格式要求128行（8个16行的分形块）。在ND2NZ转换前，先用 `Duplicate` 接口将NZ目标区域（128×128）零填充，然后逐列块搬运120行有效数据，剩余8行保持零填充。

  场景2的数据流和流水线同步如下：
  1. GM→UB（MTE2管线）：使用DataCopyPad将ND数据搬入UB源区域，同时完成width方向padding。
  2. `MTE2_V`同步：等待GM→UB完成。
  3. UB→UB零填充（V管线）：使用Duplicate将NZ目标区域零填充，处理height方向padding。
  4. UB→UB逐列块搬运（V管线）：将padded ND数据的每个C0列块按NZ排布写入UB临时区域，仅搬运有效height行。
  5. `V_MTE3`同步：等待UB→UB转换完成。
  6. UB→L1连续搬运（MTE3管线）：将UB临时区域的NZ数据搬至L1。
  7. `MTE3_V`同步：等待A矩阵搬运完成后再处理B矩阵，避免共享temp区域读写冲突。

- 样例规格：
  <table>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape（场景1 / 场景2）</td><td align="center">data type</td><td align="center">format（场景1 / 场景2）</td></tr>
  <tr><td align="center">x</td><td align="center">[128, 128] / [120, 120]</td><td align="center">half</td><td align="center">NZ / ND</td></tr>
  <tr><td align="center">y</td><td align="center">[128, 128] / [120, 120]</td><td align="center">half</td><td align="center">NZ / ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[128, 128] / [120, 120]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">data_copy_ub2l1</td></tr>
  </table>

- 样例实现：
  1. AIV核：将数据从GM（Global Memory）搬运到UB（Unified Buffer）。两种场景下GM中的数据排布不同：场景1为NZ格式（分形间列主序、分形内行主序，由 `gen_data.py` 预转换）；场景2为ND格式（原始行主序，无需预转换），搬运时通过DataCopyPad完成width方向padding。
  2. AIV核：将数据从UB搬运到L1（L1 Buffer）。场景1使用连续搬运；场景2先在UB内完成ND→NZ转换（包含height方向padding），再用连续搬运搬至L1。两种场景到达L1后的数据排布相同，后续计算流程完全一致。
  3. AIC核：调用基础API LoadData将数据从L1搬运到L0A Buffer与L0B Buffer。
  4. AIC核：调用基础API Mmad进行矩阵乘计算。
  5. AIC核：调用基础API Fixpipe将数据从L0C Buffer搬运到GM（Global Memory）。

- 调用实现  
  使用内核调用符<<<>>>调用核函数，核函数声明为 `__mix__(1, 2)` 混合核（1个AIC + 2个AIV）。
    
## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。
- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。
  ```bash
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..;make -j;     # 编译工程，默认npu模式，场景1
  python3 ../scripts/gen_data.py --scenarioNum=1                            # 生成测试输入数据（场景需与编译一致）
  ./demo                                                                    # 执行编译生成的可执行程序，执行样例
  ```

  > **注意：** `gen_data.py` 的 `--scenarioNum` 参数必须与 cmake 的 `-DSCENARIO_NUM` 保持一致，否则输入数据格式不匹配会导致精度校验失败。

  使用 CPU调试 或 NPU仿真 模式时，添加 `-DCMAKE_ASC_RUN_MODE=cpu` 或 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。
  
  示例如：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..;make -j; # cpu调试模式，场景1
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=2 ..;make -j; # NPU仿真模式，场景2
  ```

  > **注意：** 切换编译模式或场景前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | ---- | ------ | ---- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU 运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：dav-3510 对应 Ascend 950PR/950DT |
  | `SCENARIO_NUM` | `1`（默认）、`2` | 搬运场景：1=连续搬运（NZ输入，128×128对齐），2=随路ND2NZ搬运（ND输入，120×120非对齐padding至128×128） |

- 执行结果

  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
