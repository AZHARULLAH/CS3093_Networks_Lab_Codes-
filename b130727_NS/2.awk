BEGIN	{
	maxsize = 0
	minsize = 100
}

{
	if($5 == "tcp")
	{
		size = $6;
		printf("%s   %d\n", $5, size)
		if(size > maxsize)
		{
			maxsize = size
		}
		if(size < minsize)
		{
			minsize = size
		}
	}
}

END	{
	printf("Maximum size is %d\n", maxsize)
	printf("Minimum size is %d\n", minsize)
}
