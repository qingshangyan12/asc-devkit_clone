# Matrix Computation Optimization Sample Introduction

## Overview

This directory contains optimization samples based on Cube matrix computation pipelines.

## Sample List

| Directory Name | Description |
| --- | --- |
| [00_mmad_double_buffer](./00_mmad_double_buffer) | A Matmul C-API performance sample that compares a single-buffer baseline with L1 double buffering and demonstrates cross-K-iteration overlap among MTE2, MTE1, and Cube. |
| [01_mmad_direction](./01_mmad_direction) | An MMAD direction C API sample that compares M-first and N-first settings for normal Nz, B4 ChannelMerge, and Nz2ND, demonstrating how the MMAD direction must match the Fixpipe read order when UnitFlag is enabled. |
