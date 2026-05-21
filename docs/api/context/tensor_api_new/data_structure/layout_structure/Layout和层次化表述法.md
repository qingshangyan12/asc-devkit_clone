# Layout和层次化表述法

## Layout

我们使用Shape和Stride来表达Layout排布格式，比如下图中的行优先和列优先排布：

Shape用于表达Tensor形状，Stride则用于区分不同的排布：

- 行优先:Shape \(2, 4\)  Stride \(4, 1\)

- 列优先:Shape \(2, 4\)  Stride \(1, 2\)

图中每个方格中的数字表示该位置元素在内存中按顺序排列时的下标。对于相同的矩阵位置，排布方式不同时，其在内存中的顺序可能是不同的，例如，矩阵坐标 \(1, 0\) 在行优先和列优先的情况下，对应元素在内存中的顺序分别是4和1。

**图 1**  行优先排布

![行优先排布](../../figures/行优先排布.png)

**图 2**  列优先排布

![列优先排布](../../figures/列优先排布.png)

## 层次化表述法

通常Shape或者Stride中的元素是一个单独的整数（如上文的示例），但是遇到一些复杂的内存排布情况可能无法表达，所以采用[层次化表述法](https://dl.acm.org/doi/abs/10.1145/3582016.3582018)，该方式中，元素也可以是一个元组。元组的第一个元素用于描述不同层次中矩阵的行数，元组的第二个元素用于描述不同层次中矩阵的列数。

例如在下图中，Layout的Shape和Stride分别为：

- Shape\(\(2, 3\), \(2, 4\)\)

- Stride\(\(1, 4\), \(2, 12\)\)

**图 3**  层次化表述法

![层次化表述法](../../figures/层次化表述法.png "层次化表述法")

图中展示了两层矩阵：内层矩阵为内部用灰色线包裹的矩阵，外层矩阵为将内层矩阵视为一个元素时黑色线包裹的矩阵。

Shape的第一个元素描述行方向的形状，\(2，3\)表示内层矩阵和外层矩阵的行数分别为2和3；Shape的第二个元素描述的是列方向上的形状，\(2，4\)表示内存矩阵和外层矩阵的列数分别为2和4。

Stride中的每个元素与Shape中的元素对应，表示该对应维度下，相邻元素首地址在内存地址上的间隔，图片中用箭头表示了每个维度相邻元素的首地址间隔。

### 2.2常见的Layout分形

在基于Ascend C进行矩阵编程场景下，会用到以下几种常用的Layout，这些格式都采用如上文介绍的层次化表述法来表达，要求有内外层两层矩阵，具体表达方式如下：

```cpp
Layout = ((Shape): (Stride))
Shape = ((ShapeRow0, ShapeRow1), (ShapeColumn0, ShapeColumn1))
Stride = ((StrideRow0, StrideRow1), (StrideColumn0, StrideColumn1))
```

- NZ Layout

    NZ Layout格式的ShapeRow0、ShapeColumn0为固定值，下文示例中内层矩阵的Shape为\(32字节 / sizeof\(T\)\) \* 16；StrideRow0、StrideColumn0也为固定值，即内层矩阵按照行优先方式Z字形连续存储。外层矩阵按照N字型存储，所以StrideRow1应该满足如下的对齐要求。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_16, ShapeRow1), (_32 / sizeof(T), ShapeColumn1))
    Stride = ((_32 / sizeof(T), StrideRow1), (_1, StrideColumn1))
    // StrideRow1应满足：
    （StrideRow1 - ShapeRow0 * ShapeColumn0）% ShapeColumn0 = 0
    ```

    下面是一个连续的NZ Layout的示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_16, row / _16), (_32/ sizeof(T), column / (_32 / sizeof(T))))
    Stride = ((_32 / sizeof(T), _32 / sizeof(T) * _16), (_1, _32 / sizeof(T) * row))
    ```

- ZN Layout

    ZN Layout格式的ShapeRow0、ShapeColumn0为固定值，下文示例中内层矩阵的Shape为\(32字节 / sizeof\(T\)\) \* 16；StrideRow0、StrideColumn0也为固定值，即内层矩阵按照行优先方式N字形连续存储。外层矩阵按照Z字型存储，所以StrideRow1应该满足如下的对齐要求。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_32 / sizeof(T), ShapeRow1), (_16, ShapeColumn1))
    Stride = ((_1, StrideRow1), (_32 / sizeof(T), StrideColumn1))
    // StrideRow1应满足：
    （StrideColumn1 - ShapeRow0 * ShapeColumn0）% ShapeRow0 = 0
    ```

    下面是一个连续的ZN Layout的示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_32 / sizeof(T), row / 32 / sizeof(T)), (_16, column / _16))
    Stride = ((_1, 32 / sizeof(T) * column), (_32 / sizeof(T), _32 / sizeof(T) * _16))
    ```

