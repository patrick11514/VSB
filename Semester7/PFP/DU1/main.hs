import Data.List
import Data.Binary.Get (skip)
type Transition = (Int, Char, Int)
type Automaton = (Int, String, [Transition], Int, [Int])

ex1 :: Automaton
ex1 = (3, "ab", [(0,'a',1), (0,'b',0), (1,'a',1), (1,'b',2), (2,'a',1), (2,'b',0)], 0, [2])

ex2 :: Automaton
ex2 = (3, "ab", [(0,'a',1), (0,'a',0), (0,'b',0), (1,'b',2)], 0, [2])

isDeterministic:: Automaton  -> Bool
isDeterministic (_, _, transitions, _, _) = allUnique [(s, c) | (s, c, _) <- transitions]
    where
        allUnique [] = True
        allUnique (x:xs) = x `notElem` xs && allUnique xs

isAccepting :: Automaton -> String -> Bool
isAccepting (_, _, trans, start, acc) word =
    any (`elem` acc) (run word [start])
    where
        run :: String -> [Int] -> [Int]
        run [] states     = states
        run (x:xs) states = run xs (step states x)

        step :: [Int] -> Char -> [Int]
        step states ch = [ to | (from, via_char, to) <- trans, from `elem` states, ch == via_char ]

convert:: Automaton -> Automaton
convert (statesCount, alphabet, trans, start, acc) = let
        newStates = generateNewStates [[start]] []
        transitions = generateTransitions newStates
        rename = renameMap newStates

        start2 = findNewStart rename
        acc2 = acceptingStates rename
        trans2 = transformTransitions transitions rename

    in (length newStates, alphabet, trans2, start, acc)
    where
        dedup :: Eq a => [a] -> [a]
        dedup [] = []
        dedup (x:xs)
            | x `elem` xs = dedup xs
            | otherwise   = x : dedup xs

        nextState :: [Int] -> Char -> [Int]
        nextState [] _ = []
        nextState (x:xs) ch = sort [to | (from, via_char, to) <- trans, from == x, ch == via_char ] ++ nextState xs ch

        processState :: [Int] -> [[Int]]
        processState from = [nextState from ch | ch <- alphabet]

        exclude :: Eq a => [a] -> [[a]] -> [[a]]
        exclude _ [] = []
        exclude item (x:xs) | item == x = exclude item xs
                            | otherwise = x : exclude item xs

        skipKnown :: Eq a => [[a]] -> [[a]] -> [[a]]
        skipKnown _ [] = []
        skipKnown known (x:xs) | x `elem` known = skipKnown known xs
                               | otherwise      = x : skipKnown known xs

        generateNewStates :: [[Int]] -> [[Int]] -> [[Int]]
        generateNewStates [] processed = processed
        generateNewStates (x:xs) processed = let
                    newStates = processState x --Here we get all possible next states from state x
                    excluded = exclude x newStates -- We exclude the current state from the new states, because we already processed
                    skipped = skipKnown processed excluded -- We need to skip all already processed states in past
                    deduped = dedup skipped
                    -- and we run it again in 'loop'
                    in generateNewStates (xs ++ deduped) (x : processed)

        -- generate temp transitions 
        generateTransitions :: [[Int]] -> [([Int], Char, [Int])]
        generateTransitions [] = []
        generateTransitions (x:xs) = let
            next = [(x, ch, nextState x ch) | ch <- alphabet]
            in next ++ generateTransitions xs

        renameMap :: [[Int]] -> [([Int], Int)]
        renameMap states = zip states [0..]

        --findNewStart :: [([Int], Int)] -> Int
        findNewStart mapping = [idx | (stateSet, idx) <- mapping, stateSet == [start]]

        acceptingStates :: [([Int], Int)] -> [Int]
        acceptingStates mapping = [idx | (stateSet, idx) <- mapping, any (`elem` acc) stateSet]

        transformTransitions :: [([Int], Char, [Int])] -> [([Int], Int)] -> [Transition]
        transformTransitions [] _ = []
        transformTransitions ((fromSet, ch, toSet):xs) map = let
            fromIdx = head [idx | (stateSet, idx) <- map, stateSet == fromSet]
            toIdx = head [idx | (stateSet, idx) <- map, stateSet == toSet]
            in (fromIdx, ch, toIdx) : transformTransitions xs map