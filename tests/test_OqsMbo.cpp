/*
Copyright 2014 Dominic Meiser

This file is part of oqs.

oqs is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 3 of the License, or (at your
option) any later version.

oqs is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License along
with oqs.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <gtest/gtest.h>
#include <OqsMbo.h>
#include <OqsJumpTrajectory.h>
#include <Mbo.h>

struct DecayOperator : public ::testing::Test {
  void SetUp() {
    MboProdSpace h = mboProdSpaceCreate(2);
    mboTensorOpNull(h, &op);
    mboProdSpaceDestroy(&h);
    MboElemOp sz = mboSigmaZ();
    mboTensorOpAddTo(sz, 0, op);
    mboElemOpDestroy(&sz);
  }
  void TearDown() {
    mboTensorOpDestroy(&op);
  }
  MboTensorOp op;
};

TEST_F(DecayOperator, CanBeConstructedFromMboOperator) {
  struct OqsDecayOperator decayOperator = {0};
  OQS_STATUS stat = oqsMboCreateDecayOperator(op, &decayOperator);
  ASSERT_EQ(OQS_SUCCESS, stat);
  EXPECT_TRUE(0 != decayOperator.ctx);
  stat = oqsMboDestroyDecayOperator(&decayOperator);
  ASSERT_EQ(OQS_SUCCESS, stat);
}

TEST_F(DecayOperator, CanBeApplied) {
  struct OqsDecayOperator decayOperator = {0};
  OQS_STATUS stat = oqsMboCreateDecayOperator(op, &decayOperator);
  struct OqsAmplitude x[2] = {{2.3, 1.7}, {5.2, -1.8}};
  struct OqsAmplitude y[2] = {{0}};
  decayOperator.apply(x, y, decayOperator.ctx);
  EXPECT_FLOAT_EQ(-x[0].re, y[0].re);
  EXPECT_FLOAT_EQ(-x[0].im, y[0].im);
  EXPECT_FLOAT_EQ(x[1].re, y[1].re);
  EXPECT_FLOAT_EQ(x[1].im, y[1].im);
  stat = oqsMboDestroyDecayOperator(&decayOperator);
}


typedef DecayOperator SchrodingerEqn;

TEST_F(SchrodingerEqn, CanBeConstructedFromMboOperator) {
  struct OqsSchrodingerEqn schEqn = {0};
  OQS_STATUS stat = oqsMboCreateSchrodingerEqn(op, &schEqn);
  ASSERT_EQ(OQS_SUCCESS, stat);
  EXPECT_TRUE(0 != schEqn.ctx);
  stat = oqsMboDestroySchrodingerEqn(&schEqn);
  ASSERT_EQ(OQS_SUCCESS, stat);
}

TEST_F(SchrodingerEqn, CanBeApplied) {
  struct OqsSchrodingerEqn schEqn = {0};
  OQS_STATUS stat = oqsMboCreateSchrodingerEqn(op, &schEqn);
  struct OqsAmplitude x[2] = {{2.3, 1.7}, {5.2, -1.8}};
  struct OqsAmplitude y[2] = {{0}};
  schEqn.RHS(0, x, y, schEqn.ctx);
  EXPECT_FLOAT_EQ(-x[0].im, y[0].re);
  EXPECT_FLOAT_EQ(x[0].re, y[0].im);
  EXPECT_FLOAT_EQ(x[1].im, y[1].re);
  EXPECT_FLOAT_EQ(-x[1].re, y[1].im);
  stat = oqsMboDestroySchrodingerEqn(&schEqn);
}
