module Car
(Car(..), tellCar) where

data Car = Car  { company :: String
                , model :: String
                , year :: Int
                } deriving(Show)
-- レコード表記だと、自動的に company model yearの関数が作られる。

tellCar :: Car -> String
tellCar (Car {company = c, model = m, year = y}) =  -- レコード表記のパターンマッチもできる
    "This " ++ c ++ " " ++ m ++ " was made in " ++ show y 
