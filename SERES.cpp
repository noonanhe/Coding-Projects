#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
using std::stringstream;
using std::cout; using std::endl;
using std::ifstream; using std::ofstream;
using std::vector; 
using std::string;
using std::ctime;

/**
 * This is part of a project I am currently working on now in Dr. Liu's research laboratory.
 * The code is not complete but it's a little taste of what I am doing.
 */

//=======================================Alignment Class=============================================
/**
 * For statistical support, a DNA sequence alignment needs to be resampled. This class desribes a
 * single alignment. An alignment consists of multiple DNA sequences of different individuals lined
 * up so the columns represent the same location on a strand of DNA
 */
struct Alignment{
public:
    ///Experiment simulations that i ran gave me multiple alignments. Sim_number represent which alignment
    ///i was on for that particular simulation parameters.
    int sim_number;
    ///vector of original sequences
    vector<vector<char>> sequences;
    ///vector of realigned sequences
    vector<vector<char>> realigns;
    ///the label the sequence generator gave to the sequences
    vector<int> labels;
    int sequence_length;
    Alignment()=default;
    void sequence_walk(double probability);
    int startTime();
    bool prob_reversal(double probability);
};

/**
 * generates a random number between 0 and the sequence length
 */
int Alignment::startTime(){
    int r= rand()%sequence_length;
    return r;
}//startTime

/**
 * \param probability of reversal (user determined)
 * 
 * a random number between 1-100 is generated, if random number is less than or equal to the 
 * probability then a bool value of true is returned if it is greater then false value is returned
 */
bool Alignment::prob_reversal(double prob){
    int r= rand()%100;
    if (r <= prob*100){
        return 1;
    }//if r is less than prob number
    else
        return 0;
}//prob_reversal

/**
 * \param probabilty (user input)
 * 
 * this function creates a realignment by walking down an alignment and turning around if the 
 * prob_reversal function returns a value of true, or it hits the end/beginning of a sequence. The idea of
 * doing a controlled walk with random reversals is to hopefully provide a more statistically significant
 * DNA realignment because it preserves neighbors 
 */
void Alignment::sequence_walk(double probability){
    int position= startTime();
    bool direction=rand()%2;
    vector<char>temp(sequence_length,' ');
    for (auto element: sequences){
        realigns.push_back(temp);
    }//creating an empty temporary array
    int index=0;
    while(index<=sequence_length){
        for (size_t i=0; i<sequences.size(); i++){
            realigns[i][index]=sequences[i][position];
        }//for loop to put letters places
        if (position == sequence_length-1 || position==0 || prob_reversal(probability)){
            direction= !direction;
        }//changes direction if it hits either end or probabilty reversal returns1
        if (direction ==0){
            position++;
        }//if 0 travels to the right
        else
            position--;
        index++;
    }//while loop
}//sequence_walk

//=====================================General Code=================================================
vector<vector<char>> alignment;
vector<vector<char>> realignment;
vector<int> identifiers;
vector<Alignment*> simulations;

void print_sequence(vector<char> seq){
    for (auto let: seq){
        cout<<let;
    }
}

/**
 * this function is desinged to read in phylip files generated by sequence generating programs
 * for this particular experiment I was using INDELible. Each seqences has an identifier at the beginning
 * so when reading that has to be separated out from the actual DNA sequence
 */
bool readfile(string f_name){
    string identifier="";
    string sequence="";
    int index=1;
    vector<char> temp; 
    ifstream in_file(f_name);
    if (in_file.is_open()){
        while(getline(in_file, sequence)){
            if (sequence.length()>10){
                identifier="";
                for (size_t i=0; i< sequence.length()-1; i++){
                    while (isdigit(sequence[i])){
                        identifier+=sequence[i];
                        i++;
                    }
                    //identifiers.push_back(stoi(identifier));
                    if(sequence[i]!=' ')
                        temp.push_back(sequence[i]);
                }//for loop                
                identifiers.push_back(stoi(identifier));
                alignment.push_back(temp);
                temp.erase(temp.begin(), temp.end());
            }//if we aren't on the first line
            index++;
        }//wjile loop
        return true;
    }//if file opened
    else
    {
        return false;
    }
    
}//readfile

