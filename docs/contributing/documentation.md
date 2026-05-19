# 文档写作指南

> 通用 Markdown 写作规范（标题、列表、图片、表格、代码块、标点等）请参考 [CANN 文档写作规范](https://gitcode.com/cann/community/blob/master/contributor/docs/document_writing_specs.md)。

## 1. API 文档组织

```
docs/
├── api/
│   └── context/
│       ├── c_api/
│       │   ├── vector_compute/
│       │   │   ├── asc_add.md
│       │   │   └── ...
│       │   ├── general_instruction.md
│       │   └── c_api_list.md
│       └── adv_api/
│           └── math/
│               ├── axpy.md
│               └── ...
├── guide/
│   └── ...
└── README.md
```

## 2. API 文档模板

每个 API 文档（`.md`）应包含：

1. API 名称和功能概述
2. 函数原型（含模板参数说明表格、接口参数说明表格）
3. 参数约束和取值范围
4. 支持的架构和数据类型
5. 使用示例代码
6. 注意事项
