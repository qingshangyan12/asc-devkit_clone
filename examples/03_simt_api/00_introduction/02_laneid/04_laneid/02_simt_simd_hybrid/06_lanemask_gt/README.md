# Lanemask_gt 功能验证示例

## 概述
本示例用于验证 SIMT API 中 `lanemask_gt()` 接口的功能正确性。`lanemask_gt()` 返回当前线程的一个32位掩码，在当前线程所属的Warp中，将 "laneid严格大于当前线程" 的线程的对应位设为1，其余位为0。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- >= CANN 9.0.0-beta.2

## lanemask_gt() 接口说明
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ uint32_t lanemask_gt();
```

**功能**: 返回当前线程的32位掩码，将 laneid > 当前线程的所有 lane 位置为1。

**返回值**: 
- 类型: `uint32_t`
- 范围: 32位掩码值
- 特性: 对于 laneid 为 N 的线程，返回值为 `~((1 << (N+1)) - 1)`，即高位全为1，低 (N+1) 位为0

**示例**:
- laneid 为 0 的线程：`lanemask_gt() = 0xFFFFFFFE` (二进制: 1111 1111 1111 1111 1111 1111 1111 1110)
- laneid 为 1 的线程：`lanemask_gt() = 0xFFFFFFFC` (二进制: 1111 1111 1111 1111 1111 1111 1111 1100)
- laneid 为 31 的线程：`lanemask_gt() = 0x00000000` (二进制: 0000 0000 0000 0000 0000 0000 0000 0000)

## 样例实现

### 文件说明
- `lanemask_gt.asc`: 主程序文件，包含验证 lanemask_gt() 功能的完整代码

### 实现原理
1. **线程配置**: 启动 32 个线程（1 个 block，每个 block 32 个线程）
2. **掩码获取**: 每个 SIMT 线程调用 `lanemask_gt()` 获取自己的 lane 掩码
3. **结果验证**: 
   - 预期值: `lanemask_gt = ~((1 << ((threadIdx.x % 32) + 1)) - 1)`
   - 实际值: `lanemask_gt()` 返回值
   - 对比验证每个线程的 lanemask_gt 是否正确

### 核心代码逻辑
```cpp
__simt_vf__ inline void simt_lanemask_gt_test(
    __gm__ uint32_t* lanemaskOutput, uint32_t totalThreads)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= totalThreads) {
        return;
    }

    uint32_t mask = lanemask_gt();
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

执行程序后，将输出每个线程的 lanemask_gt 验证结果：

```
output:0xFFFFFFFE,0xFFFFFFFC,0xFFFFFFF8,0xFFFFFFF0,0xFFFFFFE0,0xFFFFFFC0,0xFFFFFF80,0xFFFFFF00,0xFFFFFE00,0xFFFFFC00,0xFFFFF800,0xFFFFF000,0xFFFFE000,0xFFFFC000,0xFFFF8000,0xFFFF0000,0xFFFE0000,0xFFFC0000,0xFFF80000,0xFFF00000,0xFFE00000,0xFFC00000,0xFF800000,0xFF000000,0xFE000000,0xFC000000,0xF8000000,0xF0000000,0xE0000000,0xC0000000,0x80000000,0x00000000
golden:0xFFFFFFFE,0xFFFFFFFC,0xFFFFFFF8,0xFFFFFFF0,0xFFFFFFE0,0xFFFFFFC0,0xFFFFFF80,0xFFFFFF00,0xFFFFFE00,0xFFFFFC00,0xFFFFF800,0xFFFFF000,0xFFFFE000,0xFFFFC000,0xFFFF8000,0xFFFF0000,0xFFFE0000,0xFFFC0000,0xFFF80000,0xFFF00000,0xFFE00000,0xFFC00000,0xFF800000,0xFF000000,0xFE000000,0xFC000000,0xF8000000,0xF0000000,0xE0000000,0xC0000000,0x80000000,0x00000000
[Success] Lanemask_gt verification passed.
```

## 验证要点

1. **掩码递减特性**: 线程 N 的掩码应包含线程 N+1 到 31 的所有位
2. **低位全清零**: 掩码值的低 (N+1) 位应全部为0（N 为当前线程的 laneid）
3. **高位全置位**: 掩码值的高 (32-N-1) 位应全部为1
4. **数值规律**: 
   - 线程0: 0xFFFFFFFE (31个1，最低位为0)
   - 线程1: 0xFFFFFFFC (30个1，最低2位为0)
   - 线程2: 0xFFFFFFF8 (29个1，最低3位为0)
   - ...
   - 线程30: 0x80000000 (1个1，最高位为1)
   - 线程31: 0x00000000 (0个1，所有位为0)

## 与其他 lanemask 函数的关系

| 函数名 | 功能描述 | 计算公式 | 示例(laneid=1) |
|--------|---------|----------|---------------|
| lanemask_eq | 当前线程位为1 | `(1 << laneid)` | 0x00000002 |
| lanemask_le | laneid ≤ N 的位为1 | `(1 << (laneid+1)) - 1` | 0x00000003 |
| lanemask_lt | laneid < N 的位为1 | `(1 << laneid) - 1` | 0x00000001 |
| lanemask_gt | laneid > N 的位为1 | `~((1 << (laneid+1)) - 1)` | 0xFFFFFFFC |

**重要关系**:
- `lanemask_gt() = ~lanemask_le()`
- `lanemask_eq() + lanemask_lt() + lanemask_gt() = 0xFFFFFFFF`

## 典型应用场景

`lanemask_gt()` 常用于以下场景：
1. **后缀和计算**: 在 warp 内进行后缀和（suffix sum）操作时排除前面的元素
2. **条件广播**: 将数据广播到 laneid 大于当前线程的其他线程
3. **排序算法**: 在 warp 内进行排序操作时比较后续元素
4. **并行扫描**: 实现反向扫描算法时需要访问后面元素的结果

## 注意事项

1. **Warp 大小**: Ascend NPU 的 warp 大小固定为 32
2. **线程分配**: 线程按 threadIdx.x 顺序分配到不同的 warp
3. **边界检查**: 核函数中需检查线程索引是否超出总线程数
4. **返回类型**: lanemask_gt() 返回 uint32_t 类型
5. **特殊值**: laneid 为 31 的线程返回 0（没有线程的 laneid > 31）
6. **计算公式**: 对于 laneid 为 N 的线程，掩码值为 `~((1 << (N+1)) - 1)`，等同于高 (31-N) 位全为1

## 故障排查

如果测试失败，请检查：
1. 环境变量是否正确配置
2. CANN 版本是否符合要求
3. NPU 驱动是否正常加载
4. 每个线程的掩码值是否符合 `~((1 << (laneid+1)) - 1)` 的预期