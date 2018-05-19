#include "Students.h"           //class implementation
#include <iostream>             //endl
#include <iomanip>              //setw()
#include <sstream>              //stringstream
#include <cctype>

using namespace std;

int Students::objCount = 0;     //initialize counter variable to 0

Students::Students()
{
    ID = objCount++;
    for(int i = 0; i < 3; i++)
        placement[i] = -1;
}

void Students::getInput(stringstream &sso)
{
    sso >> firstName >> lastName >> homeForm;

    for(int i = 0; i < 5; i++){
        char temp;
        if(sso >> temp){
            if((temp!='X') && (temp!='x')){
                if(islower(temp))
                    choices[i] = int(temp)-97;
                else
                    choices[i] = int(temp)-65;
            }else
                choices[i] = -1;
        }
    }
}

int Students::getID()
{
    return ID;
}

string Students::getName()
{
    return (firstName + " " + lastName);
}

string Students::getHomeForm()
{
    return homeForm;
}

int *Students::getChoicesPtr()
{
    return choices;
}

int *Students::getPlacementPtr()
{
    return placement;
}

bool Students::isEmpty()
{
    return((lastName=="") || (firstName==""));
}

bool Students::isFull()
{
    bool status = true;
    for(int i = 0; i < 3; i++){
        if(placement[i]==-1){
            status = false;
            break;
        }
    }
    return status;
}

int Students::findChoice(int num)
{
    int pos = -1;
    for(int i = 0; i < 5; i++){
        if(choices[i]==num){
            pos = i;
            break;
        }
    }
    return pos;
}

bool Students::noRepeat(int n)
{
    bool status = true;
    for(int i = 0; i < 3; i++){
        if(placement[i]==n){
            status = false;
            break;
        }
    }
    return status;
}

string Students::toString()
{
    stringstream sso;
    sso << "ID" << setw(12) << ": ";
    sso << ID << endl;
    sso << "Name" << setw(10) << ": ";
    sso << this->getName() << endl;
    sso << "Homeform" << setw(6) << ": ";
    sso << homeForm << endl;
    for(int i = 0; i < 5; i++){
        sso << "Choice " << i+1 << setw(6) << " : ";
        sso << choices[i] << endl;
    }

    for(int i = 0; i < 3; i++){
        sso << "Placement " << i+1 << setw(3) << " : ";
        sso << placement[i] << endl;
    }

    sso << endl;
    return sso.str();
}

