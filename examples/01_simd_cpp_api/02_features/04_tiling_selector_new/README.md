# Add Tiling切分样例

## 概述

本样例展示Ascend C中Add算子的多核Tiling切分方式。样例使用`half`数据类型、8个AI Core和静态Tensor编程方式，通过`SCENARIO_NUM`选择不同的数据量，再由Host侧代码按“主块、等长尾块、尾核”逐层消费剩余长度，自动选择Tiling切分方式。

本样例不使用`TPipe`、`TQue`、`TBuf`，也不使用Double Buffer。`Kernel`侧直接构造`LocalTensor`并手动插入`SetFlag`、`WaitFlag`同步事件。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|--------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```text
├── 04_tiling_selector_new
│   ├── scripts
│   │   ├── gen_data.py          // 输入数据和真值数据生成脚本。
│   │   └── verify_result.py     // 输出结果验证脚本。
│   ├── CMakeLists.txt           // 编译工程文件。
│   ├── add.asc                  // 多轻量Tiling结构体样例实现与调用样例。
│   ├── add_unified_tiling.asc   // 统一Tiling结构体样例实现与调用样例。
│   ├── data_utils.h             // 数据读入写出函数。
│   ├── new_README.md            // Tiling计算过程推导说明。
│   └── README.md                // 样例说明文档。
```

## 样例描述

样例实现两个一维Tensor相加：

```text
z = x + y
```

输入输出均为`half`类型。`add.asc`中的核函数名为`add_custom`，`add_unified_tiling.asc`中的核函数名为`add_unified_custom`。

## Tiling场景

样例固定使用8个AI Core，单次核内主块长度为3200个`half`元素。由于`half`为2字节，Ascend C数据搬运以32字节为基本对齐粒度时，一个DataBlock包含16个`half`元素。

Host侧先将`totalLength`按`alignNum`向上对齐，再按以下顺序消费长度：所有核先均分`tileLength`整数倍主块；剩余长度按DataBlock给每个核补齐等长尾块；最后剩余DataBlock分给前若干个整核。

| `SCENARIO_NUM` | 场景（核间+核内） | `totalLength`（个） | 核间切分（个） | 核内切分 | 说明 |
|----------------|-------------------|---------------------|----------------|----------|------|
| `0` | 均分+均分 | `256000` | 8核 * 32000 | 10tile * 3200 | 规整，无尾核、无尾块。 |
| `1` | 均分+尾块 | `260096` | 8核 * 32512 | 10tile * 3200 + 512尾块 | 核间均分，每个核都有尾块。 |
| `2` | 尾核+均分 | `256064` | 4整核 * 32016 + 4尾核 * 32000 | 整核：10tile * 3200 + 16尾块；尾核：10tile * 3200 | 前4个整核多处理一个DataBlock，尾核按主块均分。 |
| `3` | 尾核+尾块 | `258112` | 4整核 * 32272 + 4尾核 * 32256 | 整核尾块272；尾核尾块256 | 存在尾核与尾块。 |

## Host侧Tiling计算过程

Host侧输入为`totalLength`，输出为每个核需要处理的元素长度。计算过程按“主块、等长尾块、尾核”逐层消费剩余长度。

1. 将总长度按DataBlock对齐。

   ```cpp
   totalLengthAligned = AlignUp(totalLength, alignNum);
   ```

2. 所有核先均分`tileLength`整数倍主块。

   ```cpp
   mainTileNum = totalLengthAligned / (numBlocks * tileLength);
   mainTileRemainder = totalLengthAligned % (numBlocks * tileLength);
   ```

   此时每个核至少处理`mainTileNum * tileLength`个元素，`mainTileRemainder`表示所有核均分主块后的剩余长度。

