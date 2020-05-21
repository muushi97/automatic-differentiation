#ifndef IG_TUPLE_LIST_HPP
#define IG_TUPLE_LIST_HPP

#include "nodes.hpp"

namespace automatic_differentiation {
    // リストの宣言
    template <class... Elements>
    class tuple_list;

    // 要素からリストを生成する
    template <class FirstElement, class... RestElements>
    constexpr auto make_list(FirstElement&& first, RestElements&&... rests) {
        if constexpr (sizeof...(rests) == 0)
            return tuple_list(std::forward<FirstElement>(first));
        else
            return tuple_list(std::forward<FirstElement>(first), make_list(std::forward<RestElements>(rests)...));
    }
    // 先頭に要素を連結する
    template <class FirstElement, class List>
    constexpr auto link_list(FirstElement&& first, List&& list) {
        return tuple_list(std::forward<FirstElement>(first), std::forward<List>(list));
    }

    template <class FirstElement, class... RestElements>
    class tuple_list<FirstElement, RestElements...> {
        FirstElement first_element;
        tuple_list<RestElements...> rest_elements;

    public:
        constexpr static std::size_t size = 1 + sizeof...(RestElements);
        constexpr tuple_list(FirstElement&& first, tuple_list<RestElements...>&& rests) : first_element(first), rest_elements(rests) { }

        template <class First, class... Rests>
        constexpr auto merge(First&& f, Rests&&... r) {
            if constexpr (sizeof...(r) == 0) {
                if constexpr (get_property<std::remove_reference_t<FirstElement>>::height <= get_property<std::remove_reference_t<First>>::height)
                    return link_list(std::forward<First>(f), link_list(std::forward<FirstElement>(first_element), std::move(rest_elements)));
                else
                    return link_list(std::forward<FirstElement>(first_element), rest_elements.merge(std::forward<First>(f)));
            }
            else {
                if constexpr (get_property<std::remove_reference_t<FirstElement>>::height <= get_property<std::remove_reference_t<First>>::height)
                    return link_list(std::forward<First>(f), merge(std::forward<Rests>(r)...));
                else
                    return link_list(std::forward<FirstElement>(first_element), rest_elements.merge(std::forward<First>(f), std::forward<Rests>(r)...));
            }
        }

        constexpr auto first() const { return first_element; }
        constexpr tuple_list<RestElements...> pop_front() const { return rest_elements; }
    };
    template <>
    class tuple_list<> {
    public:
        constexpr static std::size_t size = 0;
        constexpr tuple_list() { }

        template <class... Elements>
        constexpr auto merge(Elements&&... elements) {
            return make_list(std::forward<Elements>(elements)...);
        }
    };
    template <class FirstElement>
    class tuple_list<FirstElement> {
        FirstElement first_element;
    public:
        constexpr static std::size_t size = 1;
        constexpr tuple_list(FirstElement&& first) : first_element(first) { }

        template <class First, class... Rests>
        constexpr auto merge(First&& f, Rests&&... r) {
            if constexpr (sizeof...(r) == 0) {
                if constexpr (get_property<std::remove_reference_t<FirstElement>>::height <= get_property<std::remove_reference_t<First>>::height)
                    return tuple_list<First, FirstElement>(std::forward<First>(f), make_list(std::forward<FirstElement>(first_element)));
                else
                    return tuple_list<FirstElement, First>(std::forward<FirstElement>(first_element), make_list(std::forward<First>(f)));
            }
            else {
                if constexpr (get_property<std::remove_reference_t<FirstElement>>::height <= get_property<std::remove_reference_t<First>>::height)
                    return link_list(std::forward<First>(f), merge(std::forward<Rests>(r)...));
                else
                    return link_list(std::forward<FirstElement>(first_element), make_list(std::forward<First>(f), std::forward<Rests>(r)...));
            }
        }

        constexpr auto first() const { return first_element; }
        constexpr auto pop_front() const { return tuple_list<>(); }
    };

    template <class FirstElement, class... RestElements>
    tuple_list(FirstElement&& first, tuple_list<RestElements...> second) -> tuple_list<FirstElement, RestElements...>;
    template <class FirstElement>
    tuple_list(FirstElement&& first) -> tuple_list<FirstElement>;
}

#endif

