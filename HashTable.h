#ifndef __HASHTABLE__
#define __HASHTABLE__

#include "HashUtils.h"
#include "ItemNotFoundException.h"
/* Do not add new libraries or files */

#define BUCKET_SIZE 2

// Do not modify the public interface of this class.
// Otherwise, your code will note compile!
template <class K, class T>
class HashTable {
    struct Entry {
        K Key;             // the key of the entry
        T Value;   // the value of the entry
        bool Deleted;        // flag indicating whether this entry is deleted
        bool Active;         // flag indicating whether this item is currently used

        Entry() : Key(), Value(), Deleted(false), Active(false) {}
    };

    struct Bucket {
        Entry entries[BUCKET_SIZE];
    };

    int _capacity; // INDICATES THE TOTAL CAPACITY OF THE TABLE
    int _size; // INDICATES THE NUMBER OF ITEMS IN THE TABLE

    Bucket* _table; // THE HASH TABLE

    // == DEFINE HELPER METHODS & VARIABLES BELOW ==


public:
    // TODO: IMPLEMENT THESE FUNCTIONS.
    // CONSTRUCTORS, ASSIGNMENT OPERATOR, AND THE DESTRUCTOR
    HashTable();
    // COPY THE WHOLE CONTENT OF RHS INCLUDING THE KEYS THAT WERE SET AS DELETED
    HashTable(const HashTable<K, T>& rhs);
    HashTable<K, T>& operator=(const HashTable<K, T>& rhs);
    ~HashTable();

    // TODO: IMPLEMENT THIS FUNCTION.
    // INSERT THE ENTRY IN THE HASH TABLE WITH THE GIVEN KEY & VALUE
    // IF THE GIVEN KEY ALREADY EXISTS, THE NEW VALUE OVERWRITES
    // THE ALREADY EXISTING ONE. IF THE LOAD FACTOR OF THE TABLE IS GREATER THAN 0.6,
    // RESIZE THE TABLE WITH THE NEXT PRIME NUMBER.
    void Insert(const K& key, const T& value);

    // TODO: IMPLEMENT THIS FUNCTION.
    // DELETE THE ENTRY WITH THE GIVEN KEY FROM THE TABLE
    // IF THE GIVEN KEY DOES NOT EXIST IN THE TABLE, THROW ItemNotFoundException()
    void Delete(const K& key);

    // TODO: IMPLEMENT THIS FUNCTION.
    // IT SHOULD RETURN THE VALUE THAT CORRESPONDS TO THE GIVEN KEY.
    // IF THE KEY DOES NOT EXIST, THROW ItemNotFoundException()
    T& Get(const K& key) const;

    // TODO: IMPLEMENT THIS FUNCTION.
    // AFTER THIS FUNCTION IS EXECUTED THE TABLE CAPACITY MUST BE
    // EQUAL TO newCapacity AND ALL THE EXISTING ITEMS MUST BE REHASHED
    // ACCORDING TO THIS NEW CAPACITY.
    // WHEN CHANGING THE SIZE, YOU MUST REHASH ALL OF THE ENTRIES
    void Resize(int newCapacity);

    // THE IMPLEMENTATION OF THESE FUNCTIONS ARE GIVEN TO YOU
    // DO NOT MODIFY!
    int Capacity() const; // RETURN THE TOTAL CAPACITY OF THE TABLE
    int Size() const; // RETURN THE NUMBER OF ACTIVE ITEMS
    void getKeys(K* keys); // PUT THE ACTIVE KEYS TO THE GIVEN INPUT PARAMETER
};


template <class K, class T>
HashTable<K, T>::HashTable() {
    _capacity=7;
    _size=0;
    _table=new Bucket[_capacity];
    
}

template <class K, class T>
HashTable<K, T>::HashTable(const HashTable<K, T>& rhs) {
        _capacity=rhs._capacity;
        _table=new Bucket[_capacity];
        _size=rhs._size;
        for(int i=0;i<_capacity;i++){
            _table[i].entries[0]=rhs._table[i].entries[0];
            _table[i].entries[1]=rhs._table[i].entries[1];
            
        }
    
}

template <class K, class T>
HashTable<K, T>& HashTable<K, T>::operator=(const HashTable<K, T>& rhs) {
    if( this != &rhs){
        int s=rhs._capacity;
        delete[] _table;
        _table=new Bucket[s];
        _capacity=rhs._capacity;
        _size=rhs._size;
        for(int i=0;i<s;i++){
            _table[i].entries[0]=rhs._table[i].entries[0];
            _table[i].entries[1]=rhs._table[i].entries[1];
            
        }
        
    }
    
}

template <class K, class T>
HashTable<K, T>::~HashTable() {
    delete[] _table;
    _table=NULL;
    _capacity=0;
    _size=0;
    
}

