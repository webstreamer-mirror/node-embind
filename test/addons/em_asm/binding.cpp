
#include <algorithm>
#include <time.h>
#include <iostream>
#include <cctype>
#include <codecvt>

#include <emscripten/em_asm.h>
using namespace emscripten;




struct embind_EM_ASM {
    // EM_ASM

    static void case_1() {
        EM_ASM({
        out("  takes ints: " + $0);
            },
            5);
    }

    static void case_2() {
        EM_ASM({
        out("  takes doubles: " + $0);
            }, 5.0675);
    }

    //static void case_3() {
    //    EM_ASM({ 
    //    out("  takes strings: " + Pointer_stringify($0));
    //	return 7.75;     
    //    }, "string arg");
    //}

    static void case_4() {
        EM_ASM({
        out("  takes multiple ints: " + $0 + ", " + $1);
        return 6;
            }, 5, 7);
    }

    //static void case_5() {
    //    EM_ASM({ 
    //    out("  mixed arg types: " + $0 + ", " + Pointer_stringify($1) + ", " + $2);
    //	return 8.125;     
    //    }, 3, "hello", 4.75);
    //}

    static void case_6() {
        EM_ASM({
        out("  ignores unused args");
        return 5.5;
            }, 0);
    }

    static void case_7() {
        EM_ASM({
        out("  skips unused args: " + $1);
        return 6;
            }, 5, 7);
    }

    static void case_8() {
        EM_ASM({
        out("  " + $0 + " + " + $2);
        return $0 + $2;
            }, 5.5, 7.0, 14.375);
    }
};



struct embind_EM_ASM_INT {

    //EM_ASM_INT
    static  int case_1() {
        return EM_ASM_INT({
        out("  takes ints: " + $0);
        return $0;
            }, 5);
    }

    static  int case_2() {
        return EM_ASM_INT({
        out("  takes doubles: " + $0);
        return $0;
            }, 5.0675);
    }

    //static  int case_3() {
    //    return EM_ASM_INT({ 
    //    out("  takes strings: " + Pointer_stringify($0));
    //	return 7.75;     
    //    }, "string arg");
    //}

    static  int case_4() {
        return EM_ASM_INT({
        out("  takes multiple ints: " + $0 + ", " + $1);
        return 6;
            }, 5, 7);
    }

    //static  int case_5() {
    //    return EM_ASM_INT({ 
    //    out("  mixed arg types: " + $0 + ", " + Pointer_stringify($1) + ", " + $2);
    //	return 8.125;     
    //    }, 3, "hello", 4.75);
    //}

    static  int case_6() {
        return EM_ASM_INT({
        out("  ignores unused args");
        return 5.5;
            }, 0);
    }

    static  int case_7() {
        return EM_ASM_INT({
        out("  skips unused args: " + $1);
        return 6;
            }, 5, 7);
    }

    static  int case_8() {
        return EM_ASM_INT({
        out("  " + $0 + " + " + $2);
        return $0 + $2;
            }, 5.5, 7.0, 14.375);
    }

};

struct embind_EM_ASM_DOUBLE {

    static  double case_1() {
        return EM_ASM_DOUBLE({
        out("  takes ints: " + $0);
            }, 5);
    }

    static  double case_2() {
        return EM_ASM_DOUBLE({
        out("  takes doubles: " + $0);
            }, 5.0675);
    }

    //static  double case_3() {
    //    return EM_ASM_DOUBLE({ 
    //    out("  takes strings: " + Pointer_stringify($0));
    //	return 7.75;     
    //    }, "string arg");
    //}

    static  double case_4() {
        return EM_ASM_DOUBLE({
        out("  takes multiple ints: " + $0 + ", " + $1);
        return 6;
            }, 5, 7);
    }

    //static  double case_5() {
    //    return EM_ASM_DOUBLE({ 
    //    out("  mixed arg types: " + $0 + ", " + Pointer_stringify($1) + ", " + $2);
    //	return 8.125;     
    //    }, 3, "hello", 4.75);
    //}

