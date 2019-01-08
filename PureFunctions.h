#pragma once
#include"List.h"

namespace andi::pure {
// pure, elegant, recursive & stack-overflowing functions
template<class T>
size_t length(list<T> lst) {
    // nothing adds more to code readability than the ternary operator ?:
    return null(lst) ? 0 : 1 + pure::length(tail(lst));
}

template<class T>
list<T> reverse(list<T> lst) {
    static const std::function<list<T>(list<T>, list<T>)>
        helper = [](list<T> lst, list<T> res) -> list<T>
    {
        return null(lst) ? res : helper(tail(lst), cons(head(lst), res));
    };
    return helper(lst, list<T>());
}

template<class UnaryFunction, class T>
list<std::invoke_result_t<UnaryFunction, T>> map(UnaryFunction f, list<T> lst) {
    using U = std::invoke_result_t<UnaryFunction, T>;
    return null(lst) ? list<U>() : cons(f(head(lst)), pure::map(f, tail(lst)));
}

template<class UnaryPredicate, class T>
list<T> filter(UnaryPredicate p, list<T> lst) {
    if (null(lst))
        return list<T>();
    else if (p(head(lst)))
        return cons(head(lst), pure::filter(p, tail(lst)));
    else
        return pure::filter(p, tail(lst));
}

template<class BinaryFunction, class T, class U>
std::invoke_result_t<BinaryFunction, T, U> foldr(BinaryFunction op, const U& nv, list<T> lst) {
    return null(lst) ? nv : op(head(lst), pure::foldr(op, nv, tail(lst)));
}

template<class T>
T sum(list<T> lst) {
    return pure::foldr(std::plus<T>{}, T{ 0 }, lst);
}

template<class T>
T product(list<T> lst) {
    return pure::foldr(std::multiplies<T>{}, T{ 1 }, lst);
}

template<class T>
list<T> append(list<T> lst1, list<T> lst2) {
    return null(lst1) ? lst2 : cons(head(lst1), pure::append(tail(lst1), lst2));
}
} // namespace andi::pure


