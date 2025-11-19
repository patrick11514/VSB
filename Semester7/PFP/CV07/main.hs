import System.IO (openFile, IOMode (ReadMode), Handle, hGetContents, hClose)
import GHC.Read (readField)
main = do
    file <- getLine
    content <- readContent file 
    let res = numberLines content

    putStr res


readContent :: String -> IO String 
readContent file = do
    readFile file

numberLines :: String -> String
numberLines input = let
    splited = lines input

    number :: [String] -> Int -> [String]
    number [] _ = []
    number (x:xs) num = (show num  ++ x) : number xs (num + 1)

    in unlines (number splited 1)
