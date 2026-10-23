# SIMT SoA vs AoS哈希表查找性能优化样例

## 概述

本样例以平铺开放寻址哈希表的查找为例，介绍Ascend C SIMT编程方式下，如何通过修改哈希表结构，对哈希表查找进行优化的思路。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.2.0 |

## 目录结构介绍

```text
├── hashtable_lookup
│   ├── figures                          // README中的图片资源
│   ├── CMakeLists.txt                   // 编译工程文件
│   ├── hashtable_lookup.asc             // SIMT哈希表查找优化路径实现
│   ├── README.md                  	     // 样例说明文档
│   └── README_en.md               	     // 英文样例说明文档
```

## 样例描述

- 样例功能：实现哈希表查找算子，该哈希表处理哈希冲突基于开放寻址法，具体参考[背景知识](../../../../../examples/03_simt_api/02_features/01_api_features/00_memory_access/insert_hash_table/README.md)，本样例通过不同的哈希表结构设计以及线程访问方式查找哈希表并进行性能比较，展示不同优化手段对性能的影响。

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型（OpType）</td><td colspan="4" align="center">hashtable_lookup</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">keys</td><td align="center">[1, 1048576]</td><td align="center">uint64_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">indices</td><td align="center">[1, 1048576]</td><td align="center">uint32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">预置数据</td><td colspan="4" align="center">AoS表40MB / SoA表40MB / SoA+digest表41MB——按Case构建其一</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">lookup_aos_kernel / lookup_soa_kernel / lookup_soa_digest_swar_kernel / lookup_coop_swar_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过4个Case构成一条优化路径，各Case的核函数和主要变化如下表所示。

| Case | 核函数 | 本步引入的变化 |
| ---- | ------ | -------------- |
| 0    | `lookup_aos_kernel` | 以哈希表查找基本实现作为基线 |
| 1    | `lookup_soa_kernel` | key/value分区存储 |
| 2    | `lookup_soa_digest_swar_kernel` | digest预过滤 + vcmp_eq4批量比较 |
| 3    | `lookup_coop_swar_kernel` | 线程组协作探测 |

### SoA与AoS内存布局

AoS（Array of Structures，结构体数组）与SoA（Structure of Arrays，数组结构体）为两种不同内存布局：
- AoS布局：每个结构体中包含一个对象的完整信息，多个结构体排列成数组存储在内存中。
- SoA布局：将对象的同种信息排列成数组，多个对象的同种信息存储在相邻内存中。

**图1　内存布局示意图**

![内存布局示意图](figures/soa_aos_layout.png)

图1以本样例的哈希表结构为例，展示了不同的内存布局。哈希表结构体代码实现如下所示：

```cpp
struct AoSBucket {
  uint64_t key;
  float value[DIM];
};
AoSBucket table[CAPACITY];

struct SoaTable {
  uint64_t key[CAPACITY];
  float value[CAPACITY][DIM];
};
SoaTable table;
```

### 性能指标说明

| 指标 | 说明 |
| ---- | ---- |
| Task Duration(μs) | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |
| read_main_memory_data(KB) | 任务执行期间每个block从GM读取的总数据量 |

### Case 0：AoS逐Bucket探测

**优化目标**：使用哈希表查找的基本实现建立性能基线。

**核心实现**：采用AoS内存设计，使用开放寻址法处理哈希冲突，实现基本哈希查找功能。

**关键代码**：

```cpp
struct AoSBucket {
  uint64_t key;
  float value[DIM];
};

__aicore__ inline uint32_t find_aos(const AoSBucket* table,
                                    uint64_t target_key, uint32_t start) {
  uint32_t idx = start;
  for (uint32_t i = 0; i < CAPACITY; ++i) {
    const uint64_t bucket_key = table[idx].key;
    if (bucket_key == target_key) {
      return idx;
    }
    if (bucket_key == EMPTY_KEY) {
      return NOT_FOUND;
    }
    idx = (idx + 1) & (CAPACITY - 1);
  }
  return NOT_FOUND;
}
```

**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data_all(MB) |
| ---- | :---------------: | :------------------------: |
| 0    |     1311.297      |          672.645           |

> read_main_memory_data_all(MB)为`Memory.csv`中各block的read_main_memory_data(KB)求和。

**分析**：

数据的load和store是以Cache Line为单位的，AoS将key和value存放在同一个Bucket里，其中value就占了32B，128B的Cache Line只能容纳3.2个Bucket。在查找到目标key之前不需要value字段，但value字段却作为无效载荷占据探测路径上的每个槽位，探测逐Bucket访问整张表，数据搬运成为主要开销。此时探测真正需要的只有8B的key——若把key独立成紧凑的key区，探测跨度就可以压缩到8B，且不再搬运任何无效的value字节，访存局部性显著提升，因此下一步优化将AoS布局修改为SoA布局。

