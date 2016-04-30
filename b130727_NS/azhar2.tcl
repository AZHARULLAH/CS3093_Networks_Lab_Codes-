set ns [new Simulator]
set nf [open out2.nam w]
$ns namtrace-all $nf
set nt [open out2.tr w]
$ns trace-all $nt

proc finish {} {
	global ns nf
	$ns flush-trace
	close $nf
	exec nam out2.nam &
	exit 0
}

set tcp0 [new Agent/TCP]
set tcp1 [new Agent/TCP]
set sink0 [new Agent/TCPSink]
set sink1 [new Agent/TCPSink]
set ftp0 [new Application/FTP]
set ftp1 [new Application/FTP]
$ftp0 set type_ FTP
$ftp1 set type_ FTP

set S [$ns node]
set R [$ns node]
set D [$ns node]

$ns duplex-link $S $R 1Mb 50ms DropTail
$ns duplex-link $R $D 100Kb 10ms DropTail
$ns queue-limit $S $R 20
$ns queue-limit $R $D 20

$ns attach-agent $S $tcp0
$ns attach-agent $R $tcp1
$ns attach-agent $R $sink0
$ns attach-agent $D $sink1
$ftp0 attach-agent $tcp0 
$ftp1 attach-agent $tcp1

$ns connect $tcp0 $sink0
$ns connect $tcp1 $sink1

$ns at 0.5 "$ftp0 start"
$ns at 0.5 "$ftp1 start"
$ns at 50.5 "$ftp0 stop"
$ns at 50.5 "$ftp1 stop"
$ns at 51.0 "finish"

$ns run
