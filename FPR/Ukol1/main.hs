{-# OPTIONS_GHC -Wno-incomplete-patterns #-}
type Result = [String]

pp :: Result -> IO ()
pp x = putStr (unlines x)

sampleMoves :: [(Int, Int)]
sampleMoves = [(1,1),(8,8),(2,2),(3,3),(4,2),(3,2)]

--ticktack::(Int,Int) -> [(Int,Int)] -> Result
ticktack (w,h) moves = let
    pairMovesWithSymbols :: [(Int, Int, [Char])] -> [(Int, Int)] -> Int -> [(Int, Int, [Char])]
    pairMovesWithSymbols list [] _ = list
    pairMovesWithSymbols list ((x,y):xs) symbol | odd symbol = (x,y,"x"):pairMovesWithSymbols list xs (symbol+1)
                                                | otherwise = (x,y,"o"):pairMovesWithSymbols list xs (symbol+1)
    
    makeLine :: String -> [String] -> String
    makeLine to [] = to
    makeLine to (x:xs) = x++makeLine to xs

    isHere :: Int -> Int -> (Int,Int,String) -> Bool
    isHere x y (currentX,currentY,symbol) | x == currentX && y == currentY = True
                                          | otherwise = False

    getCurrentSymbol :: [(Int, Int, String)] -> String
    getCurrentSymbol [(_,_,symbol)] = symbol

    getSymbol :: [(Int, Int, String)] -> Int -> Int -> String
    getSymbol moves x y | length (filter (isHere x y) moves) > 0 = getCurrentSymbol (filter (isHere x y) moves)
                        | otherwise = " "

    generate :: [(Int, Int, String)] -> Int -> Int -> Result
    generate moves w h = [makeLine [] [getSymbol moves x y | x <- [1..w]] | y <- [1..h]]

    addToLine :: Result -> Result
    addToLine list = ["|"++line++"|" | line <- list]

    addLine :: Int -> String
    addLine count = makeLine [] (replicate count "-")

    formatOutput :: Result -> Int -> Int -> Result
    formatOutput list w h = addLine (w+2):reverse (addLine (w+2):addToLine list)

    in formatOutput (generate (pairMovesWithSymbols [] moves 1) w h) w h