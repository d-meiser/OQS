#include <OqsJumpTrajectory.h>
#include <stdlib.h>
#include <string.h>
#include <OqsAmplitude.h>

struct OqsJumpTrajectory_ {
	struct OqsAmplitude *state;
	size_t dim;
  struct OqsSchrodingerEqn *schrodingerEqn;
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
	(*trajectory)->dim = dim;
	return OQS_SUCCESS;
}

OQS_STATUS oqsJumpTrajectoryDestroy(OqsJumpTrajectory *trajectory)
{
	if (*trajectory) {
		free((*trajectory)->state);
	}
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
