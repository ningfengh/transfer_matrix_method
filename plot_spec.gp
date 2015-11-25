set terminal postscript enhanced color font 'Helvetica,18'
set output 'spectra.eps'
set title "Reflection and Transmission Spectra"
set xlabel "Wavelength (nm)"
set ylabel "R/T"
plot "myout_p.txt" using 1:2 title 'reflection' with lines, \
"myout_p.txt" using 1:3  title 'transmission'with lines



