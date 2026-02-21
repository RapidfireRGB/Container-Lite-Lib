#ifndef CONTAINER_LITE_LIB_STATIC_VECTOR_HPP
#define CONTAINER_LITE_LIB_STATIC_VECTOR_HPP
#include <cstddef>
#include <new>

namespace container_lite {
    // Fixed capacity vector which buffers memory for N number of elements.
    // Unlike std arrays, elements can be added or removed as long as number of elements does not exceed capacity.
    template <typename T, std::size_t N>
    class static_vector {
    private:
        size_t size_;
        alignas(T) unsigned char buffer[N * sizeof(T)];
        static constexpr size_t capacity_ = N;

        T* data() {
            return reinterpret_cast<T*>(buffer);
        }

        const T* data() const {
            return reinterpret_cast<const T*>(buffer);
        }

    public:
        static_vector() : size_(0), buffer{} {
            static_assert(sizeof(static_vector) == sizeof(size_t) + N * sizeof(T),
                          "Insufficient Memory for Static Vector.\n");
        }

        ~static_vector() {
            clear();
        }

        void push_back(const T& element) {
            if (size_ == capacity_) {
                return;
            }
            new (&data()[size_]) T(element);
            size_++;
        }

        void pop_back() {
            if (size_ == 0) {
                return;
            }
            size_--;
            data()[size_].~T();
        }

        void clear() {
            if (size_ == 0) {
                return;
            }
            for (size_t i = 0; i <= size_-1; i++) {
                data()[i].~T();
            }
            size_ = 0;
        }

        [[nodiscard]] bool empty() {
            return data()[0] == nullptr;
        }
    };

}
#endif //CONTAINER_LITE_LIB_STATIC_VECTOR_HPP