- ND Layout，一种使用层次化表述法描述的二维行优先排布格式。

    其中ShapeRow是行数，ShapeColumn为列数。
    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = (ShapeRow,  ShapeColumn)
    Stride = (ShapeColumn, _1)
    ```

    下面是一个连续的ND Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = (row, column)
    Stride = (column, _1)
    ```

- NDExt Layout，一种使用层次化表述法描述的四维行优先排布格式。

    NDExt Layout格式的ShapeRow0和ShapeColumn0为固定值，即内层矩阵的Shape为1 \* 1，StrideRow0和StrideColumn0也为固定值0，内层矩阵只有一个元素，没有Stride的概念。StrideColumn1为1，表示外层矩阵列方向上是连续的。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, ShapeRow1), (_1, ShapeColumn1))
    Stride = ((_0, StrideRow1), (_0, _1))
    ```

    下面是一个连续的NDExt Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, row),(_1, column))
    Stride = ((_0, column),(_0, _1))
    ```

- DN Layout，一种使用层次化表述法描述的二维列优先排布格式。

    其中ShapeRow是行数，ShapeColumn为列数。
    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = (ShapeRow, ShapeColumn)
    Stride = (_1, ShapeRow)
    ```

    下面是一个连续的DN Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = (row, column)
    Stride = (_1, row)
    ```

- DNExt Layout，一种使用层次化表述法描述的四维列优先排布格式。

    DNExt Layout格式的ShapeRow0和ShapeColumn0为固定值，即内层矩阵的Shape为1 \* 1，StrideRow0和StrideColumn0也为固定值0，内层矩阵只有一个元素，没有Stride的概念。StrideRow1为1，表示外层矩阵行方向上是连续的。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, ShapeRow1), (_1, ShapeColumn1))
    Stride = ((_0, _1), (_0, StrideColumn1))
    ```

    下面是一个连续的DNExt Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, row),(_1, column))
    Stride = ((_0, _1),(_0, row))
    ```

- ZZ Layout

    ZZ Layout格式的ShapeRow0、ShapeColumn0为固定值，下文示例中内层矩阵的Shape为16 \* \(32字节 / sizeof\(T\)\)；StrideRow0、StrideColumn0也为固定值，即内层矩阵按行优先方式Z字形连续存储。外层矩阵同样按Z字形存储，因此外层相邻分形在列方向上连续，行方向跨度由整行分形块数决定。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_16, ShapeRow1), (_32 / sizeof(T), ShapeColumn1))
    Stride = ((_32 / sizeof(T), StrideRow1), (_1, _32 / sizeof(T) * 16))
    ```

    下面是一个连续的ZZ Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_16, Std::ceil_division(row, _16)), (_32 / sizeof(T), Std::ceil_division(column, _32 / sizeof(T))))
    Stride = ((_32 / sizeof(T), _16 * Std::ceil_align(column, _32 / sizeof(T))), (_1, _32 / sizeof(T) * 16))
    ```

- NN Layout

    NN Layout格式的ShapeRow0、ShapeColumn0为固定值，其中ShapeRow0为2，ShapeColumn0为16，仅支持 `fp8_e8m0_t` 对应的 `C0_ELEMENT == 2` 场景。StrideRow0、StrideColumn0也为固定值，即内层矩阵按列优先方式N字形连续存储；外层矩阵继续按N字形存储。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_2, ShapeRow1), (_16, ShapeColumn1))
    Stride = ((_1, _32), (_2, StrideColumn1))
    ```

    下面是一个连续的NN Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_2, row / 2), (_16, Std::ceil_division(column, 16)))
    Stride = ((_1, _32), (_2, row * 16))
    ```

- ScaleAND Layout

    ScaleAND Layout是一种用于 `fp8_e8m0_t` scaleA 数据的二维行优先排布格式，仅支持 `C0_ELEMENT == 2`。它的物理布局与连续的NDExt Layout一致，但语义上用于描述 scaleA 的 ND 形式输入。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, ShapeRow1), (_1, ShapeColumn1))
    Stride = ((_0, StrideRow1), (_0, _1))
    ```

    下面是一个连续的ScaleAND Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, row), (_1, column))
    Stride = ((_0, column), (_0, _1))
    ```

- ScaleADN Layout

    ScaleADN Layout是一种用于 `fp8_e8m0_t` scaleA 数据的分形列优先排布格式，仅支持 `C0_ELEMENT == 2`。其列方向内层分形大小固定为2，外层按DN方式组织。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, ShapeRow1), (_2, ShapeColumn1))
    Stride = ((_0, _2), (_1, StrideColumn1))
    ```

    下面是一个连续的ScaleADN Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, row), (_2, column / 2))
    Stride = ((_0, _2), (_1, 2 * row))
    ```

