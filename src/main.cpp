#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#include <thread>
#include <chrono>

//To get terminal dimensions (UNIX)
#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h> // for STDOUT_FILENO

using namespace std;
using namespace chrono;



float theta; //rads
int cubeCenter;
int K1 = 70;
int distanceToCamera = 100;
int offset;
int width, height;
string frameBuffer;
vector<float> zBuffer;
int returnCursorSize = string("\033[0;0H").size();


void calculateSurfacePoint(int cubeX, int cubeY, int cubeZ, char c)
{
    float costheta = cos(theta);
    float sintheta = sin(theta);
    float x = cubeX, y = cubeY, z = cubeZ;
    float newX, newY, newZ;

    //Rotate in X axis
    newY = y * costheta + z * sintheta;
    newZ = -y * sintheta + z * costheta;
    y = newY;
    z = newZ;

    //Rotate in Y axis
    newX = x * costheta + z * sintheta;
    newZ = x * -sintheta + z * costheta;
    x = newX;
    z = newZ;

    //Rotate in Z axis
    newX = x * costheta + y * -sintheta;
    newY = x * sintheta + y * costheta;
    x = newX;
    y = newY;

    x += cubeCenter;
    y += cubeCenter;
    z += cubeCenter;
    z +=distanceToCamera;

    int xp = K1*x/z + offset;
    int yp = K1*y/z + offset;
    int pointIndex = xp + yp*width;
    int pointIndexFrame = pointIndex + returnCursorSize;

    if(pointIndexFrame >= 0 && pointIndexFrame < frameBuffer.size())
    {
        if(z < zBuffer[pointIndex])
        {
            zBuffer[pointIndex] = z;
            frameBuffer[pointIndex] = c;
        }
    }
}



int main()
{
    //Get terminal resolution
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    
    int cubeSize;

    //No dynamic resizing for now
    width = size.ws_col;
    height = size.ws_row;

    cubeSize = min(width, height) * 0.3;
    offset = (min(width, height)-cubeSize)/2;
    cubeCenter = cubeSize/2;
    cubeCenter = cubeSize/2;
    cubeCenter = cubeSize/2;

    theta = theta = 0;
    frameBuffer = "\033[0;0H" + string(height * width, ' ');
    zBuffer = vector<float>(width*height, 2*cubeSize+distanceToCamera);

    while(1){
        //check terminal size
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        int newW = size.ws_col;
        int newH = size.ws_row;

        //if terminal size changes
        if(newW != width || newH != height)
        {
            //update variables
            width = newW;
            height = newH;
            cubeSize = min(width, height) * 0.3;
            offset = (min(width, height)-cubeSize)/2;
            cubeCenter = cubeSize/2;

            //rebuild frameBuffer and zBuffer
            frameBuffer = "\033[0;0H" + string(height * width, ' ');
            zBuffer = vector<float>(width*height, 2*cubeSize+distanceToCamera);
        }
        else
        {
            //clear buffers
            fill(frameBuffer.begin()+returnCursorSize, frameBuffer.end(), ' ');
            fill(zBuffer.begin(), zBuffer.end(), 2*cubeSize+distanceToCamera);
        }
        

        for(int y = -cubeSize; y < cubeSize; y++)
        {
            for(int x = -cubeSize; x<cubeSize; x++)
            {
                calculateSurfacePoint(x,y,cubeSize, '@');
                calculateSurfacePoint(x,y,-cubeSize, '*');
                calculateSurfacePoint(cubeSize,y,x, '.');
                calculateSurfacePoint(-cubeSize,y,x, '!');
                calculateSurfacePoint(x,cubeSize,y, '#');
                calculateSurfacePoint(x,-cubeSize,y, '$');
            }
        }

        cout<<frameBuffer;

        this_thread::sleep_for(milliseconds(10));
        theta+=0.03;
    }


    return 0;
}