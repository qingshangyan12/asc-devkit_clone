# PR Document Translation Skill

## Function Description

Detects markdown documents in the user's local repository and automatically translates them into English. It supports:
- All markdown documents outside the docs directory
- Markdown documents in the docs directory that are not in the exclusion list

## Trigger Conditions
- User specifies changes in the local repository
- Markdown document has no corresponding English translation document (`_en.md`), or has document changes

## Execution Flow

### 1. Scan All Markdown Documents and Collect Statistics

Scan all `.md` documents in the user's local repository (excluding the .git directory). You must count files in excluded paths first, then calculate the actual number of documents to translate.

**Scanning Flow:**

1. **Count total markdown files**
   ```bash
   find . -name "*.md" -not -path "./.git/*" -type f | wc -l
   ```

2. **Read exclusion list configuration**
   ```bash
   current_path=$(pwd) 
   cat ${current_path}\exclude_docs.json
   ```

3. **Count files in excluded paths** (avoid misleading users)
   ```bash
   # Count files in each excluded path
   find . -name "*.md" -not -path "./.git/*" -path "*/docs/dflow_api/*" | wc -l
   find . -name "*.md" -not -path "./.git/*" -path "*/docs/graph_engine_api/*" | wc -l
   # Or count all excluded paths together
   find . -name "*.md" -not -path "./.git/*" \
     -not -path "*/docs/dflow_api/*" \
     -not -path "*/docs/graph_engine_api/*" \
     -not -path "*/docs/llm_datadist_api/*" | wc -l
   ```

4. **Calculate actual files to translate**
   ```
   Actual files to translate = Total files - Files in excluded paths
   ```

**Statistics Report Format:**
Output clear statistics to the user:
```
Scan Results:
- Total markdown files: 1495
- Excluded paths (skipped): 1345
  - docs/dflow_api: 401
  - docs/graph_engine_api: 843  
  - docs/llm_datadist_api: 101
- Actual files to translate: 150
```

### 2. Read Exclusion List Configuration

Read the configuration file: `${current_path}\exclude_docs.json`

### 3. Determine Whether Document Needs Translation

For each markdown document, follow this flow:

**Step 1: Extract repository name and match exclusion configuration**
1. Get the local repository remote address
2. Extract the repository name from the remote address (remove .git suffix)
3. Search the configuration to find if repoName contains the repository name (keyword match)
   - Match found → This repository has docs path exclusion configuration
   - No match → Translate all docs documents

**Step 2: Determine whether document needs translation**
1. Non-docs directory document → Needs translation
2. Docs directory document → Check if it is in excluded paths
   - In excludeDocsPaths list → Skip
   - Not in excluded paths list → Needs translation
3. Translation file exists → Check for changes
   - Has changes → Needs incremental translation
   - No changes → Skip
4. No translation file exists → Needs full translation

