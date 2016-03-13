declareNum :: Double -> String
declareNum n
    | n < smallnum = "small"
    | n < makemediumnum smallnum = "medium"
    | otherwise = "large"
    where   smallnum = 1.0          -- あとで変数定義しても良い
            makemediumnum x = x * 2 -- あとで関数定義しても良い
