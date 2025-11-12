data FileType = Image | Executable | SourceCode | TextFile deriving Show
data Entry = File { name :: String, size :: Int, ftype:: FileType }
           | Directory { name :: String, entries:: [Entry] } deriving Show

root :: Entry
root = Directory "root"
    [File "logo.jpg" 5000 Image,
    Directory "classes"
        [ File "notes-fpr.txt" 200 TextFile,
          File "presentation.jpg" 150 Image,
          File "first_test.hs" 20 SourceCode ] ]

countSize :: Entry -> Int
countSize (File _ size _) = size
countSize (Directory _ entries) = sum [ countSize entry | entry <- entries ]

--countExtensions :: Entry -> [(String, Int)]
countExtensions entry =
    let
        getDotPosition :: String -> Int
        getDotPosition text = length (takeWhile (/= '.') text) + 1

        getExtension :: String -> String
        getExtension name = drop (getDotPosition name) name

        collectExtensions :: Entry -> [String]
        collectExtensions (File name _ _) = [getExtension name]
        collectExtensions (Directory _ entries) =
            let
                loop :: [Entry] -> [String]
                loop [] = []
                loop (x:xs) = collectExtensions x ++ loop xs
            in loop entries

        dedup :: [String] -> [String]
        dedup [] = []
        dedup (x:xs) | x `elem` xs = dedup xs
                     | otherwise = x : dedup xs

        count :: String -> [String ] -> Int
        count _ [] = 0
        count str (x:xs) | str == x = 1 + count str xs
                         | otherwise = count str xs

        extensions = collectExtensions entry
        deduped = dedup extensions

    in [(ext, count ext extensions) | ext <- deduped]


--copyFile :: Entry -> String -> String -> Entry
copyFile entry which to =
    let
        findFile :: [Entry] -> [Entry] 
        findFile [] = []
        findFile (x:xs) =
            let 
                tryFind :: Entry -> [Entry]
                tryFind (File name size ftype) | name == which = [File name size ftype]
                                               | otherwise = []
                tryFind (Directory _ entries) = findFile entries 
            in tryFind x ++ findFile xs

        foundFile = head $ findFile [entry]    

        injectFile :: Entry -> Entry
        injectFile (Directory name entries) | name == to = Directory name (foundFile : entries)
                                            | otherwise = Directory name [injectFile entry | entry <- entries]
        injectFile entry = entry -- for files

    in injectFile entry
