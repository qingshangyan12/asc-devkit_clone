# 量化数据生成器 Skill

## 概述
本 Skill 提供了一个通用的 C++ 实现的量化操作测试数据生成器，用于替代原有的 Python 脚本。

## 功能特性
- 为量化操作生成随机输入数据
- 支持多种量化策略（PER_TENSOR、PER_CHANNEL、PER_TOKEN、PER_GROUP）
- 支持多种数据类型转换（int8_t 到 float16/half 等）
- 使用固定随机种子确保结果可复现
- 可集成到现有的 C++ 示例代码中

## 使用方法

### 方式一：独立使用

#### 编译数据生成器
```bash
g++ -std=c++17 -o gen_data gen_data.cpp
```

#### 生成测试数据
```bash
./gen_data
```

### 方式二：集成到示例代码中

将 `GenerateTestData()` 函数添加到示例源文件中，在 `main` 函数中调用：

```cpp
int32_t main(int32_t argc, char* argv[])
{
    // 生成测试数据
    if (!GenerateTestData()) {
        printf("Failed to generate test data!\n");
        return -1;
    }
    
    // 原有的示例代码...
}
```

### 生成的文件
- `input/input_src.bin` - 输入张量
- `input/input_offset.bin` - 偏移量张量
- `input/input_scale.bin` - 缩放因子张量
- `output/golden.bin` - 真值输出

## 实现细节

### 可配置参数
- **K (行数)**: 可配置的行维度大小
- **N (列数)**: 可配置的列维度大小
- **offsetLength**: 偏移量长度
- **groupSize**: 分组大小
- **随机种子**: 固定种子确保可复现性
- **数值范围**: 可配置的随机数范围

### 支持的量化策略

#### PER_TENSOR（按张量量化）
```
dst[i][j] = scale * (src[i][j] + offset)
```

#### PER_CHANNEL（按通道量化）
```
dst[i][j] = scale[i / groupSize][j] * (src[i][j] + offset[i / groupSize][j])
```

#### PER_TOKEN（按Token量化）
```
dst[i][j] = scale[i][j / groupSize] * (src[i][j] + offset[i][j / groupSize])
```

#### PER_GROUP（按组量化）
```
dst[i][j] = scale[i / groupSize][j] * (src[i][j] + offset[i / groupSize][j])
```

### 支持的数据类型
- **输入类型**: int8_t, uint8_t, int16_t, uint16_t
- **偏移量/缩放因子类型**: float16, half, float32
- **输出类型**: float16, half, float32

## 通用编译运行流程

### 完整构建和测试流程
```bash
# 1. 编译示例
mkdir -p build && cd build
cmake ..
make -j

# 2. 生成测试数据（两种方式任选其一）
# 方式A: 使用独立的数据生成器
cd ../scripts
g++ -std=c++17 -o gen_data gen_data.cpp
./gen_data

# 方式B: 如果已集成到示例中，直接运行会自动生成数据
cd ../build

# 3. 运行示例
./demo
```

### 预期输出
```
Generated input data:
  src shape: [K, N], dtype: int8_t
  offset shape: [offsetLength, N], dtype: float16
  scale shape: [offsetLength, N], dtype: float16
  golden shape: [K, N], dtype: float16
```

## 代码模板

