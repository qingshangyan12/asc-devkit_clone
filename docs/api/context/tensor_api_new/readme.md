# Tensor API

Tensor API 是基于Ascend C的C++模板抽象库的张量编程接口，专为定义和操作层次化多维数据布局而设计。Tensor API提供Layout和Tensor对象，将数据类型、形状、存储空间和内存布局紧凑地封装在一起，并为用户自动处理复杂的索引计算，让开发者能以直观、逻辑化的方式访问数据，而无需手动计算底层地址。

Tensor API将张量Tensor看作是带有布局语义的数据视图，并由内存引擎（Engine）和数据布局（Layout）为张量的两个正交组成部分。Engine 持有硬件指针并负责底层内存访问，Layout 携带形状、步幅和布局格式，在编译期将多维坐标自动换算为线性偏移。开发者不再需要手动计算，可直接通过坐标方式访问数据，分形布局（NZ/ZN/ND 等）的复杂映射被封装在 Layout 内部。并可通过切片操作通过返回保留完整Shape和Stride信息的子张量Tensor，使数据操作可组合、可传递，在将复杂的硬件细节封装在底层的同时，通过模板参数暴露足够的控制点，让高级用户能够对硬件行为进行精细调优。基于Layout所描述的数据组织方式，Atom（原子操作）​ 进一步封装了底层硬件指令，它将操作类型和特征参数组合在一起，成为执行最小单元的计算或数据搬运操作的基础,并统一由Copy/Mmad作为调用入口和分发逻辑。

Tensor API当前仅支持Ascend 950PR/Ascend 950DT。

## Tensor API架构

<img class="eddx" id="image292993463310" src="figures/tensor_api_arch.png">

Tensor API 采用了分层设计，各层分别承担数据表达、操作封装和指令调用三类不同职责：
Tensor层：聚合 Engine和Layout，提供坐标访问、切片等用户接口。
Atom层: 将操作类型和特征参数组合，映射到昇腾处理器的硬件指令。
Arch层: 封装硬件指令的底层接口，对参数进行处理后调用硬件指令。

第一层是 Tensor 层。该层负责描述“数据是什么”以及“数据应如何被访问”。其核心对象是 Tensor，Tensor通过聚合Engine与Layout，将底层存储地址、多维逻辑形状、步幅以及布局格式统一为一个可组合的数据视图。对用户而言，Tensor 层提供的是数据访问接口，包括坐标访问、子Tensor构造、切片、获取Shape/Stride等。

第二层是 Atom 层。Atom 层作为Tensor层与Arch层之间的桥梁，负责描述“对这些数据执行什么操作”以及“该操作携带哪些特征参数”。CopyAtom和MmadAtom分别实现了搬运和计算的原子操作封装。Atom并不直接实现具体硬件指令，而是将操作类别、默认参数traits、可配置参数以及参数展开逻辑组织在一起，形成一类可调用、可配置、可分发的操作对象。并通过with等机制，提供了对特征参数的配置能力。

第三层是 Arch 层。该层负责“在当前硬件架构、数据位置和布局条件下，具体应当调用哪条实现路径”的操作。Arch层一方面封装面向具体架构的底层指令接口，另一方面通过routing机制根据物理位置、Layout分形、数据类型以及 版本号进行静态分发。以 Copy 和 Mmad 为例，用户只需要调用统一入口，最终由Arch层判断本次操作应走GM到UB、L1到L0A或其他特定路径，并进一步落到对应架构目录下的具体实现文件中。

### 接口调用

Tensor API 的典型调用遵循 **"指针 → 布局 → 张量 → 操作"** 四步流水线，在编译期完成类型推导和路径选择：

1. **构造内存指针**：通过 `MakeMemPtr` 的模板参数标记数据所在的物理存储位置（GM / L1 / L0A / L0B / L0C / UB / BiasTable Buffer / Fixpipe Buffer）；
2. **构造数据布局**：通过 `MakeFrameLayout`（标准分形排布，如 ND / NZ / ZN / DN）或 `MakeLayout`（自由形状+步长）描述张量的形状、步长和排布模式；
3. **构造张量**：通过 `MakeTensor(ptr, layout)` 将内存指针与布局绑定，生成带有完整类型信息的属性张量；
4. **调用操作接口**：将张量传入 `Copy`（数据搬运）或 `Mmad`（矩阵计算），编译器根据张量的存储位置、布局模式和数据类型自动选择底层硬件指令实现。

