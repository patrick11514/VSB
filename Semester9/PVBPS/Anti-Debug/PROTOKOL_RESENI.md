# Protokol k úloze 4: Anti-analytické techniky malwaru
**Předmět:** Počítačové viry a bezpečnost počítačových systémů (PVBPS)  
**Téma:** Detekce virtuálního prostředí a debuggeru v jazyce C  
**Výsledná binárka:** `main.exe` (x86_64 PE)  
**SHA-256:** `ae9d77a1cbdd405a5c8a780eb4b41fd78b95aa4ef4fd4f5d8c854131e8b5109f`

---

## 1. Teoretický rozbor anti-analytických technik

### Proč malware potřebuje rozpoznat své prostředí?
Dynamická analýza v bezpečnosti spoléhá na spuštění neznámého vzorku v izolovaném sandboxu (např. Cuckoo Sandbox, Any.Run, Hybrid Analysis) nebo pod dohledem analytika v debuggeru (x64dbg, WinDbg, IDA Pro). Cílem analýzy je zaznamenat chování vzorku: vytvářené soubory, modifikace registrů, síťovou komunikaci (C&C servery), injektáž kódu či eskalaci privilegií.

Pokud malware toto prostředí neodhalí, odhalí své záměry a analytici okamžitě vytvoří IoC (Indikátory kompromitace), YARA pravidla a antivirové signatury. Pokud však malware rozpozná, že je analyzován, přepne se do neškodného režimu (nebo se okamžitě ukončí / smaže). Sandbox jej pak vyhodnotí jako neškodný (falešně negativní výsledek) a k aktivaci škodlivého kódu dojde až na reálném stroji oběti.

### Jaké informace malware potřebuje?
1. **Příznaky virtualizace (Anti-VM):**
   - **BIOS / Firmware artefakty:** SMBIOS/DMI tabulky obsahující řetězce hypervizorů (`VMware`, `VirtualBox`, `QEMU`, `Bochs`, `KVM`, `Red Hat`, `innotek`).
   - **Registry Windows:** Klíče v `HARDWARE\DESCRIPTION\System\BIOS` (`SystemManufacturer`, `BIOSVendor`).
   - **Hardware & CPU artefakty:** Instrukce `cpuid` (hypervisor bit v ECX a vendor string jako `TCGTCGTCGTCG`, `KVMKVMKVM`, `VMwareVMware`), MAC adresy síťových karet (např. OUI `08:00:27` pro VirtualBox, `00:0C:29` pro VMware).
   - **Běžící procesy a služby:** `VBoxService.exe`, `vmtoolsd.exe`.

2. **Příznaky ladění (Anti-Debugging):**
   - **Win32 API:** `IsDebuggerPresent()`, `CheckRemoteDebuggerPresent()`.
   - **Přímo z paměťových struktur procesu (PEB):** Flag `BeingDebugged` na offsetu `0x2` v PEB (`gs:[0x60]` na x64), `NtGlobalFlag` na offsetu `0xBC`.
   - **Interní volání jádra:** `NtQueryInformationProcess` s třídou `ProcessDebugPort` (hodnota `-1` signalizuje debugger).
   - **Časové anomálie (Timing checks):** Měření latence mezi instrukcemi pomocí `rdtsc` – krok breakpointu v debuggeru trvá miliony cyklů oproti normálnímu běhu.

### Důsledky pro dynamickou analýzu
- Automatizované sandboxy selhávají, protože vzorek neprovede žádnou škodlivou akci.
- Analytik je nucen anti-analytické techniky ručně identifikovat a obejít (např. použitím pluginů jako ScyllaHide, úpravou BIOSu VM, nebo statickým patchováním binárky).

---

## 2. Návrh a implementace programu v jazyce C

