# Vector Compute API Samples

## Overview

This directory contains samples for multiple APIs related to vector computation. Each sample is based on Ascend C's `<<<>>>` direct invocation method, supporting both main function and kernel function implementation in the same cpp file.

## Operator Development Samples

| Directory Name | Description | Supported Products |
| -------------- | ----------- | ------------------ |
| [abs](./abs) | This sample implements Abs operation using the Reg programming interface. Relu/Exp/Sqrt/Ln/Log/Log2/Log10/Neg interfaces can also reference this sample | Ascend 950PR/Ascend 950DT |
| [arange](./arange) | This sample implements Arange operation using the Reg programming interface, generating increasing/decreasing index sequences starting from a scalar value | Ascend 950PR/Ascend 950DT |
| [cast](./cast) | This sample implements Cast operation using the Reg programming interface for RegTensor data type conversion (narrowing/widening) | Ascend 950PR/Ascend 950DT |
| [compare](./compare) | This sample implements Compare operation using the Reg programming interface for comparing two vectors or a vector with a scalar | Ascend 950PR/Ascend 950DT |
| [data_relayout](./data_relayout) | This sample demonstrates Interleave and Pack interfaces using the Reg programming interface for data relayout (Interleave + Pack low-bit extraction) | Ascend 950PR/Ascend 950DT |
| [duplicate](./duplicate) | This sample implements Duplicate operation (scalar fill) using the Reg programming interface, filling a scalar value to each position of a vector | Ascend 950PR/Ascend 950DT |
| [gather](./gather) | This sample demonstrates Gather interface (source operand is register) using the Reg programming interface to collect elements by index, using LoadAlign/StoreAlign postUpdate mode | Ascend 950PR/Ascend 950DT |
| [histogram](./histogram) | This sample implements Histogram operation using the Reg programming interface, performing histogram statistics on input data | Ascend 950PR/Ascend 950DT |
| [mergemode](./mergemode) | This sample comprehensively demonstrates MaskMergeMode::MERGING + postUpdate + UpdateMask + ReduceMax using the Reg programming interface | Ascend 950PR/Ascend 950DT |
| [mul](./mul) | This sample implements Mul operation using the Reg programming interface. Add/Sub/Div/Max/Min/Prelu interfaces can also reference this sample | Ascend 950PR/Ascend 950DT |
| [muls](./muls) | This sample implements Muls operation using the Reg programming interface. Adds/Maxs/Mins/LeakyRelu interfaces can also reference this sample | Ascend 950PR/Ascend 950DT |
| [reduce](./reduce) | This sample implements Reduce operation (SUM mode) using the Reg programming interface. Reduce interface supports SUM/MAX/MIN reduction modes | Ascend 950PR/Ascend 950DT |
| [reduce_block](./reduce_block) | This sample implements ReduceDataBlock operation (SUM mode) using the Reg programming interface, performing reduction within each DataBlock (32B) | Ascend 950PR/Ascend 950DT |
| [reduce_pair](./reduce_pair) | This sample implements PairReduceElem operation (SUM mode) using the Reg programming interface, performing reduction sum on adjacent odd-even element pairs | Ascend 950PR/Ascend 950DT |
| [select](./select) | This sample implements Select operation using the Reg programming interface, selecting elements between two vectors based on mask | Ascend 950PR/Ascend 950DT |
| [squeeze](./squeeze) | This sample implements Squeeze operation using the Reg programming interface, compressing a vector to a scalar | Ascend 950PR/Ascend 950DT |
| [truncate](./truncate) | This sample implements Truncate operation using the Reg programming interface, truncating floating-point numbers to integer bits | Ascend 950PR/Ascend 950DT |