---

### Case 1：SoA纯布局改造

**优化目标**：采用SoA（Structure of Arrays）优化内存布局，优化查找key的访存效率。

**核心实现**：与Case 0算法完全一致；唯一差异是内存布局。SoA把所有共8MB的key连续存在一起，所有共32MB的value连续存在一起，key和value分开存放。

**关键代码**：

```cpp
struct SoaTable {
  uint64_t key[CAPACITY];
  float value[CAPACITY][DIM];
};

__aicore__ inline uint32_t find_soa(const SoaTable* table, uint64_t target_key, uint32_t start)
{
    uint32_t idx = start;
    for (uint32_t i = 0; i < CAPACITY; ++i) {
        const uint64_t bucket_key = table->key[idx];
        if (bucket_key == target_key) {
            return idx;
        }
        if (bucket_key == EMPTY_KEY) {
            return NOT_FOUND;
        }
        idx = (idx + 1) & (CAPACITY - 1);
    }
    return NOT_FOUND;
}
```

**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data_all(MB) | 性能对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 0    |    1311.297    |          672.645           |  -   |
| 1    |    783.029    |          239.765           | 1.67x |

**分析**：

同一算法，Task Duration从1311.297μs降至783.029μs。Case 1相对Case 0提升x1.67；性能提升的关键在于两种布局对Cache Line的利用方式不同：

**图2　Cacheline示意图**

![Cacheline示意图](figures/cacheline.png)

图2对比了AoS与SoA布局在Cache Line利用上的差异：AoS中key和value交替存储，查询key时会将相邻的value一同加载；SoA中所有key连续存储，单条128B Cache Line可容纳更多key，例如16个8B的key，从而减少查找过程中不必要的value数据加载，降低Cache Line搬运开销，提高缓存利用率。

此时若能将探测查找的读取宽度继续缩减，降低每次探测所需字节数，则性能还会有所提升，因此我们引入Case 2。

---

### Case 2：digest预过滤

**优化目标**：提取key的局部信息作为预过滤字段，预过滤快速淘汰掉大多数key，减少探测所需字节数，提高访存效率。

**核心实现**：取key哈希值的高8位作为预筛选字段（digest），哈希值是key的确定性函数，同一个key建表和查找时算出的哈希值相同，所以digest对不上的Bucket存的一定不是目标key，直接跳过。每轮按4B对齐加载4个连续digest并进行匹配，`__ffs`逐个取出digest匹配的桶位——由于digest相同不代表key相同，因此根据匹配结果对key进行最终确认。线程处理同上。

**关键代码**：

```cpp
struct SoaTableWithDigest {
  uint8_t digest[CAPACITY];
  uint64_t key[CAPACITY];
  float value[CAPACITY][DIM];
};

// digest 取 key 哈希值的高 8 位
__host__ __aicore__ inline uint8_t digest_of(uint64_t hashed) {
  return static_cast<uint8_t>(hashed >> 32);
}

// 查找侧：目标 digest 复制 4 份拼成 4B word，供 vcmp_eq4 四路字节并行比较
__host__ __aicore__ inline uint32_t digest_word_of(uint64_t hashed) {
  return static_cast<uint32_t>(digest_of(hashed)) * 0x01010101u;
}

__aicore__ inline uint32_t find_soa_digest_vcmp(const SoaTableWithDigest* table,
                                             uint64_t target_key,
                                             uint32_t start,
                                             uint32_t target_word) {
  const uint32_t empty_word = digest_word_of(murmur3_hash(EMPTY_KEY));
  const uint32_t first_pos = start & (CAPACITY - STRIDE);  // 起始桶对齐到 word
  for (uint32_t offset = 0; offset < CAPACITY + STRIDE; offset += STRIDE) {
    const uint32_t pos = (first_pos + offset) & (CAPACITY - 1);

    // 比较目标 digest：__ffs 逐个取出 digest 匹配的桶位——digest 相同不代表 key 相同，还要读 8B key 最终确认
    const uint32_t probe =
        *reinterpret_cast<const uint32_t*>(table->digest + pos);
    uint32_t cmp = vcmp_eq4(probe, target_word) & 0x01010101u;
    while (cmp != 0) {
      const uint32_t idx =
          pos + (((static_cast<uint32_t>(
                       __ffs(static_cast<int32_t>(cmp)))) - 1u) >> 3);
      cmp &= cmp - 1;
      if (table->key[idx] == target_key) {
        return idx;  // 命中：返回 key 所在桶号
      }
    }

    cmp = vcmp_eq4(probe, empty_word) & 0x01010101u;
    while (cmp != 0) {
      const uint32_t idx =
          pos + (((static_cast<uint32_t>(
                       __ffs(static_cast<int32_t>(cmp)))) - 1u) >> 3);
      cmp &= cmp - 1;
      if (offset == 0 && idx < start) {
        continue;  
                   
      }
      if (table->key[idx] == EMPTY_KEY) {
        return NOT_FOUND; 
      }
    }
  }
  return NOT_FOUND;
}
```

