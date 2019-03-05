
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>

class Constructor {
public:
    Constructor()
        : id(0), mark(0)
    {}

    Constructor(int m)
        : id(0), mark(m)
    {}
    Constructor(int m,int i)
        : id(i), mark(m)
    {}

    ~Constructor() {
    }
    int id;
    int mark;
};

Constructor Constructor_create(int id, int mark) {
    Constructor c;
    c.id = id;
    c.mark = mark;
    return c;
}

int Constructor_id(const Constructor& c) {
    return c.id;
}

int Constructor_mark(const Constructor& c) {
    return c.mark;
}

void Constructor_set_id(Constructor& c,int id) {
    c.id=id;
}

void Constructor_set_mark( Constructor& c,int mark) {
    c.mark = mark;
}

// param is by value, return also by value
Constructor Constructor_val_return_val(Constructor c) {
    return c;
}

Constructor& Constructor_ref_return_ref(Constructor& c) {
    return c;
}

Constructor* Constructor_pointer_return_pointer(Constructor* c) {
    return c;
}

const Constructor& Constructor_const_ref_return_const_ref(const Constructor& c) {
    return c;
}

const Constructor* Constructor_const_pointer_return_const_pointer(const Constructor* c) {
    return c;
}

EMSCRIPTEN_BINDINGS(constructor)
{
    using namespace emscripten;

    // Constructor
    class_<Constructor>("Constructor")
        .constructor<>()
        .constructor<int>()
        .constructor<int,int>()
        ;

    function("Constructor_create", &Constructor_create);
    function("Constructor_id", &Constructor_id);
    function("Constructor_mark", &Constructor_mark);
    function("Constructor_set_id", &Constructor_set_id);
    function("Constructor_set_mark", &Constructor_set_mark);
    function("Constructor_val_return_val", &Constructor_val_return_val);
    function("Constructor_ref_return_ref", &Constructor_ref_return_ref);
    function("Constructor_pointer_return_pointer", &Constructor_pointer_return_pointer);
    function("Constructor_const_ref_return_const_ref", &Constructor_const_ref_return_const_ref);
    function("Constructor_const_pointer_return_const_pointer", &Constructor_const_pointer_return_const_pointer);
    
        
    
}

