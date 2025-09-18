# ......................................................................................
# ..............................Cvičení 1 - Pravděpodobnost.............................
# ..................Michal Béreš, Adéla Vrtková, Martina Litschmannová .................
# ......................................................................................

# Nezobrazuje-li se vám text korektně, nastavte File \ Reopen with Encoding... na UTF-8
# Pro zobrazení obsahu skriptu použijte CTRL+SHIFT+O
# Pro spouštění příkazů v jednotlivých řádcích použijte CTRL+ENTER

# V tomto cvičení projdeme úvod do pravděpodobnosti. Předpokládáme znalosti z přednášky,
# především pojmy:
#
#
#
# ** Základní prostor ####
# Základní prostor $\Omega$ je množina všech možných výsledků nějakého náhodného pokusu.
#
# ** Jev ####
# Jev je libovolná podmnožina základního prostoru $\Omega$.
#
# ** Elementární jev ####
# Elementární jev je jev, který obsahuje pouze jeden prvek základního prostoru $\Omega$.
#
# ** Úplná množina vzájemně disjunktních jevů ####
# Množina jevů $A_1, A_2, \ldots, A_n$ je úplná množina vzájemně disjunktních jevů,
# pokud platí:
# 1. $A_1 \cup A_2 \cup \ldots \cup A_n = \Omega$
# 2. $A_i \cap A_j = \emptyset$ pro $i \neq j$
#
#
# * Definice pravděpodobnosti ####
# Pravděpodobnost je číslo, které přiřazuje každému jevu $A$ číslo $P(A)$, splňující
# následující podmínky:
# 1. $0 \leq P(A) \leq 1$
# 2. $P(\Omega)=1$
# 3. Jestliže $A_1, A_2, \ldots$ jsou navzájem disjunktní jevy, pak $P(A_1 \cup A_2 \cup
# \ldots)=P(A_1)+P(A_2)+\ldots$
#
#
# * Věta o podmíněné pravděpodobnosti ####
# $$P(A|B)=\frac{P(A \cap B)}{P(B)}, \quad P(B) \neq 0$$
#
# * Úplná pravděpodobnost ####
# $$P(A)=\sum_{i=1}^{n}P(B_i)P(A|B_i), \quad P(B_i) \neq 0, \forall i = 1,\ldots,n$$
#
# * Bayesova věta ####
# $$P(B_k|A)=\frac{P(B_k)P(A|B_k)}{\sum_{i=1}^{n}P(B_i)P(A|B_i)}, \quad P(B_i) \neq 0,
# \forall i = 1,\ldots,n$$


# ---
#  Úlohy k procvičení ####
# ---
# * Příklad 1. ####
# Určete pravděpodobnost, že při hodu 20stěnnou spravedlivou (férovou) kostkou padne
# číslo větší než 14.


omega <- 1:20
omega
A <- c(15, 16, 17, 18, 19, 20)
A
# pravděpodobnost jako podíl příznivých ku všem
length(A) / length(omega)


# * Příklad 2. ####
# Určete pravděpodobnost, že při hodu 20stěnnou kostkou padne číslo větší než 14,
# víte-li, že sudá čísla padají 2x častěji než lichá.


# 2*P_liche = P_sude
# sum(P) = 1
P_liche <- 1 / 30
P_sude <- 2 * P_liche

P_A <- P_liche * 3 + P_sude * 3
P_A


# * Příklad 3. ####
# Určete pravděpodobnost, že ve sportce uhodnete 4 čísla. (Ve sportce se losuje 6 čísel
# ze 49.)


# kombinace jsou choose v R

# musíme trefit 4 z 6 a 2 z 43

(choose(6, 4) * choose(43, 2)) / choose(49, 6)


# * Příklad 4. ####
# Z abecedního seznamu studentů zapsaných na dané cvičení vybere učitel prvních 12 a
# nabídne jim sázku: „Pokud se každý z Vás narodil v jiném znamení zvěrokruhu, dám
# každému z Vás 100 Kč. Pokud jsou však mezi Vámi alespoň dva studenti, kteří se
# narodili ve stejném znamení, dá mi každý z Vás 100 Kč.“ Vyplatí se studentům sázku
# přijmout? S jakou pravděpodobností studenti vyhrají?


# kolik možností znamení může mít 12 studentů?
# kolik z nich vede k tomu, že každý student má jiné znamení?

factorial(12) / 12^12


