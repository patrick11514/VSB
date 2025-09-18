# ......................................................................................
# .................. Cvičení 6. Preprocesing dat a explorační analýza ..................
# .................. Michal Béreš, Adéla Vrtková, Martina Litschmannová.................
# ......................................................................................

# Nezobrazuje-li se vám text korektně, nastavte File \ Reopen with Encoding... na UTF-8
# Pro zobrazení obsahu skriptu použijte CTRL+SHIFT+O
# Pro spouštění příkazů v jednotlivých řádcích použijte CTRL+ENTER

#   1. Rozšiřující balíčky funkcí - instalace a načítání ####


# Instalování balíčků nutné pouze jednou (pokud je již nemáte)
# install.packages("readxl")
# install.packages("dplyr")
# install.packages("openxlsx")


# Načtení balíčku (nutno opakovat při každém novém spuštění Rka, vhodné mít na začátku skriptu, pro kontrolu
# co všechno se používá)

library(dplyr) # balíček pro manipulaci s daty

# výpis obsahuje upozornění na přepsané funkce případně na starší verzi balíčku

# Právě kvůli maskování (přepisování existujících funkcí) je dobré omezit kompletní
# načtení balíčků na minimum.
#
# Často potřebujeme pouze jednu funkci z balíčku, pak lze tato funkce načíst pomocí
# `::`.
#
# Ukážeme si později.


# ---


#  2. Pracovní adresář (working directory) - odkud načítáme a kam ukládáme data ####
# - Pozor aktuální otevřená složka v Rstudiu, případně umístění Rskriptu není
# automaticky pracovní adresář!


# Výpis pracovního adresáře
getwd()


# Nastavení pracovního adresáře -> do uvozovek, celou cestu (relativní nebo absolutní)
setwd("./data")


getwd() # kde jsme teď


setwd("./..") # zase zpátky


getwd() # kontrola


# ---


#  3. Načítání datových souborů ####
#
# - pro ověření správnosti načtení budeme použífat funkci `head()` která vypíše prvních
# 6 řádků datového souboru


# ** a) Ze souboru CSV ####
# Základní funkce - `read.table`, `read.csv`, `read.csv2`, ...
#
# Záleží hlavně na formátu souboru (.txt, .csv), na tzv. oddělovači jednotlivých hodnot,
# desetinné čárce/tečce, ...


# pro data oddělené středníkem lze použít read.csv2 s defaultním nastavením
# načteme data relativně k aktuálnímu adresáři, tedy ze složky data
data <- read.csv2(file = "./data/zarovky.csv")
head(data)

# pokud má soubor jiný formát, lze upravit parametry načítání
data <- read.csv2(file = "./data/zarovky.csv", sep = ";", quote = "", skip = 0, header = TRUE)
head(data)


# cesta souboru může být relativní, absulutní nebo lze použít i URL
# data <- read.csv2(file = "C:/Users/username/Documents/aku.csv")
# data <- read.csv2(file = "./data/aku.csv")
# data <- read.csv2(file = "http://am-nas.vsb.cz/lit40/DATA/aku.csv")


# ** b) z Excelu (souboru xlsx) ####
# K načtení/uložení datového souboru ve formátu xlsx použijeme funkci z balíčku `readxl`
# - konkrétně funkci `readxl::read_excel()`


# při načítání dat z Excelu si sice nemusíme dělat starosti s oddělovači a formátem čísel
# musíme však specifikovat který list chceme načíst a případně kolik řádků od začátku chceme přeskočit
data <- readxl::read_excel("./data/zarovky.xlsx", sheet = "Data", skip = 3)
head(data)

# ** c) Odstranění nepotřebných sloupců a jejich přejmenování pro snadnější adresování ####
# dat


# indexování se zápornými indexy vrátí vše kromě hodnoty indexů
# nelze míchat záporné a kladné indexy!
data <- data[, -1] # odstraníme první sloupec s indexy
head(data)


# Přejmenování sloupců (obecně diakritika a mezery v názvech sloupců komplikují práci)
colnames(data) <- c("A5", "B5", "C5", "D5", "A22", "B22", "C22", "D22")
head(data)


# **Poznámka:**
#
# V R jsou aktuálně dva formáty reprezentace tabulek: `data.frame` a `tibble`.
#
# Objekt importovaný touto cestou bude v novém RStudiu jako typ "tibble".
#
# Jedná se o modernější "data.frame" a v některých funkcích může dělat problémy a házet
# errory!
# Jednoduše lze tento objekt převést na typ data.frame pomocí **as.data.frame()**


# ---


