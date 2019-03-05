
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>


class Function {
public:
    Function()
        : V1(0), V2(0)
    {
        counter++;
    }

    int V1;
    int V2;


    int getV1()  { return V1; }
    int getV2() const { return V2; }
    void setV1(int v)  { V1 = v; }
    void setV2(int v)  { V2 = v; }
    void set(int v1, int v2) { V1 = v1; V2 = v2; }
    
    static int counter;
    static int Counter() { return counter; }
    static void SetCounter(int c) { counter=c; }
};

int Function::counter = 0;

int  GetV1(Function& c)  { return c.V1; }
int  GetV2(Function& c)  { return c.V2; }
void SetV1(Function& c,int v) { c.V1 = v; }
void SetV2(Function& c,int v) { c.V2 = v; }
void Set(Function& c,int v1, int v2) 
{ 
    c.V1 = v1; 
    c.V2 = v2; 
}


EMSCRIPTEN_BINDINGS(function)
{
    using namespace emscripten;

    // Property
    class_<Function>("Function")
        .constructor<>()
        .function("getV1", &Function::getV1)
        .function("getV2", &Function::getV2)
        .function("setV1", &Function::setV1)
        .function("setV2", &Function::setV2)
        .function("set", &Function::set)

        .function("GetV1", select_overload<int(Function&)>(&GetV1))
        .function("GetV2", select_overload<int(Function&)>(&GetV2))
        .function("SetV1", select_overload<void(Function&,int)>(&SetV1))
        .function("SetV2", select_overload<void(Function&,int)>(&SetV2))
        .function("Set", select_overload<void(Function&,int,int)>(&Set))

        .class_function("Counter",&Function::Counter)
        .class_function("SetCounter", &Function::SetCounter)
        ;
    
    
}

