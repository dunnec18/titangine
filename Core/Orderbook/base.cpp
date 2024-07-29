#include <iostream>
#include <vector>

//using namespace std;

auto get_first(std::vector<int> & c)
{
    return c[0];
}

int main() { 
    std::vector<int> col(10000000);
    for(int i = 0; i < 100; i++)
        get_first(col);
}