#  4. Standardní datový formát (a převod do něj) ####
#
# Standardní datový formát (v **R** tzv. *"long format"*) se vyznačuje tím, že:
#
# - **Každý řádek tabulky obsahuje jednu pozorovanou entitu**, například:
#   - jednoho pacienta,
#   - jednu žárovku,
#   - jednoho studenta, atd.
#
#   To znamená, že data o produktech dvou firem **nemohou** být v jednom řádku – místo
# toho se přidá samostatný sloupec s identifikátorem firmy.
#
# - **V jednom sloupci jsou vždy data jednoho typu**:
#   - Je vhodné mít **jeden samostatný sloupec pro `id` entity** (například pro
# odstranění odlehlých pozorování). Například pomocí: ```data$id <-
# seq_along(data$...)``` nebo s pomocí balíčku `dplyr` a funkce `mutate()`: ```data <-
# data %>% mutate(id = row_number())```


# ** a) Základní převod jednoduché datové matice do standardního datového formátu  ####-
# `stack(...)`


data5 <- data[, 1:4] # z dat vybereme ty sloupce, které odpovídají měřením po 5 cyklech
colnames(data5) <- c("Amber", "Bright", "Clear", "Dim") # přejmenujeme sloupce
head(data5)


data5S <- stack(data5) # a převedeme do st. datového formátu
head(data5S)


# přejmenujeme sloupce
colnames(data5S) <- c("tok5", "vyrobce")

# a přidáme slopec is id
data5S$id <- seq_along(data5S$tok5)

head(data5S)

#
# ** b) Z dat ve formátu Datová matice `reshape(...)` ####


data_DM <- readxl::read_excel("./data/datova_matice.xlsx")
head(data_DM)


# smažeme první sloupec
data_DM <- data_DM[, -1]
# přejmenujeme sloupce
colnames(data_DM) <- c("A22", "A5", "B22", "B5", "C22", "C5", "D22", "D5")
head(data_DM)


# *** Funkce `reshape`:   ####
# ```r
# reshape(data, direction, varying, v.names, times, timevar)
# ```
#
# - **`data`** – Data k převedení musí být ve formátu `data.frame` nebo `tibble`.
# - **`direction`** – Směr transformace:
#   - `"long"` – převod do standardního formátu (*long format*).
#   - `"wide"` – převod zpět do datové matice (*wide format*).
# - **`varying`** – Názvy sloupců, které označují stejná data pro různé kategorie:
#   - Je to **seznam vektorů** (`list`).
#   - Každá položka listu odpovídá **jednomu měření**.
#   - Každý vektor obsahuje seznam odpovídajících sloupců.
# - **`v.names`** – Názvy sloupců ve standardním datovém formátu:
#   - Počet názvů musí odpovídat počtu vektorů ve `varying`.
# - **`times`** – Názvy jednotlivých kategorií:
#   - ⚠ **Musí být ve stejném pořadí jako odpovídající sloupce ve `varying`!**
# - **`timevar`** – Název sloupce obsahujícího kategorie.


dataS <- reshape(
    data = as.data.frame(data_DM),
    direction = "long",
    varying = list(
        c("A5", "B5", "C5", "D5"),
        c("A22", "B22", "C22", "D22")
    ),
    v.names = c("tok5", "tok22"),
    times = c("Amber", "Bright", "Clear", "Dim"),
    timevar = "vyrobce"
)
head(dataS)


# ** c) Z datového souboru, kde jsou kategorie (např. výrobci) v jednotlivých listech ####
# Excelu
#
# V tomto případě bohužel **nelze využít vestavěnou funkci**, která by provedla operaci
# automaticky. Musíme to provést ručně následujícím postupem:
#
# 1. **Načteme všechny listy Excelu** do samostatných proměnných.
# 2. **Ke každé proměnné přidáme sloupec s kategorií** (např. výrobce).
# 3. **Spojíme všechny proměnné do jedné tabulky** pomocí funkce:
#    ```r
#    rbind(data1, data2, data3, ...)
#     ```


# načteme všechny listy do separátních proměnných
data_A <- readxl::read_excel("./data/po_listech.xlsx", sheet = 1)
head(data_A)
data_B <- readxl::read_excel("./data/po_listech.xlsx", sheet = 2)
data_C <- readxl::read_excel("./data/po_listech.xlsx", sheet = 3)
data_D <- readxl::read_excel("./data/po_listech.xlsx", sheet = 4)


# přidáme sloupec s výrobcem
data_A$vyrobce <- "Amber"
data_B$vyrobce <- "Bright"
data_C$vyrobce <- "Clear"
data_D$vyrobce <- "Dim"
head(data_A)


# sloučíme všechny listy do jednoho pomocí rbind
# v prvním sloupci je sice id, ale je jen vůči danému listu, ne vůči celé tabulce
dataS2 <- rbind(data_A, data_B, data_C, data_D)
dataS2 <- dataS2[, -1]
tail(dataS2)


# přejmenujeme sloupce
colnames(dataS2) <- c("tok5", "tok22", "vyrobce")
# přidáme sloupec s id
dataS2$id <- seq_along(dataS2$tok5)
head(dataS2)

# ---


#  5. Knihovna Dplyr ####


