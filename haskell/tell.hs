tell :: (Show a) => [a] -> String
tell [] = "Empty"
tell (x:[]) = "One: " ++ show x
tell (x:y:[]) = "Two: " ++ show x ++ " " ++ show y
tell (x:y:_) = "Many: " ++ show x ++ " " ++ show y ++ " and so on"
