#include "controllable.h"

int Controllable::control = 5;
void construct(Controllable *p, const Controllable &obj) { new (p) Controllable(obj); }
void destroy(Controllable *arr) { arr->~Controllable(); }
void destroy(Controllable *start, Controllable *end) {
    while(start != end) 
        destroy(start++);
}

struct MyVectorBuffer {
protected:
    Controllable *arr_;
    size_t size_, used_ = 0;

protected:
    MyVectorBuffer(const MyVectorBuffer& other) = delete;
    MyVectorBuffer &operator=(const MyVectorBuffer&) = delete;
    MyVectorBuffer(MyVectorBuffer&& other) noexcept 
        : arr_(other.arr_), size_(other.size_), used_(other.used_) {
            other.arr_ = nullptr;
            other.size_ = 0;
            other.used_ = 0;
        }
    MyVectorBuffer &operator=(MyVectorBuffer &&other) noexcept {
        if (this == &other) return *this;
        std::swap(arr_, other.arr_);
        std::swap(size_, other.size_);
        std::swap(used_, other.used_);
        return *this;
    }
    MyVectorBuffer(size_t sz = 0) 
        : arr_((sz == 0) ? nullptr : static_cast<Controllable*>(::operator new(sizeof(Controllable) * sz))), size_(sz)
    {}
    ~MyVectorBuffer() {
        destroy(arr_, arr_ + used_);
        ::operator delete(arr_);
    }
};

class MyVector : private MyVectorBuffer {
private:
    using MyVectorBuffer::arr_;
    using MyVectorBuffer::size_;
    using MyVectorBuffer::used_;
    
public:
    explicit MyVector(size_t sz = 0) : MyVectorBuffer(sz) {} 
    
    MyVector(const MyVector &other) : MyVectorBuffer(other.used_) {
        while(used_ < other.used_) {
            construct(arr_ + used_, other.arr_[used_]);
            ++used_;
        }
    }
    MyVector(MyVector &&other) noexcept = default;
    
    MyVector &operator=(MyVector &&other) noexcept = default;

    MyVector &operator=(const MyVector &other) {
        if (this == &other) return *this;
        MyVector tmp(other);
        std::swap(*this, tmp);
        return *this;
    }

    Controllable pop() {
        if (used_ < 1) 
            throw std::runtime_error("My vector is EMPTY. Nothing to pop");
        --used_;
    }

    Controllable back() {
        return arr_[used_ - 1];
    }

    void push(const Controllable & obj) {
        if (used_ == size_) {
            std::cout << "Realloc\n";
            MyVector tmp(size_ * 2 + 1);
            while(tmp.used_ < used_)
                tmp.push(arr_[tmp.used_]);
            tmp.push(obj);
            std::swap(tmp, *this);
            return;
        }
        construct(arr_ + used_, obj);
        ++used_;
    }
    size_t size() const noexcept { return size_; }
};

void test() {
    Controllable c1, c2, c3;
    MyVector vec(1);
    vec.push(c1);
    vec.push(c2);
    vec.push(c3);
    MyVector copy_vec{vec};
    std::cout << copy_vec.size() << std::endl;
}

int main() {
    try { test(); }
    catch(std::bad_alloc &ex) {
        std::cout << "Exception caught!";
    }
    return 0;
}