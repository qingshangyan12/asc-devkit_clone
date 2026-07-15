# Communication API Sample Introduction

## Overview

This directory provides Ascend C communication samples that query communication links through HCCL Resource
APIs, create communication channels, and copy data between ranks.

## Sample List

| Directory Name | Function Description | Supported Products |
| --- | --- | --- |
| [one_multi_path](./one_multi_path) | Queries a UB_MEM link, creates one path/multi path channels for every peer, and verifies remote data movement | Ascend 950PR/Ascend 950DT |