# ** a) Přehled funkcí knihovny `dplyr` ####
#
# - **`%>%`** je tzv. pipe operátor. Typické využití je ve tvaru:
#   ```r
#   res = data %>% operace
#   ```
#   kde výsledkem je aplikovaná operace na `data`.
#     - v Rstudio lze zapsat jako `Ctrl + Shift + M`
#
# - **`select(...)`** slouží k výběru sloupců v datovém rámci:
#   - `select(1)` – vybere první sloupec.
#   - `select(A5)` – vybere sloupec se jménem `A5`.
#   - `select(1,3,5)` – vybere sloupce 1, 3 a 5.
#
# - **`mutate(novy_sloupec = ...)`** vytvoří v datovém rámci nový sloupec pomocí výpočtu
# nad existujícími sloupci:
#   ```r
#   data %>% mutate(C = A - B)
#   ```
#   Tento příkaz vytvoří v datovém rámci `data` nový sloupec `C` jako rozdíl hodnot ve
# stávajících sloupcích `A` a `B`.
#
# - **`filter(...)`** vyfiltruje záznamy splňující zadané podmínky:
#   - `data %>% filter(vyrobce == "A" | vyrobce == "B")`
#     Vrátí datový rámec, kde sloupec `vyrobce` obsahuje pouze hodnoty `"A"` nebo `"B"`.
#   - `data %>% filter(vyrobce == "A", hodnoty > 1000)`
#     Pokud jsou podmínky odděleny čárkou, chápeme je jako **"a zároveň"**.
#
# - **`summarise(...)`** vypočítá číselné charakteristiky v rámci zadaných sloupců
# (vhodné pro kombinaci s `group_by`):
#   ```r
#   data %>% summarise(prum = mean(kap5), median = median(kap5))
#   ```
#
# - **`arrange(...)`** seřadí řádky vzestupně nebo sestupně:
#   - `data %>% arrange(pokles)` – vzestupné řazení.
#   - `data %>% arrange(desc(pokles))` – sestupné řazení.
#
# - **`group_by(...)`** seskupí data podle unikátních hodnot v zadaném sloupci:
#   ```r
#   data %>% group_by(vyrobce)
#   ```
#
# Velice užitečný `dplyr` cheat sheet naleznete zde:
# [Data Transformation Cheat
# Sheet](https://github.com/rstudio/cheatsheets/raw/main/data-transformation.pdf)


# ** b) Výběry sloupců ####


# budeme pracovat s daty, které jsme připravili
head(data)


# pomocí funkce select balíčku dplyr, která vybere zvolené sloupce
res <- data %>% select(C5)
head(res)

# Uložení prvního a pátého sloupce dat. rámce data do dat. rámce pokus
pokus <- data %>% select(1, 5)
head(pokus)


# nebo pomocí názvů sloupců
pokus <- data %>% select(A5, A22)
head(pokus)


# ** c) Vylučování dat ze souboru (pomocí záporných indexů) ####


# nebo pomocí dplyr
pokus <- data %>% select(-1, -5)
head(pokus)


# nebo pomocí názvů
pokus <- data %>% select(-A5, -A22)
head(pokus)


# ** d) Práce s daty ve standardním datovém formátu ####


# budeme používat dříve vyrobený datový rámec dataS
head(dataS)

#
# *** Definování nových sloupců (`mutate`) ####


# vytvoříme nový sloupec pokles, který bude obsahovat rozdíl mezi hodnotami tok22 a tok5
dataS <- dataS %>% mutate(pokles = tok22 - tok5)
head(dataS)

# pozor! pokud výsledek s nový sloupcem nikam neuložíme, tak se pouze vypíše a zmizí


# *** Vybírání řádků a sloupců (`filter` a `select`)  ####


# filter - vybere / vyfiltruje řádky na základě daných podmínek
# Výběr výrobků od výrobce Amber
res <- dataS %>% filter(vyrobce == "Amber")
tail(res)

# Výběr výrobků od výrobce Amber nebo Bright
# | oddělující podmínky odpovídá logickému "nebo"
res <- dataS %>% filter(vyrobce == "Amber" | vyrobce == "Bright")
tail(res)

# Výběr všech výrobků s poklesem o 5 Lumenů a větším od výrobce Clear
# čárka oddělující podmínky odpovídá logickému "a zároveň"
res <- dataS %>% filter(pokles >= 5, vyrobce == "Clear")
res

# chcemeli světelný tok při 5°C výrobce Amber
# nejprve vyfiltrujeme řádky pro Amber a pak vybereme sloupec tok5
a5 <- dataS %>%
    filter(vyrobce == "Amber") %>%
    select(tok5)
head(a5)


# *** generování souhrných charakteristik (`summarise`) ####
# Sílu tohoto nástroje oceníme zejména v kombinaci s group_by.


# Výpočet průměru a mediánu všech hodnot proměnné kap5
dataS %>% summarise(prum = mean(tok5), median = median(tok5))


