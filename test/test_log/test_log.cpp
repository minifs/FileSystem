#include <gtest/gtest.h>
#include "log.h"

TEST(create_block, errorcode)
{
	LOG_DEBUG("This can print debug messages\n");
	LOG_INFO("This can print some informations, include numbers %d\n", 10);
	LOG_WARN("Warning message is available as well, %s\n", "warning !!");
	LOG_ERROR("Of course, we can print error code\n");
	LOG_FATAL("Fatal message can be used as well\n");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}
