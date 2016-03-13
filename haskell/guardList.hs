import Control.Monad

guardList :: [Int]
guardList = do
    x <- [1..50] 
    guard ('7' `elem` show x) 
    y <- [x, -x]
    guard(y `mod` 3 == 0)
    return y
