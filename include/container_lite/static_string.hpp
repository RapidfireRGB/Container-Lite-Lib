#ifndef CONTAINER_LITE_LIB_STATIC_STRING_HPP
#define CONTAINER_LITE_LIB_STATIC_STRING_HPP

namespace container_lite {
    // Concept of a fixed-size string given a char pointer
    class static_string {
    private:
        char* data_;
        const size_t size_;

        // Helper gets element-wise size of string for constructor
        static size_t get_size(const char* string) {

            // Loop terminates when it reaches null character
            size_t i = 0;
            while (string[i] != '\0') {
                i++;
            }
            return i;
        }

    public:
        explicit static_string(char* cstring) : data_(cstring), size_(get_size(cstring)) {}

        ~static_string() = default;

        /* Operator overloading for:
         * [] - Indexing
         * !  - NOT
         * == - Equal
         * != - Not Equal
         * <  - Less Than
         * <= - Less Than/Equal to
         * >  - Greater Than
         * >= - Greater Than/Equal to
         * << - cout
         * >> - cin
         */

        char& operator[](const size_t index) const {
            if (index > size_-1) {
                std::cerr << "Index out of bounds by " << index-size_-1 << "\n";
            }
            return data_[index];
        }

        bool operator!() const {
            return size_ == 0;
        }

        bool operator==(const static_string& str) const {
            if (size_ != str.size_) {
                return false;
            }
            for (size_t i = 0; i <= size_-1; i++) {
                if (data_[i] != str.data_[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const static_string& str) const {
            for (size_t i = 0; i <= size_-1; i++) {
                if (data_[i] == str.data_[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator<(const static_string& str) const {
            for (size_t i = 0; i <= size_-1; i++) {
                if (data_[i] > str.data_[i]) {
                    return false;
                }
            }
            if (size_ > str.size_) {
                return false;
            }
            return true;
        }

        bool operator<=(const static_string& str) const {
            for (size_t i = 0; i <= size_-1; i++) {
                if (data_[i] > str.data_[i] || data_[i] != str.data_[i]) {
                    return false;
                }
            }
            if (size_ > str.size_) {
                return false;
            }
            return true;
        }

        bool operator>(const static_string& str) const {
            if (size_ < str.size_) {
                return false;
            }
            for (size_t i = 0; i <= size_-1; i++) {
                if (data_[i] < str.data_[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator>=(const static_string& str) const {
            if (size_ < str.size_) {
                return false;
            }
            for (size_t i = 0; i <= size_-1; i++) {
                if (data_[i] < str.data_[i] || data_[i] != str.data_[i]) {
                    return false;
                }
            }
            return true;
        }

        // Insertion and extraction operators for using cout and cin
        friend std::ostream& operator<<(std::ostream& os, const static_string& str) {
            os << str.data_;
            return os;
        }

        friend std::istream& operator>>(std::istream& is, const static_string& str) {
            is >> str.data_;
            return is;
        }

        // Returns pointer to stored characters
        [[nodiscard]] char* data() const {
            return data_;
        }

        [[nodiscard]] char& front() const {
            return data_[0];
        }

        [[nodiscard]] char& back() const {
            return data_[size_ - 1];
        }

        [[nodiscard]] bool empty() const {
            return size_ == 0;
        }

        [[nodiscard]] size_t length() const {
            return size_;
        }

        // Swaps contents with another string
        void swap(const static_string& str) const {
            const size_t shortest = (str.length() < size_) ? str.length() : size_;
            for (size_t i = 0; i <= shortest-1; i++) {
                std::swap(data_[i], str.data_[i]);
            }
        }

        // Returns reference to element at index position
        [[nodiscard]] char& at(const size_t index) const {
            return data_[index];
        }

        // Element-wise reversal allows for easier backward iteration
        void reverse() const {
            if (size_ < 2) {
                return;
            }
            for (size_t i = 0; i < size_/2; i++) {
                std::swap(data_[i], data_[size_ - 1 - i]);
            }
        }

        // Returns position of first char in a substring. Returns 0 if search fails
        [[nodiscard]] size_t find(const static_string& substr, const size_t index=0) const {
            if (empty() || substr.empty()) {
                return 0;
            }

            // Outer loop checks each char of parent string for first char of substring.
            // If found, inner loop will check for matching remaining characters.
            // Inner loop will either break early upon failure, or return index position upon successful full pass.
            size_t position = 0;
            size_t jump = 0;
            for (size_t i = index; i <= size_-1; i++) {
                if (data_[i] == substr[0]) {
                    for (size_t j = 0; j <= substr.length()-1;) {
                        if (data_[i+jump] == substr[j]) {
                            j++;
                            jump++;
                        } else {
                            position = 0;
                            jump = 0;
                            break;
                        }
                        if (j == substr.length()-1) {
                            position = i;
                            return position;
                        }
                    }
                }
            }
            return position;
        }
    };
}

#endif //CONTAINER_LITE_LIB_STATIC_STRING_HPP