import Debug.Trace
main = do
    (putStrLn . show) $ numDivisable 1

numDivisable :: Integer -> Integer
numDivisable n =   
    if (trace (show n ++ "#n\n" ++ show l ++ "#l") $ l >= 5) then 
        (n+1)*n`div`2
    else 
        numDivisable (n+1)
    where   l = length (divisables ((n+1)*n`div`2))
            divisables n = [x | x <- [1..(n+1)*n`div`2], n `mod` x == 0] 
               

