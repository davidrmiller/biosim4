#include "dirFactory.h"
#include "common/include/compass.h"
#include "random.h"

namespace BS {
    
    Dir DirFactory::random8()
    {
        return Dir(Compass::N).rotate(randomUint(0, 7)); 
    }


} // namespace 
