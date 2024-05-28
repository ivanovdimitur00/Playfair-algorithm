/**
 * GOAL - Encode plain text without any loss of information.
 *
 *supports (languages): English
 *supports: numbers, punctuation, symbols
 */
#include <iostream>
#include <string>
#include <istream>
#include <list>
#include <vector>

using namespace std;

const char BOGUS_LETTER = 'x'; // by default, it's either "x" or "z". Can be whatever we choose.

// english alphabet is 26 symbols long and the numbers are 10.
// that makes 62 symbols total (2x26 + 10). 
// adding '&' and '@' completes our playfair matrix to 8x8 (64 symbols)
const size_t PLAYFAIR_MATRIX_HEIGHT = 8;
const size_t PLAYFAIR_MATRIX_WIDTH = 8;
char ALPHABET[PLAYFAIR_MATRIX_HEIGHT * PLAYFAIR_MATRIX_WIDTH] = {
                                                                   'a','b','c','d','e','f','g','h',
                                                                   'i','j','k','l','m','n','o','p',
                                                                   'q','r','s','t','u','v','w','x',
                                                                   'y','z','0','1','2','3','4','5',
                                                                   '6','7','8','9','@','&','A','B',
                                                                   'C','D','E','F','G','H','I','J',
                                                                   'K','L','M','N','O','P','Q','R',
                                                                   'S','T','U','V','W','X','Y','Z'
                                                                };
bool LETTER_IN_USE[PLAYFAIR_MATRIX_HEIGHT * PLAYFAIR_MATRIX_WIDTH] = {
                                                                        false,false,false,false,false,false,false,false,
                                                                        false,false,false,false,false,false,false,false,
                                                                        false,false,false,false,false,false,false,false,
                                                                        false,false,false,false,false,false,false,false,
                                                                        false,false,false,false,false,false,false,false,
                                                                        false,false,false,false,false,false,false,false,
                                                                        false,false,false,false,false,false,false,false,
                                                                        false,false,false,false,false,false,false,false
                                                                     };

char** playfair_matrix;
enum MODE {encrypt, decrypt};
vector<bool> was_edge_case;



void showExtractedSymbols(list<int>& punct_and_whsp_pos, list<char> punct_and_whsp_char)
{
    list<int>::iterator pos = begin(punct_and_whsp_pos);
    list<int>::iterator pos_end = end(punct_and_whsp_pos);
    list<char>::iterator c = begin(punct_and_whsp_char);
    list<char>::iterator c_end = end(punct_and_whsp_char);

    for (;pos != pos_end && c != c_end; ++pos, ++c)
    {
        cout << '(' << *c << ", " << *pos << ')' << ' ';
    }
    cout << endl;
}

char** initializeMatrix()
{
    char** pfm = 0;
    pfm = new char* [PLAYFAIR_MATRIX_HEIGHT];
    for (size_t i = 0; i < PLAYFAIR_MATRIX_HEIGHT; i++)
    {
        pfm[i] = new char[PLAYFAIR_MATRIX_WIDTH];
    }

    return pfm;
}

void deleteMatrix(char** pfm)
{
    for (size_t i = 0; i < PLAYFAIR_MATRIX_HEIGHT; i++)
    {
        delete[] pfm[i];
    }
    delete[] pfm;
    pfm = 0;
}

void printMatrix()
{
    for (size_t i = 0; i < PLAYFAIR_MATRIX_HEIGHT; i++)
    {
        for (size_t j = 0; j < PLAYFAIR_MATRIX_WIDTH; j++)
        {
            cout << playfair_matrix[i][j] << ' ';
        }
        cout << endl;
    }
}

void printVector()
{
    for (int i = 0; i < was_edge_case.size(); i++)
    {
        cout << was_edge_case[i];
    }
}

void bogusCharacter(string& msg, bool& bogus)
{
    if (msg.length() % 2 == 1)
    {
        msg += BOGUS_LETTER;
        bogus = true;
        return;
    }
    if (msg.length() % 2 == 0 && bogus)
    {
        msg.erase(msg.length() - 1, 1);
        bogus = false;
    }
}

bool checkSymbol(char c)
{
    return c >= 'a' && c <= 'z' ||
           c >= 'A' && c <= 'Z' ||
           c >= '0' && c <= '9' ||
           c == '&' || 
           c == '@';
}

void cleanMessage(string& msg, list<int>& pnwp, list<char>& pnwc)
{
    string trunc_msg;

    for (size_t i = 0; i < msg.length(); i++)
    {
        if (checkSymbol(msg[i]))
        {
            trunc_msg.push_back(msg[i]);
        }
        else
        {
            pnwp.push_back(i);
            pnwc.push_back(msg[i]);
        }
    }
    msg = trunc_msg;
}