    static  double case_6() {
        return EM_ASM_DOUBLE({
        out("  ignores unused args");
        return 5.5;
            }, 0);
    }

    static  double case_7() {
        return EM_ASM_DOUBLE({
        out("  skips unused args: " + $1);
        return 6;
            }, 5, 7);
    }

    static  double case_8() {
        return EM_ASM_DOUBLE({
        out("  " + $0 + " + " + $2);
        return $0 + $2;
            }, 5.5, 7.0, 14.375);
    }

};




struct ParameterPack {

    template <typename... Args>
    static int call(Args... args) {
        return(EM_ASM_INT(
            {
              out(Array.prototype.join.call(arguments, ','));
            },
            args...
                ));
    }

    static void case_1() {
        call(1);
    }
    static void case_2() {
        call(1, 2);
    }

    static void case_3() {
        call(1, 2, 3);
    }

};



static int unused_arguments(int a, int unused, int b) {
    int sum = EM_ASM_INT({
        return $0 + $2;
        }, a, unused, b);
    return sum;
}


static int wiht_comma_max(int x, int y) {
    int n = EM_ASM_INT({
        return Math.max($0,$1)
        }, x, y);
    return n;
}
static int Main() {
    EM_ASM({
        console.log("A","B");
return 6;
        }, 5, 7);

    return 0;
}
EMSCRIPTEN_BINDINGS(binding)
{
    function("Main", Main);
    function("unused_arguments", unused_arguments);
    function("wiht_comma_max", wiht_comma_max);

    class_<embind_EM_ASM>("EM_ASM")
        .class_function("case_1", &embind_EM_ASM::case_1)
        .class_function("case_2", &embind_EM_ASM::case_2)
        //.class_function("case_3", &embind_EM_ASM::case_3)
        .class_function("case_4", &embind_EM_ASM::case_4)
        //.class_function("case_5", &embind_EM_ASM::case_5)
        .class_function("case_6", &embind_EM_ASM::case_6)
        .class_function("case_7", &embind_EM_ASM::case_7)
        .class_function("case_8", &embind_EM_ASM::case_8)
        ;

    class_<embind_EM_ASM_INT>("EM_ASM_INT")
        .class_function("case_1", &embind_EM_ASM_INT::case_1)
        .class_function("case_2", &embind_EM_ASM_INT::case_2)
        //.class_function("case_3", &embind_EM_ASM_INT::case_3)
        .class_function("case_4", &embind_EM_ASM_INT::case_4)
        //.class_function("case_5", &embind_EM_ASM_INT::case_5)
        .class_function("case_6", &embind_EM_ASM_INT::case_6)
        .class_function("case_7", &embind_EM_ASM_INT::case_7)
        .class_function("case_8", &embind_EM_ASM_INT::case_8)
        ;

    class_<embind_EM_ASM_DOUBLE>("EM_ASM_DOUBLE")
        .class_function("case_1", &embind_EM_ASM_DOUBLE::case_1)
        .class_function("case_2", &embind_EM_ASM_DOUBLE::case_2)
        //.class_function("case_3", &embind_EM_ASM_DOUBLE::case_3)
        .class_function("case_4", &embind_EM_ASM_DOUBLE::case_4)
        //.class_function("case_5", &embind_EM_ASM_DOUBLE::case_5)
        .class_function("case_6", &embind_EM_ASM_DOUBLE::case_6)
        .class_function("case_7", &embind_EM_ASM_DOUBLE::case_7)
        .class_function("case_8", &embind_EM_ASM_DOUBLE::case_8)
        ;

    class_<ParameterPack>("ParameterPack")
        .class_function("case_1", &ParameterPack::case_1)
        .class_function("case_2", &ParameterPack::case_2)
        .class_function("case_3", &ParameterPack::case_3)
        ;


}

#ifdef EMSCRIPTEN_NODE_EMBIND
#include <emscripten/node-embind>
#endif
