set terminal png size 1024,900

set output "TcpHybla-cwnd.png"
plot "TcpHybla-cwnd.data" using 1:2 title 'Congestion Window' with linespoints

set output "TcpWestWood-cwnd.png"
plot "TcpWestwood-cwnd.data" using 1:2 title 'Congestion Window' with linespoints

set output "TcpYeah-cwnd.png"
plot "TcpYeah-cwnd.data" using 1:2 title 'Congestion Window' with linespoints

set output "TcpHybla-throughput.png"
plot "TcpHybla-throughput.data" using 1:2 title 'Throughput' with linespoints

set output "TcpYeah-throughput.png"
plot "TcpYeah-throughput.data" using 1:2 title 'Throughput' with linespoints

set output "TcpWestwood-throughput.png"
plot "TcpWestwood-throughput.data" using 1:2 title 'Throughput' with linespoints

set output "TcpHybla-goodput.png"
plot "TcpHybla-goodput.data" using 1:2 title 'Goodput' with linespoints

set output "TcpWestwood-goodput.png"
plot "TcpWestwood-goodput.data" using 1:2 title 'Goodput' with linespoints

set output "TcpYeah-goodput.png"
plot "TcpYeah-goodput.data" using 1:2 title 'Goodput' with linespoints

set output "BusypHybla-cwnd.png"
plot "BusyTcpHybla-cwnd.data" using 1:2 title 'Congestion Window' with linespoints

set output "BusyTcpWestWoodPlus-cwnd.png"
plot "BusyTcpWestwood-cwnd.data" using 1:2 title 'Congestion Window' with linespoints

set output "BusyTcpYeah-cwnd.png"
plot "BusyTcpYeah-cwnd.data" using 1:2 title 'Congestion Window' with linespoints

set output "BusyTcpHybla-throughput.png"
plot "BusyTcpHybla-throughput.data" using 1:2 title 'Throughput' with linespoints

set output "BusyTcpWestWoodPlus-throughput.png"
plot "BusyTcpWestwood-throughput.data" using 1:2 title 'Throughput' with linespoints

set output "BusyTcpYeah-throughput.png"
plot "BusyTcpYeah-throughput.data" using 1:2 title 'Throughput' with linespoints

set output "BusyTcpHybla-goodput.png"
plot "BusyTcpHybla-goodput.data" using 1:2 title 'Goodput' with linespoints

set output "BusyTcpWestWoodPlus-goodput.png"
plot "BusyTcpWestwood-goodput.data" using 1:2 title 'Goodput' with linespoints

set output "BusyTcpYeah-goodput.png"
plot "BusyTcpYeah-goodput.data" using 1:2 title 'Goodputput' with linespoints

exit
