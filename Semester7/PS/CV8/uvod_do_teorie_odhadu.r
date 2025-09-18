# ......................................................................................
# ........ Cvičení 8. Úvod do teorie odhadů - Intervalové odhady (jednoho výběru).......
# ............... Michal Béreš, Martina Litschmannová, Veronika Kubíčková...............
# ......................................................................................

# Nezobrazuje-li se vám text korektně, nastavte File \ Reopen with Encoding... na UTF-8
# Pro zobrazení obsahu skriptu použijte CTRL+SHIFT+O
# Pro spouštění příkazů v jednotlivých řádcích použijte CTRL+ENTER

# Importujeme dplyr
library(dplyr)

#  1. Demonstrace na úvod - co je to intervalový odhad? ####
# Uvažujme náhodnou veličinu z normálního rozdělení se střední hodnotou $\mu$ a
# směrodatnou odchylkou $\sigma$. Budeme pracovat s výběry z této náhodné veličiny a
# pomoci jich se budeme snažit odhadnout střední hodnutu rozdělení (zde známe její
# skutečnou hodnotu, ale v praxi je její hodnota neznámá).


n <- 30 # velikost výběru
mu <- 100 # střední hodnota
sigma <- 10 # směr. odchylka

# simulace náhodného výběru ze zadané náhodné veličiny
vyber <- rnorm(n = n, mean = mu, sd = sigma)

X <- mean(vyber) # výběrový průměr jako bodový odhad
S <- sd(vyber) # výběrová směr. odch.
X
S


# Pro přehlednost si můžeme výběr vizualizovat.


options(repr.plot.width = 12) # šířka grafů v Jupyteru
par(mfrow = c(1, 2)) # matice grafů 1x2

hist(vyber)
boxplot(vyber)


# ** Samotné sestrojení Intervalového odhadu pomocí výběrové charakteristiky ####
# Použijeme tuto výběrovou charakteristiku: (předpokládáme, že neznáme žádné skutečné
# parametry rozdělení, pouze to, že je normální)
# $$Y=\frac{\bar X - \mu}{S}\sqrt{n} \sim t_{n-1}$$
# Jelikož, známe rozdělení Y jsme schopni napočítat $a$ a $b$ v následujícím výrazu:
# $$P(a<Y<b)\geq 1 - \alpha$$
# - $\alpha$ nazýváme hladinou významnosti (pravděpodobnost, že hledaná hodnota leží
# mimo náš interval)
# - $1-\alpha$ nazýváme spolehlivost intervalového odhadu


# $a$ a $b$ zvolíme tak aby byly v pravděpodobnosti symetrické, tzn.:
# $$P(Y<a)\leq \frac{\alpha}{2} \rightarrow a=t_{n-1}^{\frac{\alpha}{2}}$$
# $$P(b<Y)\leq \frac{\alpha}{2} \rightarrow P(Y\leq b)\geq 1 - \frac{\alpha}{2}
# \rightarrow b=t_{n-1}^{1-\frac{\alpha}{2}}$$


# maximální pravděpodobnost s jakou připouštíme aby
# skutečná st. hod. ležela mimo sestrojený interval
alpha <- 0.05

# příslušné kvantily studentova rozdělení
t_low <- qt(alpha / 2, df = n - 1)
t_high <- qt(1 - alpha / 2, df = n - 1)

t_low
t_high


# Dále jen doplníme do výrazu a upravíme:
#
# $$P\left(t_{n-1}^{\frac{\alpha}{2}}<\frac{\bar X -
# \mu}{S}\sqrt{n}<t_{n-1}^{1-\frac{\alpha}{2}}\right)\geq 1 - \alpha$$
# $$P\left(\bar X - t_{n-1}^{1-\frac{\alpha}{2}}\frac{S}{\sqrt{n}}<\mu<\bar X -
# t_{n-1}^{\frac{\alpha}{2}}\frac{S}{\sqrt{n}}\right)\geq 1 - \alpha$$


I_dolni <- X - t_high * S / sqrt(n)
I_horni <- X - t_low * S / sqrt(n)
paste("P(", I_dolni, " < µ < ", I_horni, ") ≥ ", 1 - alpha)


# Tento konkrétní odhad, můžeme dostat také pomocí Rkovské funkce t.test:


