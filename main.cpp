#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include "Students.h"

using namespace std;

vector<Students> stus;

typedef vector<int> Row;
typedef vector<Row> Matrix;
Matrix preferences;

int counter[12];
int choiceTerminator[5][12];

int availability[12];               //check number of remaining slots
vector<Students> unplaced;

vector<Matrix> selection;

void enterData()
{
    cout << "1-Currently on Linux, 2-Currently on Windows\n-->";
    int inum;
    cin >> inum;
    string fileName;
    if(inum==1)
        fileName = "/home/user/Desktop/dat";
    else
        fileName = "C:/Users/user/Desktop/dat.txt";


    ifstream input;
    input.open(fileName.data());
    while(!input.is_open()){
        cerr << "Unable to open file. Please try again\n-->";
        cin >> fileName;
        input.open(fileName.data());
    }


    while(!input.eof()){
        stringstream sso;
        for(int i = 0; i < 3; i++){
            string temp;
            input >> temp;
            sso << temp << " ";
        }
        for(int i = 0; i < 5; i++){
            char temp;
            input >> temp;
            sso << temp << " ";
        }
        Students stu;
        stu.getInput(sso);
        if(!stu.isEmpty())
            stus.push_back(stu);
        else
            Students::objCount--;
    }

    input.close();

    cout << "Successfully entered " << Students::objCount << " pieces of information" << endl;
    
}


void checkNum()
{
    cin.get();
    cout << "Press 'q' to skip this part. Press enter to continue to the statistics\n-->";
    if(cin.get()!='q'){
        cout << "From checkNum function" << endl;
        for(int i = 0;i < stus.size(); i++){
            for(int a = 0; a < 5; a++){
                if((stus[i].getChoicesPtr())[a]==-1)
                    break;
                counter[(stus[i].getChoicesPtr())[a]]++;
            }
        }

        for(int i = 0; i < 12; i++)
            cout << char(i+65) << " :" << counter[i] << endl;
        cout << endl << endl;

    }else
        cin.get();


}

void toPreferences()
{
//top choice
    for(int a = 0; a < 12; a++){
        Row temp;
            for(int b = 0; b < stus.size(); b++){
                if(stus[b].getChoicesPtr()[0]==a)
                    temp.push_back(b);
            }
        preferences.push_back(temp);
        choiceTerminator[0][a] = temp.size();
    }

//any other choices
    for(int i = 1; i < 3; i++){
        for(int a = 0; a < 12; a++){
            for(int b = 0; b < stus.size(); b++){
                if(stus[b].getChoicesPtr()[i]==a)
                    preferences[a].push_back(b);
            }
            choiceTerminator[i][a] = preferences[a].size();
        }
    }
}

void displayPreferences()
{
    cout << "From displayTable function" << endl;
    for(int a = 0; a <12; a++){
        for(int b = 0; b < preferences[a].size(); b++){
            cout << preferences[a][b] << '\t';
        }
        cout << endl;
    }
    cout << endl << endl;
}

