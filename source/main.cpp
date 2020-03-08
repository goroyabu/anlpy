/**
   @date 2020/03/08
**/

#include <iostream>
#include <com_cli.hpp>
#include <evs.hpp>

using std::cout;
using std::endl;

int main()
{
    std::string hi = "hi";
    com_cli::to_upper(&hi);
    cout << hi << endl;

    evs::EvsNdf();
    return 0;
}
