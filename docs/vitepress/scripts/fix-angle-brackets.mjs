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

function fixTableCell(text) {
  // Remove inline code spans first
  const parts = [];
  let remaining = text;
  const codeRegex = /(`+)([\s\S]*?)\1/g;
  let lastIndex = 0;
  let match;

  while ((match = codeRegex.exec(remaining)) !== null) {
    parts.push({ type: 'text', content: remaining.substring(lastIndex, match.index) });
    parts.push({ type: 'code', content: match[0] });
    lastIndex = match.index + match[0].length;
  }
  parts.push({ type: 'text', content: remaining.substring(lastIndex) });

  return parts.map(p => {
    if (p.type === 'code') return p.content;
    // Escape < and > with backslash in markdown text
    return p.content.replace(/</g, '\\<').replace(/>/g, '\\>');
  }).join('');
}

function fixFile(filePath) {
  const lines = readFileSync(filePath, 'utf-8').split('\n');
  let inTable = false;
  let changed = false;

  for (let i = 0; i < lines.length; i++) {
    const line = lines[i];
    if (line.startsWith('|') && line.includes('|')) {
      inTable = true;
      // Only process data rows (not header separator --- | ---)
      if (!/^\|\s*[-:]+\s*\|/.test(line)) {
        // Find the rightmost | that ends the row
        const cells = line.split('|');
        // Reconstruct: first element is empty (before first |), last is empty (after last |)
        const fixedCells = [];
        for (let j = 1; j < cells.length - 1; j++) {
          fixedCells.push(fixTableCell(cells[j]));
        }
        const newLine = '|' + fixedCells.join('|') + '|';
        if (newLine !== line) {
          lines[i] = newLine;
          changed = true;
        }
      }
    } else if (line.trim() && !line.startsWith('|')) {
      inTable = false;
    }
  }

  if (changed) {
    writeFileSync(filePath, lines.join('\n'), 'utf-8');
    return true;
  }
  return false;
}

const files = walkDir('docs/guide');
let count = 0;

files.forEach(f => {
  if (fixFile(f)) {
    console.log(`Fixed: ${f}`);
    count++;
  }
});

console.log(`\nTotal files fixed: ${count}`);
