# 使用SoA布局提升哈希表查找效率

【优先级】中

【描述】SIMT编程模式下，对哈希表等热路径仅访问结构体部分字段的负载，使用SoA（Structure of Arrays）布局替代AoS（Array of Structures）布局以提升查找效率。以开放寻址哈希表的miss查找为例，AoS把value内嵌在槽内，槽步长被撑大到40B，而AI处理器以128字节的Cache Line访问L2 Cache，一条line仅容纳3.2个槽：负载因子0.9下一次miss查找约50次探测，踩过的字节跨度约2000B（约16条Cache Line），其中真正需要的只有每槽8B的key。通过将key与value字段分区存储（SoA），探测路径压缩到紧凑的key区内，跨度降至约400B（约3~4条Cache Line），value区与探测路径完全解耦，主存读数据量减少约72%。

【样例介绍】以[soa_lookup_slim样例](./README.md)为例，负载为平铺开放寻址哈希表的all-miss查找：表容量$2^{20}$个槽，负载因子0.9，装入943718个互不重复的key，查询集为1M条从未装入表的miss key。算子内配置2048个线程块、每个线程块内512线程，thread-per-key模型下每个线程独立处理1条查询。相同配置下，将AoS布局改为SoA布局，哈希函数、起始槽位与线性探测序完全不变，唯一差异是物理布局。

实测环境为Ascend950PR_9589，CANN 9.2.0。性能数据由msOpProf工具采集，Task Duration取自`OpBasicInfo.csv`；aiv_time、aiv_vec_time等为2048个线程块的平均值，aiv_total_cycles为全部线程块周期数合计，aiv_read_hit_rate/aiv_write_hit_rate按访问量加权。

  【反例】使用AoS布局逐槽探测，value内嵌槽内撑大探测步长。

  **图1** AoS与SoA内存布局示意图

  ![img](./figures/图2SoA,AoS内存布局示意图.drawio.png "AoS与SoA内存布局示意图")

  图中左右两栏对比同一份槽位数据的两种物理布局。左栏AoS：每个槽内KEY（8B）与VALUE[DIM]（32B）交错排列，槽步长被撑大到40B——仅读完3个连续槽的key，探测就要跨越约120B；右栏SoA：KEY与VALUE[DIM]各自成区，key区中3个连续key仅占24B，value区独立成区、与探测路径完全解耦。miss查找全程不需要value，布局差异直接决定探测路径上要踩过的字节数。

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

  在上述实现中，value作为无效载荷占据探测路径上的每个槽位，128B的Cache Line仅容纳3.2个槽。按负载因子0.9下期望约50次探测计算，一次miss查找的访存跨度为：

  $$S_{AoS} = 50 \times 40B = 2000B \approx 16 \text{ 条 Cache Line}$$

  探测逐行踩过整张表，布局本身成为主要开销，而查找真正需要的只有每槽8B的key。

  在Ascend 950PR产品上，该实现的性能数据如下：

  | Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_read_hit_rate | aiv_write_hit_rate | aiv_main_mem_read_bw(GB/s) | aiv_main_mem_write_bw(GB/s) |
  |:-----------------:|:------------:|:----------------:|:----------------:|:-------------:|:-------------------:|:----------------:|:-----------------:|:------------------:|:--------------------------:|:---------------------------:|
  |    3129.454102    |    92.10     |    311214576     |      91.96       |    0.9985     |        0.132        |      0.0015      |       0.974        |        0.935        |            11.46            |            0.073             |

  从性能数据可以看出，1M条miss查询共从主存读入约2.05 GB数据——平均每条查询读约2KB，而查询本身仅8B，读放大全部来自被value撑大的探测跨度。

  【正例】使用SoA布局，key与value分区存储，探测路径与value区解耦。

  如图1右栏所示，SoA将key与value两个字段各自成区：key区（8MB）连续紧凑，value区（32MB）独立成区，探测只在8B步长的key区内滑动。

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

  上述实现与反例的算法完全一致——逐槽读8B key比较、空槽早退、全表回绕；哈希函数与逻辑槽位也完全相同，唯一差异是物理布局。key区连续紧凑，一次miss查找的访存跨度压缩为：

  $$S_{SoA} = 50 \times 8B = 400B \approx 3\text{~}4 \text{ 条 Cache Line}$$

  value区整体搬离探测路径，miss查找一次也不会触碰。

  在Ascend 950PR产品上，使用SoA布局后的性能数据如下：

  | Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_read_hit_rate | aiv_write_hit_rate | aiv_main_mem_read_bw(GB/s) | aiv_main_mem_write_bw(GB/s) |
  |:-----------------:|:------------:|:----------------:|:----------------:|:-------------:|:-------------------:|:----------------:|:-----------------:|:------------------:|:--------------------------:|:---------------------------:|
  |    2036.866089    |    59.85     |    202258780     |      59.72       |    0.9976     |        0.131        |      0.0023      |       0.972        |        0.936        |            5.05             |            0.113             |

  从性能数据可以看出，主存读数据量降至约0.58 GB，平均每条查询读约598B，与探测跨度压缩到约1/5的方向一致；value区全程零触碰。

**优化效果分析**：

- **端到端耗时**：从3129.454102 μs降至2036.866089 μs，减少34.9%，整体性能提升1.54倍。
- **AI Vector总周期数**：从311,214,576减至202,258,780，降幅达35.0%，热循环中的访存等待被显著压缩。
- **主存读数据量**：从约2.05 GB降至约0.58 GB，减少约72%；读带宽均值相应从11.46 GB/s回落至5.05 GB/s——这是不再搬运冗余数据的结果而非带宽退化，L2读命中率保持在97%以上（0.974→0.972）佐证收益来自布局本身。

**适用场景扩展**

SoA布局不局限于哈希表查找：凡热路径只访问结构体部分字段的负载，均可考虑字段分区——如分组聚合只需比对分组key、图遍历热循环只读邻居id、稀疏计算只扫索引列。此外，本样例的后续优化均构建在SoA的紧凑key区之上：Case 2在key区之前引入1B digest预过滤并以SWAR批量比较，Case 3叠加warp协作探测，详见[样例README](./README.md)。

【总结】在SIMT算子中，通过将哈希表槽内的key与value分区存储（SoA），探测跨度从2000B压缩到400B，主存读数据量减少约72%，查找性能提升1.54倍。判断布局收益时可按"探测次数×每次探测访存跨度"估算——探测次数只由负载因子决定，布局改变的是后一项。
