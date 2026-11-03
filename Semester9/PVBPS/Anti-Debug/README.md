Zadání úlohy

1)	Seznamte se s problematikou anti-analytických technik malwaru. Pozornost věnujte tomu, proč malware potřebuje rozpoznat své prostředí, jaké informace k tomu potřebuje a jaké důsledky mají tyto techniky pro dynamickou analýzu.



2)	Navrhněte a implementujte jednoduchý program v jazyce C podle následujících požadavků:

a)	Musí být schopen detekovat běh ve virtuálním prostředí na základě informací o BIOSu/systémovém firmwaru (např. SMBIOS/DMI údaje, řetězce výrobce jako VMware, VirtualBox, innotek GmbH, QEMU, Bochs apod.).

b)	Musí implementovat minimálně jednu techniku detekce debuggeru (např. IsDebuggerPresent, ověření flagu BeingDebugged v PEB, NtQueryInformationProcess s ProcessDebugPort, případně měření času).

c)	Musí obsahovat dva zřetelně odlišné režimy chování. Jeden pro běh ve VM nebo pod debuggerem a druhý pro reálný cíl (fyzický stroj, bez debuggeru).

d)	Zdrojový kód přiložte v samostatném souboru k protokolu.



3)	Ověřte funkčnost jednotlivých technik a zdokumentujte postup s výstřižky příslušných oken s titulky:

a)	Spusťte program na fyzickém počítači nebo v běžném systému mimo debugger a zdokumentujte očekávané chování.

b)	Spusťte program ve virtuálním prostředí (VirtualBox, VMware nebo jiný hypervizor) a ověřte, že detekce VM fungovala - popište, jak jste zjištění ověřili (např. výpisem zjištěných údajů o BIOSu, změnou chování vzorku).

c)	Spusťte program pod debuggerem (x64dbg, WinDbg, Visual Studio Debugger aj.) a ověřte, že antidebugovací technika fungovala - popište, čím se chování pod debuggerem lišilo.



4)	Analyzujte svůj program v sandboxu a zhodnoťte, zda byla vaše anti-analytická opatření vůči analýze účinná. Nahrajte program do online verze Cuckoo Sandbox:

a)	Přiložte webový odkaz na Cuckoo report své analýzy.

b)	Uveďte hash SHA256 analyzovaného vzorku.

c)	Zjistěte, jaké skóre analýza vzorku přidělila a vypište, co analýza zachytila (spuštěné procesy, načtené knihovny, přístupy do registru, souborové operace, síťová komunikace). Své poznatky popište a přiložte výstřižky oken, na které se budete v textu odkazovat.

d)	Zhodnoťte, zda z Cuckoo reportu poznáte, jestli program detekuje virtuální prostředí či debugger. Když ano, tak podle čeho a pokud ne, tak proč.

e)	Na základě reportu určete, ve kterém „režimu“ se program v sandboxu pravděpodobně choval a zda se toto chování shoduje s vašimi lokálními testy z bodu 3.



5)	Podařilo se vám analýzu oklamat? Co by analytik musel udělat, aby vaše techniky odhalil (např. jaké nástroje nebo jakou změnu prostředí)?



6)	(+ 3 body BONUS) Rozložte váš přeložený program disassemblerem (např. objdump, Ghidra, x64dbg) a v disassemblovaném kódu vyhledejte místa odpovídající vašim technikám, proveďte screeny a v protokolu je okomentujte. Porovnejte, zda se staticky nalezený kód shoduje s tím, co jste napsali ve zdrojovém kódu, a popište případné rozdíly. Následně zkuste některou z detekcí v binárce obejít (např. opatchováním podmíněného skoku) a ověřte, že se chování vzorku změnilo.

