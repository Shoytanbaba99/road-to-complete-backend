echo "=== SINGLE DOWNLOAD ==="
time curl -o /dev/null -s -w "Time: %{time_total}s\n" http://speedtest.tele2.net/10MB.zip

echo ""
echo "=== PARALLEL DOWNLOADS (5 files) ==="
time (for i in {1..5}; do curl -o /dev/null -s -w "File $i: %{time_total}s\n" http://speedtest.tele2.net/10MB.zip & done; wait)

=== SINGLE DOWNLOAD ===
Time: 37.190685s

real 0m37.199s
user 0m0.024s
sys 0m0.052s

=== PARALLEL DOWNLOADS (5 files) ===
File 2: 33.871433s
File 5: 37.042173s
File 3: 37.193579s
File 1: 37.700656s
File 4: 37.805086s

real 0m37.817s
user 0m0.147s
sys 0m0.228s

So, there are 3 types of bottlenecks that can occur in a system: CPU, Memory, and I/O.

CPU is when it's running at full capacity and is the limiting factor. Memory is when the system is running out of memory and is the limiting factor. I/O is when the system is waiting for I/O operations to complete and is the limiting factor.

For IO you got, l1,l2,l3 cache, RAM, Disk, Network. The closer to the CPU the faster it is. All of those are part of I/O operations.

In the test above we see the parallel downloads take almost the same time as the single download. It was not the downloading speed (i.e., the network) that was the bottleneck, but the CPU. The CPU was running and writing on only 1 file when it could have been writing on 5 files at the same time. Since the CPU was sitting idle during the single download (waiting for network packets), parallelizing the downloads allowed us to utilize that idle CPU time to manage multiple I/O streams simultaneously, resulting in 5× the data transferred in roughly the same total time.
