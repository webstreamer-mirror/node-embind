
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>

struct ValueObject {
    ValueObject()
        : v1(100), v2(200), v3(300), v4(400)
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

int  v3Get(const ValueObject& vo) { return vo.v3; }
void v3Set(ValueObject& vo,int v) { vo.v3 = v; }

ValueObject take_and_return_ValueObject(ValueObject v) {
    return v;
}

//void take_and_return_ValueObject(ValueObject v) {
//    std::cout << "---------------" << std::endl;
//}
EMSCRIPTEN_BINDINGS(value_boject) {

    using namespace emscripten;
    value_object<ValueObject>("ValueObject")
        .field("v1", &ValueObject::v1)
        .field("v2", &ValueObject::v2Get, &ValueObject::v2Set)
        .field("v3", &v3Get, &v3Set)
        .field("v4", emscripten::index<3>())
        ;
    function("take_and_return_ValueObject", &take_and_return_ValueObject);

}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
