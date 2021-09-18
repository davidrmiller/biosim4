#!/usr/bin/gnuplot --persist

# Requires a text file named "epoch-log.txt" in the log directory

set term png size 2000, 400
set output "/home/dm/sw/biosim4/images/log.png"

# Left Y axis gets scaled to the max survivors.
# Right Y axis gets scaled to 0..255.
#   1:2 Survivors 0..N        => 0..N
#   1:3 Genome length 0..50   => 0..255
#   1:4 Diversity 0..0.7      => 0..255
#   1:5 Anxiety 0..255        => 0..255

set mxtics
set ytics autofreq nomirror tc lt 2
set y2range [ 0:256 ]
set y2tics autofreq nomirror tc lt 1
set grid
set key lmargin

ScaleGenomeLength(y)= y*2
ScaleDiversity(d)= 350*d

plot "/home/dm/sw/biosim4/logs/epoch-log.txt" using 1:2 with lines lw 1 linecolor 2 title "Survivors", \
    "" using 1:(ScaleDiversity($3)) with lines lw 1 linecolor 1 title "Diversity" axes x1y2, \
    "" using 1:(ScaleGenomeLength($4)) with lines lw 1 linecolor 6 title "Genome Len" axes x1y2

