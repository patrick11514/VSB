{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Use catMaybes" #-}
type Transition = (Int, Char, Int)
type Automaton = (Int, String, [Transition], Int, [Int])

ex1 :: Automaton
ex1 = (3, "ab", [(0,'a',1), (0,'b',0), (1,'a',1), (1,'b',2), (2,'a',1), (2,'b',0)], 0, [2])

ex2 :: Automaton
ex2 = (3, "ab", [(0,'a',1), (0,'a',0), (0,'b',0), (1,'b',2)], 0, [2])

data RegExpr = Epsilon
             | Symbol Char
             | Iteration RegExpr
             | Concat RegExpr RegExpr
             | Alter RegExpr RegExpr deriving (Eq, Show)

reg1 :: RegExpr
reg1 = Concat (Concat (Iteration (Alter (Symbol 'a') (Symbol 'b'))) (Symbol 'a')) (Symbol 'b')


epsilon :: Char
epsilon = 'ε'

dedupAlphabet :: [Char] -> [Char] -> [Char]
dedupAlphabet [] [] = []
dedupAlphabet a b = a ++ [ c | c <- b, c `notElem` a ]

offsetTransitions :: [Transition] -> Int -> [Transition]
offsetTransitions trans offset = [ (s1 + offset, c, s2 + offset) | (s1, c, s2) <- trans ]

convertWithEpsilon :: RegExpr -> Automaton
convertWithEpsilon Epsilon = (2, "", [(0, epsilon, 1)], 0, [1])
convertWithEpsilon (Symbol c) = (2, [c], [(0, c, 1)], 0, [1])
convertWithEpsilon (Concat a b) =
    let (states1, alp1, trans1, start1, acc1) = convertWithEpsilon a
        (states2, alp2, trans2, start2, acc2) = convertWithEpsilon b

        offset = states1
        -- Trans1 + Trans2 with connecting ε-transitions
        concatTrans = trans1 ++ offsetTransitions trans2 offset ++ [ (a, epsilon, start2 + offset) | a <- acc1 ]

    in (states2 + offset, dedupAlphabet alp1 alp2, concatTrans, start1, [ a + offset | a <- acc2 ])
convertWithEpsilon (Iteration a) = 
    let (states, alp, trans, start, acc) = convertWithEpsilon a
        offseted = offsetTransitions trans 1 -- Here because we add a new start state

        newStart = 0
        newEnd = states + 1 

        -- new_start -> old start || new_start -> new_end || old_accept -> old_start || old_accept -> new_end
        newTrans = (newStart, epsilon, start + 1) : 
                   (newStart, epsilon, newEnd) : 
                   [ (a + 1, epsilon, start + 1) | a <- acc ] ++ 
                   [ (a + 1, epsilon, newEnd) | a <- acc ]

        allTrans = newTrans ++ offseted
    in (states + 2, alp, allTrans, 0, [newEnd])
convertWithEpsilon (Alter a b) =
    let (states1, alp1, trans1, start1, acc1) = convertWithEpsilon a
        (states2, alp2, trans2, start2, acc2) = convertWithEpsilon b

        offset = states1 + 1 -- +1 for new start state
        newStart = 0
        newEnd = states1 + states2 + 1
        -- new_start -> old_start_a || new_start -> old_start_b || old_accept_a -> new_end || old_accept_b -> new_end
        newTrans = (newStart, epsilon, start1 + 1) :
                   (newStart, epsilon, start2 + offset) :
                   [ (a + 1, epsilon, newEnd) | a <- acc1 ] ++
                   [ (b + offset, epsilon, newEnd) | b <- acc2 ]    

        allTrans = newTrans ++ offsetTransitions trans1 1 ++ offsetTransitions trans2 offset
    in (states1 + states2 + 2, dedupAlphabet alp1 alp2, allTrans, newStart, [newEnd])

--convertWithEpsilon reg1 = https://upload.patrick115.eu/screenshot/Neuložený%20obrázek%2022131563.png

znkaIntoDfa :: Automaton -> Automaton
znkaIntoDfa (znkaStateCount, alphabet, znkaTrans, znkaStart, znkaAccept) =
    let
        removeDuplicates :: [Int] -> [Int]
        removeDuplicates [] = []
        removeDuplicates (x:xs) = x : removeDuplicates [y | y <- xs, y /= x]

        --Clε -> eg: we get all transitions reachable via ε from a set of states
        epsilonClosure :: [Int] -> [Int]
        epsilonClosure stateList =
            let 
                step closure [] = closure
                step closure (s:ss) =
                    let newStates = [ s2 | (s1, c, s2) <- znkaTrans, s1 == s, c == epsilon, s2 `notElem` closure ]
                        newClosure = closure ++ newStates
                    in step newClosure (ss ++ newStates)
            in removeDuplicates (step stateList stateList) 

        -- Transition function move
        move :: [Int] -> Char -> [Int]
        move currentStates sym = removeDuplicates [ s2 | s <- currentStates, (s1, c, s2) <- znkaTrans, s1 == s, c == sym ]

        sort :: [Int] -> [Int]
        sort [] = []
        sort (x:xs) = insert x (sort xs)
          where
            insert x [] = [x]
            insert x (y:ys)
              | x <= y    = x : y : ys
              | otherwise = y : insert x ys

        buildDFA :: [[Int]] -> [([Int], Int)] -> [Transition] -> ([([Int], Int)], [Transition])
        buildDFA [] known dfaTrans = (known, dfaTrans) -- ending condition 

        --loop
        buildDFA (currentSet:work) known dfaTrans =
            let
                Just currentID = lookup currentSet known
                foldSymbol :: ( [[Int]], [([Int], Int)], [Transition] ) -> Char -> ( [[Int]], [([Int], Int)], [Transition] )
                foldSymbol (accWork, accKnown, accTrans) sym =
                    let 
                        movedStates = move currentSet sym
                        destSet_ = epsilonClosure movedStates
                        destSet = sort destSet_
                    in if null destSet
                       then (accWork, accKnown, accTrans) -- Dead end - ignore 
                       else 
                           case lookup destSet accKnown of
                             Just destID -> -- Known
                                 (accWork, accKnown, (currentID, sym, destID) : accTrans)
                                 
                             Nothing -> -- New state
                                 let newID = length accKnown
                                 in (
                                      destSet : accWork,                     
                                      (destSet, newID) : accKnown,         
                                      (currentID, sym, newID) : accTrans    
                                    )

                (newWork, newKnownStates, newTransitions) = foldl foldSymbol ([], known, []) alphabet
            
            in buildDFA (work ++ newWork) newKnownStates (dfaTrans ++ newTransitions)
        
        startSet_ = epsilonClosure [znkaStart]
        startSet = sort startSet_
        (finalStatesMap, finalTransitions) = buildDFA [startSet] [(startSet, 0)] []
        finalStateIDs = [ id | (set, id) <- finalStatesMap, 
                               any (`elem` znkaAccept) set ]

        finalStateCount = length finalStatesMap

    in (finalStateCount, alphabet, finalTransitions, 0, finalStateIDs)

-- final: https://upload.patrick115.eu/screenshot/4663f77d2b.png

convert :: RegExpr -> Automaton
convert reg = znkaIntoDfa $ convertWithEpsilon reg