# * Příklad 5. ####
# Spočtěte pravděpodobnost toho, že z bodu 1 do bodu 2 bude protékat elektrický proud,
# je-li část el. obvodu včetně pravděpodobnosti poruch jednotlivých součástek vyznačen
# na následujícím obrázku. (Poruchy jednotlivých součástek jsou na sobě nezávislé.)
# Obrázek viz sbírka úloh.


# rozdělíme na bloky I=(A,B) a II=(C,D,E)
PI <- 1 - (1 - 0.1) * (1 - 0.3)
PI
PII <- 0.2 * 0.3 * 0.2
PII
# výsledek
(1 - PI) * (1 - PII)


# * Příklad 6. ####
# Ohrada má obdélníkový tvar, východní a západní stěna mají délku 40 m, jižní a severní
# pak 100 m. V této ohradě běhá kůň. Jaká je pravděpodobnost, že je k jižní stěně blíž
# než ke zbývajícím třem?


# geometrická pravděpodobnost
ohrada <- 40 * 100
# blize k jihu
blize_J <- 20 * 60 + 20 * 20
# pravdepodobnosti
blize_J / ohrada


# * Příklad 7. ####
# V nesymptomatické populaci je 0.001 % lidí s rakovinou. Klinické testy
# kvality diagnostické metody pro detekci rakoviny v nesymptomatické
# populaci ukázaly 99.9% senzitivitu a 99.5% specificitu této metody.
# - Jako osoba bez symptomů jste podstoupili test s pozitivním výsledkem.
# Jsou starosti na místě?


# T ... pozitivní výsledek testu
# R ... pacient má skutečně nemoc

# P(R) ... pravděpodobnost, že pacient má nemoc
# P(~R) ... pravděpodobnost, že pacient nemá nemoc
# P(T|R) ... pravděpodobnost, že test bude pozitivní, pokud pacient má nemoc (sensitivita)
# P(~T|R) ... pravděpodobnost, že test bude negativní, pokud pacient má nemoc
# P(~T|~R) ... pravděpodobnost, že test bude negativní, pokud pacient nemá nemoc (specificita)
# P(T|~R) ... pravděpodobnost, že test bude pozitivní, pokud pacient nemá nemoc


P_R <- 0.00001
P_notR <- 1 - P_R
P_T_R <- 0.999
P_notT_R <- 1 - P_T_R
P_notT_notR <- 0.995
P_T_notR <- 1 - P_notT_notR

# P(R|T) ... pravděpodobnost, že jsme skutečně nemocní, pokud test byl pozitivní
P_R_T <- (P_T_R * P_R) / (P_T_R * P_R + P_T_notR * P_notR)
P_R_T * 100


# * Příklad 8. ####
# Telegrafické znaky se skládají ze signálů „tečka“, „čárka“. Je statisticky zjištěno,
# že se zkomolí 25 % sdělení „tečka“ a 20 % signálů „čárka“. Dále je známo, že signály
# se používají v poměru 3:2. Určete pravděpodobnost, že byl přijat správně signál,
# jestliže byl přijat signál „tečka“.


# OT ... odeslána tečka
# OC ... odeslána čárka
# PT ... přijata tečka
# PC ... přijata čárka

P_OT <- 3 / 5 # P(OT)
P_OC <- 2 / 5 # P(OC)

P_PC_OT <- 0.25 # P(PC|OT) ... zkomolení tečky
P_PT_OT <- 1 - P_PC_OT # P(PT|OT)

P_PT_OC <- 0.2 # P(PT|OC) ... zkomolení čárky
P_PC_OC <- 1 - P_PT_OC # P(PC|OC)

# P(OT|PT) ... pravděpodobnost, že byla odeslána tečka, pokud byla přijata tečka
P_OT_PT <- (P_PT_OT * P_OT) / (P_PT_OT * P_OT + P_PT_OC * P_OC)
P_OT_PT


# * Příklad 9. ####
# V jednom městě jezdí 85 % zelených taxíků a 15 % modrých. Svědek dopravní nehody
# vypověděl, že nehodu zavinil řidič modrého taxíku, který pak ujel. Testy provedené za
# obdobných světelných podmínek ukázaly, že svědek dobře identifikuje barvu taxíku v 80
# % případů a ve 20 % případů se mýlí.
#  - Jaká je pravděpodobnost, že viník nehody skutečně řídil modrý taxík?
#  - Následně byl nalezen další nezávislý svědek, který rovněž tvrdí, že taxík byl
# modrý. Jaká je nyní pravděpodobnost, že viník nehody skutečně řídil modrý taxík?
#  - Ovlivní pravděpodobnost, že viník nehody skutečně řídil modrý taxík to, zda dva
# výše
# zmínění svědci vypovídali postupně nebo najednou?


