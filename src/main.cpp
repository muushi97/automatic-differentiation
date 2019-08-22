#include <iostream>
#include <type_traits>
#include <tuple>
#include <cmath>

#include "dual_number.hpp"
#include "tuple_list.hpp"
#include "nodes.hpp"
#include "operations.hpp"

using namespace automatic_differentiation;

// 型確認用
template <typename T>
struct type_detector;
template <int T>
struct nontype_detector;

namespace audeff {
    template <int h> struct test_height { constexpr static int height = h; };

    template <class... Elements>
    void differ_impl(tuple_list<Elements...> &list) {
        if constexpr (list.size == 0)
            return;
        else {
            auto first = list.first();
            first.differentiate();

            auto li = list.pop_front();
            if constexpr (first.height == 0)
                differ_impl(li);
            else {
                if constexpr (first.size == 1) {
                    auto lii = li.merge(first.template get_operand<0>());
                    differ_impl(lii);
                }
                else if constexpr (first.size == 2) {
                    auto left = first.template get_operand<0>();
                    auto right = first.template get_operand<1>();
                    if constexpr (left.height < right.height) {
                        auto lii = li.merge(right, left);
                        differ_impl(lii);
                    }
                    else {
                        auto li = list.pop_front().merge(left, right);
                        differ_impl(li);
                    }
                }
                else
                    differ_impl(li);
            }
        }
    };

    template <class Expression>
    void differ(Expression &e) {
        e.add_derivative();
        auto li = make_list(e);

        differ_impl(li);
    };
}

using namespace audeff;

using std::cout;
using std::endl;

int main() {
    double a = 10;
    double b = 20;
    double c = 30;

    value A(a);
    value B(b);
    value C(c);

    a = 10000.0;
    b = 2000.0;
    c = 300.0;

    value D(11.0);
    value E(22.0);
    value G(33.0);

    auto alpha_ = A + A;
    auto alpha = A + A;
    auto beta = A - B;
    auto gamma = A * B;
    auto delta = A / B + A;

    cout << "A + B = " << alpha() << endl;
    cout << "A - B = " << beta() << endl;
    cout << "A * B = " << gamma() << endl;
    cout << "A / B = " << delta() << endl;

    differ(delta);

    cout << "       deri : " << A.get_derivative() << endl;
    cout << "       deri : " << B.get_derivative() << endl;
    cout << "       deri : " << C.get_derivative() << endl;

    return 0;
}

