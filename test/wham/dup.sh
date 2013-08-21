#!/bin/csh

foreach i (0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15)
# Traditional WHAM
#  awk '{printf "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"}' data.u$i > data.v$i
# WHAM Extrapolation
  awk '{printf "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n"}' data.u$i > data.x$i
end
