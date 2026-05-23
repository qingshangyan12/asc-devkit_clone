# 短向量Add算子调优样例

## 概述

本样例以向量加法为例，演示了在Ascend C SIMT编程中，基于短向量的性能调优方法。通过两个场景的对比（Case
0-1），展示从基础half类型逐元素加法到使用half2类型向量化加法的优化过程。重点对比了原始类型与向量化类型（half2）在数据处理效率上的差异。

**优化路径**：

- Case 0: half类型数据处理（基准）
- Case 1: half2类型向量化优化

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- \> CANN 9.0.0

## 目录结构介绍

```
short_vector_add
│   ├── figures/             // 示意图
│   ├── scripts/             // 测试脚本目录
│   │   ├── gen_data.py      // 生成测试输入和golden数据
│   │   └── verify_result.py // 验证输出结果
│   ├── CMakeLists.txt       // 编译工程文件
│   ├── data_utils.h         // 数据读写工具函数
│   └── short_vector_add.asc // Ascend C算子实现 & 调用样例
```

## 样例描述

- 样例功能：

  样例计算half类型数据，分别使用基础half类型和向量化half2类型实现向量加法运算。通过不同场景的对比，展示向量化计算和线程分配优化对性能的影响。计算公式如下：
  $$z_i = x_i + y_i$$

- 样例规格
  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">SIMT样例</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center" colspan="2">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[64 * 256 * 1024]</td><td align="center" colspan="2">half</td></tr>
  <tr><td align="center">y</td><td align="center">[64 * 256 * 1024]</td><td align="center" colspan="2">half</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">z</td><td align="center">[64 * 256 * 1024]</td><td align="center" colspan="2">half</td></tr>
  </table>

## 样例实现

### 性能指标说明

**表1 AI Core 性能指标字段说明表**

|         字段名         | 字段含义                                            |
|:-------------------:|:------------------------------------------------|
|  Task Duration(μs)  | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间。         |
|  aiv_total_cycles   | Task在 Vector Core上执行所消耗的CPU周期（Cycle）总数。         |
|      aiv_time       | Task在AI Vector Core上的理论执行时间，单位为μs。              |
|  aiv_vec_time(μs)   | vec类型指令（向量类运算指令）耗时，单位μs。                        |
|    aiv_vec_ratio    | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比。      |
| aiv_scalar_time(μs) | scalar类型指令（标量类运算指令）耗时，单位μs。                     |
|  aiv_scalar_ratio   | scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比。   |
|  aiv_mte2_time(μs)  | mte2类型指令（GM->UB搬运类指令）耗时，单位μs。                   |
|   aiv_mte2_ratio    | mte2类型指令（GM->UB搬运类指令）的cycle数在total cycle数中的占用比。 |
|  aiv_mte3_time(μs)  | mte3类型指令（UB->GM搬运类指令）耗时，单位μs。                   |
|   aiv_mte3_ratio    | mte3类型指令（UB->GM搬运类指令）的cycle数在total cycle数中的占用比。 |

本章节性能数据在Ascend 950系列产品上运行得到。

### Case 0: half类型数据处理（基准程序）

**实现方式**：

基准程序实现了`half`类型的两组输入数据的加法。

**关键代码**：

```cpp
template<typename T>
__global__ void add(T* x, T* y, T* z, uint64_t size)
{
    // gridDim.x * blockDim.x 线程总数 (64 * 1024)
    int32_t stride = gridDim.x * blockDim.x;
    // 每个线程从自己的全局idx开始，每次递增总线程数，直到处理完所有元素
    for (int32_t idx = blockIdx.x * blockDim.x + threadIdx.x; idx < size; idx += stride) {
        z[idx] = x[idx] + y[idx];
    }
}
```

**样例配置**：

- blockNum: 64
- threadNumPerBlock: 1024
- 总线程数: 65536
- 每线程处理元素数: 32

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:-----------------:|:------------:|:----------------:|:----------------:|:-------------:|:-------------------:|:----------------:|:-----------------:|:--------------:|:-----------------:|:--------------:|
|       1.811       |    1.549     |      103434      |      0.818       |     0.529     |        0.711        |      0.459       |       0.006       |     0.004      |       0.002       |     0.001      |

<p align="center">
  <img src="figures/memory_bound.png" width="1000">
</p>
**性能数据分析**：

- 端到端耗时：**1.810 us**
- cycle总数：**103,434**
- 性能分析：每个线程处理 2 字节数据，访存指令密度极大，硬件调度器忙于处理密集的标量指令，导致 Scalar Ratio（45.9%）较高。

