#!/bin/bash

flags=""
noisenames="0010006 07002027 07042249 07069064 07075028 sine50Hz30s sine441Hz30s"
signalnames="blues.00081 classical.00099 country.00047 disco.00060 hiphop.00061 jazz.00021 metal.00071 pop.00041 reggae.00052 rock.00084"
noisedir="/home/ross/PycharmProjects/noise-guy/noise-sources"
sigdir="/home/ross/PycharmProjects/noise-guy/output/upsampled"
dirtydir="/home/ross/PycharmProjects/noise-guy/output/noisy"
cleandir="/home/ross/PycharmProjects/noise-guy/output/clean"
outputcsv="output.csv"
gain=1.0
gain_step=0.1
zero=0.0
act=0


for sig in $signalnames
do
  for noise in $noisenames
  do
    while [ $act -lt 4 ]
    do
        echo "Running dnf with activation: $act"
        while [ "$(bc <<< "$gain > $zero")" == "1"  ];
        do
          echo "Input gain: $gain"
          ./SignalCleaner/signal-cleaner $flags -o "$cleandir/$sig-$noise-$gain.wav" "-g" $gain "-a" $act "-d" "$noisedir/$noise.wav" "$dirtydir/$sig-$noise.wav"
          gain=$(awk "BEGIN{ print $gain - $gain_step }")
        done
        act=$[$act+1]
        gain=1.0
     done
  done
done
