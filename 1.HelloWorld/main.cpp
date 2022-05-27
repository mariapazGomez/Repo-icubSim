#include <cstdio>
#include <yarp/os/SystemClock.h>

using namespace std;
using namespace yarp::os;


int main()
{
    printf("Starting the application\n");
    int times=10;

    while(times--)
    {
        printf("Hello iCub\n");
        SystemClock::delaySystem(0.5); //wait 0.5 seconds
    }
    printf("Goodbye!\n");
}



