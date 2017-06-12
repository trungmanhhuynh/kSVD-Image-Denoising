#include <iostream>
#include <vector>
#include <iterator>
using namespace std;

int main()
{
    vector<float> v_int;
    for (int i=0; i<10; ++i) {
        v_int.push_back(1.123);
    }
    vector<unsigned char> v_float(v_int.begin(), v_int.end());

    copy (v_float.begin(), v_float.end(), ostream_iterator<double>(cout, " "));
    cout << endl;
    return 0;
}
