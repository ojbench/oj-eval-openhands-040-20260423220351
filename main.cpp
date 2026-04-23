
#include <iostream>
#include <memory>
#include <optional>

template <typename T>
class DoublyLinkedList {
private:
    // Node structure definition
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    // Iterator class
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(node) {}

        // Dereference operator to get value
        T& operator*() {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            return current->value;
        }

        // Pre-increment operator
        iterator& operator++() {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            current = current->next;
            return *this;
        }

        // Post-increment operator
        iterator operator++(int) {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            iterator temp = *this;
            current = current->next;
            return temp;
        }

        // Equality operators
        bool operator==(const iterator& other) const {
            // 注：比较属于不同容器的非空迭代器是未定义行为，测试数据中不会出现
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };

    // Constructor
    DoublyLinkedList() : head(nullptr), length(0) {}

    // No destructor needed, memory managed by shared_ptr

    // Check if list is empty
    bool empty() const {
        return length == 0;
    }

    // Get the size of the list
    size_t size() const {
        return length;
    }

    // Add element to the front
    void push_front(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        
        if (!head) {
            // List is empty, new node becomes both head and tail
            head = new_node;
            tail = new_node;
        } else {
            // List is not empty, insert at front
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        length++;
    }

    // Add element to the back
    void push_back(const T& value) {
        auto new_node = std::make_shared<Node>(value);
        
        if (!head) {
            // List is empty, new node becomes both head and tail
            head = new_node;
            tail = new_node;
        } else {
            // List is not empty, insert at back
            auto tail_ptr = tail.lock();
            if (tail_ptr) {
                tail_ptr->next = new_node;
                new_node->prev = tail_ptr;
                tail = new_node;
            }
        }
        length++;
    }

    // Remove and return the first element
    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        
        T value = head->value;
        auto old_head = head;
        
        if (head->next) {
            // There are more nodes
            head = head->next;
            head->prev.reset();
        } else {
            // This was the only node
            head = nullptr;
            tail.reset();
        }
        
        length--;
        old_head->next = nullptr; // Break the connection to help with cleanup
        return value;
    }

    // Remove and return the last element
    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
        
        auto tail_ptr = tail.lock();
        if (!tail_ptr) {
            return std::nullopt;
        }
        
        T value = tail_ptr->value;
        
        if (head == tail_ptr) {
            // Only one node in the list
            head = nullptr;
            tail.reset();
        } else {
            // Multiple nodes, remove the last one
            auto prev_ptr = tail_ptr->prev.lock();
            if (prev_ptr) {
                prev_ptr->next = nullptr;
                tail = prev_ptr;
            }
        }
        
        length--;
        tail_ptr->prev.reset(); // Break the connection to help with cleanup
        return value;
    }

    // Iterators
    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
};

// Main function for testing (can be removed for OJ submission)
int main() {
    // Basic test
    DoublyLinkedList<int> list;
    
    // Test push_front and push_back
    list.push_front(10);
    list.push_back(20);
    list.push_front(5);
    list.push_back(30);
    
    std::cout << "List size: " << list.size() << std::endl;
    
    // Test iterator
    std::cout << "List elements: ";
    for (auto it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // Test pop_front and pop_back
    while (!list.empty()) {
        auto front_val = list.pop_front();
        if (front_val) {
            std::cout << "Popped front: " << *front_val << std::endl;
        }
        
        if (!list.empty()) {
            auto back_val = list.pop_back();
            if (back_val) {
                std::cout << "Popped back: " << *back_val << std::endl;
            }
        }
    }
    
    std::cout << "Final list size: " << list.size() << std::endl;
    
    return 0;
}
