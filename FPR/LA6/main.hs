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
flipV = map reverse

flipH :: Pic -> Pic
flipH = reverse

above :: Pic -> Pic -> Pic
above = (++)

sideBySide :: Pic -> Pic -> Pic
sideBySide = zipWith (++)

toColumn :: String -> Pic
toColumn str =  [[x] | x<-str]

rotateR :: Pic -> Pic
rotateR [] = []
rotateR [x] = toColumn x
rotateR (x:xs) = sideBySide (rotateR xs) (toColumn x)
    
rotateL :: Pic -> Pic
rotateL [] = []
rotateL [x] = toColumn x
rotateL (x:xs) = sideBySide (toColumn x) (rotateL xs)