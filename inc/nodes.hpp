#ifndef IG_NODES_HPP
#define IG_NODES_HPP

namespace automatic_differentiation {
    // 葉
    template <class T>
    class value {
    public:
        using type = typename std::conditional_t<std::is_lvalue_reference<T>::value, std::remove_reference_t<T>, T>;
        constexpr static std::size_t size = 1;
        constexpr static std::size_t height = 0;
        bool is_connected;
    private:
        T val;
        type derivative;
    public:
        constexpr value(T&& v) : val(v), derivative(0), is_connected(false) { }
        constexpr auto operator () () -> std::conditional_t<std::is_lvalue_reference<T>::value, T, T&> { return val; }
        constexpr void differentiate() { }
        constexpr void add_derivative(type d = 1) { derivative += d; }
        constexpr auto get_derivative() const { return derivative; }
        template <std::size_t I>
        constexpr auto get_operand() { return val; }
    };
    template <class T>
    value(T&& a) -> value<T>;

    // 単項演算
    template <class Op, class C>
    class unary_operation {
    private:
        C &c;
        bool is_calculated;
    public:
        using CC = typename std::conditional_t<std::is_lvalue_reference<C>::value, std::remove_reference_t<C>, C>;
        using type = decltype(Op::apply(c));
        constexpr static std::size_t size = 1;
        constexpr static std::size_t height = CC::height + 1;
    private:
        type val;
        type derivative;
    public:
        constexpr unary_operation(C& c) : c(c), val(0), derivative(0), is_calculated(false) { }
        constexpr auto operator () () { if (!is_calculated) val = Op::apply(c), is_calculated = true; return val; }
        constexpr void differentiate() { Op::differentiate(derivative, c); }
        constexpr void add_derivative(type d = 1) { derivative += d; }
        template <std::size_t I>
        constexpr auto get_operand() { return c; }
    };

    // 二項演算
    template <class Op, class L, class R>
    class binary_operation {
    private:
        L left;
        R right;
    public:
        using LL = typename std::conditional_t<std::is_lvalue_reference<L>::value, std::remove_reference_t<L>, L>;
        using RR = typename std::conditional_t<std::is_lvalue_reference<R>::value, std::remove_reference_t<R>, R>;
        using type = decltype(Op::apply(left, right));
        constexpr static std::size_t size = 2;
        constexpr static std::size_t height = std::max(LL::height, RR::height) + 1;
    private:
        bool is_calculated;
        type val;
        type derivative;
    public:
        constexpr binary_operation(L& left, R& right) : left(left), right(right), derivative(0), is_calculated(false) { }
        constexpr auto operator () () { if (!is_calculated) val = Op::apply(left, right), is_calculated = true; return val; }
        constexpr void differentiate() { Op::differentiate(derivative, left, right); }
        constexpr void add_derivative(type d = 1) { derivative += d; }
        template <std::size_t I>
        constexpr auto get_operand() {
            if constexpr (I == 0)
                return left;
            else if constexpr (I == 1)
                return right;
        }
    };


    // max
    template <std::size_t a, std::size_t b>
    constexpr std::size_t get_max() {
        if constexpr (a < b)
            return b;
        else
            return a;
    }

    // property
    template <class T>
    struct get_property;
    template <class T>
    struct get_property<value<T>> {
        constexpr static std::size_t size = 1;
        constexpr static std::size_t height = 0;
    };
    template <class Op, class C>
    struct get_property<unary_operation<Op, C>> {
        using CC = typename std::conditional_t<std::is_lvalue_reference<C>::value, std::remove_reference_t<C>, C>;
        constexpr static std::size_t size = 1;
        constexpr static std::size_t height = get_property<CC>::height + 1;
    };
    template <class Op, class L, class R>
    struct get_property<binary_operation<Op, L, R>> {
        using LL = typename std::conditional_t<std::is_lvalue_reference<L>::value, std::remove_reference_t<L>, L>;
        using RR = typename std::conditional_t<std::is_lvalue_reference<R>::value, std::remove_reference_t<R>, R>;
        constexpr static std::size_t size = 2;
        constexpr static std::size_t height = get_max<get_property<LL>::height, get_property<RR>::height>() + 1;
    };
}

#endif

