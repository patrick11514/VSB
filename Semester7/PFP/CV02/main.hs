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
