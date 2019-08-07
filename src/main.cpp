#include <iostream>

#include "dual_number.hpp"

using namespace std;

using val = dual_number<double>;

int main() {
    val res = val(10, 10) * val(20, 20) + val(2, 0);
    cout << res.value() << endl;
    cout << res.diff() << endl;

    return 0;
}

