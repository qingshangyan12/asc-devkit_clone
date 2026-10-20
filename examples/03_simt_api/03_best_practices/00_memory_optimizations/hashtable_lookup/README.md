# SIMT SoA vs AoS哈希表查找性能优化样例

## 概述

本样例以平铺开放寻址哈希表的查找为例，介绍Ascend C SIMT编程方式下哈希表查找的访存与并行优化思路。首先，以AoS（Array of Structures）逐槽探测建立基线，暴露value内嵌槽内导致探测步长被撑大到40B的布局代价；然后，将key与value字段分区存储（SoA，Structure of Arrays），使探测路径与value区完全解耦；接着，在key区前面引入digest预过滤区；最终，使用warp协作组探测，让协作组中的线程并行探测同一个查询key。本文完整呈现该样例从内存布局、访问粒度到并行协作的调优路径。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR&950DT系列产品 | >= CANN 9.1.0 |

## 目录结构介绍

```text
├── hashtable_lookup
│   ├── figures                          // README中的图片资源
│   ├── CMakeLists.txt                   // 编译工程文件
│   ├── hashtable_lookup.asc              // SIMT哈希表查找优化路径实现
│   └── README.md                        // 样例说明文档
```

## 样例描述

- 查找语义：

  ```text
  founds[i] = (keys[i] ∈ table)
  ```

  - keys为查询集，形状为[1, 1048576]，数据类型为int64，为1M条混合查询key：80%（838861条）从已装表key中抽取、必命中，20%（209715条）为从未装入表的key、必未命中，两部分shuffle打散（查询集允许重复key）
  - founds为命中标志，形状为[1, 1048576]，数据类型为uint8；
- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">hashmap_lookup</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">keys</td><td align="center">[1, 1048576]</td><td align="center">int64</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">founds</td><td align="center">[1, 1048576]</td><td align="center">uint8</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">预置数据</td><td colspan="4" align="center">哈希表设备侧预置：按Case构建其一（AoS表40MB / SoA表40MB / SoA+digest表41MB），各Case共享同一逻辑槽位分配</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">lookup_aos_kernel / lookup_soa_kernel / lookup_soa_digest_swar_kernel / lookup_coop_swar_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过4个Case构成一条优化路径，各Case的核函数、并行模型、每轮探测内容和主要变化如下表所示。

| Case | 核函数 | 并行模型 | 每轮探测 | 本步引入的变化 |
| ---- | ------ | -------- | -------- | -------------- |
| 0    | `lookup_aos_kernel` | thread-per-key | 逐槽读8B key（40B步长） | 以AoS逐槽探测建立基线 |
| 1    | `lookup_soa_kernel` | thread-per-key | 逐槽读8B key（8B步长） | key/value分区（SoA布局） |
| 2    | `lookup_soa_digest_swar_kernel` | thread-per-key | 4B word加载4个digest | digest预过滤 + vcmp_eq4批量比较 |
| 3    | `lookup_coop_swar_kernel` | warp协作 | 16槽窗口（4 lane × 每 lane 4槽） | 线程组协作探测 |

Case 0~Case 2为thread-per-key模型：每个线程独立处理一条查询key，1M条查询对应2048个线程块 × 512线程。Case 3为warp协作模型：warp内4个线程组成协作组，共同探测一条key的窗口，每个warp同时推进8条查询，外层以warp为单位grid-stride覆盖全部查询。

各Case的物理表由host侧同一份逻辑槽位派生：同一哈希函数、同一起始槽位、同一线性探测序——Case之间只有物理布局与访问方式有所区别，对比条件严格可控。

### SoA,AoS内存布局

**图1　SoA与AoS内存布局示意图** 

<img src="figures/内存布局示意图.drawio.png" width="70%">

该图对比了 AoS（Array of Structures，结构体数组） 与 SoA（Structure of Arrays，数组结构体） 两种内存布局：
- AoS 布局：每个 KEY 后紧跟对应的 VALUE，键值对交替存储，数据局部性较好。
- SoA 布局：所有 KEY 连续存储，所有 VALUE 集中存储，便于连续访问同类数据。

图中 KEY 占用 8B，VALUE 占用 32B。AoS 更适合键值对整体访问，SoA 更适合批量读取 KEY、执行并行查找等场景。

### 哈希表查找机制

