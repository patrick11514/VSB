import Data.Char (toUpper)
gcd' :: Int -> Int -> Int
gcd' a 0 = a
gcd' a b = gcd' b (mod a b)

isPrime :: Int -> Bool
isPrime x = tryLoop 9  where
    tryDiv :: Int -> Bool
    tryDiv y = mod x y == 0

    tryLoop :: Int -> Bool
    tryLoop n | x == 1 = False
              | n == 1 = True
              | n == x = tryLoop (n - 1)
              | tryDiv n = False
              | not (tryDiv n) = tryLoop (n - 1)

sumIt :: [Int] -> Int
sumIt list = acc list 0 where
    acc :: [Int] -> Int -> Int
    acc [] s = s
    acc (x:xs) s = acc xs (s + x)

getHead :: [a] -> a
getHead (x:_) = x

getLast :: [a] -> a
getLast [a] = a
getLast (_:xs) = getLast xs

isElement :: Eq a => a -> [a] -> Bool
isElement a [] = False
isElement a (x:xs) | a == x = True
                   | otherwise = isElement a xs

getInit :: [a] -> [a]
getInit [_] = []
getInit (x:xs) = x : getInit xs

combine :: [a] -> [a] -> [a]
combine xs b = foldr (:) b xs

max' :: [Int] -> Int
max' (x:xs) = isBigger xs x where
    isBigger [] max = max
    isBigger (x:xs) max | x > max = isBigger xs x
                        | otherwise = isBigger xs max

reverse' :: [a] -> [a]
reverse' [] = []
reverse' (x:xs) = create [x] xs where
    create n [] = n
    create n (x:xs) = create (x:n) xs

reverse'' :: [a] -> [a]
reverse'' [] = []
reverse'' (x:xs) = reverse'' xs ++ [x]

take' :: Int -> [a] -> [a]
take' _ [] = []
take' count (x:xs) = reverse (save (count - 1) xs [x]) where
    save 0 _ n  = n
    save _ [] n = n
    save c (x:xs) n = save (c-1) xs (x:n)

take'' :: Int -> [a] -> [a]
take'' 0 xs = []
take'' _ [] = []
take'' n (x:xs) = x : take'' (n-1) xs

drop' :: Int -> [a] -> [a]
drop' _ [] = []
drop' count a = reverse (save count a []) where
    save _ [] a = a
    save 0 (x:xs) a = save 0 xs (x:a)
    save count (x:xs) a = save (count - 1) xs a

drop'' :: Int -> [a] -> [a]
drop'' 0 xs = xs
drop'' n [] = []
drop'' n (_:xs) = drop'' (n-1) xs

divisors :: Int -> [Int]
divisors n = cycle n where
    cycle 1 = [1]
    cycle i | mod n i == 0 = i:cycle (i-1)
            | otherwise = cycle (i-1)

divisors' n = [x | x <- [1..n], mod n x == 0]

zipThem :: [a] -> [b] -> [(a, b)]
zipThem [] _ = []
zipThem _ [] = []
zipThem (a:as) (b:bs) = (a,b) : zipThem as bs

dotProduct :: [a] -> [b] -> [(a,b)]
dotProduct x y = [(_x,_y) | _x <- x, _y <- y]

allToUpper :: String -> String
allToUpper s = [toUpper ch | ch <- s]

quicksort :: (Ord a) => [a] -> [a]
quicksort [] = []
quicksort (pivot:xs) = let
    left = [x | x <- xs, x <= pivot]
    right = filter (>pivot) xs
    in quicksort left ++ [pivot] ++ quicksort right