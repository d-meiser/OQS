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
#include <OqsMbo.h>
#include <MboNumOp.h>
#include <MboAmplitude.h>

struct OqsMboOperator {
	MboNumOp op;
};

static void oqsMboApply(const struct OqsAmplitude *x, struct OqsAmplitude *y,
			void *ctx)
{
	static const struct MboAmplitude alpha = {1, 0};
	static const struct MboAmplitude beta = {0};
	struct OqsMboOperator *opCtx = ctx;
	mboNumOpMatVec(alpha, opCtx->op, (struct MboAmplitude *)x, beta,
		       (struct MboAmplitude *)y);
}

OQS_STATUS oqsMboCreateDecayOperator(MboTensorOp op,
				     struct OqsDecayOperator *dop)
{
	struct OqsMboOperator *opCtx = malloc(sizeof(opCtx));
	if (!opCtx) return OQS_OUT_OF_MEMORY;
	mboNumOpCompile(op, &opCtx->op);
	dop->ctx = opCtx;
	dop->apply = oqsMboApply;
	return OQS_SUCCESS;
}

OQS_STATUS oqsMboDestroy(struct OqsDecayOperator *dop)
{
	struct OqsMboOperator *opCtx = dop->ctx;
	mboNumOpDestroy(&opCtx->op);
	free(opCtx);
	return OQS_SUCCESS;
}
