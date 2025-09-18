# Tutoriál k Rstudiu ####

# Nezobrazuje-li se vám text korektně, nastavte File \ Reopen with Encoding... na UTF-8
# Pro zobrazení obsahu skriptu použijte CTRL+SHIFT+O
# Pro spouštění příkazů v jednotlivých řádcích použijte CTRL+ENTER

# * Instalace balíčků ####
# Balíčky stačí vždy instalovat pouze jednou a pak budou k dispozici pro všechny
# aplikace používající dané Rko

# install.packages('readxl')
# Pomocí TAB zapnete našeptávání.

# postup vykonávání daného příkazu vidíte dole v konzoli
library(readxl)

# * Načítání dat ####

# cesta kterou vidíte v 'Files' není vždy stejná s wd
getwd()

# setwd('cesta k aktuálnímu souboru')
# pomocí 'More' v prohlížeči souborů

data <- read.csv2(file = "./data/aku.csv")

# načítání lze dělat pomoci GUI
# File->Import Dataset (readr pro csv, Excel)

# * Zobrazování výstupů ####
# vykreslení do konzole
data
# nebo otevření přes 'Enviroment'

# vykreslení nějakého grafu - objeví se v 'Plots'
boxplot(data)
# pomocí 'Zoom' otevřeme ve vlastním okně
# můžeme uložit do souboru

# * Zastavení aktuálního výpočtu, restart kernelu ####
# Session->Interupt R
# Session->Restart R
a <- 1
while (5 > 3) {
  a <- a + 1
}
a

# * Nápověda
help(sequence)
