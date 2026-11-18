# PVBPS — Lab 6 Windows VM Runbook (`ADS & PowerShell — Living off the Land`)

Copy-paste guide for running all tasks from [PVBPS - Protokol 6.doc](../PVBPS%20-%20Protokol%206.doc) inside your Windows VM, capturing every required output and screenshot cleanly.

---

## 0. Pre-Lab VM Prep (Admin PowerShell)

Open **two** PowerShell windows in your Windows VM:
- **Window A:** PowerShell **Run as Administrator** (for ScriptBlock Logging, `diskpart`, and `Set-ExecutionPolicy`).
- **Window B:** Regular user PowerShell (for the lab tasks).

1. **Enable PowerShell ScriptBlock Logging (Event ID 4104)** in **Admin PowerShell**:
   ```powershell
   New-Item -Path "HKLM:\SOFTWARE\Policies\Microsoft\Windows\PowerShell\ScriptBlockLogging" -Force | Out-Null
   Set-ItemProperty -Path "HKLM:\SOFTWARE\Policies\Microsoft\Windows\PowerShell\ScriptBlockLogging" -Name "EnableScriptBlockLogging" -Value 1
   ```
2. **Create `C:\Lab6` and copy `demo.ps1`** from this repo into `C:\Lab6\demo.ps1`:
   ```powershell
   New-Item -Path "C:\Lab6" -ItemType Directory -Force | Out-Null
   Copy-Item ".\demo.ps1" "C:\Lab6\demo.ps1"
   Set-Location C:\Lab6
   ```
3. *(Optional, recommended for Screenshot 10)* Download **Sysinternals Autoruns** (`Autoruns64.exe`) from `https://learn.microsoft.com/en-us/sysinternals/downloads/autoruns` to inspect the logon registry persistence visually.

---

## Part 1 — Průzkum ADS

Run in **Regular PowerShell** inside `C:\Lab6`:

### 1a. Create text file & compute initial SHA-256
```powershell
Set-Location C:\Lab6
Set-Content -Path .\test.txt -Value "PVBPS Lab 6 - normal text content."
Get-FileHash -Path .\test.txt -Algorithm SHA256
```
- Record the `Hash` value and take **Screenshot 1**.

### 1b. Write data to an Alternate Data Stream (`:secret`) & read it back
```powershell
Set-Content -Path .\test.txt -Stream secret -Value "Hidden payload inside ADS!"
Get-Content -Path .\test.txt -Stream secret
```
- Take **Screenshot 2** (showing both writing and reading `:secret`).

### 1c. Recompute SHA-256 & compare
```powershell
Get-FileHash -Path .\test.txt -Algorithm SHA256
```
- Verify the hash is **100% identical** to Step 1a.

### 1d. List streams in CLI vs. inspect Properties in Windows Explorer
```powershell
Get-Item -Path .\test.txt -Stream *
cmd /c "dir /R test.txt"
```
- Open `C:\Lab6` in **Windows Explorer**, right-click `test.txt` -> **Properties (Vlastnosti)** -> **General** & **Details** tabs.
- Take **Screenshot 3** (PowerShell/CMD stream output vs. Explorer Properties showing no ADS and unchanged visible size).

### 1e. Move (same NTFS) vs. Copy (same NTFS) vs. Copy to FAT32

1. **Move on the same NTFS volume (`C:`):**
   ```powershell
   New-Item -Path .\moved -ItemType Directory -Force | Out-Null
   Move-Item -Path .\test.txt -Destination .\moved\test.txt
   Get-Item -Path .\moved\test.txt -Stream *
   Get-Content -Path .\moved\test.txt -Stream secret
   ```
   - Verify `:secret` is **preserved**.

2. **Copy on the same NTFS volume (`C:`):**
   ```powershell
   New-Item -Path .\copied -ItemType Directory -Force | Out-Null
   Copy-Item -Path .\moved\test.txt -Destination .\copied\test.txt
   Get-Item -Path .\copied\test.txt -Stream *
   Get-Content -Path .\copied\test.txt -Stream secret
   ```
   - Verify `:secret` is **preserved**. Take **Screenshot 4** covering Move + Copy on NTFS.

