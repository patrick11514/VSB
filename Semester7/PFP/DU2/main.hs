type Transition = (Int, Char, Int)
type Automaton = (Int, String, [Transition], Int, [Int])

ex1 :: Automaton 
ex1 = (3, "ab", [(0,'a',1), (0,'b',0), (1,'a',1), (1,'b',2), (2,'a',1), (2,'b',0)], 0, [2],)

ex2 :: Automaton 
ex2 = (3, "ab", [(0,'a',1), (0,'a',0), (0,'b',0), (1,'b',2)], 0, [2])

data RegExpr = Epsilon
             | Symbol Char
             | Iteration RegExpr
             | Concat RegExpr RegExpr
             | Alter RegExpr RegExpr deriving (Eq, Show)

reg1 :: RegExpr 
reg1 = Concat (Concat (Iteration (Alter (Symbol 'a') (Symbol 'b'))) (Symbol 'a')) (Symbol 'b')


convertWithEpsilon :: RegExpr -> Automaton
convertWithEpsilon reg = case reg of
    Epsilon -> (2, "", [(0, 'ε', 1)], 0, [1])
    Symbol c -> (2, [c], [(0, c, 1)], 0, [1])
    Iteration r -> let (n, sigma, trans, start, finals) = convert r
                       newStart = n
                       newFinal = n + 1
                       newTrans = [(newStart, 'ε', start)] ++
                                  [(f, 'ε', start) | f <- finals] ++
                                  [(f, 'ε', newFinal) | f <- finals]

--without epsilon
convert :: RegExpr -> Automaton
convert reg = case reg of
    Epsilon -> (2, "", [(0, 'ε', 1)], 0, [1])
    Symbol c -> (2, [c], [(0, c, 1)], 0, [1])
    Iteration r -> let (n, sigma, trans, start, finals) = convert r
                       newStart = n
                       newFinal = n + 1
                       newTrans = [(newStart, 'ε', start)] ++
                                  [(f, 'ε', start) | f <- finals] ++
                                  [(f, 'ε', newFinal) | f <- finals]
                     in (n + 2, sigma, trans ++ newTrans, newStart, [newFinal])
    Concat r1 r2 -> let (n1, sigma1, trans1, start1, finals1) = convert r1
                        (n2, sigma2, trans2, start2, finals2) = convert r2
                        offset = n1
                        newTrans = [(f, 'ε', start2 + offset) | f <- finals1]
                        shiftedTrans2 = [(s + offset, c, d + offset) | (s, c, d) <- trans2]
                     in (n1 + n2, sigma1 ++ sigma2, trans1 ++ newTrans ++ shiftedTrans2, start1, [f + offset | f <- finals2])
    Alter r1 r2 -> let (n1, sigma1, trans1, start1, finals1) = convert r1
                       (n2, sigma2, trans2, start2, finals2) = convert r2
                       newStart = n1 + n2
                       newFinal = n1 + n2 + 1
                       newTrans = [(newStart, 'ε', start1), (newStart, 'ε', start2)] ++
                                  [(f, 'ε', newFinal) | f <- finals1 ++ finals2]
                     in (n1 + n2 + 2, sigma1 ++ sigma2, trans1 ++ trans2 ++ newTrans, newStart, [newFinal]) 