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
  stat = oqsMboDestroy(&decayOperator);
  ASSERT_EQ(OQS_SUCCESS, stat);
}
