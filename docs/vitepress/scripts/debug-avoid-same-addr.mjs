import { readFileSync, writeFileSync } from 'fs';

const f = 'docs/guide/算子实践参考/SIMD算子性能优化/内存访问/避免同地址访问.md';
let c = readFileSync(f, 'utf-8');

// Try: keep everything except the second table (lines 40-46 area)
const lines = c.split('\n');

// Find "修改切分策略" section and keep only content before it
const idx = lines.findIndex(l => l.startsWith('**修改切分策略**'));
if (idx >= 0) {
  c = lines.slice(0, idx).join('\n');
  writeFileSync(f, c);
  console.log(`Kept ${idx} lines, removed from "修改切分策略" onwards`);
} else {
  console.log('Not found');
}