void restoreMessage(string& msg, list<int>& pnwp, list<char>& pnwc)
{
    while(!pnwp.empty() && !pnwc.empty())
    {
        msg.insert(pnwp.front(), 1, pnwc.front());
        pnwp.pop_front();
        pnwc.pop_front();
    }
}

void cleanKeyword(string& kw)
{
    int i = 0;
    while (i != kw.size())
    {
        if (checkSymbol(kw[i]))
        {
            i++;
        }
        else 
        {
            kw.erase(i,1);
        }
    }
}

bool notUsed(const char& c)
{
    for (int i = 0; i < PLAYFAIR_MATRIX_HEIGHT * PLAYFAIR_MATRIX_WIDTH; i++)
    {
        if (c == ALPHABET[i] && !LETTER_IN_USE[i]) //if we have found the symbol and it hasn't been used
        {
            LETTER_IN_USE[i] = true; //mark it as used
            return true;
        }
    }
    return false;
}

void makePlayfairMatrix(const string& key)
{
    int row = 0, col = 0; // matrix iterators

    //add all of the symbols from the keyword into the playfair matrix
    for (int i = 0; i < key.length(); i++)
    {
        if (notUsed(key[i]))
        {
            playfair_matrix[row][col] = key[i];
            col++;
            if (col == 8) 
            {
                row++;
                col = 0;
            }
        }
    }
    
    //add all of the remaining symbols from our alphabet
    for (size_t i = 0; i < PLAYFAIR_MATRIX_HEIGHT * PLAYFAIR_MATRIX_WIDTH; i++)
    {
        if (!LETTER_IN_USE[i]) //if the symbol is marked as "not used"
        {
            playfair_matrix[row][col] = ALPHABET[i];
            col++;
            if (col == 8)
            {
                row++;
                col = 0;
            }
            LETTER_IN_USE[i] = true;
        }
    }
}

