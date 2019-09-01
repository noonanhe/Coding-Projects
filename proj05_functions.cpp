#include <iostream>
#include <string>
#include <cmath>
#include <vector>

using std::string;
using std::cin; using std::cout; using std::endl; using std::noskipws;
using std::getline;
using std::pow;
using std::vector;

#include "proj05_functions.h"

/*for each char in the string its coverted to lowercase*/
string lower_case(const string& lower){
    string temp="";
    for (auto element: lower){
        temp+= tolower(element);
    }//for each letter in the stirng
    return temp;
}//lower_case function

/* if the letter is a lower alpha character, then it assigns value according
to how far from a it is. The for loop then test whether a binary
value starting from 16, the binary value of the last position in a binary
number of size 5. If this number value fits then 1 is added to the string
else a 0 is added*/
string to_binary(char letter){
    string binary="";
    int value=0;
    if (isalpha(letter)&&islower(letter)){
        value = letter - 'a';
        for (int i=16; i >0; i/=2){
            if (value/i >0){
                binary+='1';
                value= value%i;
            }//if i goes into value at least once
            else
                binary+='0';
        }//for each binary value in a string of 5
    }//if its a lowercase alphabetic character
    else
        return binary;
    return binary;
}//method to_binary

/*takes a string and based on character position and whether that character
is a 1 or 0 adds 2 to the power of the correct number 
according to that characters position. The last if statement checks to make sure
that each binary returns a lowercase letter */
char from_binary(const string& bit_str){
    int value=0;
    int power=4;
    if (bit_str.size()!=5)
        return 0;
    for (int i=0; i<=4; i++){
        if (bit_str[i]=='1')
            value += pow(2,power);
        else
            if (bit_str[i]!='0')
                return 0;
        power--;
    }//for each char in bit_str
    value= value+ 'a';
    if (value <=122)
        return static_cast<char>(value);
    else
        return 0;
}//from_binary method

/*finds the number of alpha characters in plaintext and secretmessage
and then checks that plain has 5x as many as secret */
bool check_message(const string& plaintext, const string& secret_message){
    int count_plain=0;
    int count_secret=0;
    bool check= false;
    for (auto element: plaintext){
        if (isalpha(element))
            count_plain++;
    }//for each char in plaintext
    for (auto element: secret_message){
        if (isalpha(element))
            count_secret++;
    }//for each char in secret message
    if (count_plain >= count_secret*5)
        check= true;
    return check;
}//check_message

/*first converts both strings to lowercase then checks to see that the conditions
are met. next it goes through each character in secret message and converts
it to binary, then it goes through each character in the binary string
returned and determines whether it needs to capitlize the next alpha
character in plaintext based off wheter its a 0 or 1*/
string encode(const string& plaintext, const string& secret_message){
    string new_plaintext= lower_case(plaintext);
    string new_secret_message= lower_case(secret_message);
    string binary="";
    int plain_index=0;
    if (check_message(new_plaintext, new_secret_message)== false){
        return "Error";
    }//if check message gomes back true
    else
    {   for (auto element: new_secret_message){
            binary=to_binary(element);
            for (auto num: binary){
                while (isalpha(new_plaintext[plain_index])==false)
                        plain_index++;
                if (num == '1'){
                    new_plaintext[plain_index]= toupper(new_plaintext[plain_index]);
                }//if charachter is 1
                plain_index++;
            }//for each num in the binary string
        }//for each character in secret message
    }//if check message comes back true
    return new_plaintext;
}//function encode

/*the first while loop breaks down the string into substrings of 5 characters
and puts these strings into an array. Then the for loop goes through
each string in the array, checks to see its full size is 5, then creates
a string of 1's and 0's based on the capitilization. This binary string
is put into a different array. The last for loop goes through the vector
of binary strings and creates a string of the characters returned by each
binary string*/
string decode(const string& to_decode){
    string message="";
    vector<string> subs;
    vector<string> binaries;
    string sub="";
    int index=0;
    while (index < static_cast<int>(to_decode.size())){
        while (sub.size()<5 && index < static_cast<int>(to_decode.size())){
            if (isalpha(to_decode[index]))
                sub+=to_decode[index];
            index++;
        }//while the substring is less than 5
    subs.push_back(sub);
    sub=""; 
    }//while index is less than size of to_decode
    for (auto element: subs){
        if (element.size()==5){
            for (auto let: element){
                if (isupper(let))
                    sub+='1';
                else
                    sub+='0';
            }//for each letter in that sub
            binaries.push_back(sub);
            sub="";
        }//if the substring has 5 letters
    }//for each substring in subs
    for (auto binary: binaries){
        message+= from_binary(binary);
    }//for each binary substring in binaries
    return message;
}//decode method
