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

  - keys为查询集，形状为[1, 1048576]，数据类型为int64，为1M条从未装入表的互不重复key
  - founds为命中标志，形状为[1, 1048576]，数据类型为uint8；本样例中全为0
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
| 3    | `lookup_coop_swar_kernel` | warp协作（G=4） | 16槽窗口（4 lane × 每 lane 4槽） | 协作探测 × SWAR组合 |

Case 0~Case 2为thread-per-key模型：每个线程独立处理一条查询key，1M条查询对应2048个线程块 × 512线程。Case 3为warp协作模型：warp内4个线程组成协作组，共同探测一条key的窗口，每个warp同时推进8条查询，外层以warp为单位grid-stride覆盖全部查询。

各Case的物理表由host侧同一份逻辑槽位派生：同一哈希函数、同一起始槽位、同一线性探测序——Case之间只有物理布局与访问方式有所区别，对比条件严格可控。

### SoA,AoS内存布局

**图1　SoA与AoS内存布局示意图** 

<img src="figures/内存布局示意图.drawio.png" width="70%">

图中左右两栏对比同一份槽位数据的两种物理布局。左栏AoS：每个槽内KEY（8B）与VALUE[DIM]（32B）交错排列，槽步长被撑大到40B——仅读完3个连续槽的key，探测就要跨越约120B；右栏SoA：KEY与VALUE[DIM]各自成区，key区中3个连续key仅占24B，value区独立成区、与探测路径完全解耦。miss查找全程不需要value，布局差异直接决定探测路径上要踩过的字节数。（这里可以简写）

### 哈希表查找机制

哈希表为**平铺开放寻址**：容量1M槽（2^20），以负载因子0.9装入943718个互不重复的随机key，空槽以key = EMPTY_KEY标记。查找从起始槽位开始线性探测：Key 匹配则命中，遇空槽则 Miss，否则继续探测，最多遍历全表一周。LF=

负载因子α=0.9时，查找的期望探测次数由Knuth线性探测公式给出：（在结论处给出负载因子变化的性能改变）

$$
E_{miss} = \frac{1}{2}\left(1 + \frac{1}{(1-\alpha)^2}\right) = \frac{1}{2}\left(1 + \frac{1}{0.01}\right) \approx 50
$$







### 性能指标说明

| 指标 | 说明 |
| ---- | ---- |
| Task Duration(μs) | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |
| read_main_memory_data(MB) | 任务执行期间从主存储器读取的总数据量，为`Memory.csv`中各block的read_main_memory_datas(KB)求和 |

本样例程序内不计时，各Case的性能数据使用msOpProf工具对可执行文件单独采集得到：Task Duration记录于输出目录`OpBasicInfo.csv`，read_main_memory_data记录于输出目录`Memory.csv`。

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

**图2　Cacheline搬运示意图** （修改一下，Cacheline角度的）

![Cacheline_SoA.drawio](figures/Cacheline_SoA.drawio.png)



**关键代码**：

