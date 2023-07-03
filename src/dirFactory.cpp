#include "dirFactory.h"
#include "common/compass.h"
#include "random.h"

namespace BS {
    
    Dir DirFactory::random8()
    {
        return Dir(Compass::N).rotate(randomUint(0, 7)); 
    }


} // namespace 