Implementovaný program [main.c](file:///home/patrick115/Projects/VSB/Semester9/PVBPS/Anti-Debug/main.c) splňuje všechny požadavky zadání:

### a) Detekce virtuálního prostředí na základě BIOS / Firmware
Program kombinuje dvě techniky:
1. **Inspekce SMBIOS / DMI tabulek pomocí Windows API `GetSystemFirmwareTable`:**
   - Volá `GetSystemFirmwareTable('RSMB', 0, buffer, size)`, kde `'RSMB'` je signatura poskytovatele SMBIOS.
   - Prohledává surová data tabulky SMBIOS na přítomnost známých identifikátorů hypervizorů:
     `VMWARE`, `VIRTUALBOX`, `VBOX`, `INNOTEK`, `QEMU`, `BOCHS`, `RED HAT`, `KVM`, `XEN`, `HYPER-V`.
2. **Čtení údajů z registru Windows (`HARDWARE\DESCRIPTION\System\BIOS`):**
   - Čte hodnoty `SystemManufacturer`, `SystemProductName`, `BIOSVendor` a `BIOSVersion`.
   - V případě shody s výše uvedenými klíčovými slovy označí prostředí jako virtuální.
   - *Poznámka pro sandbox:* Cuckoo Sandbox přístupy k těmto klíčům registru monitoruje a eviduje v behavioral reportu.

### b) Detekce debuggeru
Program implementuje 3 na sobě nezávislé techniky:
1. **Volání Win32 API `IsDebuggerPresent()`** – standardní dotaz na debugger.
2. **Přímá kontrola příznaku `BeingDebugged` ve struktuře PEB (Process Environment Block):**
   - Využívá compiler intrinsic `__readgsqword(0x60)` pro 64bitové Windows (případně `__readfsdword(0x30)` pro 32bit).
   - Čte hodnotu na offsetu `0x2` struktury `PEB`. Pokud je hodnota rovna `1`, proces je laděn. Tato technika nevyužívá volání žádné API funkce, čímž obchází jednoduché API hookingy.
3. **Volání `CheckRemoteDebuggerPresent(GetCurrentProcess(), &isDebuggerPresent)`** – zjišťuje, zda je proces napojen na klientský debugger přes `ProcessDebugPort`.

### c) Dva odlišné režimy chování
1. **Režim `kind_way` (při detekci VM nebo Debuggeru):**
   - Na konzoli vypíše varování o detekovaném analytickém prostředí.
   - Škodlivý kód je potlačen.
   - Vytvoří neškodný reportovací soubor `kind.txt` obsahující potvrzení detekce.
2. **Režim `malicious_way` (reálný fyzický cíl bez debuggeru):**
   - Simuluje škodlivý payload malwaru.
   - Vytvoří soubor `malicious.txt` simulující instalaci a perzistenci škodlivého kódu.

---

## 3. Ověření funkčnosti a dokumentace testů

| Scénář | Očekávaný stav detekce | Vybraný režim | Vytvořený soubor |
| :--- | :--- | :--- | :--- |
| **A. Fyzický počítač (mimo debugger)** | Debugger: `Clear`, VM: `Clear` | `malicious_way` | `malicious.txt` |
| **B. Běh ve virtuálním stroji (VirtualBox / VMware)** | VM: `DETECTED (SMBIOS/Registry string)` | `kind_way` | `kind.txt` |
| **C. Běh pod debuggerem (x64dbg / winedbg)** | Debugger: `DETECTED (IsDebuggerPresent / PEB)` | `kind_way` | `kind.txt` |

### Postup ověření:
1. **Fyzický stroj:** Spuštěním `./main.exe` se v terminálu vypíší zelené stavy `[+] Clear` u všech testů a program oznámí:
   `[+] >>> SELECTED MODE: ACTIVE MALICIOUS PAYLOAD MODE <<<` a vytvoří `malicious.txt`.
2. **Virtuální prostředí:** Při spuštění ve VirtualBoxu funkce `check_registry_bios` nebo `check_smbios_firmware` zachytí řetězec `innotek GmbH` nebo `VirtualBox`. Program přepne na `kind_way` a vytvoří `kind.txt`.
3. **Debugger:** Při otevření v x64dbg (nebo spuštění přes `winedbg main.exe`) nahlásí `IsDebuggerPresent` i `PEB->BeingDebugged` stav `[!] DETECTED`. Program aktivuje `kind_way` a odmítne nasadit payload.

---

## 4. Analýza v sandboxu (Cuckoo / Any.Run / Hybrid Analysis)

- **Analyzovaný soubor:** `main.exe`
- **SHA-256:** `ae9d77a1cbdd405a5c8a780eb4b41fd78b95aa4ef4fd4f5d8c854131e8b5109f`
- **Typ binárky:** PE32+ executable (native x86_64 console Windows binary)

### Vyhodnocení reportu ze sandboxu:
1. **Zachycené operace:**
   - **Čtení registru:** Přístup do klíče `HKLM\HARDWARE\DESCRIPTION\System\BIOS` pro čtení hodnot `SystemManufacturer`, `BIOSVendor`.
   - **Volání API:** Detekováno volání `IsDebuggerPresent`, `GetSystemFirmwareTable`.
   - **Souborové operace:** Vytvoření souboru `kind.txt` v aktuálním adresáři.
2. **Výsledné chování v sandboxu:**
   - Protože sandboxy (např. Cuckoo) běží ve virtualizovaném hostiteli (typicky VirtualBox nebo KVM/QEMU), program v sandboxu **úspěšně detekoval virtuální prostředí přes BIOS/SMBIOS řetězce**.
   - Program se v sandboxu choval v **režimu `kind_way`** a vytvořil `kind.txt`.
   - Sandbox tak nezaznamenal žádné vytvoření `malicious.txt` ani simulovanou perzistenci, což dokazuje účinnost anti-analytického opatření.

---

## 5. Zhodnocení účinnosti a postup analytika při odhalení

### Podařilo se analýzu oklamat?
**Ano.** Automatizovaný sandbox nespustil škodlivý kód, protože jeho virtualizační signatury v SMBIOS/registrech vzorek varovaly.

### Co by analytik musel udělat, aby techniky odhalil a obešel?
1. **Úprava konfigurace virtuálního stroje (Hardening VM):**
   - Změna DMI/SMBIOS řetězců ve VirtualBoxu:
     `VBoxManage setextradata "<VM_NAME>" "VBoxInternal/Devices/pcbios/0/Config/DmiBIOSVendor" "American Megatrends Inc."`
     `VBoxManage setextradata "<VM_NAME>" "VBoxInternal/Devices/pcbios/0/Config/DmiSystemVendor" "Dell Inc."`
   - Odstranění artefaktů v registru Windows.
2. **Použití specializovaných pluginů pro debugger:**
   - Použití pluginu **ScyllaHide** v x64dbg, který automaticky hookuje `NtQueryInformationProcess`, nuluje flag `BeingDebugged` v PEB a falšuje výstupy `IsDebuggerPresent()`.
3. **Statické přepsání (Patching) binárky:**
   - Analytik v disassembleru (Ghidra, IDA, x64dbg) vyhledá podmíněný skok vyhodnocující detekci a přepíše jej instrukcemi `NOP` nebo invertuje podmínku.

---

## 6. (+3 body BONUS) Disassemblování a binární patchování

### Disassemblovaný kód (výstup `x86_64-w64-mingw32-objdump -M intel -d main.exe`)

#### 1. Kontrola `PEB->BeingDebugged` ve funkci `<check_peb_being_debugged>`:
```assembly
0000000140001640 <check_peb_being_debugged>:
   140001640:   31 c0                   xor    eax,eax
   140001642:   65 48 8b 14 25 60 00    mov    rdx,QWORD PTR gs:0x60   ; Načtení ukazatele na PEB z gs:[0x60]
   140001649:   00 00 
   14000164b:   48 85 d2                test   rdx,rdx
   14000164e:   75 08                   jne    140001658 <check_peb_being_debugged+0x18>
   140001650:   c3                      ret
   140001651:   0f 1f 80 00 00 00 00    nop    DWORD PTR [rax+0x0]
   140001658:   80 7a 02 01             cmp    BYTE PTR [rdx+0x2],0x1  ; Čtení BeingDebugged na offsetu 0x2
   14000165c:   0f 94 c0                sete   al                      ; al = 1 pokud je laděn
   14000165f:   c3                      ret
```
**Komentář:**
- Instrukce `mov rdx, QWORD PTR gs:0x60` odpovídá intrinsic funkci `__readgsqword(0x60)`.
- Instrukce `cmp BYTE PTR [rdx+0x2], 0x1` přesně odpovídá přístupu k položce `pPeb->BeingDebugged` na offsetu `0x2`.

#### 2. Podmíněné větvení v `<main>`:
```assembly
   1400036a1:   84 db                   test   bl,bl                   ; bl = výsledek is_debugged()
   1400036a3:   75 37                   jne    1400036dc <main+0x8c>   ; Pokud laděn -> skok na kind_way
   1400036a5:   40 84 f6                test   sil,sil                 ; sil = výsledek is_virtual_machine()
   1400036a8:   75 32                   jne    1400036dc <main+0x8c>   ; Pokud ve VM -> skok na kind_way
   1400036aa:   e8 81 e7 ff ff          call   140001e30 <malicious_way>; BĚŽÍ POUZE PŘI NEÚSPĚCHU DETEKCÍ!
```

### Provedení binárního patche (Bypass detekce)
Pro vyřazení detekcí stačí nahradit podmíněné skoky `jne` (`0x75 0x37` a `0x75 0x32`) instrukcemi `NOP NOP` (`0x90 0x90`):
- Na offsetu v souboru `0x2ca1`:
  - Původní bajty: `84 db 75 37 40 84 f6 75 32`
  - Opatchované bajty: `84 db 90 90 40 84 f6 90 90`

### Ověření opatchované binárky (`main_patched.exe`):
Při spuštění `main_patched.exe` pod debuggerem `winedbg`:
- Detekce sice stále proběhne a vypíše `[!] DETECTED`, ale procesor přeskočí skok na `kind_way` a pokračuje přímo do `malicious_way()`.
- Výsledkem je vytvoření souboru **`malicious.txt` přímo uvnitř debuggeru**.
- Tímto byl experimentálně ověřen úspěšný bypass obou ochranných mechanismů!

---

## 7. Závěr

V rámci úlohy byl v jazyce C navržen a zkompilován vzorek malwaru, který kombinuje kontrolu debuggeru (přes Win32 API i přímým přístupem do paměťové struktury PEB) s detekcí hypervizorů prostřednictvím systémového firmwaru (SMBIOS/DMI a registry). 

Program prokazatelně mění své chování v závislosti na prostředí – v analýze či virtuálním stroji potlačuje škodlivou činnost (`kind_way`), zatímco na reálném stroji simuluje nasazení škodlivého kódu (`malicious_way`). 

V rámci bonusového úkolu byla binárka rozebrána v disassembleru, byly přesně identifikovány klíčové instrukce kontroly prostředí a pomocí binárního patche (`NOP` podmíněných skoků) byl demonstrován postup analytika pro prolomení těchto anti-analytických ochran.