3. **Create a 50 MB virtual FAT32 disk (`Z:\`) in Admin PowerShell:**
   ```powershell
   @"
   create vdisk file=C:\Lab6\fat32.vhd maximum=50 type=fixed
   select vdisk file=C:\Lab6\fat32.vhd
   attach vdisk
   create partition primary
   format fs=fat32 quick label="FAT32TEST"
   assign letter=Z
   "@ | diskpart
   ```

4. **Copy `C:\Lab6\copied\test.txt` to `Z:\` via File Explorer:**
   - Drag/copy `C:\Lab6\copied\test.txt` to `Z:\` in Windows Explorer.
   - Windows displays a warning dialog (*"Are you sure you want to copy this file without its properties?"* / *"Opravdu chcete tento soubor zkopírovat bez jeho vlastností?"*). **Screenshot 5** this dialog, then click **Yes**.
   - Verify in PowerShell that `:secret` was stripped:
     ```powershell
     Get-Item -Path Z:\test.txt -Stream *
     cmd /c "dir /R Z:\test.txt"
     ```
   - Take **Screenshot 6** showing only `:$DATA` remains on `Z:\test.txt`.

---

## Part 2 — Práce s PowerShellem

### 2a. Run & decode the intro sample command
```powershell
powershell.exe -EncodedCommand VwByAGkAdABlAC0ASABvAHMAdAAgAC0ATwBiAGoAZQBjAHQAIAAiAEgAZQBsAGwAbwAsACAAdwBvAHIAbABkACEAIgA7AA==

$b64 = "VwByAGkAdABlAC0ASABvAHMAdAAgAC0ATwBiAGoAZQBjAHQAIAAiAEgAZQBsAGwAbwAsACAAdwBvAHIAbABkACEAIgA7AA=="
[System.Text.Encoding]::Unicode.GetString([System.Convert]::FromBase64String($b64))
```
- Expected output: `Hello, world!` and decoded string `Write-Host -Object "Hello, world!";`.
- Take **Screenshot 7**.

### 2b. Open `demo.ps1` in PowerShell ISE, Base64-encode (UTF-16LE), run with shortened parameters
1. Open `C:\Lab6\demo.ps1` in **Windows PowerShell ISE** (`powershell_ise.exe C:\Lab6\demo.ps1`) and take **Screenshot 8** (showing the script in the editor).
2. In PowerShell, compute the UTF-16LE Base64 string and execute it using shortened switches (`-NoP` = `-NoProfile`, `-W` = `-WindowStyle`, `-Enc` = `-EncodedCommand`):
   ```powershell
   $code = Get-Content -Raw C:\Lab6\demo.ps1
   $enc  = [Convert]::ToBase64String([System.Text.Encoding]::Unicode.GetBytes($code))
   $enc

   powershell.exe -NoP -W Normal -Enc $enc
   Get-Content "$env:USERPROFILE\Desktop\pvbps_lab6_proof.txt"
   ```
   - **Reference UTF-16LE Base64 of LF `demo.ps1`:**
     ```text
     JAB0AGkAbQBlAHMAdABhAG0AcAAgAD0AIABHAGUAdAAtAEQAYQB0AGUAIAAtAEYAbwByAG0AYQB0ACAAIgB5AHkAeQB5AC0ATQBNAC0AZABkACAASABIADoAbQBtADoAcwBzACIACgAkAGwAbwBnAFAAYQB0AGgAIAA9ACAASgBvAGkAbgAtAFAAYQB0AGgAIAAkAGUAbgB2ADoAVQBTAEUAUgBQAFIATwBGAEkATABFACAAIgBEAGUAcwBrAHQAbwBwAFwAcAB2AGIAcABzAF8AbABhAGIANgBfAHAAcgBvAG8AZgAuAHQAeAB0ACIACgAkAGUAbgB0AHIAeQAgAD0AIAAiAFsAJAB0AGkAbQBlAHMAdABhAG0AcABdACAAUABWAEIAUABTACAATABhAGIAIAA2ACAAZQB4AGUAYwB1AHQAZQBkACAAfAAgAFUAcwBlAHIAOgAgACQAZQBuAHYAOgBVAFMARQBSAE4AQQBNAEUAIAB8ACAASABvAHMAdAA6ACAAJABlAG4AdgA6AEMATwBNAFAAVQBUAEUAUgBOAEEATQBFACAAfAAgAFAASQBEADoAIAAkAFAASQBEACIACgBBAGQAZAAtAEMAbwBuAHQAZQBuAHQAIAAtAFAAYQB0AGgAIAAkAGwAbwBnAFAAYQB0AGgAIAAtAFYAYQBsAHUAZQAgACQAZQBuAHQAcgB5AAoAVwByAGkAdABlAC0ASABvAHMAdAAgACQAZQBuAHQAcgB5ACAALQBGAG8AcgBlAGcAcgBvAHUAbgBkAEMAbwBsAG8AcgAgAEcAcgBlAGUAbgAKAA==
     ```
     *(Note: If Git checked out `demo.ps1` with Windows CRLF line endings or you resaved it in PowerShell ISE, `$enc` computed on Windows will reflect `\r\n` bytes — both decode to the exact same PowerShell script.)*
   - Take **Screenshot 9**.

### 2c. Programmatic persistence in `HKCU\...\Run`
```powershell
$runKey = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
Set-ItemProperty -Path $runKey -Name "PVBPS_Lab6" -Value "powershell.exe -NoP -W Hidden -Enc $enc"
Get-ItemProperty -Path $runKey -Name "PVBPS_Lab6"
```
- Open `regedit.exe` and navigate to `HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run` to verify `PVBPS_Lab6`.
- Take **Screenshot 10** (PowerShell output + `regedit.exe`).

### 2d. Verify execution on next logon & clean up registry entry
1. Sign out of Windows (`logoff` or Start Menu -> Sign out) and sign back in.
2. Check the desktop proof file — a **new line with the logon timestamp and new PID** should appear:
   ```powershell
   Get-Content "$env:USERPROFILE\Desktop\pvbps_lab6_proof.txt"
   ```
   - *(Optional)* Launch **Sysinternals Autoruns** -> **Logon** tab -> find `PVBPS_Lab6` under `HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Run`.
   - Take **Screenshot 11** (`pvbps_lab6_proof.txt` with new logon entry, and optionally Autoruns).
3. **Remove** the persistence registry value after verification:
   ```powershell
   Remove-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run" -Name "PVBPS_Lab6"
   Get-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
   ```
   - Take **Screenshot 12** confirming removal.

### 2e. Execution Policy (`Restricted` vs. `-Ep Bypass` vs. `-Enc`)
In **Admin PowerShell**, inspect the current scopes, clear `CurrentUser` if set, and set `LocalMachine` to `Restricted`:
```powershell
Get-ExecutionPolicy -List
Set-ExecutionPolicy -ExecutionPolicy Undefined -Scope CurrentUser -Force
Set-ExecutionPolicy -ExecutionPolicy Restricted -Scope LocalMachine -Force
Get-ExecutionPolicy -List
```
Now in **Regular PowerShell** (`C:\Lab6`), run all three tests:
```powershell
# 1) Direct script execution -> BLOCKED by Restricted policy
C:\Lab6\demo.ps1

# 2) Explicit -Ep Bypass (-ExecutionPolicy Bypass) -> SUCCEEDS
powershell.exe -Ep Bypass -File C:\Lab6\demo.ps1

# 3) -Enc (-EncodedCommand) under Restricted -> SUCCEEDS!
$code = Get-Content -Raw C:\Lab6\demo.ps1
$enc  = [Convert]::ToBase64String([System.Text.Encoding]::Unicode.GetBytes($code))
powershell.exe -NoP -Enc $enc
```
- Take **Screenshot 13** showing all three cases (`Restricted` error in red, `-Ep Bypass` success, and `-Enc` success).

---

## Part 3 — Prohlížeč událostí (Event Viewer)

1. Quick check in PowerShell (`Get-WinEvent`):
   ```powershell
   Get-WinEvent -LogName "Microsoft-Windows-PowerShell/Operational" -MaxEvents 15 |
     Where-Object Id -eq 4104 |
     Format-List TimeCreated, Id, LevelDisplayName, Message

   Get-WinEvent -LogName "Windows PowerShell" -MaxEvents 15 |
     Where-Object Id -eq 400 |
     Format-List TimeCreated, Id, Message
   ```
2. Open **Event Viewer (`eventvwr.msc`)**:
   - Navigate to:
     `Applications and Services Logs` -> `Microsoft` -> `Windows` -> `PowerShell` -> `Operational`
   - Click an **Event ID 4104** entry matching your `-Enc` run -> **General** & **Details** tabs (notice the **decoded plaintext script block** and empty `Path`!).
   - *(Optional contrast)* Also check `Applications and Services Logs` -> `Windows PowerShell` -> **Event ID 400** showing `HostApplication=powershell.exe -NoP -W Hidden -Enc ...`.
   - Take **Screenshot 14** (Event Viewer showing Event ID 4104).

---

## Part 4 — (+2 body BONUS) `Zone.Identifier`

> **IMPORTANT:** You MUST download the image using a **web browser** (Microsoft Edge, Chrome, or Firefox) and save it to an **NTFS** folder (e.g. `Downloads`). Do **NOT** use `curl` or `Invoke-WebRequest` — CLI tools do not attach the `Zone.Identifier` ADS!

1. Download any `.jpg` or `.png` image from the internet in Edge/Chrome/Firefox into `%USERPROFILE%\Downloads`.
2. Inspect its ADS streams in PowerShell (replace `<image.jpg>` with your filename):
   ```powershell
   $img = "$env:USERPROFILE\Downloads\<image.jpg>"
   Get-Item -Path $img -Stream *
   Get-Content -Path $img -Stream Zone.Identifier
   ```
3. Take **Screenshot 15** showing both `:$DATA` + `:Zone.Identifier` and the `[ZoneTransfer]` contents (`ZoneId=3`, `ReferrerUrl=...`, `HostUrl=...`).

---

## Part 5 — Cleanup (After Screenshots Are Saved)

1. Confirm `PVBPS_Lab6` is gone from `HKCU:\Software\Microsoft\Windows\CurrentVersion\Run`:
   ```powershell
   Get-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run" -Name "PVBPS_Lab6" -ErrorAction SilentlyContinue
   ```
2. Detach and delete the virtual FAT32 disk in **Admin PowerShell**:
   ```powershell
   @"
   select vdisk file=C:\Lab6\fat32.vhd
   detach vdisk
   "@ | diskpart
   Remove-Item C:\Lab6\fat32.vhd -Force
   ```
3. *(Optional)* Restore Execution Policy / ScriptBlock Logging if this is not a dedicated lab snapshot:
   ```powershell
   Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope LocalMachine -Force
   # Remove-Item "HKLM:\SOFTWARE\Policies\Microsoft\Windows\PowerShell\ScriptBlockLogging" -Recurse -Force
   ```

---

## Screenshot Checklist

| # | Protocol Section | What to Capture |
| :---: | :--- | :--- |
| **1** | 1a | Creating `test.txt` and initial `Get-FileHash -Algorithm SHA256` |
| **2** | 1b–1c | Writing/reading `:secret` ADS and recomputing identical SHA-256 hash |
| **3** | 1d | `Get-Item -Stream *` & `dir /R` side-by-side with Explorer Properties |
| **4** | 1e | `Move-Item` and `Copy-Item` on NTFS preserving `:secret` |
| **5** | 1e | Windows Explorer warning dialog when copying `test.txt` to FAT32 `Z:\` |
| **6** | 1e | `Get-Item Z:\test.txt -Stream *` showing `:secret` was stripped on FAT32 |
| **7** | 2a | Running intro `-EncodedCommand` and decoding it in PowerShell |
| **8** | 2b | `demo.ps1` open in Windows PowerShell ISE |
| **9** | 2b | Encoding `demo.ps1` to UTF-16LE Base64, running with `-NoP -W Normal -Enc`, checking proof file |
| **10** | 2c | `Set-ItemProperty` on `HKCU\...\Run` + `regedit.exe` showing `PVBPS_Lab6` |
| **11** | 2d | `pvbps_lab6_proof.txt` after re-logon (+ optional Sysinternals Autoruns) |
| **12** | 2d | `Remove-ItemProperty` removing `PVBPS_Lab6` from `HKCU\...\Run` |
| **13** | 2e | Execution Policy `Restricted` blocking `demo.ps1`, `-Ep Bypass` working, `-Enc` working |
| **14** | 3 | Event Viewer (`eventvwr.msc`) showing Event ID **4104** with decoded script block |
| **15** | Bonus | Browser-downloaded image `:Zone.Identifier` stream (`ZoneId=3`, `HostUrl`, `ReferrerUrl`) |
