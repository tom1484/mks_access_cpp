#include <iostream>
#include <thread>

using namespace std;

thread th;
bool ended = true;

int main()
{

    while (true)
    {
        // if (ended)
        // {
        th.~thread();
        th = thread(
            [&]()
            {
                cout << "thread" << endl;
                ended = true;
            });
        th.detach();
        // }
    }

    return 0;
}