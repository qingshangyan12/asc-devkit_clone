# SIMT实现Sobel边缘检测算子样例（紧凑输出版）

## 概述

本样例介绍基于SIMT实现的Sobel边缘检测算子，展示如何使用warp shuffle原语（`asc_shfl_up`/`asc_shfl_down`）在warp内高效传递邻居像素数据，以及使用`asc_ballot`实现warp级stream compaction的实践。

与基础版本输出完整的 `edge_map` 二维数组不同，本样例只输出超过阈值的边缘像素坐标和强度，组成紧凑列表。对于自然图像（边缘像素通常占5%~15%），紧凑列表的数据量远小于完整的 `edge_map`，后续处理（霍夫变换、轮廓追踪等）也可直接遍历列表，无需跳过大量零值。`asc_ballot` 在这一过程中承担了两个关键作用，详见[asc_ballot与stream compaction](#asc_ballot与stream-compaction)。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.0.0

## 目录结构介绍

```text
├── sobel_warp_shfl
│   ├── CMakeLists.txt          // cmake编译文件
│   ├── sobel_warp_shfl.asc     // Sobel边缘检测算子实现 & 调用样例
│   └── README.md
```

## 背景知识

### Sobel算子

Sobel算子是一种常用的图像边缘检测算法。图像中的边缘通常表现为像素值的剧烈变化，Sobel算子通过计算每个像素点周围像素值的梯度来衡量这种变化的强度。

对于图像中坐标为 (x, y) 的像素，取其3×3邻域：

```
(x-1,y-1)  (x,y-1)  (x+1,y-1)       tl   t  tr
(x-1,y  )  (x,y  )  (x+1,y  )   →   ml   *  mr
(x-1,y+1)  (x,y+1)  (x+1,y+1)       bl   b  br
```

水平方向梯度 Gx 衡量像素值在左右方向的变化幅度：

```
Gx = -tl + tr - 2*ml + 2*mr - bl + br
```

垂直方向梯度 Gy 衡量像素值在上下方向的变化幅度：

```
Gy = -tl - 2*t - tr + bl + 2*b + br
```

最终该像素点的边缘幅值为：

```
strength = |Gx| + |Gy|
```

幅值越大，说明该像素点周围的像素值变化越剧烈，越可能是图像边缘。图像边界（第0行、最后一行、第0列、最后一列）由于无法构成完整的3×3邻域，幅值输出为0。

### warp shuffle与邻居像素获取

在SIMT编程模型中，一个warp由32个线程组成，每个线程有一个lane id（0~31）。本样例中，一个warp负责图像同一行连续的32个像素，lane 0 对应最左侧像素，lane 31 对应最右侧像素。

计算Sobel需要每个像素点的左列和右列数据。以 lane 5（对应像素列 x=5）为例：

- 左列数据（x=4）由 lane 4 持有
- 右列数据（x=6）由 lane 6 持有

通过warp shuffle原语可以直接在寄存器间传递数据，无需访问Global Memory：

- `asc_shfl_up(val, 1, 32)`：每个lane从 lane-1 获取值，lane 5 调用后得到 lane 4 的数据，即左列像素值
- `asc_shfl_down(val, 1, 32)`：每个lane从 lane+1 获取值，lane 5 调用后得到 lane 6 的数据，即右列像素值

warp边界处shuffle结果无效：

- lane 0 调用 `asc_shfl_up` 时没有 lane-1，返回自身值，而非左列像素值
- lane 31 调用 `asc_shfl_down` 时没有 lane+1（下一个warp的 lane 0 不在同一warp内），返回自身值，而非右列像素值

因此 lane 0 和 lane 31 需要回退到全局内存读取真实的邻居像素：

```cpp
// lane 0 的左邻居在全局内存，其他lane用shuffle结果
int top_left  = (lane == 0)  ? input[(y-1)*width+(x-1)] : shfl_top_left;
// lane 31 的右邻居在全局内存，其他lane用shuffle结果
int top_right = (lane == 31) ? input[(y-1)*width+(x+1)] : shfl_top_right;
```

### asc_ballot与stream compaction

`asc_ballot(predicate)` 收集warp内所有32个lane的谓词结果，返回一个32位整数掩码。掩码的第 i 个bit为1表示 lane i 的谓词为真，为0表示为假。warp内所有lane拿到的掩码值相同。

本样例用 `asc_ballot` 实现warp级stream compaction，将边缘像素写入紧凑列表。假设warp内各lane计算出的幅值为：

```
lane:      0    1    2    3    4    5  ...  31
strength: 120  450    0  600    0  380 ...  200
```

执行 `asc_ballot(strength >= THRESHOLD)` 后，各lane得到相同的掩码（THRESHOLD=400为例）：

```
bit 0 = 0  (120 < 400)
bit 1 = 1  (450 >= 400)
bit 2 = 0  (  0 < 400)
bit 3 = 1  (600 >= 400)
bit 4 = 0  (  0 < 400)
bit 5 = 0  (380 < 400)
...
edge_mask = 0b...001010  即 lane 1 和 lane 3 是边缘像素
```

**为什么不用 ballot 代价更高**

stream compaction 本质上需要两个信息：这个 warp 里总共有几个边缘像素，以及我是其中第几个。不用 ballot 有两种替代方案：

1. **每个边缘 lane 独立 atomicAdd**：每人抢一个槽位，一个 warp 最多 32 次原子操作串行竞争同一个地址。
2. **用 shuffle 做 prefix sum**：通过 log2(32) = 5 轮 shuffle scan 算出"比我 lane id 小的边缘 lane 有几个"，每轮有数据依赖，无法并行。

`asc_ballot` 把 32 个 lane 的布尔值打包成一个 32-bit 整数，从它出发：总数用 `__popc(edge_mask)` 一条指令算出，我的偏移用 `__popc(edge_mask & mask)` 一条指令算出，5 轮 shuffle scan 被压缩成 1 次 ballot + 1 次 popcount，原子竞争也从最多 32 次降为 1 次。

**作用一：把最多32次原子操作压缩为1次**

没有ballot时，每个边缘lane都要独立调用 `asc_atomic_add(edge_count, 1)` 抢一个输出槽位，一个warp内最多产生32次原子竞争。

有了ballot，lane 0 用 `__popc(edge_mask)` 一次算出这个warp内有几个边缘像素（上例中为2），然后只做**一次** `asc_atomic_add(edge_count, 2)` 批量预留2个连续槽位，返回起始位置 `warp_base`：

```cpp
uint32_t edge_mask = asc_ballot(in_image && (strength >= THRESHOLD));
uint32_t warp_base = 0;
if (lane == 0) {
    warp_base = asc_atomic_add(edge_count, (uint32_t)__popc(edge_mask));
}
warp_base = asc_shfl(warp_base, 0, WARP_SIZE);  // 广播给全warp
```

**作用二：无冲突地分配warp内局部偏移**

每个边缘lane需要知道自己写 `warp_base + 几`。利用 `edge_mask` 中比自己lane id小的那些边缘lane的数量，即可得到无冲突的局部偏移，不需要任何额外同步：

```cpp
if ((edge_mask >> lane) & 1) {
    // lower_mask：edge_mask中比自己lane id小的所有边缘lane
    uint32_t lower_mask = edge_mask & ((1u << lane) - 1);
    int local_idx = __popc(lower_mask);  // 自己是warp内第几个边缘像素
    edge_list[warp_base + local_idx] = {(uint16_t)x, (uint16_t)y, strength};
}
```

上例中 lane 1 的 `lower_mask = 0b00`，`local_idx = 0`，写入 `edge_list[warp_base + 0]`；lane 3 的 `lower_mask = 0b10`，`local_idx = 1`，写入 `edge_list[warp_base + 1]`。两者天然不重叠。

**效果**：对于1024×1024图像，若边缘比例为7%，原子操作次数从约73000次（每像素一次）降至约2300次（每warp一次），减少约32倍。

## 算法分析

### warp shuffle取邻居像素

每个线程持有自己列 `x` 的 top/mid/bot 三行像素值，通过shuffle获取左右邻居列的数据：

- `asc_shfl_up(val, 1, 32)`：从 lane-1 获取值，即左列数据
- `asc_shfl_down(val, 1, 32)`：从 lane+1 获取值，即右列数据

warp边界处（lane 0 和 lane 31）shuffle结果无效，需回退全局内存读取真实邻居：

```cpp
int top_left  = (lane == 0)  ? input[(y-1)*width+(x-1)] : shfl_top_left;
int top_right = (lane == 31) ? input[(y-1)*width+(x+1)] : shfl_top_right;
```

### asc_ballot与stream compaction

`asc_ballot(predicate)` 收集warp内所有lane的谓词结果，返回一个32位掩码，每个bit对应一个lane。本样例利用这个掩码实现warp级stream compaction：

**作用一：把最多32次原子操作压缩为1次**

```cpp
uint32_t edge_mask = asc_ballot(in_image && (strength >= THRESHOLD));
uint32_t warp_base = 0;
if (lane == 0) {
    // __popc统计warp内边缘像素数，整个warp只做一次原子操作
    warp_base = asc_atomic_add(edge_count, (uint32_t)__popc(edge_mask));
}
warp_base = asc_shfl(warp_base, 0, WARP_SIZE);
```

**作用二：无冲突地分配warp内局部偏移**

```cpp
if ((edge_mask >> lane) & 1) {
    // lower_mask中bit为1的lane都是比自己lane id小的边缘像素
    uint32_t lower_mask = edge_mask & ((1u << lane) - 1);
    int local_idx = __popc(lower_mask);  // 自己是warp内第几个边缘像素
    edge_list[warp_base + local_idx] = {(uint16_t)x, (uint16_t)y, strength};
}
```

`lower_mask` 的popcount给每个边缘lane分配了唯一的局部偏移，不需要任何额外同步，写入位置天然不重叠。

## 算子描述

- 算子功能：
  对输入的灰度图像计算每个像素点的Sobel幅值，通过`asc_ballot`实现warp级stream compaction，将超过`THRESHOLD`的边缘像素以紧凑列表形式输出，同时用原子计数器记录边缘像素总数。

- 算子规格：
  <table>
  <tr><td rowspan="1" align="center">算子类型(OpType)</td><td colspan="4" align="center">sobel_edge_detect</td></tr>
  <tr><td rowspan="3" align="center">算子输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">height, width</td><td align="center">uint8_t</td><td align="center">ND</td></tr>
  <tr><td align="center">width, height</td><td align="center">-</td><td align="center">int</td><td align="center">-</td></tr>
  <tr><td rowspan="2" align="center">算子输出</td><td align="center">edge_list</td><td align="center">edge_count, 3</td><td align="center">uint16_t</td><td align="center">ND</td></tr>
  <tr><td align="center">edge_count</td><td align="center">1</td><td align="center">uint32_t</td><td align="center">-</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">sobel_edge_detect</td></tr>
  </table>

  输入输出说明：
  - `input`：输入灰度图，像素值范围 0~255，尺寸为 height × width，其中 width 和 height 均需大于2
  - `edge_list`：紧凑边缘列表，每个元素为 `{x, y, strength}`（各uint16_t），只包含幅值大于等于 `THRESHOLD` 的像素；列表内顺序由warp完成顺序决定，非像素位置顺序；图像边界像素不出现在列表中
  - `edge_count`：`edge_list` 中有效元素的数量

- 调用实现：
  使用内核调用符`<<<>>>`调用核函数，block数和每block线程数由`block_split`根据硬件AIV core数动态计算。

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

  - 指定路径 install_path，安装CANN软件包
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  mkdir -p build && cd build;                              # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..; make -j;   # 编译工程
  ./demo                                                   # 执行样例
  ```

  编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU 架构：本样例仅支持 dav-3510（Ascend 950PR/Ascend 950DT） |

  执行结果如下，说明精度对比成功。
  ```
  [Success] Case accuracy is verification passed.
  ```
