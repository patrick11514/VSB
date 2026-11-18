# Protokol k úloze 6: ADS & PowerShell — Living off the Land

| Položka | Hodnota |
| :--- | :--- |
| **Předmět** | Počítačové viry a bezpečnost počítačových systémů (PVBPS) |
| **Číslo úlohy** | 6 |
| **Název úlohy** | ADS & PowerShell — Living off the Land |
| **Body** | 3 (+2 bonusové) |
| **Studijní skupina** | Čtvrtek, `[DOPLNIT: čas konání cvičení]` |
| **Datum vypracování** | `[DOPLNIT: datum]` |
| **Jméno a příjmení** | `[DOPLNIT: Jméno a příjmení]` |
| **Osobní číslo** | `[DOPLNIT: osobní číslo]` |

---

## 1. Průzkum ADS (Alternate Data Streams)

### 1a. Vytvoření textového dokumentu a výpočet počátečního hashe SHA-256
V pracovním adresáři `C:\Lab6` byl vytvořen běžný textový soubor `test.txt` a pomocí PowerShell cmdletu `Get-FileHash` byl vypočten jeho kryptografický otisk SHA-256:

```powershell
Set-Content -Path .\test.txt -Value "PVBPS Lab 6 - normal text content."
Get-FileHash -Path .\test.txt -Algorithm SHA256
```

- **Naměřená hodnota SHA-256 (před zápisem ADS):** `[DOPLNIT: hash SHA-256]`

> **[SCREENSHOT 1: Vytvoření souboru `test.txt` a výpočet počátečního hashe SHA-256 pomocí `Get-FileHash`]**

---

### 1b. Zápis dat do alternativního datového proudu a jejich výpis
K existujícímu souboru `test.txt` byl připojen pojmenovaný alternativní datový proud `:secret` a jeho obsah byl následně načten zpět:

```powershell
Set-Content -Path .\test.txt -Stream secret -Value "Hidden payload inside ADS!"
Get-Content -Path .\test.txt -Stream secret
```

- **Výpis proudu `:secret`:** `Hidden payload inside ADS!`

---

### 1c. Opětovný výpočet hashe SHA-256 a porovnání
Po zápisu do alternativního datového proudu `:secret` byl hash souboru vypočten znovu stejným příkazem:

```powershell
Get-FileHash -Path .\test.txt -Algorithm SHA256
```

- **Naměřená hodnota SHA-256 (po zápisu ADS):** `[DOPLNIT: stejný hash SHA-256]`

> **[SCREENSHOT 2: Zápis do proudu `:secret`, jeho přečtení a opětovný výpočet hashe SHA-256]**

#### Vysvětlení a dopad na detekci založenou na hashích
- **Co se stalo:** Hodnota SHA-256 zůstala **zcela identická**, přestože soubor nyní na disku nese další skrytá data v proudu `:secret`.
- **Proč:** Standardní funkce pro výpočet hashe souboru (včetně `Get-FileHash`, `certutil -hashfile` i většiny klasických IoC skenerů) čtou pouze výchozí nepojmenovaný datový proud `:$DATA`. Pojmenované alternativní proudy (`<soubor>:<stream>`) do výpočtu nevstupují.
- **Význam pro detekci:**
  - Útočník může ukrýt škodlivý payload nebo konfiguraci do ADS legitimního, podepsaného či na whitelistu uvedeného souboru (např. systémového dokumentu nebo utility), aniž by se změnil hash hlavního souboru.
  - Samotná detekce založená na hashích (hash IoC / reputační databáze) tento skrytý obsah **neodhalí** — je nutné kontrolovat i jednotlivé ADS proudy nebo monitorovat jejich vytváření na úrovni souborového systému (Sysmon Event ID 15 *FileCreateStreamHash*).

---

### 1d. Výpis streamů v příkazové řádce vs. vlastnosti v Průzkumníku
Zobrazení všech datových proudů souboru v PowerShellu a v klasickém `cmd.exe`:

```powershell
Get-Item -Path .\test.txt -Stream *
cmd /c "dir /R test.txt"
```

Následně byly otevřeny vlastnosti souboru (`Pravý klik -> Vlastnosti`) v Průzkumníku Windows (File Explorer).

> **[SCREENSHOT 3: Porovnání výpisu `Get-Item -Stream *` / `dir /R` v terminálu s oknem Vlastnosti souboru v Průzkumníku Windows]**

