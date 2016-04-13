main = do 
    (putStrLn . show) $ foldr (+) 0 [x | x <-[1..1000], x `mod` 3 == 0 && x `mod` 5 == 0]
--    (putStrLn . show . foldr) $ (+) 0 [x | x <-[1..1000], x `mod` 3 == 0 && x `mod` 5 == 0]
