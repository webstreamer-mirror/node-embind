
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/bind.h>

static int marker_ = 0;
int marker() { return marker_; }
void marker(int v) { marker_ = v; }

class Empty {
public:
	Empty() {
		marker_++;
	}
	~Empty() {
		marker_--;
	}
};


class ConstMember {
public:
	ConstMember()
		: member(marker_)
	{}

	const int member;
};

class Member {
public:
	int member;
};

class MemberFunction_GetterOnly {
public:
	int get() const { return marker_; }
};

class GlobalFunction_GetterTester {
public:
    int member;
};
int GlobalFunction_Getter(const GlobalFunction_GetterTester& m) {
    return m.member;
}


class MemberFunction{
public:
    int get() const { return member_; }
    void set(int value) { member_ = value; }

    int member_;
};

class GlobalFunction {
public:
    int member;
};
int GlobalFunctionGetter(const GlobalFunction& m) {
    return m.member;
}
void GlobalFunctionSetter(GlobalFunction& m, int value) {
    m.member=value;
}

EMSCRIPTEN_BINDINGS(binding)
{
	using namespace emscripten;
	function("marker", select_overload<int(void)>(marker));
	function("marker", select_overload<void(int)>(marker));
	class_<Empty>("Empty")
		.constructor<>()
        ;

	class_<ConstMember>("ConstMember")
		.constructor<>()
		.property("member", &ConstMember::member)
        ;

	class_<Member>("Member")
		.constructor<>()
		.property("member", &Member::member)
        ;

	class_<MemberFunction_GetterOnly>("MemberFunction_GetterOnly")
		.constructor<>()
		.property("member", &MemberFunction_GetterOnly::get)
        ;

    class_<GlobalFunction_GetterTester>("GlobalFunction_GetterTester")
        .constructor<>()
        .property("member_", &GlobalFunction_GetterTester::member) // as operator
        .property("member", &GlobalFunction_Getter)
        ;

    class_<MemberFunction>("MemberFunction")
        .constructor<>()
        .property("member", &MemberFunction::get,&MemberFunction::set)
        ;
    class_<GlobalFunction>("GlobalFunction")
        .constructor<>()
        .property("member_", &GlobalFunction::member) // as operator
        .property("member", &GlobalFunctionGetter , &GlobalFunctionSetter)
        ;
}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