# *** seskupí hodnot do skupin podle zvolené proměnné (`group_by`) ####
# - Výsledkem je tzv. `grouped_df` (tibble), který obsahuje informace o seskupení.
# - Jedná se zejména o mezioperace, které nám umožní provádět další operace nad
# seskupenými daty.


# tabulka je "virtuálně" rozdělená na skupiny pro pozdější zpracování např. summarise
res <- dataS %>% group_by(vyrobce)
head(res)

# Ideální pro spočítání sumárních charakteristik pro každého výrobce zvlášť, např. průměru
dataS %>%
    group_by(vyrobce) %>%
    summarise(prum = mean(tok5), "směrodatná odchylka" = sd(tok5))


# *** setřízení dat (`arrange`) ####


# Vzestupné a sestupné seřazení řádků podle hodnoty poklesu
res <- dataS %>% arrange(pokles)

head(res)
tail(res)

# případně sestupně
res <- dataS %>% arrange(desc(pokles))
head(res)
tail(res)

# ---


#  6. Explorační analýza a vizualizace kategoriální proměnné ####


# ** a) Absolutní a relativní četnosti kategoriální proměnné ####
# - **`table()`** – vytvoří tabulku absolutních četností kategoriální proměnné ze
# sloupce datového rámce podle unikátních hodnot.
# - lze i pomocí knihovny `dplyr`:
#     - ```r
#       data %>% count(vyrobce)
#       ```
#       nebo
#     - ```r
#       data %>% group_by(vyrobce) %>% summarise(n = n())
#       ```


# Tabulka absolutních četností kategoriální proměnné výrobce...
cetnosti <- table(dataS$vyrobce)
cetnosti # výpis - objekt typu "table" - většinou vhodnější, ale těžší převedení do typu data.frame


# **Tabulka relativních četností pomocí `prop.table()`**


rel.cetnosti <- prop.table(cetnosti)
rel.cetnosti # výpis


# U tabulek relativních četností je potřeba pohlídat zaokrouhlení a s ním spojené riziko
# zaokrouhlovací chyby.


rel.cetnosti <- round(rel.cetnosti, digits = 3) # zaokrouhlení na 3 desetinná místa
rel.cetnosti[4] <- 1 - sum(rel.cetnosti[1:3]) # ohlídání zaokrouhlovací chyby
rel.cetnosti


#
# ** b) Vizualizace pomocí sloupcového grafu ####


# Sloupcový graf
barplot(cetnosti)


# lze i pro relativní četnosti, také má funkce mnoho dalších parametrů
barplot(rel.cetnosti,
    col = heat.colors(4), # alt. může být volen vektor konkrétních barev, např. c("blue","yellow,"red","green")
    # nebo jiné škály (heat.colors, topo.colors, terrain.colors a mnoho dalších)
    main = "Zastoupení výrobců ve výběru", # nadpis grafu
    space = 0.5, # parametr space vytvoří mezeru mezi sloupci
    xlab = "Výrobce", # popisek osy x
    ylab = "Relativní četnost" # popisek osy y
)


# **Graf lze uložit v podobě obrázku následovně:**


# uložení grafu
png("barplot.png") # název souboru
barplot(rel.cetnosti)
dev.off() # ukončení zápisu a uložení na disk


# **A v podobě PDF:**


# uložení grafu do pdf, pozor na diaktitiku a kódování
pdf("barplot.pdf", encoding = "ISOLatin2") # název souboru
barplot(rel.cetnosti)
dev.off() # ukončení zápisu a uložení na disk


# ---


#  7. Explorační analýza a vizualizace kvantitativní proměnné ####


# ** a) Základní deskriptivní statistiky kvantitativní proměnné ####
# - **průměr** pomocí `mean()` (pozor na chybějící hodnoty, dají se ignorovat pomocí
# `na.rm = TRUE`)
# - **medián** pomocí `median()`
# - **rozptyl** pomocí `var()`
# - **směrodatná odchylka** pomocí `sd()`
# - **kvantily** pomocí `quantile()` (např. `quantile(data$kap5, c(0.25, 0.75))`)
# - **velikost výběru** pomocí `length()`
# - **variační koeficient** pomocí `sd()/mean()*100`
# - **interkvartilové rozpětí** pomocí `IQR()`
# - **vnitřní hradby** pomocí `quantile() +/- 1.5*IQR()`
# - **šikmost** pomocí `monents::skewness()`
# - **špičatost** pomocí `moments::kurtosis() - 3` (Pozor na normalizaci!)
#
#
# **pozor na chybějící hodnoty, dají se ignorovat pomocí `na.rm = TRUE`**


