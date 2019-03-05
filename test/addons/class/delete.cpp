
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>

static std::map<int, int> ctor_count;
static std::map<int, int> dector_count;
static int ID = 0;

int get_ctor_count(int id) {
    std::map<int, int>::iterator it = ctor_count.find(id);
    if (it == ctor_count.end()) {
        return 0;
    }
    else {
        return ctor_count[id];
    }
}

int get_dector_count(int id) {
    std::map<int, int>::iterator it = dector_count.find(id);
    if (it == dector_count.end()) {
        return 0;
    }
    else {
        return dector_count[id];
    }
}


class Deleter {
public:
    Deleter()
        : id(++ID), mark(0)
    {
        std::map<int, int>::iterator it = ctor_count.find(id);
        if (it == ctor_count.end()) {
            ctor_count[id] = 1;
        }
        else {
            ctor_count[id]++;
        }
    }

    Deleter(const Deleter& d)
        : id(++ID), mark(d.mark)
    {
        std::map<int, int>::iterator it = ctor_count.find(id);
        if (it == ctor_count.end()) {
            ctor_count[id] = 1;
        }
        else {
            ctor_count[id]++;
        }
    }
    ~Deleter() {
        std::map<int, int>::iterator it = dector_count.find(id);
        if (it == dector_count.end()) {
            dector_count[id] = 1;
        }
        else {
            dector_count[id]++;
        }
    }
    int id;
    int mark;
};


int Deleter_id(const Deleter& c) {
    return c.id;
}

int Deleter_mark(const Deleter& c) {
    return c.mark;
}

void Deleter_set_mark( Deleter& c,int mark) {
    c.mark = mark;
}


Deleter Deleter_val_return_val(Deleter c) {
    return c;
}

Deleter& Deleter_ref_return_ref(Deleter& c) {
    return c;
}

Deleter* Deleter_pointer_return_pointer(Deleter* c) {
    return c;
}

const Deleter& Deleter_const_ref_return_const_ref(const Deleter& c) {
    return c;
}

const Deleter* Deleter_const_pointer_return_const_pointer(const Deleter* c) {
    return c;
}

EMSCRIPTEN_BINDINGS(deleter)
{
    using namespace emscripten;

    // Deleter
    class_<Deleter>("Deleter")
        .constructor<>()
        ;

    function("Deleter_id", &Deleter_id);
    function("Deleter_mark", &Deleter_mark);
    function("Deleter_set_mark", &Deleter_set_mark);
    function("Deleter_val_return_val", &Deleter_val_return_val);
    function("Deleter_ref_return_ref", &Deleter_ref_return_ref);
    function("Deleter_pointer_return_pointer", &Deleter_pointer_return_pointer);
    function("Deleter_const_ref_return_const_ref", &Deleter_const_ref_return_const_ref);
    function("Deleter_const_pointer_return_const_pointer", &Deleter_const_pointer_return_const_pointer);
    function("get_ctor_count", &get_ctor_count);
    function("get_dector_count", &get_dector_count);
    
    
}

