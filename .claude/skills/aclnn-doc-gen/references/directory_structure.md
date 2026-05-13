# 算子目录结构

## 标准目录布局

```
${op_name}/                                    # 算子工程目录，${op_name}表示算子名（小写下划线形式）
├── examples/                                  # 算子调用示例目录
│   ├── test_aclnn_${op_name}.cpp             # 算子通过aclnn调用的示例
│   └── test_geir_${op_name}.cpp              # 算子通过geir调用的示例
├── op_graph/                                 # 图融合相关实现
│   ├── CMakeLists.txt
│   ├── ${op_name}_graph_infer.cpp            # InferDataType，实现算子数据类型推导
│   ├── ${op_name}_proto.h                    # 算子原型定义，用于图优化和融合
│   └── fusion_pass/                          # 算子融合规则目录
├── op_host/                                  # Host侧实现
│   ├── config/                               # 可选，二进制配置文件
│   │   └── ${soc_version}/                  # 按NPU型号区分
│   │       ├── ${op_name}_binary.json
│   │       └── ${op_name}_simplified_key.ini
│   ├── ${op_name}_def.cpp                    # 算子信息库（REG_OP、DataType、Format）
│   ├── ${op_name}_infershape.cpp            # 可选，InferShape实现
│   ├── ${op_name}_tiling.cpp                # 可选，Tiling实现（张量划分策略）
│   ├── ${op_name}_tiling.h
│   ├── ${op_name}_tiling_${sub_case}.cpp   # 可选，子场景Tiling优化
│   └── CMakeLists.txt
├── op_api/                                   # 可选，算子aclnn实现（可能位于此处或op_host/下）
│   ├── aclnn_${op_name}.cpp                 # aclnn接口实现
│   ├── aclnn_${op_name}.h
│   ├── ${op_name}.cpp                       # L0接口实现
│   ├── ${op_name}.h
│   └── CMakeLists.txt
└── op_kernel/                               # AI Core算子Device侧Kernel实现
    ├── ${sub_case}/                         # 可选，子场景目录
    │   ├── ${op_name}_${model}.h           # 算子kernel实现文件，${model}表示用户自定义文件名后缀，通常为Tiling模板名
    │   └── ...
    ├── ${op_name}_tiling_key.h             # 可选，TilingKey文件，定义Tiling策略的Key，标识不同划分方式
    ├── ${op_name}_tiling_data.h            # 可选，TilingData文件，存储Tiling策略相关配置信息，如块大小、并行度
    ├── ${op_name}.cpp                      # Kernel入口文件，包含主函数和调度逻辑
    └── ${op_name}.h                        # Kernel实现文件，定义Kernel头文件，包含函数声明、结构定义、逻辑实现
```

## 关键文件说明

| 子目录 | 关键文件 | 用途 |
|--------|---------|------|
| `op_api/` | `aclnn_*.h`/`.cpp` | aclnn接口声明与实现 |
| `op_host/` | `*_def.cpp` | 算子信息库（REG_OP、DataType、Format） |
| `op_host/` | `*_tiling.cpp` | Tiling实现（张量划分策略） |
| `op_kernel/` | `*.cpp`/`.h` | AI Core Kernel入口与实现 |
| `examples/` | `test_aclnn_*.cpp` | 调用示例代码 |

## 注意事项

- `op_api/` 目录可能位于算子根目录下，也可能位于 `op_host/` 目录下
- **跳过不读取的文件类型**：所有 `.md` 文件、`tests/` 目录下的文件

## 识别算子目录

用户提供的路径可能是算子目录，也可能是包含多个算子子目录的父目录。按以下步骤识别：

### 情况一：路径本身就是算子目录

判断条件：路径下存在 `op_api/` 或 `op_host/` 或 `op_kernel/` 至少其中之一。

- `op_api/` 下应有 `aclnn*.h` 或 `aclnn*.cpp` 文件
- 扫描接口时应同时检查 `op_api/` 和 `op_host/` 目录

### 情况二：路径是包含多个算子子目录的父目录

判断条件：路径下不存在 `op_api/`、`op_host/`、`op_kernel/`，但其一级子目录中存在包含这些目录的子目录。

典型结构示例：
```
gmm/                                # 父目录（非算子目录）
├── grouped_matmul/                 # 算子子目录
│   ├── op_api/
│   ├── op_host/
│   └── op_kernel/
├── grouped_matmul_v2/              # 算子子目录
│   ├── op_api/
│   ├── op_host/
│   └── op_kernel/
└── grouped_matmul_weight_nz/       # 算子子目录
    ├── op_api/
    ├── op_host/
    └── op_kernel/
```

处理方式：列出所有识别到的算子子目录，让用户选择要处理哪个算子。
