#ifndef _RND_H_
#define _RND_H_

#include <random>
#include <limits>
#include <chrono>

template < typename T >
class Random
{
    private:

        std::default_random_engine device;
        std::uniform_real_distribution<double> dist;

    public:

        Random( T low = std::numeric_limits<T>::min() , T high = std::numeric_limits<T>::max() )
            : dist{ (double) low , (double) high }
        {
            device.seed( std::chrono::system_clock::now().time_since_epoch().count() );
        }

        operator T (void) 
        {
            return (T) dist(device);
        }
};


#endif // _RND_H_