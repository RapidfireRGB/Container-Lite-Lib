#ifndef CONTAINER_LITE_LIB_LIGHT_LIST_HPP
#define CONTAINER_LITE_LIB_LIGHT_LIST_HPP
#include <cstddef>

namespace container_lite {
    template <typename T>
    struct node {
        T data;
        node<T>* next;

        explicit node(T element) : data(element), next(nullptr) {}
    };
    template <typename T, size_t MaxBytes=1024-sizeof(size_t)>
    class light_list {
    private:
        node<T>* head;
        size_t size_;
        static constexpr size_t capacity_ = MaxBytes / sizeof(node<T>);

    public:
        explicit light_list(T first_element) : head(), size_(0) {
            node<T>* new_node = new node<T>(first_element);
            head = new_node;
            size_++;
        }

        light_list() : head(nullptr), size_(0) {}

        ~light_list() {}

        void push_back(T element) {
            if (size_ == capacity_) {
                return;
            }
            node<T>* new_node = new node<T>(element);
            node<T>* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new_node;
            size_++;
        }

        void push_front(T element) {
            if (size_ == capacity_) {
                return;
            }
            node<T>* new_node = new node<T>(element);
            new_node->next = head;
            head = new_node;
            size_++;
        }

        T& front() {
            return head->data;
        }

        T& back() {
            node<T>* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            return temp->data;
        }

        const T& front() const {
            return head->data;
        }

        const T& back() const {
            node<T>* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            return temp->data;
        }

    };
}

#endif //CONTAINER_LITE_LIB_LIGHT_LIST_HPP