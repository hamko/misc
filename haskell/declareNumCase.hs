declareNumCase :: Double -> String
declareNumCase n = "Num = " ++
    case n  of 1     -> "1"
               2     -> "2"
               _     -> "other"
