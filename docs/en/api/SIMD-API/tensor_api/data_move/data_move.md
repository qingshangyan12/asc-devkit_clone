# Movement

<!-- md-trans-meta sourceCommit=9bfdf41ea5ec539a90d71bf0d1a3098d7cd16c53 translatedAt=2026-10-27T09:51:51.784Z pushedAt=2026-10-27T13:04:46.069Z nl=2 -->

The Tensor API uses `copy` to uniformly perform movement between different storage locations. A Tensor represents data at different storage locations and types, and a Layout represents the size and arrangement rules of a Tensor.

- [copy (movement from Global Memory to L1 Buffer)](../matrix_compute/cube_compute_load/copy_gm_to_l1.md)
- [copy (movement from Global Memory to Unified Buffer)](../reg_vector_compute/vector_compute_load/copy_gm_to_ub.md)
- [copy (movement from L1 Buffer to Unified Buffer)](../matrix_compute/cube_compute_load/copy_l1_to_ub.md)
- [copy (movement from L1 Buffer to L0A Buffer)](../matrix_compute/cube_compute_load/copy_l1_to_l0a.md)
- [copy (movement from L1 Buffer to L0B Buffer)](../matrix_compute/cube_compute_load/copy_l1_to_l0b.md)
- [copy (movement from L1 Buffer to L0ScaleA Buffer)](../matrix_compute/cube_compute_load/copy_l1_to_l0scalea.md)
- [copy (movement from L1 Buffer to L0ScaleB Buffer)](../matrix_compute/cube_compute_load/copy_l1_to_l0scaleb.md)
- [copy (movement from L1 Buffer to Fixpipe Buffer)](../matrix_compute/cube_compute_load/copy_l1_to_fixbuf.md)
- [copy (movement from L1 Buffer to BiasTable Buffer)](../matrix_compute/cube_compute_load/copy_l1_to_biastable.md)
- [copy (movement from L0C Buffer to Global Memory)](../matrix_compute/cube_compute_store/copy_l0c_to_gm.md)
- [copy (movement from L0C Buffer to Unified Buffer)](../matrix_compute/cube_compute_store/copy_l0c_to_ub.md)
- [copy (movement from L0C Buffer to L1 Buffer)](../matrix_compute/cube_compute_store/copy_l0c_to_l1.md)
- [copy (movement from Unified Buffer to Global Memory)](../reg_vector_compute/vector_compute_store/copy_ub_to_gm.md)
- [copy (movement from Unified Buffer to Unified Buffer)](../reg_vector_compute/vector_compute_load/copy_ub_to_ub.md)
- [copy (movement from Unified Buffer to L1 Buffer)](../reg_vector_compute/vector_compute_load/copy_ub_to_l1.md)

For description of `copy`, `make_copy`, Copy Operation, Trait, and Atom, see [Algorithm](../algorithm.md).
