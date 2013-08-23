#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>

using namespace std;

/*
CONSTANTE OF DEBUG LEVEL
0 : NO DEBUG
1 : NORMAL MESSAGE
2 : FULL MESSAGE
*/
const int dbglvl = 0;

int ask_int()
{
    int value;
    cout << "Please Enter Value :" << endl;
    cin >> value;
    return value;
}
int ask_char()
{
    char value;
    cout << "Please Enter Value :" << endl;
    cin >> value;
    return value;
}

int main()
{
    // Open Source

    string srcFile;
    string tmp_string;
    unsigned int x_src = 0;
    unsigned int y_src = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    cout << "Welcom to Potassium Befunge Debugger" << endl;
    cout << "Befunge Filename :" << endl;
    cin >> srcFile;

    ifstream fileStream(srcFile.c_str(), ios::in);

    while(getline(fileStream, tmp_string))
    {
        y_src++;
        if(tmp_string.length() > x_src)
            x_src = tmp_string.length();

        tmp_string.clear();
    }

    char src[x_src][y_src];

    fileStream.close();
    fileStream.open(srcFile.c_str(), ios::in);

    while(getline(fileStream, tmp_string))
    {
        for(j=0;j<=x_src;j++)
        {
            if(tmp_string.length() < j)
                src[j][i] = ' ';
            else
                src[j][i] = tmp_string[j];

        }
        tmp_string.clear();
        i++;
    }


    for(i=0;i<=y_src;i++)
    {
        for(j=0;j<x_src;j++)
        {
            cout << src[j][i];
        }
        cout << endl;
    }

    fileStream.close();

    // Interpreter
    bool notend = true;
    bool charMode = false;

    unsigned int x = 0;
    unsigned int y = 0;
    int direction = 1;
    int trampoline = 1;

    // TEMP INT VALUE
    int tmp_i1;
    int tmp_i2;
    char tmp_c1;

    int *fs = NULL; // FAKE STACK
    fs = new int [0xFFF];
    int fsa = 0;    // FAKE STACK ADDRESS

    srand (time(NULL));

    while(notend)
    {

        if(dbglvl>=2)
        {
            cout << src[x][y] << endl;
            cout << "x : " << x << endl;
            cout << "y : " << y << endl;
        }

        if(charMode)
        {
            if(src[x][y] == '"')
            {
                charMode = false;
            }
            else
            {
                fs[fsa] = src[x][y];
                fsa++;
            }
        }
        else
        {
        switch(src[x][y])
        {
            case ' ':
                // nothing
            break;
            // DIRECTION
            case '>':
                direction = 1;
            break;
            case '<':
                direction = -1;
            break;
            case 'v':
                direction = 2;
            break;
            case '^':
                direction = -2;
            break;
            case '?':
                tmp_i1 = rand() % 2 + 1;
                tmp_i2 = rand() % 2 + 1;
                if(tmp_i1 == 1)
                {
                    if(tmp_i2 == 1)
                    {
                        direction = 1;
                    }
                    else
                    {
                        direction = -1;
                    }
                }
                else
                {
                    if(tmp_i2 == 1)
                    {
                        direction = 2;
                    }
                    else
                    {
                        direction = -2;
                    }
                }
            break;
            case '_':
                fsa--;
                tmp_i1 = fs[fsa];
                if(tmp_i1 == 0)
                    direction = 1;
                else
                    direction = -1;
                fsa++;
            break;
            case '|':
                fsa--;
                tmp_i1 = fs[fsa];
                if(tmp_i1 == 0)
                    direction = 2;
                else
                    direction = -2;
                fsa++;
            break;
            // OPERATION
            case '+':
                fsa --;
                tmp_i1 = fs[fsa];
                fsa--;
                tmp_i2 = fs[fsa];
                fs[fsa] = tmp_i1+tmp_i2;
                fsa++;
            break;
            case '-':
                fsa--;
                tmp_i1 = fs[fsa];
                fsa--;
                tmp_i2 = fs[fsa];
                fs[fsa] = tmp_i2-tmp_i1;
                fsa++;
            break;
            case '*':
                fsa--;
                tmp_i1 = fs[fsa];
                fsa--;
                tmp_i2 = fs[fsa];
                fs[fsa] = tmp_i2*tmp_i1;
                fsa++;
            break;
            case '/':
                fsa--;
                tmp_i1 = fs[fsa];
                fsa--;
                tmp_i2 = fs[fsa];
                while(tmp_i1 == 0)
                    tmp_i1 = ask_int();
                fs[fsa] = tmp_i2/tmp_i1;
                fsa++;
            break;
            case '%':
                fsa--;
                tmp_i1 = fs[fsa];
                fsa--;
                tmp_i2 = fs[fsa];
                while(tmp_i1 == 0)
                    tmp_i1 = ask_int();
                fs[fsa] = tmp_i2%tmp_i1;
                fsa++;
            break;
            case '!':
                fsa--;
                tmp_i1 = fs[fsa];
                if(tmp_i1 == 0)
                    fs[fsa] = 1;
                else
                    fs[fsa] = 0;
                fsa++;
            break;
            case '`':
                fsa--;
                tmp_i1 = fs[fsa];
                fsa--;
                tmp_i2 = fs[fsa];
                if(tmp_i2 > tmp_i1)
                    fs[fsa] = 1;
                else
                    fs[fsa] = 0;
                fsa++;
            break;
            case ':':
                tmp_i1 = fs[fsa-1];
                fs[fsa] = tmp_i1;
                fsa++;
            break;
            case '\\':
                fsa--;
                tmp_i1 = fs[fsa];
                fs[fsa] = fs[fsa-1];
                fs[fsa-1] = tmp_i1;
                fsa++;
            break;
            case '$':
                fsa--;
            break;
            // I/O
            case '.':
                fsa--;
                tmp_i1 = fs[fsa];
                cout << tmp_i1;
            break;
            case ',':
                fsa--;
                tmp_c1 = fs[fsa];
                cout << tmp_c1;
            break;
            case '&':
                fs[fsa] = ask_int();
                fsa++;
            break;
            case '~':
                fs[fsa] = ask_char();
                fsa++;
            break;
            // SYSTEM
            case 'p':
                fsa--;
                tmp_i2 = fs[fsa];
                fsa--;
                tmp_i1 = fs[fsa];
                fsa--;
                tmp_c1 = fs[fsa];
                src[tmp_i1][tmp_i2] = tmp_c1;
            break;
            case 'g':
                fsa--;
                tmp_i2 = fs[fsa];
                fsa--;
                tmp_i1 = fs[fsa];
                fs[fsa] = src[tmp_i1][tmp_i2];
                fsa++;
            break;
            case '"':
                charMode = true;
            break;
            case '#':
                trampoline = 2;
            break;
            case '@':
                notend = false;
            break;
            // PUSH VALUE
            default:
                if(src[x][y] > '0' ||src[x][y] < '9')
                {
                    fs[fsa] = src[x][y] - '0';
                    fsa++;
                }
            break;
        }
        }

        switch(direction)
        {
            case 1:
                x+=trampoline;
            break;
            case -1:
                x-=trampoline;
            break;
            case 2:
                y+=trampoline;
            break;
            case -2:
                y-=trampoline;
            break;
            default:
                cout << "Error : direction ?" << endl;
                return 1;
            break;
        }

        if( x>x_src || y>y_src || x<0 || y<0)
        {
            cout << "Error : Out of prog !" << endl;
            return 1;
        }
        if(dbglvl>=2)
            cin.get();
    }

    delete [] fs;
    fs = NULL;

    return 0;
}
