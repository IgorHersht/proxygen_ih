#include <unordered_map>
#include <string>
#include <iostream>


using namespace std;

class Customer {
private:
    string fname;
    string lname;
    long   no;
public:
    Customer (const string& fn, const string& ln, long n)
            : fname(fn), lname(ln), no(n) {}
    friend ostream& operator << (ostream& strm, const Customer& c) {
        return strm << "[" << c.fname << "," << c.lname << ","
                    << c.no << "]";
    }
    friend class CustomerHash;
    friend class CustomerEqual;
};

class CustomerHash
{
public:
    std::size_t operator() (const Customer& c) const {
        return c.fname.length() + c.lname.length() + c.no;
    }
};

class CustomerEqual
{
public:
    bool operator() (const Customer& c1, const Customer& c2) const {
        return c1.no == c2.no;
    }
};

int main()
{
    // unordered set with own hash function and equivalence criterion
    unordered_map<Customer,std::string, CustomerHash,CustomerEqual> map;
    Customer c1("nico","josuttis",42);
    Customer c2("dico0","josut",43);

    map.emplace(std::move(c1), "111111");
    map.emplace(std::move(c2), "2222");
    int i =1;

}
