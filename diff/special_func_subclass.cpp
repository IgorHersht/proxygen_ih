struct B{
    
    B(int ib, const std::string& sb):_ib(ib), _sb(new std::string(sb)){}
    B(const B& b):_ib(b._ib), _sb( new std::string(*(b._sb)) ){
    }
    B& operator =(const B& b){
        if(&b == this){
            return *this;
        }
        delete _sb;
        _sb = nullptr;

        _ib = b._ib;
        if(b._sb){
            _sb = new std::string(*(b._sb));
        }

        return *this;
    }
    B(B&& b):_ib(b._ib),_sb(b._sb) {
        b._ib = 0;
        b._sb = nullptr;
    }

    B& operator =(B&& b){
        if(&b == this){ // ???
            return *this;
        }
        _ib = b._ib;
        delete _sb;
        _sb = b._sb;

        b._ib = 0;
        b._sb = nullptr;
        return *this;
    }

    virtual ~B(){
        delete _sb;
    }

private:
    int _ib{};
    std::string* _sb{nullptr};
};

struct D : B{
    D(int ib, const std::string& sb, const std::string& sd ):B(ib, sb), _sd(sd){}
    D(const D& d):B(d),_sd(d._sd) {
    }

    D& operator =(const D& d){
        if(&d == this){
            return *this;
        }
        B::operator=(d);
        _sd = d._sd;
        return *this;
    }

    D(D&& d):B(std::move(d)),_sd(std::move(d._sd)){
    }

    D& operator =(D&& d) {
        if (&d == this) { // ???
            return *this;
        }
        B::operator=(std::move(d));
        _sd = std::move(d._sd);
        return *this;
    }
private:
    std::string _sd;
};

int main() {
    D d1(1, "bb1", "dd1");
    D d2(d1);

    D d3(3, "bb3", "dd3");
    d3 = d1;

    D d4(std::move(d1));
    d2 = std::move(d3);
    int i = 1;


}

