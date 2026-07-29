#!/usr/bin/env python3
import os, struct
case_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
input_dir = os.path.join(case_dir, "input")
os.makedirs(input_dir, exist_ok=True)
buf_size = 16
data = [float(i) for i in range(buf_size)]
with open(os.path.join(input_dir, "src0.bin"), "wb") as f:
    for v in data: f.write(struct.pack("f", v))
print(f"gen_data.py: generated {buf_size} floats to input/src0.bin")
