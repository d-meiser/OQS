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
#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include <stdlib.h>
#include <OqsAmplitude.h>

#ifdef __cplusplus
extern "C" {
#endif

struct Integrator;
typedef void (*RHS)(double, const struct OqsAmplitude *, struct OqsAmplitude *,
		 void *);

struct IntegratorOps {
	void (*create)(struct Integrator *self, size_t dim);
	void (*takeStep)(struct Integrator *self, struct OqsAmplitude *x, RHS f,
			 void *ctx);
	void (*advanceBeyond)(struct Integrator *self, double t,
			      struct OqsAmplitude *x, RHS f, void *ctx);
	void (*advanceTo)(struct Integrator *self, double t,
			  struct OqsAmplitude *x, RHS f, void *ctx);
	void (*destroy)(struct Integrator *self);
};

struct Integrator {
	struct IntegratorOps ops;
	double t;
	double dt;
	size_t dim;
	void *data;
};

void integratorCreate(struct Integrator* integrator, size_t dim);
void integratorDestroy(struct Integrator* integrator);
void integratorSetTime(struct Integrator* integrator, double t);
double integratorGetTime(struct Integrator* integrator);
void integratorTimeStepHint(struct Integrator* integrator, double dt);
void integratorTakeStep(struct Integrator *integrator, struct OqsAmplitude *x,
			RHS f, void *ctx);
void integratorAdvanceBeyond(struct Integrator *integrator, double t,
			     struct OqsAmplitude *x, RHS f, void *ctx);
void integratorAdvanceTo(struct Integrator *integrator, double t,
			 struct OqsAmplitude *x, RHS f, void *ctx);

#ifdef __cplusplus
}
#endif

#endif

