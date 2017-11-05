#ifndef GENERIC_H_INCLUDED
#define GENERIC_H_INCLUDED

#include<vector>

//moves an element from one vector to another
template<class c>
void transfer(std::vector<c>& srcList,std::vector<c>& destList,int index=0){
    destList.push_back(srcList[index]);
    srcList.erase(srcList.begin()+index);
}

//exchanges two variables
template<class c>
void exchange(c& e1,c& e2){
    c tempElem=e1;
    e1=e2;
    e2=tempElem;
}

#endif // GENERIC_H_INCLUDED