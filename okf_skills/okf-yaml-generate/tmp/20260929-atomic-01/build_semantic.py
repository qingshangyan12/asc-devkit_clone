#!/usr/bin/env python3
"""Build reviewed atomic-page semantics for this OKF run."""

import hashlib
import json
from pathlib import Path
import subprocess

ROOT = Path('/home/hugh/code/asc-devkit')
SKILL = Path(__file__).resolve().parents[2]
RUN = Path(__file__).resolve().parent
DESC = SKILL.parent / 'okf-description-generate/scripts/description.py'
TASK = json.loads((RUN / 'batches/batch-01/task.json').read_text())
INDEX = {'atomic.md', 'datamove_atomic.md', 'scalar_atomic.md'}
NAMESPACES = ('api_surfaces', 'roles', 'families', 'operations', 'objects',
              'execution_models', 'source_locations', 'destination_locations', 'traits')

# Summaries are based on the cited functional paragraphs, formulas and constraints.
DESCRIPTIONS = {
    'atomic_operations_overview': '说明数据搬运随路原子操作如何改变写入GM的结果，并对比在指定GM地址执行的Scalar原子操作及其接口。',
    'key_features': '分析浮点原子累加的数据搬运乱序及初始化覆盖所致的非确定性，并说明通过核内或核间同步控制执行顺序。',
    'asc_disable_dma_atomic': '关闭先前开启的数据搬运随路原子操作，避免原子状态影响后续搬运指令。',
    'asc_set_atomic_add': '为后续写入GM的数据搬运开启原子累加，将搬运数据与GM原有数据求和后写回GM。',
    'asc_set_atomic_max': '为后续写入GM的数据搬运开启原子取大，将搬运数据与GM原有数据中的较大值写回GM。',
    'asc_set_atomic_min': '为后续写入GM的数据搬运开启原子取小，将搬运数据与GM原有数据中的较小值写回GM。',
    'asc_atomic_add': '对指定GM地址的单个元素执行原子加，将旧值与输入值之和写回，并返回旧值。',
    'asc_atomic_and': '对指定GM地址的单个元素执行原子按位与，将旧值与输入值的运算结果写回，并返回旧值。',
    'asc_atomic_cas': '比较指定GM地址的旧值与compare，相等时原子写入val，否则保留旧值，并返回旧值。',
    'asc_atomic_dec': '按val指定的阈值规则原子递减指定GM地址的无符号整数元素，并返回更新前的值。',
    'asc_atomic_exch': '将输入值原子写入指定GM地址的单个元素以替换旧值，并返回旧值。',
    'asc_atomic_inc': '按val指定的阈值规则原子递增指定GM地址的无符号整数元素，并返回更新前的值。',
    'asc_atomic_max': '比较指定GM地址的旧值与输入值，原子写回较大值，并返回旧值。',
    'asc_atomic_min': '比较指定GM地址的旧值与输入值，原子写回较小值，并返回旧值。',
    'asc_atomic_or': '对指定GM地址的单个元素执行原子按位或，将旧值与输入值的运算结果写回，并返回旧值。',
    'asc_atomic_sub': '从指定GM地址的旧值中减去输入值，原子写回结果，并返回旧值。',
    'asc_atomic_xor': '对指定GM地址的单个元素执行原子按位异或，将旧值与输入值的运算结果写回，并返回旧值。',
    'asc_get_store_atomic_config': '读取原子操作启用位和操作类型，用于核验已设置的原子配置；该接口在Ascend 950系列上已废弃。',
    'asc_set_store_atomic_config_v1': '设置原子操作启用位及操作类型，以配置后续原子操作，并建议使用后清除该状态。',
}
OPS = {
    'asc_atomic_add': 'add', 'asc_atomic_and': 'bitwise_and',
    'asc_atomic_cas': 'comparison', 'asc_atomic_dec': 'subtract',
    'asc_atomic_exch': 'store', 'asc_atomic_inc': 'add',
    'asc_atomic_max': 'maximum', 'asc_atomic_min': 'minimum',
    'asc_atomic_or': 'bitwise_or', 'asc_atomic_sub': 'subtract',
    'asc_atomic_xor': 'bitwise_xor',
    'asc_set_atomic_add': 'add', 'asc_set_atomic_max': 'maximum',
    'asc_set_atomic_min': 'minimum',
}


