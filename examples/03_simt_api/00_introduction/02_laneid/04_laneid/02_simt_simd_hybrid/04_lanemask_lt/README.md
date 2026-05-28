# Lanemask_lt 功能验证示例

## 概述
本示例用于验证 SIMT API 中 `lanemask_lt()` 接口的功能正确性。`lanemask_lt()` 返回当前线程的一个32位掩码，在当前线程所属的Warp中，将 "laneid严格小于当前线程" 的线程的对应位设为1，其余位为0。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- >= CANN 9.0.0-beta.2

## lanemask_lt() 接口说明
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_lt();
```

**功能**: 返回当前线程的32位掩码，将 laneid < 当前线程的所有 lane 位置为1。

**返回值**: 
- 类型: `uint32_t`
- 范围: 32位掩码值
- 特性: 对于 laneid 为 N 的线程，返回值为 `(1 << N) - 1`

**示例**:
- laneid 为 0 的线程：`lanemask_lt() = 0x00000000` (二进制: 0000 0000 0000 0000 0000 0000 0000 0000)
- laneid 为 1 的线程：`lanemask_lt() = 0x00000001` (二进制: 0000 0000 0000 0000 0000 0000 0000 0001)
- laneid 为 31 的线程：`lanemask_lt() = 0x7FFFFFFF` (二进制: 0111 1111 1111 1111 1111 1111 1111 1111)

## 样例实现

### 文件说明
- `lanemask_lt.asc`: 主程序文件，包含验证 lanemask_lt() 功能的完整代码

### 实现原理
1. **线程配置**: 启动 32 个线程（1 个 block，每个 block 32 个线程）
2. **掩码获取**: 每个 SIMT 线程调用 `lanemask_lt()` 获取自己的 lane 掩码
3. **结果验证**: 
   - 预期值: `lanemask_lt = (1 << (threadIdx.x % 32)) - 1`
   - 实际值: `lanemask_lt()` 返回值
   - 对比验证每个线程的 lanemask_lt 是否正确

### 核心代码逻辑
```cpp
__simt_vf__ inline void simt_lanemask_lt_test(
    __gm__ uint32_t* lanemaskOutput, uint32_t totalThreads)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= totalThreads) {
        return;
    }

    uint32_t mask = lanemask_lt();
    lanemaskOutput[idx] = mask;
}
```

## 编译运行

### 环境准备
```bash
# 配置环境变量（根据实际安装路径选择）
source /usr/local/Ascend/cann/set_env.sh
```

### 编译执行
```bash
# 创建并进入 build 目录
mkdir -p build && cd build

# 编译（NPU 模式）
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j

# 执行测试
./demo
```

### 仿真模式编译
```bash
# NPU 仿真模式
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
```

## 预期输出

执行程序后，将输出每个线程的 lanemask_lt 验证结果：

```
output:0x00000000,0x00000001,0x00000003,0x00000007,0x0000000F,0x0000001F,0x0000003F,0x0000007F,0x000000FF,0x000001FF,0x000003FF,0x000007FF,0x00000FFF,0x00001FFF,0x00003FFF,0x00007FFF,0x0000FFFF,0x0001FFFF,0x0003FFFF,0x0007FFFF,0x000FFFFF,0x001FFFFF,0x003FFFFF,0x007FFFFF,0x00FFFFFF,0x01FFFFFF,0x03FFFFFF,0x07FFFFFF,0x0FFFFFFF,0x1FFFFFFF,0x3FFFFFFF,0x7FFFFFFF
golden:0x00000000,0x00000001,0x00000003,0x00000007,0x0000000F,0x0000001F,0x0000003F,0x0000007F,0x000000FF,0x000001FF,0x000003FF,0x000007FF,0x00000FFF,0x00001FFF,0x00003FFF,0x00007FFF,0x0000FFFF,0x0001FFFF,0x0003FFFF,0x0007FFFF,0x000FFFFF,0x001FFFFF,0x003FFFFF,0x007FFFFF,0x00FFFFFF,0x01FFFFFF,0x03FFFFFF,0x07FFFFFF,0x0FFFFFFF,0x1FFFFFFF,0x3FFFFFFF,0x7FFFFFFF
[Success] Lanemask_lt verification passed.
```

## 验证要点

1. **掩码累进特性**: 线程 N 的掩码应包含线程 0 到 N-1 的所有位
2. **低位全置位**: 掩码值的低 N 位应全部为1（N 为当前线程的 laneid）
3. **高位全清零**: 掩码值的高 (32-N) 位应全部为0
4. **数值规律**: 
   - 线程0: 0x00000000 (0个1)
   - 线程1: 0x00000001 (1个1)
   - 线程2: 0x00000003 (2个1)
   - ...
   - 线程31: 0x7FFFFFFF (31个1)

## 与 lanemask_le 的区别

`lanemask_lt()` 与 `lanemask_le()` 的主要区别：
- `lanemask_lt()`: 将 laneid **严格小于**当前线程的位置为1，计算公式为 `(1 << laneid) - 1`
- `lanemask_le()`: 将 laneid **小于等于**当前线程的位置为1，计算公式为 `(1 << (laneid + 1)) - 1`

**关系**: `lanemask_lt() = lanemask_le() >> 1`

## 典型应用场景

`lanemask_lt()` 常用于以下场景：
1. **前缀和计算**: 在 warp 内进行前缀和（prefix sum）操作时排除当前元素
2. **分段归约**: 需要根据线程索引进行条件选择的情况
3. **排序算法**: 在 warp 内进行排序操作时比较前后元素
4. **并行扫描**: 实现扫描算法时需要访问前面元素的结果

## 注意事项

1. **Warp 大小**: Ascend NPU 的 warp 大小固定为 32
2. **线程分配**: 线程按 threadIdx.x 顺序分配到不同的 warp
3. **边界检查**: 核函数中需检查线程索引是否超出总线程数
4. **返回类型**: lanemask_lt() 返回 uint32_t 类型
5. **特殊值**: laneid 为 0 的线程返回 0（没有线程的 laneid < 0）
6. **计算公式**: 对于 laneid 为 N 的线程，掩码值为 `(1 << N) - 1`

## 故障排查

如果测试失败，请检查：
1. 环境变量是否正确配置
2. CANN 版本是否符合要求
3. NPU 驱动是否正常加载
4. 每个线程的掩码值是否符合 `(1 << laneid) - 1` 的预期