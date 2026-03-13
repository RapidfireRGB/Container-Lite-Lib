#ifndef CONTAINER_LITE_LIB_STATIC_MAP_HPP
#define CONTAINER_LITE_LIB_STATIC_MAP_HPP
#include "internal/pair.hpp"
#include "internal/static_vector.hpp"
#include <functional>
#include <iostream>
#include <stdexcept>

// Fixed capacity hashmap which uses fixed capacity vector to store pairs
// Handles collisions by incrementing for next available index
namespace container_lite {
    template <typename K, typename V, std::size_t N>
    class static_map {
    private:
        static const size_t entries = N;        // Max N. of key value pairs
        static_vector<pair<K, V>, N> data_;     // Contiguous storage for pairs
        size_t size_;                           // Current N. of pairs

        size_t hash_function(K key) {
            const size_t hash = std::hash<K> {} (key);
            return hash;
        }

        // Suppose index = hash(key) % N. of pairs
        size_t index(K key) {
            return hash_function(key) % entries;
        }

        // Returns the first available index, or N if none
        size_t probe() {
            for (size_t i = 0; i < entries; i++) {
                if (!data_[i]) {
                    return i;
                }
            }
            return entries;
        }

    public:

        static_map() : data_(), size_(0) {}

        ~static_map() {
            for (size_t i = size_; i > 0; i--) {
                data_[i].~pair();
            }
            data_[0].~pair();
            ~data_();
        }

        bool insert(K key, V value) {
            if (size_ >= entries) {
                return false;
            }

            size_t i = index(key);
            if (data_[i].hash_ == hash_function(key)) {
                if (key != data_[i].first_) {
                    data_[i].first_ = key;
                }
                if (value != data_[i].second_) {
                    data_[i].second = value;
                }
                return true;
            }

            if (!data_[i]) {
                data_[i] = pair(key, value);
                data_[i].hash_ = hash_function(key);
                size_++;
                return true;
            }

            return false;
        }

        // Should overwrite value if key exists, else insert. Should also return Value at key
        V& operator[](K key) {
            size_t i = index(key);
            if (!data_[i]) {
                data_[i] = pair(key, V{});
                data_[i].hash_ = hash_function(key);
                size_++;
                return data_[i].second_;
            }
            if (probe() < entries) {
                size_t j = probe();
                data_[j] = pair(key, V{});
                data_[j].hash_ = hash_function(key);
                size_++;
                return data_[j].second_;
            }

            // Placeholder until I figure out better solution
            throw std::runtime_error("Static Map is full");
        }

        // Returns pointer to value if it exists, else nullptr
        V* find(K key) {
            if (data_[index(key)]) {
                return &data_[index(key)];
            }
            return nullptr;
        }


        // For debugging; not intended for final version
        void print() {
            for (size_t i = 0; i < size_; i++) {
                std::cout << "[" << i << "]: " << data_[i].first_ << ", " << data_[i].second_ << "\n";
            }
        }

        size_t size() const {
            return size_;
        }

        size_t capacity() const {
            return entries;
        }

    };
}
#endif //CONTAINER_LITE_LIB_STATIC_MAP_HPP