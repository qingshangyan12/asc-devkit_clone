import { readFileSync, writeFileSync, readdirSync, statSync } from 'fs';
import { join, extname } from 'path';

function walkDir(dir) {
  const results = [];
  const items = readdirSync(dir);
  for (const item of items) {
    const fullPath = join(dir, item);
    const stat = statSync(fullPath);
    if (stat.isDirectory()) {
      results.push(...walkDir(fullPath));
    } else if (stat.isFile() && extname(fullPath) === '.md') {
      results.push(fullPath);
    }
  }
  return results;
}

function fixSeparators(filePath) {
  const original = readFileSync(filePath, 'utf-8');
  const content = original.replace(
    /^(\|[\s\-:]+\|[\s\-:\|]*)$/gm,
    (line) => {
      const parts = line.split('|');
      const trimmed = parts.filter((p, i) => i > 0 && i < parts.length - 1);
      const newParts = trimmed.map(p => {
        const core = p.trim();
        if (/^:?-+:?$/.test(core)) {
          const hasLeft = core.startsWith(':');
          const hasRight = core.endsWith(':');
          const dash = hasLeft ? ':' : '-';
          const end = hasRight ? ':' : '-';
          return dash + dash + end;
        }
        return core;
      });
      return '| ' + newParts.join(' | ') + ' |';
    }
  );

  if (content !== original) {
    writeFileSync(filePath, content, 'utf-8');
    return true;
  }
  return false;
}

const files = walkDir('docs/guide');
let count = 0;

files.forEach(f => {
  if (fixSeparators(f)) {
    console.log(`Fixed: ${f}`);
    count++;
  }
});

console.log(`\nTotal files fixed: ${count}`);
