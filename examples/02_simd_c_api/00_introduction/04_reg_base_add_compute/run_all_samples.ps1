$ErrorActionPreference = "Stop"

$ROOT = "C:\JC\asc-devkit\examples\02_simd_c_api\00_introduction\04_reg_base_add_compute"
$LOG_DIR = Join-Path $ROOT "logs"
$TIMESTAMP = Get-Date -Format "yyyyMMdd_HHmmss"
$LOG_DIR = Join-Path $LOG_DIR $TIMESTAMP
New-Item -ItemType Directory -Path $LOG_DIR -Force | Out-Null

$SUMMARY_FILE = Join-Path $LOG_DIR "summary.txt"
$BUILD_ERR_FILE = Join-Path $LOG_DIR "build_errors.txt"
$RUN_ERR_FILE  = Join-Path $LOG_DIR "run_errors.txt"

$ARCH = if ($env:CMAKE_ASC_ARCHITECTURES) { $env:CMAKE_ASC_ARCHITECTURES } else { "dav-3510" }
$SOC_VERSION = if ($env:SOC_VERSION) { $env:SOC_VERSION } else { "Ascend910B2" }

Write-Host "================================================"
Write-Host " Run All 04_reg_base_add_compute Samples"
Write-Host " ARCH       = $ARCH"
Write-Host " SOC_VERSION= $SOC_VERSION"
Write-Host " Log Dir    = $LOG_DIR"
Write-Host "================================================"

$sampleDirs = Get-ChildItem -Path $ROOT -Recurse -Filter "CMakeLists.txt" |
    ForEach-Object { $_.DirectoryName } |
    Sort-Object -Unique

$total = $sampleDirs.Count
$pass = 0; $fail = 0; $buildFail = 0; $runFail = 0
$results = @()

"Log Timestamp: $TIMESTAMP" | Out-File $SUMMARY_FILE -Encoding utf8
"ARCH=$ARCH  SOC_VERSION=$SOC_VERSION" | Out-File $SUMMARY_FILE -Append -Encoding utf8
"Total samples: $total" | Out-File $SUMMARY_FILE -Append -Encoding utf8
"=" * 60 | Out-File $SUMMARY_FILE -Append -Encoding utf8

foreach ($idx in 0..($total - 1)) {
    $sampleDir = $sampleDirs[$idx]
    $relPath = $sampleDir.Substring($ROOT.Length + 1)
    $sampleName = Split-Path $sampleDir -Leaf

    $num = $idx + 1
    Write-Host ("[{0,3}/{1}] {2} ... " -f $num, $total, $relPath) -NoNewline

    $sampleLogDir = Join-Path $LOG_DIR $relPath
    New-Item -ItemType Directory -Path $sampleLogDir -Force | Out-Null

    $buildDir = Join-Path $sampleDir "build"
    if (Test-Path $buildDir) { Remove-Item -Recurse -Force $buildDir }

    $cmakeLog = Join-Path $sampleLogDir "cmake.log"
    $makeLog  = Join-Path $sampleLogDir "make.log"
    $runLog   = Join-Path $sampleLogDir "run.log"

    $cmakeFile = Join-Path $sampleDir "CMakeLists.txt"
    $exeName = (Select-String -Path $cmakeFile -Pattern "add_executable\((\S+)" |
        ForEach-Object { $_.Matches.Groups[1].Value }) -replace '\s',''

    if (-not $exeName) {
        Write-Host "SKIP (no executable found)"
        "$relPath : SKIP - no executable in CMakeLists.txt" | Out-File $SUMMARY_FILE -Append -Encoding utf8
        continue
    }

    # ---- Build with bash (needed for CANN cmake toolchain) ----
    $buildCmd  = "cd '$sampleDir'; rm -rf build; mkdir -p build; cd build; cmake .. -DCMAKE_ASC_ARCHITECTURES=$ARCH 2>&1; make -j 2>&1"
    $buildOk = $true
    try {
        $buildOutput = bash -lc $buildCmd 2>&1
        $buildOutput | Out-File $cmakeLog -Encoding utf8
        if ($LASTEXITCODE -ne 0) { $buildOk = $false }
    } catch {
        $buildOk = $false
        $_.Exception.Message | Out-File $cmakeLog -Encoding utf8
    }

    if (-not $buildOk) {
        Write-Host "BUILD FAIL"
        $buildFail++
        "$relPath : BUILD FAIL" | Out-File $SUMMARY_FILE -Append -Encoding utf8
        "=== $relPath ===" | Out-File $BUILD_ERR_FILE -Append -Encoding utf8
        Get-Content $cmakeLog | Out-File $BUILD_ERR_FILE -Append -Encoding utf8
        continue
    }

    # ---- Run ----
    $runOk = $true
    try {
        $runOutput = bash -lc "cd '$sampleDir/build'; soc_version=$SOC_VERSION ./$exeName 2>&1"
        $runOutput | Out-File $runLog -Encoding utf8
        if ($LASTEXITCODE -ne 0) { $runOk = $false }
    } catch {
        $runOk = $false
        $_.Exception.Message | Out-File $runLog -Encoding utf8
    }

    if (-not $runOk) {
        Write-Host "RUN FAIL"
        $runFail++
        "$relPath : RUN FAIL" | Out-File $SUMMARY_FILE -Append -Encoding utf8
        "=== $relPath ===" | Out-File $RUN_ERR_FILE -Append -Encoding utf8
        Get-Content $runLog | Out-File $RUN_ERR_FILE -Append -Encoding utf8
        continue
    }

    Write-Host "PASS"
    $pass++
    "$relPath : PASS" | Out-File $SUMMARY_FILE -Append -Encoding utf8
}

Write-Host ""
Write-Host "================================================"
Write-Host " Done. $total total | $pass PASS | $buildFail BUILD_FAIL | $runFail RUN_FAIL"
Write-Host " Logs: $LOG_DIR"
Write-Host "================================================"

"" | Out-File $SUMMARY_FILE -Append -Encoding utf8
"RESULT: $total total | $pass PASS | $buildFail BUILD_FAIL | $runFail RUN_FAIL" |
    Out-File $SUMMARY_FILE -Append -Encoding utf8
