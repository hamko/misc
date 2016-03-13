fn :: Double -> Int
fn = ceiling . negate . tan . cos . max 50 
-- fn x = ceiling(negate(tan(cos(max 50 x))))
-- fn x = ceiling . negate . tan . cos . max 50 x
-- 両辺の最後が一致していれば、引数を省略可能
