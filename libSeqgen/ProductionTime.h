#ifndef PRODUCTION_TIME_H_
#define PRODUCTION_TIME_H_

/**
 *  Compute the total production time i.e. the total bending time
 *  Total production time = time to setup + (num of parts * bending time)
 */
double computeTotalTime(
    unsigned nParts,
    unsigned nTools,
    unsigned nBends,
    unsigned nFlips,
    unsigned nRotations,
    double totalDistance,
    double bendingForce
);

/**
 *  Computes the time to setup the machine
 *  Total bending machine setup time = (the number of tools * time to remove tools for the previous part) + 
 *  (the number of tools * time to setup the tools for the current part) + time to configure the machine
 */
double machineSetUpTime(unsigned nTools);

/**
 *  Computes the time to bend the whole part
 *  Time to bend the part = (number of bends * time to perform a bend) + 
 * (linear distance * time for linear movement) + (angular movement * time to rotate) + (number of flips * time to flip)
 */
double copmputeTotalBendingTime(
    unsigned nBends,
    unsigned nFlips,
    unsigned nRotations,
    double totalDistance
);

/**
 *  Computes the total bending distance
 */
double computeLinearDistance(double totalDistance);

/**
 *  Compute rotational distance
 */
double computeRotationalDistance(unsigned nRotations);

/**
 *  Compute the time to perfom a bend
 *  Time to perform a bend = Approaching time + forming time + returning time
 */
double computeTimeToFormBend(double bendingForce);
/**
 *  Return the length of a stroke depending on the bending force required
 */
double lengthOfStroke(double bendingForce);

/**
 *  Return the Axis speed of a stroke depending on the bending force required
 */
double axisSpeed(double bendingForce);

/**
 *  Return the approaching speed of a stroke depending on the bending force required
 */
double approachingSpeed(double bendingForce);

/**
 *  Return the forming speed of a stroke depending on the bending force required
 */
double formingSpeed(double bendingForce);

/**
 *  Return the return speed of a stroke depending on the bending force required
 */
double returningSpeed(double bendingForce);

#endif