#### Porovnání viditelnosti informací
- **V příkazové řádce / PowerShellu:**
  - `Get-Item -Stream *` vypíše oba proudy včetně jejich názvů a velikostí v bajtech: výchozí `:$DATA` (`[DOPLNIT: velikost]` B) i skrytý `:secret` (`[DOPLNIT: velikost]` B).
  - Příkaz `dir /R` v `cmd.exe` zobrazí pod hlavním záznamem souboru řádek `test.txt:secret:$DATA` s jeho velikostí.
- **V Průzkumníku Windows (a běžném `dir`):**
  - Průzkumník ani okno **Vlastnosti** (karty *Obecné* a *Podrobnosti*) alternativní datové proudy nijak nezobrazují.
  - Udávaná velikost souboru odpovídá výhradně hlavnímu proudu `:$DATA`; obsah ani existence proudu `:secret` zůstávají běžnému uživateli zcela skryté.

---

### 1e. Chování ADS při přesunu a kopírování (NTFS vs. FAT32)

Byly otestovány všechny tři scénáře manipulace se souborem `test.txt` obsahujícím ADS `:secret`:

1. **Přesun (`Move-Item`) na stejném diskovém oddílu NTFS (`C:`):**
   ```powershell
   New-Item -Path .\moved -ItemType Directory -Force | Out-Null
   Move-Item -Path .\test.txt -Destination .\moved\test.txt
   Get-Item -Path .\moved\test.txt -Stream *
   ```
   - **Výsledek:** ADS `:secret` zůstal **zachován**.

2. **Kopírování (`Copy-Item`) na stejném diskovém oddílu NTFS (`C:`):**
   ```powershell
   New-Item -Path .\copied -ItemType Directory -Force | Out-Null
   Copy-Item -Path .\moved\test.txt -Destination .\copied\test.txt
   Get-Item -Path .\copied\test.txt -Stream *
   ```
   - **Výsledek:** ADS `:secret` zůstal **zachován**.

> **[SCREENSHOT 4: Ověření zachování ADS `:secret` po přesunu (`Move-Item`) a kopírování (`Copy-Item`) v rámci NTFS oddílu]**