t.test(vyber, alternative = "two.sided", conf.level = 1 - alpha)$conf.int


# *** Otestování intervalového odhadu na více výběrech ####
#
# - pro normální rozdělení provedeme mnoho výběrů o velikosti 30
# - srovnáme intervalový odhad s reálnou (známou) hodnotou střední hodnoty
# - počet "špatných" intervalů by měl být roven hladině významnosti $\alpha$ (limitně)


pocet_pokusu <- 100 # počet výběrů

n <- 30 # velikost výběru
mu <- 100 # střední hodnota
sigma <- 10 # směrodat. odchyl.

alpha <- 0.05 # hladina významnosti

# příslušné kvantily studentova rozdělení
t_low <- qt(alpha / 2, df = n - 1)
t_high <- qt(1 - alpha / 2, df = n - 1)

# vykreslení skutečné střední hodnoty
plot(c(1, pocet_pokusu), c(mu, mu), type = "l", ylim = c(90, 110))

pocet_neuspesnych <- 0
# cyklus přes jednotlivé výběry
for (i in 1:pocet_pokusu) {
    vyber <- rnorm(n = n, mean = mu, sd = sigma)
    X <- mean(vyber)
    S <- sd(vyber)
    I_dolni <- X - t_high * S / sqrt(n)
    I_horni <- X - t_low * S / sqrt(n)

    # zvolíme barvu vykreslení, podle toho zda IO obsahuje stř. hod.
    if (I_dolni < mu && mu < I_horni) {
        barva <- "blue"
    } else {
        barva <- "red"
        pocet_neuspesnych <- pocet_neuspesnych + 1
    }
    # vykreslíme IO jako vertikální čáru
    lines(c(i, i), c(I_dolni, I_horni), col = barva)
}
paste(
    "alpha = ", alpha, ", relativní četnost něúspěšných IO = ",
    pocet_neuspesnych / pocet_pokusu
)


# ---


#  2. Typy intervalových odhadů ####
# (Ukázky na odhadu střední hodnoty dat z normálního rozdělení.)
# * Dolní/Levostranný IO ####
# - $P(M_D^* < \mu) = 1-\alpha$
# - v Rku **alternative="greater"**


vyber <- rnorm(n = 30, mean = 100, sd = 10)
alpha <- 0.05
t.test(vyber, alternative = "greater", conf.level = 1 - alpha)$conf.int


# * Horní/Pravostranný IO ####
# - $P(\mu < M_H^*) = 1-\alpha$
# - v Rku **alternative="less"**


vyber <- rnorm(n = 30, mean = 100, sd = 10)
alpha <- 0.05
t.test(vyber, alternative = "less", conf.level = 1 - alpha)$conf.int


# * Oboustranný IO ####
# - $P(M_D < \mu < M_H) = 1-\alpha$
# - v Rku **alternative="two.sided"**


vyber <- rnorm(n = 30, mean = 100, sd = 10)
alpha <- 0.05
t.test(vyber, alternative = "two.sided", conf.level = 1 - alpha)$conf.int


# ---


#  3. Přehled parametrů výběru a jejich bodových/intervalových odhadů ####
# Běžně máme k dispozici více konstrukcí IO (funkcí v Rku které to za nás udělají), ale
# každá konstrukce má jiné požadavky na data a vytváří různě "kvalitní" (ve smyslu
# velikosti IO) odhady. My budeme vždy vybírat "nejkvalitnější" IO, který **má splněny**
# předpoklady použití.
# Pořadí různých IO níže bude vždy od "nejlepšího" po nejrobustnější.
# * I. Míry polohy jednoho výběru ####
# Mírami polohy rozumíme údaj určující polohu dat, nehledě na tom jak jsou rozptýlená.
# Pro data z normálního rozdělení můžeme odhadovat střední hodnotu, pro ostatní medián.
# *** a) studentův t-test IO ####
# - odhadujeme střední hodnotu - bodový odhad je výběrový průměr
# - data musejí pocházet z normálního rozdělení
#     - exploračně: šikmost a špičatost leží v (-2,2)
#     - exploračně: QQ graf má body přibližně na čáře
#     - exaktně: pomocí statistického testu, např. Shapiro-Wilk test
# (shapiro.test(data))


vyber <- rnorm(n = 30, mean = 100, sd = 10)
alpha <- 0.05


