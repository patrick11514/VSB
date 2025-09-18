# ......................................................................................
# .................. Cvičení 12. Vícevýběrové testy - Extra pro zájemce.................
# ..................................... Michal Béreš....................................
# ......................................................................................

# Nezobrazuje-li se vám text korektně, nastavte File \ Reopen with Encoding... na UTF-8
# Pro zobrazení obsahu skriptu použijte CTRL+SHIFT+O
# Pro spouštění příkazů v jednotlivých řádcích použijte CTRL+ENTER

# * Načteme si testovací data a vyrobíme post-hoc + efekty pro ANOVU a KW test ####


data <- readxl::read_excel("data/snehurka.xlsx")
# data jsou ve standardním dtovém formátu


# POST-HOC ANOVA
vysledky <- aov(data$hodnota ~ data$typ)
PH.ANOVA <- TukeyHSD(vysledky)[[1]]
PH.ANOVA


# počítání efektů ANOVA
library(dplyr)

# celkový průměr
prumer_vsech <- mean(data$hodnota)

# průměry ve skupinách
efekty <- data %>%
    group_by(typ) %>%
    summarize(mean_skup = mean(hodnota))

# efekty
efekty$efekt <- efekty$mean_skup - prumer_vsech

# vypsat setřízené
efekty.ANOVA <- efekty %>% arrange(desc(efekt))
efekty.ANOVA


# POST-HOC KW
# post hoc - jiná funkce s výstupem, který se nám více hodí
# číselně odpovídá té použité na cvičení

# install.packages("FSA")
result <- FSA::dunnTest(data$hodnota ~ data$typ, # library FSA
    method = "bonferroni"
)
PH.KW <- result$res
PH.KW


# počítání efektů KW
library(dplyr)

# celkový průměr
prumer_vsech <- median(data$hodnota)

# průměry ve skupinách
efekty <- data %>%
    group_by(typ) %>%
    summarize(mean_skup = median(hodnota))

# efekty
efekty$efekt <- efekty$mean_skup - prumer_vsech

# vypsat setřízené
efekty.KW <- efekty %>% arrange(desc(efekt))
efekty.KW


#  Pro zájemce (nepovinné) - vytvoření setřízené tabulky p-hodnot/pisménkové schéma ####
# automatizovaně


# install.packages("stringi")
# toto je knihovna pro hledání v textu
# budeme hledat jména šmoulů v párových post-hoc testech

# inicializujeme si matici (pro pěkný výsledek jako textovou)
# 7x7 protože máme 7 šmoulů
POST.HOC.Phodnoty <- matrix(rep("-", len = 7 * 7), nrow = 7, ncol = 7)
# pojmenujeme její sloupce i řádky dle setřízených šmoulů
colnames(POST.HOC.Phodnoty) <- efekty.ANOVA$typ
rownames(POST.HOC.Phodnoty) <- efekty.ANOVA$typ
POST.HOC.Phodnoty


# smyčka přes všechny testy v post-hoc (řes názvy sloupců)
for (pair.test in rownames(PH.ANOVA)) {
    # kteří trpaslíci jsou přítomni v tomto párovém testu?
    trp.truefalse <- stringi::stri_detect_fixed(pair.test, efekty.ANOVA$typ)
    # jaké jsou indexy těchto trpaslíků
    # indexy pro zápis do matice - vždy 2 hodnoty
    indexy.trp <- which(trp.truefalse)
    # zapíšu do matice (první index je menší -> automaticky do
    # horního trojuhelníku)
    POST.HOC.Phodnoty[indexy.trp[1], indexy.trp[2]] <-
        round(max(PH.ANOVA[pair.test, "p adj"], 0.001), digits = 3)
    # zapisujeme text (pokud je matice textová, čísla se automaticky
    # převedou na text), hodnoty na tisíciny
}
POST.HOC.Phodnoty


# ** Funkce pro aoutomatizované znaménkové schéma (ručně napsané a z balíčku) ####
# *** Ručně napsané funkce (to co bychom dělali na papír) ####


# tabulka p-hodnot