3. 主块剩余长度再按DataBlock给所有核补齐等长尾块。

   ```cpp
   tailBlockNumEachCore = mainTileRemainder / (numBlocks * alignNum);
   formerCoreRemainder = mainTileRemainder % (numBlocks * alignNum);
   baseLength = mainTileNum * tileLength + tailBlockNumEachCore * alignNum;
   ```

   `baseLength`表示每个核的基础处理长度。`formerCoreRemainder`表示每核补齐等长DataBlock尾块后，剩余需要分给前若干个核的长度。

4. 最后剩余的DataBlock分给前若干个整核。

   ```cpp
   if (formerCoreRemainder == 0) {
       formerNum = numBlocks;
       formerLength = baseLength;
       tailNum = 0;
       tailLength = 0;
   } else {
       formerNum = formerCoreRemainder / alignNum;
       formerLength = baseLength + alignNum;
       tailNum = numBlocks - formerNum;
       tailLength = baseLength;
   }
   ```

   `formerCoreRemainder == 0`时不存在尾核，所有核都处理`baseLength`个元素。`formerCoreRemainder != 0`时，`formerNum`表示整核数量，前`formerNum`个核处理`formerLength`个元素；`tailNum`表示尾核数量，剩余核处理`tailLength`个元素。

## 实现要点

- Host侧根据`SCENARIO_NUM`只选择总长度，Tiling模式由`LaunchAddByTiling`根据总长度自动推导。
- `LaunchAddByTiling`先计算`mainTileNum`、`tailBlockNumEachCore`、`formerCoreRemainder`和`baseLength`，再根据`tailBlockNumEachCore`与`formerCoreRemainder`得到四类场景。
- `add.asc`按四类场景使用不同的轻量Tiling结构体，避免向`Kernel`传入旧样例中的17字段大结构体。
- `add_unified_tiling.asc`提供对照实现：Host侧使用`BuildUnifiedTilingData`统一计算Tiling并只传入一个`UnifiedTilingData`结构体，函数调用更简单，但`Kernel`入参字段数多于`add.asc`中的分模式轻量结构体。
- `Kernel`侧通过`GetBlockIdx()`确定当前核处理的数据区间。
- 每个核在UB中使用三段静态Tensor地址保存`xLocal`、`yLocal`和`zLocal`。
- `Kernel`执行流程为`DataCopy GM->UB`、`Add`、`DataCopy UB->GM`。

## 编译运行

在本样例根目录下执行如下步骤。

1. 配置环境变量。

   ```bash
   source ${install_path}/cann/set_env.sh
   ```

2. 生成数据。

   ```bash
   python3 scripts/gen_data.py 0
   ```

3. 编译并执行样例。

   ```bash
   cmake -S . -B build -DSCENARIO_NUM=0 -DCMAKE_ASC_ARCHITECTURES=dav-2201
   cmake --build build -j
   ./build/demo
   ```

   若要运行统一Tiling结构体版本，执行如下命令。

   ```bash
   ./build/demo_unified
   ```

4. 验证结果。

   ```bash
   python3 scripts/verify_result.py output/output.bin output/golden.bin
   ```

执行结果如下，说明精度比对成功。

```text
test pass!
```

## 编译选项说明

| 选项 | 可选值 | 说明 |
|------|--------|------|
| `SCENARIO_NUM` | `0`、`1`、`2`、`3` | Tiling切分场景编号。 |
| `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU运行、CPU调试、NPU仿真。 |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：`dav-2201`对应Atlas A2 训练系列产品/Atlas A2 推理系列产品和Atlas A3 训练系列产品/Atlas A3 推理系列产品，`dav-3510`对应Ascend 950PR/Ascend 950DT。 |

切换`SCENARIO_NUM`后，需要重新运行数据生成、CMake配置、编译和验证。例如：

```bash
python3 scripts/gen_data.py 3
cmake -S . -B build -DSCENARIO_NUM=3 -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j
./build/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

统一Tiling结构体版本使用相同的数据生成和验证脚本，运行时将`./build/demo`替换为`./build/demo_unified`即可。