**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data_all(MB) | 性能对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 1    |    783.029    |          239.765           |  -   |
| 2    |    483.379     |          279.439           | 1.62x |

**分析**：

通过在SoA布局中增加1B的digest字段，将digest连续存储，使单条Cache Line能容纳更多digest，从而减少查找时的Cache Line搬运。Case 2相对Case 1性能提升x1.62。但目前仍存在访存效率问题：每个lane各自查询一个key，读取的4B digest分散在不同的Cache Line中，难以合并访存请求。整个warp必须等待最慢的请求完成，同时需要处理大量独立的访存事务。因此，我们引入Case 3的协作探测，让多个lane组队查询同一个key，通过连续访存减少请求数量，提高访存效率。


---

### Case 3：协作探测

**优化目标**：通过线程组协作探测结合Case 2的结构体设计，做到访存合并，从而提高整体性能。

**核心实现**：每个warp的32个线程划分为8个协作组，每组4个线程，共同查询一个key。

**图3　协作组查找示意图**

![协作组查找示意图](figures/cooperative_group.png)

图3展示了协作组查询key的过程：多个线程读取连续存储的候选key到寄存器，再通过组内投票生成匹配位图，选出命中线程，由其负责处理命中的key，例如key3，从而减少访存开销并避免多个线程重复处理同一结果。

**关键代码**：

