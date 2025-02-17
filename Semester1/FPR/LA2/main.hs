factorial :: Int -> Int
factorial 0 = 1
factorial x = x * factorial (x - 1)

fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib x = fib (x - 1) + fib (x - 2)

leapYear :: Int -> Bool
leapYear x = mod x 4 == 0 && mod x 100 /= 0 || mod x 400 == 0

max2 :: Int -> Int -> Int
max2 x y | x > y = x
         | otherwise = y

max3 :: Int -> Int -> Int -> Int
max3 x y z | x > y  && x > z  = x
           | x > y  && x <= z = z
           | y >= x && y > z  = y
           | y >= x && y <= z = z
           | otherwise = x

combinations :: Int -> Int -> Int
combinations x y = div (factorial x) (factorial y * factorial (x - y))

numberOfRoots :: Int -> Int -> Int -> Int
numberOfRoots a b c = let d = b*b - 4 * a * c
                      in if d == 0
                            then 1
                         else if d > 0
                            then 2
                         else 0

--gcd' :: Int -> Int -> Int
--gcd' x y = 

--isPrime :: Int -> Bool
--isPrime x = let z = (2^x - 2) / x
--            in round z == z