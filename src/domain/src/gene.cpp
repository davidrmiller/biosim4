#include "../include/gene.h"

namespace BS
{
    
    float Gene::weightAsFloat() const 
    { 
        return weight / 8192.0; 
    }

} // namespace BS