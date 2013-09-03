{-# LANGUAGE BangPatterns #-}
import qualified Data.List.Stream as S
import qualified Criterion.Main as M

rk4 :: (Double -> Double -> Double) -> Double -> Double -> Double -> Double
rk4 y' !h !t !y = y + (h/6) * (k1 + 2*k2 + 2*k3 + k4)
  where k1 = y' t y
        k2 = y' (t + h/2) (y + ((h/2) * k1))
        k3 = y' (t + h/2) (y + ((h/2) * k2))
        k4 = y' (t + h) (y + (h * k3))

iterateRK :: (Double -> Double -> Double) -> Double -> Double -> Double -> [Double]
iterateRK y' h = curry $ S.unfoldr $ \(!t0, !y0) -> Just (y0, (t0 + h, rk4 y' h t0 y0))

benchmarkRK n =
  let y' t y  = -y
      h       = 1e-3
      y0      = 1.0
      t0      = 0
  in S.take n (iterateRK y' h t0 y0)

main :: IO ()
main = M.defaultMain
        [ M.bench "\n\nBeginning Haskell Benchmark..."  $ M.nf benchmarkRK 10000000 ]
