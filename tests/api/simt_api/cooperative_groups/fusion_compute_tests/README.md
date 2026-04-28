# Cooperative Groups SIMD与SIMT混合编程测试用例

## 概述

本目录包含SIMD与SIMT混合编程场景下cooperative_groups.h中定义的API的测试用例，验证协作组在混合编程模式下的正确性。

## 与纯SIMT测试的区别

### 编译选项差异
- **纯SIMT测试**：需要 `--enable-simt` 编译选项
- **混合编程测试**：**无需** `--enable-simt` 编译选项，仅需 `--npu-arch=dav-3510`

### 算子调用方式差异
- **纯SIMT测试**：
  - 核函数定义：`__global__ void kernel_name(...)`
  - 直接在核函数中使用协作组API
  
- **混合编程测试**：
  - 核函数定义：`__global__ __vector__ void kernel_name(...)`
  - SIMT VF函数：`__simt_vf__ __launch_bounds__(thread_num) inline void simt_vf_func(...)`
  - 调用方式：`asc_vf_call<simt_vf_func>(dim3(thread_num), args...)`

## 样例列表

| 测试用例 | 功能描述 |
| -------- | -------- |
| [thread_block_fusion](./thread_block_fusion.asc) | 在SIMD-SIMT混合编程中测试thread_block协作组 |
| [thread_block_tile_fusion](./thread_block_tile_fusion.asc) | 在SIMD-SIMT混合编程中测试thread_block_tile分区 |
| [coalesced_group_fusion](./coalesced_group_fusion.asc) | 在SIMD-SIMT混合编程中测试coalesced_group合并组 |
| [shuffle_functions_fusion](./shuffle_functions_fusion.asc) | 在SIMD-SIMT混合编程中测试shuffle函数 |
| [vote_functions_fusion](./vote_functions_fusion.asc) | 在SIMD-SIMT混合编程中测试vote函数 |

## 混合编程测试特点

### 编程模式（参考simt_gather_with_ub样例）
- **核函数**：使用 `__global__ __vector__` 标记
- **SIMT VF函数**：使用 `__simt_vf__ __launch_bounds__(THREAD_COUNT)` 标记
- **启动方式**：通过 `asc_vf_call<simt_vf_func>(dim3(THREAD_COUNT), args...)` 启动SIMT VF
- **无需SIMD VF**：不使用 `__simd_vf__` 函数
- **无需复杂流程**：不使用 CopyIn/Compute/CopyOut 流程，直接在核函数中启动SIMT VF

### 核函数示例
```cpp
__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void simt_thread_block_test(
    __gm__ uint32_t* output_rank,
    __gm__ uint32_t* output_size)
{
    thread_block g = this_thread_block();
    
    uint32_t rank = static_cast<uint32_t>(g.thread_rank());
    uint32_t size = static_cast<uint32_t>(g.size());
    
    int32_t global_idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (global_idx < DATA_SIZE) {
        output_rank[global_idx] = rank;
        output_size[global_idx] = size;
    }
}

__global__ __vector__ void thread_block_fusion_kernel(
    __gm__ uint32_t* output_rank,
    __gm__ uint32_t* output_size)
{
    asc_vf_call<simt_thread_block_test>(dim3(THREAD_COUNT), output_rank, output_size);
}
```

### 核函数启动参数
- **numBlocks**：线程块数
- **dynUBufSize**：动态UB内存大小，混合编程测试中通常设置为0（不分配动态UB）
- **stream**：aclrtStream流

```cpp
kernel<<<numBlocks, dyn_ubuf_size, stream>>>(args...);
```

## 编译运行

### 配置环境变量
请根据当前环境上CANN开发套件包的安装方式，选择对应配置环境变量的命令：

```bash
source /usr/local/Ascend/cann/set_env.sh  # 默认路径，root用户安装
source $HOME/Ascend/cann/set_env.sh       # 默认路径，非root用户安装
source ${install_path}/cann/set_env.sh    # 指定路径install_path
```

### 编译执行

**实际硬件环境**
```bash
mkdir -p build && cd build
cmake ..
make -j
./cg_thread_block_fusion       # 测试thread_block
./cg_thread_block_tile_fusion  # 测试thread_block_tile
./cg_coalesced_group_fusion    # 测试coalesced_group
./cg_shuffle_functions_fusion  # 测试shuffle函数
./cg_vote_functions_fusion     # 测试vote函数
```

**仿真环境**
```bash
mkdir -p build && cd build
cmake -DRUN_MODE=sim -DSOC_VERSION=Ascend950PR_9599 ..
make -j
export LD_LIBRARY_PATH=$ENV{ASCEND_HOME_PATH}/tools/simulator/Ascend950PR_9599/lib:$LD_LIBRARY_PATH
./cg_thread_block_fusion       # 仿真执行测试
```

### 验证结果
执行成功后输出：
```
[Success] thread_block fusion test passed.
```

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- >= CANN 9.0.0-beta.2

## 技术要点

### 协作组在混合编程中的使用
1. **thread_block**：获取整个线程块信息，用于块级同步
2. **thread_block_tile**：将线程块划分为更小的tile，用于tile内协作
3. **coalesced_group**：合并活跃线程，适用于条件分支场景
4. **shuffle函数**：在tile内进行数据交换，避免共享内存访问
5. **vote函数**：在tile内进行投票操作，判断谓词条件

### 与纯SIMT编程的区别
- 混合编程中，协作组API的使用方式与纯SIMT完全相同
- 区别在于编译选项和核函数调用方式
- 混合编程可以与SIMD VF函数结合，实现更灵活的算子设计

### 编译选项说明
- **无需** `--enable-simt` 编译选项
- 仅需 `--npu-arch=dav-3510` 指定NPU架构
- 混合编程模式由核函数标记和asc_vf_call自动识别

## 参考文档

- [SIMD与SIMT混合编程](../../../../../docs/guide/编程指南/编程模型/AI-Core-SIMD编程/SIMD与SIMT混合编程.md)
- [协作组API参考](../../../../../include/simt_api/cooperative_groups.h)
- [simt_gather_with_ub样例](../../../../../examples/01_simd_cpp_api/04_best_practices/03_fusion_compute_practices/simt_gather_with_ub/)