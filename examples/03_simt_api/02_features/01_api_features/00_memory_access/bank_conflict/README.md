# SIMT UB Subbank冲突样例

## 概述

本样例介绍Ascend C SIMT编程方式下同一个Warp内多个线程访问UB时的subbank冲突现象。样例通过6个case构造不同的UB读地址模式，展示同地址合并、同一8B地址范围合并、同subbank不同行冲突、跨bank/subbank分散访问，以及同一bank group内同编号subbank冲突等典型场景。

本样例中Case 3为跨bank/subbank分散访问基线。Case 1与Case 2构成同subbank内“同一8B范围合并”和“不同行冲突”的对照，Case 4和Case 5可与Case 3对比观察冲突访问模式带来的cycles差异。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \>= CANN 9.1.0

## 目录结构介绍

```text
├── bank_conflict
│   ├── CMakeLists.txt       // cmake编译文件
│   ├── bank_conflict.asc    // SIMT UB subbank冲突样例实现
│   └── README.md
```

## UB Bank与Subbank规则

以Ascend 950PR/Ascend 950DT为例，UB空间大小为256KB，划分为16个bank。每个bank包含512行，每行32B，共16KB。16个bank进一步组织为8个bank group，每个bank group包含2个bank。

在SIMT编程方式下，每个bank的32B行宽又划分为4个subbank，每个subbank宽度为8B。同一个Warp内的多个线程可能在同一条UB访问指令中同时访问UB。若这些访问落在同一个bank group的相同编号subbank资源上，硬件无法在一个周期内处理全部请求，需要排队等待，从而形成subbank冲突。

需要注意的是，如果多个线程访问同一个subbank中同一行内的同一个8B地址范围，硬件可以将这些请求合并处理，不形成subbank冲突。

UB地址采用低位交织方式。对float类型而言，一个元素占4B：

- 相邻2个float位于同一个8B subbank范围内。
- 相邻8个float位于同一个32B bank行内。
- 32个float共128B，覆盖4个bank group。
- 64个float共256B，会切换bank group内的bank编号，但bank group和subbank编号保持一致。
- 128个float共512B，会回到相同bank、相同bank group和相同subbank的下一行。

## 样例描述

本样例只启动1个Thread Block，block内包含32个线程，即只使用1个Warp。所有case先初始化同一块UB数组，再让同一个Warp内32个线程按照不同地址模式重复读取UB数据，并使用`clock()`统计循环内访问开销。

### Case实现说明

| Case | 场景描述 | UB索引模式 | 预期访问特征 |
| --- | --- | --- | --- |
| Case 0 | 同一Warp的线程读同一个地址 | `ub[0]` | 同地址请求合并 |
| Case 1 | 同一Warp的线程读同一8B范围内的地址 | `ub[lane_id & 1]` | 同subbank同一行内8B范围合并 |
| Case 2 | 同一bank的同subbank不同行，两地址访问 | `ub[(lane_id & 1) * 128]` | 与Case 1同为两个目标地址，但落到同subbank不同行，产生冲突 |
| Case 3 | 跨bank/subbank分散访问 | `ub[lane_id * 2]` | 32个线程分散到8个bank group的不同subbank，作为无冲突基线 |
| Case 4 | 同一bank的同subbank不同行，32地址访问 | `ub[lane_id * 128]` | 512B stride使访问落到同bank、同subbank的不同depth，产生冲突 |
| Case 5 | 同bank group的同编号subbank访问 | `ub[lane_id * 64]` | 256B stride切换bank编号，但保持同bank group和同subbank编号，产生冲突 |

#### 线程块布局

本样例固定采用1个Thread Block，每个Thread Block包含32个线程：

- `threadIdx.x`范围为0-31，对应同一个Warp内的32个lane。
- 每个线程根据自己的`lane_id`计算UB读索引。
- 所有线程执行相同次数的循环，使case之间只体现UB地址模式差异。

```cpp
constexpr uint32_t THREAD_COUNT = 32;
uint32_t lane_id = laneid();
uint32_t ub_index = get_ub_index(lane_id);
```

#### 访问打点

样例使用`clock()`记录重复UB读取前后的时间戳，仅由`threadIdx.x == 0`输出cycles信息，避免多个线程重复打印。

```cpp
uint64_t start = clock();
for (uint32_t i = 0; i < REPEAT_TIMES; ++i) {
    acc += ub_ptr[ub_index];
}
uint64_t end = clock();
```

为避免重复读取被优化掉，样例将读取值累加到`acc`并写回GM，Host侧对每个lane的输出结果做校验。

### Case 0：同一Warp的线程读同一个地址

**样例目标**：展示同一个Warp内多个线程读取完全相同UB地址时，硬件可以合并同地址请求。

**核心实现**：

```cpp
return 0;
```

32个lane都读取`ub[0]`。这些访问的起始地址完全相同，属于同地址读场景，硬件可合并处理，不产生subbank冲突。

### Case 1：同一8B范围内的地址合并读

