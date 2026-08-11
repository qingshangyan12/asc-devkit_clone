#!/usr/bin/env python3
import argparse,numpy as np
D=32;p=argparse.ArgumentParser();p.add_argument("--route",type=int,required=True);p.add_argument("--scenario",type=int,required=True);p.add_argument("--quant-mode",type=int,default=0);p.add_argument("--output",default="output/output.bin");a=p.parse_args()
idt=np.float16 if a.quant_mode==0 else np.int8;odt=np.float16 if a.route==2 or a.quant_mode!=0 else np.float32
x=np.fromfile("input/a.bin",idt).reshape(D,D).astype(np.float32);y=np.fromfile("input/b.bin",idt).reshape(D,D).T.astype(np.float32);result=x@y
qscale=np.concatenate((np.ones(D//2,dtype=np.float32),np.full(D//2,0.5,dtype=np.float32)))
golden=np.full((D,D),-17,dtype=odt)
if a.route==1:
    if a.scenario==0:golden[:,0:16]=result[:,16:32].astype(odt)
    else:golden[16:32,:]=result[0:16,:].astype(odt)
elif a.scenario==1:golden[:]=(result*qscale if a.quant_mode==2 else result).astype(odt)
elif a.scenario==0:
    region=result[:,16:32]
    golden[:,0:16]=(region*qscale[:16] if a.quant_mode==2 else region).astype(odt)
else:
    region=result[0:16,:]
    golden[16:32,:]=(region*qscale if a.quant_mode==2 else region).astype(odt)
if a.route==2:golden=golden.reshape(D,D//16,16).transpose(1,0,2).reshape(-1)
out=np.fromfile(a.output,odt).reshape(golden.shape)
if not np.allclose(out,golden,rtol=2e-3,atol=2e-3):raise SystemExit("result mismatch")
print("test pass")
