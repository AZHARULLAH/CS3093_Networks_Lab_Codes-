BEGIN	{
	count = 0
	dropped = 0
}

{
	if($1 == "d" && $3 == 1)
	{
		dropped = dropped + 1
	}
	if($1=="r" && $3 == 1 && $4 == 2)
	{
		count = count + 1
	}
}

END	{
	printf("Dropped is %d\n", dropped)
	printf("Count is %d\n", count)
	per = (dropped / count) * 100
	printf("Percentage dropped is %f percent\n", per)
}
