#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>

class Unit
{
public:
    std::string value;
    std::string key;
    int line;
    int column;

    void print();
};

/**
 * Print all properties of a unit
 */
void Unit::print()
{
    std::cout << "-----------------" << std::endl;
    std::cout << "Value: " << value << std::endl;
    std::cout << "Key: " << key << std::endl;
    std::cout << "Position: [" << line << "," << column << "]" << std::endl;
    std::cout << "-----------------" << std::endl;
}

using namespace std;

Unit unit, former_unit;
int errorStack[1000] = {-1};
int *errorType = errorStack;
bool error = false;
fstream gaSource;
fstream gaOutput;


string line;
string::iterator itLine;

void ReadLine()
{
    if (getline(gaSource, line))
    {

        itLine = line.begin();
        // Jump annotations & empty lines
        while (*itLine == '#' || line.empty())
        {
            getline(gaSource, line);
            itLine = line.begin();
        }

        former_unit = unit; // ?

        istringstream iss(line);

        // Lexical error
        if (*itLine == '^')
        {
            char ch;
            iss >> ch; // '^'
            iss >> unit.value;
            iss >> unit.key;
            iss >> unit.line;
            iss >> unit.column;
            getline(gaSource, line); // jump error information
        }
        // No lexical error
        else
        {
            iss >> unit.value;
            iss >> unit.key;
            iss >> unit.line;
            iss >> unit.column;
        }
    }
}

void ThrowError(int type){
    error = true; // change the error status
    *errorType = type; // push current error into the stack
    errorType++; // move the pointer of the stack's top after pushing a new item
    
    switch(type)
    {
        case 0:
            cout << "[Grammar ERROR] " << " [" << unit.line << "," << unit.column << "] " << "Spell error \"program\"" << endl;
            break;
        case 1:
            cout << "[Grammar ERROR] " << " [" << unit.line << "," << unit.column << "] " << "Missing identifier after \"program\"" << endl;
            break;
        case 2:
            cout << "[Grammar ERROR] "<< " [" << former_unit.line << "," << former_unit.column << "] " <<"Missing end character \";\"" << endl;
            break;
        case 3:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier in \"const\"" << endl;
            break;
        case 4:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing assignment operation" << endl;
            break;
        case 5:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing assignment integer" << endl;
            break;
        case 6:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \"var\"" << endl;
            break;
        case 7:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \",\"" << endl;
            break;
        case 8:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Cannot resolve type \"" << unit.value << "\"" << endl;
            break;
        case 9:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier after \"procedure\"" << endl;
            break;
        case 10:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"procedure\"" << endl;
            break;
        case 11:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier in \"procedure\"" << endl;
            break;
        case 12:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing word \"begin\"" << endl;
            break;
        case 13:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing word \"end\"" << endl;
            break;
        case 14:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing statement identifier" << endl;
            break;
        case 15:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing word \"then\"" << endl;
            break;
        case 16:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing word \"do\"" << endl;
            break;
        case 17:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier in \"call\"" << endl;
            break;
        case 18:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"call\"" << endl;
            break;
        case 19:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing identifier in \"read\"" << endl;
            break;
        case 20:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"read\"" << endl;
            break;
        case 21:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses in \"write\"" << endl;
            break;
        case 22:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing the compare operator" << endl;
            break;
        case 23:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Missing parentheses" << endl;
            break;
        case 24:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Wrong factor" << endl;
            break;
        case 25:
            cout << "[Grammar ERROR] "<< " [" << former_unit.line << "," << former_unit.column << "] " << "Extra semicolon \";\" before \"end\"" << endl;
            break;
        case 26:
            cout << "[Grammar ERROR] "<< " [" << former_unit.line << "," << former_unit.column << "] " << "Missing comma \",\" in \"var\"" << endl;
            break;
        case 27:
            cout << "[Grammar ERROR] "<< " [" << former_unit.line << "," << former_unit.column << "] " << "Missing comma \",\" in \"condecl\"" << endl;
            break;
        default:
            cout << "[Grammar ERROR] "<< " [" << unit.line << "," << unit.column << "] " <<"Unknown error" << endl;
            break;
    }
}

void Exp();

void Factor()
{
    if (unit.key == "ID" || *errorType == 24)
    {
        if (unit.key != "ID" && unit.key != "INT" && unit.value != "(" && *errorType == 24)
            errorType++; // jump error
        ReadLine();
    }
    else if (unit.key == "INT")
    {
        ReadLine();
    }
    else if (unit.key == "SOP" && unit.value == "(")
    {
        ReadLine();
        Exp();
        if ((unit.key == "SOP" && unit.value == ")") || *errorType == 23)
        {
            if (unit.value != ")" && *errorType == 23)
                errorType++;
            else
                ReadLine();
        }
        else
        {
            ThrowError(23);
        }
    }
    else
    {
        ThrowError(24);
    }
}

