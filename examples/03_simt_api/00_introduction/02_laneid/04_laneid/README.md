# Laneid 示例

本示例演示如何使用 `laneid()` API 获取线程在 Warp 内的唯一标识符。

## 功能说明

`laneid()` 返回当前线程在 Warp 内的 lane ID。一个 Warp 包含 32 个线程，因此 laneid 的取值范围为 [0, 31]。

### API 定义

```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ int32_t laneid();
```

## 核函数实现

```cpp
__global__ __maxnreg__(REG_COUNT) void laneid_test(int32_t* laneid_output, uint64_t total_length)
{
    int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx >= total_length) {
        return;
    }
    
    laneid_output[idx] = laneid();
}
```

## 验证逻辑

1. 启动 2 个 block，每个 block 64 个线程，共 128 个线程
2. 每个 block 包含 64 / 32 = 2 个 warp
3. 每个线程调用 `laneid()` 并存储结果
4. 验证 laneid 是否符合预期：
   - Lane ID 应在 [0, 31] 范围内
   - Lane ID = threadIdx % 32（线程在 warp 内的索引）

## 预期输出

```
Testing laneid() API
Configuration: 2 blocks x 64 threads/block
Expected: Each block has 2 warps
          Each warp contains 32 threads
          laneid range: [0, 31]

Lane ID verification:
Total threads: 128
Threads per block: 64
Warp size: 32

First warp (32 threads) lane IDs:
  Thread 0: laneid = 0
  Thread 1: laneid = 1
  ...
  Thread 31: laneid = 31

First 64 lane IDs (2 warps):
  Warp 0: [0]=0 [1]=1 [2]=2 ... [31]=31
  Warp 1: [0]=0 [1]=1 [2]=2 ... [31]=31

[Success] Laneid verification passed.
Laneid range: [0, 31]
```

## 编译运行

```bash
mkdir build && cd build
cmake ..
make
./demo
```

## 关键概念

- **Lane ID**: 线程在 Warp 内的唯一标识符，范围 [0, 31]
- **Warp**: 一组同时执行的 32 个线程，是 SIMT 执行的基本单位
- **Block**: 线程块，包含多个 warp（本例中每个 block 有 64/32=2 个 warp）
- **Grid**: 线程块的网络结构

## 线程索引关系

| 全局线程 ID | Block ID | Block 内线程 ID | Warp ID | Lane ID |
|------------|----------|----------------|---------|---------|
| 0-31 | 0 | 0-31 | 0 | 0-31 |
| 32-63 | 0 | 32-63 | 1 | 0-31 |
| 64-95 | 1 | 0-31 | 0 | 0-31 |
| 96-127 | 1 | 32-63 | 1 | 0-31 |

**计算公式：**
- Lane ID = threadIdx % 32
- Warp ID (block内) = threadIdx / 32
- 每个 block 有 64/32 = **2 个 warp**
- 共有 2 个 block × 2 个 warp/block = **4 个 warp**