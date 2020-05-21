#ifndef IG_AUTODIFFER_HPP
#define IG_AUTODIFFER_HPP

#include "dual_number.hpp"
#include "tuple_list.hpp"
#include "nodes.hpp"
#include "operations.hpp"

namespace autodiffer {
    template <int h> struct test_height { constexpr static int height = h; };

    template <class... Elements>
    constexpr void differ_impl(tuple_list<Elements...> &list) {
        //if constexpr (list.size == 0)
        if constexpr (sizeof...(Elements) == 0)
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

#endif

