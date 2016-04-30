set ns [new Simulator]
set nf [open out.nam w]
$ns namtrace-all $nf
set nt [open out.tr w]
$ns trace-all $nt

proc finish {} {
	global ns nf
	$ns flush-trace
	close $nf
	exec nam out.nam &
	exit 0
}

set tcp0 [new Agent/TCP]
set sink0 [new Agent/TCPSink]
set ftp0 [new Application/FTP]
$ftp0 set type_ FTP

set S [$ns node]
set D [$ns node]

$ns duplex-link $S $D 1Mb 50ms DropTail
$ns queue-limit $S $D 10

$ns attach-agent $S $tcp0
$ns attach-agent $D $sink0 
$ftp0 attach-agent $tcp0

$ns connect $tcp0 $sink0

$ns at 0.5 "$ftp0 start"
$ns at 50.5 "$ftp0 stop"
$ns at 51.0 "finish"

$ns run
