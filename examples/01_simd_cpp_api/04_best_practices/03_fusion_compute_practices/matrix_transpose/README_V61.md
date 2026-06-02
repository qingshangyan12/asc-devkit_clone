# Sobel边缘检测 SIMT+SIMD 混合编程样例（v4：固定block数+多tile循环+大块UB分配）

## 概述

本样例以Sobel边缘检测为例，介绍Ascend C SIMT编程方式下通过DataCopyPad批量搬运数据避免UB bank写冲突的优化思路，并在v3重叠tile设计的基础上进一步优化block调度策略。样例采用重叠tile设计：每个block搬运32×32数据到UB，仅计算内部30×30像素，边界1行/列自然充当光晕，相邻blocks间距30像素（重叠2像素），从而消除光晕加载的if分支。

v4相比v3的核心变化：blockNum从动态计算（等于tile总数，如910个block）改为固定64个block，每个block通过循环处理多个tile（`tilesPerBlock = ceil(totalTiles/64)`）。同时，LD和MTE变体的UB分配策略统一：外层kernel预分配16KB大块UB（bigTile），VF函数接收指向各tile独立slot的`__ubuf__`指针，不同tile间无UB复用冲突，MTE变体仅需MTE2_V同步（无需V_MTE2），同步开销减半。

样例包含3个kernel版本：直接访问GM的基准版本、通过SIMT线程逐元素加载到UB的版本、以及通过DataCopyPad批量搬运到UB的版本。重点对比SIMT load指令与DataCopyPad接口在写入UB时对bank写冲突的影响差异，以及固定block数+多tile循环策略对调度开销和资源利用率的优化效果。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── matrix_transpose
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── figures                // README中的图片资源
│   ├── matrix_transpose.asc   // 矩阵转置样例实现
│   ├── sobel_simt.asc         // Sobel边缘检测样例实现（v4：固定block数+多tile循环+大块UB分配）
│   ├── sobel_simt_v2.asc      // Sobel v2样例（非重叠tile+8个if分支）
│   ├── sobel_simt_v3.asc      // Sobel v3样例（重叠tile+1block/tile）
│   ├── sobel_simt_v4.asc      // Sobel v4样例（固定block数+大块UB分配+LD/MTE统一）
│   ├── README.md              // Sobel v2说明文档
│   ├── README_v3.md           // Sobel v3说明文档
│   └── README_V61.md          // Sobel v4说明文档（当前文档）
```

## 样例描述

- 计算公式：

  Sobel边缘检测对每个像素取其3×3邻域，分别计算水平梯度Gx和垂直梯度Gy：

  $$Gx = -T_L + T_R - 2M_L + 2M_R - B_L + B_R$$

  $$Gy = -T_L - 2T_C - T_R + B_L + 2B_C + B_R$$

  $$magnitude = |Gx| + |Gy|$$

  - input为输入图像，形状为[height, width]，数据类型为uint8_t
  - output为输出图像，形状为[height, width]，数据类型为uint8_t
  - 边缘像素（第一行/列、最后一行/列）保持原值不变，kernel只写入[1, height-2]×[1, width-2]区域的Sobel结果

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SobelFilter</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1024,768]</td><td align="center">uint8_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[1024,768]</td><td align="center">uint8_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">sobel_kernel_global / sobel_kernel_shared_with_ld / sobel_kernel_shared_with_mte</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过3个独立的kernel实现不同的数据加载策略，每个kernel对应特定的Case版本。

| Case   | 实现特点                                                  | 使用的核函数                    | 优化特性                               |
| ------ | --------------------------------------------------------- | ------------------------------- | -------------------------------------- |
| Case 0 | 每个线程直接从GM读取3×3邻域，不使用共享内存               | sobel_kernel_global             | 直接访问GM版本（基准）                 |
| Case 1 | 使用UB共享内存，通过SIMT load指令逐元素写入UB             | sobel_kernel_shared_with_ld     | UB共享内存 + SIMT逐元素加载 + 大块UB分配 |
| Case 2 | 使用UB共享内存，通过DataCopyPad批量写入UB                 | sobel_kernel_shared_with_mte    | UB共享内存 + DataCopyPad批量搬运 + 大块UB分配 |

### v3与v4设计差异

| 特性               | v3（sobel_simt_v3.asc）                                | v4（sobel_simt.asc）                                  |
| ------------------ | ------------------------------------------------------ | ----------------------------------------------------- |
| blockNum           | 动态计算：blockNum = gridX × gridY（如910个block）     | 固定64：BLOCK_NUM = 64                                |
| 每block处理tile数  | 1个block处理1个tile                                    | 1个block循环处理tilesPerBlock个tile                   |
| VF tile坐标来源    | VF内部通过`blockIdx.x % gridX`推算tile坐标             | 外层kernel循环传入`(startX, startY)`给VF              |
| LD VF的UB分配      | VF内部声明`__ubuf__ shared_block[32][32]`              | 外层kernel预分配16KB bigTile，VF接收`__ubuf__`指针    |
| MTE VF的UB分配     | 外层kernel分配1个tile的LocalTensor（1024B）            | 外层kernel预分配16KB bigTile，各tile写入不同slot      |
| MTE同步方式        | 1个tile仅需MTE2_V（单tile无流水线）                    | 仅MTE2_V（不同slot无跨迭代覆写，无需V_MTE2）          |
| LD/MTE UB策略差异  | LD内部声明，MTE外层分配，两者不一致                    | LD和MTE均外层分配bigTile，策略统一                    |
| block调度开销      | 较高（910个block调度）                                 | 较低（64个block调度）                                 |

#### 线程块布局

本样例采用SIMD与SIMT混合编程方式，所有Case采用完全一致的重叠tile布局：

- 每个block搬运32×32的数据到UB，但只计算内部30×30像素的Sobel结果
- tile的边界1行/列（threadIdx为0或BLOCK_SIZE-1的线程）自然充当光晕：这些线程不执行计算，但其数据被内部30×30线程的3×3邻域读取
- blocks间距为COMPUTE_SIZE=30，相邻blocks重叠2像素，确保所有内部像素都被覆盖
- block配置固定为(32, 32, 1)，单个block总线程数为32×32=1024
- v4中grid配置：BLOCK_NUM=64个block，每个block循环处理`tilesPerBlock = ceil(totalTiles/64)`个tile
- tile的二维坐标由外层kernel循环中的`tileIdx % gridX`和`tileIdx / gridX`计算，传入VF的`(startX, startY)`

```cpp
// 外层kernel循环处理多个tile
int32_t tilesPerBlock = (totalTiles + BLOCK_NUM - 1) / BLOCK_NUM;
int32_t tileStart = blockId * tilesPerBlock;
int32_t tileEnd = min(tileStart + tilesPerBlock, totalTiles);

