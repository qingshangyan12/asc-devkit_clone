# 向量乘加性能调优样例

## 概述

逐元素向量乘加是一种基础的向量运算，即将两个向量的逐元素乘积累加到目标向量上。

本样例以逐元素向量乘加为载体，展示如何用原子加进行指令优化。与[atomic_histogram样例](../atomic_histogram)不同，本样例中各线程访问互不重叠的地址，**不存在同地址竞争，原子性本身并非功能需求**，使用原子加是一种主动引入的优化手段。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── vector_muladd_atomic
│   ├── CMakeLists.txt         // 样例构建脚本
│   ├── vector_muladd_atomic.asc       // Ascend C SIMT核函数实现 & Host调用样例
│   ├── README.md
│   └── README_en.md
```

## 样例描述

本样例对三个float数组执行向量乘加，将`b`与`c`的逐元素乘积累加到`a`上。

- 计算公式：

  ```text
  a[i] += b[i] * c[i],  i = 0, 1, ..., element_count - 1
  ```

  三个数组的取值均为2的负幂的小整数倍（0.5/0.25/0.125），乘积与和在float下都能精确表示，因此校验可直接做精确比对，无需设置浮点容差。

- 样例规格：

  <table>
  <tr><td align="center">样例类型（OpType）</td><td colspan="4" align="center">Atomic Optimization</td></tr>
  <tr><td rowspan="4" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">c</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">a</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">vector_muladd_plain / vector_muladd_atomic</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过2个Case构成优化对比，各Case的核函数、累加方式如下表所示。

| Case | 核函数 | 计算方式 | 
|---|---|---|
| 0 | vector_muladd_plain | a[index] += b[index] * c[index] | 
| 1 | vector_muladd_atomic | asc_atomic_add(&a[index], b[index] * c[index]) | 

### 性能指标说明

| 指标 | 说明 |
| --- | --- |
| Task Duration（μs） | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |
| 循环体指令数 | 循环体编译后的指令条数，决定每个Warp每次迭代需要发射多少条指令 |

---

### Case 0：a[i] += b[i] * c[i]

**样例目标**：各线程直接进行`a[index] += b[index] * c[index]`计算，作为性能基线。

**核心实现**：启动64个线程块，每个线程块2048个线程。各线程以总线程数`gridDim.x * blockDim.x`为固定步长遍历整个数组，每处理一个元素，都执行一次乘加：算出`b[i] * c[i]`，累加到`a[i]`的旧值上，写回`a[i]`。

线程必须先把`a[i]`的旧值读出来才能参与累加，因此每个元素产生**3次读（a、b、c）、1次写（a）**。

对应的核函数实现如下。

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void vector_muladd_plain(
    float* a, const float* b, const float* c, uint32_t element_count)
{
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; index < element_count; index += stride) {
        a[index] += b[index] * c[index];
    }
}
```

**性能数据**：

| Case | Task Duration（μs） | 
|:---:|:---:|
| 0 | 51.70 | 


---

### Case 1：asc_atomic_add(&a[i], b[i] * c[i])

**样例目标**：用[asc_atomic_add()](../../../../../docs/zh/api/SIMT-API/atomic_operations/asc_atomic_add.md)把乘加交给原子加完成，线程无需读出`a[i]`的旧值，验证少发一条读指令带来的性能收益。

**核心实现**：线程块数量、线程数、遍历方式与Case 0完全相同。各线程只算出`b[i] * c[i]`的乘积，再对`a[i]`发起一次原子加，`a[i]`的旧值不必读出。

每个元素的读取从3次降为**2次（b、c）**，写仍为1次。

> **说明：** 本Case中各线程访问互不重叠的地址，不存在同地址竞争。使用原子加并非出于原子性的功能需求，而是借用原子加"就地完成累加、线程无需读出旧值"的特性来减少循环体的指令条数。这与[atomic_histogram样例](../atomic_histogram)形成对照：后者的原子性是功能必需，且因高竞争而需要优化掉GM原子操作；本样例则是主动引入原子操作作为优化手段。

