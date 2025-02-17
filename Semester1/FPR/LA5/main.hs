import Data.Char (isUpper)
--cv 4

take' :: Int -> [a] -> [a]
take' _ []     = []
take' 0 _      = []
take' n (x:xs) = x:take' (n-1) xs

minimum' :: Ord a => [a] -> a
minimum' [x]    = x
minimum' (x:y:z) | x < y = minimum' (x:z)
                | otherwise = minimum' (y:z)

dotProduct :: [a] -> [b] -> [(a,b)]
dotProduct a b = [(x, y) | y <- b, x <- a]

--cv5

oddList :: Int -> Int -> [Int]
oddList a b = [x | x <- [a..b], odd x]

removeAllUpper :: String -> String
removeAllUpper a = [x | x <- a, not (isUpper x)]

union :: Eq a => [a] -> [a] -> [a]
union a b = a ++ [x | x <- b, notElem x a]

intersection :: Eq a => [a] -> [a] -> [a]
intersection a b = [x | x <- a, elem x b]