# exploračně test normality
moments::skewness(vyber)
moments::kurtosis(vyber) - 3
qqnorm(vyber)
qqline(vyber)


# exaktně test normality dat
shapiro.test(vyber)$p.value
# vysledná p-hodnota musím být větší než hl. výz. (př. 0.05)


# bodový odhad
mean(vyber)
# IO
t.test(vyber, alternative = "two.sided", conf.level = 1 - alpha)$conf.int


# *** b) Wilcoxnův test IO ####
# - odhadujeme medián - bodový odhad je výběrový medián
# - data musejí pocházet ze symetrického rozdělení
#     - exploračně: šikmost leží v (-2,2)
#     - exploračně: histogram vypadá přibližně symetricky
#     - exaktně: pomocí statistického testu, např. balíček "lawstat", funkce
# "symmetry.test(data,boot=FALSE)"
# - funkce v Rku vyžaduje dodatečný parametr (conf.int = TRUE)


vyber <- runif(n = 30, min = 80, max = 120)
alpha <- 0.05


# exploračně
moments::skewness(vyber)
hist(vyber, breaks = 5)


# exaktně: test symetrie
# install.packages("lawstat")
lawstat::symmetry.test(vyber, boot = FALSE)$p.value
# vysledná p-hodnota musím být větší než hl. výz. (př. 0.05)


# bodový odhad
quantile(vyber, probs = 0.5)
median(vyber)
# IO
wilcox.test(vyber,
    alternative = "two.sided", conf.level = 1 - alpha,
    conf.int = TRUE
)$conf.int


# *** c) znaménkový test test IO ####
# - odhadujeme medián - bodový odhad je výběrový medián
# - výběr většího rozsahu (>10)
# - funkce v Rku vyžaduje dodatečný parametr (conf.int = TRUE)
# - vyžaduje knihovnu "BSDA"
# - jakožto nejrobustnější test, se dá použít i na nespojitá data - např. pořadí v
# nějakém seznamu


vyber <- rexp(n = 30, rate = 1 / 100)
alpha <- 0.05


# skutečný medián
qexp(p = 0.5, rate = 1 / 100)


# bodový odhad
# quantile(vyber, probs = 0.5)
median(vyber)
# IO
# install.packages("BSDA")
BSDA::SIGN.test(vyber,
    alternative = "two.sided", conf.level = 1 - alpha,
    conf.int = TRUE
)$conf.int


# * II. Míry variability jednoho výběru ####
# Mírami variability rozumíme údaj určující rozptýlenost/variabilitu dat, nehledě na
# celkových hodnotách. Pro data z normálního rozdělení můžeme odhadovat směrodatnou
# odchylku.
# *** IO směrodatné odchylky ####
# - odhadujeme směrodatnou odchylku - bodovým odhadem je výběrový směrodatná odchylka
# - data musejí pocházet z normálního rozdělení
#     - exploračně: šikmost a špičatost leží v (-2,2)
#     - exploračně: QQ graf má body přibližně na čáře
#     - exaktně: pomocí statistického testu, např. Shapiro-Wilk test
# (shapiro.test(data))
# - vyžaduje balíček "EnvStats"
# - funkce v Rku, dává výpočet rozptylu - nutná odmocnina výsledku


vyber <- rnorm(n = 30, mean = 100, sd = 10)
alpha <- 0.05


# exploračně test normality
moments::skewness(vyber)
moments::kurtosis(vyber) - 3
qqnorm(vyber)
qqline(vyber)


# exaktně test normality dat
shapiro.test(vyber)$p.value
# vysledná p-hodnota musím být větší než hl. výz. (př. 0.05)


# bodový odhad
sd(vyber)
# IO
# install.packages("EnvStats")
sqrt(EnvStats::varTest(vyber, alternative = "two.sided", conf.level = 1 - alpha)$conf.int)


