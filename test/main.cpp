#include <gtest/gtest.h>
using namespace std;
using namespace testing;

TEST(test_suite_name, test_name)
{
    ASSERT_EQ(1, 1);
}

int main(int argc, char *argv[])
{
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}