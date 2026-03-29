# Ascend C 算子代码重构规范示例

> 本文档记录 `set_atomic_add_baseline` 到 `set_atomic_add_refactor` 的重构细节，供其他 SKILL 参考。

---

## 一、重构概述

| 项目 | Baseline | Refactor |
|------|----------|----------|
| **定位** | 单类型(float)演示代码 | 多类型+饱和度模式完整测试 |
| **数据类型** | 仅 `float` | `int8_t`, `int16_t`, `half`, `bfloat16_t`, `int32_t`, `float` |
| **功能模式** | 固定 | 支持 saturation/non-saturation 切换 |
| **代码行数** | 108 行 | 277 行 |

---

## 二、核心差异明细

### 2.1 算子类模板化

**Before (Baseline):**
```cpp
class KernelSetAtomicAdd {
    // 硬编码 float 类型
    AscendC::GlobalTensor<float> src0Global, dstGlobal;
};
```

**After (Refactor):**
```cpp
template <typename T>
class KernelSetAtomicAdd {
    // 模板化，支持多种数据类型
    AscendC::GlobalTensor<T> src0Global, dstGlobal;
};
```

**重构要点:**
- 类声明添加 `template <typename T>`
- 所有 `float` 替换为 `T`
- 全局张量、LocalTensor、队列类型均需模板化

---

### 2.2 Init 函数扩展

**Before:**
```cpp
__aicore__ inline void Init(__gm__ uint8_t* src0Gm, __gm__ uint8_t* dstGm, AscendC::TPipe* pipeIn)
```

**After:**
```cpp
__aicore__ inline void Init(__gm__ uint8_t* src0Gm, __gm__ uint8_t* dstGm, AscendC::TPipe* pipeIn, bool isSaturation)
{
    isSat = isSaturation;  // 新增饱和度模式参数
    // ...
}
```

---

### 2.3 饱和度控制逻辑

**Refactor 新增:**
```cpp
template <typename DataType>
__aicore__ inline void SetSaturationMode()
{
    if constexpr (std::is_same_v<DataType, float> || 
                  std::is_same_v<DataType, half> || 
                  std::is_same_v<DataType, bfloat16_t>) {
        // 浮点类型: CTRL_48_BIT
        if (isSat) {
            set_ctrl(sbitset0(get_ctrl(), AscendC::CTRL_48_BIT));
        } else {
            set_ctrl(sbitset1(get_ctrl(), AscendC::CTRL_48_BIT));
        }
    } else {
        // 整数类型: CTRL_53_BIT
        if (isSat) {
            set_ctrl(sbitset1(get_ctrl(), AscendC::CTRL_53_BIT));
        } else {
            set_ctrl(sbitset0(get_ctrl(), AscendC::CTRL_53_BIT));
        }
    }
}
```

**调用位置:**
```cpp
__aicore__ inline void CopyOut()
{
    // ... DataCopy 完成
    SetSaturationMode<T>();          // 设置饱和度模式
    AscendC::SetAtomicAdd<T>();     // 执行原子加
    // ...
}
```

---

### 2.4 内核函数模板化与实例化

**Before:**
```cpp
__global__ __vector__ void set_atomic_add_custom(GM_ADDR x, GM_ADDR z);
```

**After:**
```cpp
template <typename T>
__global__ __vector__ void set_atomic_add_custom(GM_ADDR x, GM_ADDR z, bool isSaturation);

// 显式实例化 6 种类型
template __global__ __vector__ void set_atomic_add_custom<int8_t>(GM_ADDR, GM_ADDR, bool);
template __global__ __vector__ void set_atomic_add_custom<int16_t>(GM_ADDR, GM_ADDR, bool);
template __global__ __vector__ void set_atomic_add_custom<half>(GM_ADDR, GM_ADDR, bool);
template __global__ __vector__ void set_atomic_add_custom<bfloat16_t>(GM_ADDR, GM_ADDR, bool);
template __global__ __vector__ void set_atomic_add_custom<int32_t>(GM_ADDR, GM_ADDR, bool);
template __global__ __vector__ void set_atomic_add_custom<float>(GM_ADDR, GM_ADDR, bool);
```

---

### 2.5 RunKernel 特化分发

**Refactor 新增:**
```cpp
template <typename T>
void RunKernel(aclrtStream stream, uint8_t* xDevice, uint8_t* outDevice, DataType dtype, bool isSaturation);

template <>
void RunKernel<int8_t>(aclrtStream stream, uint8_t* xDevice, uint8_t* outDevice, DataType dtype, bool isSaturation)
{
    (void)dtype;
    set_atomic_add_custom<int8_t><<<3, nullptr, stream>>>(xDevice, outDevice, isSaturation);
}

// ... 其他 5 种类型类似
```

---

### 2.6 运行时参数解析

**Refactor 新增:**
```cpp
int main(int argc, char* argv[]) 
{
    if (argc < 2) {
        printf("Usage: %s <dtype> [saturation]\n");
        // 0=int8, 1=int16, 2=half, 3=bfloat16, 4=int32, 5=float
        return -1;
    }
    int dtypeIdx = atoi(argv[1]);
    int saturation = (argc >= 3) ? atoi(argv[2]) : 1;
    
    DataType dtype = static_cast<DataType>(dtypeIdx);
    // ... 根据 dtype 调用对应 RunKernel
}
```