# *** Přidáme si ruční výpočet: ####
#
# $$
# \frac{S^2}{\sigma^2} (n-1) \sim \chi^2_{n-1}
# $$
#
# Horní mez:
#
# $$
# P\left(\frac{S^2}{\sigma^2} (n-1) < \chi^{\frac{\alpha}{2}}_{n-1} \right) =
# \frac{\alpha}{2}
# \quad \Rightarrow \quad
# P\left(\frac{S^2}{\chi^{\frac{\alpha}{2}}_{n-1}} (n-1) < \sigma^2 \right) =
# \frac{\alpha}{2}
# $$
#
# Dolní mez:
#
# $$
# P\left(\frac{S^2}{\sigma^2} (n-1) > \chi^{1-\frac{\alpha}{2}}_{n-1} \right) =
# \frac{\alpha}{2}
# \quad \Rightarrow \quad
# P\left(\frac{S^2}{\chi^{1-\frac{\alpha}{2}}_{n-1}} (n-1) > \sigma^2 \right) =
# \frac{\alpha}{2}
# $$
#
# Dohromady:
#
# $$
# P\left(\frac{S^2}{\chi^{1-\frac{\alpha}{2}}_{n-1}} (n-1) < \sigma^2 <
# \frac{S^2}{\chi^{\frac{\alpha}{2}}_{n-1}} (n-1) \right) = 1 - \alpha
# $$


# ruční výpočet
alpha <- 0.05
n <- 30
S <- sd(vyber)


hor_q <- qchisq(1 - alpha / 2, n - 1)
dol_q <- qchisq(alpha / 2, n - 1)
hor_q
dol_q


sqrt(S^2 * (n - 1) / dol_q)
sqrt(S^2 * (n - 1) / hor_q)


# * III. Pravděpodobnost výskytu u jednoho výběru ####
# *** IO pravděpodobnosti ####
# - odhadujeme pravděpodobnost - bodový odhad je relativní četnost
# - vyžadujeme dostatečný počet dat: $n>\frac{9}{p(1-p)}$
# - **Clopperův - Pearsonův odhad (`binom.test`)**
#     - jako parametr nebere data, ale počet úspěchů a počet pozorování
# - Waldův - z výběrových charakteristik


pi <- 0.3
n <- 60
alpha <- 0.05
vyber <- runif(n = n, min = 0, max = 1) < pi


# ověření předpokladů
p <- mean(vyber)
p
9 / (p * (1 - p))


# bodový odhad
p
# intervalový odhad Clopperův - Pearsonův
celk_pocet <- length(vyber)
pocet_poz <- sum(vyber)
binom.test(
    x = pocet_poz, n = celk_pocet, alternative = "two.sided",
    conf.level = 1 - alpha
)$conf.int


# Intervalový odhad Waldův
dol_q <- qnorm(alpha / 2)
hor_q <- qnorm(1 - alpha / 2)

p - hor_q * sqrt(p * (1 - p) / n) # dolní mez IO
p - dol_q * sqrt(p * (1 - p) / n) # horní mez IO


# Výpočet 11 nejčastěji používaných intervalů spolehlivosti param. bin. rozdělení
# pomocí balíčku binom
# install.packages("binom")
binom::binom.confint(n = celk_pocet, x = pocet_poz)


# ---


#  Příklady ####
# * Příklad 1. ####
# Při kontrolních zkouškách 16 žárovek byl stanoven odhad střední hodnoty $\bar x$ = 3
# 000 hodin a směrodatné odchylky s = 20 hodin jejich životnosti. Za předpokladu,že
# životnost žárovky má normální rozdělení, určete 90% intervalový odhad pro parametry µ
# a σ


# Odhadujeme stř.hodnotu a směr.odchylku životnosti žárovek
# Součástí zadání je informace o normalitě dat

n <- 16 # rozsah souboru
x.bar <- 3000 # hodin.... průměr (bodový odhad střední hodnoty)
s <- 20 # hodin.... výběrová směrodatná odchylka (bodový odhad sm. odchylky)
alpha <- 0.1 # hladina významnosti (spolehlivost 1-alpha = 0.9)


# Oboustranný intervalový odhad střední hodnoty
dol_q <- qt(alpha / 2, n - 1)
hor_q <- qt(1 - alpha / 2, n - 1)

x.bar - hor_q * s / sqrt(n) # dolní mez IO
x.bar - dol_q * s / sqrt(n) # horní mez IO


# Oboustranný intervalový odhad směrodatné odchylky
dol_q <- qchisq(alpha / 2, n - 1)
hor_q <- qchisq(1 - alpha / 2, n - 1)

sqrt((n - 1) * s^2 / hor_q) # dolní mez IO
sqrt((n - 1) * s^2 / dol_q) # horní mez IO