```cpp
// SoA：key / value 两个字段各自成区，数组直存、无指针
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

**性能数据**：（保留三位小数）（可以再扩展一下msopprof里面的搬运数据）

| Case | Task Duration(μs) | read_main_memory_data(MB) | 对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 0    |    1850.870    |          679.142           |  -   |
| 1    |    1158.006    |          249.740           | x1.60 |

**分析**：

同一算法、同一逻辑槽位，仅改变物理布局，Task Duration从1850.870μs降至1158.006μs。两种布局下一次查找的访存跨度为：

$$
S_{AoS} = 50 \times 40B = 2000B \approx 16 \text{ 条 cache line}
$$

$$
S_{SoA} = 50 \times 8B = 400B \approx 4 \text{ 条 cache line}
$$

Case 1相对Case 0提升x1.60；value区（32MB）整体搬离探测路径。但查看结构体发现探测仍需逐槽读取8B的key，"判断这个槽位是否匹配"这件事的读取宽度还是8B。由于查找定位起始槽本来就要计算目标key的哈希值，哈希值又是key的确定性函数，因此我们可以为每个槽预存其key哈希值的高8位作为1B的digest——digest不等则key必不等，先用1B比对排除绝大多数槽位，digest命中才读8B key精确确认。

---

### Case 2：digest预过滤与word批量比较

**优化目标**：若能把匹配判断的读取宽度压到1B、只在匹配时才读key确认，探测跨度的字节数还能继续降低，所以我们在key区之前增加1B/槽的digest预过滤区（哈希高8位），先用1B digest判断槽位是否可能匹配，digest命中才读8B key精确确认；同时以4B word为单位批量加载digest，四路字节并行比较，保证主循环访存的流水连续性。

**核心实现**：digest取key哈希值的高8位；空槽的digest为hash(EMPTY_KEY)的高8位。每轮按4B对齐加载一个word（4个连续digest），两次vcmp_eq4分别与目标digest、空槽digest比较；命中位图由`__ffs`迭代处理，命中槽位读8B key精确确认。



**图3　cacheline示意图**（再画一张图，图里面将key读取出来，然后内存中每个key他们的哈希值都占有8B，然后我们可以只提取1B可以删去大部分的访存）将此图删去，然后将SWAR删去

![Cacheline_digest.drawio](figures/Cacheline_digest.drawio.png)





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

digest预过滤围绕"读取宽度从8B压到1B、疑似命中才回读key"展开，处理流程分为四步：

1. **预存与复用**：建表时对每个槽的key计算Murmur3哈希，取哈希值高8位作为1B digest随槽位写入digest区（共1MB）；空槽digest同样取hash(EMPTY_KEY)的高8位，是有意义的值，可用来判空槽。查找定位起始槽本来就要计算目标key的哈希，因此查找侧零额外计算——哈希是key的确定性函数，digest不等则key必不等（预过滤无假阴性），digest相等仅有1/256的概率是巧合（假阳性），由后续读key兜底。
2. **word打包与批量比较**：目标digest经`DigestWordOf`复制4份拼满32bit word，与word中4个连续槽的digest逐字节对齐；每轮探测从按4对齐的起始槽位一次4B load读入4个digest，先与目标word、再与空槽word各做一次`VcmpEq4`——SWAR位并行比较，约7条ALU指令在寄存器内一次比完4对字节，每个相等字节产出0x01位图。一次加载、两次比较覆盖4个槽，主循环访存保持4B粒度连续。
3. **位图消费与key确认**：`__ffs`定位命中位图中最低的置位字节，(bit-1)>>3换算为word内槽位偏移，读该槽8B key精确确认：与目标key相等即命中返回；不等则说明是digest假阳性，清位（`cmp &= cmp - 1`）继续消费下一个命中位。
4. **早退与回绕保护**：空槽digest命中同样读key确认，确认为空槽即miss早退；唯一例外在第0轮——起始槽按4对齐会把位于start之前的槽也带进word，这些槽逻辑上是全表回绕后才探到的，第0轮的空槽不能作为miss证据，需跳过。循环上界为capacity + stride，保证绕回重探首个word时该限制已解除、早退条件可以成立。



**性能数据**：

| Case | Task Duration(μs) | read_main_memory_data(MB) | 对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 1    |    1158.006    |          249.740           |  -   |
| 2    |    829.345     |          295.490           | x1.40 |

**分析**：

引入digest后，一次查找的主访存从"约50次8B key读取"变为"约13次4B word加载"：

$$
S_{digest} = 13 \times 4B \approx 52B < 1 \text{ 条 cache line}
$$

整个digest区仅1MB，一次探测的跨度稳落单条cache line内，Case 2相对Case 1性能提升x1.40。**此时新瓶颈**（修改一下，前面的新xx给删去，后面一句话引入下一个case即可）转移到每轮load的32个lane各服务各的key，4B地址散落在互不相干的cache line上，无法做到访存合并，warp只能等32个散落请求中最慢的一个，每笔还各算一次访存事务。Case 3的协作探测恰好弥补这处缺陷：4个lane组队探测同一条key、各取窗口内连续4槽，加载地址连成16B、整段落入同一条cache line，同line请求合并为一次访存事务——每warp-轮的地址形态由32个孤立4B变为8段连续16B，一轮只需等8段中最慢的一段。若4个lane的地址不连续，每轮4笔事务、每key约16笔，反超thread-per-key的13笔；有了合并，每key的串行轮数从约13压到约4的同时，访存事务数同比例压缩而非随lane数膨胀。

---

### Case 3：协作探测

**优化目标**：把每key的串行依赖链从约13轮压到约4轮。方法是warp协作与SWAR的两级组合：4个lane（后面都改成线程）组成协作组共同探测一条key，每个lane负责窗口内连续4槽（一次4B word加载 + 两次vcmp_eq4），组窗口每轮覆盖16槽。

**核心实现**：每个warp的32个lane分为8个协作组（每组4 lane），每组处理一条查询key，warp内同时推进8条查询。组内lane覆盖关系为`pos = (first_pos + round×16 + rank×4) & (CAPACITY-1)`，窗口按16对齐；word内命中位图由`__ffs`迭代消费，跨lane结果由`asc_ballot`投票汇总（每轮2票，确认已折进谓词）。round-0回绕保护细化到字节粒度：按`shift = min(4, start - pos)`清掉位于起始槽位之前的字节位。轮循环按warp一致组织：所有组同步推进轮次，已完成的组以谓词0惰性参与投票，`asc_all`判定全warp完成后进入下一批查询；外层以warp为单位grid-stride覆盖全部1M条查询。（尽量不要 用代码）

**图4　协作组并行访问key示意图**（右边修改为KEY[0] 左边的改为KEY0）

![协作组查找.drawio](figures/协作组查找.drawio.png)

图中上下两栏对比两种探测组织方式。上栏thread-per-key形态：T0、T1、T2各自独立服务一条查询（Q0、Q1、Q2），在key区中顺序扫描、逐槽比较KEY0、KEY1……直至命中或空槽。下栏协作组形态：T0~T3四个lane共同服务同一条查询Q0，各自并行访问key区中不同的key，比较结果汇总判定一次探测结论——同样一轮等待内覆盖的槽位数扩大4倍，访存事务数同比例压缩。

**关键代码**：

```cpp
// 每 lane 一次 4B word load + 两次 SWAR 比较（每 warp-轮 32 次 word load = 128 槽）
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

