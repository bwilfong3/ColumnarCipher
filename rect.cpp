// Name: Ben Wilfong
// File Name: rect.cpp
// Due Date: 3/22/16
// Program Description:
/*   This program will encrypt or decrypt a given text using
 *   a columnar transposition cipher system, based on
 *   the user's input from the command line. The user will provide
 *   an input file name and a destination file name, along with
 *   a keyword.
 */

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <math.h>

using namespace std;

void encryptionMode();
void decryptionMode();
string getFileText(string);
char** decrypt(char** rectangle,int*,int,int);
void writeToFile(string,char**,int,int);
int* getKeySequence(string);
char** putInRectangle(string,string,int,int);
char** encrypt(char**,int*,int,int); 

int main(int argc, char** argv)
{
     if(argc == 1) // not enough arguments
     {
          cout << "To use this program, please provide a command line argument"
               << " (either -e or -d) to enter encryption or decryption mode. "
               << endl;
          
          return 0;
     }

     else if(argc == 2)
     {
          if(strcmp(argv[1], "-e") == 0)
          {
               cout << "Entering encryption mode..." << endl;
               encryptionMode(); // start encryption process
          }

          else if(strcmp(argv[1], "-d") == 0)
          {
               cout << "Entering decryption mode..." << endl;
               decryptionMode(); // start decryption process
          }

          else // bad arguments
          {
               cout << "Invalid command line arguments! Please try again with"
                       " ./rectangle [-e | -d]" << endl;
               return 0;
          }
     }

     else // bad arguments
     {
          cout << "Invalid command line arguments! Please try again with"
                  " ./rectangle [-e | -d]" << endl;
          return 0;
     }
}

//===============================================================================

/*
 * This function handles the dialog for encryption as well as
 * calls the actual encryption function. When the user specifies
 * their input/output files, the text from the input file is obtained.
 * The text obtained from this file is put into a 2d character array,
 * one that will represent the rectangle used for encryption.
 * The user will also specify a key, which is pass off to a method to
 * determine the key sequence for encryption. The rows and columns
 * for the rectangle are also calculated in this function. After
 * encryption is performed, the results are written to the specified
 * output file in a normal text format (no rectangle formatting).
 */

void encryptionMode()
{
     string plainText, key, inputFile, cipherText, outputFile;
     int *keySequence;
     int rows, columns;
     char **rectangle, **cipherRectangle;

     cout << "Please enter the name of your input (plaintext) file:" << endl;
     getline(cin, inputFile);

     cout << "Please provide a name for the output (ciphertext) file:" << endl;
     getline(cin, outputFile);

     cout << "Please enter the keyword you wish to use for encryption." << endl;
     
     getline(cin,key);
 
     plainText = getFileText(inputFile); // pull input from specified file

     cout << "Plaintext acquired:\n" << plainText << endl;

     keySequence = getKeySequence(key); // get the order in which to organize the rows

     columns = key.length();

     if(plainText.length() % key.length() == 0)
          rows = plainText.length() / key.length();
     else
          rows = (plainText.length() / key.length()) + 1;

     rectangle = putInRectangle(plainText, key, rows, columns); // set up plaintext in 2d array

     cipherRectangle = encrypt(rectangle, keySequence, rows, columns);

     writeToFile(outputFile, cipherRectangle, rows, columns);

     delete[] keySequence; // free up dynamically allocated arrays

     for (int i = 0; i < rows; i++)
          delete[] rectangle[i];

     delete[] rectangle;

     for (int i = 0; i < rows; i++)
          delete[] cipherRectangle[i];

     delete[] cipherRectangle;
}

//===============================================================================

/*
 * This function takes the keyword provided by the user
 * and generates an array to act as the key sequence for
 * the keyword. This is done by taking the smallest element
 * that hasn't already been recorded in the array (handling 
 * duplicates) and then recording their sequence number in
 * their respective position in the sequence array.
 */

int* getKeySequence(string keyword)
{
     int* keySequence = new int[keyword.length()];
     char temp;
     int pos;

     for(int i = 0; i < keyword.length(); i++)
          keySequence[i] = -1; // placeholder for key order

     for(int i = 0; i < keyword.length(); i++)
     {
          temp = '~'; // arbitrary "max" ascii char, shouldn't be part of your keyword!
          pos = 0;    // arbitrary start position 

          for(int j = 0; j < keyword.length(); j++)
          {
               if(temp > keyword.at(j) && keySequence[j] == -1) // if temp is larger than the
                                                                // next element and that element 
                                                                // hasn't been recorded
               {
                    temp = keyword.at(j); // get the new smallest element of the keyword
                    pos = j; // record the position for the key sequence
               }    
          }

          keySequence[pos] = i; // record the sequence number of the smallest unrecorded char
     }     

     cout << "Keyword sequence: ";

     for(int i = 0; i < keyword.length(); i++)
          cout << keySequence[i] << " "; // show key sequence

     cout << endl;

     return keySequence;
}

//===============================================================================

/*
 * This function takes the given plain/ciphertext and fits
 * it into a rectangle to be manipulated for encryption
 * or decryption. If we run out of plain/ciphertext to
 * fit into the rectangle, we will just use X's to fill
 * up the space. These X's will also help in saving the 
 * format when reloading the file for decryption later on.
 */

