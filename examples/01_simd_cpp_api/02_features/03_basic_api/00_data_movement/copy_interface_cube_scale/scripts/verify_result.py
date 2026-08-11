#!/usr/bin/env python3
import argparse,numpy as np
try: import ml_dtypes
except ImportError as e: raise SystemExit("install ml_dtypes to verify bfloat16 data") from e
p=argparse.ArgumentParser();p.add_argument("--route",type=int,required=True);p.add_argument("--scenario",type=int,required=True);p.add_argument("--output",default="output/output.bin");a=p.parse_args();M=N=32;SK=4
base_as=np.fromfile("input/base_as.bin",np.uint8).reshape(M,SK)
base_bs=np.fromfile("input/base_bs.bin",np.uint8).reshape(N,SK).T
test_as=np.fromfile("input/test_as.bin",np.uint8).reshape(M,SK)
test_bs=np.fromfile("input/test_bs.bin",np.uint8).reshape(N,SK).T
final_as=base_as.copy();final_bs=base_bs.copy()
if a.route==0:
 if a.scenario==1:final_as[:]=test_as
 elif a.scenario==0:final_as[:,0:2]=test_as[:,2:4]
 else:final_as[16:32,:]=test_as[0:16,:]
else:
 if a.scenario==1:final_bs[:]=test_bs
 elif a.scenario==0:final_bs[:,0:16]=test_bs[:,16:32]
 else:final_bs[2:4,:]=test_bs[0:2,:]
golden=(32*np.sum(np.exp2(final_as.astype(np.int16)[:,:,None]-127)*np.exp2(final_bs.astype(np.int16)[None,:,:]-127),axis=1)).astype(ml_dtypes.bfloat16).astype(np.float32)
out=np.fromfile(a.output,dtype=ml_dtypes.bfloat16).astype(np.float32).reshape(M,N)
if not np.array_equal(out,golden):raise SystemExit("result mismatch")
print("test pass")
