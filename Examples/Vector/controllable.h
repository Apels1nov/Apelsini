#pragma once

#include <iostream>
#include <stdexcept>
#include <utility>

struct Controllable {
    static int control;
    int *resource_;
    Controllable() : resource_(new int(42)) {}
    Controllable(const Controllable &other) : resource_(new int(*other.resource_)) {
        if (control == 0) {
            control = 5;
            std::cout << "Control reached!\n";
            throw std::bad_alloc{};
        }
        --control;
    }
    Controllable(Controllable &&other) noexcept : resource_(other.resource_) {
        other.resource_ = nullptr;
    }
    Controllable &operator=(const Controllable &other) {
        if (this == &other) return *this;
        Controllable tmp{other};
        std::swap(*this, tmp);
        return *this;
    }
    Controllable &operator=(Controllable &&other) {
        if (this == &other) return *this;
        std::swap(resource_, other.resource_);
        return *this;
    }
    ~Controllable() { delete resource_; }
};