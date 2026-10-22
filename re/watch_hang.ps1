$out = "C:/Users/PC/AppData/Local/Temp/amtrucks-hang.dmp"
$needed = 0
Write-Output "watching for amtrucks hang..."
while ($true) {
    $p = Get-Process -Name "amtrucks" -ErrorAction SilentlyContinue | Select-Object -First 1
    if (-not $p) { Start-Sleep -Seconds 2; $needed = 0; continue }
    if (-not $p.Responding) {
        $needed++
        if ($needed -ge 3) {
            Write-Output "hang detected, capturing dump..."
            & powershell -NoProfile -ExecutionPolicy Bypass -File "D:/plugins para ets/trailer axles control/re/capture_hang_dump.ps1" -ProcessName "amtrucks" -OutFile $out
            Write-Output "done -> $out"
            exit 0
        }
    } else { $needed = 0 }
    Start-Sleep -Seconds 2
}
