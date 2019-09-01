#include <iostream>
using std::ostream; using std::cout; using std::endl;
#include <sstream>
using std::ostringstream;
#include <vector>
using std::vector;
#include <string>
using std::string; using std::isalpha; using std::tolower; using std::to_string;
#include <fstream>
using std::ifstream; using std::basic_ifstream;
#include <iterator>
using std::iterator;
#include <map>
using std::map; using std::find;
#include <algorithm>
using std::set_intersection;
#include <cmath>
#include "proj07_functions.h"

/*This functions takes each element in the vector and adds them to a 
blank string along with a comma, then at the end, the last
comma is taken off, the string is placed in the ostream and the 
ostream is returned*/
ostream& print_vector (ostream &out, const vector<string> &v){
    string print="";
    for (auto element: v){
        print+=element;
        print+=",";
    }//for each string in the vector
    print= print.substr(0, print.size()-1);
    out << print;
    return out;
}//print_vector

/*the functions goes through each character in the string
if it's an alphabetic character its converted to lower case and added
to a blank string. This string is then returned*/
string clean_word(const string& s){
    string clean="";
    for (auto element: s){
        if (isalpha(element))
            clean+=tolower(element);
    }//for each character in the string
    return clean;
}//clean_word

/*the functions goes through each character in the string, if it's not
equal to the separating character then its added to a string called word
if it is a separating character then word is added into a vector before
being reinitialized to blank string in preparation for the next word*/
vector<string> split(const string &s, char delim){
    vector<string> words;
    string word="";
    for (auto element :s){
        if (element != delim){
            word+=element;
        }//if its a char other than a blank space
        else
        {
            words.push_back(word);
            word="";
        }//if it hits a separator character
    }//for each char in the string
    words.push_back(word);
    return words;
}//split

/*first the functions passes the file indicated by the string into
the input file stream, then it checks to make sure that the file was 
successfully opened. Then each line in the file is sent off to the
split functions so that it can be separated into its respective words.
Then each word in the vector is cleaned by the clean_word fucntion. Once
its cleaned, the map passed in is searched to see if it already contains
this word. If it doesn't, the word is used as a key and its value is set
to one. If the it is found, the value for that key is incremented.*/
bool process_words(map_type& m, string s){
    ifstream in_file(s);
    string line;
    vector<string>v;
    string word;
    map_type::iterator itr;
    if (in_file.is_open()){
        while(getline(in_file, line)){
            v= split(line);
            for (auto element: v){
                word= clean_word(element);
                if (m.find(word)==m.end()){
                    m[word]=1;
                }//if the word is not already in the map
                else{
                    itr=m.find(word);
                    ++(*itr).second;
                }//if the word is in the map increase its value by 1
            }//for each word in the line of input
        }//while there is still lines in the file to get
        return true;
    }//if the file was successfully opened
    else{
        return false;
    }//if the file couldn't be opened
}//proces_words

/*Each element of the map is grabbed, then its key is added to an empty
string, then a colon, then its value is added, then a comma. At the end
the last comma is taken off of the string before its passed to the ostream
and the ostream is then returned*/
ostream& print_map(ostream& out, const map_type& m){
    string print="";
    for (auto element: m){
        print+=element.first;
        print+=":";
        print+=to_string(element.second);
        print+=",";
    }//for each string in the vector
    print= print.substr(0, print.size()-1);
    out << print;
    return out;
}//print_map

/*first the keys from each map are both put in separate vectors. Then 
the set intersection function is used on these two vectors where the elements
they both have in common are input into a vector which is then resized
to be the appropriate size for its contents. The jaccard similarity is then
calclulated using the provided equations and using the size of each vectors
to get the correct numbers*/
double jaccard_similarity(const map_type &a, const map_type &b){
    vector<string> one;
    vector<string> two;
    vector<string> joined(10);
    vector<string>::iterator itr;
    double similarity=0;
    for (auto element: a){
        one.push_back(element.first);
    }//for each element in first map
    for (auto element: b){
        two.push_back(element.first);
    }//for each element in second map
    itr= set_intersection(one.begin(), one.end(), two.begin(), two.end(), 
        joined.begin());
    joined.resize(itr-joined.begin());
    similarity= static_cast<double>(joined.size())/static_cast<double>((one.size()+
        two.size()-joined.size()));
    return similarity;
}//jaccard_simlarity

/*each the value of each element in the map is squared and then added
to a double called norm_factor. The square root of norm_factor is returned*/
double calc_norm_factor(const map_type &m){
    double norm_factor=0;
    for (auto element: m){
        norm_factor+=pow(element.second,2);
    }//for each element in the map
    return sqrt(norm_factor);
}//calc_norm_factor

/*the keys of each map are but in their separate vectors and then a vector
of the elements they have in common are created using the same logic as
in the jaccard_similarity function. Next the method iterates through the 
vector of common elements, finds the value held at that element in each map
dividing it by the norm factor calculated via the norm_factor functions
then this value is returned*/
double cosine_similarity_tf(const map_type &a, const map_type &b){
    vector<string> one;
    vector<string> two;
    vector<string> joined(10);
    vector<string>::iterator itr;
    map_type::iterator it;
    double similarity=0;
    for (auto element: a){
        one.push_back(element.first);
    }//for each element in first map
    for (auto element: b){
        two.push_back(element.first);
    }//for each element in second map
    itr= set_intersection(one.begin(), one.end(), two.begin(), two.end(), 
        joined.begin());
    joined.resize(itr-joined.begin());
    for (auto element: joined){
        similarity+= (static_cast<double>(a.find(element)->second)/calc_norm_factor(a))
            *(static_cast<double>(b.find(element)->second)/calc_norm_factor(b));
    }//for each word that the two documents share
    return similarity;
}//cosine_similarity_tf