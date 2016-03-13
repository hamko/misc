import Control.Applicative 
myAction :: IO String
myAction = (++) <$> getLine <*> getLine
