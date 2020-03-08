/**
   @date 2020/03/08
**/

#include <iostream>

using std::cout;
using std::endl;

int hello()
{
    cout << "hi" << endl;
    return 0;
}

#include <pybind11/pybind11.h>

PYBIND11_MODULE(hi, m)
{
    m.doc() = "This is a test!!";
    m.def("hello", &hello, "greeting");
}
