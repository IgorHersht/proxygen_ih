
constexpr std::array car = {1,2,3};
constexpr std::span c_sp{car};

std::array ar = {4,5,6};
const std::span sp{ar};


void print( const auto& s) {
    for(const auto& el: s) {
        std::cout << el << ' ';
    }
    std::cout << std::endl;
}

void tr( auto& s) {
    for(auto& el: s) {
       ++el;
    }
}
// Must not use vector. Array only
// because It is OK to change value of a element, but not OK to change size!!!
std::vector v1 {1,2,3};
std::span v_sp { v1};



int main() {

    print(sp);

    print(c_sp);
    ////
    tr(sp);
    print(sp);

    sp[0] = 1;
    print(sp);

    ar[2] = 8;
    print(sp);
////
    print(v_sp);
    // It is OK to change value of a element, but not OK to change size!!!
    v1.emplace_back(4); //! Bad!!! undefined runtime v1
    print(v_sp);
    
}
