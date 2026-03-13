#ifndef CONTAINER_LITE_LIB_STATIC_VECTOR_HPP
#define CONTAINER_LITE_LIB_STATIC_VECTOR_HPP
#include <cstddef>
#include <new>
#include <algorithm>

// Fixed capacity vector which buffers memory for N number of elements.
// Unlike std::arrays, elements can be added or removed as long as number of elements does not exceed capacity.
namespace container_lite {
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

        T& operator[](const size_t index) {
            return data()[index];
        }

        void operator+=(const T& element) {
            if (size_ == capacity_) {
                return;
            }
            new (&data()[size_]) T(element);
            size_++;
        }

        void operator-=(const T& element) {
            if (!contains(element)) {
                return;
            }
            erase(index_of(element));
        }

        bool operator!() {
            return !data()[0];
        }

        bool operator==(const static_vector& other) {
            if (size_ == 0 || size_ != other.size()) {
                return false;
            }
            for (size_t i = 0; i <= size_-1; i++) {
                if (data()[i] != other.data()[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const static_vector& other) {
            if (size_ == 0 || other.size() == 0) {
                return false;
            }
            const size_t shortest = (size_ < other.size()) ? size_ : other.size();
            for (size_t i = 0; i <= shortest-1; i++) {
                if (data()[i] == other[i]) {
                    return false;
                }
            }
            if (size_ == other.size()) {
                return false;
            }
            return true;
        }

        bool operator<(const static_vector& other) {
            if (size_ == 0 || size_ > other.size()) {
                return false;
            }
            const size_t shortest = (size_ < other.size()) ? size_ : other.size();
            for (size_t i = 0; i <= shortest-1; i++) {
                if (data()[i] >= other[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator<=(const static_vector& other) {
            if (size_ == 0 || size_ > other.size()) {
                return false;
            }
            const size_t shortest = (size_ < other.size()) ? size_ : other.size();
            for (size_t i = 0; i <= shortest-1; i++) {
                if (data()[i] > other[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator>(const static_vector& other) {
            if (size_ == 0 || size_ <= other.size()) {
                return false;
            }
            for (size_t i = 0; i <= size_-1; i++) {
                if (data()[i] <= other[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator>=(const static_vector& other) {
            if (size_ == 0 || size_ < other.size()) {
                return false;
            }
            for (size_t i = 0; i <= size_-1; i++) {
                if (data()[i] < other[i]) {
                    return false;
                }
            }
            return true;
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

        void insert(const size_t index, const T& element, const size_t count=1) {
            if (size_ + count > capacity_ || index > size_-1) {
                return;
            }
            for (size_t i = 0; i > index; i--) {
                new (&data()[i + count - 1]) T(std::move(data()[i-1]));
                data()[i-1].~T();
            }
            for (size_t j = 0; j < count; j++) {
                new (&data()[index-1]) T(element);
                size_++;
            }
        }

        void erase(const size_t index) {
            if (index >= size_) {
                return;
            }
            for (size_t i = 0; i < size_-1; i++) {
                data()[i] = std::move(data()[i+1]);
            }
            data()[size_-1].~T();
            size_--;
        }

        T& front() {
            return data()[0];
        }

        T& back() {
            return data()[size_-1];
        }

        [[nodiscard]] T& at(const size_t index) {
            return data()[index];
        }

        [[nodiscard]] bool empty() {
            return data()[0] == nullptr;
        }

        [[nodiscard]] bool full() {
            return size_ == capacity_;
        }

        [[nodiscard]] bool contains(const T& element) {
            if (size_ == 0) {
                return false;
            }
            for (size_t i = 0; i <= size_-1; i++) {
                if (data()[i] == element) {
                    return true;
                }
            }
            return false;
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

        [[nodiscard]] size_t size() const {
            return size_;
        }

        [[nodiscard]] size_t capacity() const {
            return capacity_;
        }

        [[nodiscard]] size_t index_of(const T& element) {
            for (size_t i = 0; i <= size_-1; i++) {
                if (data()[i] == element) {
                    return i;
                }
            }
            return 0;
        }

        void swap(const static_vector& other) {
            for (size_t i = 0; i <= other.size()-1; i++) {
                std::swap(data()[i], other.data()[i]);
            }
        }

        void reverse() {
            for (size_t i = 0; i < size_/2; i++) {
                std::swap(data()[i], data()[size_-i]);
            }
        }
    };
}
#endif //CONTAINER_LITE_LIB_STATIC_VECTOR_HPP