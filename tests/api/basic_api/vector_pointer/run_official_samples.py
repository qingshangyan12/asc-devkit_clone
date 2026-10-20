"""Run the official sample projects against baseline/Tensor/pointer headers.

Sources and data scripts are copied verbatim from baseline. Pointer mode changes
only listed API operand expressions, using an overload that preserves scalars.
Every configure/build/generate/run/verify command and result is recorded.
"""
import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
from pathlib import Path

SCENARIOS = {'brcb':[0], 'cast':[0,1], 'compare':[1,2,3,4],
 'create_vec_index':[0], 'duplicate':[0], 'element_wise_arithmetic':[0],
 'element_wise_compound_compute':[1,2], 'element_wise_logic':[1,2,3,4],
 'gather':[1,2,3,4], 'interleave_pair':[1,2], 'mrg_sort':[1,2,3],
 'reduce':[0], 'reduce_computation':[1,2,3,4,5,6], 'reduce_data_block':[1,2,3],
 'reduce_pair_elem':[0], 'reduce_repeat':[1,2,3,4],
 'region_proposal_sort':[0], 'select':[1,2,3,4], 'transpose':[1,2,3]}
HELPER = '''
namespace VectorMigration {
template<class T> __aicore__ inline __ubuf__ AscendC::PrimT<T>* Ptr(AscendC::LocalTensor<T>& x) {
    return (__ubuf__ AscendC::PrimT<T>*)x.GetPhyAddr();
}
template<class T> __aicore__ inline __ubuf__ AscendC::PrimT<T>* Ptr(const AscendC::LocalTensor<T>& x) {
    return (__ubuf__ AscendC::PrimT<T>*)x.GetPhyAddr();
}
template<class T> __aicore__ inline __ubuf__ AscendC::PrimT<T>* Ptr(AscendC::LocalTensor<T>&& x) {
    return (__ubuf__ AscendC::PrimT<T>*)x.GetPhyAddr();
}
template<class T> __aicore__ inline T&& Ptr(T&& x) { return static_cast<T&&>(x); }
}
'''

def migrate(source, names):
    edits=[]
    pattern=r'AscendC::('+'|'.join(sorted(names,key=len,reverse=True))+r')(?:<[^;\n]*?>)?\s*\('
    for m in re.finditer(pattern,source):
        start=m.end(); depth=1; pos=start; arg_start=start; args=[]; square=brace=angle=0
        while pos<len(source):
            c=source[pos]
            if c=='(': depth+=1
            elif c==')':
                depth-=1
                if depth==0:
                    args.append(source[arg_start:pos]);break
            elif c=='[':square+=1
            elif c==']':square-=1
            elif c=='{':brace+=1
            elif c=='}':brace-=1
            elif c=='<':angle+=1
            elif c=='>' and angle:angle-=1
            elif c==',' and depth==1 and not(square or brace or angle):
                args.append(source[arg_start:pos]);arg_start=pos+1
            pos+=1
        mapped=[a if a.strip().startswith('{') or not a.strip() else 'VectorMigration::Ptr('+a.strip()+')' for a in args]
        edits.append((start,pos,', '.join(mapped),m.group(1)))
    for start,end,replacement,_ in reversed(edits):source=source[:start]+replacement+source[end:]
    source=source.replace('#include "kernel_operator.h"','#include "kernel_operator.h"\n'+HELPER)
    return source,[x[3] for x in edits]

def flag_for(path):
    text=path.read_text()
    m=re.search(r'["\'](-scenario(?:_num|Num)?)["\']',text)
    return m.group(1) if m else None

