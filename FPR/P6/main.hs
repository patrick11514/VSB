import Data.Char
type Result = [String]

pp :: Result -> IO ()
pp x = putStr (concat (map (++"\n") x))



ships :: Result -> [(Char, Int)] -> Result
ships input moves = let
    input' = reverse input --reverse input
    moves' = [((ord c - ord 'a'),r-1) | (c,r)<-moves] --a => 0..., 1 => 0 => (a,+) > (0,0)...
    get coord ch | elem coord moves' =  if ch == 'o' then 'x' else '.'
                 | otherwise = ch
    result = reverse [[get (ci, ri) col | (ci, col) <- zip [0..] row] | (ri, row) <- zip [0..] input']
    niceShow 10 = " 10"
    niceShow x = "  " ++ show x
    in [niceShow i ++ row | (i, row) <- zip (reverse [1..10]) result] ++ ["   abcdefg"]

sampleMoves :: [(Char, Integer)]
sampleMoves = [('a',1),('d',1),('d',2),('c',1),('b',1),('e',1),('f',1),('g',1),('c',7),('c',10)]
sampleInput :: [[Char]]
sampleInput = ["  o    o  ",
               "      ooo ",
               "   oo     ",
               "          ",
               "     o    ",
               "     o    ",
               "     o    ",
               "          ",
               "          ",
               "  oooo    "]
