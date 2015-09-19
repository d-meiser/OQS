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
#include <cmath>
#include <Integrator.h>

TEST(Integrator, Create) {
  struct Integrator integrator;
  integratorCreate(&integrator, 2);
  integratorDestroy(&integrator);
}

TEST(Integrator, SetTime) {
  struct Integrator integrator;
  integratorCreate(&integrator, 2);
  integratorSetTime(&integrator, 3.5);
  double t = integratorGetTime(&integrator);
  EXPECT_FLOAT_EQ(3.5, t);
  integratorDestroy(&integrator);
}

TEST(Integrator, TimeStepHint) {
  struct Integrator integrator;
  integratorCreate(&integrator, 2);
  integratorTimeStepHint(&integrator, 1.0e-6);
  integratorDestroy(&integrator);
}

struct DecayCtx {
  double gamma;
};
void exponentialDecay(double t, const struct OqsAmplitude* x,
                      struct OqsAmplitude* y, void* ctx) {
  struct DecayCtx* decCtx = (struct DecayCtx*)ctx;
  y[0].re = -decCtx->gamma * x[0].re;
  y[0].im = -decCtx->gamma * x[0].im;
}

TEST(Integrator, TakeStep) {
  struct Integrator integrator;
  integratorCreate(&integrator, 1);
  double dt = 1.0e-3;
  integratorTimeStepHint(&integrator, dt);
  struct OqsAmplitude x;
  x.re = 1.0;
  x.im = 0.0;
  struct DecayCtx ctx;
  ctx.gamma = 1.0;
  integratorTakeStep(&integrator, &x, &exponentialDecay, &ctx);
  EXPECT_FLOAT_EQ(exp(-ctx.gamma * dt), x.re);
  EXPECT_FLOAT_EQ(0, x.im);
  integratorDestroy(&integrator);
}

TEST(Integrator, AdvanceBeyond) {
  struct Integrator integrator;
  integratorCreate(&integrator, 1);
  double dt = 1.0e-3;
  integratorTimeStepHint(&integrator, dt);
  struct OqsAmplitude x;
  x.re = 1.0;
  x.im = 0.0;
  struct DecayCtx ctx;
  ctx.gamma = 1.0;
  integratorAdvanceBeyond(&integrator, 0.3, &x, &exponentialDecay, &ctx);
  double finalTime = integratorGetTime(&integrator);
  EXPECT_LE(0.3, finalTime);
  EXPECT_FLOAT_EQ(exp(-finalTime * ctx.gamma), x.re);
  integratorDestroy(&integrator);
}

TEST(Integrator, AdvanceTo) {
  struct Integrator integrator;
  integratorCreate(&integrator, 1);
  double dt = 1.0e-3;
  integratorTimeStepHint(&integrator, dt);
  struct OqsAmplitude x;
  x.re = 1.0;
  x.im = 0.0;
  struct DecayCtx ctx;
  ctx.gamma = 1.0;
  double targetTime = 0.33458;
  integratorAdvanceTo(&integrator, targetTime, &x, &exponentialDecay, &ctx);
  double finalTime = integratorGetTime(&integrator);
  EXPECT_FLOAT_EQ(targetTime, finalTime);
  EXPECT_FLOAT_EQ(exp(-finalTime * ctx.gamma), x.re);
  integratorDestroy(&integrator);
}

