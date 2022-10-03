sude :: [a] ->[a]
sude [] = []
sude [x] = [x]
sude (x:_:xs) = x:sude xs

len :: [a] -> Int
len [] = 0
len (x : xs) = 1 + len xs

sumIt :: [Int] -> Int
sumIt [] = 0
sumIt (x:xs) = x + sumIt xs

getHead :: [a] -> a
getHead [] = error "Empty list"
getHead (x:_) = x

getLast :: [a] -> a
getLast [] = error "Empty list"
getLast [x] = x
getLast (_:xs) = getLast xs

isElement :: Eq a => a -> [a] -> Bool
isElement _ [] = False
isElement x (y:ys) = x == y || isElement x ys

getTail :: [a] -> [a]
getTail [] = []
getTail (_:xs) = xs

getInit :: [a] -> [a]
getInit [] = []
getInit [x, _] = [x]
getInit (x:xs) = x : getInit xs

combine :: [a] -> [a] -> [a]
combine [] []         = []
combine [x] []        = [x]
combine (x:xs) []   =  x:xs
combine [] (x:y)      = x:y
combine (x:xs) (y:ys) = x:xs++y:ys

maxL :: [Int] -> Int
maxL [] = 0
maxL [x] = x
maxL [x, y] | x > y = x
            | otherwise = y
maxL (x:y:xs) | x >  y && x > maxL xs = x
              | x >  y && x < maxL xs = maxL xs
              | x <= y && x > maxL xs = y
              | x <= y && x < maxL xs && y > maxL xs = y
              | otherwise = maxL xs

reverseL :: [a] -> [a]
reverseL []   = []
reverseL [x] = [x]
reverseL [x, y] = [y,x]
reverseL (x : xs) = (reverseL xs)++[x]

scalar :: [Int] -> [Int] -> Int
scalar [_] []        = 0
scalar [] []         = 0
scalar [] (_:_)      = 0
scalar (_:_:_) []    = 0
scalar (x:xs) (y:ys) = x*y + scalar xs ys