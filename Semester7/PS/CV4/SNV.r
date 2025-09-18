# ......................................................................................
# .........................Cvičení 4 - Spojitá náhodná veličina.........................
# ..................Michal Béreš, Martina Litschmannová, Adéla Vrtková..................
# ......................................................................................

# Nezobrazuje-li se vám text korektně, nastavte File \ Reopen with Encoding... na UTF-8
# Pro zobrazení obsahu skriptu použijte CTRL+SHIFT+O
# Pro spouštění příkazů v jednotlivých řádcích použijte CTRL+ENTER

# **Obsah tohoto skriptu je pouze jako doplňující ilustrace k cvičení, není nutno znát
# ke zkoušce. Důležité je to umět spočítat ručně.**
# * Numerická integrace v Rku ####
# Rkovská funkce **integrate**
# integrate(f, a, b) = $\int_{a}^{b}f(x)dx$
# - **f** je Rková funkce (námi definovaná) která má jeden vstupní argument - vektor
# hodnot ve kterém má vrátit své hodnoty
# - **a** dolní integrační mez
# - **b** horní integrační mez


f <- function(x) {
    return(x * x)
} # x^2
a <- -1
b <- 2
integrate(f, a, b)


# ---
#  Příklady k procvičení ####
# ---
# * Příklad 1. ####
# Náhodná veličina X má distribuční funkci
# $F(x)=\begin{cases}
# 0    &      x \leq 0 \\
# cx^2 &  0 < x \leq 1 \\
# 1    &  1 < x
# \end{cases}$
# Jaké hodnoty může nabývat konstanta c?


# c = 1, ze spojitosti F(x)
# cx^2 = 1 pro x = 1

# * Příklad 2. ####
# Rozdělení náhodné veličiny X je dáno hustotou
# $f(x)=\begin{cases}
# 2x+2 & x \in <-1;0> \\
# 0    & x \notin <-1;0>
# \end{cases}$
# Určete:
#
# **2. a)**
# $F(x)$,


F.dist <- function(x) {
    res <- x * x + 2 * x + 1 # x^2+2x+1
    res[x < -1] <- 0 # 0 pro x<=0
    res[x > 0] <- 1 # 1 pro x>1
    return(res)
}


x <- seq(from = -2, to = 1, by = 0.01) # body na ose x
FX <- F.dist(x) # hodnoty f(x)
plot(x, FX, type = "l") # vykreslit tečky (cex je velikost)


# **2. b)**
# $P(−2 ≤ X ≤ −0.5), P(−2 ≤ X ≤ −1), P(X > 0.5), P(X = 0.3)$


f.dens <- function(x) {
    res <- 2 * x + 2
    # pozor na x<-1 protože '<-' je v rku přiřazení
    res[x < -1] <- 0 # 0 pro x<=0
    res[x > 0] <- 0 # 1 pro x>1
    return(res)
}

# P(−2 ≤ X ≤ −0.5)
integrate(f.dens, -2, -0.5)$value
integrate(f.dens, -1, -0.5)$value


# P(−2 ≤ X ≤ −1)
integrate(f.dens, -2, -1)$value


# P(X > 0.5)
integrate(f.dens, 0.5, 100)$value # tohle nebude vždy fungovat


# P(X = 0.3)
integrate(f.dens, 0.3, 0.3)$value
# je jasné že tato pravděpodobnost je 0
# odpovídá integrálu s a=b tedy s nulovou velikostí na ose x


# **2. c)**
# střední hodnotu, rozptyl a směrodatnou odchylku náhodné veličiny X.


# E(X)
x_fx <- function(x) {
    fx <- f.dens(x)
    return(x * fx)
}
# integrujeme jen tam kde víme, že je f(x) nenulová
E_X <- integrate(x_fx, -1, 0)$value
E_X


# E(X^2)
xx_fx <- function(x) {
    fx <- f.dens(x)
    return(x * x * fx)
}
# integrujeme jen tam kde víme, že je f(x) nenulová
E_XX <- integrate(xx_fx, -1, 0)$value
E_XX


# D(X)
D_X <- E_XX - E_X^2
D_X


# sigma(x)
std_X <- sqrt(D_X)
std_X


# **2. d)**
# modus $\hat{x}$


# modus = 0


# **2. e)**
# medián $x_{0,5}$


x <- seq(from = -2, to = 1, by = 0.001) # body na ose x
FX <- F.dist(x)
plot(x, FX, type = "l")
lines(c(-2, 1), c(0.5, 0.5))


# * Příklad 3. ####
# Náhodná veličina $Y$ je definována jako: $Y = 3X+1$, kde $X$ je náhodná veličina z
# předcházejícího
# příkladu. Určete:
#
#
# **3. a)**
# $F_Y(y)$


FY.dist <- function(y) {
    # spočteno ze vztahu FY(y) = P(Y < y) = P(3X + 1 < y) = ...
    x <- (y - 1) / 3
    FY <- F.dist(x)
    return(FY)
}
y <- seq(from = -3, to = 2, by = 0.001) # body na ose x
FY <- FY.dist(y)
plot(y, FY, type = "l")


# **3. b)**
# $f_Y(y)$


# derivace F_Y
fY.dens <- function(y) {
    res <- 2 / 9 * (y + 2)
    res[y < -2] <- 0 # 0 pro x<-2
    res[y > 1] <- 0 # 1 pro x>1
    return(res)
}
integrate(fY.dens, -2, 1)$value # kontrola celkového integrálu
y <- seq(from = -3, to = 2, by = 0.001) # body na ose x
fY <- fY.dens(y)
plot(y, fY, cex = 0.2)


# **3. c)**
# $E(Y), D(Y), \sigma(Y)$


# E(Y)
y_fy <- function(y) {
    fy <- fY.dens(y)
    return(y * fy)
}
# integrujeme jen tam kde víme, že je f(y) nenulová
E_Y <- integrate(y_fy, -2, 1)$value
E_Y
0


# alternativně
E_Y <- 3 * E_X + 1
E_Y


# E(Y^2)
yy_fy <- function(y) {
    fy <- fY.dens(y)
    return(y * y * fy)
}
# integrujeme jen tam kde víme, že je f(y) nenulová
E_YY <- integrate(yy_fy, -2, 1)$value
E_YY
1 / 2


# D(Y)
D_Y <- E_YY - E_Y^2
D_Y
1 / 2


# alternativně
D_Y <- 3^2 * D_X
D_Y


# sigma(Y)
sqrt(D_Y)
sqrt(2) / 2


# * Příklad 4. ####
# Spočtěte $\omega$ takové, aby náhodná veličina $X$ s hustotou pravděpodobnosti:
# $f(x)=\begin{cases}
# 0 & x < 0 \\
# 3e^{-3x}    & x \geq 0
# \end{cases}$
# byla s pravděpodobností 0.3 větší než $\omega$


F.dist <- function(x) {
    res <- 1 - exp(-3 * x)
    res[x < 0] <- 0 # 0 pro x<=0
    return(res)
}


x <- seq(from = -1, to = 3, by = 0.001) # body na ose x
FX <- F.dist(x)
plot(x, FX, type = "l")
lines(c(-1, 3), c(0.7, 0.7))


x[FX >= 0.7][1]


-1 / 3 * log(0.3)
