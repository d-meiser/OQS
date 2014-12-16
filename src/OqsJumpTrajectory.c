#include <OqsJumpTrajectory.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <OqsAmplitude.h>
#include <Integrator.h>

struct OqsJumpTrajectory_ {
	struct OqsAmplitude *state;
	size_t dim;
	struct OqsSchrodingerEqn *schrodingerEqn;
	double z;
	struct Integrator integrator;
	struct OqsAmplitude *previousState;
	double previousTime;
	double decayTimeTolerance;
	double decayNormTolerance;
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
	(*trajectory)->previousState = (struct OqsAmplitude *)malloc(
	    dim * sizeof(*(*trajectory)->previousState));
	if ((*trajectory)->previousState == 0) {
		free(*trajectory);
		*trajectory = 0;
		return OQS_OUT_OF_MEMORY;
	}
	(*trajectory)->dim = dim;
	(*trajectory)->schrodingerEqn = 0;
	(*trajectory)->z = (double)rand() / RAND_MAX;
	integratorCreate(&(*trajectory)->integrator, dim);
	(*trajectory)->decayTimeTolerance = 1.0e-7;
	(*trajectory)->decayNormTolerance = 1.0e-12;
	return OQS_SUCCESS;
}

OQS_STATUS oqsJumpTrajectoryDestroy(OqsJumpTrajectory *trajectory)
{
	if (*trajectory) {
		free((*trajectory)->state);
		free((*trajectory)->previousState);
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

void oqsJumpTrajectorySetDecayTimeTolerance(OqsJumpTrajectory trajectory,
					    double tol)
{
	trajectory->decayTimeTolerance = tol;
}

double oqsJumpTrajectoryGetDecayTimeTolerance(OqsJumpTrajectory trajectory)
{
	return trajectory->decayTimeTolerance;
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

static void backTrack(OqsJumpTrajectory trajectory)
{
	copyArray(trajectory->state, trajectory->previousState,
		  trajectory->dim);
	integratorSetTime(&trajectory->integrator, trajectory->previousTime);
}

static void findDecayTime(OqsJumpTrajectory trajectory)
{
	double tGuess, tLeft, tRight, normGuess, normLeft, normRight;
	tLeft = trajectory->previousTime;
	tRight = integratorGetTime(&trajectory->integrator);
	assert(tRight > tLeft);

	while (tRight - tLeft > trajectory->decayTimeTolerance) {
		normLeft =
		    normSquared(trajectory->dim, trajectory->previousState);
		assert(normLeft >= trajectory->z);
		normRight = normSquared(trajectory->dim, trajectory->state);
		assert(normRight <= trajectory->z);
		// To find the decay time we assume that the square of the norm
		// decays exponentially during the integration time interval.
		// This is typically a better approximation than linear
		// variation (which would lead to the secant method).
		// Exponential decay is still monotonically decreasing, so we
		// maintain the bracketing property of the secand method.  To
		// find the new guess for the decay time based on the left and
		// right ends of the interval, we use that
		//      n(t) = nL exp(-gamma (t - tL))
		// where nL is the norm at the beginning of the interval, tL.
		// To find gamma we use that
		//      n(tR) = nL exp(-gamma * (tR - tL)) == nR
		//      => gamma = -log(nR / nL) / (tR - tL)
		// The guess for the decay time is then found by solving
		//      n(tGuess) == z
		//      => nL exp(log(nR /nL)(tGuess - tL) / (tR - tL)) == z
		//      => log(nR / nL)(tGuess - tL)/(tR - tL) = log (z / nL)
		//      => tGuess = tL + (tR - tL) * log (z / nL) / log(nR / nL)
		tGuess = tLeft +
			 (tRight - tLeft) * log(trajectory->z / normLeft) /
			     log(normRight / normLeft);

		backTrack(trajectory);
		integratorAdvanceTo(&trajectory->integrator, tGuess,
				    trajectory->state,
				    trajectory->schrodingerEqn->RHS,
				    trajectory->schrodingerEqn->ctx);
		normGuess = normSquared(trajectory->dim, trajectory->state);
		if (abs(normGuess - trajectory->z) <
		    trajectory->decayNormTolerance) {
			return;
		}
		if (normGuess > trajectory->z) {
			normLeft = normGuess;
			copyArray(trajectory->previousState, trajectory->state,
				  trajectory->dim);
			trajectory->previousTime =
			    integratorGetTime(&trajectory->integrator);
			tLeft = trajectory->previousTime;
		} else {
			normRight = normGuess;
			tRight = integratorGetTime(&trajectory->integrator);
		}
	}
}

int oqsJumpTrajectoryAdvance(OqsJumpTrajectory trajectory, double t)
{
	double currentTime;
	int decayed = 0;

	currentTime = integratorGetTime(&trajectory->integrator);
  if (currentTime > t) return decayed;

	while (1) {
		copyArray(trajectory->previousState, trajectory->state,
			  trajectory->dim);
		trajectory->previousTime = currentTime;
		integratorTakeStep(&trajectory->integrator, trajectory->state,
				   trajectory->schrodingerEqn->RHS,
				   trajectory->schrodingerEqn->ctx);
		decayed = decayHappened(trajectory);
		if (decayed) break;
		currentTime = integratorGetTime(&trajectory->integrator);
		if (currentTime > t) break;
	}
	if (currentTime > t) {
		backTrack(trajectory);
		integratorAdvanceTo(&trajectory->integrator, t,
				    trajectory->state,
				    trajectory->schrodingerEqn->RHS,
				    trajectory->schrodingerEqn->ctx);
	}
	decayed = decayHappened(trajectory);
	if (decayed) {
		findDecayTime(trajectory);
	}
	return decayed;
}

double oqsJumpTrajectoryGetNextDecayNorm(OqsJumpTrajectory trajectory)
{
	return trajectory->z;
}
