$timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
$logPath = Join-Path $env:USERPROFILE "Desktop\pvbps_lab6_proof.txt"
$entry = "[$timestamp] PVBPS Lab 6 executed | User: $env:USERNAME | Host: $env:COMPUTERNAME | PID: $PID"
Add-Content -Path $logPath -Value $entry
Write-Host $entry -ForegroundColor Green
