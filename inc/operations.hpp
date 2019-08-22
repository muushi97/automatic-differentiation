#ifndef IG_OPERATIONS_HPP
#define IG_OPERATIONS_HPP

namespace automatic_differentiation {
    // 加法
    struct addition {
        template <class L, class R> static constexpr auto apply(L&& left, R&& right) { return (left() + right()); }
        template <class T, class L, class R> static constexpr void differentiate(T&& d, L&& left, R&& right) {
            left.add_derivative(d * 1);
            right.add_derivative(d * 1);
        }
    };
    // 減法
    struct subtraction {
        template <class L, class R> static constexpr auto apply(L&& left, R&& right) { return (left() - right()); }
        template <class T, class L, class R> static constexpr void differentiate(T&& d, L&& left, R&& right) {
            left.add_derivative(d * 1);
            right.add_derivative(d * -1);
        }
    };
    // 乗法
    struct multiplication {
        template <class L, class R> static constexpr auto apply(L&& left, R&& right) { return (left() * right()); }
        template <class T, class L, class R> static constexpr void differentiate(T&& d, L&& left, R&& right) {
            left.add_derivative(d * right());
            right.add_derivative(d * left());
        }
    };
    // 除法
    struct division {
        template <class L, class R> static constexpr auto apply(L&& left, R&& right) { return (left() / right()); }
        template <class T, class L, class R> static constexpr void differentiate(T&& d, L&& left, R&& right) {
            left.add_derivative(d / right());
            right.add_derivative(d * left() * -1 / std::pow(right(), 2));
        }
    };

    template <class L, class R> using addition_operation = binary_operation<addition, L, R>;
    template <class L, class R> using subtraction_operation = binary_operation<subtraction, L, R>;
    template <class L, class R> using multiplication_operation = binary_operation<multiplication, L, R>;
    template <class L, class R> using division_operation = binary_operation<division, L, R>;
}

template <class L, class R> auto operator + (L&& left, R&& right) { return automatic_differentiation::addition_operation<L, R>(left, right); }
template <class L, class R> auto operator - (L&& left, R&& right) { return automatic_differentiation::subtraction_operation<L, R>(left, right); }
template <class L, class R> auto operator * (L&& left, R&& right) { return automatic_differentiation::multiplication_operation<L, R>(left, right); }
template <class L, class R> auto operator / (L&& left, R&& right) { return automatic_differentiation::division_operation<L, R>(left, right); }

#endif