3. **Kopírování na médium se souborovým systémem FAT32 (virtuální disk `fat32.vhd`, jednotka `Z:`):**
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
   Při kopírování souboru `C:\Lab6\copied\test.txt` na disk `Z:\` v Průzkumníku Windows systém zobrazil varování o ztrátě vlastností souboru (*„Opravdu chcete tento soubor zkopírovat bez jeho vlastností?“*). Po potvrzení byly ověřeny proudy cílového souboru:
   ```powershell
   Get-Item -Path Z:\test.txt -Stream *
   ```
   - **Výsledek:** Proud `:secret` byl **odstraněn** — na disku `Z:\` existuje pouze výchozí obsah souboru.

> **[SCREENSHOT 5: Varovný dialog Průzkumníka Windows při kopírování souboru s ADS na FAT32 disk `Z:\`]**
>
> **[SCREENSHOT 6: Výstup `Get-Item Z:\test.txt -Stream *` potvrzující ztrátu proudu `:secret` na FAT32]**

#### Vysvětlení rozdílů a význam pro útočníka i analýzu
- **Proč se chování liší:**
  - **Přesun na stejném NTFS oddílu:** Nejedná se o fyzické kopírování dat — NTFS pouze aktualizuje ukazatel/záznam v adresářovém indexu k témuž záznamu v **MFT (Master File Table)**, kde jsou uloženy všechny `$DATA` atributy včetně ADS.
  - **Kopírování na NTFS:** Nativní Windows API pro kopírování souborů (`CopyFileEx` / Explorer / PowerShell na NTFS) cíleně přenáší všechny datové proudy do nově vytvořeného MFT záznamu na cílovém NTFS oddílu.
  - **Kopírování na FAT32:** Souborový systém FAT32 nezná MFT ani koncept více datových proudů na jeden soubor (každý soubor v FAT32 má jen jeden datový řetězec clusterů). Všechny alternativní proudy jsou proto při zápisu na FAT32 nenávratně zahozeny.
- **Význam pro útočníka:**
  - Útočník nemůže doručit ADS přímo přes FAT32 médium, HTTP přenos ani běžný ZIP archiv (payload do ADS musí zapsat až na cílovém NTFS systému).
  - Naopak přenesením souboru přes FAT32 (nebo nahráním do nepodporujícího úložiště) může útočník či uživatel nechtěně i záměrně odstranit bezpečnostní příznak `Zone.Identifier`.
- **Význam pro analýzu a forenziku:**
  - Při forenzním zajišťování důkazů (triage sběr podezřelých souborů) **nesmí analytik ukládat artefakty na flash disk/oddíl s FAT32**, jinak zničí jak případný payload v ADS, tak důležitá forenzní metadata o původu souboru (`Zone.Identifier`). Důkazy je nutné sbírat na NTFS nebo v bitových/forenzních kontejnerech (např. E01, AFF4, tar/wim s podporou ADS).

---

## 2. Práce s PowerShellem

### 2a. Spuštění a dekódování ukázkového příkazu z úvodu
Byl spuštěn příkaz z úvodu zadání:

```powershell
powershell.exe -EncodedCommand VwByAGkAdABlAC0ASABvAHMAdAAgAC0ATwBiAGoAZQBjAHQAIAAiAEgAZQBsAGwAbwAsACAAdwBvAHIAbABkACEAIgA7AA==
```

Následně byl řetězec dekódován v PowerShellu:

```powershell
$b64 = "VwByAGkAdABlAC0ASABvAHMAdAAgAC0ATwBiAGoAZQBjAHQAIAAiAEgAZQBsAGwAbwAsACAAdwBvAHIAbABkACEAIgA7AA=="
[System.Text.Encoding]::Unicode.GetString([System.Convert]::FromBase64String($b64))
```

- **Co příkaz dělá:** Vypíše na standardní výstup konzole text `Hello, world!`.
- **Dekódovaný obsah:** `Write-Host -Object "Hello, world!";`

> **[SCREENSHOT 7: Spuštění ukázkového `-EncodedCommand` a jeho dekódování v PowerShellu]**

#### Jak je obsah zakódován a proč zpomaluje čtení
- Parametr `-EncodedCommand` v PowerShellu vyžaduje řetězec zakódovaný v **Base64 nad kódováním UTF-16 Little Endian (UTF-16LE)** (v .NET odpovídá `System.Text.Encoding.Unicode`).
- U běžného ASCII textu má každý znak v UTF-16LE dva bajty (`<znak>\x00`), což při následném kódování Base64 vytváří charakteristické opakující se nulové bajty a meziznakové sekvence (často s písmeny `A` v Base64, např. `VwByAGkAdABl...`).
- **Proč zpomaluje lidskou analýzu:**
  - Člověk není schopen text v Base64 přečíst z hlavy bez dekódování.
  - Zároveň selhává prosté vyhledávání ASCII podřetězců (např. `grep` / `strings` nad příkazovou řádkou v logu) a při dekódování v naivním UTF-8/ASCII Base64 dekodéru se mezi každým písmenem zobrazují nulové bajty (`W\0r\0i\0t\0e\0...`), což čtení dále znepřehledňuje.
  - Nejde však o šifrování — jde pouze o reverzibilní kódování, které PowerShell před spuštěním okamžitě převede zpět do čitelného textu.

---

### 2b. Vlastní skript `.ps1`, převod do Base64 (UTF-16LE) a zkrácené parametry

V editoru Windows PowerShell ISE byl vytvořen skript [demo.ps1](file:///home/patrick115/Projects/VSB/Semester9/PVBPS/PowerShell/demo.ps1), který zapisuje časové razítko, uživatelské jméno, název počítače a PID do souboru na ploše uživatele a vypisuje potvrzení na konzoli:

```powershell
$timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
$logPath = Join-Path $env:USERPROFILE "Desktop\pvbps_lab6_proof.txt"
$entry = "[$timestamp] PVBPS Lab 6 executed | User: $env:USERNAME | Host: $env:COMPUTERNAME | PID: $PID"
Add-Content -Path $logPath -Value $entry
Write-Host $entry -ForegroundColor Green
```

> **[SCREENSHOT 8: Skript `demo.ps1` otevřený ve Windows PowerShell ISE]**

Skript byl převeden do UTF-16LE Base64 a spuštěn pomocí zkrácených parametrů `powershell.exe`:

```powershell
$code = Get-Content -Raw C:\Lab6\demo.ps1
$enc  = [Convert]::ToBase64String([System.Text.Encoding]::Unicode.GetBytes($code))
$enc

