#pragma once
#include <iostream>
#include <memory>
#include <functional>
#include <type_traits>
// requires c++14

namespace andi
{
    template<class T>
    class list
    {
        struct node;
        using pointer_t = std::shared_ptr<const node>;

        struct node
        {
            const T value; // само това ни интересува да е const, всичко останало може да не е
            pointer_t next;
            node(const T& _value, const pointer_t& _next) : value(_value), next(_next) {}
        };

        // only data member you need
        pointer_t head;

        list(const pointer_t& _ptr) : head(_ptr) {}
        list(const T& val, list<T> lst) : list(std::make_shared<const node>(val, lst.head)) {};
    public:
        list() : head(nullptr) {};

        friend list cons(const T& val, list<T> lst)
        {
            return{ val,lst }; // само извикването на cons води до алокация на памет за една "кутийка"
        }

        friend const T& head(list<T> lst)
        {
            return lst.head->value;
        }

        friend list tail(list<T> lst)
        {
            return{ lst.head->next };
        }

        friend bool null(list<T> lst)
        {
            return !lst.head;
        }
    };

    // standard functions: length, reverse, map, filter, foldr, append,...
    template<class T>
    size_t length(list<T> lst)
    {
        size_t result = 0;
        for (list<T> l = lst; !null(l); l = tail(l))
            ++result;
        return result;
    }

    template<class T>
    list<T> reverse(list<T> lst)
    {
        list<T> result;
        for (list<T> l = lst; !null(l); l = tail(l))
            result = cons(head(l), result);
        return result;
    }

    template<class UnaryFunction, class T>
    list<std::result_of_t<UnaryFunction(T)>> map(UnaryFunction f, list<T> lst)
    {
        list<std::result_of_t<UnaryFunction(T)>> result;
        for (list<T> l = reverse(lst); !null(l); l = tail(l))
            result = cons(f(head(l)), result);
        return result;
    }

    template<class UnaryPredicate, class T>
    list<T> filter(UnaryPredicate p, list<T> lst)
    {
        list<T> result;
        for (list<T> l = lst; !null(l); l = tail(l))
            if (p(head(l)))
                result = cons(head(l), result);
        return reverse(result);
    }

    template<class BinaryFunction, class T, class U>
    std::result_of_t<BinaryFunction(T, U)> foldr(BinaryFunction op, const U& nv, list<T> lst)
    {
        std::result_of_t<BinaryFunction(T, U)> result = nv;
        for (list<T> l = reverse(lst); !null(l); l = tail(l))
            result = op(head(l), result);
        return result;
    }

    template<class T>
    T sum(list<T> lst)
    {
        return foldr(std::plus<T>(), T(0), lst); // haha
    }

    template<class T>
    T product(list<T> lst)
    {
        return foldr(std::multiplies<T>(), T(1), lst);
    }

    template<class T>
    list<T> append(list<T> lst1, list<T> lst2)
    {
        for (list<T> l = reverse(lst1); !null(l); l = tail(l))
            lst2 = cons(head(l), lst2);
        return lst2;
    }
}

// for code cleanliness
template<class T>
andi::list<T> operator|=(const T& val, andi::list<T> lst)
{
    return cons(val, lst);
}

// помощна функция, връщаща празен списък от съотв. тип
template<class T>
andi::list<T> _() { return{}; } // lol

template<class T>
std::ostream& operator<<(std::ostream& os, andi::list<T> lst)
{
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
