import Data.Char 

main = do
    putStrLn "Hello, what's your name?"
    name <- getLine -- 束縛
--    name = getLine -- =ではだめ！これはgetLine関数に別の呼称nameを与えるに過ぎない。IOアクションから値を取り出すには束縛しなければならない。
    let largename = map toUpper name
    putStrLn ("Hey " ++ largename ++ "!")