powershell.exe -NoP -W Normal -Enc $enc
Get-Content "$env:USERPROFILE\Desktop\pvbps_lab6_proof.txt"
```

- **Hodnota Base64 (UTF-16LE, LF):**
  ```text
  JAB0AGkAbQBlAHMAdABhAG0AcAAgAD0AIABHAGUAdAAtAEQAYQB0AGUAIAAtAEYAbwByAG0AYQB0ACAAIgB5AHkAeQB5AC0ATQBNAC0AZABkACAASABIADoAbQBtADoAcwBzACIACgAkAGwAbwBnAFAAYQB0AGgAIAA9ACAASgBvAGkAbgAtAFAAYQB0AGgAIAAkAGUAbgB2ADoAVQBTAEUAUgBQAFIATwBGAEkATABFACAAIgBEAGUAcwBrAHQAbwBwAFwAcAB2AGIAcABzAF8AbABhAGIANgBfAHAAcgBvAG8AZgAuAHQAeAB0ACIACgAkAGUAbgB0AHIAeQAgAD0AIAAiAFsAJAB0AGkAbQBlAHMAdABhAG0AcABdACAAUABWAEIAUABTACAATABhAGIAIAA2ACAAZQB4AGUAYwB1AHQAZQBkACAAfAAgAFUAcwBlAHIAOgAgACQAZQBuAHYAOgBVAFMARQBSAE4AQQBNAEUAIAB8ACAASABvAHMAdAA6ACAAJABlAG4AdgA6AEMATwBNAFAAVQBUAEUAUgBOAEEATQBFACAAfAAgAFAASQBEADoAIAAkAFAASQBEACIACgBBAGQAZAAtAEMAbwBuAHQAZQBuAHQAIAAtAFAAYQB0AGgAIAAkAGwAbwBnAFAAYQB0AGgAIAAtAFYAYQBsAHUAZQAgACQAZQBuAHQAcgB5AAoAVwByAGkAdABlAC0ASABvAHMAdAAgACQAZQBuAHQAcgB5ACAALQBGAG8AcgBlAGcAcgBvAHUAbgBkAEMAbwBsAG8AcgAgAEcAcgBlAGUAbgAKAA==
  ```
  *(Pozn.: Při ukládání ve Windows s konci řádků CRLF `\r\n` se výsledný Base64 řetězec liší v kódování konců řádků, dekódovaná funkčnost zůstává totožná.)*

- **Přehled použitých zkrácených parametrů PowerShellu:**

| Zkrácená forma | Plný parametr | Význam |
| :--- | :--- | :--- |
| `-NoP` | `-NoProfile` | Nenačítá uživatelský profil PowerShellu (rychlejší a předvídatelnější běh). |
| `-W Normal` / `-W Hidden` | `-WindowStyle` | Nastaví styl okna (`Hidden` skryje okno při perzistenci). |
| `-Ep Bypass` | `-ExecutionPolicy` | Dočasně obejde Execution Policy pouze pro daný proces (použito v bodě 2e). |
| `-Enc` | `-EncodedCommand` | Předá příkaz zakódovaný v Base64 (UTF-16LE). |

> **[SCREENSHOT 9: Převod skriptu `demo.ps1` do Base64, spuštění pomocí zkrácených parametrů `-NoP -W Normal -Enc` a výpis `pvbps_lab6_proof.txt`]**

---

### 2c. Programové přidání zakódovaného skriptu do registru (`HKCU\...\Run`)
Zakódovaný příkaz byl programově zapsán do větve registru aktuálního uživatele pro automatické spuštění při přihlášení:

```powershell
$runKey = "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
Set-ItemProperty -Path $runKey -Name "PVBPS_Lab6" -Value "powershell.exe -NoP -W Hidden -Enc $enc"
Get-ItemProperty -Path $runKey -Name "PVBPS_Lab6"
```

> **[SCREENSHOT 10: Programový zápis klíče `PVBPS_Lab6` do `HKCU\...\Run` a kontrola v `regedit.exe`]**

---

### 2d. Ověření po opětovném přihlášení, analýza detekce a odstranění
Po odhlášení ze systému (`logoff`) a novém přihlášení uživatele byl zkontrolován soubor `Desktop\pvbps_lab6_proof.txt`:

```powershell
Get-Content "$env:USERPROFILE\Desktop\pvbps_lab6_proof.txt"
```

- **Výsledek:** V souboru přibyl nový řádek s časovým razítkem odpovídajícím času přihlášení a novým `PID` procesu, což jednoznačně prokazuje automatické spuštění bez nutnosti existence `.ps1` souboru při startu (payload je vložen přímo v hodnotě registru).

> **[SCREENSHOT 11: Nový záznam v `pvbps_lab6_proof.txt` po odhlášení a opětovném přihlášení (+ volitelně Sysinternals Autoruns)]**

Po úspěšném ověření byla testovací hodnota z registru odstraněna:

```powershell
Remove-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run" -Name "PVBPS_Lab6"
Get-ItemProperty -Path "HKCU:\Software\Microsoft\Windows\CurrentVersion\Run"
```

> **[SCREENSHOT 12: Odstranění hodnoty `PVBPS_Lab6` z klíče `HKCU\...\Run`]**

#### Proč je tento typ perzistence pro malware atraktivní a jak je detekován
- **Atraktivita pro útočníka:**
  - **Nevyžaduje administrátorská práva:** Větev `HKCU` je zapisovatelná z kontextu běžného uživatele.
  - **Bez-souborový payload (Fileless):** Kód skriptu je uložen přímo uvnitř řetězce v registru jako Base64 argument pro `powershell.exe`, nikoliv jako samostatný `.ps1` / `.exe` soubor na disku.
  - **Zneužití důvěryhodného LOLBin nástroje:** Spouštěná binárka `powershell.exe` je legitimní, Microsoftem podepsaný systémový nástroj.
- **Jak jej odhalí analytické a bezpečnostní nástroje:**
  - **Sysinternals Autoruns:** Systematicky prohledává známá místa automatického spouštění (ASEP — *Auto-Start Extensibility Points*) včetně `HKCU\...\Run` a okamžitě zobrazí celý podezřelý příkazový řádek s `-Enc` a `-W Hidden`.
  - **EDR a Sysmon:**
    - **Sysmon Event ID 13 (*RegistryEvent — Value Set*):** Zaznamená samotný zápis do citlivé větve `...\CurrentVersion\Run`.
    - **Sysmon Event ID 1 (*ProcessCreate*) / Security Event ID 4688:** Zachytí podezřelý strom procesů (`explorer.exe` / `userinit.exe` -> `powershell.exe -NoP -W Hidden -Enc ...`) včetně kompletních argumentů příkazové řádky.

---

### 2e. Průzkum Execution Policy (`Restricted` vs. `Bypass` vs. `-EncodedCommand`)

V administrátorském PowerShellu byla nejprve zkontrolována aktuální politika a nastavena zásada `Restricted`:

```powershell
Get-ExecutionPolicy -List
Set-ExecutionPolicy -ExecutionPolicy Undefined -Scope CurrentUser -Force
Set-ExecutionPolicy -ExecutionPolicy Restricted -Scope LocalMachine -Force
Get-ExecutionPolicy -List
```

Následně byly v běžné relaci provedeny tři testy:

1. **Přímé spuštění skriptu `C:\Lab6\demo.ps1`:**
   - **Výsledek:** **Blokováno** (*„...cannot be loaded because running scripts is disabled on this system“*).
2. **Spuštění s přepínačem `-ExecutionPolicy Bypass` (`-Ep Bypass`):**
   ```powershell
   powershell.exe -Ep Bypass -File C:\Lab6\demo.ps1
   ```
   - **Výsledek:** **Úspěšné** — skript se bez varování spustil.
3. **Spuštění přes `-EncodedCommand` (`-Enc`) při stále aktivní zásadě `Restricted`:**
   ```powershell
   powershell.exe -NoP -Enc $enc
   ```
   - **Výsledek:** **Úspěšné** — příkaz se vykonal i bez přepínače `Bypass` a při systémové politice `Restricted`.

> **[SCREENSHOT 13: Chování PowerShellu v režimu `Restricted` — blokování `.ps1`, obejití přes `-Ep Bypass` i přímý běh přes `-Enc`]**

#### Vysvětlení chování a role AMSI
- **Proč je Execution Policy omezená ochrana:** Microsoft výslovně definuje Execution Policy jako **bezpečnostní pojistku proti nechtěnému spuštění skriptu uživatelem**, nikoliv jako bezpečnostní hranici (*security boundary*). Uživatel ji může kdykoliv na úrovni procesu přepsat parametrem `-ExecutionPolicy Bypass`.
- **Proč `-EncodedCommand` (a `-Command`) fungují i v `Restricted`:** Zásada `Restricted` zakazuje pouze načítání konfiguračních profilů a lokálních `.ps1` skriptů z disku. Parametry `-Command` a `-EncodedCommand` předávají příkazy přímo interpretu jako řetězec přes příkazovou řádku, nikoliv jako soubor ze souborového systému, a proto se na ně omezení `Restricted` nevztahuje.
- **Co do procesu vstupuje i přesto (AMSI):** Jakmile PowerShell dekóduje Base64 řetězec z `-EncodedCommand` do paměti, předá výsledný čitelný kód rozhraní **AMSI (Antimalware Scan Interface)** dříve, než se kód provede. Antivirus / EDR (např. Microsoft Defender) tak obsah skriptu zkontroluje bez ohledu na Execution Policy nebo na to, že kód nepochází ze souboru na disku.

---

## 3. Prohlížeč událostí (Event Viewer)

Záznamy byly vyhledány v nástroji `eventvwr.msc` i ověřeny v PowerShellu pomocí `Get-WinEvent`:

```powershell
Get-WinEvent -LogName "Microsoft-Windows-PowerShell/Operational" -MaxEvents 15 |
  Where-Object Id -eq 4104 |
  Format-List TimeCreated, Id, LevelDisplayName, Message