| Case | Task Duration(μs) | read_main_memory_data(MB) | 对比 |
| ---- | :---------------: | :------------------------: | :--: |
| 2    |    829.345032     |          295.490           |  -   |
| 3    |    596.943970     |          284.774           | x1.39 |

**分析**：

Case 3的关键在于保持与Case 2完全相同的访存指令量的同时减少访存事务：每warp-轮同样是32次word加载、同样覆盖128个槽位，但分摊方式不同——Case 2把128槽摊给32条查询（每key每轮4槽、串行约13轮），Case 3把128槽集中给8条查询（每key每轮16槽、串行约4轮）。而且每warp-轮32次word加载的合并条件也截然不同：thread-per-key下32条查询的起始槽互不相关，32个4B word散落在digest区各处，最多凑成32次独立访存事务；协作组内G=4个lane的word地址连续（rank×4紧挨着排），每组的4次加载落在同一段16B内，可合并为一次访存事务——同一轮等待买到的不再只是更宽的探测窗口，还有更省的事务开销。**Case 3以x1.39反超Case 2成为本样例耗时最低的实现。**直接描述

窗口粒度上，16槽接近但不超过探测跨度（约50槽）：更窄的窗口（4槽）轮数过多，更宽的窗口则浪费加载带宽——批量粒度并非越大越好，窗口接近但不超过探测跨度时收益最大。

## 性能对比总结

