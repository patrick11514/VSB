pythagoras a b = sqrt(a^2 + b ^2)

factorial :: Int -> Int
factorial 0 = 1
factorial n = n * factorial (n -1)

fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib n = fib (n - 1) + fib (n - 2)