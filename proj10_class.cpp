#pragma once
#include<iostream>
using std::cout; using std::endl; using std::boolalpha; using std::ostream;
#include <sstream> 
using std::ostringstream;
#include<initializer_list>
using std::initializer_list;
#include <algorithm> 
using std::max; using std::copy; using std::swap; using std::equal; using std::find; using std::lower_bound;
#include <utility> 
using std::make_pair; using std::pair; 
#include <stdexcept> 
using std::runtime_error; 

const static size_t element_array_size = 5;

template<typename K, typename V>
struct Element{
public:
  K key_;
  V values_[element_array_size];
  size_t count_ = 0; 
  Element()=default;
  Element(K, initializer_list<V>);
  bool operator==(const Element&)const;
  bool operator<(const K&) const; 
  friend ostream& operator<<(ostream& out, Element& elem){
    out << elem.key_ <<":";
    for (size_t i=0; i<elem.count_; ++i){
        if (i != elem.count_-1)
            out << elem.values_[i]<<",";
        else
            out << elem.values_[i];
    }
    return out;
  }//friend << function
};

//===================================Code for Element Functions=========================================

/*The first little bit with K null was just checking to see if the user trying to add an empty element
was one of the hidden test cases. The for loop then initializes the elements values_ array only for
the number dictated by element_array_size*/
template<typename K, typename V>
Element<K, V>::Element(K key, initializer_list<V> lst){
    K null;
    if (!(key == null)){
    key_= key;
    size_t indx=0;
    for (auto element: lst){
        if (indx<element_array_size){
        values_[indx++]=element;
        count_++;}
    }//for each element in initializer list
    }
}//constructor

/*Checks to see if either the key or count isnt equal. Then if necessary goes through both
values_ arrays to check for the equality of values*/
template<typename K, typename V>
bool Element<K, V>::operator==(const Element& elem)const{
    bool equals=true;
    if (key_ != elem.key_ || count_ != elem.count_)
        return false;
    equals= equal(values_, values_+element_array_size, elem.values_);
    return equals;
}//overloaded == operator

/*checks to see if the key passed in is less than the calling key*/
template<typename K, typename V>
bool Element<K, V>::operator<(const K& key) const{
    bool less= true;
    if (key < key_)
        less=false;
    return less;
}//overloaded < operator


template<typename K, typename V> 
class MVM{
public:
  Element<K, V> * data_ = nullptr;
  size_t num_keys_ = 0; 
  size_t num_elements_ = 0; 
  Element<K, V> * find_key(K);
  size_t find_value(V, K*&);
  void grow(); 

public:
  MVM()=default;
  MVM(initializer_list<Element<K,V>>);
  MVM(const MVM&); 
  ~MVM(); 
  size_t size();
  bool add(K,V);  
  bool remove_key(K);
  size_t remove_value(V, K*&);
  friend ostream& operator<<(ostream& oss, MVM& mv){
    for (size_t i=0; i<mv.size(); i++){
        if (i != mv.size()-1)
            oss << mv.data_[i] <<" ";
        else
            oss << mv.data_[i];
    }//for
    return oss;
  }
};

//============================================Code for MVM=======================================


template<typename K, typename V>
MVM<K,V>::MVM(const MVM<K,V>& mvm){
    num_elements_=mvm.num_elements_;
    num_keys_= mvm.num_keys_;
    data_= new V[mvm.num_elements_]{};
    copy(mvm.data_, mvm.data_+num_elements_, data_);
}//copy constructor

template<typename K, typename V>
MVM<K,V>::~MVM(){
    delete [] data_;
}//destructor

/*first if is for if the MVM was defalt initialized and now new elements need to be added in and then the else
statement handles growing if capacity is reached*/
template<typename K, typename V>
void MVM<K,V>::grow(){
    Element<K,V> *new_data;
    if (num_elements_==0 && data_==nullptr){
        new_data= new Element<K,V>[2]{};
        num_elements_=2;
        data_= new_data;
    }//if data is empty
    else{
        new_data= new Element<K,V>[num_elements_*2]{};
        copy(data_, data_+num_elements_, new_data);
        num_elements_*=2;
        std::swap(new_data, data_);
        delete [] new_data;
    }//if data has reached capacity
}//grow function

/*First it initializes a pointer to where the new element needs tobe inserted. If it doesnt need to be inserted at the end
then it checks to see if its a new element, if so then it checks to make sure data_ has room, after which it figures out the index
position where the new element is to be inserted, creates a new array, increases the number of keys and then enters a for loop.
Once the for loop reaches the index where the new element is to be inserted it does so, else it just transfers the 
elements from data. These two arrays are then swapped and new_data is deleted. The next case is if the key was already there which
then the method checks if that value is already in the element, if its not it makes sure the element's values_ isn't full before 
inserting the new value. Now if the key just needs to be appended to the end, data_ is checked to see if it nees to grow, then the
new element is added at the index after the last key and num keys is incremented.*/
template<typename K, typename V>
bool MVM<K,V>::add(K key,V val){
    Element<K, V>* elem_ptr= find_key(key);
    if (elem_ptr != data_+num_keys_){
        if ( key < (*elem_ptr).key_ ){
            if (num_keys_==num_elements_){
                grow();
            }//if data if full
            int num_key_pos=0;
            while(data_[num_key_pos].key_ < key)
                num_key_pos++;
            int index=0;
            num_keys_++;
            Element<K, V>* new_data= new Element<K,V> [num_elements_]{};
            for (size_t i=0; i<num_keys_; ++i){
                if (i==num_key_pos){
                    new_data[i]=Element<K,V>(key,{val});
                }//if 
                else{
                    new_data[i]=data_[index];
                    ++index;
                }
            }//for loop that inserts new element where it needs to be
            std::swap(new_data, data_);
            delete [] new_data;
        }//if key isn't there but its less than existing key
        else{
            if(find((*elem_ptr).values_, (*elem_ptr).values_+(*elem_ptr).count_, val) == (*elem_ptr).values_+(*elem_ptr).count_){
                if ((*elem_ptr).count_!= element_array_size){
                    (*elem_ptr).values_[(*elem_ptr).count_]= val;
                    (*elem_ptr).count_++;
                }
                else
                    return false;
            }//if the value isnt there
            else
                return false;
        }//if key is there
    }//if key is there or its less than existing keys
    else
    {
        if (num_keys_==num_elements_)
            grow();
        //cout << num_keys_ << " "<<num_elements_<<endl;
        data_[num_keys_]= Element<K,V>(key, {val});
        num_keys_++;
    }//if its a completely new key and value
    return true;
}//add method