### 基础数据生成函数模板
```cpp
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include <filesystem>
#include <cstdint>

namespace fs = std::filesystem;

static bool GenerateTestData() {
    // 配置参数
    const int K = 8;           // 行数
    const int N = 128;         // 列数
    const int offsetLength = 1; // 偏移量长度
    const int groupSize = K / offsetLength; // 分组大小
    const int seed = 10;       // 随机种子
    const float minVal = -10.0f; // 最小值
    const float maxVal = 10.0f;  // 最大值

    // 初始化随机数生成器
    std::mt19937 gen(seed);
    std::uniform_real_distribution<float> dist(minVal, maxVal);

    // 分配内存
    std::vector<int8_t> src(K * N);
    std::vector<uint16_t> offset(offsetLength * N);
    std::vector<uint16_t> scale(offsetLength * N);
    std::vector<uint16_t> dst(K * N);

    // 生成输入数据
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            src[i * N + j] = static_cast<int8_t>(std::round(dist(gen)));
        }
    }

    // 生成偏移量和缩放因子
    for (int i = 0; i < offsetLength; i++) {
        for (int j = 0; j < N; j++) {
            float offset_val = dist(gen);
            float scale_val = dist(gen);
            std::memcpy(&offset[i * N + j], &offset_val, sizeof(uint16_t));
            std::memcpy(&scale[i * N + j], &scale_val, sizeof(uint16_t));
        }
    }

    // 计算真值输出（根据选择的量化策略）
    for (int i = 0; i < K; i++) {
        for (int j = 0; j < N; j++) {
            float src_val = static_cast<float>(src[i * N + j]);
            
            float offset_val, scale_val;
            int offset_idx = (i / groupSize) * N + j;
            std::memcpy(&offset_val, &offset[offset_idx], sizeof(uint16_t));
            std::memcpy(&scale_val, &scale[offset_idx], sizeof(uint16_t));
            
            // 反量化计算公式
            float dst_val = scale_val * (src_val + offset_val);
            std::memcpy(&dst[i * N + j], &dst_val, sizeof(uint16_t));
        }
    }

    // 创建目录并写入文件
    try {
        fs::create_directories("input");
        fs::create_directories("output");

        std::ofstream src_file("input/input_src.bin", std::ios::binary);
        src_file.write(reinterpret_cast<const char*>(src.data()), src.size() * sizeof(int8_t));
        src_file.close();

        std::ofstream offset_file("input/input_offset.bin", std::ios::binary);
        offset_file.write(reinterpret_cast<const char*>(offset.data()), offset.size() * sizeof(uint16_t));
        offset_file.close();

        std::ofstream scale_file("input/input_scale.bin", std::ios::binary);
        scale_file.write(reinterpret_cast<const char*>(scale.data()), scale.size() * sizeof(uint16_t));
        scale_file.close();

        std::ofstream golden_file("output/golden.bin", std::ios::binary);
        golden_file.write(reinterpret_cast<const char*>(dst.data()), dst.size() * sizeof(uint16_t));
        golden_file.close();

        std::cout << "Generated input data:" << std::endl;
        std::cout << "  src shape: [" << K << ", " << N << "], dtype: int8_t" << std::endl;
        std::cout << "  offset shape: [" << offsetLength << ", " << N << "], dtype: float16" << std::endl;
        std::cout << "  scale shape: [" << offsetLength << ", " << N << "], dtype: float16" << std::endl;
        std::cout << "  golden shape: [" << K << ", " << N << "], dtype: float16" << std::endl;

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error generating test data: " << e.what() << std::endl;
        return false;
    }
}
```

## C++ 实现的优势

1. **一致性**: 与主示例代码使用相同的编程语言
2. **性能**: 对于大数据集生成速度更快
3. **可移植性**: 无需 Python 依赖
4. **集成性**: 可轻松集成到 C++ 构建系统中
5. **通用性**: 模板化设计，易于适配不同的量化场景

## 与 Python 版本的对比

| 特性 | Python | C++ |
|---------|--------|-----|
| 依赖项 | numpy | 无 |
| 编译 | 无需 | 需要编译 |
| 执行速度 | 中等 | 快速 |
| 集成方式 | 外部调用 | 原生集成 |
| 随机种子 | numpy.seed | mt19937 |
| 类型安全 | 动态类型 | 静态类型检查 |

## 适用场景

- **反量化操作**: AntiQuantize, AntiQuant
- **量化操作**: Quantize, Quant
- **混合精度计算**: 低精度输入到高精度输出
- **模型推理测试**: 量化模型的精度验证
- **性能测试**: 大规模数据生成

## 注意事项

1. **随机数一致性**: C++ 实现使用 `std::mt19937`，与 Python 的 numpy 随机数生成器可能略有差异
2. **Float16 处理**: Float16 值以 `uint16_t` 格式存储，使用 `memcpy` 进行拷贝
3. **内存对齐**: 确保数据类型与硬件架构对齐要求一致
4. **量化策略**: 根据实际应用选择合适的量化策略
5. **数值范围**: 根据量化精度要求调整随机数范围

## 扩展和定制

### 支持不同的数据类型
```cpp
// 根据需要修改数据类型
std::vector<int16_t> src(K * N)();        // 使用 int16_t
std::vector<float> offset(offsetLength * N);  // 使用 float32
```

### 支持不同的量化策略
```cpp
// 修改计算公式部分
switch (quantizationStrategy) {
    case PER_TENSOR:
        dst_val = scale_val * (src_val + offset_val);
        break;
    case PER_CHANNEL:
        // 按通道量化的计算逻辑
        break;
    case PER_GROUP:
        // 按组量化的计算逻辑
        break;
}
```

### 添加更多验证
```cpp
// 添加数据范围检查
if (dst_val < minOutput || dst_val > maxOutput) {
    std::cerr << "Warning: Output value out of range" << std::endl;
}
```