**性能优化建议**：
> 💡 **引入half2类型**
>
> 对于16位half类型的数据，底层指令支持一次完成两个数据的计算，可以提高数据计算效率。

---

### Case 1: half2类型向量化优化

**实现方式**：

使用`half2`类型的两组输入数据替代原有`half`类型数据，需要处理元素个数位奇数场景的单个`half`数据。

**关键代码**：

```cpp
template<typename T, typename U>
__global__ void add2(T* x, T* y, T* z, uint64_t size)
{
    // 转为向量类型
    const U* x2 = reinterpret_cast<const U*>(x);
    const U* y2 = reinterpret_cast<const U*>(y);
    U* z2 = reinterpret_cast<U*>(z);

    uint64_t vectorSize = size / 2;
    int32_t stride = gridDim.x * blockDim.x;
    // 向量化循环处理偶数对（步长同样是总线程数）
    for (int32_t idx = blockIdx.x * blockDim.x + threadIdx.x; idx < vectorSize; idx += stride) {
        z2[idx] = x2[idx] + y2[idx];
    }
    // 最后一个奇数元素处理（如果size是奇数，由全局第0号线程在最后处理）
    if (blockIdx.x == 0 && threadIdx.x == 0 && (size % 2 != 0)) {
        uint64_t lastIdx = size - 1;
        z[lastIdx] = x[lastIdx] + y[lastIdx];
    }
}
```

**样例配置**：

- blockNum: 64
- threadNumPerBlock: 1024
- 总线程数: 65536
- 每线程处理half2元素数: 16

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:-----------------:|:------------:|:----------------:|:----------------:|:-------------:|:-------------------:|:----------------:|:-----------------:|:--------------:|:-----------------:|:--------------:|
|       1.805       |    1.536     |      102540      |      0.809       |     0.527     |        0.708        |       0.46       |       0.006       |     0.004      |       0.002       |     0.001      |

**优化效果分析**：

- 端到端耗时：微降至**1.804 μs**
- cycle总数：：微降至**102,539**
- 性能分析：逻辑优化了，调度未优化，总任务量由于向量化其实只需要32核就能算完，前32个核使用half2跑快了，但剩下的32个核进进入后无实际操作（执行了
  idx >= size/2）就退出。

**性能优化建议**：
> 💡 **避免空耗**
>
> 由于依然启动了 64 个 Block，硬件调度器必须经历 64 次启动和退出流程。空跑核产生的“管理费”抵消了 half2
> 带来的“效率红利”，下一步执行核数减半。

---

### 优化要点总结

| 优化手段 | 核心原理              | 适用场景                                                                                        |
|:-----|:------------------|:--------------------------------------------------------------------------------------------|
| 短向量  | 底层指令支持一次完成两个数据的计算 | half/bfloat16_t类型元素进行[基本运算](../../../../../../docs/guide/编程指南/语言扩展层/SIMT-BuiltIn关键字.md#运算符) |

---

## 编译运行

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

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
  # 场景2：使用half2类型
  SCENARIO_NUM=2
  mkdir -p build && cd build;                                               # 创建并进入build目录
  cmake .. -DSCENARIO_NUM=$SCENARIO_NUM; make -j;                           # 编译工程
  python3 ../scripts/gen_data.py;                                           # 生成测试输入数据
  ./demo                                                                    # 执行样例
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin;  # 验证输出结果
  ```

  使用NPU仿真模式时，添加`-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  示例如下：
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;   # NPU仿真模式
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 参数                        | 可选值             | 说明                                    |
  |:--------------------------|:----------------|:--------------------------------------|
  | `SCENARIO_NUM`            | `1`（默认）、`2`     | 1: half Add计算；<br/>2: 短向量half2 Add计算； |
  | `CMAKE_ASC_RUN_MODE`      | `npu`（默认）、`sim` | 运行模式：NPU 运行、NPU仿真                     |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`      | `dav-3510`                            |

- 执行结果  
  执行结果如下，说明精度对比成功。
  ```bash
  test pass!
  ```

## 性能数据获取

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./demo   # 分析性能
```

当前目录下会生成PROF_前缀的文件夹，`mindstudio_profiler_output`目录保存Host和各个Device的性能数据汇总，性能数据分析推荐查看该目录下文件

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存Host和各个Device的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```

查看具体的性能分析结果：

```
# 查看Task Duration 以及各项数据
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```