| Case | 核函数 | Task Duration(μs) | read_main_memory_data(MB) | 相对Case 0 |
| ---- | ------ | :---------------: | :------------------------: | :--------: |
| 0    | `lookup_aos_kernel` | 1850.869995 | 679.142 | 1.00x |
| 1    | `lookup_soa_kernel` | 1158.005981 | 249.740 | 1.60x |
| 2    | `lookup_soa_digest_swar_kernel` | 829.345032 | 295.490 | 2.23x |
| 3    | `lookup_coop_swar_kernel` | 596.943970 | 284.774 | 3.10x |

**综合优化效果**：

- 主路径Case 0 → Case 1 → Case 2 → Case 3，Task Duration由1850.869995μs依次降至1158.005981μs、829.345032μs、596.943970μs，相对Case 0基线累计提升约3.10倍；其中Case 0→1（SoA纯布局）x1.60、Case 1→2（digest预过滤与word批量比较）x1.40、Case 2→3（协作探测 × SWAR）x1.39。
- 三步优化作用于两个正交维度：Case 0→1→2压缩探测跨度（2000B → 400B → 52B），Case 3压缩每key串行轮数（约13轮 → 约4轮）。

## 调优建议

![image-20261016155256161](figures/image-20261016155256161.png)

（什么场景下适合什么样的东西，然后分点描述，每个点一两句话即可）

哈希表类workload调优时，建议先审视探测路径上真正需要的字段。像value这类"无效载荷"字段内嵌槽内会撑大探测步长，让一次查找逐行踩过整张表；把它们分区到独立区域，miss路径减少无效载荷的搬运，即可获得可观收益——本样例中仅此一步就提升x1.60。判断布局收益时可按"探测次数 × 每次探测访存跨度"估算，探测次数只由负载因子决定，布局改变的是后一项。

需要预过滤字段时，应从哈希值的特性出发设计，核心是以空间换时间：查找定位起始槽本来就要计算目标key的哈希，而哈希是key的确定性函数——digest不等则key必不等（预过滤无假阴性）、取值分布均匀（假阳性仅1/256），且可在建表时随槽位一次性生成。于是每槽多存1B digest（本样例1MB，约+2.5%表容量），把"判断槽位是否匹配"的读取宽度从8B压到1B，查找侧零额外计算——目标digest就是已算出的哈希的高8位；一次miss探测的跨度由400B压到约52B，稳落1条cache line。粒度则按"不浪费寄存器空间"与批量访问方式配套选择：1B digest单独进寄存器只占32bit位宽的1/4，而4个连续槽的digest打包成一个4B word恰好占满整个32bit寄存器、无一位浪费（目标digest同样复制4份拼满整字）。打包后以SWAR（SIMD within a Register，寄存器内SIMD）提高并行度：一次4B word加载覆盖4槽，一条vcmp_eq4即在寄存器内同时判定4对字节（目标digest与空槽digest各比一次），load与比较各压到标量形态的1/4。

批量策略可以两级组合使用：lane内SWAR压指令数（1条load覆盖4槽），lane间协作压串行轮数。让协作组并行探测同一个查询key，首先是重塑了访存形态——thread-per-key下一轮load指令的32个lane地址是32个散落点，各lane服务各的key、各拉各的cache line，无从合并；协作组内4个lane各负责窗口内连续4槽，加载地址天然连成16B、整段落入同一条cache line，落在同一条line内的**请求即可合并为一次访存事务**，每warp-轮的地址形态由32个孤立4B变为8段连续16B。收益体现在两处：每key串行轮数从约13轮压到约4轮，轮间访存延迟被组内并行掩盖；每key的访存请求次数随轮数同比例压缩（而非随lane数放大到约16次）——注意每key实际读取的digest字节不变（仍约50B、1条line内），合并省下的是请求次数与轮间延迟，不是数据量。两级组合后反超纯SWAR（x1.39）；窗口粒度接近但不超过探测跨度（由负载因子决定，LF 0.9下约50槽）时收益最大。

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
  Queries: 1048576 all-miss keys, expected probes ~50 (Knuth: (1+1/(1-a)^2)/2, a=0.9)
  Soc: Ascend950PR_9589  device: 0
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
