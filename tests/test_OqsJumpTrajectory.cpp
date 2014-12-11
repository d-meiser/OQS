#include <gtest/gtest.h>
#include <OqsJumpTrajectory.h>
#include <cmath>

static double normSquared(const struct OqsAmplitude* a) {
  return a->re * a->re + a->im * a->im;
}

static double vecNormSquared(const struct OqsAmplitude* v, int n) {
  double result = 0;
  for (int i = 0; i < n; ++i) {
    result += normSquared(v + i);
  }
  return result;
}

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

void RabiOscillationsRHS(double t, const struct OqsAmplitude* x,
                         struct OqsAmplitude* y, void* ctx) {
  double omega = *(double*)ctx;
  y[0].re = 0.5 * omega * x[1].im;
  y[0].im = -0.5 * omega * x[1].re;
  y[1].re = 0.5 * omega * x[0].im;
  y[1].im = -0.5 * omega * x[0].re;
}

class RabiOscillations : public JumpTrajectory {
 public:
  struct OqsSchrodingerEqn eqn;
  std::vector<OqsAmplitude> initialState;
  double omega;
  void SetUp() {
    JumpTrajectory::SetUp();
    omega = 1.0;
    eqn.RHS = &RabiOscillationsRHS;
    eqn.ctx = (void*)&omega;
    oqsJumpTrajectorySetSchrodingerEqn(trajectory, &eqn);
    initialState.resize(2);
    initialState[0].re = 1;
    initialState[0].im = 0;
    initialState[1].re = 0;
    initialState[1].im = 0;
    oqsJumpTrajectorySetState(trajectory, &initialState[0]);
  }
  void TearDown() { JumpTrajectory::TearDown(); }
};

TEST_F(RabiOscillations, Advance) {
  double tstart = oqsJumpTrajectoryGetTime(trajectory);
  oqsJumpTrajectoryAdvance(trajectory, 1.0e-4);
  double tend = oqsJumpTrajectoryGetTime(trajectory);
  EXPECT_GT(tend, tstart);
}

TEST_F(RabiOscillations, NoDecay) {
  int decayOccurred = oqsJumpTrajectoryAdvance(trajectory, 1.0e-4);
  EXPECT_EQ(0, decayOccurred);
}

TEST_F(RabiOscillations, PhaseProperties) {
  oqsJumpTrajectoryAdvance(trajectory, 0.1);
  struct OqsAmplitude* finalState = oqsJumpTrajectoryGetState(trajectory);
  EXPECT_FLOAT_EQ(0, finalState[0].im);
  EXPECT_FLOAT_EQ(0, finalState[1].re);
}

TEST_F(RabiOscillations, NormConservation) {
  oqsJumpTrajectoryAdvance(trajectory, 0.9);
  struct OqsAmplitude* finalState = oqsJumpTrajectoryGetState(trajectory);
  EXPECT_FLOAT_EQ(1, vecNormSquared(finalState, 2));
}

TEST_F(RabiOscillations, PopulationOscillations) {
  double t = 0.3;
  oqsJumpTrajectoryAdvance(trajectory, t);
  struct OqsAmplitude* finalState = oqsJumpTrajectoryGetState(trajectory);
  double c = cos(0.5 * omega * t);
  EXPECT_FLOAT_EQ(c * c, normSquared(finalState + 0));
}

void ExcitedStateDecayRHS(double t, const struct OqsAmplitude* x,
                         struct OqsAmplitude* y, void* ctx) {
  double gamma = *(double*)ctx;
  y[1].re = -0.5 * gamma * x[1].im;
  y[1].im = -0.5 * gamma * x[1].re;
}

class ExcitedStateDecay : public JumpTrajectory {
 public:
  struct OqsSchrodingerEqn eqn;
  std::vector<OqsAmplitude> initialState;
  double gamma;
  void SetUp() {
    JumpTrajectory::SetUp();
    gamma = 1.0;
    eqn.RHS = &ExcitedStateDecayRHS;
    eqn.ctx = (void*)&gamma;
    oqsJumpTrajectorySetSchrodingerEqn(trajectory, &eqn);
    initialState.resize(2);
    initialState[0].re = 1;
    initialState[0].im = 0;
    initialState[1].re = 0;
    initialState[1].im = 0;
    oqsJumpTrajectorySetState(trajectory, &initialState[0]);
  }
  void TearDown() { JumpTrajectory::TearDown(); }
};

TEST_F(ExcitedStateDecay, NextDecayNorm) {
  double z = oqsJumpTrajectoryGetNextDecayNorm(trajectory);
  ASSERT_GE(z, 0);
  ASSERT_LE(z, 1);
}

