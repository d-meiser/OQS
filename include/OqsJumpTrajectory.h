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

#include <OqsErrors.h>

#ifdef __cplusplus
extern "C" {
#endif

struct OqsJumpTrajectory_;
typedef struct OqsJumpTrajectory_ *OqsJumpTrajectory;

OQS_STATUS oqsJumpTrajectoryCreate(OqsJumpTrajectory *trajectory);
OQS_STATUS oqsJumpTrajectoryDestroy(OqsJumpTrajectory *trajectory);

#ifdef __cplusplus
}
#endif
#endif