for (int32_t tileIdx = tileStart; tileIdx < tileEnd; ++tileIdx) {
    int32_t blockIdxX = tileIdx % gridX;
    int32_t blockIdxY = tileIdx / gridX;
    int32_t startX = blockIdxX * COMPUTE_SIZE;
    int32_t startY = blockIdxY * COMPUTE_SIZE;
    asc_vf_call<sobel_vf_global>(dim3(BLOCK_SIZE, BLOCK_SIZE),
        input, output, width, height, startX, startY);
}
```

### 性能指标说明

| 指标                | 说明                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(us)   | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间                  |
| aiv_time(us)        | Task在AI Vector Core上的理论执行时间，单位为us                                            |
| aiv_total_cycles    | 该Task被分配到每个AI Vector Core计算单元上后，每个AI Vector Core计算单元上的执行cycle总数 |
| aiv_vec_time(us)    | vec类型指令（向量类运算指令）耗时，单位为us                                               |
| aiv_vec_ratio       | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比                           |
| aiv_scalar_time(us) | scalar类型指令（标量类运算指令）耗时，单位为us                                            |
| aiv_scalar_ratio    | scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比                        |

### Case 0: 直接访问GM版本

**样例目标**：建立Sobel算子直接访问GM的耗时基线，为后续共享内存版本提供性能参照

**核心实现**：

- 固定64个block，每个block循环处理多个tile
- 每次循环为一个tile调用`sobel_vf_global`，传入`(startX, startY)`给VF
- VF中只有内部30×30线程执行Sobel计算，每个计算线程直接从GM读取3×3邻域的9个元素
- 不使用共享内存，同一个Warp的线程对GM的读取地址跨行跳跃

**关键代码**：

```cpp
__global__ __vector__ void sobel_kernel_global(
    __gm__ uint8_t* input, __gm__ uint8_t* output, int width, int height,
    int32_t gridX, int32_t totalTiles)
{
    uint32_t blockId = AscendC::GetBlockIdx();
    int32_t tilesPerBlock = (totalTiles + BLOCK_NUM - 1) / BLOCK_NUM;
    int32_t tileStart = blockId * tilesPerBlock;
    int32_t tileEnd = min(tileStart + tilesPerBlock, totalTiles);

    for (int32_t tileIdx = tileStart; tileIdx < tileEnd; ++tileIdx) {
        int32_t blockIdxX = tileIdx % gridX;
        int32_t blockIdxY = tileIdx / gridX;
        int32_t startX = blockIdxX * COMPUTE_SIZE;
        int32_t startY = blockIdxY * COMPUTE_SIZE;
        asc_vf_call<sobel_vf_global>(dim3(BLOCK_SIZE, BLOCK_SIZE),
            input, output, width, height, startX, startY);
    }
}
```

```cpp
__simt_vf__ void sobel_vf_global(
    __gm__ uint8_t* input, __gm__ uint8_t* output, int width, int height,
    int32_t startX, int32_t startY)
{
    int32_t x = startX + threadIdx.x;
    int32_t y = startY + threadIdx.y;

    if (threadIdx.x == 0 || threadIdx.x == BLOCK_SIZE - 1 ||
        threadIdx.y == 0 || threadIdx.y == BLOCK_SIZE - 1) return;

    int tl = input[(y-1) * width + (x-1)];
    ...
    output[y * width + x] = static_cast<uint8_t>(magnitude);
}
```

**分析**：

- Case 0的Task Duration作为直接访问GM的基准耗时
- 每个计算线程需要9次GM读取（3×3邻域），同一Warp中相邻线程的读取地址跨行跳跃，GM访问效率较低
- 固定64个block降低了block调度开销，但每个block循环中的多次VF调用增加了指令开销
- 边界线程（threadIdx为0或31）直接返回，不参与计算

---

### Case 1: UB共享内存 + SIMT逐元素加载版本（大块UB分配）

**样例目标**：使用UB共享内存减少GM重复读取，通过SIMT load指令逐元素写入UB，采用外层kernel预分配大块UB策略

**核心实现**：

- 外层kernel预分配16KB大块UB（`bigTile`），VF函数接收指向各tile独立slot的`__ubuf__`指针
- 每个VF线程仅执行1行加载代码：通过clamp处理越界坐标，将数据写入`shared_block[threadIdx.y * BLOCK_SIZE + threadIdx.x]`
- 无需任何if分支单独加载光晕行列或角点——tile边界1行/列自然包含光晕数据
- 调用`asc_syncthreads()`同步后，内部30×30线程从UB读取3×3邻域计算Sobel
- 不同tile使用不同UB区域（offset = i * TILE_ELEMS），不同迭代间无UB复用冲突

**外层关键代码（大块UB分配+循环调度）**：

```cpp
__global__ __vector__ void sobel_kernel_shared_with_ld(
    __gm__ uint8_t* input, __gm__ uint8_t* output, int width, int height,
    int32_t gridX, int32_t totalTiles)
{
    uint32_t blockId = AscendC::GetBlockIdx();
    int32_t tilesPerBlock = (totalTiles + BLOCK_NUM - 1) / BLOCK_NUM;
    int32_t tileStart = blockId * tilesPerBlock;
    int32_t tileEnd = min(tileStart + tilesPerBlock, totalTiles);
    int32_t numTiles = tileEnd - tileStart;

    constexpr uint32_t TILE_ELEMS = BLOCK_SIZE * BLOCK_SIZE;  // 1024B
    constexpr uint32_t UB_TOTAL = 16U * 1024U;                // 16KB
    AscendC::LocalTensor<uint8_t> bigTile(AscendC::TPosition::VECCALC, 0, UB_TOTAL);

    __ubuf__ uint8_t* basePtr = reinterpret_cast<__ubuf__ uint8_t*>(bigTile.GetPhyAddr());

    for (int32_t i = 0; i < numTiles; ++i) {
        int32_t tileIdx = tileStart + i;
        int32_t blockIdxX = tileIdx % gridX;
        int32_t blockIdxY = tileIdx / gridX;
        int32_t startX = blockIdxX * COMPUTE_SIZE;
        int32_t startY = blockIdxY * COMPUTE_SIZE;

        __ubuf__ uint8_t* sharedPtr = basePtr + i * TILE_ELEMS;
        asc_vf_call<sobel_vf_shared_with_ld>(dim3(BLOCK_SIZE, BLOCK_SIZE),
            sharedPtr, input, output, width, height, startX, startY);
    }
}
```

**内层关键代码（VF逐元素加载+计算）**：

```cpp
__simt_vf__ void sobel_vf_shared_with_ld(
    __ubuf__ uint8_t* shared_block, __gm__ uint8_t* input, __gm__ uint8_t* output,
    int width, int height, int32_t startX, int32_t startY)
{
    int32_t x = startX + threadIdx.x;
    int32_t y = startY + threadIdx.y;

    int32_t cx = (x < 0) ? 0 : (x >= width) ? width - 1 : x;
    int32_t cy = (y < 0) ? 0 : (y >= height) ? height - 1 : y;
    shared_block[threadIdx.y * BLOCK_SIZE + threadIdx.x] = input[cy * width + cx];

    asc_syncthreads();

    if (threadIdx.x == 0 || threadIdx.x == BLOCK_SIZE - 1 ||
        threadIdx.y == 0 || threadIdx.y == BLOCK_SIZE - 1) return;

    int tl = shared_block[(threadIdx.y - 1) * BLOCK_SIZE + (threadIdx.x - 1)];
    ...
}
```

**问题分析：SIMT load指令导致UB bank写冲突**

sobel_vf_shared_with_ld的核心问题在于：SIMT线程通过load指令逐元素将数据写入UB时，多个线程在同一时刻会同时写同一个bank group，引发**写写冲突（bank write-write conflict）**。

以Ascend 950PR/Ascend 950DT（NPU架构版本3510）的UB为例，其bank结构如下：

- UB总大小256KB，划分为16个bank，组织为8个bank group（每个bank group包含2个bank）
- 每个bank由512行组成，每行长度32B
- bank group内的两个bank共享写口：**同一拍内，多个写操作访问同一个bank group时，必须排队等待，导致写写冲突**

在sobel_vf_shared_with_ld中，1024个线程同时执行 `shared_block[threadIdx.y * BLOCK_SIZE + threadIdx.x] = input[...]`。shared_block为32×32的uint8_t数组，行宽恰好32B：

- 同一行的32个线程（threadIdx.y相同，threadIdx.x从0到31）写入同一行的0~31字节
- 32B恰好等于1个DataBlock，这32字节分布在bank0的1行和bank8的1行（它们属于同一个bank group 0）
- 32个线程中有16个线程写入bank0的同一行，16个线程写入bank8的同一行
- **同一bank group内的写操作必须排队**：一个bank group同一拍只能接受1次写操作，32个写请求需要逐拍排队完成

虽然重叠tile设计消除了光晕加载的if分支，但Load阶段的bank写冲突本质上仍然存在——同一行的32个线程仍然集中写入同一个bank group。

**v4大块UB分配策略的优势**：

v4中LD变体不再在VF内部声明`__ubuf__ shared_block[32][32]`，而是由外层kernel预分配16KB大块UB，VF接收指向各tile独立slot的指针。这一设计带来以下优势：

- LD和MTE变体的UB分配策略统一，差异仅在load方式（per-thread写 vs DataCopyPad向量写），对比更纯粹
- 不同tile使用不同UB区域（`basePtr + i * TILE_ELEMS`），不同迭代间无UB复用冲突

---

### Case 2: UB共享内存 + DataCopyPad批量搬运版本（大块UB分配）

**优化目标**：通过DataCopyPad批量搬运数据到UB，避免SIMT load指令导致的bank写冲突，采用外层kernel预分配大块UB策略

**核心实现**：

sobel_kernel_shared_with_mte分为两层：

1. **外层（SIMD层）**：预分配16KB大块UB（bigTile），循环中通过DataCopyPad将各tile从GM写入UB的不同slot（`bigTile[i * TILE_ELEMS]`），MTE2_V同步后启动该tile的VF计算
2. **内层（SIMT层）**：sobel_vf_shared_with_mte直接从预加载的UB读取邻域数据计算Sobel，无需逐元素加载GM，也无需asc_syncthreads()

**外层关键代码（大块UB分配+DataCopyPad搬运+循环调度）**：

```cpp
__global__ __vector__ void sobel_kernel_shared_with_mte(
    __gm__ uint8_t* input, __gm__ uint8_t* output, int width, int height,
    int32_t gridX, int32_t totalTiles)
{
    uint32_t blockId = AscendC::GetBlockIdx();
    int32_t tilesPerBlock = (totalTiles + BLOCK_NUM - 1) / BLOCK_NUM;
    int32_t tileStart = blockId * tilesPerBlock;
    int32_t tileEnd = min(tileStart + tilesPerBlock, totalTiles);
    int32_t numTiles = tileEnd - tileStart;

    constexpr uint32_t TILE_ELEMS = BLOCK_SIZE * BLOCK_SIZE;  // 1024B
    constexpr uint32_t UB_TOTAL = 16U * 1024U;                // 16KB
    AscendC::LocalTensor<uint8_t> bigTile(AscendC::TPosition::VECCALC, 0, UB_TOTAL);

    AscendC::GlobalTensor<uint8_t> inputGm;
    inputGm.SetGlobalBuffer(input);

    __ubuf__ uint8_t* basePtr = reinterpret_cast<__ubuf__ uint8_t*>(bigTile.GetPhyAddr());

    for (int32_t i = 0; i < numTiles; ++i) {
        int32_t tileIdx = tileStart + i;
        int32_t blockIdxX = tileIdx % gridX;
        int32_t blockIdxY = tileIdx / gridX;
        int32_t startX = blockIdxX * COMPUTE_SIZE;
        int32_t startY = blockIdxY * COMPUTE_SIZE;

        int32_t validCols = min(BLOCK_SIZE, width - startX);
        int32_t validRows = min(BLOCK_SIZE, height - startY);
        int32_t rightPad = BLOCK_SIZE - validCols;

        AscendC::DataCopyExtParams copyParams = {
            static_cast<uint16_t>(validRows),
            static_cast<uint32_t>(validCols),
            static_cast<uint32_t>(width - validCols),
            0U,
            0U
        };
        AscendC::DataCopyPadExtParams<uint8_t> padParams(
            static_cast<bool>(rightPad > 0),
            static_cast<uint8_t>(0),
            static_cast<uint8_t>(rightPad),
            uint8_t(0)
        );

        uint64_t srcOffset = startY * width + startX;
        AscendC::DataCopyPad<uint8_t>(
            bigTile[static_cast<uint32_t>(i) * TILE_ELEMS],
            inputGm[srcOffset],
            copyParams,
            padParams
        );

        // 仅需MTE2_V同步：不同slot无跨迭代覆写，VF读region i与DataCopyPad写region i+1不冲突
        AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(0);
        AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(0);

        __ubuf__ uint8_t* sharedPtr = basePtr + static_cast<uint32_t>(i) * TILE_ELEMS;
        asc_vf_call<sobel_vf_shared_with_mte>(dim3(BLOCK_SIZE, BLOCK_SIZE),
            sharedPtr, output, width, height, startX, startY);
    }
}
```

**内层关键代码（SIMT计算）**：

```cpp
__simt_vf__ void sobel_vf_shared_with_mte(
    __ubuf__ uint8_t* shared_block, __gm__ uint8_t* output,
    int width, int height, int32_t startX, int32_t startY)
{
    int tl = shared_block[(threadIdx.y - 1) * BLOCK_SIZE + (threadIdx.x - 1)];
    int tc = shared_block[(threadIdx.y - 1) * BLOCK_SIZE + threadIdx.x];
    ...
}
```

**优化分析：DataCopyPad vs SIMT load指令在bank写冲突上的差异**

DataCopyPad相比SIMT load指令，在写入UB时有本质的区别：

| 特性             | SIMT load指令（sobel_vf_shared_with_ld）                    | DataCopyPad（sobel_kernel_shared_with_mte）               |
| ---------------- | ----------------------------------------------------------- | --------------------------------------------------------- |
| 写入粒度         | 每个线程每拍写1个uint8_t（1B）                              | 每拍写256B（8个DataBlock×32B）                             |
| 写入方式         | 1024个线程并发，每拍多个线程同时写同一个bank group          | 硬件批量搬运，每拍8个DataBlock分散写到不同的bank group     |
| bank写冲突       | 同一行的32个线程写入同一个bank group，严重写写冲突          | 每拍8个DataBlock分别写到8个不同bank group，无写写冲突      |
| 光晕处理         | clamp越界坐标，仍需asc_syncthreads()                        | DataCopyPadExtParams自动补零，无需线程同步                 |
| 同步开销         | 需要asc_syncthreads()保证UB写入完成                         | 仅需MTE2_V同步（不同slot无跨迭代覆写，无需V_MTE2）        |

**DataCopyPad避免写写冲突的原理**：

DataCopyPad是MTE2（Memory Transfer Engine 2）硬件搬运指令，其写入UB的方式是：每拍写入8个DataBlock（每个DataBlock 32B），这8个DataBlock按照UB的物理布局依次写到连续的8个bank group上。由于3510架构有8个bank group，每拍8个DataBlock恰好分布到8个不同的bank group，每个bank group只被写一次，因此**不产生任何写写冲突**。

本样例中DataCopyPad搬运32×32瓦片（行宽32B=1 DataBlock）的具体过程：

- 每行32B = 1个DataBlock，写入1个bank group
- DataCopyPad每拍写8行（8个DataBlock），分别写到8个不同bank group
- 32行数据只需4拍即可完成写入，每拍8个bank group各被写1次，**零写写冲突**
- 整体写入效率：4拍完成32×32=1024B数据的写入

而SIMT load指令的情况则截然不同：

- 每个SIMT线程通过 `shared_block[threadIdx.y * BLOCK_SIZE + threadIdx.x] = value` 写入1字节
- 同一Warp的32个线程在同一时刻发出写请求
- 这些写请求的地址集中在同一行的0~31字节，落在同一个bank group（bank0+bank8组成bank group 0）
- 一个bank group一拍只能接受1次写操作，32个写请求需要排队逐拍完成
- 写完一行32字节的数据需要约16拍（考虑bank group写口限制），而非DataCopyPad的1拍（8行并行写入）

**v4大块UB分配策略对MTE同步的优化**：

v4中MTE变体采用大块UB分配（16KB bigTile），各tile的DataCopyPad写入不同slot（`bigTile[i * TILE_ELEMS]`），VF读取对应slot的数据。这一设计消除了跨迭代的UB覆写冲突：

- v3中每个block只处理1个tile，DataCopyPad写入唯一的1KB UB区域，不存在跨迭代冲突
- v4初版中若各tile复用同一1KB UB区域，每次迭代需要2次同步（MTE2_V + V_MTE2），确保VF读完前一tile后DataCopyPad才能写入
- v4优化版中各tile使用不同slot，DataCopyPad写slot(i+1)与VF读slot(i)无冲突，仅需1次MTE2_V同步
- 同步事件从`2 × numTiles`次降为`1 × numTiles`次，开销减半
- DataCopyPad和VF在同一循环中，具备流水线潜力：V引擎计算tile i时，MTE2引擎可并行加载tile i+1

UB容量约束：TILE_ELEMS = 32×32 = 1024B；16KB / 1024B = 16个tile上限。当前768×1024测试用例tilesPerBlock = ceil(910/64) = 15，满足约束。

**DataCopyPad的额外优势：自动padding简化边界处理**

DataCopyPad通过DataCopyPadExtParams的rightPadding参数处理右边界block的补零，无需任何if分支或clamp逻辑：

- 大部分block完全在图像内：validCols=32, rightPad=0，无补零
- 右/下边界block：validCols < 32，DataCopyPad自动在每行右侧补零到32B
- leftPadding始终为0（startX = blockIdxX × COMPUTE_SIZE ≥ 0，无需左侧补零）
- 补零位置不会被计算线程读取（它们的全局坐标在图像内部）

这使得Case 2的VF函数sobel_vf_shared_with_mte极为简洁——只需从预加载的UB读取邻域数据并计算，无需任何GM加载或边界处理逻辑。

**32B天然对齐stride的优势**

本样例中UB stride恰好为32B（BLOCK_SIZE=32，uint8_t类型），行宽天然32B对齐，无需Normal模式补齐。这一特性带来两个优势：

1. **无padding浪费**：每行32B恰好1个DataBlock，没有额外的对齐padding占用UB空间
2. **读阶段bank分布均匀**：行步长=32B=1 DataBlock，同一列的相邻行元素间隔32B，在3510架构的bank布局中，同一列的相邻元素落在不同的bank行上，读取邻域3行（上、中、下）时各行偏移32B，分布在不同的bank group上，降低了读读冲突的规模

---

## 性能对比总结

### Ascend 950PR性能数据

| Case version | 核函数                         | block数 | 数据加载方式           | UB分配策略          | UB写冲突情况       | 光晕if分支 | 同步方式           |
| ------------ | ------------------------------ | ------- | ---------------------- | ------------------- | ------------------ | ---------- | ------------------ |
| Case 0       | sobel_kernel_global            | 64(固定) | 直接访问GM             | 无UB分配            | 无UB写入           | 0          | 无                 |
| Case 1       | sobel_kernel_shared_with_ld    | 64(固定) | SIMT load逐元素写入UB  | 16KB bigTile多slot  | 严重写写冲突       | 0          | asc_syncthreads()  |
| Case 2       | sobel_kernel_shared_with_mte   | 64(固定) | DataCopyPad批量写入UB  | 16KB bigTile多slot  | 无写写冲突         | 0          | 仅MTE2_V           |

### v3与v4性能对比

| 版本 | block数 | 每blocktile数 | LD/MTE UB策略 | MTE同步次数(15个tile) | block调度开销 |
| ---- | ------- | ------------- | ------------- | --------------------- | ------------- |
| v3   | 910     | 1             | 不统一        | 15（仅MTE2_V）        | 较高          |
| v4   | 64      | 15            | 统一bigTile   | 15（仅MTE2_V）        | 较低          |

**核心结论**：

- DataCopyPad以一拍256B的粒度写入UB，8个DataBlock分散到8个bank group，避免了SIMT load指令的写写冲突
- SIMT load指令每拍只能写1字节，同一行32个线程并发写入导致同一bank group上严重的写写排队
- 重叠tile设计消除了光晕加载的if分支（0个if），LD版本仅需1行clamp代码完成数据加载
- DataCopyPad通过rightPadding参数自动处理右边界补零，进一步简化边界逻辑
- 32B天然对齐的stride在无padding浪费的同时保证了读阶段的bank分布均匀
- 固定64个block降低了block调度开销，增加了每个block的工作量
- 大块UB分配策略统一了LD和MTE变体的实现方式，差异仅在load方式和同步方式
- MTE变体不同slot无跨迭代覆写，仅需MTE2_V同步，同步开销减半，具备流水线潜力

## 调优建议

1. **优先使用DataCopyPad批量搬运数据到UB**：在SIMD+SIMT混合编程场景中，应优先通过DataCopyPad将所需数据从GM批量搬运到UB，再由SIMT线程从UB读取计算，避免SIMT线程逐元素写入UB带来的bank写冲突。
2. **理解SIMT load与DataCopyPad的写入粒度差异**：SIMT load指令每个线程一拍写1个数据元素，多个线程并发写入同一bank group会产生写写冲突；DataCopyPad一拍写256B（8个DataBlock），自然分散到不同bank group，无写写冲突。
3. **采用重叠tile设计消除光晕if分支**：让每个block搬运包含光晕的完整tile（如32×32），仅计算内部区域（如30×30），blocks间距等于计算区域大小（如30），重叠像素自然提供光晕数据，无需额外加载逻辑。
4. **利用DataCopyPadExtParams处理边界补零**：对于边界block，使用rightPadding参数自动补零，避免在SIMT代码中引入clamp或if分支。
5. **选择天然32B对齐的tile大小**：当数据类型为uint8_t时，BLOCK_SIZE=32使得行宽恰好32B对齐，既满足DataCopyPad的对齐要求，又无padding浪费，同时改善读阶段的bank分布。
6. **采用固定block数+多tile循环策略**：将blockNum固定为较小值（如64），每个block循环处理多个tile，降低block调度开销，提高单block资源利用率。注意UB容量约束：16KB / TILE_ELEMS = 16个tile上限。
7. **采用大块UB分配策略统一LD/MTE实现**：外层kernel预分配覆盖所有tile的大块UB，VF接收指向各tile独立slot的`__ubuf__`指针，不同tile间无UB复用冲突。LD和MTE差异仅在load方式和同步方式，对比更纯粹。
8. **利用不同UB slot消除跨迭代同步**：MTE变体中各tile的DataCopyPad写入不同UB区域，VF读slot(i)与DataCopyPad写slot(i+1)无冲突，仅需MTE2_V同步（无需V_MTE2），同步开销减半，DataCopyPad与VF具备流水线潜力。

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
  SCENARIO_NUM=2                       # 选择执行场景，可选0-2
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./demo                               # 执行样例
  ```

  注意：当前CMakeLists.txt编译的是sobel_simt_v2.asc。若需编译v4版本（sobel_simt.asc），请修改CMakeLists.txt中的源文件名：

  ```cmake
  add_executable(demo
      sobel_simt.asc
  )
  ```

- 编译选项说明

  | 选项             | 可选值      | 说明              |
  | ---------------- | ----------- | ----------------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM` | `0`-`2` | 样例类型，默认为2 |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy verification passed.
  ```

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof op ./demo   # 分析case的性能
```

命令完成后，会在默认目录下生成以"OPPROF_{timestamp}_XXX"命名的文件夹，性能数据文件夹结构示例如下：

```text
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的 bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```