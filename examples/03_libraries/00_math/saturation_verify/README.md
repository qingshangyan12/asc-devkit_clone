# Ascend C API输入特殊值/边界值(Min/Max/Inf/NaN等)场景下，饱和/非饱和模式验证测试工具

本工具用于验证 Ascend C 数学 API 在饱和模式和非饱和模式下的行为差异。

## 支持的 API

| API_ID | API 名称 | 类型 | 支持的数据类型 |
|--------|----------|------|---------------|
| 1 | Exp | 单目 | half, float |
| 2 | Mul | 双目 | half, float, int16, int32 |

新API自动适配和验证请参考[快速开始-方式一](#快速开始)。

## 快速开始

### 方式一：使用 skill 自动配置（新 API）

如果您需要测试新的 API，请参考 [saturation_verify_skill.md](./saturation_verify_skill.md)：

- Step1. 填写 `user_input.md` 中的 API 信息
- Step2. 提示词：执行`saturation_verify_skill.md`
- Step3. 运行测试  
    执行命令：`INPUT_API_ID=xx INPUT_API_TYPE=yy bash test.sh`；命令中xx和yy替换为代码生成结果，参考[环境变量说明](#环境变量说明)，在Step2完成后会自动完成代码适配。

### 方式二：直接运行现有 API 测试

```bash
# 测试 Exp（单目 API）
INPUT_API_ID=1 INPUT_API_TYPE=1 bash test.sh

# 测试 Mul（双目 API）
INPUT_API_ID=2 INPUT_API_TYPE=2 bash test.sh
```

## 环境变量说明

| 变量 | 说明 | 默认值 | 可选值 |
|------|------|--------|--------|
| `INPUT_API_ID` | API 编号 | 1 | 1=Exp, 2=Mul |
| `INPUT_API_TYPE` | API 类型 | 1 | 1=单目(Unary), 2=双目(Binary) |
| `INPUT_DATA_TYPE` | 数据类型 | 0（遍历所有） | 1=float16, 2=float32, 3=int16, 4=int32 |
| `INPUT_SIZE` | 张量大小 | 128 | 当前固定值 |

如果新增了其他API，API编号会依次递增。比如新增验证Rsqrt，INPUT_API_ID=3；再新增Sub，INPUT_API_ID=4等等。

## 使用示例

### 示例 1：测试指定 API 的所有支持类型

```bash
INPUT_API_ID=1 bash test.sh
```

输出：
```
=========================================
Running supported data types for API_ID=1: 1 2
=========================================
...
```

### 示例 2：测试指定数据类型

```bash
INPUT_API_ID=1 INPUT_API_TYPE=1 INPUT_DATA_TYPE=1 bash test.sh
```

## 输出结果说明

测试完成后会输出验证表格：

```
====================================================================================================================
API: exp | Data Type: float16 | Verification Results
====================================================================================================================
type     | src                        | dst (API Output, Non-Sat) | dst (API Output, Saturation)
--------------------------------------------------------------------------------------------------------------------
float16  | min   -65504.000000        | 0.000000                 | 0.000000
float16  | max   65504.000000         | inf                      | 65504.000000
float16  | +0    0.000000             | 1.000000                 | 1.000000
float16  | -0    -0.000000            | 1.000000                 | 1.000000
float16  | NaN   nan                  | nan                      | nan
float16  | +INF  inf                  | inf                      | 65504.000000
float16  | -INF  -inf                 | 0.000000                 | 0.000000
--------------------------------------------------------------------------------------------------------------------
Total: 7, Matched: 7, Error Ratio: 0.0000
====================================================================================================================

Test PASSED!
```

### 表格列说明

| 列名 | 说明 |
|------|------|
| `src` | 输入的特殊值（min/max/+0/-0/NaN/+INF/-INF） |
| `dst (API Output, Non-Sat)` | 非饱和模式下的API输出结果 |
| `dst (API Output, Saturation)` | 饱和模式下的API输出结果 |
| `Matched` | 非饱和模式下的API输出结果，与 Golden 结果匹配的用例数 |
| `Error Ratio` | 错误率 |

## 文件结构

```
saturation_verify/
├── saturation_verify.asc      # 算子实现
├── test.sh                    # 测试入口脚本
├── user_input.md              # 用户输入 API 信息（skill 使用）
├── saturation_verify_skill.md # 自动配置 skill 文档
├── CMakeLists.txt            # 编译配置
├── data_utils.h               # 数据读写工具
├── scripts/
│   ├── gen_data.py            # 测试数据生成入口
│   ├── gen_data_exp.py        # Exp 数据生成
│   ├── gen_data_mul.py        # Mul 数据生成
│   ├── gen_data_utils.py      # 公共函数
│   └── verify_result.py       # 结果验证
├── input/                     # 输入数据目录
└── output/                    # 输出结果目录
```

## 故障排除

### 编译失败

确保已安装 CANN 工具链并配置环境变量：

```bash
source ${ASCEND_INSTALL_PATH}/set_env.sh
```

### 测试失败

1. 检查 `Error Ratio` 是否为 0
2. 确认 API_ID 和 API_TYPE 匹配
3. 检查数据类型是否在Ascend C API支持范围内（当前test.sh默认遍历执行half/float/int16_t/int32_t）