# 1.

# ZZ ... nehodu skutečně způsobil zelený taxík
# ZM ... nehodu skutečně způsobil modrý taxík
# OZ ... svědek označil zelený taxík
# OM ... svědek označil modrý taxík

P_ZM <- 0.15 # P(ZM) - pravděpodobnost, že nehodu skutečně způsobil modrý taxík
P_ZZ <- 0.85 # P(ZZ) - pravděpodobnost, že nehodu skutečně způsobil zelený taxík

P_OM_ZM <- 0.8 # P(OM|ZM) - nehodu způsobil modrý a svědek správně označil modrý
P_OZ_ZM <- 1 - P_OM_ZM # P(OZ|ZM) - nehodu způsobil modrý, ale svědek označil zelený

P_OZ_ZZ <- 0.8 # P(OZ|ZZ) - nehodu způsobil zelený a svědek správně označil zelený
P_OM_ZZ <- 1 - P_OZ_ZZ # P(OM|ZZ) - nehodu způsobil zelený, ale svědek označil modrý

# P(ZM|OM) ... pravděpodobnost, že nehodu skutečně způsobil modrý, pokud svědek označil modrý taxík
P_ZM_OM <- (P_OM_ZM * P_ZM) / ((P_OM_ZM * P_ZM) + (P_OM_ZZ * P_ZZ))
P_ZM_OM


# 2. nebo odpověděli najednou

# ZZ ... nehodu skutečně způsobil zelený taxík
# ZM ... nehodu skutečně způsobil modrý taxík
# OZ1, OZ2 ... první a druhý svědek označili zelený taxík
# OM1, OM2 ... první a druhý svědek označili modrý taxík

P_ZM <- 0.15 # P(ZM)
P_ZZ <- 0.85 # P(ZZ)

P_OM1_ZM <- 0.8 # P(OM1|ZM) - první svědek správně označil modrý
P_OZ1_ZM <- 1 - P_OM1_ZM # P(OZ1|ZM)

P_OZ1_ZZ <- 0.8 # P(OZ1|ZZ) - první svědek správně označil zelený
P_OM1_ZZ <- 1 - P_OZ1_ZZ # P(OM1|ZZ)

P_OM2_ZM <- 0.8 # P(OM2|ZM) - druhý svědek správně označil modrý
P_OZ2_ZM <- 1 - P_OM2_ZM # P(OZ2|ZM)

P_OZ2_ZZ <- 0.8 # P(OZ2|ZZ) - druhý svědek správně označil zelený
P_OM2_ZZ <- 1 - P_OZ2_ZZ # P(OM2|ZZ)

# P(OM1, OM2 | ZM) = P(OM1 | ZM) * P(OM2 | ZM) (nezávislí svědci)
P_OM1OM2_ZM <- P_OM1_ZM * P_OM2_ZM

# P(OM1, OM2 | ZZ) = P(OM1 | ZZ) * P(OM2 | ZZ)
P_OM1OM2_ZZ <- P_OM1_ZZ * P_OM2_ZZ

# P(ZM | OM1, OM2) ... pravděpodobnost, že nehodu skutečně způsobil modrý, pokud oba svědci označili modrý taxík
P_ZM_OM1OM2 <- (P_OM1OM2_ZM * P_ZM) / ((P_OM1OM2_ZM * P_ZM) + (P_OM1OM2_ZZ * P_ZZ))
P_ZM_OM1OM2


# 2. druhý průchod Bayesem

# ZZ ... nehodu skutečně způsobil zelený taxík
# ZM ... nehodu skutečně způsobil modrý taxík
# OZ2 ... druhý svědek označil zelený taxík
# OM2 ... druhý svědek označil modrý taxík

P_ZM <- P_ZM_OM # Výsledek z první úlohy
P_ZZ <- 1 - P_ZM

P_OM2_ZM <- 0.8 # P(OM2|ZM) - druhý svědek správně označil modrý
P_OZ2_ZM <- 1 - P_OM2_ZM # P(OZ2|ZM)

P_OZ2_ZZ <- 0.8 # P(OZ2|ZZ) - druhý svědek správně označil zelený
P_OM2_ZZ <- 1 - P_OZ2_ZZ # P(OM2|ZZ)

# P(ZM | OM2, OM) ... pravděpodobnost, že nehodu skutečně způsobil modrý, pokud oba svědci označili modrý taxík
P_ZM_OM2 <- (P_OM2_ZM * P_ZM) / ((P_OM2_ZM * P_ZM) + (P_OM2_ZZ * P_ZZ))
P_ZM_OM2


