#ifndef CONTAINER_LITE_LIB_LIGHT_VECTOR_HPP
#define CONTAINER_LITE_LIB_LIGHT_VECTOR_HPP
#include <cstddef>
#include <new>

namespace container_lite {
    // Small vector implementation which consumes up to 1kb of memory at a time, or specified byte limit
    template <typename T, std::size_t MaxBytes=1024-sizeof(size_t)>
    class light_vector {
    private:
        size_t size_;
        alignas(T) unsigned char buffer[MaxBytes];
        static constexpr size_t capacity_ = MaxBytes / sizeof(T);

        // Returns const/non const pointer to stored elements
        T* data() {
            return reinterpret_cast<T*>(buffer);
        }

        const T* data() const {
            return reinterpret_cast<const T*>(buffer);
        }

    public:
        light_vector() : size_(0), buffer{} {
            static_assert(capacity_ > 0, "Type is too large for light vector.\n");
        }

        ~light_vector() {
            clear();
        }

        /* Operator Overloading for:
         * [] - Indexing
         * =  - Copy Assignment
         * += - Element Insertion
         * -= - Element Erasure
         * !  - NOT
         * == - Equal To
         * != - Not Equal
         * <  - Less Than
         * <= - Less Than/Equal To
         * >  - Greater Than
         * >= - Greater Than/Equal To
         */

        T& operator[](size_t index) {
            return data()[index];
        }

        light_vector& operator=(const light_vector& other) {
            // Early function exit if vectors are already equivalent
            if (this == &other) {
                return *this;
            }

            clear();

            for (size_t i = 0; i <= other.size()-1; i++) {
                new (&data()[i]) T(other.data()[i]);
            }

            size_ = other.size();
            return *this;
        }

        void operator+=(const T& element) {
            if (size_ > capacity_) {
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
            return data()[0] == nullptr;
        }

        bool operator==(const light_vector& other) {
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

        bool operator!=(const light_vector& other) {
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

        bool operator<(const light_vector& other) {
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

        bool operator<=(const light_vector& other) {
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

        bool operator>(const light_vector& other) {
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

        bool operator>=(const light_vector& other) {
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
            if (size_ >= capacity_) {
                return;
            }
            new (&data()[size_]) T(element);
            size_++;
        }

        void pop_back() {
            if (size_ > 0) {
                size_--;
                data()[size_].~T();
            }
        }

        void insert(const size_t pos, const T& value, const size_t count=1) {
            if (size_ + count > capacity_ || pos > size_-1) {
                return;
            }

            for (size_t i = 0; i > pos; i--) {
                new (&data()[i + count - 1]) T(std::move(data()[i-1]));
                data()[i - 1].~T();
            }

            for (size_t j = 0; j < count; j++) {
                new (&data()[pos-1]) T(value);
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

        [[nodiscard]] bool empty() {
            return data()[0] == nullptr;
        }

        [[nodiscard]] bool full() const {
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

        void swap(const light_vector& other) {
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

#endif //CONTAINER_LITE_LIB_LIGHT_VECTOR_HPP