
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>

struct ValueArray {
    ValueArray()
        : v1(100), v2(200), v3(300), v4(400)
    {}
    ValueArray(int _1,int _2 ,int _3 ,int _4)
        : v1(_1), v2(_2), v3(_3), v4(_4)
    {}
    int v1;
    int v2;
    int v3;
    int v4;
    int& operator[](int i) {
        return (&v1)[i];
    }
    int v2Get() const { return v2; }
    void v2Set(int v) { v2 = v; }

};

int  v3Get(const ValueArray& vo) { return vo.v3; }
void v3Set(ValueArray& vo,int v) { vo.v3 = v; }

ValueArray take_and_return_ValueArray(ValueArray v) {
    return v;
}

ValueArray return_ValueArray() {
    return ValueArray(1,2,3,4);
}

void xValueArray(ValueArray v) {
    
}



EMSCRIPTEN_BINDINGS(value_boject) {

    using namespace emscripten;
    value_array<ValueArray>("ValueArray")
        .element(&ValueArray::v1)
        .element(&ValueArray::v2Get, &ValueArray::v2Set)
        .element(&v3Get, &v3Set)
        .element(emscripten::index<3>())
        ;
    function("take_and_return_ValueArray", &take_and_return_ValueArray);
    function("return_ValueArray", &return_ValueArray);
    function("xValueArray", &xValueArray);

}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
