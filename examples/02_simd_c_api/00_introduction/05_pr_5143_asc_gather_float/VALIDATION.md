# PR 5143验证结果

## 验证基线

- PR：`cann/asc-devkit!5143`
- PR提交：`9b6962b7eb431045f74735c3ea219232681f360d`
- 目标架构：`dav-3510`
- CANN路径：`/usr/local/Ascend/cann-9.2.0`
- 验证日期：`2026-08-17`
- 样例：`asc_gather_vector_float.asc`

## 验证结果

| 验证项 | 结果 | 说明 |
| --- | --- | --- |
| 仅使用当前CANN头文件编译 | 符合预期地失败 | CANN 9.2.0尚无三参数`vector_float`重载，报错`no matching function for call to 'asc_gather'`。 |
| 使用PR头文件默认优化编译 | 通过 | `bisheng --npu-arch=dav-3510`编译成功。 |
| 默认优化实机运行 | 通过 | `vector_float`和`uint32_t`两组各64个结果均与位级golden一致。 |
| `-O0`编译和实机运行 | 通过 | 无mismatch、无超时。 |
| `-O2`编译和实机运行 | 通过 | 无mismatch、无超时。 |

三种构建的末行输出均为：

```text
[Success] vector_float gather is bit-exact with uint32 gather and golden.
```

部分关键数据如下：

```text
lane  index  src-lane  input(value/bits)       float-output     uint32-reference  golden
   1     65         1          -0/0x80000000  0x80000000  0x80000000  0x80000000
   3    195         3          -1/0xbf800000  0xbf800000  0xbf800000  0xbf800000
   8      8         8         nan/0x7fc12345  0x7fc12345  0x7fc12345  0x7fc12345
   9     73         9        -nan/0xffc54321  0xffc54321  0xffc54321  0xffc54321
  10    138        10  1.4013e-45/0x00000001  0x00000001  0x00000001  0x00000001
  11    203        11 -1.4013e-45/0x80000001  0x80000001  0x80000001  0x80000001
```

## 结论

在本样例覆盖范围内未发现PR 5143新增接口的问题。`vector_float`路径与底层`uint32_t`路径行为一致，浮点原始位模式未被数值转换或规范化；负零符号位、NaN payload和非规格化数均完整保留。超过寄存器元素数的索引也按文档规定执行`index % 64`。

本次验证覆盖单个256B矢量寄存器和DAV 3510实机，不代表其他架构或未定义输入范围。