```

- **Umístění v Prohlížeči událostí:**
  - `Protokoly aplikací a služeb` -> `Microsoft` -> `Windows` -> `PowerShell` -> `Operational` (**Event ID 4104**).
  - Doplňkově také `Protokoly aplikací a služeb` -> `Windows PowerShell` (**Event ID 400**).

> **[SCREENSHOT 14: Záznam Event ID 4104 v Prohlížeči událostí (`Microsoft-Windows-PowerShell/Operational`) s dekódovaným obsahem skriptu]**

### Co z vybraného záznamu analytik vyčte
1. **Event ID 4104 (*Execute a Remote Command / ScriptBlock Logging*):**
   - **Kompletní dekódovaný zdrojový kód skriptu:** Přestože byl skript spuštěn přes Base64 (`-Enc`) a s oknem `-W Hidden`, v textu události 4104 je zachycen **čitelný plaintext kód** (`$timestamp = Get-Date... Add-Content...`).
   - **Časová značka (`TimeCreated`) a uživatel (`User` / SID):** Přesný čas spuštění a identita účtu, pod kterým příkaz běžel (vhodné pro korelaci s časem přihlášení při testu perzistence).
   - **Identifikátor bloku a procesu (`ScriptBlock ID`, `Process ID`):** Umožňuje poskládat i rozsáhlejší skripty rozdělené do více bloků a navázat je na konkrétní proces `powershell.exe`.
   - **Prázdné pole `Path`:** Indikuje, že kód nebyl spuštěn ze souboru na disku, ale přímo z příkazové řádky nebo paměti — typický znak **fileless** aktivity.
2. **Event ID 400 (v protokolu `Windows PowerShell`):**
   - Pole `HostApplication` zachycuje surovou příkazovou řádku přesně tak, jak byla spuštěna (včetně příznaků `-NoP -W Hidden -Enc JAB0...`), což analytikovi okamžitě odhalí pokus o obfuskaci a skrytí okna.

---

## 4. Odpovědi na teoretické otázky

### 4.1 Co je to AMSI, proč se ji malware snaží obejít a jak souvisí se ScriptBlock Logging (Event ID 4104)?
- **AMSI (Antimalware Scan Interface):** Univerzální rozhraní operačního systému Windows, které propojuje aplikační/skriptovací enginy (PowerShell, WSH/VBScript/JScript, .NET CLR, makra Office VBA) s nainstalovaným antivirem nebo EDR řešením. Skriptovací engine načte do paměti `amsi.dll` a těsně před samotným vykonáním kódu zavolá funkci `AmsiScanBuffer()`, která předá již **deobfuskovaný** obsah v paměti ke kontrole.
- **Proč se ji malware snaží obejít:** Jelikož AMSI vidí kód až po dekódování Base64, dekompresi či rozbalení z paměti (např. při `IEX`), jednoduché kódování ani bezsouborové spouštění proti ní nepomáhají. Útočníci proto používají techniky *AMSI bypass* — např. přepsání paměti funkce `AmsiScanBuffer` (memory patching na `ret`), manipulaci s interním polem `amsiInitFailed` přes .NET reflection nebo vynucení downgrade na PowerShell v2 (který AMSI nepodporuje).
- **Souvislost s Event ID 4104:**
  - Obě technologie pracují v téže fázi zpracování skriptu — **po deobfuskaci / dekódování těsně před exekucí**.
  - **AMSI** plní **preventivní** roli (blokuje škodlivý skript v reálném čase před spuštěním).
  - **ScriptBlock Logging (Event ID 4104)** plní **detektivní a forenzní** roli (zapisuje deobfuskovaný blok do ETW / Event Logu). Navíc PowerShell i bez explicitně zapnuté politiky automaticky na úrovni `Warning` loguje bloky obsahující podezřelé vzory — včetně samotných pokusů o obejití AMSI!

### 4.2 Co je to PowerSploit a k čemu se používá? Co je to Meterpreter a je možné ho spustit přímo z paměti počítače?
- **PowerSploit:** Známý open-source modulární post-exploitační framework napsaný v PowerShellu určený pro penetrační testování a red teaming (v praxi však často zneužívaný i útočníky). Skládá se z řady specializovaných modulů, např.:
  - `PowerView` (průzkum Active Directory, vyhledávání oprávnění a uživatelů),
  - `PowerUp` (hledání cest k eskalaci privilegií na Windows),
  - `Invoke-ReflectivePEInjection` (injektáž PE/DLL binárek přímo do paměti bez uložení na disk),
  - `Invoke-Mimikatz` (extrakce přihlašovacích údajů z paměti procesu `lsass.exe`).
- **Meterpreter:** Pokročilý modulární post-exploitační payload frameworku **Metasploit**, poskytující útočníkovi interaktivní šifrovaný komunikační kanál (C2), přístup k souborům, registrům, zachytávání vstupů, pivoting po síti či správu procesů.
- **Spuštění přímo z paměti:** **Ano.** Meterpreter je od základu navržen pro běh výhradně z operační paměti pomocí techniky **Reflective DLL Injection**. Počáteční loader (např. přes PowerShell nebo shellcode stager) alokuje paměť v cílovém procesu (`VirtualAlloc`), stáhne knihovnu přes síť a pomocí vlastního reflektivního zavaděče ji zmapuje přímo do RAM bez použití standardní funkce `LoadLibrary` a bez zápisu souboru `.dll`/`.exe` na disk. Další rozšíření se rovněž nahrávají dynamicky po síti pouze do paměti.

### 4.3 Z jakého důvodu chceme spouštět kód pouze z paměti počítače bez uložení na úložiště? (Výhody a nevýhody)

Hlavním důvodem spouštění kódu výhradně z paměti (*fileless execution*) je minimalizace stop na souborovém systému a obcházení tradiční statické ochrany založené na kontrole souborů na disku.

| Pohled | Výhody | Nevýhody |
| :--- | :--- | :--- |
| **Útočník** | • Obchází klasický statický antivirus skenující soubory na disku a hashové IoC.<br>• Nezanechává spustitelné binárky pro snadnou zpětnou analýzu či extrakci z disku.<br>• Využívá důvěryhodné systémové nástroje (LOLBins), což splývá s běžnou administrací. | • Kód je **volatilní** — po restartu zmizí (pro přežití je nutná perzistence v registru, WMI, Scheduled Tasks či ADS).<br>• Stále podléhá kontrole v paměti (AMSI, ETW, EDR memory skenery).<br>• Příkazové řádky (`powershell -Enc ...`) bývají výrazně podezřelé v telemetrii. |
| **Obránce** | • Čistě volatilní payload bez perzistence se po restartu systému automaticky vymaže.<br>• Zneužívání LOLBins zanechává silné behaviorální a textové stopy v lozích (Event ID 4104, 4688, Sysmon, AMSI). | • Tradiční antivirová kontrola disku a mrtvá disková forenzika (*dead-box forensics*) nemusí škodlivý kód vůbec zachytit.<br>• Vyžaduje pokročilé logování a aktivní EDR; při podezření je nutné provést live akvizici paměti (RAM dump) před vypnutím stroje. |

---

## 5. (+2 body BONUS) `Zone.Identifier`, SmartScreen a CVE-2023-24880

### 5a. Stažení obrázku přes webový prohlížeč a průzkum ADS
Z internetu byl přes webový prohlížeč (`[DOPLNIT: název prohlížeče, např. Edge / Chrome / Firefox]`) stažen obrázek do složky `Downloads` na disku NTFS (nikoliv přes `curl` nebo `Invoke-WebRequest`, které ADS nevytvářejí). Následně byly vypsány jeho datové proudy:

```powershell
$img = "$env:USERPROFILE\Downloads\[DOPLNIT: nazev_obrazku]"
Get-Item -Path $img -Stream *
Get-Content -Path $img -Stream Zone.Identifier
```

- **Obsah proudu `:Zone.Identifier`:**
  ```ini
  [ZoneTransfer]
  ZoneId=[DOPLNIT: 3]
  ReferrerUrl=[DOPLNIT: URL odkazující stránky]
  HostUrl=[DOPLNIT: přesná URL staženého obrázku]
  ```

> **[SCREENSHOT 15: Výpis ADS proudů staženého obrázku a obsahu proudu `:Zone.Identifier`]**

---

### 5b. Co je to `Zone.Identifier` a co z něj lze zjistit?
- **`Zone.Identifier`** (často označovaný jako **Mark of the Web — MotW**) je legitimní alternativní datový proud (ADS) systému NTFS, který Windows (Attachment Execution Service) a webové prohlížeče či e-mailoví klienti automaticky připojují ke staženým souborům z nedůvěryhodných zdrojů.
- **Co z něj může analytik zjistit:**
  - **`ZoneId`:** Bezpečnostní zónu původu souboru.
  - **`HostUrl`:** Přesnou URL adresu, odkud byl soubor stažen (např. C2 server nebo kompromitovaný web).
  - **`ReferrerUrl`:** Adresu stránky, ze které uživatel na stažení přešel (pomáhá zrekonstruovat vektor infekce — phishingový portál, malvertising, vyhledávač apod.).
  - U příloh z Outlooku nebo zpráv se zde mohou objevit i další identifikátory původu (např. `AppZoneId` či metadata klienta).

---

### 5c. Jaké `ZoneId` má stažený obrázek a jaký je význam hodnot?
Stažený obrázek z internetu má hodnotu **`ZoneId=3`**, což označuje **Internetovou zónu** (*Internet Zone* — nedůvěryhodný soubor stažený z veřejného internetu).

Přehled definovaných hodnot `ZoneId` v URL Security Zones systému Windows:

| `ZoneId` | Název zóny | Význam |
| :---: | :--- | :--- |
| **0** | Local Machine | Místní počítač (lokálně vytvořené / nainstalované soubory) |
| **1** | Local Intranet | Místní podniková síť (intranet) |
| **2** | Trusted Sites | Důvěryhodné weby |
| **3** | Internet | Veřejný internet (výchozí pro soubory stažené z webu) |
| **4** | Restricted Sites | Omezené / nedůvěryhodné weby |

---

### 5d. Souvislost mezi `Zone.Identifier`, Microsoft SmartScreen a zranitelností CVE-2023-24880
- **Vztah `Zone.Identifier` a Microsoft Defender SmartScreen:**
  - Když se uživatel pokusí otevřít nebo spustit soubor označený MotW (`Zone.Identifier` s `ZoneId=3` nebo `4`), operační systém vyhodnotí jeho původ jako potenciálně nebezpečný a předá jej ke kontrole službě **Microsoft Defender SmartScreen** (případně aplikacím Office, které dokument otevřou v režimu *Chráněné zobrazení / Protected View*).
  - SmartScreen ověří digitální podpis (Authenticode) a online reputaci souboru či vydavatele; u neznámých nebo podezřelých binárek zobrazí varovnou obrazovku (*„Systém Windows ochránil váš počítač“*).
- **Zranitelnost CVE-2023-24880 (*Windows SmartScreen Security Feature Bypass*):**
  - Šlo o kritickou zero-day zranitelnost opravenou v březnu 2023 (navazovala jako obcházení neúplné opravy starší zranitelnosti **CVE-2022-44698**) a aktivně zneužívanou v reálných kampaních pro šíření **ransomwaru Magniber**.
  - **Mechanismus:** Útočníci vytvářeli speciálně upravené instalační balíčky **`.MSI`** (Microsoft Installer) s poškozeným / nevalidním digitálním podpisem **Authenticode**.
  - **Důsledek:** Přestože stažený `.MSI` soubor obsahoval korektní ADS `Zone.Identifier` (`ZoneId=3`), při jeho zpracování došlo kvůli poškozenému podpisu v rutině ověřování podpisů k chybovému stavu (*fail-open*), v jehož důsledku SmartScreen kontrolu reputace i zobrazení bezpečnostního varování zcela přeskočil. Zranitelnost tak umožnila spouštět stažené škodlivé instalátory z internetu bez varování SmartScreenu navzdory přítomnému MotW.

---

## 6. Závěr

V rámci úlohy č. 6 byly prakticky ověřeny dvě klíčové techniky moderního malwaru spadající do přístupu **Living off the Land**: skrývání dat v **NTFS Alternate Data Streams (ADS)** a spouštění zakódovaných příkazů v **PowerShellu**.

Experimentálně bylo prokázáno, že zápis do ADS neovlivní hash SHA-256 hlavního proudu `:$DATA` ani viditelnost v Průzkumníku Windows, přičemž ADS se zachovávají při přesunu a kopírování na NTFS, avšak při kopírování na FAT32 jsou nevratně odstraněny. Dále byla demonstrována tvorba UTF-16LE Base64 payloadu, programová perzistence v registru `HKCU\...\Run` a skutečnost, že zásada Execution Policy `Restricted` nezabrání spuštění kódu přes `-EncodedCommand` a nepředstavuje bezpečnostní hranici.

Z pohledu obrany bylo potvrzeno, že i bezsouborový a v Base64 zakódovaný PowerShell zanechává detailní forenzní stopy v **Prohlížeči událostí (Event ID 4104 a 400)** a před spuštěním prochází kontrolou **AMSI**. V bonusové části byl prozkoumán legitimní ADS **`Zone.Identifier` (MotW)** včetně jeho klíčové role při vyvolání **Microsoft SmartScreen** a způsobu jeho obejití u zranitelnosti **CVE-2023-24880**.
