BEGIN {
        count = 0
        rttime = 0
        maxrtt = 0
        minrtt = 100
}

{
        if($1=="+" && $3==0 && $5=="tcp")
        {
                rtt[$11] = $2
                size = size + $6
        }
        else if($1=="r" && $4==0 && $5=="ack")
        {
                rttime = rttime + $2 - rtt[$11]
                printf("%d %f \n",$11, ($2 - rtt[$11]))
                rttdiff = $2 - rtt[$11]
                if(rttdiff > maxrtt)
                {
                	maxrtt = rttdiff
                }
                if(rttdiff < minrtt)
                {
                	minrtt = rttdiff
                }
                count = count + 1
        }
}

END     {
        average = rttime/count
        printf("Average RTT = %f\n",average)
        printf("Max RTT = %f\n",maxrtt)
        printf("Min RTT = %f\n",minrtt)
}

