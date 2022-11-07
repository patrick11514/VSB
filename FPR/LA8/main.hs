{-# OPTIONS_GHC -Wno-incomplete-patterns #-}
data Expr = Num Int
          | Add Expr Expr
          | Sub Expr Expr
          | Mul Expr Expr
          | Div Expr Expr
          | Var Char
        deriving (Eq)

eval :: Expr -> Int
eval (Num x) = x
eval (Add x y) = eval x + eval y
eval (Sub x y) = eval x - eval y
eval (Mul x y) = eval x * eval y
eval (Div x y) = div (eval x) (eval y)

--deriv :: Expr-> Char -> Expr
--deriv 