---

## 三、Python 脚本重构

### 3.1 数据生成脚本 (gen_data.py)

| 特性 | Baseline | Refactor |
|------|----------|----------|
| 依赖 | `numpy` | `numpy` + `tensorflow` (bfloat16) |
| 数据类型 | `float32` | 6 种 |
| 数据内容 | 全 1 向量 | 特殊值 (min, max, zero, NaN, ±INF) |
| 饱和度 | 无 | 分别生成 sat/non-sat 两套 golden |

**Refactor 数据内容:**
```python
def construct_special_values(data_type):
    limits = get_dtype_limits(data_type)
    specials = [limits['min'], limits['max'], limits['pos_zero']]
    if np.issubdtype(data_type, np.floating):
        specials.extend([limits['nan'], limits['pos_inf'], limits['neg_inf']])
    return specials
```

---

### 3.2 验证脚本 (verify_result.py)

| 特性 | Baseline | Refactor |
|------|----------|----------|
| 验证方式 | 单文件比对 | 遍历 6 种类型 |
| 输出格式 | 简单文本 | 彩色表格 |
| 特殊值处理 | 无 | NaN/Inf 专门处理 |

**Refactor 表格输出示例:**
```
======================================================================
  Verify Output vs Golden
======================================================================
[int64] BLOCK=256 CORES=3
----------------------------------------------------------------------
 name     | input         | golden(sat)   | golden(non-sat) | output(sat)    | output(non-sat)
----------------------------------------------------------------------
 min      | -9.2234e+18   | -2.7668e+19   | -2.7668e+19    | -2.7668e+19    | -2.7668e+19
 max      | 9.2234e+18    | 2.7668e+19    | 2.7668e+19     | 2.7668e+19     | 2.7668e+19
 ...
----------------------------------------------------------------------
   Saturation:     OK (mismatches=0)
   Non-saturation: OK (mismatches=0)
 Result: sat=PASS, non-sat=PASS [PASS]
```

---

### 3.3 工具模块 (gen_data_utils.py) - 新增

**职责:**
- `get_dtype_limits()`: 获取各类型数值范围
- `construct_special_values()`: 构造特殊测试值
- `save_bin()`: 保存二进制文件
- `get_special_names()`: 获取特殊值名称列表

**依赖关系:**
```
gen_data.py → gen_data_utils.py
verify_result.py → gen_data_utils.py
```

---

## 四、重构检查清单

### 4.1 算子代码

- [ ] 类模板化 `template <typename T>`
- [ ] GlobalTensor/LocalTensor 类型参数化
- [ ] TQue 队列类型参数化
- [ ] Init 函数添加运行时参数
- [ ] 内核函数模板化 + 显式实例化
- [ ] 添加 `SetSaturationMode()` 控制逻辑
- [ ] 使用 `set_ctrl` / `sbitset0` / `sbitset1` API

### 4.2 主函数

- [ ] 添加命令行参数解析
- [ ] 定义 `DataType` 枚举
- [ ] 添加类型分发 `switch` 语句
- [ ] 实现 `RunKernel` 特化版本
- [ ] 动态构建输入/输出文件路径

### 4.3 Python 脚本

- [ ] 支持多种 numpy 数据类型
- [ ] 使用 tensorflow 处理 bfloat16
- [ ] 生成特殊值测试用例 (min, max, zero, NaN, ±INF)
- [ ] 分别生成 sat/non-sat 两套 golden
- [ ] 验证脚本支持彩色输出
- [ ] 提取公共工具到 `*_utils.py`

---

## 五、关键 API 参考

| API | 作用 | 适用场景 |
|-----|------|----------|
| `set_ctrl(sbitset0(...))` | 清除控制位 | 关闭饱和度 |
| `set_ctrl(sbitset1(...))` | 设置控制位 | 开启饱和度 |
| `AscendC::CTRL_48_BIT` | 48位浮点饱和 | float/half/bfloat16 |
| `AscendC::CTRL_53_BIT` | 53位整数饱和 | int8/16/32 |
| `SetAtomicAdd<T>()` | 启用原子加 | 数据写入时 |

---

## 六、文件结构

```
set_atomic_add_refactor/
├── set_atomic_add.asc      # 核心算子 (模板化)
├── CMakeLists.txt         # 构建配置 (不变)
├── data_utils.h           # 文件读写 (不变)
├── test.sh                # 测试脚本
├── scripts/
│   ├── gen_data.py        # 数据生成 (扩展)
│   ├── gen_data_utils.py # 工具模块 (新增)
│   └── verify_result.py  # 结果验证 (增强)
├── input/                 # 输入数据 (6种类型)
└── output/               # 输出+golden (sat/non-sat)
```

---

## 七、构建与运行

```bash
# 编译
cd build && cmake .. && make

# 生成测试数据
cd scripts && python3 gen_data.py

# 运行 (参数: dtype saturation)
./build/demo 5 1   # float + saturation
./build/demo 0 0   # int8_t + non-saturation

# 验证结果
python3 scripts/verify_result.py
```

---

*本文档可作为 Ascend C 算子从单类型演示代码重构为完整多类型测试用例的参考模板。*
