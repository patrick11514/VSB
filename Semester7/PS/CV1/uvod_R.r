# ......................................................................................
# .............................Cvičení 1 - Stručný úvod do R............................
# ..................Michal Béreš, Adéla Vrtková, Martina Litschmannová..................
# ......................................................................................

# Nezobrazuje-li se vám text korektně, nastavte File \ Reopen with Encoding... na UTF-8
# Pro zobrazení obsahu skriptu použijte CTRL+SHIFT+O
# Pro spouštění příkazů v jednotlivých řádcích použijte CTRL+ENTER

# Nejprve si projdeme některé základy Rka a naimplementujeme si některé kombinatorické
# funkce.
#
#  1. Krátký úvod do R ####


# jednoduché početní operace
2 + 4
5 / 2


# POZOR na závorky! Pro počítání se používají pouze kulaté!
# Hranaté a složené mají v R jinou funkci!
(((10 + 2) * (340 - 33)) - 2) / 3


# kombinační číslo, faktoriály
choose(10, 2)
factorial(4)


# datové typy -> numeric, character, logical, (complex)
# funkcí class zjišťujeme typ objektu
a <- 2 + 3
class(a)


b <- "pismenko"
class(b)


c <- (1 > 3)
class(c)


d <- 3 + 1i
class(d)


#  2. datové struktury v R ####
#
# - vector (rozumíme sloupcový vektor)
# - factor (speciální případ vektoru)
# - matrix (matice s rozměry n x m)
# - data.frame (datový rámec s rozměry n x p)
# - list (seznam)


# definování vektoru (sloupcový=column)
a <- c(3, 4, 6, 7)
a <- c(3, 4, 6, 7)
a[2]


# další možnosti
rep(1, 4) # vytvoří vektor se čtyřmi jedničkami


seq(1, 10, 2) # posloupnost od 1 do 10 s krokem 2


1:10 # posloupnost od 1 do 10 s krokem 1


b <- c("A", "B", "C", "D")
b


class(b)


# předefinování objektu na jiný typ - např. as.vector, as.matrix, as.factor,...
b <- as.factor(b)
b


# práce s vektory - slučování podle sloupců/řádků
cbind(a, b)


rbind(a, b)


c(a, b)


## definování matice
A <- matrix(c(3, 4, 6, 7, 3, 2), nrow = 2, ncol = 3)
B <- matrix(c(3, 4, 6, 7, 3, 2), nrow = 2, ncol = 3, byrow = TRUE)
C <- matrix(c(3, 4, 6, 7, 3, 2), nrow = 3, ncol = 2)


B


B[1, 3]


A[1, ]


A[, 2:3]


# diagonální matice
diag(4)


diag(4, 2)


## operace s maticemi - pozor na maticové násobení -> %*%
A + B


A - B


A * B


A %*% C