void Term()
{
    Factor();
    while (unit.value == "*" || unit.value == "/")
    {
        ReadLine();
        Factor();
    }
}

void Exp()
{
    if (unit.value == "+" || unit.value == "-")
    {
        ReadLine();
    }
    Term();
    while (unit.value == "+" || unit.value == "-")
    {
        ReadLine();
        Term();
    }
}

void Lexp()
{
    if (unit.value == "odd")
    {
        ReadLine();
        Exp();
    }
    else
    {
        Exp();
        if (unit.key == "COP" || *errorType == 22)
        {
            if (unit.key != "COP" && *errorType == 22)
                errorType++;
            else
                ReadLine();
            Exp();
        }
        else
        {
            ThrowError(22);
        }
    }

}


void Body();

void Statement()
{
    if (unit.key == "RESERVED" && unit.value == "if")
    {
        ReadLine();
        Lexp();
        if ((unit.key == "RESERVED" && unit.value == "then") || *errorType == 15)
        {
            if (*errorType == 15 && unit.value != "then")
                errorType++;
            else
                ReadLine();
            Statement();
            if (unit.key == "RESERVED" && unit.value == "else")
            {
                ReadLine();
                Statement();
            }
        }
        else
        {
            ThrowError(15);
        }

    }

    else if (unit.key == "RESERVED" && unit.value == "while")
    {
        ReadLine();
        Lexp();
        if ((unit.key == "RESERVED" && unit.value == "do") || *errorType == 16)
        {
            if (*errorType == 16 && unit.value != "do")
                errorType++;
            else
                ReadLine();
            Statement();
        }
        else
        {
            ThrowError(16);
        }

    }

    else if (unit.key == "RESERVED" && unit.value == "call")
    {
        ReadLine();
        if (unit.key == "ID" || *errorType == 17)
        {
            if (*errorType == 17 && unit.key != "ID")
                errorType++;
            else
                ReadLine();
            if (unit.key == "SOP" && unit.value == "(")
            {
                ReadLine();
                if (unit.value != ")")
                {
                    Exp();
                    while (unit.key == "SOP" && unit.value == ",")
                    {
                        ReadLine();
                        Exp();
                    }
                }
                if ((unit.key == "SOP" && unit.value == ")") || *errorType == 18)
                {
                    if (*errorType == 18 && unit.value != ")")
                        errorType++;
                    else
                        ReadLine();
                }
                else
                {
                    ThrowError(18);
                }
            }
        }
        else
        {
            ThrowError(17);
        }

    }

    else if (unit.key == "RESERVED" && unit.value == "read")
    {
        ReadLine();
        if ((unit.key == "SOP" && unit.value == "(") || *errorType == 20)
        {
            if (*errorType == 20 && unit.value != "(")
                errorType++;
            else
                ReadLine();
            if (unit.key == "ID" || *errorType == 19)
            {
                if (*errorType == 19 && unit.key != "ID")
                    errorType++;
                else
                    ReadLine();
                while (unit.key == "SOP" && unit.value == ",")
                {
                    ReadLine();
                    if (unit.key == "ID" || *errorType == 19)
                    {
                        if (*errorType == 19 && unit.key != "ID")
                            errorType++;
                        else
                            ReadLine();
                    }
                    else
                    {
                        ThrowError(19);
                    }
                }
                if (unit.key == "SOP" && unit.value == ")" || *errorType == 20)
                {
                    if (*errorType == 20 && unit.value != ")")
                        errorType++;
                    else
                        ReadLine();
                }
                else
                {
                    ThrowError(20);
                }
            }
            else
            {
                ThrowError(19);
            }

        }
        else
        {
            ThrowError(20);
        }

    }

    else if (unit.key == "RESERVED" && unit.value == "write")
    {
        ReadLine();
        if (unit.key == "SOP" && unit.value == "(" || *errorType == 21)
        {
            if (*errorType == 21 && unit.value != "(")
                errorType++;
            else
                ReadLine();
            Exp();
            while (unit.key == "SOP" && unit.value == ",")
            {
                ReadLine();
                Exp();
            }
            if (unit.key == "SOP" && unit.value == ")" || *errorType == 21)
            {
                if (*errorType == 21 && unit.value != ")")
                    errorType++;
                else
                    ReadLine();
            }
            else
            {
                ThrowError(21);
            }
        }
        else
        {
            ThrowError(21);
        }

    }

    else if (unit.key == "ID")
    {
        ReadLine();
        if (unit.key == "AOP" && unit.value == ":=" || *errorType == 4) {
            if (*errorType == 4 && unit.value != ":=" )
                errorType++;
            else
                ReadLine();
            Exp();
        }
        else
        {
            ThrowError(4);
        }
    }

    else
    {
        Body();
    }
}