哈希表为**平铺开放寻址**：容量1M槽（2^20），以负载因子0.9装入943718个互不重复的随机key，空槽以key = EMPTY_KEY标记。查找从起始槽位开始线性探测：Key 匹配则命中，遇空槽则 Miss，否则继续探测，最多遍历全表一周。

负载因子α=0.9时，miss查找的期望探测次数由Knuth线性探测公式给出：

$$
E_{miss} = \frac{1}{2}\left(1 + \frac{1}{(1-\alpha)^2}\right) = \frac{1}{2}\left(1 + \frac{1}{0.01}\right) \approx 50
$$

### 性能指标说明

| 指标 | 说明 |
| ---- | ---- |
| Task Duration(μs) | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |
| read_main_memory_data(MB) | 任务执行期间从主存储器读取的总数据量，为`Memory.csv`中各block的read_main_memory_datas(KB)求和 |

各Case的性能数据使用msOpProf工具对可执行文件单独采集得到：Task Duration记录于输出目录`OpBasicInfo.csv`，read_main_memory_data记录于输出目录`Memory.csv`。

性能数据实测环境：Ascend950PR_9589，CANN 9.2.0。

### Case 0：AoS逐槽探测基线

**优化目标**：建立AoS（Array of Structures）布局下逐槽探测的耗时基线，暴露value内嵌槽内导致探测步长被撑大到40B的布局代价。

**核心实现**：一个槽 = {key, value[8]}共40B，value数组内嵌槽内；查找逻辑逐槽读8B key比较、空槽早退、全表回绕。128B的cache line只能容纳3.2个槽，查找逻辑逐行探测整张表。

**关键代码**：

```cpp
// AoS：一个槽 = {key, value[8]} 共 40B，value 内嵌槽内
struct AoSSlot {
  uint64_t key;
  float value[DIM];
};
// 逐槽探测：读 8B key，命中 / 空槽即返回；步长被 value 撑大到 40B
const uint64_t slot_key = slots[idx].key;
if (slot_key == target_key) { return true; }
if (slot_key == EMPTY_KEY) { return false; }
idx = (idx + 1) & (CAPACITY - 1);  // 全表回绕
```

**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data(MB) |
| ---- | :---------------: | :------------------------: |
| 0    |     1850.870      |          679.142           |

**分析**：

AoS把value内嵌在槽内，128B的cache line只能容纳3.2个槽。按期望50次探测计算，一次miss查找的访存跨度为：

$$
S_{AoS} = 50 \times 40B = 2000B \approx 16 \text{ 条 cache line}
$$

在查找到目标key或者在空槽早退之前不需要value字段，但value字段却作为无效载荷占据探测路径上的每个槽位，探测逐行踩过整张表，布局本身成为主要开销。此时探测真正需要的只有8B的key——若把key独立成紧凑的key区，步长即压缩到8B，且不再搬运任何无效的value字节，访存局部性显著提升，因此下一步优化将AoS布局修改为SoA布局。

---

### Case 1：SoA纯布局改造

**优化目标**：将key与value字段分区存储（SoA，Structure of Arrays），使探测路径与value区完全解耦，消除内嵌value对探测访存跨度的干扰。

**核心实现**：与Case 0算法完全一致——逐槽读8B key比较、空槽早退、全表回绕；唯一差异是内存布局。SoA的key区（8MB）与value区（32MB）各自成区，字段为数组直存、无指针。

**图2　SoA_Cacheline搬运示意图** 

![Cacheline_SoA.drawio](figures/Cacheline_SoA.drawio.png)

图2对比了 AoS 与 SoA 布局在 Cacheline 利用上的差异：AoS 中 KEY 和 VALUE 交替存储，查询 KEY 时会将相邻的 VALUE 一同加载；SoA 中所有 KEY 连续存储，单条 128B Cacheline 可容纳更多 KEY（如 16 个 8B KEY），从而减少查找过程中不必要的 VALUE 数据加载，降低 Cacheline 搬运开销，提高缓存利用率。

**关键代码**：

```cpp
// SoA：key / value 两个字段各自成区
struct SoaTableNoDigest {
  uint64_t key[CAPACITY];
  float value[CAPACITY][DIM];
};
// 探测只在 8B 步长的 key 区内滑动，value 区与探测路径完全解耦
const uint64_t slot_key = table->key[idx];
if (slot_key == target_key) { return true; }
if (slot_key == EMPTY_KEY) { return false; }
idx = (idx + 1) & (CAPACITY - 1);  // 全表回绕
```