以下代码在digest预筛选的基础上引入协作组机制：每4个线程协作查询同一个key，通过组内投票汇总匹配结果，并由命中线程完成后续处理。

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void lookup_coop_swar_kernel(
    const SoaTableWithDigest* table, const uint64_t* keys, uint32_t* indices, uint32_t query_num) {
    ...
    const uint32_t word = *reinterpret_cast<const uint32_t*>(table->digest + pos);
    uint32_t tmask = vcmp_eq4(word, target_word) & 0x01010101u;
    uint32_t emask = vcmp_eq4(word, empty_word) & 0x01010101u;
    if (round == 0) {  
      const uint32_t shift = (start > pos)
          ? (kBucketsPerLane < start - pos ? kBucketsPerLane : start - pos) : 0u;
      emask &= ~static_cast<uint32_t>((1ull << (shift * 8)) - 1ull);
    }
    ...
    const uint32_t match_ballot = asc_ballot(match);
    if (((match_ballot >> group_shift) & kGroupMask) != 0) {
      found = true;
      done = true;
    }
    ...
}
```

**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data_all(MB) | 性能对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 2    |    483.379     |          279.439           |  -   |
| 3    |    423.152    |          283.177           | 1.14x |

**分析**：

前文各Case的性能数据均在负载因子0.9下测得，而负载因子不同，各优化方案的表现也会随之变化：

**图4　性能对比示意图**

![性能对比示意图](figures/performance_comparison.png)

图4展示了不同优化方案在不同负载因子下的查找性能变化。当负载因子较低时，协作探测的查找性能始终低于其它Case；但随着负载因子增加，哈希冲突增多、探测序列变长，此时协作探测通过扩大单轮探测范围、合并访存请求，缓解了高负载下的性能退化。

Case 3的关键在于：在保持访存指令数量不变的前提下，减少实际访存事务。两种探测方式不同，使得两者的访存合并效率不同。Case 2中，各线程查询不同的key，访问地址彼此分散，32次加载可能产生多达32个独立访存事务；Case 3中，每组4个线程访问连续地址，4次4B加载覆盖同一段16B数据，可合并为一次访存事务。因此，Case 3减少了访存事务数量，最终成为本样例中耗时最低的实现。


## 性能对比总结

| Case | 核函数 | Task Duration(μs) | read_main_memory_data(MB) | 相对Case 0 |
| ---- | ------ | :---------------: | :------------------------: | :--------: |
| 0    | `lookup_aos_kernel` | 1311.297 | 672.645 | 1.00x |
| 1    | `lookup_soa_kernel` | 783.029 | 239.765 | 1.67x |
| 2    | `lookup_soa_digest_swar_kernel` | 483.379 | 279.439 | 2.71x |
| 3    | `lookup_coop_swar_kernel` | 423.152 | 283.177 | 3.10x |

## 调优建议

1. 内存布局优化：当算子存在局部同字段批量访问时，优先考虑SoA而非AoS，将同类字段连续存储，减少无关数据的加载，提高Cache Line利用率。
2. digest预过滤：本质是以少量额外空间换取查找时间。对于哈希值这类可提取digest的场景，可先并行比较digest，筛除大部分不匹配项，再读取完整key精确确认，减少不必要的访存。
3. 协作探测：当查询存在较长的探测序列时，可让多个线程协作查询同一个key，使各线程访问连续地址，将分散的访存请求合并为更少的事务，从而缩短串行探测时间并提高访存效率。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/zh/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。
  >
- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=3                                                                     # 选择执行场景，可选0-3
  mkdir -p build && cd build;                                                        # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./hashtable_lookup                                                                  # 执行样例
  ```

  运行时环境变量`SOA_AOS_DEVICE=<id>`可指定执行设备，默认0。

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如下：

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;  # NPU仿真模式
  ```

  > **注意：** `SCENARIO_NUM`为CMake缓存变量，切换Case或切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。
- 编译选项说明

  | 选项                        | 可选值       | 说明                                                       |
  | --------------------------- | ------------ | ---------------------------------------------------------- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR&950DT系列产品） |
  | `SCENARIO_NUM`            | `0`-`3`  | 样例类型，默认为3。0：AoS逐槽探测基线；1：SoA纯布局改造；2：digest预过滤与word批量比较；3：协作探测×SWAR组合 |

  - `--cce-res-usage`用于输出寄存器和栈等编译资源信息，便于分析资源使用。

- 执行结果

  执行结果如下（各Case输出相同），末行说明精度校验通过。

  ```text
  [Success] Case accuracy verification passed.
  ```

## 性能调试

### msOpProf工具介绍

msOpProf工具是单算子性能分析工具。包含msopprof和msopprof simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

使用 `msOpProf` 工具获取详细性能数据：

```bash
msopprof ./hashtable_lookup   # 分析case的性能
```

命令执行完成后，会在输出目录生成以 `OPPROF_{timestamp}_XXX`命名的文件夹，性能数据文件夹结构示例如下：

```text
├── dump                       // 原始性能数据
├── ArithmeticUtilization.csv  // cube/vector指令cycle占比
├── L2Cache.csv                // L2 Cache命中率
├── Memory.csv                 // UB、L1和主存储器读写带宽
├── MemoryL0.csv               // L0A、L0B、L0C读写带宽
├── MemoryUB.csv               // Vector和Scalar到UB的读写带宽
├── OpBasicInfo.csv            // 算子基础信息，包含Task Duration(μs)
├── PipeUtilization.csv        // 计算单元和搬运单元耗时及占比
├── ResourceConflictRatio.csv  // UB bank group、bank conflict和资源冲突率
└── visualize_data.bin         // MindStudio Insight呈现文件
```

本样例README中的Task Duration即取自`OpBasicInfo.csv`：

```bash
cat ./OPPROF_*/OpBasicInfo.csv
```

- 仿真性能采集

  可以使用 `msopprof simulator` 进行仿真性能分析，生成可视化的指令流水图等信息。命令如下：

  ```bash
  SCENARIO_NUM=3                                                                     # 选择执行场景，可选0-3
  mkdir -p build && cd build;                                                        # 创建并进入build目录
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  msopprof simulator --soc-version=<soc_version> ./hashtable_lookup
  ```

> 使用仿真调优功能前，需要在 CMakeLists.txt 中添加 `-g` 编译选项，用于生成调试信息，使仿真器能够采集指令流水图。`soc_version` 获取方式及仿真调优其他说明可参考[仿真调优样例](../../../01_utilities/07_simulator)。

  命令完成后，会在当前目录下生成以 `OPPROF_{timestamp}_XXX` 命名的文件夹，产物结构如下：

  ```text
  OPPROF_{timestamp}_XXX/
  ├── dump                    // 原始性能数据，用户无需关注
  └── simulator
      ├── core*.veccore*/     // 各向量核的仿真指令流水图文件
      └── visualize_data.bin  // MindStudio Insight呈现文件
  ```

  执行后，可以在 **MindStudio Insight** 中打开 `visualize_data.bin` 查看可视化指令流水图。

更多 `msOpProf` 工具使用方法，请参见[MindStudio工具调优（msOpProf）快速入门](https://www.hiascend.com/document/detail/zh/canncommercial/900/devaids/optool/docs/zh/quick_start/msopprof_quick_start.md)。