def main():
    ap=argparse.ArgumentParser();ap.add_argument('--root',type=Path,required=True);ap.add_argument('--baseline',type=Path,required=True);ap.add_argument('--output',type=Path,required=True);ap.add_argument('--only');ap.add_argument('--modes',default='baseline,tensor,pointer');args=ap.parse_args()
    root=args.root.resolve(); baseline=args.baseline.resolve();out=args.output.resolve();out.mkdir(parents=True,exist_ok=True)
    entries=json.loads((root/'tests/api/basic_api/vector_pointer/scope.json').read_text())['entries']
    names={e['name'] for e in entries}
    includes='\n'.join('#include "basic_api/'+h+'"' for h in sorted({e['header'] for e in entries}|{'kernel_operator_common_intf.h'}))
    sample_base=Path('examples/01_simd_cpp_api/03_basic_api/01_memory_vector_compute')
    results=[]
    for name, scenarios in SCENARIOS.items():
        if args.only and name not in args.only.split(','):continue
        for scenario in scenarios:
            for mode in args.modes.split(','):
                dest=out/f'{name}_{scenario}'/mode; project=dest/'project'; build=dest/'build'
                dest.mkdir(parents=True,exist_ok=True)
                shutil.copytree(baseline/sample_base/name,project,dirs_exist_ok=True)
                source=next(project.glob('*.asc')); original=source.read_text();calls=[]
                if mode=='pointer':
                    migrated,calls=migrate(original,names);source.write_text(migrated)
                # The September umbrella imports an unrelated SIMT printf intrinsic
                # absent in the installed July CANN 9.1 compiler. Use explicit Basic
                # API includes identically in all three variants; archive the delta.
                adapted=source.read_text().replace('#include "kernel_operator.h"',includes)
                adaptations=['umbrella replaced by scope Basic API includes for CANN 9.1']
                if 'AscendC::printf' in adapted:
                    adapted=re.sub(r'AscendC::printf\([^;]*\);','(void)0;',adapted)
                    adaptations.append('omit optional device diagnostic printf absent from scoped includes')
                if 'LocalMemAllocator' in adapted and not re.search(r'AscendC::TPipe\s+\w+',adapted):
                    adapted=adapted.replace('AscendC::InitSocState();','AscendC::InitSocState();\n    AscendC::TPipe vectorMigrationPipe;')
                    adaptations.append('initialize TPipe context before LocalMemAllocator')
                source.write_text(adapted)
                headers=baseline if mode=='baseline' else root
                inc=' '.join('-iquote'+str(headers/x) for x in ['.','include','include/basic_api','include/utils','impl','impl/basic_api'])
                with (project/'CMakeLists.txt').open('a') as f:
                    f.write('\nset(CMAKE_ASC_FLAGS "${CMAKE_ASC_FLAGS} '+inc+' -I'+str(headers)+' -std=c++17 -O2 -H")\n')
                build.mkdir(exist_ok=True)
                record={'sample':name,'scenario':scenario,'mode':mode,'migrated_calls':calls,'sample_adaptations':adaptations,'source_sha256':hashlib.sha256(source.read_bytes()).hexdigest(),'steps':[]}
                commands=[('configure',['cmake','-S',str(project),'-B',str(build),'-DCMAKE_ASC_ARCHITECTURES=dav-3510',f'-DSCENARIO_NUM={scenario}']),('build',['cmake','--build',str(build),'-j2'])]
                gen=project/'scripts/gen_data.py';verify=project/'scripts/verify_result.py'
                gen_args=['python3','-c','import numpy as np,runpy,sys; np.random.seed(20260916); p=sys.argv.pop(1); sys.argv[0]=p; runpy.run_path(p,run_name="__main__")',str(gen)]
                if flag_for(gen):gen_args +=[flag_for(gen),str(scenario)]
                commands += [('generate',gen_args),('run',[str(build/'demo')])]
                if verify.exists():
                    verify_args=['python3',str(verify),'./output/output.bin','./output/golden.bin']
                    if name=='interleave_pair':
                        verify_args=['python3',str(verify),'./output/output_dst0.bin','./output/output_dst1.bin','./output/golden_dst0.bin','./output/golden_dst1.bin']
                    elif name=='gather':verify_args.append(str(scenario))
                    elif flag_for(verify):verify_args +=[flag_for(verify),str(scenario)]
                    commands.append(('verify',verify_args))
                else:record['verification_note']='official sample has no verify_result.py'
                for step,cmd in commands:
                    with (dest/f'{step}.log').open('w') as log:
                        try:r=subprocess.run(cmd,cwd=build,stdout=log,stderr=subprocess.STDOUT,timeout=180);rc=r.returncode
                        except subprocess.TimeoutExpired:rc=124
                    record['steps'].append({'step':step,'command':cmd,'returncode':rc})
                    if rc:break
                record['pass']=len(record['steps'])==len(commands) and all(x['returncode']==0 for x in record['steps']) and verify.exists()
                (dest/'result.json').write_text(json.dumps(record,indent=2))
                results.append(record);(out/'summary.json').write_text(json.dumps(results,indent=2))
                print(json.dumps({k:record[k] for k in ['sample','scenario','mode','pass']},ensure_ascii=False),flush=True)

if __name__=='__main__':main()
