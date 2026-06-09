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

1. 启动 1 个 block，每个 block 32 个线程，共 32 个线程
2. 每个 block 包含 32 / 32 = 1 个 warp
3. 每个线程调用 `laneid()` 并存储结果
4. 生成 golden 数据：golden[i] = i % 32
5. 验证 output 与 golden 是否完全匹配

## 预期输出

```
Output:
[0]=0 [1]=1 [2]=2 [3]=3 [4]=4 [5]=5 [6]=6 [7]=7 
[8]=8 [9]=9 [10]=10 [11]=11 [12]=12 [13]=13 [14]=14 [15]=15 
[16]=16 [17]=17 [18]=18 [19]=19 [20]=20 [21]=21 [22]=22 [23]=23 
[24]=24 [25]=25 [26]=26 [27]=27 [28]=28 [29]=29 [30]=30 [31]=31 

Golden:
[0]=0 [1]=1 [2]=2 [3]=3 [4]=4 [5]=5 [6]=6 [7]=7 
[8]=8 [9]=9 [10]=10 [11]=11 [12]=12 [13]=13 [14]=14 [15]=15 
[16]=16 [17]=17 [18]=18 [19]=19 [20]=20 [21]=21 [22]=22 [23]=23 
[24]=24 [25]=25 [26]=26 [27]=27 [28]=28 [29]=29 [30]=30 [31]=31 

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
- **Block**: 线程块（本例中每个 block 有 32/32=1 个 warp）
- **Grid**: 线程块的网络结构

## 配置说明

- 1 个 block × 32 线程/block = **32 个线程**
- 每个 block 有 32/32 = **1 个 warp**
- 共有 1 block × 1 warp/block = **1 个 warp**
- Lane ID 范围：**[0, 31]**