merge :: Ord a => [a] -> [a] -> [a]
merge xs [] = xs
merge [] ys = ys
merge (x:xs) (y:ys) | x < y = x : merge xs (y:ys)
                    | otherwise = y : merge (x:xs) ys

splitIt :: [a] -> (a->b) -> (a->c) -> ([b], [c])
splitIt xs f1 f2 = ([f1 x | x <- xs], [f2 x | x <- xs])

--splitWith :: Eq a => [a] -> [a] -> [[a]]
--splitWith [] _ = []
splitWith haystack needles = skipEmpty (reverse (collect haystack []))
    where
        collect [] collected = collected
        collect haystack collected = let 
            taked = takeWhile (`notElem` needles) haystack
            rest = dropWhile (`notElem` needles) haystack
            in collect (tail rest) (taked : collected)

        skipEmpty [] = []
        skipEmpty (x:xs) | length x > 0 = x : skipEmpty xs
                         | otherwise = skipEmpty xs

        
