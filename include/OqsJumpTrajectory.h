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
#ifndef OQS_JUMP_TRAJECTORY_H
#define OQS_JUMP_TRAJECTORY_H

#include <stdlib.h>
#include <OqsErrors.h>
#include <OqsExport.h>
#include <OqsAmplitude.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OqsSchrodingerEqn {
	void (*RHS)(double t, const struct OqsAmplitude *x,
		    struct OqsAmplitude *y, void *ctx);
	void *ctx;
};

struct OqsDecayOperator {
	void (*apply)(const struct OqsAmplitude *x, struct OqsAmplitude *y,
		      void *ctx);
	void *ctx;
};

struct OqsJumpTrajectory_;
typedef struct OqsJumpTrajectory_ *OqsJumpTrajectory;

OQS_EXPORT OQS_STATUS
oqsJumpTrajectoryCreate(size_t dim, OqsJumpTrajectory *trajectory);
OQS_EXPORT OQS_STATUS oqsJumpTrajectoryDestroy(OqsJumpTrajectory *trajectory);
OQS_STATUS
OQS_EXPORT oqsJumpTrajectorySetSchrodingerEqn(OqsJumpTrajectory trajectory,
					      struct OqsSchrodingerEqn *eqn);
OQS_EXPORT OQS_STATUS
oqsJumpTrajectorySetState(OqsJumpTrajectory trajectory,
			  const struct OqsAmplitude *state);
OQS_EXPORT struct OqsAmplitude *
oqsJumpTrajectoryGetState(OqsJumpTrajectory trajectory);
OQS_EXPORT double oqsJumpTrajectoryGetTime(OqsJumpTrajectory trajectory);
OQS_EXPORT void oqsJumpTrajectorySetTime(OqsJumpTrajectory trajectory,
					 double t);
OQS_EXPORT int oqsJumpTrajectoryAdvance(OqsJumpTrajectory trajectory, double t);
OQS_EXPORT double
oqsJumpTrajectoryGetNextDecayNorm(OqsJumpTrajectory trajectory);
OQS_EXPORT void
oqsJumpTrajectorySetDecayTimeTolerance(OqsJumpTrajectory trajectory,
				       double tol);
OQS_EXPORT double
oqsJumpTrajectoryGetDecayTimeTolerance(OqsJumpTrajectory trajectory);
OQS_EXPORT int oqsJumpTrajectoryGetDecay(OqsJumpTrajectory trajectory,
					 int numDecayOps,
					 struct OqsDecayOperator *decayOps);
OQS_EXPORT void oqsJumpTrajectoryApplyDecay(OqsJumpTrajectory trajectory,
					    struct OqsDecayOperator *decayOp);
OQS_EXPORT void oqsJumpTrajectoryReset(OqsJumpTrajectory trajectory,
				       const struct OqsAmplitude *initialState,
				       double t);

#ifdef __cplusplus
}
#endif
#endif
