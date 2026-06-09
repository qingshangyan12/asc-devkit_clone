# LaneID 功能验证示例

## 概述
本示例用于验证 SIMT API 中 `laneid()` 接口的功能正确性。`laneid()` 返回当前线程在 warp（线程束）中的 lane ID，范围应为 0-31。

## 支持的产品
- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本
- >= CANN 9.0.0-beta.2

## laneid() 接口说明
```cpp
static __SIMT_DEVICE_FUNCTIONS_DECL__ int32_t laneid();
```

**功能**: 返回当前线程在 warp 中的 lane ID（0-31）。

**返回值**: 
- 类型: `int32_t`
- 范围: 0 到 31（每个 warp 包含 32 个 lanes）

## 样例实现

### 文件说明
- `laneid_test.asc`: 主程序文件，包含验证 laneid() 功能的完整代码

### 实现原理
1. **线程配置**: 启动 32 个线程（1 个 block，每个 block 32 个线程）
2. **LaneID 获取**: 每个 SIMT 线程调用 `laneid()` 获取自己的 lane ID
3. **结果验证**: 
   - 预期值: `laneid = threadIdx.x % 32`
   - 实际值: `laneid()` 返回值
   - 对比验证每个线程的 laneid 是否正确

### 核心代码逻辑
```cpp
__simt_vf__ inline void simt_laneid_test(
    __gm__ int32_t* laneidOutput, uint32_t totalThreads)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= totalThreads) {
        return;
    }

    int32_t lane = laneid();
    laneidOutput[idx] = lane;
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

执行程序后，将输出每个线程的 laneid 验证结果：

```
output:0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
golden:0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
[Success] Laneid verification passed.
```

## 验证要点

1. **LaneID 范围**: 所有返回值必须在 0-31 范围内
2. **LaneID 唯一性**: 同一个 warp 内的 32 个线程应有不同的 laneid（0-31）
3. **Warp 循环**: 每 32 个线程 laneid 应重新从 0 开始
4. **总数匹配**: 32 个线程应正确分布在 1 个 warp 中

## 注意事项

1. **Warp 大小**: Ascend NPU 的 warp 大小固定为 32
2. **线程分配**: 线程按 threadIdx.x 顺序分配到不同的 warp
3. **边界检查**: 核函数中需检查线程索引是否超出总线程数
4. **内存对齐**: 输出缓冲区大小需考虑内存对齐要求

## 故障排查

如果测试失败，请检查：
1. 环境变量是否正确配置
2. CANN 版本是否符合要求
3. NPU 驱动是否正常加载
4. 输出目录 `output/` 是否存在且有写权限