#include <iostream>

#include "dual_number.hpp"

using namespace std;

using val = dual_number<double>;

struct ryuunen {
    constexpr ryuunen(int &a) : a(a) { }
    int &a;
};

constexpr bool hoge() {
    int A = 10;
    int B = 20;

    ryuunen a(A);
    ryuunen b(A);

    return &a.a == &b.a;
}

int main() {
    val res = val(10, 10) * val(20, 20) + val(2, 0);
    cout << res.value() << endl;
    cout << res.diff() << endl;

    static_assert(hoge(), "reference pointer equal");

    constexpr int q = 10;
    constexpr int p = 10;
    constexpr auto e = (&q == &q);
    constexpr auto n = (&q == &p);

    static_assert(e, "not equal");
    static_assert(!n, "equal");

    return 0;
}

