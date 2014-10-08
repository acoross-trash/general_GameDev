#include "gtest\gtest.h"

#pragma comment(lib, "gtest_maind.lib")
#pragma comment(lib, "gtestd.lib")

int Factorial(int num)
{
	if (num <= 0)
	{
		return 1;
	}
	else
	{
		return num * Factorial(num - 1);
	}
}

TEST(FactorialTest, Negative)
{
	EXPECT_EQ(1, Factorial(-5));
}

void main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);

	RUN_ALL_TESTS();

	system("pause");
}