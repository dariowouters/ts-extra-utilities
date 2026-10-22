param(
    [string]$ProcessName = "amtrucks",
    [string]$OutFile = "$env:TEMP\$ProcessName-hang.dmp"
)
$proc = Get-Process -Name $ProcessName -ErrorAction SilentlyContinue | Select-Object -First 1
if (-not $proc) { Write-Error "process $ProcessName not running"; exit 1 }

$sig = @'
using System;
using System.Runtime.InteropServices;
public class Dbg {
    [DllImport("dbghelp.dll", SetLastError=true)]
    public static extern bool MiniDumpWriteDump(IntPtr hProcess, uint pid, IntPtr hFile,
        int dumpType, IntPtr excepParam, IntPtr userStreamParam, IntPtr callbackParam);
    [DllImport("kernel32.dll", SetLastError=true)]
    public static extern IntPtr OpenProcess(uint access, bool inherit, uint pid);
}
'@
Add-Type -TypeDefinition $sig
$fs = [System.IO.File]::Create($OutFile)
$h = [Dbg]::OpenProcess(0x1F0FFF, $false, $proc.Id)
$ok = [Dbg]::MiniDumpWriteDump($h, $proc.Id, $fs.SafeFileHandle.DangerousGetHandle(), 6, [IntPtr]::Zero, [IntPtr]::Zero, [IntPtr]::Zero)
$fs.Close()
Write-Output "dump=$ok -> $OutFile"
