# Compile Sample Introduction

## Overview

This document presents compile samples based on Ascend C SIMT, demonstrating various SIMT compilation modes.

## Operator Development Samples

| Directory Name | Description |
|-----------------------------------------------------|------------------------|
| [00_basic_compile](./00_basic_compile) | Demonstrates the basic heterogeneous compilation workflow of calling a Host + Device mixed .asc file containing Kernel definitions and <<<>>> kernel launches from Host-side .cpp. |
| [01_separate_compile](./01_separate_compile) | Demonstrates splitting Device-side code into multiple .asc files with separate compilation, enabling cross-file device function calls. |
| [02_dynamic_library_compile](./02_dynamic_library_compile) | Demonstrates compiling Host + Device mixed .asc files into dynamic shared libraries (.so). |
| [03_static_library_compile](./03_static_library_compile) | Demonstrates compiling Host + Device mixed .asc files into static libraries (.a). |
