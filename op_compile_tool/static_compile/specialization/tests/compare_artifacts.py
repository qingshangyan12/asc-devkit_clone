"""比较真实的 JIT/OPC 产物对；原始字节相等与公共元数据比较分开处理。"""

import argparse
import hashlib
import json
from pathlib import Path

COMMON_FIELDS = (
    'binFileName',
    'binFileSuffix',
    'blockDim',
    'workspace',
    'opParaSize',
    'localMemorySize',
    'schedule_mode',
    'coreType',
    'core_type',
    'magic',
    'intercoreSync',
    'taskRation',
    'parameters',
    'supportSuperKernel',
    'platformInfo',
    'supportInfo',
)


def compare_artifacts(jit_json, opc_json):
    jit_json, opc_json = Path(jit_json), Path(opc_json)
    jit, opc = json.loads(jit_json.read_text()), json.loads(opc_json.read_text())
    jit_o, opc_o = jit_json.with_suffix('.o').read_bytes(), opc_json.with_suffix('.o').read_bytes()
    for label, value, data in (('jit', jit, jit_o), ('opc', opc, opc_o)):
        if not data.startswith(b'\x7fELF') or hashlib.sha256(data).hexdigest() != value['sha256']:
            raise ValueError(label + ' ELF/JSON integrity mismatch')
    differences = {}
    for key in sorted(jit.keys() | opc.keys()):
        values_match = jit.get(key) == opc.get(key)
        presence_matches = (key in jit) == (key in opc)
        if values_match and presence_matches:
            continue
        differences[key] = {
            'jit': jit.get(key),
            'opc': opc.get(key),
            'jit_present': key in jit,
            'opc_present': key in opc,
        }
    return {
        'jit_json': str(jit_json),
        'opc_json': str(opc_json),
        'o_byte_equal': jit_o == opc_o,
        'json_byte_equal': jit_json.read_bytes() == opc_json.read_bytes(),
        'json_value_equal': jit == opc,
        'jit_o_bytes': len(jit_o),
        'opc_o_bytes': len(opc_o),
        'jit_sha256': hashlib.sha256(jit_o).hexdigest(),
        'opc_sha256': hashlib.sha256(opc_o).hexdigest(),
        'common_fields_equal': all(jit.get(k) == opc.get(k) for k in COMMON_FIELDS),
        'common_fields_checked': list(COMMON_FIELDS),
        'differences': differences,
    }


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--jit', required=True)
    parser.add_argument('--opc', required=True)
    parser.add_argument('--report', required=True)
    args = parser.parse_args()
    result = compare_artifacts(args.jit, args.opc)
    Path(args.report).write_text(json.dumps(result, indent=2))
    print(json.dumps({k: v for k, v in result.items() if k not in ('differences', 'common_fields_checked')}, indent=2))
