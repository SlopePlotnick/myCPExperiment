#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

using namespace std;

fstream source;
fstream output;

const string key[15] = { "program", "const", "var", "procedure", "begin", "if", "end", "while", "call", "read", "write", "then", "odd", "do" };

// To determine the place of fault.
int line;
int column;

/**
 * Verify whether is blank char, 1 is BC, 0 is not.
 * @param ch char
 */
int isBC(char ch)
{
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
    {
        switch (ch)
        {
        case ' ':
            column++;
            break;
        case '\t':
            column += 4; // A tab = 4 spaces
            break;
        case '\r':
        case '\n':
            line++;
            column = 1;
            break;
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Add a character to strToken.
 * @param a strToken
 * @param ch add charater
 * @return new strToken string
 */
string Concat(string a, char ch)
{
    a.push_back(ch);
    return a;
}

/**
 * Check a character whether is a digit.
 * @param ch A character
 * @return 1 is true, 0 is false.
 */
int isDigit(char ch)
{
    if (ch >= '0' && ch <= '9')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Check a character whether is a letter.
 * @param ch A character
 * @return 1 is true, 0 is false.
 */
int isLetter(char ch)
{
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * Check strToken whether is a reserved word.
 * @param strToken search-forward string.
 * @return 1 is reserved, 0 is not.
 */
int Reserve(string strToken)
{
    int i;
    for (i = 0; i < 15; i++)
    {
        if (strToken.compare(key[i]) == 0)
        {
            return 1; // is reserved;
        }
    }
    return 0;
}

/**
 * Move the cursor back a character.
 */
void Retract()
{
    if (!source.eof())
    {
        source.seekg(-1, ios::cur);
    }
}

/**
 * Main function.
 * @return 0
 */
int main()
{
    string inputFileName;

    cout << "Enter the program file name: ";
    cin >> inputFileName;
    cout << endl;

    source.open(inputFileName, ios::in);           // Read file
    output.open("la_output", ios::out | ios::trunc); // Write file
    line = 1;
    column = 1;

    if (!source.is_open())
    {
        cout << "Cannot open the source file!\a" << endl;
        exit(1);
    }
    if (!output.is_open())
    {
        cout << "Cannot open the output file!\a" << endl;
    }
    else
    {
        // Header of the file (DateTime & File name & Lang set)

        time_t rawtime;
        struct tm* timeinfo;

        time(&rawtime);
        timeinfo = localtime(&rawtime);

        output << "# Generate Time: " << asctime(timeinfo);
        output << "# Program File Name: " << inputFileName << endl;
        output << "# Language Set: PL/0" << endl;
        output << endl;
    }

    string strToken;
    char ch;
    while (!source.eof())
    {
        ch = source.get();

        if (isBC(ch))
        { // Blank character check
            strToken = "";
        }
        else if (isLetter(ch))
        { // Letter check

            while (isLetter(ch) || isDigit(ch))
            { // ID check loop
                strToken = Concat(strToken, ch);
                column++;
                ch = source.get();
            }

            if (Reserve(strToken))
            { // Reserved word check
                //                cout << strToken << ", RESERVED" << endl;
                output << strToken << " RESERVED" << ' ' << line << ' ' << column << endl;
            }
            else
            {
                //                cout << strToken << ", ID" << endl;
                output << strToken << " ID" << ' ' << line << ' ' << column << endl;
            }
            strToken = "";

            Retract();
        }
        else if (isDigit(ch))
        { // Digit check
            while (isDigit(ch))
            {
                strToken = Concat(strToken, ch);
                column++;
                ch = source.get();
            }

            if (isLetter(ch))
            {
                // Fault: an ID cannot start with a number.
                cout << "^" << ' ' << strToken << ' ' << "error" << ' ' << line << ' ' << column << endl;
                cout << "[Program ERROR]"
                    << " [" << line << "," << column << "] "
                    << "Invalid ID: ";
                output << "^" << ' ' << strToken << ' ' << "error" << ' ' << line << ' ' << column << endl;
                output << "[Program ERROR]"
                    << " [" << line << "," << column << "] "
                    << "Invalid ID: ";

                while (isLetter(ch) || isDigit(ch))
                {
                    strToken = Concat(strToken, ch);
                    column++;
                    ch = source.get();
                }

                // Output the entire invalid ID.
                cout << "\"" << strToken << "\"" << endl;
                output << "\"" << strToken << "\"" << endl;
            }
            else
            {
                //                cout << strToken << ", INT" << endl;
                output << strToken << " INT" << ' ' << line << ' ' << column << endl;
            }

            Retract();
            strToken = "";
        }
        else
        {
            switch (ch)
            { // Other characters check
            case '=':
                column++;
                //                    cout << ch << ", COP" << endl;
                    output << ch << " COP" << ' ' << line << ' ' << column << endl;
                break;
            case '<':
                column++;
                // Advance searching
                ch = source.get();
                if (ch == '>')
                {
                    column++;
                    //                        cout << "<>, COP" << endl;
                    output << "<> COP" << ' ' << line << ' ' << column << endl;
                }
                else if (ch == '=')
                {
                    column++;
                    //                        cout << "<=, COP" << endl;
                    output << "<= COP" << ' ' << line << ' ' << column << endl;
                }
                else
                {
                    //                        cout << "<, COP" << endl;
                    output << "< COP" << ' ' << line << ' ' << column << endl;
                    Retract();
                }
                break;
            case '>':
                column++;
                ch = source.get();
                if (ch == '=')
                {
                    column++;
                    //                        cout << ">=, COP" << endl;
                    output << ">= COP" << ' ' << line << ' ' << column << endl;
                }
                else
                {
                    //                        cout << ">, COP" << endl;
                    output << "> COP" << ' ' << line << ' ' << column << endl;
                    Retract();
                }
                break;
            case ':':
                column++;
                ch = source.get();
                if (ch == '=')
                {
                    column++;
                    //                        cout << ":=, AOP" << endl;
                    output << ":= AOP" << ' ' << line << ' ' << column << endl;
                }
                else
                {
                    cout << "^" << ' ' << strToken << ' ' << "error" << ' ' << line << ' ' << column << endl;
                    cout << "[Program ERROR]"
                        << " [" << line << "," << column << "] "
                        << "Missing \"=\" near the \":\" ;" << endl;
                    output << "^" << ' ' << strToken << ' ' << "error" << ' ' << line << ' ' << column << endl;
                    output << "[Program ERROR]"
                        << " [" << line << "," << column << "] "
                        << "Missing \"=\" near the \":\" ;" << endl;
                    Retract();
                }
                break;
            case '+':
            case '-':
            case '*':
            case '/':
                column++;
                //                    cout << ch << ", OOP" << endl;
                output << ch << " OOP" << ' ' << line << ' ' << column << endl;
                break;
            case ';':
                column++;
                //                    cout << ch << ", EOP" << endl;
                output << ch << " EOP" << ' ' << line << ' ' << column << endl;
                break;
            case '(':
            case ')':
            case ',':
                column++;
                //                    cout << ch << ", SOP" << endl;
                output << ch << " SOP" << ' ' << line << ' ' << column << endl;
                break;
            default:
                bool is_chinese = false;
                strToken = Concat(strToken, ch);
                // Advance searching to avoid Chinese fault
                if (ch == '\377') break;
                if (ch < 0)
                {
                    is_chinese = true;
                    ch = source.get();
                    strToken = Concat(strToken, ch);
                    ch = source.get();
                    strToken = Concat(strToken, ch);
                }
                cout << "^" << ' ' << strToken << ' ' << "error" << ' ' << line << ' ' << column << endl;
                cout << "[Program ERROR]"
                    << " [" << line << "," << column << "] "
                    << "Unknown character: " << strToken << endl;
                output << "^" << ' ' << strToken << ' ' << "error" << ' ' << line << ' ' << column << endl;
                output << "[Program ERROR]"
                    << " [" << line << "," << column << "] "
                    << "Unknown character: " << strToken << endl;
                column++;
                if (is_chinese)
                {
                    column = column + 2;
                }
                strToken = "";
            }
        }
    }

    source.close();
    output.close();
    cout << endl;
    cout << "Save results successfully" << endl;
    return 0;
}