# * Příklad 10. ####
# Potřebujeme zjistit odpověď na určitou citlivou otázku. Jak odhadnout, kolik procent
# dotazovaných na otázku odpoví ANO a přitom všem respondentům zaručit naprostou
# anonymitu? Jedním z řešení je tzv. dvojitě anonymní anketa:
#
# Necháme respondenty hodit korunou a dvojkorunou a ti, kterým padl na koruně líc
# napíšou na lísteček odpověď (ANO/NE) na citlivou otázku. Ostatní respondenti napíší,
# zda jim padl na dvojkoruně líc (ANO/NE). Jakým způsobem určíme podíl studentů, kteří
# na citlivou otázku odpověděli ANO?
#
# Předpokládejme, že respondenti byli dotazování, zda podváděli u zkoušky. Z anketních
# lístků se zjistilo, že „ANO“ odpovědělo 120 respondentů a „NE“ odpovědělo 200
# respondentů. Kolik procent studentů podvádělo u zkoušky?


# PC ... respondent odpovídá na citlivou otázku
# PP ... respondent odpovídá podle hodu druhou mincí
# ANO ... odpověď ANO na anketním lístku
# NE ... odpověď NE na anketním lístku

P_PC <- 0.5 # Pravděpodobnost, že respondent odpovídá na citlivou otázku
P_PP <- 0.5 # Pravděpodobnost, že respondent odpovídá podle hodu druhou mincí

P_ANO_PP <- 0.5 # Pravděpodobnost, že respondent odpovídající hodem mince napíše ANO

# Získaná data z anketních lístků:
N_ANO <- 120 # Počet odpovědí ANO
N_NE <- 200 # Počet odpovědí NE
N_total <- N_ANO + N_NE # Celkový počet respondentů

# Pozorovaná celková pravděpodobnost odpovědi ANO:
P_ANO_obs <- N_ANO / N_total # 120 / 320 = 0.375

# Celková pravděpodobnost odpovědi ANO:
# P(ANO) = P(PC) * P(ANO|PC) + P(PP) * P(ANO|PP)
# P(ANO) = P_PC * P_ANO_PC + P_PP * P_ANO_PP
# Řešíme rovnici: P_ANO_obs = 0.5 * P_ANO_PC + 0.5 * 0.5
# Tedy: P_ANO_PC = (P_ANO_obs - 0.5 * 0.5) / 0.5

P_ANO_PC <- (P_ANO_obs - P_PP * P_ANO_PP) / P_PC
P_ANO_PC