# výpočty popisné statistiky pro proměnnou tok5 (pro všechny výrobce)
mean(dataS$tok5, na.rm = TRUE) # průměr
median(dataS$tok5, na.rm = TRUE) # medián
var(dataS$tok5, na.rm = TRUE) # rozptyl
sd(dataS$tok5, na.rm = TRUE) # směrodatná odchylka
quantile(dataS$tok5, probs = c(0.25, 0.75), na.rm = TRUE) # kvartily
length(dataS$tok5) # počet hodnot (pozor jsou včetně NA)
length(dataS$tok5[!is.na(dataS$tok5)]) # počet hodnot bez NA
sd(dataS$tok5, na.rm = TRUE) / mean(dataS$tok5, na.rm = TRUE) * 100 # variační koeficient
IQR(dataS$tok5, na.rm = TRUE) # interkvartilové rozpětí
quantile(dataS$tok5, 0.25, na.rm = TRUE) - 1.5 * IQR(dataS$tok5, na.rm = TRUE) # dolní mez vnitřních hradeb
quantile(dataS$tok5, 0.75, na.rm = TRUE) + 1.5 * IQR(dataS$tok5, na.rm = TRUE) # horní mez vnitřních hradeb
moments::skewness(dataS$tok5, na.rm = TRUE) # šikmost
moments::kurtosis(dataS$tok5, na.rm = TRUE) - 3 # špičatost

# **Totéž lze udělat pomocí knihovny `dplyr`**


tabulka_statistik <- dataS %>% summarise(
    prumer = mean(tok5, na.rm = TRUE),
    median = median(tok5, na.rm = TRUE),
    rozptyl = var(tok5, na.rm = TRUE),
    smer.odchylka = sd(tok5, na.rm = TRUE),
    dolni.kvartil = quantile(tok5, 0.25, na.rm = TRUE),
    horni.kvartil = quantile(tok5, 0.75, na.rm = TRUE),
    pocet.radku = n(),
    pocet.hodnot.bez.NA = sum(!is.na(tok5)),
    var.koeficient = sd(tok5, na.rm = TRUE) / mean(tok5, na.rm = TRUE) * 100,
    interkvartilove.rozp = IQR(tok5, na.rm = TRUE),
    dolni.mez.vnitrnihradeb = quantile(tok5, 0.25, na.rm = TRUE) - 1.5 * IQR(tok5, na.rm = TRUE),
    horni.mez.vnitrnihradeb = quantile(tok5, 0.75, na.rm = TRUE) + 1.5 * IQR(tok5, na.rm = TRUE),
    skewness = moments::skewness(tok5, na.rm = TRUE),
    kurtosis = moments::kurtosis(tok5, na.rm = TRUE) - 3
)
t(tabulka_statistik) # transpozice tabulky pro lepší čitelnost

# **A finálně pro každou skupinu zvlášť skrze `group_by`**


tabulka_statistik <- dataS %>%
    group_by(vyrobce) %>%
    summarise(
        prumer = mean(tok5, na.rm = TRUE),
        median = median(tok5, na.rm = TRUE),
        rozptyl = var(tok5, na.rm = TRUE),
        smer.odchylka = sd(tok5, na.rm = TRUE),
        dolni.kvartil = quantile(tok5, 0.25, na.rm = TRUE),
        horni.kvartil = quantile(tok5, 0.75, na.rm = TRUE),
        pocet.radku = n(),
        pocet.hodnot.bez.NA = sum(!is.na(tok5)),
        var.koeficient = sd(tok5, na.rm = TRUE) / mean(tok5, na.rm = TRUE) * 100,
        interkvartilove.rozp = IQR(tok5, na.rm = TRUE),
        dolni.mez.vnitrnihradeb = quantile(tok5, 0.25, na.rm = TRUE) - 1.5 * IQR(tok5, na.rm = TRUE),
        horni.mez.vnitrnihradeb = quantile(tok5, 0.75, na.rm = TRUE) + 1.5 * IQR(tok5, na.rm = TRUE),
        skewness = moments::skewness(tok5, na.rm = TRUE),
        kurtosis = moments::kurtosis(tok5, na.rm = TRUE) - 3
    )
t(tabulka_statistik) # transpozice tabulky pro lepší čitelnost


# **Můžeme si třeba vytvořit funkci, co nám takovýto přehled vytvoří pro náš zadaný
# datový rámec a příslušné třízení + data.**


