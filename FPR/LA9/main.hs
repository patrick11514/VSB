data Tree a = Leaf a | Branch a (Tree a) (Tree a)

myTree :: Tree Int
myTree = Branch 4 (Branch 8 (Leaf 1) (Branch 8 (Leaf 7) (Leaf 5))) (Leaf 9)

sum' :: Tree Int -> Int
sum' (Leaf a) = a
sum' (Branch a b c) = a + sum' b + sum' c

toList :: Tree a -> [a]
toList (Leaf a) = [a]
toList (Branch a b c) = [a] ++ toList b ++ toList c

toString :: Show a => Tree a -> String
toString (Leaf a) = show a
toString (Branch a b c) = show a ++ "(" ++ toString b ++ "," ++ toString c ++ ")"