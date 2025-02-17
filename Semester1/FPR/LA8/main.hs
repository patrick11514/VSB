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

instance (Show Expr) where
  show = showExpr

showExpr :: Expr -> String
showExpr (Num x) = show x
showExpr (Add x y) = "(" ++ showExpr x ++ "+" ++ showExpr y ++ ")"
showExpr (Mul x y) = showExpr x ++ "*" ++ showExpr y
showExpr (Sub x y) ="(" ++ showExpr x ++ "-" ++ showExpr y ++ ")"
showExpr (Div x y) =showExpr x ++ "/" ++ showExpr y
showExpr (Var x) = [x]


deriv :: Expr-> Char -> Expr
deriv (Num x)   c = Num 0 
deriv (Add x y) c = Add (deriv x c) (deriv y c) 
deriv (Mul x y) c = Add (Mul (deriv x c) y) (Mul y (deriv y c))
deriv (Sub x y) c = Sub (deriv x c) (deriv y c)
deriv (Div x y) c = Div (Sub (Mul (deriv x c) y) (Mul y (deriv y c))) (Mul y y)
deriv (Var x)   c | c==x = Num 1
                | otherwise = Num 0 