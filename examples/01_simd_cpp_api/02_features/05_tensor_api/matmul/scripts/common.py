import numpy as np

cpp_to_py_dtype = {
    "bool": np.bool_,
    "int8_t": np.int8,
    "int16_t": np.int16,
    "int32_t": np.int32,
    "int": np.int32,
    "int64_t": np.int64,
    "uint8_t": np.uint8,
    "uint16_t": np.uint16,
    "uint32_t": np.uint32,
    "uint64_t": np.uint64,
    "half": np.float16,
    "float": np.float32
}