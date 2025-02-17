--Funkce která z mužského příjmení doplní přichýlení a udělá ženské
makeGirl :: String -> String
makeGirl x = x++"ova"

-- Seznam obsahující Y mocnin čísla X
powers :: Int -> Int -> [Int]
powers x y = let
        make _ _ 0 p = p
        make x y c p = x^y:make x (y + 1) (c - 1) p
        in make x 0 y []

--Číslo, které se opakuje nejvíce krát v seznamu
mostFrequent :: [Int] -> Int
mostFrequent x = let
        isEqual :: Int -> Int -> Bool
        isEqual a b = a == b

        count :: [Int] -> Int
        count [] = 0
        count (a:ax) = 1 + count ax

        getCount :: Int -> [Int] -> Int
        getCount x l = count (filter (isEqual x) l)

        twins :: [Int] -> [(Int, Int)]
        twins x = [(a, getCount a x) | a <- x]

        getHighest :: [(Int, Int)] -> Int -> Int -> Int
        getHighest [] n cc = n
        getHighest ((a,c):xs) n cc | c > n = getHighest xs a c
                                   | otherwise = getHighest xs n cc  

        in getHighest (twins x) 0 0