statistika_sloupce <- function(data, column_group, column_data) {
    `%>%` <- dplyr::`%>%` # Define the pipe operator explicitly

    tabulka_statistik <- data %>%
        dplyr::group_by({{ column_group }}) %>% # Group by the specified column
        dplyr::summarise(
            rozsah_souboru = sum(!is.na({{ column_data }})),
            minimum = min({{ column_data }}, na.rm = TRUE),
            dolni_kvartil = quantile({{ column_data }}, 0.25, na.rm = TRUE),
            median = median({{ column_data }}, na.rm = TRUE),
            prumer = mean({{ column_data }}, na.rm = TRUE),
            horni_kvartil = quantile({{ column_data }}, 0.75, na.rm = TRUE),
            maximum = max({{ column_data }}, na.rm = TRUE),
            smerodatna_odchylka = sd({{ column_data }}, na.rm = TRUE),
            variacni_koeficient = sd({{ column_data }}, na.rm = TRUE) /
                mean({{ column_data }}, na.rm = TRUE) * 100,
            sikmost = moments::skewness({{ column_data }}, na.rm = TRUE),
            spicatost = moments::kurtosis({{ column_data }}, na.rm = TRUE) - 3,
            dolni_mez_vnitrni_hradeb = quantile({{ column_data }}, 0.25, na.rm = TRUE) -
                1.5 * IQR({{ column_data }}, na.rm = TRUE),
            horni_mez_vnitrni_hradeb = quantile({{ column_data }}, 0.75, na.rm = TRUE) +
                1.5 * IQR({{ column_data }}, na.rm = TRUE)
        )

    names_vars <- tabulka_statistik %>% dplyr::pull({{ column_group }})

    tabulka_statistik <- tabulka_statistik[, -1] # Remove the grouping column
    tabulka_statistik <- as.data.frame(t(tabulka_statistik)) # Transpose for better readability
    colnames(tabulka_statistik) <- names_vars # Set column names

    return(tabulka_statistik)
}

tabulka_statistik <- statistika_sloupce(dataS, vyrobce, tok5)
tabulka_statistik


# **Může se hodit si takovou tabulku uložit do excelu pomocí `openxlsx::write.xlsx()`:**


# export tabulky do excelu (opět transponovaně), bohužel konverze na data.frame je nutná
openxlsx::write.xlsx(tabulka_statistik,
    file = "tabulka_statistik.xlsx",
    rowNames = TRUE, # jednotlivé názvy polí jsou rownames
    colNames = TRUE # názvy sloupců jsou jednotlivé skupiny
)


# ** b) Krabicový graf ####
#
# Co na grafu vidíme:
# - **medián** (čára uvnitř krabice),
# - **kvartily** (spodní a horní okraj krabice),
# - **minimální a maximální hodnoty bez odlehlých pozorování** (čárky/whiskers),
# - **odlehlá pozorování** (body mimo čáry).
#
# **Vykreslujeme vždy pro originální data, můžeme doplnit i vykreslení pro data bez OP v
# případech, kdy byl originální graf příliš roztažený extrémními OP.**


# Jednoduché a rychlé vykreslení pomocí základní funkce pro jeden vektor
boxplot(dataS$tok5)


# *** Vždy když budeme uvádět více boxplotů pro srovnatelná data (například různé ####
# skupiny), je ***nutné*** je vykreslit v jednom grafu
# - vícenásobný boxplot se jednoduše vykreslí pro data ve standardním datovém formátu
# - syntaxe je pomocí `~`: `boxplot(hodnoty ~ trizeni, data = data)`


# vykreslení vícenásobného krabicového grafu
boxplot(tok5 ~ vyrobce, dataS)


# Při vytváření grafu pro publikace je nutné dodržet určité standardy úpravy

boxplot(tok5 ~ vyrobce, dataS,
    xlab = "Výrobce", # popisek osy x
    ylab = "Světelný tok [lm]", # popisek osy y
    main = "Světelný tok při 5°C v závislosti na výrobci" # nadpis grafu (vynechat pokud je součástí textu)
)
grid() # přidání mřížky do grafu pro snadnější porovnání hodnot

# **Samozřejmě můžeme uložit stejným způsobem jako u předchozího sloupcového grafu.**


png("boxplot.png", width = 800, height = 500) # název souboru a velikost grafu v pixelech
boxplot(tok5 ~ vyrobce, dataS, xlab = "Výrobce", ylab = "Světelný tok [lm]")
grid()
dev.off() # ukončení zápisu a uložení na disk

# velikost (v palcích) a kódování
pdf("boxplot.pdf", width = 8, height = 5, encoding = "ISOLatin2")
boxplot(tok5 ~ vyrobce, dataS, xlab = "Výrobce", ylab = "Světelný tok [lm]")
grid()
dev.off() # ukončení zápisu a uložení na disk

# ** c) Vnitřní hradby a identifikace odlehlých pozorování ####
# - odlehlá pozorování, které jsme viděli i v boxplotu jsou identifikována pomocí
# vnitřních hradeb
# - tato pozorování často umí znehodnotit číselné charakteristiky, proto je vhodné se s
# nimi nějakým způsobem vypořádat
# - jedním z "universálních" způsobů je jejich odstranění
#
# Identifikaci odlehlých pozorování budeme provádět pomocí `dplyr` a funkce
# `rstatix::identify_outliers()`
# - je třeba aby data byla ve standardním datovém formátu a obsahovala sloupec s
# identifikátorem entity (např. `id`)
# - odstranění děláme vždy pro každou skupinu zvlášť!
#
# Následné odstranění provedeme pomocí definice nového sloupce, který bude obsahovat
# pouze data bez odlehlých pozorování. **Tímto způsobem si zachováme původní data!**
# ```
# data$hodnota_bez_OP <- ifelse(data$id %in% OP$id, NA, data$hodnota)
#  nebo ####
# data <- data %>% mutate(hodnota_bez_OP = ifelse(id %in% OP$id, NA, hodnota))
# ```


