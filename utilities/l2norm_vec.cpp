#include "utilities.h"

//**************************************************************
// Compute l2-norm of vector 
//***********************************************************
float l2norm_vec(std::vector<float> const& u) {
    float accum = 0;
    for (int i = 0; i < u.size(); ++i) {
        accum += u[i] * u[i];
    }
    return sqrt(accum);
}


