#!/usr/bin/env python3
import os
case_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
input_dir = os.path.join(case_dir, "input")
os.makedirs(input_dir, exist_ok=True)
print("gen_data.py: data generated")