void findCoords(const char& c, int& c_x, int& c_y )
{
    for (int i = 0; i < PLAYFAIR_MATRIX_HEIGHT; i++)
    {
        for (int j = 0; j < PLAYFAIR_MATRIX_WIDTH; j++)
        {
            if (c == playfair_matrix[i][j])
            {
                c_x = i;
                c_y = j;
                return;
            }
        }
    }
}
// removee unnecessary if statements and combine decrypt
string replace(const char& c1, const char& c2, const MODE& mode, const int& tuple_pos)
{
    string replacement;
    
    // coordinates of letters
    int c1_x = 0;
    int c1_y = 0;
    int c2_x = 0;
    int c2_y = 0;

    //c1 = pfm[c1_x][c2_y];
    //c2 = pfm[c2_x][c1_y];
    
    //the two letters are the same (example: caddy ---> ca | dd | yx). Letters do not change
    if (mode == encrypt)
    {
        if (c1 == c2)
        {
            replacement.push_back(c1);
            replacement.push_back(c2);
            was_edge_case.push_back(false);
            cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
            return replacement;
        }

        //the two letters are different, so we find their coords 
        findCoords(c1, c1_x, c1_y);
        findCoords(c2, c2_x, c2_y);

        //we have a square/rectangle where either dimension is > 1
        if (c1_x != c2_x && c1_y != c2_y)
        {
            replacement.push_back(playfair_matrix[c1_x][c2_y]);
            replacement.push_back(playfair_matrix[c2_x][c1_y]);
            was_edge_case.push_back(false);
            cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
            return replacement;
        }

        // we have a rectangle of size 1xN or Nx1
        // NOTE: direction is c1 ---> c2
        if (c1_x == c2_x) // they are on the same row
        {
            if (c1_y < c2_y) // direction is right
            {
                if (c2_y == PLAYFAIR_MATRIX_WIDTH - 1) // is a right edge case
                {
                    replacement.push_back(playfair_matrix[c1_x][c1_y + 1]);
                    replacement.push_back(playfair_matrix[c2_x][0]);
                    was_edge_case.push_back(true);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
                else // is not an edge case, then we do normal right shift
                {
                    replacement.push_back(playfair_matrix[c1_x][c1_y + 1]);
                    replacement.push_back(playfair_matrix[c2_x][c2_y + 1]);
                    was_edge_case.push_back(false);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
            }
            else // direction is left
            {
                if (c2_y == 0) // is a left edge case
                {
                    replacement.push_back(playfair_matrix[c1_x][c1_y - 1]);
                    replacement.push_back(playfair_matrix[c2_x][PLAYFAIR_MATRIX_WIDTH - 1]);
                    was_edge_case.push_back(true);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
                else // is not an edge case, then we do normal left shift
                {
                    replacement.push_back(playfair_matrix[c1_x][c1_y - 1]);
                    replacement.push_back(playfair_matrix[c2_x][c2_y - 1]);
                    was_edge_case.push_back(false);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
            }
        }
        else // they are on the same column
        {
            if (c1_x < c2_x) // direction is down
            {
                if (c2_x == PLAYFAIR_MATRIX_HEIGHT - 1) // is a bottom edge case
                {
                    replacement.push_back(playfair_matrix[c1_x + 1][c1_y]);
                    replacement.push_back(playfair_matrix[0][c2_y]);
                    was_edge_case.push_back(true);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
                else // is not an edge case, then we do normal bottom shift
                {
                    replacement.push_back(playfair_matrix[c1_x + 1][c1_y]);
                    replacement.push_back(playfair_matrix[c2_x + 1][c2_y]);
                    was_edge_case.push_back(false);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
            }
            else // direction is up
            {
                if (c2_x == 0) //is a ceiling edge case
                {
                    replacement.push_back(playfair_matrix[c1_x - 1][c1_y]);
                    replacement.push_back(playfair_matrix[PLAYFAIR_MATRIX_HEIGHT - 1][c2_y]);
                    was_edge_case.push_back(true);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
                else // is not an edge case, then do normal upwartd shift
                {
                    replacement.push_back(playfair_matrix[c1_x - 1][c1_y]);
                    replacement.push_back(playfair_matrix[c2_x - 1][c2_y]);
                    was_edge_case.push_back(false);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
            }
        }
    }
    
    if (mode == decrypt)
    {
        //the two letters are the same (example: caddy ---> ca | dd | yx). Letters do not change
        if (c1 == c2)
        {
            replacement.push_back(c1);
            replacement.push_back(c2);
            cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
            return replacement;
        }

        //the two letters are different, so we find their coords 
        findCoords(c1, c1_x, c1_y);
        findCoords(c2, c2_x, c2_y);

        //we have a square/rectangle where either dimension is > 1
        if (c1_x != c2_x && c1_y != c2_y)
        {
            replacement.push_back(playfair_matrix[c1_x][c2_y]);
            replacement.push_back(playfair_matrix[c2_x][c1_y]);
            cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
            return replacement;
        }

        // we have a rectangle of size 1xN or Nx1
        // NOTE: direction is c1 ---> c2
        if (!was_edge_case[tuple_pos]) //none of the transitions were former edge cases
        {
            if (c1_x == c2_x) // they are on the same row
            {
                if (c1_y < c2_y) // direction is right
                {
                    replacement.push_back(playfair_matrix[c1_x][c1_y - 1]);
                    replacement.push_back(playfair_matrix[c2_x][c2_y - 1]);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
                else // direction is left
                {
                    replacement.push_back(playfair_matrix[c1_x][c1_y + 1]);
                    replacement.push_back(playfair_matrix[c2_x][c2_y + 1]);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
            }
            else // they are on the same column
            {
                if (c1_x < c2_x) // direction is down
                {
                    replacement.push_back(playfair_matrix[c1_x - 1][c1_y]);
                    replacement.push_back(playfair_matrix[c2_x - 1][c2_y]);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
                else // direction is up
                {
                    replacement.push_back(playfair_matrix[c1_x + 1][c1_y]);
                    replacement.push_back(playfair_matrix[c2_x + 1][c2_y]);
                    cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                    return replacement;
                }
            }
        }

        if (was_edge_case[tuple_pos]) //if the symbol pair was a former edge case
        {
            if (c1_x == c2_x) // they are on the same row
            {
                if (c1_y < c2_y) // direction is right
                {
                    if (c2_y == PLAYFAIR_MATRIX_WIDTH - 1) // is a right edge case
                    {
                        replacement.push_back(playfair_matrix[c1_x][c1_y + 1]);
                        replacement.push_back(playfair_matrix[c2_x][0]);
                        cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                        return replacement;
                    }
                    else // is not an edge case, then we do normal shift
                    {
                        replacement.push_back(playfair_matrix[c1_x][c1_y - 1]);
                        replacement.push_back(playfair_matrix[c2_x][c2_y - 1]);
                        cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                        return replacement;
                    }
                }
                else // direction is left
                {
                    if (c2_y == 0) // is a left edge case
                    {
                        replacement.push_back(playfair_matrix[c1_x][c1_y - 1]);
                        replacement.push_back(playfair_matrix[c2_x][PLAYFAIR_MATRIX_WIDTH - 1]);
                        cout << c1 << ' ' << c2 << " ---> "<< replacement.at(0) << ' ' << replacement.at(1) << endl;
                        return replacement;
                    }
                    else // is not an edge case, then we do normal shift
                    {
                        replacement.push_back(playfair_matrix[c1_x][c1_y + 1]);
                        replacement.push_back(playfair_matrix[c2_x][c2_y + 1]);
                        cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                        return replacement;
                    }
                }
            }
            else // they are on the same column
            {
                if (c1_x < c2_x) // direction is down
                {
                    if (c2_x == PLAYFAIR_MATRIX_HEIGHT - 1) // is a bottom edge case
                    {
                        replacement.push_back(playfair_matrix[c1_x + 1][c1_y]);
                        replacement.push_back(playfair_matrix[0][c2_y]);
                        cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                        return replacement;
                    }
                    else // is not an edge case, then we do normal shift
                    {
                        replacement.push_back(playfair_matrix[c1_x + 1][c1_y]);
                        replacement.push_back(playfair_matrix[c2_x + 1][c2_y]);
                        cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                        return replacement;
                    }
                }
                else // direction is up
                {
                    if (c2_x == 0) //is a ceiling edge case
                    {
                        replacement.push_back(playfair_matrix[c1_x - 1][c1_y]);
                        replacement.push_back(playfair_matrix[PLAYFAIR_MATRIX_HEIGHT - 1][c2_y]);
                        cout << c1 << ' ' << c2 << " ---> " << replacement.at(0) << ' ' << replacement.at(1) << endl;
                        return replacement;
                    }
                    else // is not an edge case, then do normal shift
                    {
                        replacement.push_back(playfair_matrix[c1_x - 1][c1_y]);
                        replacement.push_back(playfair_matrix[c2_x - 1][c2_y]);
                        cout << c1 << ' ' << c2 << " ---> "<< replacement.at(0) << ' ' << replacement.at(1) << endl;
                        return replacement;
                    }
                }
            }
        }
    }
}

string substitute(const string& msg, const MODE& mode)
{
    string cryptogram;

    for (int i = 0; i < msg.length(); i += 2)
    {
        cryptogram.append(replace(msg[i], msg[i + 1], mode, i/2));
    }

    return cryptogram;
}

string playfair(string& msg, const string& key, const MODE& mode)
{   
    makePlayfairMatrix(key);
    cout << "----------------" << endl 
         << "Playfair matrix:" << endl 
         << "----------------" << endl;
    printMatrix();


    return substitute(msg, mode);
}

int main()
{
    string decrypted_message;

    string message;
    cout << "Input the message you want to encode: ";
    getline(cin, message);

    string keyword;
    cout << "Input the keyword you want to use for encoding: ";
    getline(cin, keyword);

    //cleans up the massage to prepare it for playfair algorithm
    list<int> punct_and_whsp_pos;
    list<char> punct_and_whsp_char;
    cleanMessage(message, punct_and_whsp_pos, punct_and_whsp_char);
    cout << "Cleaned message:" << message << endl;
    cout << "Extracted symbols: ";
    showExtractedSymbols(punct_and_whsp_pos, punct_and_whsp_char);

    //cleans the keyword. punctuation and whitespaces are unnecessary in the process
    cleanKeyword(keyword);
    cout << "Cleaned keyword:" << keyword << endl;


    //if the message's length is odd, append a character
    //the boolean is used, so we can later remove the bogus character
    bool bogus_character_used = false;
    bogusCharacter(message, bogus_character_used);
    // cout << "Bogus character used? " << bogus_character_used << endl;
    // cout << ' ' << message << endl;

    playfair_matrix = initializeMatrix();
    string cryptogram = playfair(message,keyword, encrypt);
    cout <<"Encrypted message: "<< cryptogram << endl;

    decrypted_message = playfair(cryptogram, keyword, decrypt);
    deleteMatrix(playfair_matrix);
    bogusCharacter(decrypted_message, bogus_character_used);
    restoreMessage(decrypted_message, punct_and_whsp_pos, punct_and_whsp_char);
    cout <<"Decrypted message: "<< decrypted_message << endl;

    return 0;
}

/**
 * IMPROVEMENTS
 * 
 *  - make the size of playfair matrix, the alphabet array and the bool taken array generateable from
 *    an alphabet input (CSV,xls,txt)
 *  - unify cleaning operation over message and keyword
 *  - rewrite replacement with switch-case
 * 
 *  - support for more languages
 */