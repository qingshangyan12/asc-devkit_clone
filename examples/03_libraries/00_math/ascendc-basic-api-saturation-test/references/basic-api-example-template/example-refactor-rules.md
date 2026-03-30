# 重构规范详细参考

> 本文档为SKILL的详细补充，提供代码示例和检查清单。参考模板代码见 `set_atomic_add_refactor/` 目录。

---

## 一、重构要点（按文件汇总）

### 1.1 `{api_name}.asc`

| 要点 | 说明 |
|------|------|
| **多核→单核** | numBlocks>1改为1，适配地址偏移和tiling |
| **模板化** | 使用`template<typename T>`，GlobalTensor/LocalTensor/TQue类型参数化 |
| **SetSaturationMode** | API调用前设置饱和模式（区分浮点型/整型） |
| **内核函数** | 模板化 + 显式实例化支持多种类型 |
| **主函数** | 命令行参数解析(dtype, index, saturation)、类型分发switch |

**命令行参数规范**（根据输入数量决定）：
- 单输入（1 tensor）：`./demo <dtype> <input1_index> <saturation>`
- 双输入-类型A（2 tensor）：`./demo <dtype> <input1_index> <input2_index> <saturation>`
- 双输入-类型B（1 tensor + 1 scalar）：`./demo <dtype> <input1_index> <input2_index> <saturation>`

**模板代码**：见 `set_atomic_add_refactor/set_atomic_add.asc`

**重要事项**
- 双输入-类型B（1 tensor + 1 scalar）场景，以`Adds`为例，`Compute`计算函数代码实现模板如下，禁止自行编写。
```cpp
AscendC::LocalTensor<T> src1Local = inQueueSrc1.DeQue<T>(); // 第一个tensor输入
AscendC::LocalTensor<T> src2Local = inQueueSrc2.DeQue<T>(); // 第二个scalar输入
AscendC::LocalTensor<T> dstLocal = outQueueDst.AllocTensor<T>();
AscendC::LocalTensor<T> dstLocalTmp = outQueueDst.AllocTensor<T>();
            
// 整数类型，MIN/MAX/+0/-0
int32_t specialValueCnt = 4;
if constexpr (std::is_same_v<DataType, float> || std::is_same_v<DataType, half> || std::is_same_v<DataType, bfloat16_t>) {
    // 浮点数类型，MIN/MAX/+0/-0/NaN/INF/-INF
    specialValueCnt = 7;
}
SetSaturationMode<T>();
// scalar数据从输入的src2Local获取
// 浮点类型输入，则tensor输入为 [MIN, MIN, MIN, MIN, MIN, MIN, MIN, MAX, MAX, ...]
// 浮点类型输入，则scalar输入为 [MIN, MAX, +0, -0, NaN, +INF, -INF, ...]
for (int i = 0; i < specialValueCnt; ++i) {
    AscendC::Duplicate(dstLocalTmp, T(0), INPUT_TENSOR_SIZE);
    for (int j = 0; j < specialValueCnt; ++j) {
        // 通过for循环，对tensor用不同的scalar进行计算处理（MIN->MAX->+0->-0->NaN->+INF->-INF）
        T scalar = src2Local.GetValue(j);
        AscendC::Adds(dstLocalTmp, src1Local, scalar, INPUT_TENSOR_SIZE);
        // tensor+scalar = MIN + MIN的结果在dstLocalTmp.GetValue(i * specialValueCnt)
        // tensor+scalar = MIN + MAX，放到dstLocald的j + i * specialValueCnt位置。
        // ...依此类推
        dstLocal.SetValue(j + i * specialValueCnt, dstLocalTmp.GetValue(i * specialValueCnt));
    }
}
outQueueDst.EnQue<T>(dstLocal);
inQueueSrc1.FreeTensor(src1Local);
inQueueSrc2.FreeTensor(src2Local);
```

---

### 1.2 `gen_data.py`（必须严格拷贝模板）

