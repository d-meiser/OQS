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
#ifndef OQS_MBO_H
#define OQS_MBO_H

#include <OqsConfig.h>
#ifndef OQS_WITH_MBO
#error "This version of OQS has been configure without MBO support.\n" \
       "Please reconfigure with MBO support enabled.\n"
#endif

#include <OqsExport.h>
#include <OqsErrors.h>
#include <OqsJumpTrajectory.h>
#include <MboTensorOp.h>

#ifdef __cplusplus
extern "C" {
#endif

OQS_EXPORT OQS_STATUS
oqsMboCreateDecayOperator(MboTensorOp op, struct OqsDecayOperator *dop);
OQS_EXPORT OQS_STATUS oqsMboDestroyDecayOperator(struct OqsDecayOperator *dop);
OQS_EXPORT OQS_STATUS oqsMboCreateSchrodingerEqn(MboTensorOp hamiltonian,
						 struct OqsSchrodingerEqn *eqn);
OQS_EXPORT OQS_STATUS
oqsMboDestroySchrodingerEqn(struct OqsSchrodingerEqn *eqn);

#ifdef __cplusplus
}
#endif
#endif
