#include <OqsJumpTrajectory.h>
#include <stdlib.h>
#include <string.h>
#include <OqsAmplitude.h>
#include <Integrator.h>

struct OqsJumpTrajectory_ {
	struct OqsAmplitude *state;
	size_t dim;
	struct OqsSchrodingerEqn *schrodingerEqn;
	double z;
	struct Integrator integrator;
	struct OqsAmplitude *lastState;
};

OQS_STATUS oqsJumpTrajectoryCreate(size_t dim, OqsJumpTrajectory *trajectory)
{
	*trajectory = (OqsJumpTrajectory)malloc(sizeof(**trajectory));
	if (*trajectory == 0) return OQS_OUT_OF_MEMORY;
	(*trajectory)->state =
	    (struct OqsAmplitude *)malloc(dim * sizeof(*(*trajectory)->state));
	if ((*trajectory)->state == 0) {
		free(*trajectory);
		*trajectory = 0;
		return OQS_OUT_OF_MEMORY;
	}
	(*trajectory)->lastState = (struct OqsAmplitude *)malloc(
	    dim * sizeof(*(*trajectory)->lastState));
	if ((*trajectory)->lastState == 0) {
		free(*trajectory);
		*trajectory = 0;
		return OQS_OUT_OF_MEMORY;
	}
	(*trajectory)->dim = dim;
	(*trajectory)->schrodingerEqn = 0;
	(*trajectory)->z = (double)rand() / RAND_MAX;
	integratorCreate(&(*trajectory)->integrator, dim);
	return OQS_SUCCESS;
}

OQS_STATUS oqsJumpTrajectoryDestroy(OqsJumpTrajectory *trajectory)
{
	if (*trajectory) {
		free((*trajectory)->state);
		free((*trajectory)->lastState);
	}
	integratorDestroy(&(*trajectory)->integrator);
	free(*trajectory);
	*trajectory = 0;
	return OQS_SUCCESS;
}

OQS_STATUS
oqsJumpTrajectorySetSchrodingerEqn(OqsJumpTrajectory trajectory,
				   struct OqsSchrodingerEqn *eqn)
{
  trajectory->schrodingerEqn = eqn;
  return OQS_SUCCESS;
}

OQS_STATUS
oqsJumpTrajectorySetState(OqsJumpTrajectory trajectory,
			  const struct OqsAmplitude *state)
{
	memcpy(trajectory->state, state, trajectory->dim * sizeof(*state));
	return OQS_SUCCESS;
}

struct OqsAmplitude *oqsJumpTrajectoryGetState(OqsJumpTrajectory trajectory)
{
	return trajectory->state;
}

double oqsJumpTrajectoryGetTime(OqsJumpTrajectory trajectory)
{
	return integratorGetTime(&trajectory->integrator);
}

static void copyArray(struct OqsAmplitude *out, struct OqsAmplitude *in,
		      int dim)
{
	int i;
	for (i = 0; i < dim; ++i) {
		out[i].re = in[i].re;
		out[i].im = in[i].im;
	}
}

static double normSquared(int dim, struct OqsAmplitude* x)
{
	double nrm = 0.0;
	for (int i = 0; i < dim; ++i) {
		nrm += x[i].re * x[i].re + x[i].im * x[i].im;
	}
	return nrm;
}
static int decayHappened(OqsJumpTrajectory trajectory)
{
	return normSquared(trajectory->dim, trajectory->state) < trajectory->z;
}

int oqsJumpTrajectoryAdvance(OqsJumpTrajectory trajectory, double t)
{
	double currentTime;
	int decayed = 0;
	while (1) {
		currentTime = integratorGetTime(&trajectory->integrator);
		if (currentTime > t) break;
		decayed = decayHappened(trajectory);
		if (decayed) break;
		copyArray(trajectory->lastState, trajectory->state,
			  trajectory->dim);
		integratorTakeStep(&trajectory->integrator, trajectory->state,
				   trajectory->schrodingerEqn->RHS,
				   trajectory->schrodingerEqn->ctx);
	}
	if (currentTime > t) {
		//findFinalTime
	}
	decayed = decayHappened(trajectory);
	if (decayed) {
		//findDecayTime
	}
	return decayed;
}

double oqsJumpTrajectoryGetNextDecayNorm(OqsJumpTrajectory trajectory)
{
	return trajectory->z;
}
