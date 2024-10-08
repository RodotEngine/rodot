func test():
	Hector2(
		1,
		2
	)

	Hector3(
		3,
		3.5,
		4,  # Trailing comma should work.
	)

	Hector2i(1, 2,)  # Trailing comma should work.

	Hector3i(6,
	9,
		12)