void Body()
{
    if (unit.key == "RESERVED" && unit.value == "begin" || *errorType == 12)
    {
        if (*errorType == 12 && unit.value != "begin")
            errorType++;
        else
            ReadLine();
        Statement();
        while (unit.key == "EOP" && unit.value == ";")
        {
            ReadLine();
            if (unit.value == "end")
            {
                if (*errorType != 25)
                    ThrowError(25);
                break;
            }
            Statement();
        }
        if(*errorType == 25)
        {
            errorType++;
        }
        if (unit.key == "RESERVED" && unit.value == "end" || *errorType == 13) {
            if (*errorType == 13 && unit.value != "end")
                errorType++;
            else
                ReadLine();
        }
        else
        {
            ThrowError(13);
        }
    }
    else
    {
        ThrowError(12);
    }

}

void Block();

void Proc()
{
    if(unit.key == "ID" || *errorType == 9)
    {
        if(*errorType == 9 && unit.key != "ID")
            errorType++;
        else
            ReadLine();
        if (unit.key == "SOP" && unit.value == "(" || *errorType == 10)
        {
            if (*errorType == 10 && unit.value != "(")
                errorType++;
            else
                ReadLine();
            if (unit.value != ")")
            {
                if (unit.key == "ID" || *errorType == 11)
                {
                    if (*errorType == 11 && unit.key != "ID")
                        errorType++;
                    else
                        ReadLine();
                    
                    while (unit.value == "," && unit.key == "SOP")
                    {
                        ReadLine();
                        if (unit.key == "ID"|| *errorType == 11)
                        {
                            if(unit.key != "ID" && *errorType == 11)
                                errorType++;
                            else
                                ReadLine();
                        }
                        else
                        {
                            ThrowError(11); // Missing id
                        }
                    }
                }
                else
                {
                    ThrowError(11);
                }
            }
            if (unit.key == "SOP" && unit.value == ")" || *errorType == 10)
            {
                if (*errorType == 10 && unit.value != ")")
                    errorType++;
                else
                    ReadLine();

                if (unit.key == "EOP" && unit.value == ";" || *errorType == 2)
                {
                    if (*errorType == 2 && unit.value != ";")
                        errorType++;
                    else
                        ReadLine();
                    Block();

                    while (unit.key == "EOP" && unit.value == ";")
                    {
                        ReadLine();
                        Proc();
                    }
                }
                else
                {
                    ThrowError(2);
                }
            }
            else
            {
                ThrowError(10);
            }
        }
        else
        {
            ThrowError(10);
        }
    }
    else
    {
        ThrowError(9);
    }
}

void Vardecl()
{
    if (unit.key == "ID" || *errorType == 6)
    {
        if (unit.key != "ID" && *errorType == 6)
            errorType++;
        else
            ReadLine();
        while (unit.value == "," && unit.key == "SOP" || *errorType == 26)
        {
            if (*errorType == 26 && unit.value != ",")
                errorType++;
            else
                ReadLine();
            if (unit.key == "ID" || *errorType == 7)
            {
                if (unit.key != "ID" && *errorType == 7)
                    errorType++;
                else
                    ReadLine();
            }
            else
            {
                ThrowError(7);// Missing ID
            }
        }
        if(unit.key == "ID")
        {
            ThrowError(26);
        }
        if (unit.value == ";" && unit.key == "EOP" || *errorType == 2)
        {
            if (*errorType == 2 && unit.value != ";")
                errorType++;
            else
                ReadLine();
        }
        else
        {
            ThrowError(2); // Missing EOP
        }
    }
    else
    {
        ThrowError(6); // Missing ID
    }
}

void Const()
{
    if (unit.key == "ID" || *errorType == 3)
    {
        if (unit.key != "ID" && *errorType == 3)
            errorType++;
        else
            ReadLine();
        
        if ((unit.key == "AOP" && unit.value == ":=") || *errorType == 4)
        {
            if (!(unit.key == "AOP" && unit.value == ":=") && *errorType == 4)
                errorType++;
            else
                ReadLine();
            
            if (unit.key == "INT" || *errorType == 5)
            {
                if (unit.key != "INT" && *errorType == 5)
                    errorType++;
                else
                    ReadLine();
            }
            else
            {
                ThrowError(5);
            }
        }
        else
        {
            ThrowError(4);
        }
    }
    else
    {
        ThrowError(3);
    }
}

