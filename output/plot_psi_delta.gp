set terminal postscript enhanced color font 'Helvetica,18'
set output 'psi_delta.eps'
set title "Ellipsometry result (Psi)"
set xlabel "Wavelength (nm)"
set ylabel "Psi"
plot "psi_delta.txt" using 1:2 title 'Psi' with lines


set title "Ellipsometry result (Delta)"
set xlabel "Wavelength (nm)"
set ylabel "Delta"
plot "psi_delta.txt" using 1:3 title 'Delta' with lines

