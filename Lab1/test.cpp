#include "pch.h"
using namespace trit_set;
TEST(testTritSet, MemoryTest)
{
	TritSet set(1000);
	size_t alloc_length = set.capacity();
	EXPECT_TRUE(alloc_length >= 1000 * 2 / 8 / sizeof(uint));
	set[1000000000] = Unknown;
	EXPECT_EQ(alloc_length, set.capacity());
	if (set[2000000000] == True) {}
	EXPECT_EQ(alloc_length, set.capacity());
	set[1000000000] = True;
	EXPECT_LT(alloc_length, set.capacity());
	alloc_length = set.capacity();
	set[1000000000] = Unknown;
	set[1000000] = False;
	EXPECT_EQ(alloc_length, set.capacity());
	set.shrink();
	EXPECT_GT(alloc_length, set.capacity());
	set.trim(100);
	EXPECT_EQ(100, set.capacity());
}

TEST(testTritSet, OperationsTest) {
	trit_set::TritSet setA(1000);
	trit_set::TritSet setB(2000);

	setA[1] = trit_set::True;
	setA[2] = trit_set::True;
	setA[3] = trit_set::True;
	setA[4] = trit_set::Unknown;
	setA[5] = trit_set::Unknown;
	setA[6] = trit_set::Unknown;
	setA[7] = trit_set::False;
	setA[8] = trit_set::False;
	setA[9] = trit_set::False;

	setB[1] = trit_set::True;
	setB[2] = trit_set::Unknown;
	setB[3] = trit_set::False;
	setB[4] = trit_set::True;
	setB[5] = trit_set::Unknown;
	setB[6] = trit_set::False;
	setB[7] = trit_set::True;
	setB[8] = trit_set::Unknown;
	setB[9] = trit_set::False;

	trit_set::TritSet setC = setA | setB;
	EXPECT_EQ(setC.capacity(), setB.capacity());
	EXPECT_EQ(setC[1], trit_set::True);
	EXPECT_EQ(setC[2], trit_set::True);
	EXPECT_EQ(setC[3], trit_set::True);
	EXPECT_EQ(setC[4], trit_set::True);
	EXPECT_EQ(setC[5], trit_set::Unknown);
	EXPECT_EQ(setC[6], trit_set::Unknown);
	EXPECT_EQ(setC[7], trit_set::True);
	EXPECT_EQ(setC[8], trit_set::Unknown);
	EXPECT_EQ(setC[9], trit_set::False);
	setC.trim(0);
	setC = setA & setB;
	EXPECT_EQ(setC.capacity(), setB.capacity());
	EXPECT_EQ(setC[1], trit_set::True);
	EXPECT_EQ(setC[2], trit_set::Unknown);
	EXPECT_EQ(setC[3], trit_set::False);
	EXPECT_EQ(setC[4], trit_set::Unknown);
	EXPECT_EQ(setC[5], trit_set::Unknown); //
	EXPECT_EQ(setC[6], trit_set::False);
	EXPECT_EQ(setC[7], trit_set::False);
	EXPECT_EQ(setC[8], trit_set::False);
	EXPECT_EQ(setC[9], trit_set::False);
	setC.trim(0);
	setC = ~setB;
	EXPECT_EQ(setC.capacity(), setB.capacity());
	EXPECT_EQ(setC[1], trit_set::False);
	EXPECT_EQ(setC[2], trit_set::Unknown);
	EXPECT_EQ(setC[3], trit_set::True);
}
TEST(testTritSet, FunctionTest) {
	trit_set::TritSet set(1000);
	for (int i = 0; i < 100; i++)
	{
		set[i] = trit_set::True;
	}
	size_t length = set.length();
	EXPECT_EQ(length, 100);
	size_t trits_number = set.cardinality(trit_set::True);
	EXPECT_EQ(trits_number, 100);
	set[500] = trit_set::False;
	std::unordered_map< trit_set::Trit, size_t, std::hash<int> > m = set.cardinality();
	EXPECT_EQ(m[trit_set::True], 100);
	EXPECT_EQ(m[trit_set::False], 1);
	EXPECT_EQ(m[trit_set::Unknown], 1000 - 100 - 1);
}