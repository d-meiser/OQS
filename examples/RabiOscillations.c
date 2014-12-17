#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <Oqs.h>

struct RabiOscillationsCtx {
	int dim;
	double omega;
	double gamma;
};

static void RabiOscillationsRHS(double t, const struct OqsAmplitude *x,
				struct OqsAmplitude *y, void *ctx)
{
	struct RabiOscillationsCtx *c = (struct RabiOscillationsCtx*)ctx;
	y[0].re = 0.5 * c->omega * x[1].im;
	y[0].im = -0.5 * c->omega * x[1].re;
	y[1].re = 0.5 * c->omega * x[0].im - 0.5 * c->gamma * x[1].re;
	y[1].im = -0.5 * c->omega * x[0].re - 0.5 * c->gamma * x[1].im;
}

struct EToGCtx {
  int dim;
  double gamma;
};
void excitedToGroundDecay(const struct OqsAmplitude *x, struct OqsAmplitude *y,
			  void *ctx)
{
	struct EToGCtx *c = (struct EToGCtx *)ctx;
	for (int i = 0; i < c->dim; ++i) {
		y[i].re = 0;
		y[i].im = 0;
	}
	double sgamma = sqrt(c->gamma);
	y[0].re = sgamma * x[1].re;
	y[0].im = sgamma * x[1].im;
}

static void advanceTo(OqsJumpTrajectory trajectory, int numDecays,
		      struct OqsDecayOperator *decays, double targetTime)
{
	int decayHappened = 1;
	int decay;

	while (decayHappened) {
		decayHappened =
		    oqsJumpTrajectoryAdvance(trajectory, targetTime);
		if (decayHappened) {
			decay = oqsJumpTrajectoryGetDecay(trajectory, numDecays,
							  decays);
			oqsJumpTrajectoryApplyDecay(trajectory, decays + decay);
		}
	}
}

static double excitedStatePopulation(OqsJumpTrajectory trajectory)
{
	struct OqsAmplitude* state;
	double pe, pg;
	state = oqsJumpTrajectoryGetState(trajectory);
	pe = state[1].re * state[1].re + state[1].im * state[1].im;
	pg = state[0].re * state[0].re + state[0].im * state[0].im;
	return pe / (pe + pg);
}

static double nrmSquared(OqsJumpTrajectory trajectory)
{
	struct OqsAmplitude *state = oqsJumpTrajectoryGetState(trajectory);
	double nrm = 0;
	int i;
	for (i = 0; i < 2; ++i) {
		nrm += state[i].re * state[i].re + state[i].im * state[i].im;
	}
	return nrm;
}

int main(int argn, char **argv)
{
	double tmax, t, dt;
	OQS_STATUS stat;
	OqsJumpTrajectory trajectory;
	struct OqsAmplitude initialState[2];
	int i;
	int numTrajectories = 10;

	stat = oqsJumpTrajectoryCreate(2, &trajectory);
	assert(stat == OQS_SUCCESS);

	initialState[0].re = 1.0;
	initialState[0].im = 0.0;
	initialState[1].re = 0.0;
	initialState[1].im = 0.0;

	struct OqsDecayOperator decay;
	decay.apply = &excitedToGroundDecay;
	struct EToGCtx decayCtx;
	decayCtx.dim = 2;
	decayCtx.gamma = 0.5;
	decay.ctx = &decayCtx;

	struct OqsSchrodingerEqn eqn;
	eqn.RHS = &RabiOscillationsRHS;
	struct RabiOscillationsCtx ctx;
	ctx.dim = 2;
	ctx.omega = 1.0;
	ctx.gamma = 0.5;
	eqn.ctx = &ctx;
	oqsJumpTrajectorySetSchrodingerEqn(trajectory, &eqn);

	tmax = 20.0;
	dt = 0.1;
	for (i = 0; i < numTrajectories; ++i) {
		t = 0;
		oqsJumpTrajectoryReset(trajectory, initialState, t);
		while (t < tmax) {
			printf("%d %lf %lf %lf\n", i, t,
			       excitedStatePopulation(trajectory),
			       nrmSquared(trajectory));
			advanceTo(trajectory, 1, &decay, t + dt);
			t = oqsJumpTrajectoryGetTime(trajectory);
		}
		printf("%d %lf %lf %lf\n", i, t,
		       excitedStatePopulation(trajectory),
		       nrmSquared(trajectory));
	}
	oqsJumpTrajectoryDestroy(&trajectory);
}