/**
 * The sequence generations I got from the program INDELible gave me 20 different alignments based on
 * the same input paramters. This function was designed to take all the sequences i read in and separate
 * them into their respective alignments.
 */
void separate(){
    int total=0;
    int index=0;
    Alignment* new_alignment;
    while(total<20){
        new_alignment= new Alignment();
        new_alignment->sim_number = (total+1);
        while(index<10){
            new_alignment->sequences.push_back(alignment[(total*10)+index]);
            new_alignment->labels.push_back(identifiers[(total*10)+index]);
            index++;  
        }//while loop putting 10 sequences into a new Alignment
        new_alignment->sequence_length= new_alignment->sequences[0].size();
        simulations.push_back(new_alignment);
        index=0;
        total++;
    }//while loop counting to 20 Alignment creations
    cout<<simulations.size()<<endl;
}//separate function

/**
 * the experiment simulations i ran were under an indel model meaning dashes were inserted when insertion
 * and deletion events happened. The dashes were left in for the sequence walk, but this function was meant
 * to take them out when writing the new realignments into their own file
 */
void remove_dashes(vector<vector<char>> sequence_group){
    cout<<"Here"<<endl;
    vector<char> temp;
    cout<<"Alignment group size: "<<sequence_group.size()<<endl;
    for (size_t seq_index=0; seq_index<sequence_group.size(); seq_index++){
        for (auto letter: sequence_group[seq_index]){
            if (letter!='-'){
                temp.push_back(letter);
            }//if the letter isn't a dash
        }//for every character in a sequence
        //print_sequence(temp);
        //cout<<"Before: "<<&sequence_group[seq_index]<<&temp;
        std::swap(sequence_group[seq_index], temp);
        //cout<<"\tAfter: "<<&sequence_group[seq_index]<<&temp<<endl;
        temp.erase(temp.begin(),temp.end());
    }//for each sequence in an alignment
}//remove_dashes method 

/**
 * writes the realignments into their own text file
 */
void write_normal_file(string f_name, Alignment* align){
    ofstream out_file;
    out_file.open(f_name);
    out_file<<10<<" "<<align->sequence_length<<endl;
    for (size_t seq_index=0; seq_index<align->labels.size(); seq_index++){
        out_file << align->labels[seq_index]<<" ";
        for (auto element: align->realigns[seq_index]){
            out_file << element;
        }//for every character in a sequence
        out_file << endl;
    }//for every sequence
}//write_file


//=================================================Main ========================================

/**
 * there is still lots of commented out code in this main because I am currently using it to test
 * different simulation parameters and to read in different files.
 */
int main(){
    srand(time(0));
    readfile("output_10B_TRUE.phy");
    separate();
    //simulations[0]->sequence_walk(.25);
    std::cout.clear();

    cout<<simulations.size()<<endl;


    for (int i=0; i<simulations.size(); i++){
        simulations[i]->sequences.size();
    }

    //write_normal_file("10A_normal.phy", simulations[0]);

    /*for (size_t i=0; i<simulations.size(); i++){
        stringstream name;
        name <<"10B_normal"<<i<<".phy";
        cout<<name.str()<<endl;

        simulations[i]->sequence_walk(.5);
        write_normal_file(name.str(), simulations[i]);
    }*/

    /* cout<< "Original Alignment: "<<endl;
    for (auto element: simulations[0]->sequences){
        print_sequence(element);
        cout<<endl;
    }
    cout<<endl;
    cout<<"Realigned Alignment: "<<endl;
    for (auto element: simulations[0]->realigns){
        print_sequence(element);
        cout<<endl;
    }
    cout<<endl;
    cout<<"Without Dashes: "<<endl;
    remove_dashes(simulations[0]->realigns);
    for (auto element: simulations[0]->realigns){
        print_sequence(element);
        cout<<endl;
    }*/

    }//end of main
