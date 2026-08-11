#!/usr/bin/env python3
import os,numpy as np
try:
 import en_dtypes
except ImportError as e: raise SystemExit("install en_dtypes to generate fp4 data") from e
M=N=32;K=128;SK=4;f4=en_dtypes.float4_e1m2
def pack(x):
 u=x.reshape(-1).view(np.uint8);return ((u[1::2]&15)<<4)|(u[::2]&15)
os.makedirs("input",exist_ok=True)
os.makedirs("output",exist_ok=True)
pack(np.ones((M,K),dtype=f4)).tofile("input/a.bin");pack(np.ones((N,K),dtype=f4)).tofile("input/b.bin")
groups=np.arange(SK,dtype=np.uint8)
base_as=np.broadcast_to(126+groups%3,(M,SK)).copy()
base_bs=np.broadcast_to((126+groups%3)[:,None],(SK,N)).copy()
test_as=(126+(np.arange(M,dtype=np.uint8)[:,None]//16+2*groups[None,:])%3).astype(np.uint8)
test_bs=(126+(2*groups[:,None]+np.arange(N,dtype=np.uint8)[None,:]//16)%3).astype(np.uint8)
base_as.tofile("input/base_as.bin");base_bs.T.tofile("input/base_bs.bin")
test_as.tofile("input/test_as.bin");test_bs.T.tofile("input/test_bs.bin")
