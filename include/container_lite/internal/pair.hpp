#ifndef CONTAINER_LITE_LIB_INTR_PAIR_HPP
#define CONTAINER_LITE_LIB_INTR_PAIR_HPP
#include <cstddef>

namespace container_lite {
    template <typename T1, typename T2>
    struct pair {
        T1 first_;
        T2 second_;
        size_t hash_;

        pair(T1 first, T2 second) : first_(first), second_(second), hash_(0) {}

        ~pair() {
            delete first_;
            delete second_;
        }

        pair operator=(pair other) {
            pair new_pair(other.first_, other.second_);
            return new_pair;
        }

        size_t size_of() {
            return sizeof(T1) + sizeof(T2) + sizeof(size_t);
        }

    };
}
#endif //CONTAINER_LITE_LIB_INTR_PAIR_HPP