> For detailed flow and examples, see [Q1: How to Determine Whether Document Needs Translation](#q1-how-to-determine-whether-document-needs-translation)

**Important Notes:**
- The `excludeDocsPaths` configuration means: these docs sub-paths are already maintained in public repositories and do not need translation in this repository
- Documents in other docs paths still need translation
- Repository name matching uses **keyword matching** (for example, `ge_en_test` matches `ge`)

### 4. Determine Translation File Storage Path

**Non-docs directory documents:**
- Storage rule: Same directory as the original file
- File naming: `<original_filename>_en.md`
- Example: `README.md` → `README_en.md`

**Docs directory documents (not in exclusion list):**
- **README.md in docs root directory**: Store in the same directory, named `README_en.md`
  - Example: `docs/README.md` → `docs/README_en.md`
- **Other docs documents**: Store in `docs/en/` directory, maintaining the original directory hierarchy
  - File naming: `<original_filename>_en.md`
  - Example: `docs/api/quantize.md` → `docs/en/api/quantize_en.md`
  - Example: `docs/api/README.md` → `docs/en/api/README_en.md`

**PR template files (special rule):**
- Identification rule: Filename contains `PULL_REQUEST_TEMPLATE` and ends with a language code (for example, `.zh-CN.md`)
- Storage rule: Same directory as the original file (`.gitcode/` directory)
- File naming: Replace the language code with the corresponding English code, do not use `_en` suffix
- Examples:
  - `.gitcode/PULL_REQUEST_TEMPLATE.zh-CN.md` → `.gitcode/PULL_REQUEST_TEMPLATE.en-US.md`
  - Other platform templates: `ISSUE_TEMPLATE.zh-CN.md` → `ISSUE_TEMPLATE.en-US.md`

> For detailed path descriptions and examples, see [Q2: How to Determine Translation File Storage Path](#q2-how-to-determine-translation-file-storage-path)

### 5. Check If Translation File Exists

Check whether the corresponding translation file exists based on the document path type.

> For detailed checking methods and examples, see [Q4: How to Check If Translation File Exists](#q4-how-to-check-if-translation-file-exists)

### 6. Check Document Changes

If the translation file exists, check whether the original document has changes.

> For detailed checking methods, see [Q5: How to Detect Document Changes](#q5-how-to-detect-document-changes)

### 7. Read Translation Standards (Must Execute)

> **⚠️ Important Warning: You must read the translation standards before translating anything! Missing this step will result in non-compliant translations that require rework!**

**Before translating any content**, read the key content of the English style guide at `https://developers.google.com/style`.

#### Translation Standards Key Points (Must Follow)

**1. Voice Standards**

| Standard | Requirement | Example |
|----------|-------------|---------|
| Prefer active voice | Use active voice primarily for user-facing content | ❌ "Designed for..." → ✅ "This guide provides..." |
| Use imperative mood for operations | Omit "you" in operation steps, start directly with a verb | ❌ "You can enter the password" → ✅ "Enter the password" |
| Passive voice exceptions | Use passive voice when the actor is unknown/irrelevant, or to avoid blaming users in error messages | "The dialog box is displayed" |

**2. Tense Standards**

| Scenario | Tense to Use | Example |
|----------|--------------|---------|
| State facts/principles/mechanisms | Simple present | "A ping command sends packets to test connectivity." |
| Instant results after operations | Simple present | ❌ "The dialog box will appear" → ✅ "The dialog box appears" |
| Results requiring longer intervals | Future tense | "The system will restart after installation." |
| Completed actions | Present perfect | "You have successfully logged in." |

**3. Vocabulary Standards**

| Do Not Use | Correct Usage |
|------------|---------------|
| etc. | and so on (with limited scope) |
| e.g. | for example |
| i.e. | that is |
| via | through / by / using |
| can't, it's, don't | cannot, it is, do not |
| you're, they're | you are, they are |
| won't, shouldn't | will not, should not |

**4. Sentence and Paragraph Standards**

| Standard | Requirement |
|----------|-------------|
| Place important information first | Put key information at the beginning of sentences or paragraphs |
| Avoid overly long sentences | Each sentence should not exceed 25 words |
| Use parallel structure | Use consistent sentence patterns for similar descriptions |
| Avoid double negatives | Use direct statements |

**5. Good English Style Must Meet**

- Use consistent terminology
- Use simple vocabulary
- Define abbreviations (when first appearing)
- Prefer active voice
- Prefer simple present tense
- Use parallel structure
- Use second person (imperative mood for operations)
- Organize information clearly and correctly

**6. Good English Style Must Avoid**

- Dangling prepositions
- Unnecessary repetition or redundancy
- Foreign words (etc, e.g., i.e., via)
- Outdated vocabulary (thus, hereinafter, hence)
- Colloquial vocabulary (figure out)
- Word abbreviations (char, config)
- Contractions (can't, it's)

### 8. Execute Translation

**No translation file exists (full translation):**
1. Use the read tool to read the complete content of the Chinese document
2. Perform full translation following the translation standards
3. Determine the storage location based on the document path type and create the translation file

**Translation file exists and has changes (incremental translation):**
1. Use the bash tool to get the changes in the Chinese document (diff)
2. Analyze the changes
3. Translate the changes into English
4. Use the edit tool to update the corresponding English document

> For detailed processing methods, see [Q8: How to Handle Existing Translation Files](#q8-how-to-handle-existing-translation-files)

### 9. Use todowrite to Track Progress

Create a task list to track translation progress, **must include statistics step** (avoid misleading users).

**Example (correct statistics flow):**
```
[
  {"content": "Scan all markdown documents and count total", "status": "in_progress", "priority": "high"},
  {"content": "Count files in excluded paths (docs/dflow_api, docs/graph_engine_api, etc.)", "status": "pending", "priority": "high"},
  {"content": "Calculate actual files to translate (total - excluded)", "status": "pending", "priority": "high"},
  {"content": "Read translation standards document (must execute)", "status": "pending", "priority": "high"},
  {"content": "Translate root directory core documents (README/CONTRIBUTING/SECURITY, etc.)", "status": "pending", "priority": "high"},
  {"content": "Translate docs/architecture/ directory (non-excluded paths)", "status": "pending", "priority": "high"},
  {"content": "Translate docs/build.md and other core documents (non-excluded paths)", "status": "pending", "priority": "high"},
  {"content": "Commit translation changes to remote repository", "status": "pending", "priority": "high"}
]
```

**Statistics Report Example:**
After scanning completes, output clear statistics to the user:
```
Scan Results:
- Total markdown files: 1495
- Excluded paths (skip translation): 1345
  - docs/dflow_api: 401
  - docs/graph_engine_api: 843  
  - docs/llm_datadist_api: 101
- Actual files to translate: 150
- Existing translation files: 3 (README_en.md, etc.)
- Files pending translation: 147
```

### 10. Commit Changes to Remote Repository

After translation completes, commit all changed Chinese and English markdown documents to the remote repository.

**Commit Flow:**
1. View all changed markdown documents
2. Add changed files to staging area
3. Create commit record
4. Push to remote repository

> For detailed commit methods, see [Q9: How to Commit Translation Changes to Remote Repository](#q9-how-to-commit-translation-changes-to-remote-repository)

## Configuration File Description

### Excluded Document Configuration
- **Path**: `${current_path}\exclude_docs.json`
- **Purpose**: Define public product documentation repositories and their docs sub-paths to exclude from translation
- **Format**: JSON format, contains a mapping array, each element includes:
  - `repoName`: Repository name (for matching)
  - `publicRepo`: Complete public repository address
  - `excludeDocsPaths`: List of docs sub-paths to skip translation
- **Matching Rules**:
  1. Get the local repository remote address
  2. Extract repository name from remote address
  3. Search for repoName match in configuration
  4. If match found, skip translation for sub-paths listed in excludeDocsPaths under the docs directory of that repository

> For detailed checking methods, see [Q3: How to Check Exclusion List](#q3-how-to-check-exclusion-list)

### Translation Standards Document
- **Main Document**: https://developers.google.com/style

## Directory Structure Example

```
Repository Root/
├── README.md                  # Original Chinese document (non-docs)
├── README_en.md               # Translated English document (same directory)
├── CONTRIBUTING.md            # Original Chinese document (non-docs)
├── CONTRIBUTING_en.md         # Translated English document (same directory)
├── docs/                      # docs directory
│   ├── README.md              # Original document (README.md in docs root)
│   ├── README_en.md           # Translated English document (same directory)
│   ├── api/
│   │   ├── README.md          # Original document (README.md in docs subdirectory)
│   │   └── quantize.md        # Original document
│   ├── build.md               # Original document
│   ├── guide/
│   │   └── install.md         # Original document
│   ├── public/                # Exclusion list (public product docs, no translation)
│   │   └── guide.md
│   └── en/                    # Unified translation file directory (maintain original directory hierarchy)
│       ├── api/
│       │   ├── README_en.md   # Translated English document
│       │   └── quantize_en.md # Translated English document
│       ├── build_en.md        # Translated English document
│       └── guide/
│           └── install_en.md  # Translated English document
├── examples/
│   └── README.md              # Original Chinese document (non-docs)
│   └── README_en.md           # Translated English document (same directory)
└── npu_ops/
    └── README.md              # Original Chinese document (non-docs)
    └── README_en.md           # Translated English document (same directory)
```

## Tool Usage Recommendations

### Parallel Operations for Efficiency
- Read multiple related files in parallel (Chinese original, English version, style guide, etc.)
- Execute multiple bash commands in parallel to get different information

## Important Notes

1. **Scan Scope and Statistics (Important)**:
   - Scan all markdown documents, including docs and non-docs directories
   - **Must count files in excluded paths first**, then calculate actual files to translate
   - Output clear statistics to users (total, excluded, actual to translate)
   - Avoid misleading users into thinking all files need translation
2. **Exclusion List Judgment**: Documents in the docs directory need to check if they are in the exclusion list
3. **Must Read Translation Standards First**: Read the English style guide before translating any content
4. **File Storage Path**: Non-docs directory files in same level as original; README.md in docs root in same level; other docs documents in `docs/en/` maintaining original hierarchy
5. **File Naming Rule**: Translation files are uniformly named `<original_filename>_en.md`
6. **Incremental Update**: When translation file exists, only translate changed parts, avoid duplicate translation
7. **Format Consistency**: Maintain markdown format, table structure, link references, etc.
8. **Commit Changes**: After translation, you must commit all changed Chinese and English markdown documents to the remote repository

## Common Issues

### Q1: How to Determine Whether Document Needs Translation

**Judgment Flow:**

```
1. Does the document path start with docs/?
   → No: Needs translation (non-docs directory document)
   → Yes: Continue to check exclusion list

2. Extract repository name from local repository remote address
   → Example: https://gitcode.com/sophia1213/ge_en_test.git → Extract repository name "ge"

3. Search for repoName match in exclude_docs.json
   → No match: Needs translation (repository not in exclusion list)
   → Match found: Continue to check if document path is in excludeDocsPaths

4. Is document path in excludeDocsPaths list?
   → Yes: Skip (documents in this path are maintained in public repository, no translation needed)
   → No: Needs translation

5. Does corresponding translation file exist?
   → No: Needs full translation
   → Yes: Check for changes

6. Does document have changes?
   → Yes: Needs incremental translation update
   → No: Skip
```

**Check Commands:**
```bash
# 1. Count total markdown documents (exclude .git directory)
find . -name "*.md" -not -path "./.git/*" -type f | wc -l

# 2. Count files in excluded paths (important: avoid misleading users)
# Count single excluded path
find . -name "*.md" -not -path "./.git/*" -path "*/docs/dflow_api/*" | wc -l

# Count all excluded paths
find . -name "*.md" -not -path "./.git/*" \
  -not -path "*/docs/dflow_api/*" \
  -not -path "*/docs/graph_engine_api/*" \
  -not -path "*/docs/llm_datadist_api/*" | wc -l

# 3. Check if document is in docs directory
echo $filepath | grep "^./docs/"

# 4. Get local repository remote address
git remote -v

# 5. Read exclusion list configuration
current_path=$(pwd) 
cat ${current_path}\exclude_docs.json
```

**Example Scenarios:**

| Local Repository Remote Address | Extracted Repo Name | repoName Match in Config | docs Path | excludeDocsPaths | Translate? |
|--------------------------------|---------------------|---------------------------|-----------|------------------|------------|
| `https://gitcode.com/sophia1213/ge.git` | ge | Match | `docs/dflow_api/guide.md` | `["docs/dflow_api", "docs/graph_engine_api"]` | Skip (in excluded path) |
| `https://gitcode.com/sophia1213/ge.git` | ge | Match | `docs/architecture/feature.md` | `["docs/dflow_api", "docs/graph_engine_api"]` | Translate (not in excluded path) |
| `https://gitcode.com/user/pypto.git` | pypto | Match | `docs/api/guide.md` | `["docs"]` | Skip (entire docs directory excluded) |
| `https://gitcode.com/user/custom.git` | custom | No match | `docs/any.md` | - | Translate (not in exclusion list) |

### Q2: How to Determine Translation File Storage Path

**Rules:**
- **Non-docs directory**: Translation file in same directory as original file
- **README.md in docs root directory**: Translation file in same directory as original file
- **Other documents in docs directory**: Translation file in `docs/en/` directory, maintaining original directory hierarchy
- **PR template files**: Translation file in same directory as original file, language code replacement (for example, `.zh-CN.md` → `.en-US.md`)

**Examples:**

| Document Type | Original Document Path | Translated English Document Path |
|--------------|------------------------|----------------------------------|
| Non-docs directory | `README.md` | `README_en.md` |
| Non-docs directory | `examples/README.md` | `examples/README_en.md` |
| docs root README.md | `docs/README.md` | `docs/README_en.md` |
| docs subdirectory README.md | `docs/api/README.md` | `docs/en/api/README_en.md` |
| Other docs documents | `docs/api/quantize.md` | `docs/en/api/quantize_en.md` |
| Other docs documents | `docs/guide/install.md` | `docs/en/guide/install_en.md` |
| Other docs documents | `docs/build.md` | `docs/en/build_en.md` |
| PR template file | `.gitcode/PULL_REQUEST_TEMPLATE.zh-CN.md` | `.gitcode/PULL_REQUEST_TEMPLATE.en-US.md` |
| Issue template | `.gitcode/ISSUE_TEMPLATE.zh-CN.md` | `.gitcode/ISSUE_TEMPLATE.en-US.md` |

**Why is README.md in docs root stored in same directory?**
- docs/README.md is the entry document for the docs directory, needs to be in same directory as original for easy access
- README.md and other documents in docs subdirectories are stored in docs/en/ directory for unified management
- Maintains clear directory structure, avoids confusion between original and translated documents

### Q3: How to Check Exclusion List

**Check Steps:**

1. **Read exclusion list configuration file**
```bash
current_path=$(pwd) 
cat ${current_path}\exclude_docs.json
```

2. **Get local repository remote address and extract repository name**
```bash
# Get remote address
git remote -v

# Extract repository name examples:
# https://gitcode.com/sophia1213/ge_en_test.git → ge
# https://gitcode.com/cann/pypto.git → pypto
# https://gitcode.com/user/custom_repo.git → custom_repo
```

3. **Matching Logic**
   - Search for repoName field in the mapping array of the configuration file
   - If repoName matches local repository name:
     - Check if document path is in excludeDocsPaths list
     - In excluded path: Skip translation
     - Not in excluded path: Needs translation
   - If repoName does not match: Needs translation (all documents in this repository)

**Configuration File Format:**
```json
{
    "mapping": [
        {
            "repoName": "ge",
            "publicRepo": "https://gitcode.com/cann/ge",
            "excludeDocsPaths": ["docs/dflow_api", "docs/graph_engine_api", "docs/llm_datadist_api"]
        },
        {
            "repoName": "pypto",
            "publicRepo": "https://gitcode.com/cann/pypto",
            "excludeDocsPaths": ["docs"]
        }
    ]
}
```

**Matching Examples:**

| Local Repo Name | Config repoName | excludeDocsPaths | docs Document Path | Skip? |
|-----------------|-----------------|------------------|-------------------|-------|
| ge | ge | `["docs/dflow_api"]` | `docs/dflow_api/cpp/AddInvokedClosure.md` | Skip |
| ge | ge | `["docs/dflow_api"]` | `docs/architecture/architecture.md` | Translate |
| pypto | pypto | `["docs"]` | `docs/any/path.md` | Skip (entire docs excluded) |
| custom | No match | - | `docs/any.md` | Translate |

**Code Implementation Example:**
```bash
# Extract repository name from remote address
remote_url=$(git remote get-url origin)
repo_name=$(basename "$remote_url" .git)

# Check if in exclusion list
# Need to parse JSON configuration and match repoName
```

### Q4: How to Check If Translation File Exists

**Non-docs directory documents:**
```bash
# Check if _en.md file exists in same directory
ls README_en.md
ls examples/README_en.md
ls npu_ops/guide_en.md
```

**README.md document in docs root directory:**
```bash
# Check if _en.md file exists in same directory
ls docs/README_en.md
```

**Other documents in docs directory (including subdirectory README.md):**
```bash
# Check if translation file exists in docs/en/ directory (maintain original directory hierarchy)
ls docs/en/api/README_en.md
ls docs/en/api/quantize_en.md
ls docs/en/guide/install_en.md
ls docs/en/build_en.md
```

**Check Logic:**
```bash
# Non-docs directory: Check same directory
ls <file_directory>/<filename_without_extension>_en.md

# README.md in docs root: Check same directory
ls docs/README_en.md

# Other documents in docs directory (including subdirectory README.md): Check corresponding directory hierarchy in docs/en/
ls docs/en/<original_directory_path>/<filename_without_extension>_en.md
```

### Q5: How to Detect Document Changes

**Method 1: Compare with previous commit**
```bash
git diff HEAD~1 -- README.md
git diff HEAD~1 -- docs/api/quantize.md
```

**Method 2: Check working directory status**
```bash
git status README.md
git status docs/api/quantize.md
```

**Method 3: Get change statistics**
```bash
git diff HEAD~1 --stat -- README.md
git diff HEAD~1 --stat -- docs/api/quantize.md
```

### Q6: Keep Chinese Links During Translation

For links referencing other Chinese documents, keep the original link path unchanged. Update them after the corresponding documents are translated.

### Q7: Why Must Read Translation Standards First

**Not reading translation standards will cause the following issues:**

1. **Overuse of passive voice**: Chinese commonly uses expressions like "面向..." and "设计用于...", translating directly to "Designed for..." is passive voice. You should use active voice like "This guide provides..."

2. **Use of foreign words**: Chinese commonly uses "等"、"例如"、"即", translating directly to etc, e.g., i.e. is foreign word usage. You should use and so on, for example, that is

3. **Abbreviated forms**: Using can't, it's and other contractions in translation. You should use cannot, it is

4. **Tense errors**: Using future tense "will appear" for results after operation steps. You should use simple present "appears"

**Consequence:** After translation completes, you discover non-compliance and need rework, wasting time. Therefore, you must read standards before translation.

### Q8: How to Handle Existing Translation Files

**Processing Flow:**
1. Check if original document has changes
2. If changes exist, analyze the changed content
3. Translate only the changed parts
4. Based on document path type, update the corresponding translation file:
   - **Non-docs directory**: Use edit tool to update translation file in same directory
   - **docs directory**: Use edit tool to update translation file in corresponding directory hierarchy under docs/en/

**Incremental Translation Example:**
```bash
# 1. Get changed content
git diff HEAD~1 -- README.md

# 2. Analyze changes
# 3. Translate changed parts
# 4. Use edit tool to update translation file
```

### Q9: How to Commit Translation Changes to Remote Repository

**Commit Flow:**

1. **View all changed markdown documents**
```bash
# View all changed files
git status

# View changed markdown documents (including Chinese original and English translation)
git status -- "*.md"
```

2. **Add changed files to staging area**
```bash
# Method 1: Add all markdown document changes
git add "*.md"

# Method 2: Add specific files
git add README.md README_en.md
git add docs/api/quantize.md docs/en/api/quantize_en.md
```

3. **Create commit record**
```bash
# Create commit (recommended commit message format)
git commit -m "docs: add/update English translation for markdown files"

# Or more detailed commit message
git commit -m "docs: translate README.md to README_en.md

- Add README_en.md (English translation)
- Update docs/api/quantize.md and docs/en/api/quantize_en.md"
```

4. **Push to remote repository**
```bash
# Push to remote repository
git push origin <branch_name>

# If new branch, use -u parameter
git push -u origin <branch_name>
```

**Complete Commit Example:**
```bash
# 1. View changes
git status

# 2. Add all markdown file changes
git add "*.md"

# 3. Create commit
git commit -m "docs: add English translation for markdown files"

# 4. Push to remote
git push origin main
```

**Important Notes:**
- Ensure all translation files are created or updated before committing
- Commit message should clearly describe the translation content
- Chinese original and English translation should be committed together to stay synchronized
- For multiple translation files, recommend batch commit