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


vector<vector<vector<char>>> rotateY(vector<vector<vector<char>>> space3d, int theta){
    vector<vector<vector<char>>> rotatedSpace3d = space3d;
    int centerX = space3d.size()/2;
    int centerZ = space3d[0][0].size()/2;

    double cosTheta = cos(theta);
    double sinTheta = sin(theta);

    for(size_t x = 0; x<space3d.size(); x++)
    {
        for(size_t y = 0; y<space3d[0].size(); y++)
        {
            for(size_t z = 0; z<space3d[0][0].size(); z++)
            {
                int relX = x-centerX;
                int relZ = z-centerZ;
                int newX = round(relX * cosTheta + relZ * sinTheta) + centerX;
                int newZ = round(-relX * sinTheta + relZ * cosTheta) + centerZ;
                //cout<<"newX="<<newX<<" x="<<x<<" newZ="<<newZ<<" z="<<z<<endl;
                if (newX >= 0 && newX < space3d.size() && newZ >= 0 && newZ < space3d[0][0].size())
                {
                    rotatedSpace3d[newX][y][newZ] = space3d[x][y][z];
                }
            }
        }
    }

    return rotatedSpace3d;
}



char view(vector<char>& v)
{
    for(int i = 0; i<v.size(); i++){
        if(v[i] != ' ')
            return v[i];
    }

    return ' ';
}



int main()
{

    //Get terminal resolution
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    
    vector<char> cubeFaces = {'@', '$', '#', '*', '!', '.'};
    int space3dSize = min(size.ws_row, size.ws_col);
    int cubeSide = space3dSize/1.5;

    if(cubeSide%2 == 1)
        cubeSide--;

    vector<vector<vector<char>>> space3d(space3dSize, vector<vector<char>>(space3dSize, vector<char>(space3dSize, ' ')));


    //CUBE COORDS
    //x: wide
    //y: top
    //z: depth
    //[x][y][z]

    //INITIALIZE BASIC CUBE
    //reference coord of cube in space3d
    int cubeStartRef = (space3dSize-cubeSide)/2;
    int cubeEndRef = cubeStartRef+cubeSide-1;
    cout<<"CubeSide= "<<cubeSide<<" - screenSize= "<<space3dSize<<endl;
    for(int i = cubeStartRef; i<=cubeEndRef; i++)
    {
        for(int j = cubeStartRef; j<=cubeEndRef; j++)
        {
            //x-z plane
            space3d[i][cubeStartRef][j] = cubeFaces[1];
            space3d[i][cubeEndRef][j] = cubeFaces[4];
            //y-z plane
            space3d[cubeStartRef][i][j] = cubeFaces[2];
            space3d[cubeEndRef][i][j] = cubeFaces[3];
            //x-y plane
            space3d[i][j][cubeStartRef] = cubeFaces[0];
            space3d[i][j][cubeEndRef] = cubeFaces[5];

        }
    }

    for(int i = 0; i<50; i++){
        space3d = rotateY(space3d, 3);

        string screen = "\033[0;0H";
        for(int i = 0; i<space3dSize; i++)
        {
            for(int j = 0; j<space3dSize; j++)
            {
                screen += view(space3d[i][j]);
            }
            screen += '\n';
        }
        cout<<screen;

        this_thread::sleep_for(milliseconds(1000));
    }

    

    
    return 0;
}