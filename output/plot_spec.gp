set terminal postscript enhanced color font 'Helvetica,18'
set output 'spectra.eps'
set title "Reflection and Transmission Spectra (p)"
set xlabel "Wavelength (nm)"
set ylabel "R/T"
plot "spec_p.txt" using 1:2 title 'reflection' with lines, \
"spec_p.txt" using 1:3  title 'transmission'with lines

set title "Reflection and Transmission Spectra(s)"
set xlabel "Wavelength (nm)"
set ylabel "R/T"
plot "spec_s.txt" using 1:2 title 'reflection' with lines, \
"spec_s.txt" using 1:3  title 'transmission'with lines