### Tensor 层接口

Tensor 层是 Tensor API 的数据抽象层，负责描述"数据在哪里、长什么样、如何访问"。通过生成带类型标签的张量对象，供 Copy 和 Mmad 接口消费。

核心组件及对应接口：

- **内存指针**：`MakeMemPtr(...)` 标记数据的物理存储位置和基地址。`Location` 支持 `GM`、`L1`、`L0A`、`L0B`、`L0C`、`UB`、`BIAS`、`FIXBUF`。
- **形状与步长**：`MakeShape(...)`、`MakeStride(...)`定义张量各维度大小、步长。
- **坐标**：`MakeCoord(...)` 描述多维坐标点；`Crd2Idx(coord, shape, stride)` 将多维坐标转换为一维线性索引。
- **布局**：
  - `MakeLayout(shape, stride)` 构造自由形状+步长的通用布局；
  - `MakeFrameLayout<LayoutPtn, LayoutTrait>(...)` 构造硬件标准分形排布布局，附带数据类型和量化属性。
  - 支持 5 种 Frame 布局模式：`ND`（连续排列）、`NZ`（矩阵分形）、`ZN`（NZ 的转置形式）、`DN`（反交织排列）、`ZNZ`（双重分形排列）。
- **张量构造**：`MakeTensor(ptr, ...args)` 将内存指针与一个或多个布局绑定，生成带完整类型信息的属性张量。
- **切片操作**：`Slice(tensor, coord, info)` 从张量指定坐标处切出一个子张量，保持与原张量相同的存储位置。
- **属性查询**：`Get()`、`Size()`、`Rank()`、`Cosize()`、`Select()`、`Capacity()` 对布局对象查询维度、大小、秩、容量等属性。
- **元组操作**：`Get<I>(tuple)` 从编译期元组中提取指定位置的元素。

### Atom抽象

将操作类型和特征参数组合，映射到昇腾处理器的硬件指令：

- **Atom**：原子操作对象
  - `CopyAtom`：数据搬运原子操作；
  - `MmadAtom`：矩阵乘加原子操作。
- **Traits**：特征参数配置，包含Operation和自定义Trait
  - `CopyTraits`：数据搬运特征配置；
  - `MmadTraits`：矩阵乘加特征配置。
- **Operation**：操作类型，指定数据通路
  - 数据搬运：`CopyGM2L1`、`CopyL12L0`、`CopyL0C2Out`等；
  - 矩阵计算：`MmadOperation`等。

### Arch层接口

封装硬件指令的底层接口，对参数进行处理后调用硬件指令，用户也可以直接调用：

- **DataCopy**: GM到L1 Buffer，L1 Buffer到BiasTable Buffer/Fixpipe Buffer的数据搬运指令；
- **LoadData**: L1 Buffer到L0A Buffer/L0B Buffer的数据加载指令；
- **Fixpipe**: L0C Buffer到GM/UB的数据输出指令；
- **Mmad**: 矩阵乘加计算指令。

## 参考资源

### 文档

#### 数据结构

- [Layout](Layout和层次化表述法.md)：张量的内存排布描述，由Shape（形状）和Stride（步长）组成。支持层次化表达，可描述多重分形格式。
- [Tensor](struct/tensor/LocalTensor.md)：包含指向张量位置的指针，也包含一个用于访问其元素的Layout。

#### 接口

- [Copy](data_movement/data_movement_overview.md)：数据搬运算法，封装多级存储间的数据传输。包括DataCopy（GM→L1）、LoadData（L1→L0A/L0B）、FixPipe（L0C→OUT）等通路。
- [Mmad](matrix_computation)：矩阵乘加算法，封装MMAD指令。通过Traits配置数据类型、计算块大小等参数。

完整数据结构和接口列表请参考[Tensor API列表](tensor_api_list.md)。

### 测试和样例

本仓库提供[测试用例](../../../../tests/api/tensor_api) 以及[示例代码](../../../../examples/01_simd_cpp_api/02_features/05_tensor_api)，方便快速了解功能用法并进行验证。