# ---
#
# * Bonus - Monty Hall Problem ####
#
# Monty Hall problém je známý statistický "paradox" založený na herní show, kde
# soutěžící vybírá mezi třemi dveřmi. Za jedněmi dveřmi se skrývá automobil (hlavní
# cena) a za zbývajícími dvěma dveřmi jsou kozy.
#
# **Popis soutěže (problému):**
#
# 1. **Výběr:** Soutěžící si vybere jedny ze tří dveří.
# 2. **Otevření:** Moderátor, který zná umístění ceny, otevře jedny z nezvolených dveří.
# Vždy odhalí kozu.
# 3. **Volba:** Soutěžící dostane možnost buď setrvat u původního výběru, nebo změnit
# volbu na druhé původně nezvolené dveře.
# 4. **Otázka:** Jaká strategie (setrvat nebo změnit) maximalizuje šanci na výhru
# automobilu?
#
# ---
#
# Je asi zřejmé, že v prvním kole je pravděpodobnost, že dveře zvolené soutěžícím
# obsahují automobil $ \frac{1}{3} $.
#
# Mohlo by se zdát, že po moderátorově otevření dveří s kozou se situace "resetuje" a
# zbývají dvě dveře mají pravděpodobnost $ \frac{1}{2} $ na automobil za nimi. **To však
# není správný závěr.** Tím, že moderátor otevřel jedny dveře, nám ve skutečnosti
# poskytl dodatečnou informaci, kterou je třeba započítat do výpočtu pravděpodobností.
#
# ***Výsledek se dá spočítat pomocí podmíněné pravděpodobnosti:***
# - pro jednoduchost si označíme následující jevy:
#     - $D_1$ - auto je za prvními dveřmi
#     - $D_2$ - auto je za druhými dveřmi
#     - $D_3$ - auto je za třetími dveřmi
# - pokud bychom neměli žádnou informaci, tak $$P(D_1) = P(D_2) = P(D_3) =
# \frac{1}{3}$$
# - bez újmy na obecnosti předpokládejme, že soutěžící si vybral dveře číslo 1 a
# moderátor otevřel dveře číslo 3
# - tedy nastal jev $M$ - moderátor otevřel dveře číslo 3 s kozou z výběru dveří číslo 2
# a 3
# - nyní už pracujeme s podmíněnými pravděpodobnostmi $P(D_1|M)$, $P(D_2|M)$ a
# $P(D_3|M)$ (asi je už jasné, že $P(D_3|M) = 0$)
# - pojďme si vyjádřit $P(D_1|M)$ pomocí Bayesovy věty: $$P(D_1|M) =
# \frac{P(M|D_1)P(D_1)}{P(M|D_1)P(D_1) + P(M|D_2)P(D_2)+ P(M|D_3)P(D_3)}$$
#     - co vlastně znamená $P(M|D_1)$, $P(M|D_2)$ a $P(M|D_3)$?
#     - $P(M|D_1)$ - je pravděpodobnost, že moderátor otevře dveře číslo 3, za
# předpokladu, že automobil je za dveřmi číslo 1. Tedy moderátor mohl otevřít dveře
# číslo 2 nebo 3 (oba obsahují kozy). Tedy $$P(M|D_1) = \frac{1}{2}$$
#     - $P(M|D_2)$ - je pravděpodobnost, že moderátor otevře dveře číslo 3, za
# předpokladu, že automobil je za dveřmi číslo 2. Tedy moderátor mohl otevřít pouze a
# jenom dveře číslo 3. Tedy $$P(M|D_2) = 1$$
#     - $P(M|D_3)$ - je pravděpodobnost, že moderátor otevře dveře číslo 3, za
# předpokladu, že automobil je za dveřmi číslo 3. Což je nemožné, protože moderátor by
# otevřel s cenou. Tedy $$P(M|D_3) = 0$$
# - dosadíme do vzorce číselné hodnoty a dostaneme $$P(D_1|M) = \frac{\frac{1}{2} \cdot
# \frac{1}{3}}{\frac{1}{2} \cdot \frac{1}{3} + 1 \cdot \frac{1}{3}+ 0 \cdot \frac{1}{3}}
# = \frac{1}{3}$$
# - obodobně můžeme spočítat $P(D_2|M) = \frac{2}{3}$
# - Tedy výherní strategie je vždy provést změnu dveří, což zvyšuje pravděpodobnost
# výhry na $ \frac{2}{3} $.


# ---
# **Kdo nevěří, může si vyzkoušet krátkou simulaci:**


n <- 10000 # počet pokusů
cena <- sample.int(n = 3, size = n, replace = TRUE) # náhdný výběr dveří
head(cena) # head vykresli prvních 6 prvků/řádků


# Totéž pro naši původní volbu - náhodný index dveří.


volba_orig <- sample.int(n = 3, size = n, replace = TRUE) # původní volba
head(volba_orig)


# V prvním kole moderátor jedny prázdné dveře otevře, takto se to dá nasimulovat:


otevrene_dvere <- rep(0, n) # inicializace vektoru
dvere_c <- 1:3 # pomocná proměnná - identifikátory dveří
for (i in 1:n) {
    dvere_k_otevereni <- c(TRUE, TRUE, TRUE) # inicializace
    dvere_k_otevereni[cena[i]] <- FALSE # nesmíme otevřít dveře s cenou
    dvere_k_otevereni[volba_orig[i]] <- FALSE # ani naše vybrané dveře
    # ve zbytku jsou buď 2 (pokud jsme se trefili) nebo 1 dveře (pokud ne)
    idx_dvere <- dvere_c[dvere_k_otevereni]
    if (length(idx_dvere) == 1) {
        otevrene_dvere[i] <- idx_dvere # pokud jedny otevřeme je
    } else { # pokud 2 tak jedny náhodně vybereme a otevřeme je
        otevrene_dvere[i] <- sample(x = idx_dvere, size = 1)
    }
}
head(otevrene_dvere)


# Naše nová volba pokud se tak rozhodneme - součet indexů je 1+2+3=6 takže pokud my máme
# vybraný nějaký index, dále nějaký index se otevře, tak do zbytku 6 jsou ty třetí =
# naše nová volba.


nova_volba <- 6 - (volba_orig + otevrene_dvere)
head(nova_volba)


# Úspěšnost při originální volbě:


p_orig <- sum(cena == volba_orig) / n
p_orig


# Úspěšnost při výměně:


p_zmena <- sum(cena == nova_volba) / n
p_zmena


p_orig + p_zmena
