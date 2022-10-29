#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <forward_list>

#include "serializer.hpp"

using namespace std;

int main() {
    ofstream ofs("test.ser", ofstream::out | ofstream::binary);

    int a = 10;
    string stst("Test! And test, and test..");
    vector<string> vtst{ "This is 1!", "This is 2!", "This is 3!", "This is not 1!" };
    map<int, string> mtst{ {1, "This is mapped_1!"}, {2, "This is mapped_2!"}, {3, "This is mapped_3!"}};
    forward_list<char> ltst{ 'a', 'b', 'c' };

    serialize(a, ofs);
    serialize(stst, ofs);
    serialize(vtst, ofs);
    serialize(mtst, ofs);
    serialize(ltst, ofs);

    ofs.close();

    int _a = 0;
    string nstst;
    vector<string> nvtst;
    map<int, string> nmtst;
    forward_list<char> nltst;

    ifstream ifs("test.ser", ifstream::in | ofstream::binary);
    ifs >> noskipws;
    
    deserialize(_a, ifs);
    deserialize(nstst, ifs);
    deserialize(nvtst, ifs);
    deserialize(nmtst, ifs);
    deserialize(nltst, ifs);


    if (_a == a)
        cout << "int OK!" << endl;

    if (stst == nstst)
        cout << "Str OK!" << endl;

    if (nvtst == vtst)
        cout << "Vect OK!" << endl;

    if (nmtst == mtst)
        cout << "Map OK!" << endl;

    if (nltst == ltst)
        cout << "List OK!" << endl;

    return 0;
}