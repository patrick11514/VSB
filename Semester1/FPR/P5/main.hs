{-# OPTIONS_GHC -Wno-incomplete-patterns #-}
isPrime :: Int -> Bool
isPrime n = cyklus 2 n where
    cyklus i n | i == n = True
               | n `mod` i == 0 = False
               | otherwise = cyklus (i+1) n

isElement :: Eq a => a ->[a] -> Bool
isElement a [] = False
isElement a (x:xs) | x == a = True
                   | otherwise = isElement a xs

reverse' :: [a] -> [a]
reverse' xs = tmp xs [] where 
    tmp [] ys = ys
    tmp (x:xs) ys = tmp xs (x:ys)

reverse'' :: [a] -> [a]
reverse'' xs = foldl' (\acc x -> x:acc) [] xs

foldl1' :: (a->a->a) -> [a] -> a
foldl1' f (x:xs) = tmp f x xs where
    tmp f i [] = i
    tmp f i (x:xs) = tmp f (f i x) xs

foldl' :: (b->a->b) -> b -> [a] -> b
foldl' f i []     = i
foldl' f i (x:xs) = foldl' f (f i x) xs 