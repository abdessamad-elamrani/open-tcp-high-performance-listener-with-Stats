# open-tcp-high-performance-listener-with-Stats
this is a C code for high throughput TCP Listner, can be used as syslog listner, DB listner, or any other testing where a high throughput TCP peer is required to receive and ack data at high speed, it includes also live stats every 5 sec about the data  rate


# to run

gcc open-tcp.c -o open-tcp
sudo ./open-tcp

# example of output :

2023-03-09 15:21:07 - Received : 627118.09 logs/sec, Data: 195660845.60 bytes/sec

2023-03-09 15:21:12 - Received : 625764.39 logs/sec, Data: 195238491.20 bytes/sec

2023-03-09 15:21:17 - Received : 628443.26 logs/sec, Data: 196074297.00 bytes/sec

2023-03-09 15:21:22 - Received : 625546.62 logs/sec, Data: 195170546.00 bytes/sec

2023-03-09 15:21:27 - Received : 626678.62 logs/sec, Data: 195523728.80 bytes/sec

