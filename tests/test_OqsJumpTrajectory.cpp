#include <gtest/gtest.h>
#include <OqsJumpTrajectory.h>

TEST(JumpTrajectory, Create) {
  OqsJumpTrajectory trajectory;
  oqsJumpTrajectoryCreate(&trajectory);
  EXPECT_TRUE(0 != trajectory);
  oqsJumpTrajectoryDestroy(&trajectory);
}