# data jsou ve standardním formátu
# obsahují také sloupec id
# kdyby zde nebyl přítomen lze doplnit buďto
dataS$id <- seq_along(dataS$tok5)
# nebo
dataS <- dataS %>% mutate(id = row_number())
head(dataS)

# pro srovnán vykreslíme oba grafy s i bez OP
boxplot(tok5 ~ vyrobce, dataS)


# díky funkci identify_outliers z balíčku rstatix můžeme identifikovat odlehlé hodnoty
# dostaneme jejich seznam včetně náležitosti do skupin, id, ...
# toto se hodí pro jednoduchý reporting počtu OP a jejich identifikaci
OP_tok5 <-
    dataS %>%
    group_by(vyrobce) %>%
    rstatix::identify_outliers(tok5)
OP_tok5


# informaci o OP můžeme využít pro vyrobení nového sloupce s vynechanými OP
dataS$tok5OP <- ifelse(dataS$id %in% OP_tok5$id, NA, dataS$tok5)
# nebo
dataS <- dataS %>% mutate(tok5OP = ifelse(id %in% OP_tok5$id, NA, tok5))

boxplot(tok5OP ~ vyrobce, dataS)


# a můžeme použít dříve definovanou funkci pro výpočet statistik bez OP
tabulka_statistik <- statistika_sloupce(dataS, vyrobce, tok5OP)
tabulka_statistik


# **Analytik může vždy říct, že odlehlá pozorování odstraňovat nebude, ale tuto
# informaci musí do zápisu o analýze uvést!**


# ** d) Histogramy a aproximace hustoty pravděpodobnosti ####
#
# - **`hist()`** – vytvoří histogram kvantitativní proměnné, **Pozor, vyžaduje vstup v
# podobě numerického vektoru!**
#     - není ochotno fungovat pro `tibble`/`data.frame` i pokud máme jen jeden sloupec,
# je třeba jej dostat přes `data$sloupec`
#     - parametr `breaks` určuje počet intervalů
#     - parametr `freq` určuje, zda chceme zobrazit absolutní četnosti nebo odhad
# hustoty pravděpodobnosti
# - **`density()`** – vytvoří aproximaci hustoty pravděpodobnosti z histogramu
#     - **Nemá rádo NA hodnoty!**
#
# **Vykreslujeme vždy pro data bez odlehlých pozorování!!**


# Jednoduché a rychlé vykreslení
a5 <- dataS %>%
    filter(vyrobce == "Amber") %>%
    select(tok5OP)
a5 <- na.omit(a5$tok5OP) # odstraní řádky s NA a vybere pouze numerické hodnoty
hist(a5)


# Lze si hrát s parametry, přidat do jednoho grafu i odhad hustoty pravděpodobnosti, a
# fit pomocí normálního rozdělení.


# Již tradičně lze nastavit popisky a další parametry
hist(a5,
    main = "Histogram světelného toku při 5°C pro výrobce Amber",
    xlab = "Světelný tok [lm]",
    ylab = "Hustota pravděpodobnosti",
    freq = FALSE, # relativní četnosti
    breaks = 10 # počet intervalů
) # přidá absolutní četnosti daných kategorií ve formě popisků

lines(density(a5), col = "red") # přidá odhad hustoty pravděpodobnosti

# generování hodnot pro osu x
xfit <- seq(min(a5), max(a5), length = 100)
# generování hodnot pro osu y
yfit <- dnorm(xfit, mean = mean(a5), sd = sd(a5))

# přidání křivky odhdu pomocí normálního rozdělení
lines(xfit, yfit, col = "blue", lwd = 2)
# Takto kombinovaný graf může posloužit k vizuálnímu posouzení normality.

# A samozřejmě exportovat například do PDF.


pdf("histogram.pdf", width = 8, height = 5, encoding = "ISOLatin2")
hist(a5,
    freq = FALSE, breaks = 10,
    xlab = "Světelný tok [lm]", ylab = "Hustota pravděpodobnosti", main = ""
)
grid()
dev.off() # ukončení zápisu a uložení na disk


# ** e) QQ-graf ####
# Vykreslíme pomocí `qqnorm()` a `qqline()`. Stajně jako u histogramu, je třeba mít data
# ve formátu numerického vektoru.
#
# **Vykreslujeme vždy pro data bez odlehlých pozorování!!**


# Nezapomenout na popisků os...
qqnorm(a5,
    xlab = "Teoretické kvantily",
    ylab = "Výběrové kvantily",
    main = "QQ-graf, pro žárovky výrobce Amber při 5°C"
)
qqline(a5)