对应的核函数实现如下。

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void vector_muladd_atomic(
    float* a, const float* b, const float* c, uint32_t element_count)
{
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; index < element_count; index += stride) {
        asc_atomic_add(&a[index], b[index] * c[index]);
    }
}
```

**性能数据**：

| Case | Task Duration（μs） | 
|:---:|:---:|
| 1 | 47.72 | 

**分析**：

- **循环体指令数减少**。Case 0的循环体包含3次读、1次写；Case 1减少了一次读`a[i]`的读指令，包含2次读、1次写。
- **耗时减少8.1%**，从51.70μs降至47.72μs。

**结论**：把累加交给原子加完成，线程省去了读出`a[i]`旧值的那条读指令，耗时下降8.1%。收益来自线程少发一条读指令。

---

## 性能对比总结

| Case | 累加方式  | 线程读取次数 | Task Duration（μs） | 相对基线 |
|:---:|:---:|:---:|:---:|:---:|
| 0 | a[i] += b[i] * c[i] |  3 | 51.70 | 1× |
| 1 | asc_atomic_add | 2 | 47.72 | **1.08×** |

耗时降幅8.1%。线程读取次数从3次降为2次，降幅看似应有33%，实际只有8.1%，原因如下：

- **读GM的耗时大头是数据往返的延迟，而这段延迟省不掉。** 发出一条读指令只占用很短的发射时间，之后大部分时间都在等数据从GM返回。两条读指令的等待是重叠的，去掉一条，另一条的等待时长不变，因此省下的只有那条指令的发射时间，数据往返的延迟依然要等。


## 调优建议

**累加类场景优先考虑用原子加完成累加**：当目标地址的旧值仅用于累加、不参与其他计算时，改用原子加可以省去线程读出旧值的那条指令，减少指令数。

**注意适用前提**：本优化的收益来自线程少发一条指令，而非原子性，也不是数据搬运量的减少。若访问地址存在竞争，同地址上的原子操作只能串行执行，反而会带来显著的性能损失（详见[atomic_histogram样例](../atomic_histogram)与[atomic_add_perf样例](../../../02_features/01_api_features/02_atomic_operation/atomic_add_perf/README.md)）。使用前需确认各线程访问的地址互不重叠。


## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}`为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  mkdir -p build && cd build                                # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..               # 配置工程
  make -j                                                   # 编译样例
  ./vector_muladd_atomic 0                                          # 执行Case 0（基线）
  ./vector_muladd_atomic 1                                          # 执行Case 1（优化）
  ```

  编译选项说明：

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SKIP_VALIDATION` | `ON`/`OFF` | 是否跳过结果校验，默认`OFF`。使用`msopprof`采集性能时建议设为`ON` |

  执行结果如下，说明精度对比成功。

  ```text
  Case 0 (plain update): 4194304 elements, 64 blocks, 2048 threads per block
  [Success] Case accuracy is verification passed.
  ```

## 性能分析

使用`msOpProf`工具获取详细性能数据：

```bash
msopprof ./vector_muladd_atomic 0    # 采集Case 0性能
msopprof ./vector_muladd_atomic 1    # 采集Case 1性能
```

> **关于性能采集时的Validation failed：** 本样例的累加目标`a`在host侧分配时写入初值，kernel仅在其上累加。`msopprof`的warmup+replay会在同一块GM内存上重复执行kernel，`a`被累加多次，因此严格校验模式下会报`Validation failed`。该现象是replay机制与校验逻辑的固有冲突。采集性能时建议先以`-DSKIP_VALIDATION=ON`重新编译再执行`msopprof`，跳过校验。

命令完成后，会在默认目录下生成以"OPPROF_{timestamp}_XXX"命名的文件夹，性能数据文件夹结构示例如下：

```text
├──dump                       # 原始的性能数据，用户无需关注
├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
├──L2Cache.csv                # L2 Cache命中率
├──Memory.csv                 # UB，L1和主存储器读写带宽速率
├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
├──OpBasicInfo.csv            # 算子基础信息
├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
└──visualize_data.bin         # MindStudio Insight呈现文件
```

