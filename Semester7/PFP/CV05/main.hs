data Expr = Num Int
          | Add Expr Expr
          | Sub Expr Expr
          | Mul Expr Expr
          | Div Expr Expr
          | Var Char
      deriving (Eq)

eval :: Expr -> Int
eval (Num n) = n
eval (Add a b) = eval a + eval b
eval (Sub a b) = eval a - eval b
eval (Mul a b) = eval a * eval b
eval (Div a b) = eval a `div` eval b

showExpr :: Expr -> String
showExpr (Num n) = show n
showExpr (Add a b) = showExpr a ++ "+" ++ showExpr b
showExpr (Sub a b) = showExpr a ++ "-" ++ showExpr b
showExpr (Mul a b) = showExpr a ++ "*" ++ showExpr b
showExpr (Div a b) = showExpr a ++ "/" ++ showExpr b
showExpr (Var c) = [c]

expr1 = Mul (Add (Num 1) (Num 2)) (Num 3)

deriv :: Expr -> Char -> Expr
deriv (Num _) ch = Num 0 
deriv (Var a) ch | a == ch = Num 1
                 | otherwise = Num 0
deriv (Add a b) ch = Add (deriv a ch) (deriv b ch)
deriv (Sub a b) ch = Sub (deriv a ch) (deriv b ch)
deriv (Mul a b) ch = Add (Mul (deriv a ch) b) (Mul b (deriv b ch))
deriv (Div a b) ch = Div (Sub (Mul (deriv a ch) b) (Mul b (deriv b ch))) (Mul b b)

data Tree a = Leaf a 
            | Branch a (Tree a) (Tree a)

someTree :: Tree Int
someTree = Branch 10 (Leaf 5) (Branch 8 (Leaf 2) (Leaf 5)) 

sum' :: Tree Int -> Int
sum' (Leaf a) = a
sum' (Branch a l r) = a + sum' l + sum' r

toList :: Tree a -> [a]
toList (Leaf a) = [a]
toList (Branch a l r) = [a] ++ toList l ++ toList r

toString :: Show a => Tree a -> String
toString (Leaf a) = show a
toString (Branch a l r) = show a ++ "(" ++ toString l ++ "," ++ toString r ++ ")"

--fromString :: Read a => String -> Tree a
--fromString 

test :: String -> Int
test str = read str