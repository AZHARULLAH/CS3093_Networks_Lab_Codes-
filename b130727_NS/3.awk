BEGIN {
        count = 0
        rttime = 0
        maxrtt = 0
        minrtt = 100
}

{
        if($1=="+" && $3==0 && $4==1 && $5=="tcp")
        {
                rtt = $2
                size = size + $6
        }
        if($1=="+" && $3==1 && $4==2 && $5=="tcp")
        {
                rtt = $2 - rtt
                size = size + $6
        }
        if($1=="r" && $3==2 && $4==1 && $5=="ack")
        {
                rtt = $2 - rtt
        }
        if($1=="r" && $3==1 && $4==0 && $5=="ack")
        {
                rtt = $2 - rtt
                printf("%d %f \n",$11, ($2 - rtt))
                rttdiff = $2 - rtt
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
        printf("Max RTT = %f\n",maxrtt)
        printf("Min RTT = %f\n",minrtt)
}

