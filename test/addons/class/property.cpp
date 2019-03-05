
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>


class Property {
public:
    static const int Counter_Base_ =1000;
    static int Counter_;

    Property()
        : id(Counter_++), mark(0)
    {}

    Property(int mark)
        : id(Counter_++), mark(mark)
    {}

    Property(int mark,int Id)
        : id(Id), mark(mark)
    {
        Counter_++;
    }

    const int id;

    int mark;

    int GetId() const { return id; }

    void  SetMark(int mark) { this->mark = mark; }
    int   GetMark() const { return this->mark; }
    
    static int Counter() { return Counter_; }
};

int Property::Counter_ = 0;

int GetId(const Property& m) {
    return m.id;
}

int GetMark(const Property& m) {
    return m.mark;
}
void SetMark(Property& m, int mark) {
    m.mark = mark;
}



EMSCRIPTEN_BINDINGS(property)
{
    using namespace emscripten;

    // Property
    class_<Property>("Property")
        .constructor<>()
        .constructor<int>()
        .constructor<int,int>()
        .property("id", &Property::id)
        .property("mark", &Property::mark)
        .property("Id", &Property::GetId)
        .property("Mark", &Property::GetMark, &Property::SetMark)
        .property("fId", select_overload<int(const Property&)>(&GetId))
        .property("fMark", select_overload<int(const Property&)>(&GetMark),
            select_overload<void(Property&, int)>(&SetMark))
        .class_property("Counter",&Property::Counter_)
        .class_property("COUNTER_BASE", &Property::Counter_Base_)
        ;
    
    
}