tabulka.phodnot <- function(setrizene.typy, parove.testy.nazvy,
                            parove.testy.phodnoty) {
    # pocet skupin
    n <- length(setrizene.typy)
    POST.HOC.Phodnoty <- matrix(rep(0, len = n * n), nrow = n, ncol = n)
    # pojmenujeme její sloupce i řádky dle setřízených typů
    colnames(POST.HOC.Phodnoty) <- setrizene.typy
    rownames(POST.HOC.Phodnoty) <- setrizene.typy

    # smyčka přes všechny testy v post-hoc (řes názvy sloupců)
    for (i in seq_along(parove.testy.nazvy)) {
        pair.test <- parove.testy.nazvy[i]
        # kteří trpaslíci jsou přítomni v tomto párovém testu?
        typ.truefalse <- stringi::stri_detect_fixed(pair.test, setrizene.typy)
        # jaké jsou indexy těchto trpaslíků
        # indexy pro zápis do matice - vždy 2 hodnoty
        indexy.typ <- which(typ.truefalse)
        # zapíšu do matice (první index je menší -> automaticky do
        # horního trojuhelníku)
        POST.HOC.Phodnoty[indexy.typ[1], indexy.typ[2]] <-
            parove.testy.phodnoty[i]
    }
    return(POST.HOC.Phodnoty)
}


# písmenkové schéma z tabulky

pismenkove.schema <- function(POST.HOC.Phodnoty, alpha) {
    # jak je velká matice
    n <- nrow(POST.HOC.Phodnoty)
    # inicilizace matice
    pis.schema <- matrix(rep(0, len = n * n), nrow = n, ncol = n)
    # názvy řádků - kopie od vstupu
    rownames(pis.schema) <- rownames(POST.HOC.Phodnoty)
    # nastavení diagonály na 1 - je v dané skupině
    diag(pis.schema) <- 1

    # cyklus přes všechny sloupce kde můžeme něco vyplňovat
    for (i in 1:(n - 1)) {
        # cyklus přes všechny řádky v sloupci kde sledujeme pval
        for (j in (i + 1):n) {
            # je-li pval > alpha tak přidáme do hom. skupiny
            pis.schema[j, i] <- POST.HOC.Phodnoty[i, j] > alpha
        }
    }
    return(pis.schema)
}


# *** Jak použít ručně napsané funkce pro ANOVU a KW test? ####


# Jak to udělat z POST-HOC ANOVY:

# vyrobíme vstupní data
setrizene.typy <- efekty.ANOVA$typ
parove.testy.nazvy <- rownames(PH.ANOVA)
parove.testy.phodnoty <- PH.ANOVA[, "p adj"]

# vyrobíme setřízenou tabulku phodnot
p.val.tab <- tabulka.phodnot(
    setrizene.typy, parove.testy.nazvy,
    parove.testy.phodnoty
)
# vykreslíme zaokrouhlené na tisíciny
round(p.val.tab, digits = 3)

# z tabulky phodnot vyrobíme písmenkové schéma
pis.schema <- pismenkove.schema(p.val.tab, 0.05)
pis.schema


# Jak to udělat z POST-HOC KW:

# vyrobíme vstupní data
setrizene.typy <- efekty.KW$typ
parove.testy.nazvy <- PH.KW$Comparison
parove.testy.phodnoty <- PH.KW$P.adj

# vyrobíme setřízenou tabulku phodnot
p.val.tab <- tabulka.phodnot(
    setrizene.typy, parove.testy.nazvy,
    parove.testy.phodnoty
)
# vykreslíme zaokrouhlené na tisíciny
round(p.val.tab, digits = 3)

# z tabulky phodnot vyrobíme písmenkové schéma
pis.schema <- pismenkove.schema(p.val.tab, 0.05)
pis.schema


# * Písmenkové schéma pomocí vestavěné Rkové funkce ####
# Balíček rcompanion, funkce cldList


# v případě ANOVY

# nejprve vyrobíme dataframe se sloupci dvojic a phodnot
input <- data.frame(
        dvojice = rownames(PH.ANOVA),
        pval = PH.ANOVA[, "p adj"]
)

# písmenkové schéma, library rcompanion
# install.packages("rcompanion")
rcompanion::cldList(pval ~ dvojice,
        data = input,
        threshold = 0.05
)


# v případě KW

# nejprve vyrobíme dataframe se sloupci dvojic a phodnot
input <- data.frame(
        dvojice = PH.KW$Comparison,
        pval = PH.KW$P.adj
)

# písmenkové schéma, library rcompanion
# install.packages("rcompanion")
rcompanion::cldList(pval ~ dvojice,
        data = input,
        threshold = 0.05
)