# ---


#  8. pravidlo 3 $\sigma$ a Čebyševova nerovnost ####
# ** a) Empirické ověření normality ####
# - provádíme vizuálně:
#     - pomocí QQ-grafu: pokud body leží na přímce, můžeme předpokládat normální
# rozdělení
#     - pomocí histogramu: musí být "podobný" normálnímu rozdělení
# - numericky:
#     - pomocí šikmosti: musí ležet v intervalu $(-2,2)$
#     - pomocí špičatosti: musí ležet v intervalu $(-2,2)$


# vyrobíme si data pro ukázku
data1 <- rnorm(100, mean = 1, sd = 1)
data2 <- runif(100, min = 0, max = 2)
data3 <- rexp(100, rate = 1)


# **Všechny nástroje pro ověření normality jsme si již ukázali, můžeme je zabalit do
# jedné funkce:**


exploracni_overeni_normality <- function(data) {
    # výpočet popisných statistik
    sikmost <- moments::skewness(data)
    spicatost <- moments::kurtosis(data) - 3
    cat("Šikmost:", sikmost, "\n")
    cat("Špičatost:", spicatost, "\n")

    # QQ graf
    qqnorm(data)
    qqline(data)

    # histogram s odhadem hustoty pravděpodobnosti normálního rozdělení
    hist(data,
        freq = FALSE, breaks = 10,
        xlab = "Světelný tok [lm]", ylab = "Hustota pravděpodobnosti", main = ""
    )
    xfit <- seq(min(data), max(data), length = 100)
    yfit <- dnorm(xfit, mean = mean(data), sd = sd(data))
    lines(xfit, yfit, col = "blue", lwd = 2)
}


# jak to vypadá pro skutečná data z normálního rozdělení
exploracni_overeni_normality(data1)

# jak to vypadá pro skutečná data z uniformního rozdělení
exploracni_overeni_normality(data2)

# jak to vypadá pro skutečná data z exponenciálního rozdělení
exploracni_overeni_normality(data3)

# ** b) Pravidlo 3 $\sigma$ a Čebyševova nerovnost ####
#
# **Je-li splněna normalita dat → pravidlo $3\sigma$**
# $\sigma$:  $P(\mu - \sigma < X < \mu + \sigma) = 0.6827$
# $2\sigma$: $P(\mu - 2\sigma < X < \mu + 2\sigma) = 0.9545$
# $3\sigma$: $P(\mu - 3\sigma < X < \mu + 3\sigma) = 0.9973$
#
# **Není-li splněna normalita dat → Čebyševova nerovnost**
# $\sigma$:  $P(\mu - \sigma < X < \mu + \sigma) = 0$
# $2\sigma$: $P(\mu - 2\sigma < X < \mu + 2\sigma) = 0.75$
# $3\sigma$: $P(\mu - 3\sigma < X < \mu + 3\sigma) = 0.8889$


# data pro světelný tok výrobce Amber při 5°C bez OP
A_tok5OP <- dataS %>%
    filter(vyrobce == "Amber") %>%
    select(tok5OP)
A_tok5OP <- na.omit(A_tok5OP$tok5OP)
A_tok5OP

# příslušné 1, 2 a 3 sigma intervaly
mu <- mean(A_tok5OP)
sigma <- sd(A_tok5OP)
cat("<", mu - sigma, ", ", mu + sigma, ">\n")
cat("<", mu - 2 * sigma, ", ", mu + 2 * sigma, ">\n")
cat("<", mu - 3 * sigma, ", ", mu + 3 * sigma, ">\n")


# ---


#  9. Zaokrouhlování ####
#
# Vše potřebné k zaokrouhlování naleznete na LMS v dokumentu
# [Zaokrouhlování](https://lms.vsb.cz/pluginfile.php/1298954/mod_folder/content/0
# /Leg%C3%A1ln%C3%AD%20tah%C3%A1ky/zaokrouhlovani.pdf).
#
# *** To nejdůležitější: ####
# - Směrodatnou odchylku zaokrouhlujeme na předepsaný počet cifer nahoru (*ceiling*):
#   - Velikost datového souboru $\in \langle 2,10 \rangle$ → **1 platná cifra**
#   - Velikost datového souboru $\in (10,30 \rangle$ → **2 platné cifry**
#   - Velikost datového souboru $\in (30,2000 \rangle$ → **3 platné cifry**
#
# - Míry polohy (průměry, kvantily, ...) pak zaokrouhlujeme klasicky (*round*) na
# stejnou platnou cifru jako směrodatnou odchylku.


# podíváme se na velikost souboru a směrodatnou odchylku
length(A_tok5OP)
smer_odch <- sd(A_tok5OP)
smer_odch


# na 3 platné cifry, tedy na jedno desetinné místo
ceiling(smer_odch * 10) / 10

prumer <- mean(A_tok5OP)
prumer


round(prumer, digits = 1)
