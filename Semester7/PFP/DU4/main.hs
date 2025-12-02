import Data.Array
import Data.Array.ST
import Control.Monad.ST
import Control.Monad (when)

quickSort :: Array Int Int -> Array Int Int
quickSort inputArr = runST $ do
    arr <- thaw inputArr :: ST s (STArray s Int Int)
    let (minIdx, maxIdx) = bounds inputArr

    qsort arr minIdx maxIdx

    freeze arr

  where
    qsort arr lo hi = when (lo < hi) $ do
        p <- partition arr lo hi
        qsort arr lo (p - 1)
        qsort arr (p + 1) hi

    partition arr lo hi = do
        pivot <- readArray arr hi
        
        let loop i j
                | j == hi   = return i
                | otherwise = do
                    val <- readArray arr j
                    if val <= pivot
                    then do
                        swap arr i j
                        loop (i + 1) (j + 1)
                    else loop i (j + 1)
        
        finalP <- loop lo lo
        swap arr finalP hi
        return finalP

    swap arr i j = do
        vi <- readArray arr i
        vj <- readArray arr j
        writeArray arr i vj
        writeArray arr j vi