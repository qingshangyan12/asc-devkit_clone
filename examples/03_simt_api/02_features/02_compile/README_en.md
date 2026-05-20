# Compile Sample Introduction

## Overview

This document presents compile samples based on Ascend C SIMT, demonstrating various SIMT compilation modes.

## Operator Development Samples

| Directory Name | Description |
|-----------------------------------------------------|------------------------|
| [basic_compile](./basic_compile/) | Demonstrates the basic heterogeneous compilation workflow of Host-side .cpp and Device-side .asc mixed files. |
| [seperate_compile](./seperate_compile/) | Demonstrates splitting Device-side code into multiple .asc files with separate compilation, enabling cross-file device function calls. |
| [dynamic_library_compile](./dynamic_library_compile/) | Demonstrates compiling Device-side .asc files into dynamic shared libraries (.so). |
| [static_library_compile](./static_library_compile/) | Demonstrates compiling Device-side .asc files into static libraries (.a). |