# * Příklad 2. ####
# Hloubka moře se měří přístrojem, jehož systematická chyba je rovna nule a náhodné
# chyby mají normální rozdělení se směrodatnou odchylkou 20 m. Kolik nezávislých měření
# je třeba provést, aby s pravděpodobností 95 % stanovila hloubku s chybou menší než 10
# m?


# Určujeme odhad potřebného rozsahu výběru (počtu potřebnych měření)

# Předpokládáme normalitu dat, se známým rozpylem (dle zadání)

sigma <- 20 # metrů .... známá směrodatná odchylka
alpha <- 0.05 # hladina významnosti (spolehlivost 1-alpha = 0.95)
delta <- 10 # metrů ... přípustná chyba měření

# Odhad rozsahu výběru
# Y = delta/sigma*sqrt(n) ~ N(0,1), delta = X-mu
# P(Y > Z_(1-alpha/2)) = alpha/2

(qnorm(1 - alpha / 2) * sigma / delta)^2


# * Příklad 3. ####
# Jaký musí být počet pozorování, jestliže chceme s pravděpodobností 0,95 stanovit
# průměrnou hodnotu hemoglobinu u novorozenců s chybou nejvýše 1,0 $g/l$. Populační
# rozptyl hodnot se
# odhaduje hodnotou 46,0 $g^2/l^2$.


# Určujeme odhad potřebného rozsahu výb. (počtu novorozenců, které musíme testovat)

# Předpokládáme normalitu dat, bez tohoto předpokladu je příklad neřešitelný

sigma <- sqrt(46) # g/l .... známá směrodatná odchylka
alpha <- 0.05 # hladina významnosti (spolehlivost 1-alpha = 0.95)
delta <- 1 # g/l ... přípustná chyba měření


# Odhad rozsahu výběru
# Y = delta/sigma*sqrt(n) ~ N(0,1), delta = X-mu
# P(Y > Z_(1-alpha/2)) = alpha/2

(qnorm(1 - alpha / 2) * sigma / delta)^2


# * Příklad 4. ####
# Úkolem je určit průměrnou hladinu cholesterolu v séru v určité populaci mužů. V
# náhodném výběru (pocházejícím z normálního rozdělení ) 25 mužů je výběrový průměr 6,3
# mmol/l a výběrová směrodatná odchylka 1,3 mmol/l.


# Odhadujeme střední hladinu cholesterolu v séru
# Předpokládáme normalitu dat (dle zadání)

n <- 25 # rozsah souboru
x.bar <- 6.3 # mmol/l .... průměr (bodový odhad střední hodnoty)
s <- 1.3 # mmol/l .... výběrová směr. odchylka (bodový odhad sm. odchylky)
alpha <- 0.05 # hladina významnosti (spolehlivost 1-alpha = 0.95)


# Oboustranný intervalový odhad střední hodnoty
dol_q <- qt(alpha / 2, n - 1)
hor_q <- qt(1 - alpha / 2, n - 1)

x.bar - hor_q * s / sqrt(n) # dolní mez IO
x.bar - dol_q * s / sqrt(n) # horní mez IO


# * Příklad 5. ####
# Předpokládejme, že v náhodném výběru 200 mladých mužů má 120 z nich vyšší než
# doporučenou hladinu cholesterolu v séru. Určete 95% interval spolehlivosti pro
# procento mladých mužů
# s vyšší hladinou cholesterolu v populaci.


# Odhadujeme podíl mužů s vyšší hladinou cholesterolu v celé populaci,
# tj. pravděpodobnost,že náhodně vybraný muž bude mít vyšší hladinu cholesterolu

n <- 200 # rozsah souboru
x <- 120 # počet "úspěchů"
p <- x / n # relativní četnost (bodový odhad pravděpodobnosti)
p
alpha <- 0.05 # hladina významnosti (spolehlivost 1-alpha = 0.95)


# Ověření předpokladů
9 / (p * (1 - p))


# Oboustranný Clopperův - Pearsonův (exaktní) int.ý odhad param. binom. rozdělení
binom.test(x, n, alternative = "two.sided", conf.level = 0.95)$conf.int


# Waldův (asymptotický) odhad (z-statistika) - aprox. normálním rozdělením dle CLV
dol_q <- qnorm(alpha / 2)
hor_q <- qnorm(1 - alpha / 2)

