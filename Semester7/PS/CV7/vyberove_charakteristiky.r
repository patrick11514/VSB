# ......................................................................................
# ......................... Cvičení 7. Výběrové charakteristiky.........................
# .................. Michal Béreš, Martina Litschmannová, Adéla Vrtková.................
# ......................................................................................

# Nezobrazuje-li se vám text korektně, nastavte File \ Reopen with Encoding... na UTF-8
# Pro zobrazení obsahu skriptu použijte CTRL+SHIFT+O
# Pro spouštění příkazů v jednotlivých řádcích použijte CTRL+ENTER

#  1. Další vybraná spojitá rozdělení ####
#
# * $\chi^2$ - Chí-kvadrát rozdělení (Pearsonovo rozdělení) ####
#
# - **Použití**: při odhadu směrodatné odchylky (za použití výběrové)
# - Má jediný parametr – **počet stupňů volnosti**
# $$\frac{S^2}{\sigma^2} (n-1) \sim \chi^2_{n-1}$$
# - $S$ je výběrová směrodatná odchylka


n <- 5 # počet stupňů volnosti
x <- seq(from = 0, to = 30, by = 0.01) # osa x
fx <- dchisq(x = x, df = n) # hustota pravděpodobnosti chí-kvad. rozdělení

plot(x, fx, type = "l")


Fx <- pchisq(q = x, df = n) # distrib. fce. chí-kvad. rozdělení

plot(x, Fx, type = "l")


# * $t$ - Studentovo rozdělení ####
#
# - **Použití**: při odhadu střední hodnoty bez přesné znalosti rozptylu (pouze
# výběrového rozptylu)
# $$\frac{\bar{X} - \mu}{S} \sqrt{n} \sim t_{n-1}$$
# - $\bar{X}$ je výběrový průměr
# - $S$ je výběrová směrodatná odchylka
# - S rostoucím počtem stupňů volnosti konverguje k normovanému normálnímu rozdělení.


n <- 5 # počet stupňů volnosti
x <- seq(from = -3, to = 3, by = 0.01) # osa x
fx <- dt(x = x, df = n) # hustota pravděpodobnosti studentova rozdělení

plot(x, fx, type = "l")

fnorm <- dnorm(x, mean = 0, sd = 1) # hodnoty norm. normálního roz.
lines(x, fnorm, col = "red") # do posledního grafu


Fx <- pt(q = x, df = n) # hustota pravděpodobnosti studentova rozdělení

plot(x, Fx, type = "l")

Fnorm <- pnorm(x, mean = 0, sd = 1) # hodnoty norm. normálního roz.
lines(x, Fnorm, col = "red") # do posledního grafu


# * $F$ - Fisher-Snedecorovo rozdělení ####
#
# - **Použití**: k testování shody rozptylů
# $$\frac{S_1^2 / \sigma_1^2}{S_2^2 / \sigma_2^2} \sim F_{n_1 - 1, n_2 - 1}$$
# - $S_1^2$ a $S_2^2$ jsou výběrové rozptyly
# - $\sigma_1^2$ a $\sigma_2^2$ jsou skutečné rozptyly
# - $n_1$ a $n_2$ jsou počty prvků v obou výběrech


n <- 5 # počet stupňů volnosti výběr. 1
m <- 6 # počet stupňů volnosti výběr. 2
x <- seq(from = 0, to = 10, by = 0.01) # osa x
fx <- df(x = x, df1 = n, df2 = m) # hustota pravděpodobnosti chí-kvad. rozdělení

plot(x, fx, type = "l")


Fx <- pf(q = x, df1 = n, df2 = m) # hustota pravděpodobnosti chí-kvad. rozdělení

plot(x, Fx, type = "l")


#  2. Jak se chová průměr hodnot z normálního rozdělení? ####
#
# - průměr $$\bar{X} = \frac{1}{n} \sum_{i=1}^{n} X_i$$ je náhodná veličina, tedy každý
# výběr unikátních $n$ hodnot vede k jiné hodnotě průměru
# - ukážeme, že průměr hodnot z normálního rozdělení má normální rozdělení
#
# Funkce **`rnorm(n, mean, sd)`** generuje **n** hodnot z normálního rozdělení se
# střední hodnotou **mean** a směrodatnou odchylkou **sd**.