**样例目标**：展示多个线程访问同一个subbank同一行内的8B地址范围时，不形成subbank冲突。

**核心实现**：

```cpp
return lane_id & 1;
```

对float类型而言，`ub[0]`和`ub[1]`分别位于同一个8B subbank范围内的前4B和后4B。该case中32个lane交替读取`ub[0]`和`ub[1]`，访问集中在同一个subbank的同一行内8B范围，硬件可合并处理。

### Case 2：同一bank的同subbank不同行，两地址冲突

**样例目标**：与Case 1构成对照，验证同subbank访问只有在同一行内8B范围可合并；当两个目标地址位于同subbank不同行时会产生冲突。

**核心实现**：

```cpp
return (lane_id & 1) * 128;
```

该case中32个lane仍然只访问两个目标地址，但目标地址从Case 1中的`ub[0]`和`ub[1]`变为`ub[0]`和`ub[128]`。`ub[0]`与`ub[128]`相隔512B，落在同一个bank、同一个subbank的不同depth行。同样是两个目标地址，Case 1满足同一8B范围合并条件，Case 2不满足，因此Case 2用于验证同subbank不同行访问会产生冲突。

### Case 3：跨bank/subbank分散访问

**样例目标**：构造无subbank冲突的UB访问基线。

**核心实现**：

```cpp
return lane_id * 2;
```

`lane_id * 2`表示相邻线程之间相隔2个float，即8B。这样32个lane的访问覆盖连续256B空间，并按照8B粒度分散到8个bank group的4个subbank上。同一条UB访问指令下，每个bank group的每个subbank只被一个lane访问，因此该case可作为无冲突访问基线。

### Case 4：同一bank的同subbank不同行冲突

**样例目标**：与Case 3构成对照，构造32个lane分别访问不同地址，但全部集中到同一bank、同一subbank的不同行。

**核心实现**：

```cpp
return lane_id * 128;
```

128个float等于512B。由于UB低位交织规则，512B stride会使地址回到相同bank group、相同bank和相同subbank，但depth行号递增。因此，同一个Warp内32个lane会同时访问同一bank的同编号subbank的不同行，形成subbank冲突。

### Case 5：同bank group的同编号subbank冲突

**样例目标**：与Case 3构成对照，构造32个lane分别访问不同地址，但全部集中到同一个bank group的同编号subbank资源。

**核心实现**：

```cpp
return lane_id * 64;
```

64个float等于256B。256B stride会切换bank group内的bank编号，但bank group编号和subbank编号保持一致。因此，同一个Warp内32个lane会集中访问同一个bank group的同编号subbank资源，形成subbank冲突。该case用于说明subbank冲突不仅发生在同一个物理bank内，同一个bank group内不同bank的同编号subbank也需要关注。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=3                       # 选择执行场景，可选0-5
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;  # 编译工程
  ./demo                               # 执行样例
  ```

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  | --- | --- | --- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM` | `0`-`5` | 样例类型，默认为3 |

  执行结果如下，说明精度对比成功。

  ```text
  Usage: ./demo (compile-time SCENARIO_NUM=3)
  Running case 3: cross_bank_subbank_baseline...
  case 3 execute cycle : xxxx
  Case 3 total cycles: xxxx
  [Success] Case accuracy verification passed.
  ```

## 验证方法

本样例使用`clock()`打点验证SIMT场景下的UB subbank冲突。`clock()`输出的是kernel内部重复UB读取循环的cycles，可用于观察不同UB地址模式下的相对差异。建议按以下对照关系分析：

- Case 1与Case 2对照：二者都只访问两个目标地址，Case 1位于同一8B范围内，Case 2位于同subbank不同行。若Case 2 cycles高于Case 1，说明同subbank不同行访问无法合并并产生冲突。
- Case 3与Case 4对照：二者都是32个lane访问32个不同地址，Case 3跨bank/subbank分散，Case 4集中到同一bank的同subbank不同行。若Case 4 cycles高于Case 3，说明同bank同subbank不同行访问产生冲突。
- Case 3与Case 5对照：二者都是32个lane访问32个不同地址，Case 5集中到同bank group的同编号subbank。若Case 5 cycles高于Case 3，说明同bank group同编号subbank资源会产生竞争。

Case 0用于展示完全同地址合并规则。由于完全同地址访问与其他case的有效请求形态不同，Case 0不作为冲突证明的主要对照。

需要注意，`ResourceConflictRatio.csv`当前不统计SIMT场景下的subbank冲突情况，因此本样例不使用该文件作为冲突判断依据。可使用`msprof`观察整体Task Duration等信息，但判断是否存在SIMT subbank冲突时，以`clock()` cycles对比为主。

可选地使用`msprof`工具采集整体性能数据：

```bash
msprof op ./demo
```

命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的文件夹。可查看：

```text
├──OpBasicInfo.csv      # 算子基础信息，可查看Task Duration
├──MemoryUB.csv         # UB读写带宽速率
├──PipeUtilization.csv  # 采集计算单元和搬运单元耗时和占比
```