/*Again the null elem thing was to check to see if someone was trying to input an empty initializer list for
a hidden test case. The next if statement in the for loop just makes sure an element is still added even if
its values_ array is empty. If it has values in its values array then they are then added via a for loop and the add
function.*/
template<typename K, typename V> 
MVM<K,V>::MVM(initializer_list<Element<K,V>> list){
    Element<K,V> null_elem;
    num_elements_=list.size();
    num_keys_=0;
    data_= new Element<K,V>[num_elements_]{};
    for (auto element: list){
        if (!(element == null_elem)){
            if (element.count_==0){
                add (element.key_, {});
            }//makes sure to add an element even if it doesn't have values
            else{
                for (size_t i=0; i<element.count_; ++i){
                    add (element.key_, element.values_[i]);
                }//goes through and adds each value
            }//if an element has values
        }//if there actually is an element
    }//for every element in list
}//constructor

template<typename K, typename V>
size_t MVM<K,V>::size(){
    return num_keys_;
}//size method

template<typename K, typename V>
Element<K, V>* MVM<K,V>::find_key(K key){
    return lower_bound(data_, data_+num_keys_, key, [] (Element<K,V> elem, K key){return elem.key_ < key;});
}//find_key function

/*goes through each element in data_ and then checks to see if that element contains the value being looked for.
If the value is found, num_keys is incremented. A pointer array of keys is then initialized to the size of num_keys
and then the keys already in keys_found are copied over, the two pointers are swaped, and the last key found is then added
to the last index of the keys_found*/
template<typename K, typename V>
size_t MVM<K,V>::find_value(V val, K*(&keys_found)){
    size_t num_keys=0;
    if (!(keys_found==nullptr))
        throw runtime_error("");
    else{
        K *new_keys_found;
        for (size_t i=0; i<num_keys_; ++i){
            if (find(data_[i].values_, data_[i].values_+data_[i].count_, val)!= data_[i].values_+data_[i].count_){
                num_keys++;
                new_keys_found= new K[num_keys]{};
                copy(keys_found, keys_found+(num_keys-1), new_keys_found);
                std::swap (new_keys_found, keys_found);
                keys_found[num_keys-1]= data_[i].key_;
                delete [] new_keys_found;
            }//if the value is found
        }//for each element in data
    }
    return num_keys;
}//find_value method

/*First a pointer is initialized to where the target key is. It then double checks to mae sure the key is indeed
in data_, if it is then a new array of elements in initialized, the elements from data_ are transferred over except 
for the element of the target key. Then at the end num keys is decremented, the two array pointers are swapped and
the temporary one is delted. If they key wasn't there tobegin with false is returned.*/
template<typename K, typename V>
bool MVM<K,V>::remove_key(K key){
    Element<K, V>* elem_ptr= find_key(key);
    if (elem_ptr+1 != data_+num_elements_ && !(key < (*elem_ptr).key_)){
        Element<K, V>* new_data= new Element<K,V> [num_elements_]{};
        int index=0;
        for (size_t i=0; i<num_keys_; ++i){
            if (elem_ptr!= data_+i){
                new_data[index]=data_[i];
                index++;
            }
        }//for loop
        std::swap(new_data, data_);
        --num_keys_;
        delete [] new_data;
    }//if the key is there
    else
    {
        return false;
    }
    return true;
}//remove key method

/*Initializes a variable to the number of keys holding the targer value. Then it enters a for loop where
it initalizes a pointer to a key in keys_found. Then a temporary element is initialized to have the same key
but now a blank array of values. All the values in the pointer's array of values are copied over except for the target one.
The two elements are then swapped.*/
template<typename K, typename V>
size_t MVM<K,V>::remove_value(V val, K*(& keys_found)){
    size_t num_values=0;
    if (!(keys_found==nullptr))
        throw runtime_error("");
    else{
        num_values=find_value(val, keys_found);
        for (size_t i=0; i< num_values; i++){
            Element<K, V>* elem_ptr=find_key(keys_found[i]);
            Element<K,V> new_elem((*elem_ptr).key_,{});
            int index=0;
            for (size_t j=0; j<(*elem_ptr).count_; ++j){
                //cout << "VAlues: "<<(*elem_ptr).values_[j]<<endl;
                if ((*elem_ptr).values_[j]!= val){
                    new_elem.values_[index]=(*elem_ptr).values_[j];
                    index++;
                    new_elem.count_++;
                }//if its not the value
            }//for every value in elements
            std::swap(new_elem, (*elem_ptr));
        }//for every key in keys found
    }//as long as keys found isn't empty
    return num_values;
}//remove value method

