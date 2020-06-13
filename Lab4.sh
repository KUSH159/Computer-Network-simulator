#!/bin/bash

./waf --run scratch/Part1
./waf --run scratch/Part2

echo "To plot graphs, you need to have gnuplot installed.\n"
echo "Installing gnuplot...(if not already installed)"
sudo apt-get install gnuplot
gnuplot scratch/plot.p
