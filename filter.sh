
flags=""
noisenames="0010006 07002027 07042249 07069064 07075028 sine50Hz30s sine441Hz30s"
signalnames="blues.00081 classical.00099 country.00047 disco.00060 hiphop.00061 jazz.00021 metal.00071 pop.00041 reggae.00052 rock.00084"
noisedir="/home/ross/PycharmProjects/noise-guy/noise-sources"
sigdir="/home/ross/PycharmProjects/noise-guy/output/upsampled"
dirtydir="/home/ross/PycharmProjects/noise-guy/output/noisy"
cleandir="/home/ross/PycharmProjects/noise-guy/output/clean"
outputcsv="output.csv"

for sig in $signalnames
do
	for noise in $noisenames
	do
		./SignalCleaner/signal-cleaner $flags -o "$cleandir/$sig-$noise-f.wav" -f "$noisedir/$noise.wav" "$dirtydir/$sig-$noise.wav"
		./SignalCleaner/signal-cleaner $flags -o "$cleandir/$sig-$noise-d.wav" -d "$noisedir/$noise.wav" "$dirtydir/$sig-$noise.wav"
		echo "[Dirty signal '$sig-$noise']"
	        ./correlate/wav-correlate "$sigdir/$sig.wav" "$dirtydir/$sig-$noise.wav" "$outputcsv" "dirty"
		echo "['$sig-$noise' filtered by FIR]"
		./correlate/wav-correlate "$sigdir/$sig.wav" "$cleandir/$sig-$noise-f.wav" "$outputcsv" "fir"
		echo "['$sig-$noise' filtered by DNF]"
		./correlate/wav-correlate "$sigdir/$sig.wav" "$cleandir/$sig-$noise-d.wav" "$outputcsv" "dnf"
	done
done