vel_nah_vyberu <- 30
mu <- 10
sigma <- 3
nah_vyber <- rnorm(n = vel_nah_vyberu, mean = mu, sd = sigma)
nah_vyber


mean(nah_vyber)
sd(nah_vyber)


# *** Náhodná veličina: průměr hodnot ####


poc_vyberu <- 1000
prumery <- numeric(poc_vyberu) # numeric vyrobi vektor 0
smer_odchylky <- numeric(poc_vyberu)
for (i in 1:poc_vyberu) {
    nah_vyber <- rnorm(n = vel_nah_vyberu, mean = mu, sd = sigma)
    prumery[i] <- mean(nah_vyber)
    smer_odchylky[i] <- sd(nah_vyber)
}


hist(prumery)


qqnorm(prumery)
qqline(prumery)


mean(prumery)
sd(prumery)
sigma / sqrt(vel_nah_vyberu)


#  3. Jak se chová průměr hodnot z uniformního rozdělení? ####
#
# - nyní se podíváme jaké rozdělení má průměr (jako náhodná veličina) hodnot z
# uniformního rozdělení
#
# Fukce **runif(n, min, max)** generuje **n** hodnot z uniformního rozdělení
# U(**min,max**).


vel_nah_vyberu <- 30
a <- 1
b <- 7
nah_vyber <- runif(n = vel_nah_vyberu, min = a, max = b)
# nah_vyber


hist(nah_vyber)
qqnorm(nah_vyber)
qqline(nah_vyber)


mu <- (a + b) / 2
mu
mean(nah_vyber)
sigma <- sqrt((b - a)^2 / 12)
sigma
sd(nah_vyber)


# *** Náhodná veličina: průměr hodnot ####


poc_vyberu <- 1000
prumery <- numeric(poc_vyberu)
smer_odchylky <- numeric(poc_vyberu)
for (i in 1:poc_vyberu) {
    nah_vyber <- runif(n = vel_nah_vyberu, min = a, max = b)
    prumery[i] <- mean(nah_vyber)
    smer_odchylky[i] <- sd(nah_vyber)
}


hist(prumery)


qqnorm(prumery)
qqline(prumery)


mean(prumery)
sd(prumery)
sigma / sqrt(vel_nah_vyberu)


# ---
#  Příklady ####
# * Příklad 1. ####
# Zatížení letadla s 64 místy nemá překročit 6 000 kg. Jaká je pravděpodobnost, že při
# plném obsazení bude tato hodnota překročena, má-li hmotnost cestujícího střední
# hodnotu 90 kg a směrodatnou odchylku 10 kg?


# X...hmotnost 64 cestujících
# X ~ N(64*90; 64*100)
# P(X > 6000) = 1 - F(6000)

1 - pnorm(q = 6000, mean = 64 * 90, sd = sqrt(64 * 100))


# * Příklad 2. ####
# Zásilka obsahuje 300 výrobků určitého typu. Je známo, že pravděpodobnost zhotovení
# vadného výrobku tohoto typu je 0,04.
#
# **a)**
# Odhadněte pravděpodobnost, že absolutní odchylka podílu vadných výrobků v zásilce od
# pravděpodobnosti vyrobení vadného výrobku bude menší než 1 %.


# X = (p − π)/sqrt(π*(1 − π))*sqrt(n) ∼ N(0, 1)
# P(-0.01/sqrt(π*(1 − π))*sqrt(n) < X < 0.01/sqrt(π*(1 − π))*sqrt(n))
pi <- 0.04
n <- 300
bound <- 0.01 / sqrt(pi * (1 - pi)) * sqrt(n)
pnorm(q = bound, mean = 0, sd = 1) - pnorm(q = -bound, mean = 0, sd = 1)


# **b)**
# Jak se změní výsledek, jestliže zásilka bude obsahovat 3 000 výrobků?


n <- 3000
bound <- 0.01 / sqrt(pi * (1 - pi)) * sqrt(n)
pnorm(q = bound, mean = 0, sd = 1) - pnorm(q = -bound, mean = 0, sd = 1)


# * Příklad 3. ####
# Cestující pravidelně jezdí do zaměstnání a zpět MHD. Je známo, že doba čekání na
# příjezd MHD se pohybuje v mezích od 0 do 3 minut. Jaká je pravděpodobnost, že celková
# doba čekání zaměstnance na příjezd MHD během 23 pracovních dnů bude kratší než 80
# minut?


