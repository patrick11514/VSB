type Result = [String]

pp :: Result -> IO ()
pp x = putStr (unlines x)

data Point = Point Int Int
data Shape = Ellipse Point Point Int
           | Square {topLeft:: Point, size::Int}

example :: [Shape]
example = [Ellipse (Point 8 4) (Point 16 4) 6, Square {topLeft = Point 15 5, size = 6 }, Ellipse (Point 25 7) (Point 35 12) 7]

--view :: (Int,Int) -> [Shape] -> Result
view (width, height) shapes = let 
    placeChar :: Int -> Int -> [(Int, Int)] -> [Char]
    placeChar x y coords | elem (x,y) coords = "#"
                         | otherwise = "."

    renderBackground :: Int -> Int -> [(Int, Int)] -> Result
    renderBackground w h coords = [concat [placeChar cw ch coords | cw <- [0..w]] | ch <- [0..h]]

    render :: Int -> Int -> [Shape] -> Result
    render w h s =  renderBackground w h (parseShapes s)

    parseShapes :: [Shape] -> [(Int, Int)]
    parseShapes [] = []
    parseShapes ((Ellipse (Point x1 y1) (Point x2 y2) a):xs) = parseShapes xs -- elipse not implemented
    parseShapes ((Square (Point x y) w):xs) = squareToPoints x y w ++ parseShapes xs

    squareToPoints :: Int -> Int -> Int -> [(Int, Int)]
    squareToPoints x y w = [(sx, y) | sx <- [x..w+x]]  ++  --top lone
                           [(sx, y+w) | sx <- [x..w+x]] ++ --bottom line
                           [(x, sy) | sy <- [y..y+w]] ++   --left line
                           [(x+w, sy) | sy <- [y..y+w]]    --right line

    --ellipseToPoints (x1,y1) (x2,y2) a = 

    in render width height shapes