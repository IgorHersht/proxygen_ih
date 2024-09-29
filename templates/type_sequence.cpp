//
template<typename... TS> struct type_sequence{};
template<auto... > struct non_type_sequence{};

int main()
{
    enum x{y};
    type_sequence<int, double> s1;
    non_type_sequence<1, y, 'c'> nt;
    TrackNew::reset();



    TrackNew::status();

}


