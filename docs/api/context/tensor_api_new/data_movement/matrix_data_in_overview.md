# 矩阵计算的搬入总体说明

矩阵计算的搬入接口主要实现Global Memory到L1 Buffer、L1 Buffer到L0A Buffer/L0B Buffer/L0ScaleA Buffer/L0ScaleB Buffer/BiasTable Buffer/Fixpipe Buffer的数据高效传输。

针对Ascend 950PR/Ascend 950DT：

**表1** **数据通路和存储层级**

<a name="tensor_api_matrix_data_in_overview_table_data_path"></a>
|源位置|源地址对齐要求|目的位置|目的地址对齐要求|格式转换|典型应用场景|
|--------|--------|--------|--------|--------|--------|
|Global Memory|1字节|L1 Buffer(A1)|32字节|ND2ND、ND2NZ、DN2NZ、NZ2NZ|从全局内存加载左矩阵数据到L1 Buffer|
|Global Memory|1字节|L1 Buffer(B1)|32字节|ND2ND、ND2ZN、DN2ZN、ZN2ZN|从全局内存加载右矩阵数据到L1 Buffer|
|Global Memory|1字节|L1 Buffer(ScaleA)|32字节|ScaleA ND2ZZ、ScaleA DN2ZZ、ZZ2ZZ|从全局内存加载左矩阵缩放系数到L1 Buffer|
|Global Memory|1字节|L1 Buffer(ScaleB)|32字节|ScaleB ND2NN、ScaleB DN2NN、NN2NN|从全局内存加载右矩阵缩放系数到L1 Buffer|
|L1 Buffer(A1)|32字节|L0A Buffer(A2)|512字节|NZ2NZ、ZN2NZ|从L1 Buffer加载左矩阵数据到L0A Buffer|
|L1 Buffer(B1)|32字节|L0B Buffer(B2)|512字节|ZN2ZN、NZ2ZN|从L1 Buffer加载右矩阵数据到L0B Buffer|
|L1 Buffer(ScaleA)|32字节|L0ScaleA Buffer|512字节|ZZ2ZZ|从L1 Buffer加载左矩阵缩放系数到L0ScaleA Buffer|
|L1 Buffer(ScaleB)|32字节|L0ScaleB Buffer|512字节|NN2NN|从L1 Buffer加载右矩阵缩放系数到L0ScaleB Buffer|
