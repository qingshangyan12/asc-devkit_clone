import { readFileSync, writeFileSync } from 'fs';

const f = 'docs/guide/算子实践参考/SIMD算子性能优化/内存访问/避免UB的bank冲突/避免bank冲突（NPU架构版本2201）.md';
let c = readFileSync(f, 'utf-8');
const lines = c.split('\n');

// Test: keep only content before "如何避免bank冲突" (which contains simpler tables)
const idx = lines.findIndex(l => l.startsWith('## 如何避免bank冲突'));
if (idx >= 0) {
  c = lines.slice(0, idx).join('\n');
  writeFileSync(f, c);
  console.log(`Kept lines 1 to ${idx}, removed after`);
} else {
  console.log('Not found');
}
