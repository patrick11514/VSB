data Tree a = Leaf a | Branch a (Tree a) (Tree a)

myTree :: Tree Int
myTree = Branch 4 (Branch 8 (Leaf 1) (Branch 8 (Leaf 7) (Leaf 5))) (Leaf 9)

depthTree :: Tree a -> Int
depthTree tree =  let
    nodeDepth :: Tree a -> Int -> [Int]
    nodeDepth (Leaf _) i = [i]
    nodeDepth (Branch _ a b) i = nodeDepth a (i+1) ++ nodeDepth b (i+1)
    in maximum (nodeDepth tree 0)

data MTree a = Node a [MTree a] deriving Show

myTree2 :: MTree Int
myTree2 = Node 4 [Node 8 [Node 1 [], Node 8 [Node 7 [], Node 5 []]], Node 5 [], Node 9 [Node 2 []], Node 10 [Node 1 [], Node 3 [], Node 5 []]]

sumTree :: MTree Int -> Int
sumTree (Node i [])= i
sumTree (Node i (x:xs)) = i + sumTree x + sumTree (Node 0 xs)

sumTree' :: MTree Int -> Int
sumTree' (Node i list) = i + sum (map sumTree' list)

toList :: MTree a -> [a]
toList (Node i []) = [i]
toList (Node i list) = i : concatMap toList list

myCount :: MTree a -> Int
myCount (Node _ list) | length list > 2 = 1 + sum(map myCount list)
                      | otherwise = sum(map myCount list)