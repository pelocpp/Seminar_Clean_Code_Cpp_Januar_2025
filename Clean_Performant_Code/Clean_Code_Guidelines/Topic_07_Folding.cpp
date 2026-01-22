// =====================================================================================
// Folding.cpp // Variadic Templates // Folding
// =====================================================================================

#include <print>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <iostream>
#include <functional>

namespace Folding_Seminar {

    template <typename ... TArgs> // TArgs: einpacken von Typen
  
    auto addierer(TArgs ... args) {  // einpacken: parameter pack für args // WERTE

        // std::initializer_list

        auto list = { args ... };                     // auspacken

        int result{};

        std::for_each(
            list.begin(),
            list.end(),
            [&result](int value) {
                result += value;
            }
        );

        return result;
    }

    template <typename ... TArgs> // TArgs: einpacken von Typen
    auto addiererFolding(TArgs ... args) {  // einpacken: parameter pack für args // WERTE

        // int sum = (a1 + a2) + a3 + a4 + a5 + a6 + a7 + a8;
        //          ((pack1 + pack2) + ...) + packN
        int result = ( ... + args );

        return result;
    }

    template <typename ... TArgs> 
    auto subtrahierer(TArgs ... args) {

        // Beispiel:
        //    1 - 2 - 3 = -4
        //    (1 - (2 - 3)) = +2

        // int result = (... - args);
        int result = (args - ...);

        return result;
    }

    template <typename ... TArgs>
    void printer (TArgs ... args) {

        // std::cout << a1 << a2 << a3 << a4 ......
        // (((init op pack1) op pack2) op ...) op packN

        (std::cout << ... << args);
    }

    template <typename TFirst, typename ... TArgs>
    void printerImproved(TFirst first, TArgs ... args) {

        // std::cout << a1 << a2 << a3 << a4 ......
        // (((init op pack1) op pack2) op ...) op packN

        std::cout << first;

        (...  ,  (std::cout << " - " << args) );

    }

    void test_folding_seminar() {

        auto result = addiererFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

        result = subtrahierer(1, 2, 3);

       // printer(1, " - ", 2, " - ", 3, " - ", 4, " - ", 5);

        printerImproved(1, 2, 3, 4, 5);

    }
}

namespace Folding {

    /* folding examples: introduction
    */

    template<typename... TArgs>
    static auto anotherAdder(TArgs... args) {
        return (... + args);  // unary left fold
    }

    static void test_01() {
        int sum = anotherAdder(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        std::cout << "Sum from 1 up to 10: " << sum << std::endl;
    }

    template<typename... TArgs>
    static void printer(TArgs... args) {
        // binary left fold (init == ostream)
        (std::cout << ... << args) << std::endl;
    }

    // demonstrating fold expressions
    static void test_02() {
        printer(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // =================================================================================
    /* demonstrating all four versions of folding expressions
    */

    template<typename... TArgs>
    static auto anotherSubtracterBRF(TArgs... args) {
        return (args - ... - 0);  // binary right fold (init == 0)
    }

    static void test_03a() {
        // binary right fold: 1 - (2 - (3 - (4 - ( 5 - 0)))) = 3
        int result = anotherSubtracterBRF(1, 2, 3, 4, 5);
        std::cout << "BRF: 1 - (2 - (3 - (4 - ( 5 - 0)))): " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterBLF(TArgs... args) {
        return (0 - ... - args);  // binary left fold (init == 0)
    }

    static void test_03b() {
        // binary left fold: ((((0 - 1) - 2) - 3) - 4) - 5 =  -15
        int result = anotherSubtracterBLF(1, 2, 3, 4, 5);
        std::cout << "BLF: ((((0 - 1) - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterURF(TArgs... args) {
        return (args - ...);  // unary right fold
    }

    static void test_03c() {
        // unary right fold: 1 - (2 - (3 - (4 - 5))) = 3
        int result = anotherSubtracterURF(1, 2, 3, 4, 5);
        std::cout << "URF: 1 - (2 - (3 - (4 - 5))): " << result << std::endl;
    }

    // ----------------------------------------------------------------------

    template<typename... TArgs>
    static auto anotherSubtracterULF(TArgs... args) {
        return (... - args);  // unary left fold
    }

    static void test_03d() {
        // unary left fold: ((((1 - 2) - 3) - 4) - 5 = -13
        int result = anotherSubtracterULF(1, 2, 3, 4, 5);
        std::cout << "URF: ((((1 - 2) - 3) - 4) - 5: " << result << std::endl;
    }

    // -----------------------------------------------------------------------

    // Folding over a comma: ',' operator
    // (left or right folding is the same in this case)

    template <typename... TArgs>
    static void printerWithSeperatorRight(TArgs... args) {
        std::string sep = " ";
        ((std::cout << args << sep), ...) << std::endl;
    }

    template <typename... TArgs>
    static void printerWithSeperatorLeft(TArgs... args) {
        std::string sep = " ";
        (..., (std::cout << args << sep)) << std::endl;
    }

    // demonstrating fold expressions
    static void test_04() {
        printerWithSeperatorRight(1, 2, 3, "ABC", "DEF", "GHI");
        printerWithSeperatorLeft(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Folding over a comma - pack arguments can be handled by a separate function

    template <typename T>
    static std::ostream& handleArg(T arg) {
        std::cout << arg << "-";
        return std::cout;
    }

    template <typename... TArgs>
    static void printerWithSeperator(TArgs... args) {
        (handleArg(args), ...) << std::endl;
    }

    // demonstrating fold expressions
    static void test_05() {
        printerWithSeperator(1, 2, 3, "ABC", "DEF", "GHI");
    }

    // -----------------------------------------------------------------------

    // Performance Comparison
    static auto addFolding(auto ... values) {
        return (... + values);
    }

    static auto addIterating(auto ... values) {

        auto list = { values ... };

        //using ElemType = decltype (list)::value_type;
        //ElemType sum{};

        auto sum{ 0 };
        for (auto elem : list) {
            sum += elem;
        }

        return sum;
    }

#ifdef _DEBUG
    constexpr size_t MaxIterations = 10'000'000;     // debug
#else
    constexpr size_t MaxIterations = 100'000'000;    // release
#endif
}


void main_folding()
{
    using namespace Folding;

    Folding_Seminar::test_folding_seminar();
    return;
    test_01();
    test_02();
    test_03a();
    test_03b();
    test_03c();
    test_03d();
    test_04();
    test_05();

}

// =====================================================================================
// End-of-File
// =====================================================================================
