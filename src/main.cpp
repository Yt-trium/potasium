#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include <windows.h>

using namespace std;

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
char ask_key()
{
    char  ch;
    DWORD  mode;
    DWORD  counte;
    HANDLE hstdin = GetStdHandle( STD_INPUT_HANDLE );

    GetConsoleMode( hstdin, &mode );
    SetConsoleMode( hstdin, 0 );
    WaitForSingleObject( hstdin, INFINITE );
    ReadConsole( hstdin, &ch, 1, &counte, NULL );
    SetConsoleMode( hstdin, mode );
    return ch;
}

int main()
{
    // -----------------------------------------------------------------
    // Open Source
    // -----------------------------------------------------------------
    string srcFile;
    string tmp_string;
    unsigned int x_src = 0;
    unsigned int y_src = 0;
    unsigned int i = 0;
    unsigned int j = 0;

    bool debugger = true;
    int id;

    cout << "Welcom to Potassium Befunge Interpreter / Debugger" << endl;

    // ACTIVATE ONLY FOR RELEASE

    cout << "1. Interpreter" << endl << "2. Debugger" << endl;
    cin >> id;
    if(id == 1)
    {
        debugger = false;
    }
    else if(id == 2)
    {
        debugger = true;
    }
    else
        return 1;

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

    // -----------------------------------------------------------------
    // Interprette / Debug
    // -----------------------------------------------------------------

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
    unsigned int fsa = 0;    // FAKE STACK ADDRESS

    string output_save = "";

    srand (time(NULL));

    // DEBUGGER VARS
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    unsigned int x_d = 0;
    unsigned int y_d = 0;

    char bp[x_src][y_src];

    for(i=0;i<y_src;i++)
    {
        for(j=0;j<x_src;j++)
        {
            bp[j][i] = 0;
        }
    }

    bool execute = true;
    bool stop = true;
    string str_search_result;

    while(notend)
    {
        if(execute)
        {
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
            break;
            case '|':
                fsa--;
                tmp_i1 = fs[fsa];
                if(tmp_i1 == 0)
                    direction = 2;
                else
                    direction = -2;
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
                if(debugger)
                    output_save += tmp_i1;
                else
                    cout << tmp_i1;
            break;
            case ',':
                fsa--;
                tmp_c1 = fs[fsa];
                if(debugger)
                    output_save += tmp_c1;
                else
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
                if(src[x][y] >= '0' && src[x][y] <= '9')
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
        trampoline = 1;

        if( x>x_src || y>y_src || x<0 || y<0)
        {
            cout << "Error : Out of prog !" << endl;
            return 1;
        }
            if(debugger && bp[x][y] == 1)
                stop = true;
            if(stop && debugger)
                execute = false;
        }
        if(debugger && stop)
        {
            switch(ask_key())
            {
                case 'Z':
                case 'z':
                case 'W':
                case 'w':
                    y_d--;
                    if(y_d<0)
                        y_d = 0;
                break;
                case 'S':
                case 's':
                    y_d++;
                    if(y_d>=y_src)
                        y_d = y_src-1;
                break;
                case 'Q':
                case 'q':
                case 'A':
                case 'a':
                    x_d--;
                    if(x_d<0)
                        x_d = 0;
                break;
                case 'D':
                case 'd':
                    x_d++;
                    if(x_d>=x_src)
                        x_d = x_src-1;
                break;
                case 'E':
                case 'e':
                    src[x_d][y_d] = ask_key();
                break;
                case '+':
                    fs[fsa] = ask_key();
                    fsa++;
                break;
                case '-':
                    fsa--;
                break;
                case 8:         // RETURN
                    x = x_d;
                    y = y_d;
                break;
                case 9:         // TAB
                    if(bp[x_d][y_d] == 1)
                        bp[x_d][y_d] = 0;
                    else
                        bp[x_d][y_d] = 1;
                break;
                case ' ':
                    execute = true;
                    stop = true;
                break;
                case 13:        // ENTER
                    execute = true;
                    stop = false;
                break;
                default:

                break;
            }
        }
        if(debugger)
        {
            system("CLS");
            for(i=0;i<y_src;i++)
            {
                for(j=0;j<x_src;j++)
                {
                    if(j==x && i==y)            // POINTER
                    {
                        SetConsoleTextAttribute(hConsole, 0xAC);
                        cout << src[j][i];
                        SetConsoleTextAttribute(hConsole, 0x07);
                    }
                    else if(j==x_d && i==y_d && bp[j][i] != 0)
                    {
                        SetConsoleTextAttribute(hConsole, 0xCE);
                        cout << src[j][i];
                        SetConsoleTextAttribute(hConsole, 0x07);
                    }
                    else if(j==x_d && i==y_d)   // CURSOR
                    {
                        SetConsoleTextAttribute(hConsole, 0xCA);
                        cout << src[j][i];
                        SetConsoleTextAttribute(hConsole, 0x07);
                    }
                    else if(bp[j][i] != 0)   // BreakPoint
                    {
                        SetConsoleTextAttribute(hConsole, 0x0E);
                        cout << src[j][i];
                        SetConsoleTextAttribute(hConsole, 0x07);
                    }
                    else
                        cout << src[j][i];
                }
                cout << endl;
            }

            for(j=0;j<x_src;j++)
                cout << "-";
            cout << endl;

            cout << "STACK  : ";
            for(i=0;i<fsa;i++)
                cout << fs[i] << "(" << (char)fs[i] << ")"<< " ";
            cout << endl;

            cout << "OUTPUT : " << output_save << endl;
        }
    }

    delete [] fs;
    fs = NULL;

    ask_key();
    return 0;
}
