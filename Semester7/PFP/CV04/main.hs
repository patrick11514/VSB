import Data.Char (isUpper, isLower)
oddList :: Int -> Int -> [Int]
oddList a b = [x | x <- [a..b], odd x]

removeAllUpper :: String -> String
removeAllUpper s = [ch | ch <- s, isLower ch]

union :: Eq a => [a] -> [a] -> [a]
union a b = [x | x <- a, x `notElem` b] ++ b

intersection :: Eq a => [a] -> [a] -> [a]
intersection a b = [x | x <- a, x `elem` b]

unique :: Eq a => [a] -> [a]
unique [] = []
unique (x:xs) = x : unique [y | y <- xs, y /= x]

countThem :: String -> [(Char, Int)]
countThem str =[(ch, count str ch 0) | ch <- unique str] where
    count [] _ n = n
    count (x:xs) ch n | x == ch = count xs ch (n + 1)
                      | otherwise = count xs ch n

partition :: (a -> Bool) -> [a] -> ([a], [a])
partition f l = ([x | x <- l, f x], [x | x <- l, not (f x)])

split :: [(a,b)] -> ([a],[b])
split [] = ([], [])
split ((a,b):rest) = let
    (xs, ys) = split rest
    in (a:xs, b:ys)

divideList :: [a] -> Int -> [[a]]
divideList l n  | length l < n = [l]
                | otherwise = take n l : divideList (drop n l) n

type Pic = [String]

pp :: Pic -> IO ()
pp x = putStr (unlines x)

pic :: Pic
pic = [ "....#....",
        "...###...",
        "..#.#.#..",
        ".#..#..#.",
        "....#....",
        "....#....",
        "....#####"]


flipV :: Pic -> Pic
flipV [] = []
flipV xs = [reverse x | x <- xs]

flipH :: Pic -> Pic
flipH = reverse

above :: Pic -> Pic -> Pic
above a b = a ++ b

sideBySide :: Pic -> Pic -> Pic
--sideBySide xs ys = [x ++ y | (x, y) <- zip xs ys]
sideBySide = zipWith (++)

toCol :: String -> Pic
toCol xs = [[x] | x <- xs]

rotateR :: Pic -> Pic
rotateR [a] = toCol a
rotateR (x:xs) = rotateR xs `sideBySide` toCol x 

rotateL :: Pic -> Pic
rotateL [x] = reverse(toCol x)
rotateL (x:xs) = reverse (toCol x) `sideBySide` rotateL xs