**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data(MB) | 性能对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 0    |    1850.870    |          679.142           |  -   |
| 1    |    1158.006    |          249.740           | x1.60 |

**分析**：

同一算法、同一测试集仅改变物理布局，Task Duration从1850.870μs降至1158.006μs。两种布局下一次查找的访存跨度为：

$$
S_{AoS} = 50 \times 40B = 2000B \approx 16 \text{ 条 cache line}
$$

$$
S_{SoA} = 50 \times 8B = 400B \approx 4 \text{ 条 cache line}
$$

Case 1相对Case 0提升x1.60；value区（32MB）整体搬离探测路径。但查看结构体发现探测仍需逐槽读取8B的key，"判断这个槽位是否匹配"这件事的读取宽度还是8B。由于查找定位起始槽本来就要计算目标key的哈希值，哈希值又是key的确定性函数，因此我们可以为每个槽预存其key哈希值的高8位作为1B的digest

---

### Case 2：digest预过滤与word批量比较

**优化目标**：若能把匹配判断的读取宽度压到1B、只在匹配时才读key确认，探测跨度的字节数还能继续降低，所以我们在key区前面增加1B/槽的digest预过滤区（哈希高8位），digest不等则key必不等，先用1B比对排除绝大多数槽位，digest命中才读8B key精确确认；同时以4B word为单位批量加载digest，四路字节并行比较，保证主循环访存的流水连续性。

**核心实现**：digest取key哈希值的高8位；空槽的digest为hash(EMPTY_KEY)的高8位。每轮按4B对齐加载一个word（4个连续digest），两次vcmp_eq4分别与目标digest、空槽digest比较；候选槽位集合通过 `__ffs` 逐个提取，读取命中槽位的 8B key 进行精确确认。

**图3　digest_Cacheline示意图**

![Cacheline_digest.drawio](figures/Cacheline_digest.drawio.png)

图3 通过在 SoA 布局中增加 1B 的 digest 字段，将 KEY 的哈希摘要连续存储，使单条 Cacheline 能容纳更多 digest（128B 可存储 128 个 digest），从而减少查找时的 Cacheline 搬运，并通过 digest 预筛选减少 8B KEY 的读取次数。

**关键代码**：

```cpp
// digest 取 key 哈希值的高 8 位（1B/槽），建表时随槽位一次性写入
__host__ __aicore__ inline uint8_t DigestOf(uint64_t hashed) {
  return static_cast<uint8_t>(hashed >> 32);
}

// 查找侧：目标 digest 复制 4 份拼成 4B word，供 vcmp_eq4 四路字节并行比较
__host__ __aicore__ inline uint32_t DigestWordOf(uint64_t hashed) {
  return static_cast<uint32_t>(DigestOf(hashed)) * 0x01010101u;
}

// SoA + digest 查找主循环：每轮一次 4B word load + 两次 vcmp_eq4
__aicore__ inline bool FindSoaDigestVcmp(const SoaTable* table,
                                         uint64_t target_key,
                                         uint32_t start,
                                         uint32_t target_word) {
  const uint32_t empty_word = DigestWordOf(Murmur3Hash(EMPTY_KEY));
  const uint32_t first_pos = start & (CAPACITY - STRIDE);  // 起始槽对齐到 word
  for (uint32_t offset = 0; offset < CAPACITY + STRIDE; offset += STRIDE) {
    const uint32_t pos = (first_pos + offset) & (CAPACITY - 1);

    // 1. 比目标 digest：__ffs 逐个消费命中槽位，读 8B key 精确确认
    const uint32_t probe =
        *reinterpret_cast<const uint32_t*>(table->digest + pos);
    uint32_t cmp = VcmpEq4(probe, target_word) & 0x01010101u;
    while (cmp != 0) {
      const uint32_t idx =
          pos + (((static_cast<uint32_t>(
                       __ffs(static_cast<int32_t>(cmp)))) - 1u) >> 3);
      cmp &= cmp - 1;
      if (table->key[idx] == target_key) {
        return true;
      }
    }

    // 2. 比空槽 digest：起始位置之后确认空槽 → miss 早退
    cmp = VcmpEq4(probe, empty_word) & 0x01010101u;
    while (cmp != 0) {
      const uint32_t idx =
          pos + (((static_cast<uint32_t>(
                       __ffs(static_cast<int32_t>(cmp)))) - 1u) >> 3);
      cmp &= cmp - 1;
      if (offset == 0 && idx < start) {
        continue;  // 回绕保护：第 0 轮 word 内位于 start 之前的槽，
                   // 逻辑上是回绕后才探到的，空了不能作为 miss 证据
      }
      if (table->key[idx] == EMPTY_KEY) {
        return false;  // 读 key 确认，排除 digest 假阳性
      }
    }
  }
  return false;
}
```

