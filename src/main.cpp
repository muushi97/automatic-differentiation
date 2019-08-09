#include <iostream>
#include <type_traits>

//#include "dual_number.hpp"



// 型確認用
template <typename T>
struct type_detector;



// 葉になる型
//template <class T>
//struct variable {
//    const T val;
//    constexpr variable(T&& v) : val(v) { std::cout << "new instance lvalue reference" << std::endl; }
//    constexpr auto operator () () { return val; }
//};
//template <class T>
//struct constant {
//    T val;
//    constexpr constant(T v) : val(v) { std::cout << "new instance rvalue reference" << std::endl; }
//    constexpr auto operator () () { return val; }
//};

template <class T>
class value {
    T val;
    typename std::conditional_t<std::is_lvalue_reference<T>::value, std::remove_reference_t<T>, T> derivative;
public:
    constexpr value(T&& v) : val(v), derivative() { }
    constexpr auto operator () () -> std::conditional_t<std::is_lvalue_reference<T>::value, T, T&> { return val; }
};
template <class T>
value(T&& a) -> value<T>;

// 単項演算
template <class Op, class C>
class unary_operation {
    C &c;
public:
    constexpr unary_operation(C& c) : c(c) { }
    constexpr auto operator () () { return Op::apply(c); }
};
struct assignment { template <class C> static constexpr auto apply(C& c) { return c(); } };


// 二項演算
template <class Op, class L, class R>
class binary_operation {
    L left;
    R right;

    inline constexpr auto getLeftValue() const { return left(); }
    inline constexpr auto getRightValue() const { return right(); }
public:
    constexpr binary_operation(L& left, R& right) : left(left), right(right) { }
    constexpr auto operator () () { return Op::apply(left, right); }
};

//template <class L, class R>
//binary_operation<addision, L, R>::

// 加法
struct addition { template <class L, class R> static constexpr auto apply(L& left, R& right) { return (left() + right()); } };
template <class L, class R> auto operator + (L&& left, R&& right) { return binary_operation<addition, L, R>(left, right); }
// 減法
struct subtraction { template <class L, class R> static constexpr auto apply(L& left, R& right) { return (left() - right()); } };
template <class L, class R> auto operator - (L&& left, R&& right) { return binary_operation<subtraction, L, R>(left, right); }
// 乗法
struct multiplication { template <class L, class R> static constexpr auto apply(L& left, R& right) { return (left() * right()); } };
template <class L, class R> auto operator * (L&& left, R&& right) { return binary_operation<multiplication, L, R>(left, right); }
// 除法
struct division { template <class L, class R> static constexpr auto apply(L& left, R& right) { return (left() / right()); } };
template <class L, class R> auto operator / (L&& left, R&& right) { return binary_operation<division, L, R>(left, right); }


template <typename T>
void fuga(T&& a) {
    type_detector<decltype(a)> bbb;
}

using namespace std;

template <class T>
struct hoge {
    using type = T;
    hoge(T&& a)  { }
};
template <class T>
hoge(T&& a) -> hoge<T>;

int main() {
    //fuga(a);

    int a = 10;
    int b = 20;
    int c = 30;

    value A(a);
    value B(b);
    value C(c);

    a = 10000;
    b = 2000;
    c = 300;

    value D(11);
    value E(22);
    value G(33);

    auto alpha_ = A + A;
    auto alpha = alpha_ + A;
    auto beta = A - B;
    auto gamma = A * B;
    auto delta = A / B;

    //type_detector<decltype(alpha)> fuga;

    cout << "A + B = " << alpha() << endl;
    cout << "A - B = " << beta() << endl;
    cout << "A * B = " << gamma() << endl;
    cout << "A / B = " << delta() << endl;

    return 0;
}