void putInPlace(int maxi = 150)
{
    for(int i = 0; i < 12; i++)
        availability[i] = maxi;

    for(int i = 2; i > -1; i--){
        for(int a = 0; a < 12; a++){
            for(int b = 0; b < choiceTerminator[i+1][a]; b++){
                if(availability[a]>0){
                    if(stus[preferences[a][b]].noRepeat(a)){

                        if(stus[preferences[a][b]].getPlacementPtr()[b/maxi]==-1){
                            stus[preferences[a][b]].getPlacementPtr()[b/maxi] = a;
                            availability[a]--;
                        }else{
                            int pos = stus[preferences[a][b]].findChoice(a);
                            if(pos < stus[preferences[a][b]].findChoice(stus[preferences[a][b]].getPlacementPtr()[b/maxi])){
                                stus[preferences[a][b]].getPlacementPtr()[b/maxi] = a;
                            }else{
                                //looking for alternitives
                                for(int c = 0; c < 3; c++){
                                    if(stus[preferences[a][b]].getPlacementPtr()[c]==-1){
                                        stus[preferences[a][b]].getPlacementPtr()[c] = a;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    for(int a = 0; a < stus.size(); a++){
        if(!stus[a].isFull()){
            for(int b = 0; b < 3; b++){
                if(stus[a].getPlacementPtr()[b]==-1){
                    for(int c = 0; c < 3; c++){
                        if(availability[stus[a].getChoicesPtr()[c]] > 0){
                            if(stus[a].noRepeat(stus[a].getChoicesPtr()[c])){
                                stus[a].getPlacementPtr()[b] = stus[a].getChoicesPtr()[c];
                            }
                        }
                    }
                }
            }
        }
    }



    for(int i = 0; i < stus.size(); i++){
        if(!stus[i].isFull())
            unplaced.push_back(stus[i]);
    }
}

void outputSuccessfulIndividual()
{
    ofstream output;
    output.open("/home/bill/Desktop/successful.txt", ios::trunc | ios::out);
    for(int i = 0; i < stus.size(); i++){
        //if(stus[i].isFull()){
            output << setw(3) << stus[i].getID() << " ";
            output << setw(30) << stus[i].getName() << " ";
            output << setw(15) << stus[i].getHomeForm() << '\t';
            for(int a = 0; a < 3; a++)
                output << ((stus[i].getChoicesPtr()[a]==-1) ? 'X' :(char(stus[i].getChoicesPtr()[a] + 65)))<< " ";
            output << '\t';
            for(int a = 0; a < 3; a++)
                output << char(stus[i].getPlacementPtr()[a] + 65) << " ";
            output << endl;
        /*}else{
            output << "Student no." << i << " could not be assigned. Try to look up in the file 'failed.txt'." << endl;
        }*/
    }
    output.close();
}

void outputSuccessfulAll()
{
    typedef vector<Matrix> Schedule;
    Schedule table;
    for(int a = 0; a < 3; a++){
        Matrix tempM;
        for(int b = 0; b < 12; b++){
            Row tempR;
            for(int c = 0; c < stus.size(); c++){
                if(stus[c].getPlacementPtr()[a]==b)
                    tempR.push_back(c);
            }
            tempM.push_back(tempR);
        }
        table.push_back(tempM);
    }

    ofstream output;
    output.open("/home/user/Desktop/all.txt", ios::trunc | ios::out);
    for(int a = 0; a < 3; a++){
        output << "Session " << a + 1 << ": " << endl << endl;
        for(int b = 0; b < 12; b++){
            output << setw(2) << char(b + 65) << ": ";
            for(int c = 0; c < table[a][b].size(); c++){
                output << setw(4) << table[a][b][c];
            }
            output << endl;
        }
        output << endl << endl;
    }
    output.close();
    /*int assignment[3][12][stus.size()];
    for(int a = 0; a < 12; a++){
        for(int b = 0; b < 3; b++){
            for(int c = 0; c < stus.size(); c++){
                if(stus[c].isFull()){
                    if(stus[c].getPlacementPtr()[b]==a)
                        assignment[b][a][c] = stus[c].getID();
                }
            }
        }
    }

    ofstream output;
    output.open("/home/bill/all_schdule.txt", ios::trunc | ios::out);
    for(int a = 0; a < 3; a++){
        output << "-------------------------Session " << a+1 << "---------------------------\n";
        output << "Lectures no:\n";
        for(int b = 0; b < 12; b++)
            output << setw(5) << b << '\t';
        output << endl;

        for(int c = 0; c < stus.size(); c++){
            for(int b = 0; b < 12; b++)
                output << setw(5) << assignment[a][b][c];
        }
        output << endl << endl;
    }
    output.close();*/
}

void outputFailed()
{
    ofstream output;
    output.open("/home/user/Desktop/failed.txt", ios::trunc | ios::out);
    for(int i = 0; i < unplaced.size(); i++){
        output << unplaced[i].getID() << " ";
        output << setw(18) << unplaced[i].getName() << " ";
        output << setw(10) << unplaced[i].getHomeForm() << '\t';

        for(int a = 0; a < 5; a++)
            output << ((unplaced[i].getChoicesPtr()[a]==-1)? 'X' : char(unplaced[i].getChoicesPtr()[a] + 65)) << " ";
        output << '\t';
        for(int a = 0; a < 3; a++){
            if(unplaced[i].getPlacementPtr()[a]==-1)
                output << 0 << " ";
            else
                output << char(unplaced[i].getPlacementPtr()[a] + 65) << " ";
        }
        output << endl;
    }
    output.close();
}

void outputTop3Counts()
{
        for(int a = 0; a < 12; a++){
        cout << char(a+65) << ": ";
        for(int b = 0; b < 3; b++){
            cout << choiceTerminator[b][a] << '\t';
        }
       /* for(int b = 0; b < 3; b++)
            cout << choiceTerminator[b][a] - ((b==0)? 0:choiceTerminator[b-1][a]) << '\t';
            */
        cout << endl;
    }
}

void outputNetTop3Counts()
{
    for(int a = 0; a < 12; a++){
        cout << char(a+65) << ": ";

        for(int b = 0; b < 3; b++)
            cout << choiceTerminator[b][a] - ((b==0)? 0:choiceTerminator[b-1][a]) << '\t';

        cout << endl;
    }
}

void read_updated_table()
{
    string fileName = "/home/user/Desktop/tab";
    ifstream input;
    input.open(fileName.data());
    while(!input.is_open()){
        cerr << "Unable to open file. Please try again\n-->";
        cin >> fileName;
        input.open(fileName.data());
    }
    int sz_deciding = 12;
    for(int a = 0; a < 3; a++){

        Matrix tempM;
        for(int i = 0; i < sz_deciding; i++){
            string temp,str;
            getline(input, temp);
            stringstream sso;
            sso << temp;
            sso >> str;
            int inum;
            Row tempR;
            while(sso >> inum)
                tempR.push_back(inum);
            tempM.push_back(tempR);
        }
        selection.push_back(tempM);

    }
    int conversionConst = 65;
/*
    for(int c = 0; c < 3; c++){
            for(int a = 0; a < selection[c].size(); a++){
            cout << char(a+conversionConst) << ": ";
            for(int b = 0; b < selection[c][a].size(); b++){
                cout << selection[c][a][b] << '\t';
            }
            cout << endl;
        }
    }

    for(int c = 0; c < 3; c++){
        int counter = 0;
        for(int b = 0; b < selection[c].size(); b++){
            counter += selection[c][b].size();
        }
        cout << endl << counter << endl;
    }
*/

}



int main()
{
    enterData();
    read_updated_table();

    /*checkNum();
    toPreferences();
    displayPreferences();
    putInPlace();
    cin.get();*/

/*
    for(int i = 0; i < unplaced.size(); i++)
        cout << unplaced[i].toString() << endl;*/

/*
    outputSuccessfulIndividual();
    outputSuccessfulAll();
    outputFailed();
*/


    return 0;
}