- ScaleBND Layout

    ScaleBND Layout是一种用于 `fp8_e8m0_t` scaleB 数据的分形行优先排布格式，仅支持 `C0_ELEMENT == 2`。其行方向内层分形大小固定为2，外层按ND方式组织。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_2, ShapeRow1), (_1, ShapeColumn1))
    Stride = ((_1, StrideRow1), (_0, _2))
    ```

    下面是一个连续的ScaleBND Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_2, row / 2), (_1, column))
    Stride = ((_1, 2 * column), (_0, _2))
    ```

- ScaleBDN Layout

    ScaleBDN Layout是一种用于 `fp8_e8m0_t` 的排布格式。它的物理布局与连续的DNExt Layout一致，但语义上用于描述 scale 的 DN 形式输入。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, ShapeRow1), (_1, ShapeColumn1))
    Stride = ((_0, _1), (_0, StrideColumn1))
    ```

    下面是一个连续的ScaleBDN Layout示例。

    ```cpp
    Layout = ((Shape) : (Stride))
    Shape = ((_1, row), (_1, column))
    Stride = ((_0, _1), (_0, row))
    ```

## 数据结构表

每种分形对应的Layout格式如下表所示，其中T指的是支持的数据类型中除fp8_e8m0_t之外的数据类型，`C0_ELEMENT = Std::Int<32/sizeof(T)>{}`，特殊情况当T为fp4x2_e2m1_t或fp4x2_e1m2_t时，`C0_ELEMENT = _64`。

| LayoutFormatPattern | 类型 | ShapeRow0 | ShapeRow1 | ShapeColumn0 | ShapeColumn1 | StrideRow0 | StrideRow1 | StrideColumn0 | StrideColumn1 |
|-------------|------|-----------|-----------|--------------|--------------|------------|------------|---------------|---------------|
| NZLayoutPtn | T | Int<16>{} | ceil_div(row, Int<16>{}) | C0_ELEMENT | ceil_div(column, C0_ELEMENT) | C0_ELEMENT | C0_ELEMENT * Int<16>{} | Int<1>{} | C0_ELEMENT * ceil_align(row, Int<16>{}) |
| ZNLayoutPtnZn | T | C0_ELEMENT | ceil_div(row, C0_ELEMENT) | Int<16>{} | ceil_div(column, Int<16>{}) | Int<1>{} | C0_ELEMENT * ceil_align(column, Int<16>{}) | C0_ELEMENT | C0_ELEMENT * Int<16>{} |
| DNExtLayoutPtn | T | Int<1>{} | row | Int<1>{} | column | Int<0>{} | Int<1>{} | Int<0>{} | row |
| NDExtLayoutPtn | T | Int<1>{} | row | Int<1>{} | column | Int<0>{} | column | Int<0>{} | Int<1>{} |
| DNExtLayoutPtn | T || row || column || Int<1>{} || row |
| NDLayoutPtn | T || row || column || column || Int<1>{} |
| ZZLayoutPtn | T | Int<16>{} | ceil_div(row, Int<16>{}) | C0_ELEMENT | ceil_div(column, C0_ELEMENT) | C0_ELEMENT | Int<16>{} * ceil_align(column, C0_ELEMENT) | Int<1>{} | C0_ELEMENT * Int<16>{} |
| ZZLayoutPtn | fp8_e8m0_t | Int<16>{} | ceil_div(row, Int<16>{}) | Int<2>{} | column / Int<2>{} | Int<2>{} | column * Int<16>{} | Int<1>{} | Int<32>{} |
| NNLayoutPtn | fp8_e8m0_t | Int<2>{} | row / Int<2>{} | Int<16>{} | ceil_div(column, Int<16>{}) | Int<1>{} | Int<32>{} | Int<2>{} | row * Int<16>{} |
| ScaleANDLayoutPtn | fp8_e8m0_t | Int<1>{} | row | Int<1>{} | column | Int<0>{} | column | Int<0>{} | Int<1>{} |
| ScaleADNLayoutPtn | fp8_e8m0_t | Int<1>{} | row | Int<2>{} | column / Int<2>{} | Int<0>{} | Int<2>{} | Int<1>{} | Int<2>{} * row |
| ScaleBNDLayoutPtn | fp8_e8m0_t | Int<2>{} | row / Int<2>{} | Int<1>{} | column | Int<1>{} | Int<2>{} * column | Int<0>{} | Int<2>{} |
| ScaleBDNLayoutPtn | fp8_e8m0_t | Int<1>{} | row | Int<1>{} | column | Int<0>{} | Int<1>{} | Int<0>{} | row |
