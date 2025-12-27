# asc_mmad_sparse

## 产品支持情况

| 产品     | 是否支持 |
| ----------- |:----:|
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | √    |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | √    |

## 功能说明

完成矩阵乘加操作，传入的左矩阵A为稀疏矩阵，右矩阵B为稠密矩阵。对于矩阵A，在asc_mmad_sparse计算时完成稠密化；对于矩阵B，在计算执行前的输入数据准备时自行完成稠密化，所以输入本接口的矩阵B为稠密矩阵。稠密矩阵B需要通过[asc_copy_l12l0b_sparse](xxxxxxxxxxxxxx)载入，同时加载索引矩阵，索引矩阵在矩阵B稠密化的过程中生成，再用于矩阵A的稠密化。

## 函数原型

```cpp
__aicore__ inline void asc_mmad_sparse(__cc__ int32_t* c, __ca__ int8_t* a, __cb__ int8_t* b, uint16_t m, uint16_t k, uint16_t n, uint8_t unitFlag, bool cmatrixSource, bool cmatrixInitVal)
```


## 参数说明

表1 参数说明

| 参数名       | 输入/输出 | 描述               |
| --------- | ----- | ---------------- |
| c       | 输出    | 	目的操作数，结果矩阵C，起始地址需要1024字节对齐。            |
| a       | 输入    |   源操作数，左矩阵A，起始地址需要512字节对齐。|
| b       | 输入    | 	源操作数，右矩阵B，起始地址需要512字节对齐。      |
| m       | 输入    | 	左矩阵Height，取值范围：m∈[0,4095]。      |
| k       | 输入    | 	左矩阵Width、右矩阵Height，取值范围：k∈[0,4095]。      |
| n       | 输入    | 	右矩阵Width，取值范围：n∈[0,4095]。      |
| unitFlag       | 输入    | 	unitFlag是一种Mmad指令和Fixpipe指令细粒度的并行，使能该功能后，硬件每计算完一个分形，计算结果就会被搬出，该功能不适用于L0C Buffer累加的场景。取值说明如下： <br> 0：保留值。 <br> 2：使能unitFlag，硬件执行完指令后，不会关闭unitFlag功能。<br> 3：使能unitFlag，硬件执行完指令后，会关闭unitFlag功能。<br> 使能该功能时，Mmad指令的unitFlag在最后一个分形设置为3，其余分形计算设置为2即可。     |
| cmatrixSource       | 输入    | 	配置C矩阵初始值是否来源于BiasTable Buffer（存放Bias的硬件缓存区）。取值说明如下：  <br>&bull; true：来源于BiasTable Buffer。 <br>&bull; false：来源于L0C Buffer。    |
| cmatrixInitVal       | 输入    | 	配置C矩阵初始值是否为0。取值说明如下：  <br>&bull; true：C矩阵初始值为0。 <br>&bull; false：C矩阵初始值通过cmatrixSource参数进行配置。     |


## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 原始稀疏矩阵B每4个元素中应保证最多2个非零元素，如果存在3个或更多非零元素，则仅使用前2个非零元素。
- 当m、k、n中的任意一个值为0时，该指令不会被执行。

## 调用示例

```cpp
uint32_t total_length = 128;
__ca__ int8_t a[total_length];
__cb__ int8_t b[total_length];
__cc__ int32_t c[total_length];
uint16_t m = 2;
uint16_t k = 2;
uint16_t n = 1;
uint8_t unitFlag = 2;
bool cmatrixSource = true;
bool cmatrixInitVal = true;
asc_mmad_sparse(c, a, b, m, k, n, unitFlag, cmatrixSource, cmatrixInitVal)
```