**处理逻辑**：
digest预过滤处理流程分为三步：
1. **Digest 预筛选**：每个槽位存储 1B 的 key 哈希摘要，每轮连续读取 4B，通过 VcmpEq4 并行比较 4 个槽位的 digest。
2. **候选确认**：使用 __ffs 逐个提取匹配槽位，读取 8B key 精确确认，排除 digest 假阳性。
3. **空槽早退**：并行匹配空槽 digest 后，读取对应 key 确认槽位确实为空，即可提前判定查找失败。

**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data(MB) | 性能对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 1    |    1158.006    |          249.740           |  -   |
| 2    |    829.345     |          295.490           | x1.40 |

**分析**：

引入 digest 后，先通过一次 4B 加载并行筛选 4 个槽位，再读取候选槽位的完整 key，将一次查找中约 50 次 8B key 读取减少到约 13 次 4B 加载。
$$
S_{digest} = 13 \times 4B \approx 52B < 1 \text{ 条 cache line}
$$

整个digest区仅1MB，一次探测的跨度稳落单条cache line内，Case 2相对Case 1性能提升x1.40。但目前仍存在访存效率问题：每个 lane 各自查询一个 key，读取的 4B digest 分散在不同的 Cacheline 中，难以合并访存请求。整个 warp 必须等待最慢的请求完成，同时需要处理大量独立的访存事务。因此，我们引入 Case 3 的协作探测，让多个 lane 组队查询同一个 key，通过连续访存减少请求数量，提高访存效率。


---

### Case 3：协作探测

**优化目标**：将每个 key 的串行探测轮数从约 13 轮压缩到约 4 轮。方法是结合线程协作与 digest 预筛选：4 个线程组成一个协作组，共同探测同一个 key，每个线程负责窗口内连续的 4 个槽位，通过一次 4B word 加载和两次 vcmp_eq4 完成比较，整个协作组每轮可覆盖 16 个槽位。

**核心实现**：每个 warp 的 32 个线程划分为 8 个协作组，每组 4 个线程，共同查询一个 key，因此一个 warp 可以同时处理 8 条查询事务。每轮每组协作检查连续的 16 个槽位，通过 digest 并行筛选候选槽位，再读取完整 key 进行精确确认。每轮探测推进采用 warp 统一调度：已完成查询的线程不再产生有效结果，但仍参与同步投票；当整个 warp 的查询全部完成后，再开始处理下一批查询。最后，每个 warp 通过分批处理方式遍历全部 1M 条查询。

**图4　协作组并行访问key示意图**

![协作组查找.drawio](figures/协作组查找.drawio.png)

图四展示了协作组查询 key 的过程：多个线程通过合并访存连续读取候选 key 到寄存器，再通过组内投票生成匹配位图，最后选出命中线程，由其负责处理命中的 key（如 key3），从而减少访存开销并避免多个线程重复处理同一结果。

**关键代码**：

以下代码在 digest 预筛选的基础上引入协作组机制：每 4 个线程协作查询同一个 key，通过组内投票汇总匹配结果，并由命中线程完成后续处理；同时保留空槽检测与回绕保护，确保查找结果正确。

```cpp
// 每 lane 一次 4B word load + 两次并行比较（每 warp-轮 32 次 word load = 128 槽）
const uint32_t word =
    *reinterpret_cast<const uint32_t*>(table->digest + pos);
uint32_t tmask = VcmpEq4(word, target_word) & 0x01010101u;
uint32_t emask = VcmpEq4(word, empty_word) & 0x01010101u;
if (round == 0) {  // 回绕保护（字节粒度）：清位于 start 之前的字节
  const uint32_t shift = (start > pos)
      ? (kSlotsPerLane < start - pos ? kSlotsPerLane : start - pos) : 0u;
  emask &= ~static_cast<uint32_t>((1ull << (shift * 8)) - 1ull);
}
// word 内 __ffs 迭代消费位图读 key 确认，跨 lane 由投票汇总（2 票/轮）
const uint32_t match_ballot = asc_ballot(match);
if (((match_ballot >> group_shift) & kGroupMask) != 0) {
  found = true;
  done = true;
}
```

