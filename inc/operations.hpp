#ifndef IG_OPERATIONS_HPP
#define IG_OPERATIONS_HPP

namespace autodiffer {
    //struct Template {
    //    template <class L, class R> static constexpr auto apply(L&& left, R&& right) { return 1.0; }
    //    template <class T, class L, class R> static constexpr void differentiate(T&& d, L&& left, R&& right) {
    //        left.add_derivative(d * 1);
    //        right.add_derivative(d * 1);
    //    }
    //};

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

    // 指数関数
    struct power {
        template <class L, class R> static constexpr auto apply(L&& left, R&& right) { return std::pow(left(), right()); }
        template <class T, class L, class R> static constexpr void differentiate(T&& d, L&& left, R&& right) {
            left.add_derivative(d * right() * std::pow(left(), right() - 1));
            right.add_derivative(d * std::pow(left(), right()) * std::log(left()));
        }
    };
    // 対数関数
    struct logarithm {
        template <class L, class R> static constexpr auto apply(L&& left, R&& right) { return std::log(right()) / std::log(left()); }
        template <class T, class L, class R> static constexpr void differentiate(T&& d, L&& left, R&& right) {
            left.add_derivative(d * (std::log(left()) / right() - std::log(right()) / left()) / (std::pow(std::log(left()), 2) * left()));
            right.add_derivative(d / (right() * std::log(left())));
        }
    };

    //struct Template {
    //    template <class C> static constexpr auto apply(C&& c) { return 1; }
    //    template <class T, class C> static constexpr void differentiate(T&& d, C&& c) {
    //        c.add_derivative(d * 1);
    //    }
    //};

    // 三角関数
    struct sine {
        template <class C> static constexpr auto apply(C&& c) { return std::sin(c()); }
        template <class T, class C> static constexpr void differentiate(T&& d, C&& c) {
            c.add_derivative(d * std::cos(c()));
        }
    };
    struct cosine {
        template <class C> static constexpr auto apply(C&& c) { return std::cos(c()); }
        template <class T, class C> static constexpr void differentiate(T&& d, C&& c) {
            c.add_derivative(d * -1 * std::sin(c()));
        }
    };
    struct tangent {
        template <class C> static constexpr auto apply(C&& c) { return std::tan(c()); }
        template <class T, class C> static constexpr void differentiate(T&& d, C&& c) {
            c.add_derivative(d / std::pow(std::cos(c()), 2));
        }
    };

    template <class L, class R> using addition_operation = binary_operation<addition, L, R>;
    template <class L, class R> using subtraction_operation = binary_operation<subtraction, L, R>;
    template <class L, class R> using multiplication_operation = binary_operation<multiplication, L, R>;
    template <class L, class R> using division_operation = binary_operation<division, L, R>;
    template <class L, class R> using power_operation = binary_operation<power, L, R>;
    template <class L, class R> using logarithm_operation = binary_operation<logarithm, L, R>;

    template <class C>          using sine_operation = unary_operation<sine, C>;
    template <class C>          using cosine_operation = unary_operation<cosine, C>;
    template <class C>          using tangent_operation = unary_operation<tangent, C>;
}

template <class L, class R> auto operator + (L&& left, R&& right) { return autodiffer::addition_operation<L, R>(left, right); }
template <class L, class R> auto operator - (L&& left, R&& right) { return autodiffer::subtraction_operation<L, R>(left, right); }
template <class L, class R> auto operator * (L&& left, R&& right) { return autodiffer::multiplication_operation<L, R>(left, right); }
template <class L, class R> auto operator / (L&& left, R&& right) { return autodiffer::division_operation<L, R>(left, right); }

template <class L, class R> auto pow(L&& left, R&& right) { return autodiffer::power_operation<L, R>(left, right); }
template <class L, class R> auto log(L&& left, R&& right) { return autodiffer::logarithm_operation<L, R>(left, right); }

template <class C> auto sin(C&& c) { return autodiffer::sine_operation<C>(c); }
template <class C> auto cos(C&& c) { return autodiffer::cosine_operation<C>(c); }
template <class C> auto tan(C&& c) { return autodiffer::tangent_operation<C>(c); }

#endif

