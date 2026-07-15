# DataCachePreload 间接索引预加载样例

## 概述
本样例演示[DataCachePreload](../../../../../docs/api/SIMD-API/基础API/缓存控制/DataCachePreload.md)在间接索引（随机跳转）场景下的使用方法：通过Scalar单元读取GM中分散的数据时，利用DataCachePreload提前将即将访问的Cache Line加载到DCache，减少Scalar访存等待时间，提升性能。

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── data_cache_preload
│   ├── scripts
│   │   ├── gen_data.py             // 输入数据和真值数据生成脚本
│   │   └── verify_result.py        // 验证输出数据和真值数据是否一致的验证脚本
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── data_utils.h                // 数据读入写出函数
│   ├── data_cache_preload.asc      // Ascend C样例实现 & 调用样例
│   ├── test.sh                     // 一键编译执行脚本
│   └── README.md                   // 样例说明文档
```

## 样例描述

本样例通过SCENARIO_NUM控制执行分支，该变量的不同取值对应是否使用DataCachePreload。

| SCENARIO_NUM取值 | 业务场景 | 说明 |
|-----------------|---------|------|
| 1 | 间接索引累加（无preload） | 直接Scalar读取，不使用DataCachePreload |
| 2 | 间接索引累加（有preload） | 使用DataCachePreload提前预取下一轮数据到DCache |

### 计算公式与样例规格

两个场景的计算公式相同，均为间接索引累加求和：

$$
result = \sum_{i=0}^{255} data\_gm[indices[i]]
$$

- data_gm为64KB随机uint8_t数据缓冲区
- indices为256个随机偏移量，每个指向不同Cache Line对齐位置
- 由于indices随机分散在64KB范围内，绝大多数Scalar读都会触发DCache miss，导致大量访存延迟
- 通过DataCachePreload在读取当前数据时，异步预取下一轮数据所在的Cache Line到DCache，使Scalar读与GM→DCache搬移形成流水线

#### SCENARIO_NUM=1（间接索引累加，无DataCachePreload）
- 样例规格：
  <table border="1" align="center">
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">DataCachePreloadNoPreload</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">size</td><td align="center">data type</td><td align="center">描述</td></tr>
  <tr><td align="center">data_gm</td><td align="center">[65536]</td><td align="center">uint8_t</td><td align="center">64KB随机数据缓冲区（1024个Cache Line）</td></tr>
  <tr><td align="center"></td><td align="center">indices</td><td align="center">[256]</td><td align="center">uint32_t</td><td align="center">随机索引，指向Cache Line对齐位置</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">result</td><td align="center">[1]</td><td align="center">uint32_t</td><td align="center">累加和</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">kernel_no_preload</td></tr>
  </table>

#### SCENARIO_NUM=2（间接索引累加，有DataCachePreload）
- 样例规格：
  <table border="1" align="center">
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">DataCachePreloadWithPreload</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">size</td><td align="center">data type</td><td align="center">描述</td></tr>
  <tr><td align="center">data_gm</td><td align="center">[65536]</td><td align="center">uint8_t</td><td align="center">64KB随机数据缓冲区（1024个Cache Line）</td></tr>
  <tr><td align="center"></td><td align="center">indices</td><td align="center">[256]</td><td align="center">uint32_t</td><td align="center">随机索引，指向Cache Line对齐位置</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">result</td><td align="center">[1]</td><td align="center">uint32_t</td><td align="center">累加和</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">kernel_with_preload</td></tr>
  </table>

## 样例实现

- DataCachePreload使用方法
  1. 创建`GlobalTensor<uint64_t>`绑定data_gm：
     ```cpp
     AscendC::GlobalTensor<uint64_t> dataGlobal;
     dataGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ uint64_t*>(data_gm), size);
     ```
  2. 计算Cache Line对齐偏移：`cacheOffset = idx & ~63ULL`（对齐到64B边界）
  3. 调用`DataCachePreload(dataGlobal, cacheOffset)`将64B加载到DCache

- 约束说明
  - 每次调用只预加载1个Cache Line（64B）
  - 连续调用建议不超过4次，否则可能导致保留站拥塞
  - preload是异步指令，可与后续计算/访存重叠

## 性能测试数据与分析

通过`msprof`运行后，可以对比两个核函数的执行时间：
- `kernel_no_preload`：DCache miss率高，Scalar读延迟大
- `kernel_with_preload`：通过提前preload下一轮数据，DCache命中率大幅提升

### 性能对比数据

以下性能数据为排除单次执行的随机波动，取5次独立运行的平均值。

#### Atlas A2/A3系列产品（dav-2201架构）

| 指标 | kernel_no_preload | kernel_with_preload | 优化效果 |
|------|------------------|---------------------|---------|
| **Task Duration (us)** | 44.000 | 26.760 | **加速1.64x** |
| **aiv_scalar_time (us)** | 41.942 | 24.622 | **降低41.3%** |
| **aiv_scalar_ratio** | 96.5% | 93.9% | Scalar仍为主要瓶颈 |

#### Ascend 950系列产品（dav-3510架构）

| 指标 | kernel_no_preload | kernel_with_preload | 优化效果 |
|------|------------------|---------------------|---------|
| **Task Duration (us)** | 72.897 | 42.051 | **加速1.73x** |
| **aiv_scalar_time (us)** | 70.230 | 39.242 | **降低44.1%** |
| **aiv_scalar_ratio** | 97.7% | 95.7% | Scalar仍为主要瓶颈 |

> **指标说明：**
> - **Task Duration**：核函数端到端执行耗时，是最直观的性能指标
> - **aiv_scalar_time**：Scalar单元耗时，间接索引场景下Scalar访存是主要瓶颈
> - **aiv_scalar_ratio**：Scalar耗时占总时间的比例，用于定位性能瓶颈所在

#### 5次独立运行详细数据（Atlas A2/A3系列产品）

| 执行次数 | no_preload Duration(us) | with_preload Duration(us) | 加速比 |
|---------|------------------------|--------------------------|--------|
| 第1次 | 43.780 | 26.900 | 1.63x |
| 第2次 | 43.400 | 27.200 | 1.60x |
| 第3次 | 43.820 | 27.140 | 1.61x |
| 第4次 | 45.160 | 26.300 | 1.72x |
| 第5次 | 43.840 | 26.260 | 1.67x |
| **平均** | **44.000** | **26.760** | **1.64x** |

#### 5次独立运行详细数据（Ascend 950系列产品）

| 执行次数 | no_preload Duration(us) | with_preload Duration(us) | 加速比 |
|---------|------------------------|--------------------------|--------|
| 第1次 | 72.773 | 42.396 | 1.72x |
| 第2次 | 73.002 | 41.780 | 1.75x |
| 第3次 | 73.046 | 42.002 | 1.74x |
| 第4次 | 73.130 | 42.032 | 1.74x |
| 第5次 | 72.535 | 42.043 | 1.73x |
| **平均** | **72.897** | **42.051** | **1.73x** |

### 分析结论

**DataCachePreload在Atlas A2/A3系列和Ascend 950系列产品上均有显著优化效果**，`kernel_with_preload`的Task Duration、aiv_scalar_time均明显低于`kernel_no_preload`。在间接索引（随机跳转）场景下，通过DataCachePreload将下一轮数据提前加载到DCache，**可有效隐藏Scalar访存延迟，实现约1.64倍（Atlas A2/A3）和约1.73倍（Ascend 950）的性能加速**。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，配置环境变量。
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至`/usr/local/Ascend`下。

- 样例执行
  本样例通过`SCENARIO_NUM`参数分别运行两个核函数，便于独立采集性能数据：
  - `SCENARIO_NUM=1`：运行`kernel_no_preload`（无DataCachePreload）
  - `SCENARIO_NUM=2`：运行`kernel_with_preload`（有DataCachePreload）

  在本样例目录下执行如下命令。
  ```bash
  SCENARIO_NUM=1;                                                           # 设置场景编号（1=无preload，2=有preload）
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # 编译工程（默认npu模式）
  python3 ../scripts/gen_data.py                                            # 生成测试输入数据
  msprof ./demo $SCENARIO_NUM
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # 验证输出结果是否正确，确认算法逻辑正确
  ```

  使用CPU调试或NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=cpu`或`-DCMAKE_ASC_RUN_MODE=sim`参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # cpu调试模式
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理cmake缓存，可在build目录下执行`rm CMakeCache.txt`后重新cmake。

- 编译选项说明

  | 选项 | 可选值 | 说明 |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`（默认）、`cpu`、`sim` | 运行模式：NPU运行、CPU调试、NPU仿真 |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`（默认）、`dav-3510` | NPU架构：dav-2201对应Atlas A2/A3系列产品，dav-3510对应Ascend 950系列产品 |
  | `SCENARIO_NUM` | `1`（默认）、`2` | 场景编号：1（无preload）、2（有preload） |

- 执行结果
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```
