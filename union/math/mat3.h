#pragma once
#include <string.h>
#include "vec4.h"

namespace base {

template<class treal>
class mat3
{
public:
    treal data[9];

    inline explicit mat3()
    {
        memset(data, 0, 9 * sizeof(treal));
    }

    inline explicit mat3(treal *source)
    {
        memcpy(data, source, 9 * sizeof(treal));
    }

    /**
     * Direct access
     */
    operator const treal*()const
    {
        return &data[0];
    }
};

} // namespace base

typedef base::mat3<float> mat3;
typedef base::mat3<double> mat3d;
