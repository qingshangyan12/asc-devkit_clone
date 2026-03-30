# 重构规范详细参考

> 本文档为SKILL的详细补充，提供代码示例和检查清单。

---

## 一、基线样例对比

### 1.1 set_atomic_add 样例

| 项目 | Baseline | Refactor |
|------|----------|----------|
| 数据类型 | 仅 `float` | int8/16、half、bfloat16、int32、float |
| 饱和模式 | 不支持 | 支持 sat/non-sat 切换 |

### 1.2 Add 样例 (basic_api_memory_allocator_add)

| 项目 | Baseline | Refactor |
|------|----------|----------|
| 核数 | 8核 | **1核** |
| 数据类型 | 仅 `float` | int16、int32、half、float |
| 饱和模式 | 不支持 | 支持 sat/non-sat 切换 |

---

## 二、重构要点（按文件）

### 2.1 `{api_name}.asc`

| 要点 | 说明 | 代码示例 |
|------|------|----------|
| **多核→单核** | numBlocks>1改为1，适配地址偏移 | `add_custom<<<1, nullptr, stream>>>` |
| **模板化** | 使用`template<typename T>` | `template <typename T> class KernelAdd` |
| **SetSaturationMode** | API调用前设置饱和模式 | 见下方完整代码 |

```cpp
template <typename T>
class KernelAddV3 {
public:
    __aicore__ inline void Init(GM_ADDR x, GM_ADDR y, GM_ADDR z, 
                                 uint32_t singleCoreLength, bool isSaturation)
    {
        isSat = isSaturation;
        // ...
    }

private:
    template <typename DataType>
    __aicore__ inline void SetSaturationMode()
    {
        if constexpr (std::is_same_v<DataType, float> || 
                      std::is_same_v<DataType, half> || 
                      std::is_same_v<DataType, bfloat16_t>) {
            isSat ? set_ctrl(sbitset0(get_ctrl(), AscendC::CTRL_48_BIT))
                  : set_ctrl(sbitset1(get_ctrl(), AscendC::CTRL_48_BIT));
        } else {
            isSat ? set_ctrl(sbitset1(get_ctrl(), AscendC::CTRL_53_BIT))
                  : set_ctrl(sbitset0(get_ctrl(), AscendC::CTRL_53_BIT));
        }
    }
};
```

**内核函数模板化 + 实例化**：
```cpp
template <typename T>
__global__ __vector__ void add_custom(GM_ADDR x, GM_ADDR y, GM_ADDR z, 
                                      GM_ADDR tiling, bool isSaturation);

template __global__ __vector__ void add_custom<int16_t>(...);
template __global__ __vector__ void add_custom<int32_t>(...);
template __global__ __vector__ void add_custom<half>(...);
template __global__ __vector__ void add_custom<float>(...);
```

---

### 2.2 `gen_data.py`

| 要点 | 说明 |
|------|------|
| 特殊值 | 浮点型：min, max, +0, -0, NaN, +INF, -INF；整型：min, max, +0, -0 |
| 单输入 | `generate_one_input(data_type)` → 7个(浮点)或4个(整型) |
| 双输入 | `generate_two_inputs(data_type)` → 49个(浮点)或16个(整型)组合 |
| Golden | 分别计算sat和non-sat两套结果 |

```python
# 单目API
special_vals = generate_one_input(data_type)

# 双目API  
inputs_a, inputs_b = generate_two_inputs(data_type)

# Golden计算
def compute_golden(a, b, dtype, saturation=True):
    result = a + b
    limits = get_dtype_limits(dtype)
    if saturation:
        result = np.clip(result, limits['min'], limits['max'])
    return result
```

---

### 2.3 `verify_result.py`

| 要点 | 说明 |
|------|------|
| 输出格式 | 参考 `references/output-format.md` |
| 颜色 | non-sat: 橙色，sat: 蓝色 |
| 多输入遍历 | `range(num_special * num_special)` |

```python
# 多输入场景遍历（关键！）
total_combos = num_special * num_special
for i in range(total_combos):  # 不是 range(num_special)
    name_a = special_names[i // num_special]
    name_b = special_names[i % num_special]
```

---

### 2.4 `test.sh`

```bash
DTYPES=("int16_t" "int32_t" "half" "float")
INDICES=(0 1 2 3)

for i in "${!DTYPES[@]}"; do
    ./build/add ${INDICES[$i]} 1  # sat模式
    ./build/add ${INDICES[$i]} 0  # non-sat模式
done
```

---

## 三、检查清单

### 3.1 算子代码 (.asc)
- [ ] 类模板化 `template <typename T>`
- [ ] GlobalTensor/LocalTensor 类型参数化
- [ ] TQue 队列类型参数化
- [ ] Init 函数添加 `isSaturation` 参数
- [ ] 内核函数模板化 + 显式实例化
- [ ] 添加 `SetSaturationMode()` 函数
- [ ] 多核改为单核

### 3.2 主函数
- [ ] 命令行参数解析 (dtype, saturation)
- [ ] `DataType` 枚举定义
- [ ] 类型分发 `switch` 语句
- [ ] 动态文件路径构建

### 3.3 Python脚本
- [ ] 支持多种numpy数据类型
- [ ] `generate_one_input()` / `generate_two_inputs()`
- [ ] 生成 sat/non-sat 两套 golden
- [ ] verify_result.py 遍历所有组合
- [ ] 输出格式符合 `output-format.md`

---

## 四、文件结构

```
{api_name}_refactor/
├── {api_name}.asc           # 核心算子 (模板化)
├── CMakeLists.txt          # 构建配置
├── data_utils.h            # 文件读写
├── test.sh                 # 测试脚本
├── scripts/
│   ├── gen_data.py         # 数据生成
│   ├── gen_data_utils.py  # 工具模块
│   └── verify_result.py   # 结果验证
├── input/                  # 输入数据
└── output/                 # 输出+golden
```
