# FunctionalLists
C++11 proof-of-concept implementation of immutable functional style lists.

As a demonstration, the most common list operations such as `length`, `reverse`, the higher-oreder functions `map`, `filter` and `foldr`, as well as `sum`, `product` and `append` are implemented. C++17 is needed for simple additional syntactic features.

Each of these functions has a purely-functional variant, separated in `namespace andi::pure`, which needs the `pure::` specifier to be used. They are left in the repo simply for being elegant, straight-forward, and a little bit mind-boggling. The actual list operations have been implemented in an imperative style to avoid stack overflow, but are still valid & correct.

The list itself `andi::list<...>` contains only a reference-counted `std::shared_ptr<...>` pointer to its "head". This pointer is regarded as small & efficient enough, that copying lists around (instead of passing references, etc.) is considered cheap - it would actually cost copying two pointers and an atomic integer increment, dereferenced from a pointer <=> possibly a cache miss. It is expected that `sizeof(andi::list<...>) == sizeof(std::shared_ptr<...>) == 2*sizeof(void*)` and could be decreased to `sizeof(andi::list<...>) == sizeof(void*)` by using a special smart pointer (one that does not need protection from cycles & weak_ptr awareness).
