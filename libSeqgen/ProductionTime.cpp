#include "ProductionTime.h"

#ifndef PRODUCTION_TIME_H_
#define PRODUCTION_TIME_H_

const double clampingTime = 30;
const double unClampingTime = 40;
const double configurationTime = 120;

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
    double totalDistance,
    double bendingForce
)
{
    double timeToPerformBend = computeTimeToFormBend(bendingForce);
}

double computeLinearDistance(double bendingForce, double totalDistance)
{
    double axisSpd = axisSpeed(bendingForce);

    return (totalDistance / axisSpd); // FIX 
}

double computeRotationalDistance(double bendingForce, unsigned nRotations)
{
    double axisSpd = axisSpeed(bendingForce);

    double rotate = nRotations * (M_PI / 2);

    return (rotate / axisSpd); // FIX
}

double computeTimeToFormBend(double bendingForce)
{
    double approachingTime = approachingSpeed(bendingForce);
    double formingTime = formingSpeed(bendingForce);
    double returningTime = returningSpeed(bendingForce);

    return (approachingTime + formingTime + returningTime);
}

double lengthOfStroke(double bendingForce)
{
    double lengthOfStroke;

    if (bendingForce > 0 && bendingForce <= 2300)
    {
        lengthOfStroke = 280;
    } else (bendingForce > 2300)
    {
        lengthOfStroke = 385;
    }

    return lengthOfStroke;
}

double axisSpeed(double bendingForce)
{
    double speed;

    if (bendingForce > 0 && bendingForce <= 1800)
    {
        speed = 800;
    } else if (bendingForce > 1800 && bendingForce <= 3700)
    {
        speed = 400;
    } else if (bendingForce > 3700)
    {
        speed = 300;
    }

    return speed;
}

double approachingSpeed(double bendingForce)
{
    double speed;

    if (bendingForce <= 600)
    {
        speed = 200;
    } else if (bendingForce > 600 && bendingForce <= 900)
    {
        speed = 180;
    } else if (bendingForce > 900 && bendingForce <= 1300)
    {
        speed = 160;
    } else if (bendingForce > 1300 && bendingForce <= 2300)
    {
        speed = 120;
    } else if (bendingForce > 2300 && bendingForce <= 3700)
    {
        speed = 100;
    } else if (bendingForce > 3700)
    {
        speed = 90;
    }

    return speed;   
}

double formingSpeed(double bendingForce)
{
    double speed;

    if (bendingForce > 0 && bendingForce <= 2300)
    {
        speed = 10;
    } else if (bendingForce > 2300 && bendingForce <= 3100)
    {
        speed = 9;
    } else if (bendingForce > 3100)
    {
        speed = 8;
    } 

    return speed;    
}

double returningSpeed(double bendingForce)
{
    double speed;

    if (bendingForce <= 600)
    {
        speed = 110;
    } else if (bendingForce > 600 && bendingForce <= 900)
    {
        speed = 120;
    } else if (bendingForce > 900 && bendingForce <= 2300)
    {
        speed = 120;
    } else if (bendingForce > 2300 && bendingForce <= 3700)
    {
        speed = 80;
    } else if (bendingForce > 3700)
    {
        speed = 70;
    }  

    return speed;   
}

#endif