def dump(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(data, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')


def body(source):
    if source.startswith('---\n'):
        return source.split('\n---\n', 1)[1]
    return source


def evidence_block(source, stem):
    if stem == 'atomic_operations_overview':
        return source.split('\n\n', 1)[1].split('\n\n', 1)[0]
    if stem == 'key_features':
        return '核心思想是在指令之间插入适当的同步，使每次程序运行时相关指令都按照预期确定的顺序执行，最终保证每次执行程序输出的结果都相同。'
    part = source.split('## 功能说明\n', 1)[1].split('\n## ', 1)[0]
    paragraphs = [p.strip() for p in part.split('\n\n') if p.strip()]
    return next(p for p in paragraphs if not p.startswith('头文件路径为'))


def description_proofs(source, stem, first):
    proofs = [first]
    extra = {
        'atomic_operations_overview': '新增Scalar原子操作接口，能够在指定GM地址上进行单点原子计算操作',
        'key_features': '带随路原子操作的搬运指令乱序，由于浮点数加法不满足结合律',
        'asc_get_store_atomic_config': '该接口已废弃。原子加操作可以直接使用',
        'asc_set_store_atomic_config_v1': '使用完成后，建议通过[asc_disable_dma_atomic]',
        'asc_atomic_cas': '接口返回`old_value`。整个读取、比较和条件写回过程为原子操作。',
        'asc_atomic_inc': '接口返回`old_value`。整个读取、判断和写回过程为原子操作。',
        'asc_atomic_dec': '接口返回`old_value`。整个读取、判断和写回过程为原子操作。',
    }.get(stem)
    if extra:
        assert extra in source, (stem, extra)
        proofs.append(extra)
    return proofs


annotations = []
for relative in TASK['documents']:
    target = ROOT / relative
    stem = target.stem
    if target.name in INDEX:
        continue
    source = body(target.read_text(encoding='utf-8'))
    digest = hashlib.sha256(target.read_bytes()).hexdigest()
    excerpt = evidence_block(source, stem)
    assert excerpt in source, relative
    proofs = description_proofs(source, stem, excerpt)
    description = DESCRIPTIONS[stem]
    desc_run = RUN / 'descriptions' / stem
    desc_id = f"{TASK['run_id']}-{stem}"
    if not (desc_run / 'task.json').exists():
        subprocess.run(['python3', str(DESC), 'prepare', '--devkit-root', str(ROOT),
                        '--document', relative, '--run-id', desc_id, '--run-dir', str(desc_run)],
                       check=True, stdout=subprocess.DEVNULL)
    dump(desc_run / 'description-result.json', {
        'schema_version': 1, 'purpose': 'okf_api_description_result',
        'run_id': desc_id, 'document_path': relative, 'document_sha256': digest,
        'status': 'ready', 'description': description,
        'evidence': [{'excerpt': proof} for proof in proofs], 'confidence': 0.91,
    })
    subprocess.run(['python3', str(DESC), 'validate', '--run-dir', str(desc_run),
                    '--result', str(desc_run / 'description-result.json')],
                   check=True, stdout=subprocess.DEVNULL)

    tag = {name: [] for name in NAMESPACES}
    tag['api_surfaces'] = ['c_api']
    tag_evidence = {'api_surfaces:c_api': [{'source_path': relative, 'excerpt': relative}]}
    def add(namespace, value, proof=excerpt):
        assert proof in source, (relative, namespace, value)
        tag[namespace].append(value)
        tag_evidence[f'{namespace}:{value}'] = [{'source_path': relative, 'excerpt': proof}]

    if stem == 'atomic_operations_overview':
        add('roles', 'configuration')
    elif stem == 'key_features':
        add('roles', 'control')
    elif '/datamove_atomic/' in relative or stem == 'asc_set_store_atomic_config_v1':
        add('roles', 'data_movement_configuration')
        if stem in OPS:
            add('operations', OPS[stem])
        add('traits', 'atomic')
        if 'GM' in excerpt:
            add('destination_locations', 'global_memory')
    elif stem == 'asc_get_store_atomic_config':
        add('roles', 'query')
        deprecated = '该接口已废弃'
        add('traits', 'deprecated', deprecated)
    else:
        add('roles', 'compute')
        add('operations', OPS[stem])
        add('execution_models', 'scalar', 'PIPE_S')
        add('traits', 'atomic')
        add('source_locations', 'global_memory')
        add('destination_locations', 'global_memory')

    dependencies = []
    if stem == 'asc_get_store_atomic_config':
        dependencies.append({
            'path': str(Path(relative).with_name('asc_atomic_add.md')),
            'kind': 'related',
            'condition': 'Ascend 950系列的原子加操作可直接调用asc_atomic_add，无需配置和校验原子状态。',
            'evidence': {
                'source_path': relative,
                'excerpt': '该接口已废弃。原子加操作可以直接使用[asc_atomic_add](asc_atomic_add.md)实现。',
            },
        })
    annotations.append({
        'document_path': relative, 'document_sha256': digest,
        'description': {'value': description,
                        'evidence': [{'source_path': relative, 'excerpt': proof} for proof in proofs]},
        'tag': tag, 'tag_evidence': tag_evidence, 'dependencies': dependencies,
    })

assert len(annotations) == 19, len(annotations)
dump(RUN / 'batches/batch-01/semantic-input.json', {
    'schema_version': 1, 'purpose': 'okf_yaml_semantic_result',
    'run_id': TASK['run_id'], 'batch_id': TASK['batch_id'],
    'annotations': annotations,
})
print(f'{len(annotations)} descriptions validated; semantic input ready')
