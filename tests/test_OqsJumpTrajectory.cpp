#include <gtest/gtest.h>
#include <OqsJumpTrajectory.h>

class JumpTrajectory : public ::testing::Test {
  public:
    OqsJumpTrajectory trajectory;
    void SetUp() {
      OQS_STATUS stat = oqsJumpTrajectoryCreate(2, &trajectory);
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

TEST_F(JumpTrajectory, SetState) {
  struct OqsAmplitude state[2];
  state[0].re = 2.0;
  state[0].im = 0.2;
  state[1].re = 1.7;
  state[1].im = 2.4;
  OQS_STATUS stat = oqsJumpTrajectorySetState(trajectory, state);
  ASSERT_EQ(OQS_SUCCESS, stat);
  struct OqsAmplitude* s = oqsJumpTrajectoryGetState(trajectory);
  EXPECT_FLOAT_EQ(state[0].re, s[0].re);
  EXPECT_FLOAT_EQ(state[1].im, s[1].im);
}

TEST_F(JumpTrajectory, GetTime) {
  double t = oqsJumpTrajectoryGetTime(trajectory);
  EXPECT_FLOAT_EQ(0, t);
}

TEST_F(JumpTrajectory, Advance) {
  double tstart = oqsJumpTrajectoryGetTime(trajectory);
  int decayOccurred = oqsJumpTrajectoryAdvance(trajectory, 1.0e-4);
  double tend = oqsJumpTrajectoryGetTime(trajectory);
  EXPECT_GT(tend, tstart);
}
