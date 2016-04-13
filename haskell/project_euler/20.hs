import Data.Char
main = do
    (putStrLn . show . foldr (\x y -> y + ord x - ord '0') 0 . show) $ factorical 100

factorical :: Integer -> Integer
factorical 0 = 1
factorical n = n * factorical(n-1)
