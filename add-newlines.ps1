$files = @(
    'D:\code\fanxy\14-opencode\skills-master\skills\teams\ascendc-kernel-dev-team\asc-devkit\examples\03_simt_api\02_features\README.md',
    'D:\code\fanxy\14-opencode\skills-master\skills\teams\ascendc-kernel-dev-team\asc-devkit\examples\03_simt_api\01_utilities\README.md',
    'D:\code\fanxy\14-opencode\skills-master\skills\teams\ascendc-kernel-dev-team\asc-devkit\examples\02_simd_c_api\02_features\README.md'
)
foreach ($f in $files) {
    if (Test-Path $f) {
        $content = Get-Content $f -Raw
        if (-not $content.EndsWith([Environment]::NewLine) -and -not $content.EndsWith("`n")) {
            Add-Content -Path $f -Value ''
            Write-Host "Added trailing newline to: $f"
        }
    }
}
Write-Host 'Done!'
