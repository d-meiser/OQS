#include <gtest/gtest.h>
#include <OqsJumpTrajectory.h>

class JumpTrajectory : public ::testing::Test {
  public:
    OqsJumpTrajectory trajectory;
    void SetUp() {
      OQS_STATUS stat = oqsJumpTrajectoryCreate(&trajectory);
      ASSERT_EQ(OQS_SUCCESS, stat);
    }
    void TearDown() {
      oqsJumpTrajectoryDestroy(&trajectory);
    }
};

TEST_F(JumpTrajectory, Create) {
  EXPECT_TRUE(0 != trajectory);
}

TEST_F(JumpTrajectory, SetSchrodingerEqn) {
  struct OqsSchrodingerEqn eqn;
  eqn.RHS = 0;
  eqn.ctx = 0;
  OQS_STATUS stat = oqsJumpTrajectorySetSchrodingerEqn(trajectory, &eqn);
  ASSERT_EQ(OQS_SUCCESS, stat);
}

