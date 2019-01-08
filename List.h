#pragma once
#include <iostream>
#include <memory>
#include <functional>
#include <type_traits>

namespace andi {

// Serves as the polymorhic constant that is the empty list.
// Note than calling anything other than cons
// and operator|= is not yet implemented.
static const struct nil_t {} nil;

template<class T>
class list {
    struct node;
    using pointer_t = std::shared_ptr<const node>;

    struct node {
        const T value; // the only 'const' that matters
        pointer_t next;

        node(const T& value, const pointer_t next) : value{ value }, next{ next } {};
    };

    pointer_t head; // no more data members needed

    list(const pointer_t& _ptr) : head{ _ptr } {}
    list(const T& val, list<T> lst) : list(std::make_shared<const node>(val, lst.head)) {};
public:
    list() = default;
    list(nil_t) : list() {} // the empty list should be able to be "interpreted" as any type
    // to-do: add construction by initializer list, reversible and non-reversible iterators

    friend list cons(const T& val, list<T> lst) {
        return{ val,lst }; // the only function to make an allocation is cons, allocating a single new node
    }

    friend const T& head(list<T> lst) {
        return lst.head->value;
    }

    friend list tail(list<T> lst) {
        return{ lst.head->next };
    }

    friend bool null(list<T> lst) {
        return !lst.head;
    }
};

// Standard functions: length, reverse, map, filter, foldr, append,...
// The purely functional style leads to a stack overflow, necessitating an imperative approach.
template<class T>
size_t length(list<T> lst) {
    size_t result = 0;
    for (list<T> l = lst; !null(l); l = tail(l))
        ++result;
    return result;
}

template<class T>
list<T> reverse(list<T> lst) {
    list<T> result;
    for (list<T> l = lst; !null(l); l = tail(l))
        result = cons(head(l), result);
    return result;
}

template<class UnaryFunction, class T>
auto map(UnaryFunction f, list<T> lst) { // the return type is immediately below
    list<std::invoke_result_t<UnaryFunction, T>> result;
    for (list<T> l = reverse(lst); !null(l); l = tail(l))
        result = cons(f(head(l)), result);
    return result;
}

template<class UnaryPredicate, class T>
list<T> filter(UnaryPredicate p, list<T> lst) {
    list<T> result;
    for (list<T> l = lst; !null(l); l = tail(l))
        if (p(head(l)))
            result = cons(head(l), result);
    return reverse(result);
}

template<class BinaryFunction, class T, class U>
auto foldr(BinaryFunction op, const U& nv, list<T> lst) { // the return type is immediately below
    std::invoke_result_t<BinaryFunction, T, U> result = nv;
    for (list<T> l = reverse(lst); !null(l); l = tail(l))
        result = op(head(l), result);
    return result;
}

template<class T>
T sum(list<T> lst) {
    return foldr(std::plus<T>{}, T{ 0 }, lst);
}

template<class T>
T product(list<T> lst) {
    return foldr(std::multiplies<T>{}, T{ 1 }, lst);
}

template<class T>
list<T> append(list<T> lst1, list<T> lst2) {
    for (list<T> l = reverse(lst1); !null(l); l = tail(l))
        lst2 = cons(head(l), lst2);
    return lst2;
}

template<class T>
list<T> cons(const T& val, nil_t) {
    return cons(val, andi::list<T>{});
}
} // namespace andi

// for code cleanliness
template<class T>
andi::list<T> operator|=(const T& val, andi::list<T> lst) {
    return cons(val, lst);
}

template<class T>
andi::list<T> operator|=(const T& val, andi::nil_t) {
    return cons(val, andi::list<T>{});
}

template<class T>
std::ostream& operator<<(std::ostream& os, andi::list<T> lst) {
    // more ternary magic in a pure recursive function...
    // return null(lst) ? os << "\n" : (os << head(lst) << " " << tail(lst));

    // pretty print instead
    if (null(lst))
        return os << "()";
    os << "(" << head(lst);
    for (andi::list<T> l = tail(lst); !null(l); l = tail(l))
        os << ", " << head(l);
    return os << ")";
}
