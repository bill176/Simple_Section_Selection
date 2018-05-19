#ifndef STUDENTS_H
#define STUDENTS_H



#include <string>   //used to define type "string"
#include <sstream>

class Students
{
    public:
        Students();
        void getInput(std::stringstream &sso);
        int getID();
        std::string getName();
        std::string getHomeForm();
        int *getChoicesPtr();
        int *getPlacementPtr();
        std::string toString();

        bool isEmpty();
        bool isFull();
        int findChoice(int num);
        bool noRepeat(int n);
        static int objCount;


    private:
        int ID;
        std::string firstName;
        std::string lastName;
        std::string homeForm;
        int choices[5];
        int placement[3];
};

#endif // STUDENTS_H
