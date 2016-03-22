import Data.List 
-- import qualified Data.List as M -- M.nubのように名前空間付きアクセス
-- import Data.List (nub, sort) -- 一部だけロード
-- import Data.List hiding (nub, sort) -- 一部以外ロード

numUniques :: (Eq a) => [a] -> Int
numUniques = length . nub
