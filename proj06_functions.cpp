#include <vector>
using std::vector;
#include <iostream>
using std::cout; using std::endl;
#include <cmath>
using std::abs;
#include <sstream>
using std::ostringstream;
#include <ostream>
using std::ostream;
#include <string>
using std::string;

/*this takes in a vector and prints out each element to the o stream
before converting the ostream to a string and returning it*/
ostream& print_vector(const vector<long>& v, ostream& o){
ostringstream oss;
string result="";
    for (auto element: v){
        oss << element << ",";
    }//for each element in the vector
result= oss.str().substr(0, oss.str().size()-1);
o << result;
return o;
}//print vector method

/*takes the four corners of a 2D matrix by converting the index position
it would occupy in a 2D matrix and converting it into what it's index position would be 
would be in just a vector array. It then gathers the numbers at these indeces
and adds them together*/
long four_corner_sum(const vector<long>& v, int row, int col){
    if (row <2 || col <2)
        return 0;
    else{
        return v[0]+v[col-1]+v[(row-1)*col]+v[(row-1)*col+(col-1)];
    }//as long as theres at least 2 rows and columns
}//method four_corner_sum

/*first the function checks to make sure the vector is of a decent size. then
it goes through each element in the vector and calculates its row index. After
finding that row index, it checks to see if its less than one, and if so it 
changes the value to the last row index, the same process is repeated for
the column index. It then uses these new calculated 2D indeces to find the
new position when converted to a vector. It then adds that element to that 
position in the new vector before returning it.*/
vector<long> rotate_rows_up(const vector<long>& v, int row, int col){
    int r; int c; int index;
    vector<long> rotated(row*col);
    if (row <2 || col <2){
        rotated.clear();
        return rotated;
    }//if theres less than 2 columns or rows
    else {
        for (size_t i=0; i<v.size(); i++){
            r= (i/col)-1;
            if (r <0){
                r=row-1;
            }//if a number is in the first row
            c= i%col;
            index= ((r)*(col)+c);
            rotated[index]=v[i];
        }//for each element in the vector passed in
    }//as long as theres more than 2 columns or rows
    return rotated;
}//method rotate_rows_up

/*first it checks to make sure the vector is of adequate size, then it finds
the would be indeces of each element in the vector if the vector were to be converted
into a 2D array, but instead it calculates what would be the row index, and
makes it the column index and vice versa. Then it caculates the new 1D position from 
the 2D positions, but instead of multiplying by the number of columns
as would normally take place in ths conversion, the number of rows is used
as rows will now be the columns. It then assigns the elment into the new vector
using this index*/
vector<long> column_order(const vector<long>& v, int row, int col){
    int r; int c; int index; int dif= abs(row-col);
    vector<long> col_order((row*col));
    if (row <2 || col <2){
        col_order.clear();
        return col_order;
    }//if theres less than 2 columns or rows
    else {
        for (size_t i=0; i<v.size(); i++){
            c= (i/col);
            r= i%col;
            index= ((r*row)+c);
            col_order[index]=v[i];
        }//for each element in the vector passed in
    }//as long as theres more than 2 columns or rows
    return col_order;
}//method col_order

/*first we convert the vector into a 2D matrix. Then we travel through the
2D matrix starting from the last column and going down each row, adding each
element into a new vector*/
vector<long> matrix_ccw_rotate(const vector<long>& v, int row, int col){
    vector <long> rotated_ccw;
    vector<vector<long>> rotated;
    vector<long> rows;
    int index=0;
    for (int r=0; r<row; r++){
        for (int c=0; c<col; c++){
            rows.push_back(v[index]);
            index++;
        }//fills in each column
        rotated.push_back(rows);
        rows.clear();
    }//fills in each row
    //doing the actual rotating        
    for (int c=col-1; c>=0; c--){
        for (int r=0; r<row; r++){
            rotated_ccw.push_back(rotated[r][c]);
        }//goes down a column
    }//reads down each colmn starting from the last
    return rotated_ccw;
}//method rotate_ccw

/*goes through the vector, then it figures out which column each element 
is in by finding the remainder when dividing by number of cols, it then 
adds this element into that index of a vector of sums based on column index
then a for loop goes through each element of the sums vector, finds the largest
element and returns the index of that element*/
long max_column_diff(const vector<long>& v, int row, int col){
    int c; int max_dif=0;
    vector<long> sums(col);
    for (size_t i=0; i <v.size(); i++){
        c=i%col;
        sums[c]+=v[i];
    }//adds up all the columns
    for (size_t i= 0; i<sums.size(); i++){
        int index=i+1;
        while(index< sums.size()){
            if (abs(sums[i]-sums[index])>max_dif){
                max_dif= abs(sums[i]-sums[index]);
            }//if greated than max_sum
            index++;
        }//while that goes through all elements
    }//for each element in vector sums
    return max_dif;
}//method max_column_diff

/*so first we put this vector into a 2D matrix, then we travel through
each element in the matrix and if the top, bottom, right, and left are larger
than the element at that position, count increases*/
long trapped_vals(const vector<long>& v, int rows, int cols){
    vector<vector<long>> two_Di;
    vector<long> row;
    int index=0;
    int count=0;
    for (int r=0; r<rows; r++){
        for (int c=0; c<cols; c++){
            row.push_back(v[index]);
            index++;
        }//fills in each column
        two_Di.push_back(row);
        row.clear();
    }//fills in each row
    for (int r=0; r<rows; r++){
        for (int c=0; c<cols; c++){
            if (two_Di[r][(c+1)%cols]> two_Di[r][c])
                if (two_Di[r][(c+cols-1)%cols]> two_Di[r][c])
                    if (two_Di[(r+1)%rows][c]> two_Di[r][c])
                        if (two_Di[(r+rows-1)%rows][c] > two_Di[r][c])
                            count++;

        }//for each element in a row
    }//for each row
    return count;
}//finds trapped values
