#include <iostream>
#include "com_cli.hpp"

int main()
{
    using namespace com_cli;
    using std::cout;
    using std::endl;

    init("CLItest");
    
    auto year = read_value<int>("Input INTEGER", 2020);
    cout << "This year is " << year << endl;
    
    auto pi = read_value<float>("Input REAL*4", 3.14);
    cout << "Pi = " << pi << endl;

    auto name = read_text("What is your name", "ipmu taro");
    cout << "You are " << name << endl;

    auto choise = read_keyword("Blood Type",
			       std::vector<std::string>{"A","B","O","AB"} );
    cout << "Your blood type is " << choise << endl;
}