# Y...doba i-tého čekání na MHD
# y ~ R(0; 3)
# X...celková doba čekání během 23 dnů (cesta tam a zpět ⇒ 46 čekání)
# X ~ N(46*EY; 46*DY)
# P(X < 80)

a <- 0
b <- 3
n <- 46
EY <- (a + b) / 2
DY <- (b - a)^2 / 12

pnorm(q = 80, mean = n * EY, sd = sqrt(n * DY))


# * Příklad 4. ####
# Předpokládejme, že průměrná spotřeba elektrické energie domácností v určitém městě v
# lednu je 120 kWh a směrodatná odchylka spotřeby je 100 kWh. Určete pravděpodobnost, že
# průměrná spotřeba 100 náhodně vybraných domácností bude větší než 140 kWh.


# Xi...spotřeba i-té domácnosti
# X...průměrná spotřeba 100 domácností
# X ~ N(EXi;Dxi/n)
# P(X > 140)

EXi <- 120
DXi <- 100^2
n <- 100
1 - pnorm(q = 140, mean = EXi, sd = sqrt(DXi / n))


# * Příklad 5. ####
# Společnost Acme Battery Company vyvinula nový typ baterie mobilních telefonů. V
# průměru vydrží baterie 60 minut na jedno nabití. Směrodatná odchylka této doby je 4
# minuty. Předpokládejme, že výrobní oddělení po 6 měsících spustí test kontroly
# kvality. Provedli dva náhodné výběry o rozsahu 10 baterií a v obou zjistili
# směrodatnou odchylku výdrže baterií větší než 6 minut. S jakou pravděpodobností takový
# výsledek mohli očekávat?


# X = (n − 1)S^2/σ^2
# X ∼ χ_n-1
# P(S > 6) = P(X > ...)

n <- 10
S_obs <- 6
sigma <- 4
X_obs <- (n - 1) * S_obs^2 / sigma^2

P_jedno_mereni <- 1 - pchisq(q = X_obs, df = n - 1)
P_jedno_mereni
P_jedno_mereni^2


# * Příklad 6. ####
# Z úmrtnostních tabulek vyplývá pravděpodobnost 0,99, že se 35 - letý muž dožije
# dalšího roku. Roční pojistné této věkové skupiny činí 2 000 Kč, v případě úmrtí
# pojišťovna vyplatí 100 000 Kč. Jaká je pravděpodobnost, že zisk z 500 pojištěných mužů
# ve věku 35 let bude alespoň 500 000 Kč?
#
# **a)** Řešte pomocí binomického rozdělení.


# X...počet mužů z 500, kteří se nedožijí dalšího roku
# X ~ Bi(500; 0.01)
# Z = 500 · 2 000 − X · 100 000
# P(Z ≥ 500 000) = P(X ≤ 5)

pbinom(5, size = 500, prob = 0.01)


# **b)** Řešte pomoci aproximace binomického rozdělení rozdělením normálním.


# X ~ Bi(500; 0.01) ~ N(500*0.01; 500*0.01*(1-0.01))
# P(X ≤ 5) ~ P(X < 5.5) (oprava na spojitost)

pnorm(5.5, mean = 500 * 0.01, sd = sqrt(500 * 0.01 * (1 - 0.01)))


# * Příklad 7. ####
# Předpokládejme, že v populaci má přibližně 60 % mladých mužů vyšší než doporučenou
# hladinu cholesterolu v séru. S jakou pravděpodobností bude mít v náhodném výběru 200
# mladých mužů více než 120 z nich vyšší než doporučenou hladinu cholesterolu v séru?


# **a)** Řešte pomocí binomického rozdělení.


# X...počet mladých mužů z 200 s vyšší než doporučenou hladinou cholesterolu v séru
# X ∼ Bi(200; 0.6)
# P(X > 120) = 1 - P(X ≤ 120)

1 - pbinom(120, size = 200, prob = 0.6)


# **b)** Řešte pomocí aproximace binomického rozdělení normálním rozdělením.


# X ~ N(200*0.6; 200*0.6(1-0.6)), tj. X ≈ N(120; 48)
# 1 - P(X ≤ 120) ~ 1 - P(X < 120.5) (oprava na spojitost)

1 - pnorm(120.5, mean = 200 * 0.6, sd = sqrt(200 * 0.6 * (1 - 0.6)))
