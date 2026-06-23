# Tiling计算过程推导

## 固定输入和派生量

1. `dataBlockSize`：数据搬运的基本粒度，本样例为32字节。
2. `numBlocks`：参与计算的AI Core数量，本样例为8。
3. `tileLength`：每个核一次主块处理的元素个数，本样例为3200个`half`元素。
4. `alignNum`：一个DataBlock中包含的元素个数，计算公式为`dataBlockSize / sizeof(T)`，本样例为16个`half`元素。

## 原则

1. 数据搬运按DataBlock粒度处理，Host侧Tiling计算需要保证每个核处理的长度为`alignNum`的整数倍。
2. 每个核优先使用`tileLength`长度处理主块，减少搬运次数并充分利用UB。
3. 主块之后的剩余长度先尝试按DataBlock给所有核均分，保证核间负载尽可能均衡。
4. 若最后还有不足以分给所有核的DataBlock，则分给前若干个整核，每个整核只比尾核多处理一个DataBlock。

> [!NOTE]说明
> 需要先将`totalLength`向上对齐到`alignNum`的整数倍。否则可能访问Host侧未分配的尾部buffer。

totalLength对齐有好处，但是也不是必须的，
对齐就是在最后一个划分的时候不用再对齐，其实也只有这一个需要对齐
=》偏移到最后一个可能不是32字节对齐，然后gm其实也不会被覆盖，但是能比较好地保证数据不会越界

## 计算过程

输入为`totalLength`，输出为每个核处理的元素长度。

1. 先将总长度按DataBlock对齐。

   ```cpp
   totalLengthAligned = AlignUp(totalLength, alignNum);
   ```

2. 所有核先均分`tileLength`整数倍主块。

   ```cpp
   mainTileNum = totalLengthAligned / (numBlocks * tileLength);
   mainTileRemainder = totalLengthAligned % (numBlocks * tileLength);
   ```

   此时每个核至少处理`mainTileNum * tileLength`个元素。

3. 主块剩余长度再按DataBlock给所有核补齐等长尾块。

   ```cpp
   tailBlockNumEachCore = mainTileRemainder / (numBlocks * alignNum);
   formerCoreRemainder = mainTileRemainder % (numBlocks * alignNum);
   baseLength = mainTileNum * tileLength + tailBlockNumEachCore * alignNum;
   ```

   此时每个核的基础处理长度为`baseLength`。如果`tailBlockNumEachCore != 0`，说明每个核都有等长尾块。

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

## 场景归类

四类Tiling场景只是上述统一流程的结果，不作为第一层分支。

| 条件 | 场景 | 含义 |
|------|------|------|
| `tailBlockNumEachCore == 0 && formerCoreRemainder == 0` | 均分+均分 | 每个核只有主块，无尾核、无尾块。 |
| `tailBlockNumEachCore != 0 && formerCoreRemainder == 0` | 均分+尾块 | 每个核有相同长度的尾块，无尾核。 |
| `tailBlockNumEachCore == 0 && formerCoreRemainder != 0` | 尾核+均分 | 前若干个整核比尾核多一个DataBlock，尾核只有主块。 |
| `tailBlockNumEachCore != 0 && formerCoreRemainder != 0` | 尾核+尾块 | 所有核都有尾块，前若干个整核再多一个DataBlock。 |

## 本样例参数结果

本样例使用`numBlocks = 8`、`tileLength = 3200`、`alignNum = 16`。

| `SCENARIO_NUM` | `totalLength` | `mainTileNum` | `tailBlockNumEachCore * alignNum` | `formerCoreRemainder` | 核间结果 |
|----------------|---------------|---------------|-----------------------------------|--------------|----------|
| `0` | `256000` | `10` | `0` | `0` | 8核均为32000。 |
| `1` | `260096` | `10` | `512` | `0` | 8核均为32512。 |
| `2` | `256064` | `10` | `0` | `64` | 4个整核为32016，4个尾核为32000。 |
| `3` | `258112` | `10` | `256` | `64` | 4个整核为32272，4个尾核为32256。 |
