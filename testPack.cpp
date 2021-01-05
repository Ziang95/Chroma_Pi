#include <vector>
#include <string>
#include <iostream>

using namespace std;


void printHello(string s)
{
    cout << s << endl;
}

class tes
{
public:
    template<typename Callable, typename... Args>
    void testPack(Callable &&f, Args&&... args)
    {
        // f(args...);
    }
};

int main()
{
    tes ttet;
    ttet.testPack(printHello);
}