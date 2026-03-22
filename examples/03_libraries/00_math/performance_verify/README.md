# Ascend C API 性能验证工具

用于验证 Ascend C API 的性能表现（也附带基本精度功能验证）。

---

## 一、已支持 API 验证

### 配置测试场景

编辑 `testcases.json`：

```json
{
  "test_apis": ["exp"],    // 指定测试的API，如 ["all"] 测试全部
  "exp": {
    "half": [4, 16, 48],          // 数据类型: [长度列表]，单位 K
    "float": [2, 8, 24]
  },
  "theory": {
    "exp": {"half": 32, "float": 32}   // 理论并行度
  }
}
```

### 快速执行

```bash
bash test.sh
```


### 结果查看

| 文件 | 内容 |
|------|------|
| `result.md` | 性能报告（达标率） |
| `performance_results.log` | 详细执行日志 |


## 二、添加新 API 验证

### 步骤 1：编辑 `new_api.md`

```markdown
**1. API 名称**: Rsqrt

**2. 函数原型**:
template <typename T>
__aicore__ inline void Rsqrt(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t& count)

**3. API 类型**:
单目API / 双目API / 双目标量API

**4. 支持的数据类型**:
half/float/int16/int32

**5. 功能说明**:
按元素进行开方后取倒数的计算

**6. 理论性能**:
{"half": 128, "float": 64}
```

### 步骤 2：执行 SKILL

```bash
# 在 performance_verify 目录下执行
提示词：重新执行SKILL performance_verify_skill.md
```
SKILL 会自动完成：输入信息解析、代码适配、测试用例执行与结果展示。  
注：部分未支持的API类型，可能需要手动适配，可参考`performance_verify_skill.md`的[Step2. 代码自动适配]，进行适配。


## 三、API 类型说明

| 类型 | TYPE值 | 签名 | 示例 |
|------|--------|------|------|
| 单目 | 1 | `dst = Op(src)` | Exp等 |
| 双目 | 2 | `dst = Op(src0, src1)` | Mul等 |
| 双目标量 | 3 | `dst = Op(src, scalar)` | Adds等 |
| 其他 | ... | ... | 待补充 |

---

## 四、当前支持

| API_ID | API | 类型 | 数据类型 |
|--------|-----|------|----------|
| 1 | Exp | Unary | half, float |
| 2 | Mul | Binary | half, float, int16, int32 |
| 3 | Adds | BinaryScalar | half, float, int16, int32 |
| 4 | LeakyRelu | BinaryScalar | half, float |