p - hor_q * sqrt(p * (1 - p) / n) # dolní mez IO
p - dol_q * sqrt(p * (1 - p) / n) # horní mez IO


# * Příklad 6. ####
# V rámci výzkumné studie pracujeme s náhodným výběrem 70 žen z české populace. U každé
# z žen byl změřen hemoglobin s přesností 0,1 g/100 ml. Naměřené hodnoty jsou v uvedeny
# v souboru `data/intervalove_odhady.xlsx` na stránce `Hemoglobin`. Nalezněte 95%
# intervalové odhady směrodatné odchylky a střední hodnoty hemoglobinu v populaci
# českých žen. (Normalitu ověřte na základě exploračních grafů.)


# Odhadujeme střední hodnotu a směrodatnou odchylku hemoglobinu v séru

# Načtení dat z xlsx souboru (pomoci balíčku readxl)
hem <- readxl::read_excel("data/intervalove_odhady.xlsx",
    sheet = "Hemoglobin"
)
colnames(hem) <- "hodnoty"
head(hem)


# Explorační analýza - Zhodnocení OP
boxplot(hem$hodnoty)


# ověření normality: exaktně - test normality.
# ověříme Shapirovým . Wilkovým testem.
shapiro.test(hem$hodnoty)$p.value
# Na hl. významnosti 0.05


# 95% oboustranný intervalový odhad střední hodnoty
mean(hem$hodnoty)
t.test(hem$hodnoty, altarnative = "two.sided", conf.level = 0.95)$conf.int


## 95% oboustranný intervalový odhad směrodatné odchylky
sd(hem$hodnoty)

IO_rozptyl <- EnvStats::varTest(hem$hodnoty, alternative = "two.sided", conf.level = 0.95)$conf.int

sqrt(IO_rozptyl)


# jaká je přesnost zaokrouhlení (pro intervalové odhady)?
length(hem$hodnoty)


# * Příklad 7. ####
# V datovém souboru `data/pr7.xlsx` naleznete měření hluku způsobeného větrákem počítače
# [dB]. Spočtěte 95% intervalový odhad průměrného hluku a 95% intervalový odhad
# variability hluku.


# načtení dat
data <- readxl::read_excel("data/pr7.xlsx")
head(data)


# zhodnocení OP
boxplot(data$dB)


# odstranění OP
data_op <- data %>% rstatix::identify_outliers(dB)
data_op

data <- data %>% mutate(dBOP = ifelse(ID %in% data_op$ID, NA, dB))
boxplot(data$dBOP)

# test normality exaktně
shapiro.test(data$dBOP)$p.value


# bodový a intervalový odhad střední hodnoty
mean(data$dBOP, na.rm = TRUE)

t.test(data$dBOP, alternative = "two.sided", conf.level = 0.95)$conf.int


# bodový a intervalový odhad směrodatné odchylky
sd(data$dBOP, na.rm = TRUE)

IOvar <- EnvStats::varTest(data$dBOP, alternative = "two.sided", conf.level = 0.95)$conf.int
sqrt(IOvar)


# na jakou přesnost můžeme zaokrouhlit?
sum(!is.na(data$dBOP))

# * Příklad 8. ####
# V datovém souboru `data/pr8.xlsx` naleznete měření doby do poruchy elektrické
# součástky [h]. Spočtěte 99% intervalový odhad průměrné životnosti daného typu
# součastky.


# načtení dat
data <- readxl::read_excel("data/pr8.xlsx")
head(data)


# vizualizace a ověření OP
boxplot(data$cas_h)


# jedná se skutečně o OP? nebo rozdělení je "odlišné" od normálního?
# např. exponenciální...

hist(data$cas_h)
# dle histogramu to vypadá, že data pocházejí z exponenciálního rozdělení
# tedy identifikované "OP" jsou ve skutečnosti "správná" data

# můžeme jít rovnou bodový a intervalový odhad mediánu
# tedy znaménkový test, exponentiální rozdělení není symetrické
median(data$cas_h)
# IO
# install.packages("BSDA")
alpha <- 0.01
BSDA::SIGN.test(data$cas_h,
    alternative = "two.sided", conf.level = 1 - alpha,
    conf.int = TRUE
)$conf.int


# na jakou přesnost můžeme zaokrouhlit?
length(data$cas_h)
sd(data$cas_h)
