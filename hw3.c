// r is a 1 x 2 vector that contains an x and y value for position
// v is same as above except is it velocity value

//this will be called in a function for 2 balls


r12[1] = r1[1] - r2[1];
r12[2] = r1[2] - r2[2];

v12[1] = v1[1] - v2[1];
v12[2] = v1[2] - v2[2];

rDotV = r12[1]*v12[1] + r12[2] *v12[2];

//would use the equation below but too slow, can use known parameters
//magSqrd = r12[1]^2 + r12[2]^2;

//instead use:
magSqrd = (2*ballRadius)^2;

deltaV[1] = -r12[1]*rDotV / magSqrd;
deltaV[2] = -r12[2]*rDotV / magSqrd;