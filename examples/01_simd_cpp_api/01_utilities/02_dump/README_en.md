# Dump Sample Introduction

## Overview

This sample demonstrates the basic usage of the Ascend C asc_dump series interfaces and the simd_vf asc_dump interfaces.

## Sample List

| Directory Name | Description | Supported Products |
| -------------- | ----------- | --- |
| [00_simple_dump](./00_simple_dump) | Implements matrix multiplication using static Tensor programming mode, demonstrating the basic usage of asc_dump_gm/asc_dump_l1buf/asc_dump_cbuf/asc_dump_ubuf interfaces | Ascend 950PR/Ascend 950DT<br>Atlas A3 Training Series Products/Atlas A3 Inference Series Products<br>Atlas A2 Training Series Products/Atlas A2 Inference Series Products |
| [01_simd_vf_dump](./01_simd_vf_dump) | Demonstrates the basic usage of simd_vf `__asc_simd_vf::asc_dump_ubuf`/asc_dump/`__asc_simd_vf::asc_dump_reg` interfaces using vector programming mode | Ascend 950PR/Ascend 950DT<br>Atlas A3 Training Series Products/Atlas A3 Inference Series Products |