| 要点 | 说明 |
|------|------|
| **【必须】** | 直接拷贝 `set_atomic_add_refactor/scripts/gen_data.py`，禁止自行编写 |
| 特殊值 | 浮点型：min, max, +0, -0, NaN, +INF, -INF（7个）；整型：min, max, +0, -0（4个） |

**【重要】根据输入数量决定数据生成方式**：

| 输入类型 | 数据生成方式 | 遍历数量 |
|---------|-------------|---------|
| 单输入（1 tensor） | 每个special_index对应一个特殊值 | 7 |
| 双输入-类型A（2 tensor） | 所有输入组合 | 7×7=49 (浮点型) / 4×4=16 (整型) |
| 双输入-类型B（1 tensor + 1 scalar） | 所有输入组合 | 7×7=49 (浮点型) / 4×4=16 (整型) |

- 单输入：使用 `generate_one_input()`
- 双输入-类型A（2 tensor）：使用 `generate_two_inputs()`
- 双输入-类型B（1 tensor + 1 scalar）：使用 `generate_two_inputs()`，scalar跟tensor一样构造方式
- Golden计算：分别计算sat和non-sat两套结果

**模板代码**：直接拷贝 `set_atomic_add_refactor/scripts/gen_data.py`

---

### 1.3 `gen_data_utils.py`（必须严格拷贝模板）

| 函数 | 说明 |
|------|------|
| **【必须】** | 直接拷贝 `set_atomic_add_refactor/scripts/gen_data_utils.py`，禁止自行编写 |
| `get_dtype_limits()` | 获取各类型数值范围(min/max/0/NaN/INF) |
| `generate_one_input()` | 生成单输入测试数据 |
| `generate_two_inputs()` | 生成双输入测试数据组合 |
| `save_bin()` | 保存二进制文件 |
| `get_special_names()` | 获取特殊值名称列表 |

**模板代码**：直接拷贝 `set_atomic_add_refactor/scripts/gen_data_utils.py`

---

### 1.4 `verify_result.py`（必须严格拷贝模板）

| 要点 | 说明 |
|------|------|
| **【必须】** | 直接拷贝 `set_atomic_add_refactor/scripts/verify_result.py`，禁止自行编写 |
| 输出格式 | 必须严格符合 `references/output-format.md` |

**【重要】表格列必须与输入数量严格匹配**：

| 输入类型 | 表格列 | 遍历数量 |
|---------|--------|---------|
| 单输入（1 tensor） | input1 | 7 |
| 双输入-类型A（2 tensor） | input1, input2 | 49 |
| 双输入-类型B（1 tensor + 1 scalar） | input1, **scalar** | 49 |

- 颜色：non-sat: 橙色，sat: 蓝色

**模板代码**：直接拷贝 `set_atomic_add_refactor/scripts/verify_result.py`

---

### 1.5 `test.sh`（必须严格拷贝模板）

```bash
# 单输入示例
DTYPES=("half" "float")
SPECIAL_INDICES=(0 1 2 3 4 5 6)

for i in "${!DTYPES[@]}"; do
    for j in "${!SPECIAL_INDICES[@]}"; do
        ./build/demo $i $j 1  # sat模式
        ./build/demo $i $j 0  # non-sat模式
    done
done
```

```bash
# 双输入-类型A（2 tensor）或 双输入-类型B（1 tensor + 1 scalar）
DTYPES=("half" "float") # 根据API实际支持的数据类型改写

# 注意：多个输入的数据类型相同，只需要一层循环遍历数据类型。如果多个输入的数据类型不相同，需要按支持的数据类型组合方式，进行多层循环遍历。
for i in "${!DTYPES[@]}"; do
    # sat模式
    ./build/demo $i 1
    # non-sat模式
    ./build/demo $i 0
done
```

**模板代码**：直接拷贝 `set_atomic_add_refactor/test.sh`

---

## 二、检查清单

