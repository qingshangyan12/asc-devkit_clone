# 使用SoA布局提升哈希表查找效率

【优先级】中

【描述】SIMT编程模式下，对于哈希表等热路径仅访问结构体部分字段的负载场景下，使用SoA（Structure of Arrays）布局替代AoS（Array of Structures）布局可以提升查找效率。以平铺开放寻址哈希表的查找为例，AoS把value内嵌在槽内，槽步长被撑大到40B，而AI处理器以128字节的Cache Line访问L2 Cache，一条line仅容纳3.2个槽：负载因子0.9下一次miss查找期望约50次探测，踩过的字节跨度约2000B（约16条Cache Line），其中真正需要的只有每槽8B的key。通过将key与value字段分区存储（SoA），探测路径压缩到紧凑的key区内，跨度降至约400B（约3~4条Cache Line），value区与探测路径完全解耦，主存读数据量减少约63%。

【样例介绍】以[hashtable_lookup样例](../../../../../../examples/03_simt_api/03_best_practices/00_memory_optimizations/hashtable_lookup)为例，在一张容量$2^{20}$个槽、负载因子0.9的平铺开放寻址哈希表上做查找：表内预先装入943718个互不重复的key，共发起1M条混合查询，每条查询从哈希计算出的起始槽位开始逐槽线性探测，key匹配则命中，遇空槽则判定Miss；同一张表、同一批查询，分别按AoS和SoA两种物理布局存放并查找，哈希函数、起始槽位与线性探测序完全不变，唯一差异是物理布局。查找全程只读每槽8B的key，AoS布局却把32B的value内嵌在槽内，槽步长被撑大到40B，探测会将用不到的value字段一起搬运；优化手段则是把key与value各自分区存储（SoA），让探测只在紧凑的key区内滑动，value区与探测路径完全解耦。

实测环境为Ascend950PR_9589，CANN 9.2.0。性能数据使用msOpProf工具对可执行文件单独采集得到：Task Duration(μs)为Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间，记录于输出目录`OpBasicInfo.csv`；read_main_memory_data(MB)为任务执行期间从主存储器读取的总数据量，为`Memory.csv`中各block的read_main_memory_datas(KB)求和。

  【反例】使用AoS布局逐槽探测，value内嵌槽内撑大探测步长。

  **图1** AoS与SoA内存布局示意图

<img src="../../../figures/内存布局示意图.drawio.png" width="70%">

图中左右两栏对比同一份槽位数据的两种物理布局。左栏AoS：每个槽内KEY（8B）与VALUE[DIM]（32B）交错排列，槽步长被撑大到40B——仅读完3个连续槽的key，探测就要跨越约120B；右栏SoA：KEY与VALUE[DIM]各自成区，key区中3个连续key仅占24B，value区独立成区、与探测路径完全解耦。无论hit还是miss，查找全程只读key、一次也不需要value，布局差异直接决定探测路径上所需要搬运的字节数。

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

在上述实现中，value作为无效载荷占据探测路径上的每个槽位，128B的Cache Line仅容纳3.2个槽。按负载因子0.9下miss查找期望约50次探测计算，一次miss查找的访存跨度为：

$$S_{AoS} = 50 \times 40B = 2000B \approx 16 \text{ 条 Cache Line}$$

探测逐行踩过整张表，布局本身成为主要开销，而查找真正需要的只有每槽8B的key。

在Ascend 950PR产品上，该实现（Case 0）的性能数据如下：

  | Case | Task Duration(μs) | read_main_memory_data(MB) |
  | ---- | :---------------: | :------------------------: |
  | 0    |     1850.870      |          679.142           |


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

value区整体搬离探测路径，查找一次也不会触碰value区；hit查找同步受益，期望约5.5次探测的跨度从220B（约2条Cache Line）压缩到44B（不足1条Cache Line）。

在Ascend 950PR产品上，使用SoA布局（Case 1）后的性能数据如下：

  | Case | Task Duration(μs) | read_main_memory_data(MB) | 性能对比 |
  | ---- | :---------------: | :------------------------: | :--: |
  | 0    |     1850.870      |          679.142           |  -   |
  | 1    |     1158.006      |          249.740           | x1.60 |

从性能数据可以看出，同一算法、同一测试集仅改变物理布局，Task Duration从1850.870μs降至1158.006μs，相对Case 0提升x1.60；主存读数据量降至249.740MB，约为AoS的37%。

**优化效果分析**：

- **端到端耗时**：Task Duration从1850.870μs降至1158.006μs，减少37.4%，整体性能提升1.60倍。
- **主存读数据量**：从679.142MB降至249.740MB，减少约63%——这是不再搬运冗余value字节的结果；一次miss查找的探测跨度从约2000B（约16条Cache Line）压缩到约400B（约3~4条Cache Line），hit查找的跨度同步从约220B压缩到约44B。
- **收益归因**：同一算法、同一测试集仅改变物理布局，哈希函数、起始槽位与线性探测序完全不变，，收益全部来自布局本身——key独立成8B步长的紧凑key区，value区（32MB）整体搬离探测路径。

**适用场景扩展**

SoA布局不局限于哈希表查找：对于热路径只访问结构体部分字段的工作负载，均可考虑字段分区——如分组聚合只需比对分组key、图遍历热循环只读邻居id、稀疏计算只扫索引列。此外，本样例的后续优化均构建在SoA的紧凑key区之上：Case 2在key区之前引入1B digest预过滤，Case 3叠加warp协作探测，详见[样例README](../../../../../../examples/03_simt_api/03_best_practices/00_memory_optimizations/hashtable_lookup/README.md)。

【总结】在SIMT算子中，通过将哈希表槽内的key与value分区存储（SoA），miss查找的探测跨度从2000B压缩到400B，主存读数据量减少约63%，混合查找性能提升1.60倍。