**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data(MB) | 性能对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 2    |    829.345     |          295.490           |  -   |
| 3    |    596.943    |          284.774           | x1.39 |

**分析**：

Case 3 的关键在于：在保持访存指令数量不变的前提下，减少实际访存事务，并缩短每个 key 的串行探测过程。 每个 warp 每轮同样执行 32 次 4B word 加载，覆盖 128 个槽位，但查询分配方式不同：Case 2 将这 128 个槽位分配给 32 条查询，每个 key 每轮探测 4 个槽位，约需 13 轮；Case 3 则分配给 8 条查询，每个 key 每轮探测 16 个槽位，约需 4 轮。

两种探测方式不同，使得两者的访存合并效率不同。Case 2 中，各线程查询不同的 key，访问地址彼此分散，32 次加载可能产生多达 32 个独立访存事务；Case 3 中，每组 4 个线程访问连续地址，4 次 4B 加载覆盖同一段 16B 数据，可合并为一次访存事务。因此，Case 3 不仅扩大了每个 key 每轮的探测范围，还减少了访存事务数量，最终成为本样例中耗时最低的实现。


## 性能对比总结

| Case | 核函数 | Task Duration(μs) | read_main_memory_data(MB) | 相对Case 0 |
| ---- | ------ | :---------------: | :------------------------: | :--------: |
| 0    | `lookup_aos_kernel` | 1850.870 | 679.142 | 1.00x |
| 1    | `lookup_soa_kernel` | 1158.006 | 249.740 | 1.60x |
| 2    | `lookup_soa_digest_swar_kernel` | 829.345 | 295.490 | 2.23x |
| 3    | `lookup_coop_swar_kernel` | 596.943 | 284.774 | 3.10x |


## 调优建议

**图5　不同负载因子下各优化方案的查找性能对比图**

![image-20261016155256161](figures/image-20261016155256161.png)

图 5 展示了不同优化方案在不同负载因子下的查找性能变化。当负载因子较低时，Case 0（AoS）和 Case 1（SoA）均能保持较好的查找性能；随着负载因子增加，哈希冲突增多、探测序列变长，各方案的耗时均明显上升。相比之下，SoA 布局减少了不必要的数据加载，Digest 预筛选降低了完整 key 的读取开销，而协作探测通过扩大单轮探测范围、合并访存请求，进一步缓解了高负载下的性能退化。

因此，不同优化手段需要结合具体的访问模式和性能瓶颈选择，才能发挥更好的优化效果。 具体来看：
1. 内存布局优化： 当 workload 存在局部同字段批量访问时，优先考虑 SoA 而非 AoS，将同类字段连续存储，减少无关数据的加载，提高 Cacheline 利用率。
2. 摘要预过滤： 本质是以少量额外空间换取查找时间。对于哈希值这类可提取摘要的场景，可先并行比较短摘要，筛除大部分不匹配项，再读取完整 key 精确确认，减少不必要的访存。
3. 协作探测： 当查询存在较长的探测序列时，可让多个线程协作查询同一个 key，使各线程访问连续地址，将分散的访存请求合并为更少的事务，从而缩短串行探测时间并提高访存效率。

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
  >
- 编译选项说明

  | 选项                        | 可选值       | 说明                                                       |
  | --------------------------- | ------------ | ---------------------------------------------------------- |
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`sim` | 运行模式：NPU运行、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR&950DT系列产品） |
  | `SCENARIO_NUM`            | `0`-`3`  | 样例类型，默认为3。0: AoS逐槽探测基线；1: SoA纯布局改造；2: digest预过滤与word批量比较；3: 协作探测×SWAR组合 |

  - `--cce-res-usage`用于输出寄存器和栈等编译资源信息，便于分析资源使用。

- 执行结果

  执行结果如下（默认Case 3，其余Case仅"Running case"行不同），末行说明精度校验通过。

  ```text
  Usage: ./hashtable_lookup (compile-time SCENARIO_NUM=3)
  Running case 3: coop G=4 x swar: 16-slot window, asc_ballot...
  Threads/Thread Block: 512
  Table: capacity=1048576 slots, load factor=0.9, inserted=943718
  Queries: 1048576 mixed keys, hit ratio=0.8 (838861 hits + 209715 misses)
  Expected probes: hit ~5.5, miss ~50, mixed ~14.4 (Knuth, a=0.9)
  Soc: Ascend950PR_9589  device: 0
  Hits: actual=838861, expected=838861
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
