# 数据结构(TensorAPI，试验特性)

- **[内置数据类型](../../basic_api/data_structures/builtin_data_types.md)**

- **[精度转换](../../basic_api/data_structures/precision_conversion.md)**

- **layout相关基础数据结构**
    - **[layout和层次化表述法](layout_structure/layout_hierarchical_representation.md)**
    - **[layout](layout_structure/layout.md)**
    - **[shape](layout_structure/shape.md)**
    - **[stride](layout_structure/stride.md)**
    - **[coord](layout_structure/coord.md)**
    - **layout相关接口**
        - **[make_layout](layout_structure/make_layout.md)**
        - **[make_frame_layout](layout_structure/make_frame_layout.md)**
        - **[make_shape](layout_structure/make_shape.md)**
        - **[make_stride](layout_structure/make_stride.md)**
        - **[make_coord](layout_structure/make_coord.md)**
        - **[get_shape](layout_structure/get_shape.md)**
        - **[get_stride](layout_structure/get_stride.md)**
        - **[get](layout_structure/get.md)**
        - **[select](layout_structure/select.md)**
        - **[squeeze](layout_structure/squeeze.md)**
        - **[capacity](layout_structure/capacity.md)**
        - **[size](layout_structure/size.md)**
        - **[coshape](layout_structure/coshape.md)**
        - **[cosize](layout_structure/cosize.md)**
        - **[rank](layout_structure/rank.md)**

- **Tensor相关数据结构**
    - **[Tensor](tensor_structure/tensor.md)**
    - **[Pointer](tensor_structure/pointer.md)**
    - **[view_engine](tensor_structure/view_engine.md)**
    - **Tensor相关接口**
        - **[make_tensor](tensor_structure/make_tensor.md)**
        - **[make_mem_ptr](tensor_structure/make_mem_ptr.md)**
        - **[slice](tensor_structure/slice.md)**

- **工具接口**
    - **[get_layout_pattern](utils/get_layout_pattern.md)**
