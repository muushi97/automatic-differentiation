#include <iostream>
#include <type_traits>
#include <tuple>
#include <cmath>

#include "autodiffer.hpp"

using namespace autodiffer;

using std::cout;
using std::endl;

// 型確認用
template <typename T>
struct type_detector;
template <int T>
struct nontype_detector;

int main() {
    //{
    //    double a = 10;
    //    double b = 20;
    //    double c = 30;

    //    double pp = 3.14159265359 * 0.5;

    //    value A(a);
    //    value B(b);
    //    value C(c);

    //    value PP(pp);

    //    a = 5.0;
    //    b = 3.0;
    //    c = 9.0;

    //    value D(11.0);
    //    value E(22.0);
    //    value G(33.0);

    //    auto alpha = A + A;
    //    auto beta = A - B;
    //    auto gamma = A * B;
    //    auto delta = A / B;
    //    auto epsilon = A / B + A;
    //    auto zeta = pow(A, C);
    //    auto eta = log(C, B);
    //    auto theta = sin(PP);
    //    auto iota = cos(PP);
    //    auto kappa = tan(PP);
    //    auto lambda = log(A, pow(A, B));

    //    cout << "A = " << A() << endl;
    //    cout << "B = " << B() << endl;
    //    cout << "C = " << C() << endl;
    //    cout << "PP = " << PP() << endl;
    //    cout << "D = " << D() << endl;
    //    cout << "E = " << E() << endl;
    //    cout << "G = " << G() << endl;

    //    cout << endl;

    //    cout << "A + B = " << alpha() << endl;
    //    cout << "A - B = " << beta() << endl;
    //    cout << "A * B = " << gamma() << endl;
    //    cout << "A / B = " << delta() << endl;
    //    cout << "A / B - A = " << epsilon() << endl;
    //    cout << "A ^ C = " << zeta() << endl;
    //    cout << "log_C B = " << eta() << endl;
    //    cout << "sin PP = " << theta() << endl;
    //    cout << "cos PP = " << iota() << endl;
    //    cout << "tan PP = " << kappa() << endl;
    //    cout << "log_A A ^ B = " << lambda() <<  endl;

    //    differ(lambda);

    //    cout << "       deri A : " << A.get_derivative() << endl;
    //    cout << "       deri B : " << B.get_derivative() << endl;
    //    cout << "       deri C : " << C.get_derivative() << endl;
    //    cout << "       deri PP : " << PP.get_derivative() << endl;
    //}

    {
        double a = 10;
        double b = 20;
        double c = 30;

        value A(a);
        value B(b);
        value C(c);

        a = 5.0;
        b = 3.0;
        c = 9.0;

        value D(11.0);
        value E(22.0);
        value F(33.0);

        value half(0.5);
        value two(2);

        auto error = half * (pow(A - D, two) + pow(B - E, two) + pow(C - F, two));
        auto lambda = log(A, pow(A, B));

        cout << "A    = " << A() << endl;
        cout << "B    = " << B() << endl;
        cout << "C    = " << C() << endl;
        cout << "D    = " << D() << endl;
        cout << "E    = " << E() << endl;
        cout << "F    = " << F() << endl;
        cout << "half = " << half() << endl;
        cout << "two  = " << two() << endl;

        cout << endl;

        cout << "half * (pow(A - D, two) + pow(B - E, two) + pow(C - F, two)) = " << error() << endl;
        cout << "log_A A ^ B = " << lambda() <<  endl;

        differ(error);
        //differ(lambda);

        cout << "       deri A : " << A.get_derivative() << endl;
        cout << "       deri B : " << B.get_derivative() << endl;
        cout << "       deri C : " << C.get_derivative() << endl;
    }

    return 0;
}

