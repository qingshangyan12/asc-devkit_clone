# L1 Buffer, L0A Buffer/L0B Buffer Memory Structure

<!-- md-trans-meta sourceCommit=5df1d1c781b51fcb7826db7f2afab5f8bc453e93 translatedAt=2026-10-22T13:40:09.859Z pushedAt=2026-10-28T12:45:45.503Z nl=2 -->

To improve data access efficiency and throughput, the Buffer adopts a memory structure design with equal-capacity memory modules (Banks). The memory space is divided into multiple Banks, which are further grouped into multiple Bank Groups (BGs). Each Bank Group contains several Banks, and each Bank is an independently operable storage unit submodule.

The following describes the detailed structure of the Cube-side memories (L1 Buffer, L0A Buffer, and L0B Buffer).

## L1 Buffer Memory Structure

The L1 Buffer has a total capacity of 512 KB and consists of 16 Banks. Each Bank has a capacity of 32 KB and consists of 1024 rows, with each row being 32 bytes wide. **A single Bank supports at most one read or one write at the same time.**

These 16 Banks are further organized into 8 Bank Groups. Each Bank Group contains 2 Banks (Bank0 and Bank1). **Two Banks belonging to the same Bank Group support one read and the other write, but do not support simultaneous reads or simultaneous writes.**

The memory structure of the L1 Buffer is shown in Figure 1 below, and the address encoding format is as follows:

```cpp
L1_ADDR[18:0] = {BANK[0:0], BANK_DEPTH[9:0], BG[2:0], BANK_WIDTH[4:0]}    // Bit order is from high to low, 19 bits in total.
// BANK indicates the number of the Bank where the address is located, occupying 1 bit, with a value range of [0, 1].
// BANK_DEPTH indicates the row number of the address within the Bank, occupying 10 bits, with a value range of [0, 1023].
// BG indicates the number of the Bank Group where the address is located, occupying 3 bits, with a value range of [0, 7].
// BANK_WIDTH indicates the offset of the address within a row of the Bank, occupying 5 bits, with a value range of [0, 31].
```

**Figure 1** L1 Buffer memory structure diagram  
![](../../../figures/l1_memory_bank.png "L1 Buffer memory structure diagram")

## L0A Buffer/L0B Buffer Memory Structure

The L0A Buffer and L0B Buffer each have a capacity of 64 KB and contain only one Bank with 128 rows, each row being 512 bytes wide. **Simultaneous read and write to the same address is not allowed. Matrix transfer-in writing and asc_mmad reading of different addresses in the same Bank are allowed simultaneously.**

The memory structure of the L0A Buffer/L0B Buffer is shown in Figure 2 below. The L0A Buffer and L0B Buffer use the same address encoding format:

```cpp
L0A_ADDR[15:0] = { BANK_DEPTH[6:0], BANK_WIDTH[8:0] }    // Bit order is from high to low, 16 bits in total.
L0B_ADDR[15:0] = { BANK_DEPTH[6:0], BANK_WIDTH[8:0] }
// BANK_DEPTH indicates the number of rows of the address in the Bank, occupying 7 bits, with a value range of [0, 127].
// BANK_WIDTH indicates the offset of the address within a row of the Bank, occupying 9 bits, with a value range of [0, 511].
```

**Figure 2** L0A Buffer/L0B Buffer memory structure diagram  
![](../../../figures/l0al0b_memory_bank.png "L0A Buffer/L0B Buffer memory structure diagram")