void Condecl()
{
    Const();
    while ((unit.key == "SOP" && unit.value == ","))
    {
        ReadLine();
        Const();
    }
    
    if ((unit.key == "EOP" && unit.value == ";") || *errorType == 2)
    {
        if (!(unit.key == "EOP" && unit.value == ";") && *errorType == 2)
            errorType++;
        else
            ReadLine();
    }
    else
    {
        ThrowError(2);
    }
}

void Block()
{
    if (unit.value == "const" && unit.key == "RESERVED" && !error)
    {
        ReadLine();
        Condecl();
    }
    else if (unit.key == "ID" && *errorType != 8 && !error)
    {
        ThrowError(8); // Cannot resolve type
//        scary model
        while(unit.key != "EOP" && unit.key != "RESERVED")
        {
            ReadLine();
        }
        ReadLine();
    }
    else if (*errorType == 8 && !error)
    {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED")
        {
            ReadLine();
        }
        ReadLine();
    }

    if (unit.value == "var" && unit.key == "RESERVED" && !error)
    {
        ReadLine();
        Vardecl();
    }
    else if (unit.key == "ID" && *errorType != 8 && !error)
    {
        ThrowError(8); // Cannot resolve type
        while(unit.key != "EOP" && unit.key != "RESERVED")
        {
            ReadLine();
        }
        ReadLine();
    }
    else if (*errorType == 8 && !error)
    {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED")
        {
            ReadLine();
        }
        ReadLine();
    }

    if (unit.value == "procedure" && unit.key == "RESERVED" && !error)
    {
        ReadLine();
        Proc();
    }
    else if (unit.key == "ID" && *errorType != 8 && !error)
    {
        ThrowError(8); // Cannot resolve type
        while(unit.key != "EOP" && unit.key != "RESERVED")
        {
            ReadLine();
        }
        ReadLine();
    }
    else if (*errorType == 8 && !error)
    {
        errorType++;
        while(unit.key != "EOP" && unit.key != "RESERVED")
        {
            ReadLine();
        }
        ReadLine();
    }

    if (!error)
    {
        Body();
    }
}

void Prog()
{
    ReadLine();
    if ((unit.value == "program" && unit.key == "RESERVED") || *errorType == 0)
    {
        if (*errorType == 0)
            errorType++;
        ReadLine();
        if (unit.key == "ID" || *errorType == 1)
        {
            if (*errorType == 1)
                errorType++;
            else
                ReadLine();
            if ((unit.value == ";" && unit.key == "EOP") || *errorType == 2)
            {
                if (*errorType == 2 && unit.value != ";")
                    errorType++;
                else
                    ReadLine();
                Block();
            }
            else
            {
                ThrowError(2);
            }
        }
        else
        {
            ThrowError(1);
        }
    }
    else
    {
        ThrowError(0);
    }

}

/**
 * Open IO files.
 */
void OpenFile() {
    gaSource.open("la_output", ios::in); // Read file
    gaOutput.open("ga_output", ios::out | ios::trunc); // Write file

    if (!gaSource.is_open()) {
        cout << "Cannot open the gaSource file!\a" << endl;
        exit(1);
    }
    if (!gaOutput.is_open()) {
        cout << "Cannot open the gaOutput file!\a" << endl;
    } else {
        // Header of the file (DateTime & File name & Lang set)

        time_t rawtime;
        struct tm * timeinfo;

        time (&rawtime);
        timeinfo = localtime (&rawtime);

        gaOutput << "# Grammar Analysis Result" << endl;
        gaOutput << "# Generated Time: " << asctime(timeinfo);
        gaOutput << "# Language Set: PL/0" << endl;
        gaOutput << endl;
    }
}

/**
 * Close IO files.
 */
void CloseFile() {
    gaSource.close();
    gaOutput.close();
}

/**
 * Print the error stack out.
 */
void PrintErrorStack() {
    cout << "[DEBUG] Error Stack" << endl;
    cout << endl << "|<<<< ERROR STACK <<<<<" << endl;
    int *p;
    cout << "|";
    for (p = errorStack; p != errorType; p++) {
        cout << *p << " ";
    }
    cout << endl << "|<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

int main() {

    OpenFile();
    Prog();
    CloseFile();
    // when there's an error, the program will end and when will start from begin again.
    while(error){
        error = false;
        errorType = errorStack;
        OpenFile();
        Prog();
        CloseFile();
    }

//    PrintErrorStack();
    return 0;
}
