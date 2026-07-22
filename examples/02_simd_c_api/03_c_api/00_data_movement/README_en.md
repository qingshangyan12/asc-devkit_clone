# Data Movement API Sample Introduction

## Overview

This directory demonstrates Ascend C API data-movement interfaces between UB and vector registers. The samples cover element- and DataBlock-indexed loads, aligned loads and stores, and MaskReg loading and storing.

## Sample List

| Directory Name | Description |
| --- | --- |
| [reg_load_gather](./reg_load_gather) | Demonstrates indexed loads with `asc_gather` and `asc_gather_datablock`. |
| [reg_load_store_align](./reg_load_store_align) | Demonstrates aligned load and store modes using offsets, PostUpdate, AddrReg, DataBlock, broadcast, and upsample. |
| [reg_load_store_mask](./reg_load_store_mask) | Demonstrates MaskReg loading, storing, and selection operations. |