char** putInRectangle(string plainText, string keyword, int rows, int columns)
{
     char** rectangle = new char*[rows];

     for (int i = 0; i < rows; i++)
          rectangle[i] = new char[columns];

     for(int i = 0; i < rows; i++)
     {
          for(int j = 0; j < columns; j++)
          {
               if ((i * columns + j) >= plainText.length())
                    rectangle[i][j] = 'X'; // placeholders for empty spots
               else
                    rectangle[i][j] = plainText.at(i * columns + j);
          }
     }

     for(int i = 0; i < rows; i++)
     {
          for(int j = 0; j < columns; j++)
               cout << rectangle[i][j] << " ";

          cout << endl;
     }

     cout << endl;

     return rectangle;
}

//===============================================================================

/*
 * This function uses the rectangle (2d array) and the key sequence
 * array to move contents of the unmodified rectangle into a new
 * rectangle (2d array) in the encrypted format.
 */

char** encrypt(char** rectangle, int* keySequence, int rows, int columns)
{
     char** cipherRectangle = new char*[rows];

     for (int i = 0; i < rows; i++)
          cipherRectangle[i] = new char[columns];

     for(int j = 0; j < columns; j++)
     {
          for(int i = 0; i < rows; i++)
               cipherRectangle[i][keySequence[j]] = rectangle[i][j];
               
     }

     cout << "Rectangle (rearranged): " << endl;

     for(int i = 0; i < rows; i++)
     {
          for(int j = 0; j < columns; j++)
               cout << cipherRectangle[i][j] << " ";

          cout << endl;
     }

     return cipherRectangle;
}

//===============================================================================

/*
 * This function takes the unmodified rectangle and key sequence
 * and populates a new rectangle in the decrypted format.
 */

char** decrypt(char** rectangle, int* keySequence, int rows, int columns)
{
     char** plainRectangle = new char*[rows];

     for (int i = 0; i < rows; i++)
          plainRectangle[i] = new char[columns];

     for(int j = 0; j < columns; j++)
     {
          for(int i = 0; i < rows; i++)
               plainRectangle[i][j] = rectangle[i][keySequence[j]];
               
     }

     cout << "Rectangle (rearranged): " << endl;

     for(int i = 0; i < rows; i++)
     {
          for(int j = 0; j < columns; j++)
               cout << plainRectangle[i][j] << " ";

          cout << endl;
     }

     return plainRectangle;
}

//===============================================================================

/*
 * This function handles the decryption dialog, as well as the user input
 * and calling the appropriate methods for decryption, such as getting the
 * text from the file, putting it into a rectangle, decrypting that rectangle,
 * and writing the results to a file, etc.
 */

void decryptionMode()
{
     string key, inputFile, cipherText, outputFile;
     char **rectangle, **plainRectangle;
     int *keySequence;
     int rows, columns;

     cout << "Please enter the name of your input (ciphertext) file:" << endl;
     getline(cin, inputFile);

     cout << "Please provide a name for the output (plaintext) file:" << endl;
     getline(cin, outputFile);

     cout << "Please enter the decryption key used with this ciphertext" << endl;
     
     getline(cin,key);

     keySequence = getKeySequence(key);

     cipherText = getFileText(inputFile);

     cout << "Ciphertext acquired:\n" << cipherText << endl;

     if(cipherText.length() % key.length() == 0)
          rows = cipherText.length() / key.length();
     else
          rows = (cipherText.length() / key.length()) + 1;

     columns = key.length();

     cout << "Rows, columns: " << rows << ", " << columns << endl;

     rectangle = putInRectangle(cipherText, key, rows, columns);

     plainRectangle = decrypt(rectangle, keySequence, rows, columns);

     writeToFile(outputFile, plainRectangle, rows, columns);

     delete[] keySequence; // free up dynamically allocated arrays

     for (int i = 0; i < rows; i++)
          delete[] rectangle[i];

     delete[] rectangle;

     for (int i = 0; i < rows; i++)
          delete[] plainRectangle[i];

     delete[] plainRectangle;

     return;
}

//===============================================================================

string getFileText(string inputFileName)
{
     string input = "";
     char buffer;

     ifstream inputFile;
     inputFile.open(inputFileName.c_str());

     if(!inputFile)
     {
          cout << "Bad file name. Please try again." << endl;
          exit(0); // quit
     }

     while(inputFile >> buffer)
     {
          if(buffer >= 'a' && buffer <= 'z') // do character filtering
               input = input + buffer;

          else if(buffer >= 'A' && buffer <= 'Z')
          {
               buffer += 32; // convert to lowercase
               input = input + buffer; 
          }
     }
     
     inputFile.close();

     return input;         
}

//===============================================================================

void writeToFile(string outputFileName, char** text, int rows, int columns)
{
     ofstream outputFile;
     outputFile.open(outputFileName.c_str());

     if(!outputFile)
     {
          cout << "Bad file name. Please try again." << endl;
          exit(0); // quit
     }

     for(int i = 0; i < rows; i++)
     {
          for(int j = 0; j < columns; j++)
               outputFile << text[i][j]; // write to file
     }
     outputFile.close();

     cout << "File write to " << outputFileName << " complete." << endl;

     return;
}
