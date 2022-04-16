
flags=
noisenames=
signalnames=
noisedir=
sigdir=
dirtydir=
cleandir=

for sig in $signalnames
do
	for noise in $noisenames
	do
		./signal-cleaner $flags -o "$cleandir/$sig-$noise-f.wav" -f "$noisedir/$noise.wav" "$dirtydir/$sig-$noise.wav"
		./signal-cleaner $flags -o "$cleandir/$sig-$noise-d.wav" -d "$noisedir/$noise.wav" "$dirtydir/$sig-$noise.wav"
		echo "[Dirty signal '$sig-$noise']"
	        ./wav-correlate "$sigdir/$sig.wav" "$dirtydir/$sig-$noise.wav"
		echo "['$sig-$noise' filtered by FIR]"
		./wav-correlate "$sigdir/$sig.wav" "$cleandir/$sig-$noise-f.wav"
		echo "['$sig-$noise' filtered by DNF]"
		./wav-correlate "$sigdir/$sig.wav" "$cleandir/$sig-$noise-d.wav"
	done
done