### 2.1 算子代码 (.asc)
- [ ] 类模板化 `template <typename T>`
- [ ] GlobalTensor/LocalTensor/TQue 类型参数化
- [ ] Init 函数添加 `isSaturation` 参数
- [ ] 内核函数模板化 + 显式实例化
- [ ] 添加 `SetSaturationMode()` 函数
- [ ] 多核改为单核
- [ ] 命令行参数解析 (dtype, index, saturation)
- [ ] DataType 枚举定义
- [ ] 类型分发 switch 语句
- [ ] 动态文件路径构建

### 2.2 Python脚本（必须严格按模板）
- [ ] **严格拷贝模板代码**（gen_data.py, gen_data_utils.py, verify_result.py）
- [ ] **禁止自行编写** Python脚本，必须基于模板修改
- [ ] 根据API支持的数据类型修改 DTYPES
- [ ] **根据输入数量（单输入/双输入）修改循环嵌套**
- [ ] **verify_result.py 表格列必须与输入数量严格匹配**
- [ ] **单输入：表格必须有 input1 列**
- [ ] **双输入-类型A（2 tensor）：表格必须有 input1, input2 列**
- [ ] **双输入-类型B（1 tensor + 1 scalar）：表格必须有 input1, scalar 列**
- [ ] 输出格式符合 `references/output-format.md`

---

## 三、判断API输入类型的标准

**【重要】必须先判断API类型再开始实现！**

### 判断步骤

1. **查看API参数说明**（必做）
   - 如果只有1个`src`输入 源操作数 → 单输入（1 tensor）
   - 如果有`src0`, `src1`两个输入 源操作数 → 双输入-类型A（2 tensor）
   - 如果有`src` + `scalarValue`输入 源操作数 → 双输入-类型B（1 tensor + 1 scalar）

2. **对照常见API速查表**（必做）
   | API输入数量类型 | 示例API | 特征 |
   |---------|--------|------|
   | 单输入（1 tensor） | Exp, Ln, Abs, Reciprocal, Sqrt, Rsqrt, Relu | 参数说明只有1个src输入参数 |
   | 双输入-类型A（2 tensor） | Add, Sub, Mul, Div, Max, Min | 参数说明有src0, src1两个tensor输入参数 |
   | 双输入-类型B（1 tensor + 1 scalar） | Adds, Muls, Maxs, Mins, LeakyRelu | 参数说明有src和scalarValue输入参数 |

3. **确认后再实现**（必做）
   - 单输入：生成1个输入文件，使用generate_one_input
   - 双输入-类型A：生成2个tensor输入文件，使用generate_two_inputs
   - 双输入-类型B：生成1个tensor+1个scalar输入文件，使用generate_two_inputs

### 常见错误警示

❌ **错误1：将Adds等当作单输入处理**
- Adds有src和scalarValue两个参数，是双输入-类型B
- 需要2个输入文件（input_x和input_scalar）
- Compute函数需要双循环遍历所有组合

❌ **错误2：忽略scalar参数**
- 双输入-类型B的scalar也是特殊值，需要构造测试数据

---

## 四、文件结构

```
{api_name}_refactor/
├── {api_name}.asc           # 核心算子 (模板化)
├── CMakeLists.txt          # 构建配置
├── data_utils.h            # 文件读写
├── test.sh                 # 测试脚本
├── scripts/
│   ├── gen_data.py         # 数据生成 (严格拷贝模板)
│   ├── gen_data_utils.py  # 工具模块 (严格拷贝模板)
│   └── verify_result.py   # 结果验证 (严格拷贝模板)
├── input/                  # 输入数据
└── output/                 # 输出+golden
```

---

## 五、参考模板位置

| 文件 | 路径 |
|------|------|
| 算子代码 | `set_atomic_add_refactor/set_atomic_add.asc` |
| 数据生成 | `set_atomic_add_refactor/scripts/gen_data.py` |
| 工具模块 | `set_atomic_add_refactor/scripts/gen_data_utils.py` |
| 结果验证 | `set_atomic_add_refactor/scripts/verify_result.py` |
| 测试脚本 | `set_atomic_add_refactor/test.sh` |
| 输出格式 | `references/output-format.md` |
