# Introduction to thread_group

<!-- md-trans-meta sourceCommit=e552eb8ded44e154ba5812983f5bf0383b80631c translatedAt=2026-10-16T11:32:13.105Z pushedAt=2026-10-20T03:02:40.200Z -->

`thread_group` is the base class of all other cooperative group classes. It can be regarded as a generic cooperative group type used to represent different types of thread groups in a unified manner. Developers can use `thread_group` to implement polymorphism. For details, see the [reduce_sum sample](../../../../../../examples/03_simt_api/02_features/01_api_features/04_cooperative_groups/reduce_sum/README.md).

Through `thread_group`, only the basic interfaces supported by all common cooperative groups can be called.

## Public Member Functions

```cpp
void sync() const;
unsigned long long size() const;
unsigned long long num_threads() const;
unsigned long long thread_rank() const;
group_type get_type() const;
```
