#
set title "Enrolments in Data Structures and Computer Networks"
set xlabel "Year"
set ylabel "Enrolments"
#
set grid
set yrange [0:]
#
plot	"enrolments-datastructures" title "Data Structures" with linespoints, \
	"enrolments-networks"	title "Computer Networks" with linespoints
#
pause -1