template <class K, class T>
void HashTable<K, T>::Insert(const K& key, const T& value) {
    int hash=Hash(key);
    if(float(_size)/_capacity>1.2){
        Resize(NextCapacity(_capacity));
        
    }
    int place=hash%_capacity;
    int prob=1;
    bool check=true;
    do{
        if(_table[place].entries[0].Active==false||_table[place].entries[0].Active==true && _table[place].entries[0].Key==key){
            if(_table[place].entries[0].Active==false){
                _size++;
            }
            _table[place].entries[0].Key=key;
            _table[place].entries[0].Value=value;
            _table[place].entries[0].Active=true;
            _table[place].entries[0].Deleted=false;
            check=false;
        }
        else if(_table[place].entries[1].Active==false||_table[place].entries[1].Active==true && _table[place].entries[1].Key==key){
            if(_table[place].entries[1].Active==false){
                _size++;
            }
            _table[place].entries[1].Key=key;
            _table[place].entries[1].Value=value;
            _table[place].entries[1].Active=true;
            _table[place].entries[1].Deleted=false;
            check=false;
        }
        else{
            place=hash+prob*prob;
            place=place%_capacity;
            prob++;
        }
    }
    while(check);
    
  
}

template <class K, class T>
void HashTable<K, T>::Delete(const K& key) {
    int hash=Hash(key);
    int place=hash%_capacity;
    bool check=true;
    int prob=1;
    do{
        if(_table[place].entries[0].Active==true &&_table[place].entries[0].Key==key){
               _table[place].entries[0].Active=false;
               _table[place].entries[0].Deleted=true;
               check=false;
               _size--;
               break;
        }
        
        if(_table[place].entries[1].Active==true &&_table[place].entries[1].Key==key){
               _table[place].entries[1].Active=false;
               _table[place].entries[1].Deleted=true;
               check=false;
               _size--;
               break;
        }
           
        else if(_table[place].entries[1].Active==false||_table[place].entries[0].Active==false){
            throw ItemNotFoundException ( );
            break;
            
        }
        
        else{
            place=hash+prob*prob;
            place=place%_capacity;
            prob++;
        }
    }
    while(check);
    
  
}
    
    
    

template <class K, class T>
T& HashTable<K, T>::Get(const K& key) const {
    int hash=Hash(key);
    int place=hash%_capacity;
    bool check=true;
    int prob=1;
    do{
        if(_table[place].entries[0].Active==true &&_table[place].entries[0].Key==key){
          
               return _table[place].entries[0].Value;
        }
        if(_table[place].entries[1].Active==true && _table[place].entries[1].Key==key){
          
                return _table[place].entries[1].Value;
           }
        else if(_table[place].entries[1].Active==false||_table[place].entries[0].Active==false){
            throw ItemNotFoundException ( );
            break;
        }
        else{
            place=hash+prob*prob;
            place=place%_capacity;
            prob++;
        }
    }
    while(check);
    
}


template <class K, class T>
void HashTable<K, T>::Resize(int newCapacity) {
    Bucket *table_ex=new Bucket[_capacity];
   for(int i=0;i<_capacity;i++){
            table_ex[i].entries[0]=_table[i].entries[0];
            table_ex[i].entries[1]=_table[i].entries[1];
            
        }
     delete[] _table;
     int new_capacity=NextCapacity(_capacity);
     _table=new Bucket[new_capacity];
     for(int i=0;i <_capacity;i++){
         for(int x=0;x<2;x++){
             if(table_ex[i].entries[x].Active==true){
                 int hash=Hash(table_ex[i].entries[x].Key);
                 int place=hash%new_capacity;
                 int prob=1;
                 bool check=true;
                 do{
                     if(_table[place].entries[0].Active==false){
                     _table[place].entries[0]=table_ex[i].entries[x];
                         check=false;
                     }
                     else if(_table[place].entries[1].Active==false){
                         _table[place].entries[1]=table_ex[i].entries[x];
                         check=false;
                     }
                     else{
                         place=hash+prob*prob;
                         place=place%new_capacity;
                         prob++;
                     }
                 }
                 while(check);
             }
         }
     }
     _capacity=new_capacity;
     delete[] table_ex;
    
}


template <class K, class T>
int HashTable<K, T>::Capacity() const {
    return _capacity;
}

template <class K, class T>
int HashTable<K, T>::Size() const {
    return _size;
}


template <class K, class T>
void HashTable<K, T>::getKeys(K* keys) {
    int index = 0;

    for (int i = 0; i < _capacity; i++) {
        Bucket& bucket = _table[i];
        for (int j = 0; j < BUCKET_SIZE; j++) {
            if (bucket.entries[j].Active && !bucket.entries[j].Deleted) {
                keys[index++] = bucket.entries[j].Key;
            }
        }
    }
}

#endif