doMonad :: [(Int, Char)]
doMonad = do 
    n <- [1, 2]
    ch <-['a', 'b']
    return (n, ch)

