module Shapes
(Point(..), Shape(..), area) where
-- (Point, Shape, area) where -- こうすると、外部からPoint, Shapeの値コンストラクタを利用できるようになる。そうしないと、値コンストラクタは隠蔽される

-- データ型と値コンストラクタの定義
-- Point, Shapeは「データ型」
-- Point, Circle, Rectangleは「値コンストラクタ」
data Point = Point Float Float deriving (Show)
data Shape = Circle Point Float | Rectangle Point Point deriving (Show)

area :: Shape -> Float
area (Circle _ r) = pi * r ^ 2 -- 値コンストラクタのパターンマッチは括弧でくくる。
area (Rectangle (Point x1 y1) (Point x2 y2)) = (abs $ x2 - x1) * (abs $ y2 - y1) -- 引数の中の値コンストラクタのパターンマッチも同様に括弧でくくる。
