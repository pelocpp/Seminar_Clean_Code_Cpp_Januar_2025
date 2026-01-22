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
#include <memory>

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

    void test_folding_seminar_zum_ersten() {

        auto result = addiererFolding(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);

        result = subtrahierer(1, 2, 3);

       // printer(1, " - ", 2, " - ", 3, " - ", 4, " - ", 5);

        printerImproved(1, 2, 3, 4, 5);

    }
    class Unknown {
    private:
        int m_var1;
        int m_var2;
        int m_var3;
        std::string m_data{"ABC"};

    public:
        Unknown(const Unknown& other) 
            : m_var1{ other.m_var1 }, m_var2{ other.m_var2 }, m_var3{ other.m_var3 }
        {
            m_data = other.m_data;
            std::cout << "copy c'tor()" << std::endl;
        }

        Unknown(Unknown&& other) /*noexcept*/
            : m_var1{ other.m_var1 }, m_var2{ other.m_var2 }, m_var3{ other.m_var3 }
        {
            m_data = std::move (other.m_data);
            std::cout << "move c'tor()" << std::endl;
        }

        Unknown() : m_var1{ -1 }, m_var2{ -1 }, m_var3{ -1 } {
            std::cout << "c'tor()" << std::endl;
        }

        Unknown(int n) : m_var1{ n }, m_var2{ -1 }, m_var3{ -1 } {
            std::cout << "c'tor(int)" << std::endl;
        }

        Unknown(int n, int m) : m_var1{ n }, m_var2{ m }, m_var3{ -1 } {
            std::cout << "c'tor(int, int)" << std::endl;
        }

        Unknown(int n, int m, int k) : m_var1{ n }, m_var2{ m }, m_var3{ k } {
            std::cout << "c'tor(int, int, int)" << std::endl;
        }

        friend std::ostream& operator<< (std::ostream&, const Unknown&);
    };

    std::ostream& operator<< (std::ostream& os, const Unknown& obj) {
        os
            << "var1: " << obj.m_var1
            << ", var2: " << obj.m_var2
            << ", var3: " << obj.m_var3;

        return os;
    }

    void test_folding_seminar_move_semantic() {

        std::vector<Unknown> vec;

        //vec.push_back( Unknown{1, 2, 3} );

        //vec.push_back(Unknown{ 2, 3, 4 });

        vec.emplace_back(1, 2, 3);

    }


    void dummy(const int& a)
    {

    }

    // Gute Variante, aber sie arbeitet mit Kopien
    template <typename T, typename ... TArgs>
    std::unique_ptr<T> my_make_unique(TArgs ... args) {

        std::unique_ptr<T> tmp{ new T{ args ... } };
        return tmp;
    }

    // Perfekte Variante, sie arbeitet NUR mit Referenzen
    // Beobachtung: Perfekte Weiterleiten
    template <typename T, typename ... TArgs>
    std::unique_ptr<T> my_make_unique_02(TArgs&& ... args) {

        std::unique_ptr<T> tmp{ new T{ args ... } };
        return tmp;
    }


    void test_folding_seminar_mit_unique() {

        std::unique_ptr<int> ptr1 = std::make_unique<int>(123);

        std::unique_ptr<Unknown> ptr2 = std::make_unique<Unknown>(1, 2, 3);

        int a = 123;
        std::unique_ptr<Unknown> ptr3 = my_make_unique_02<Unknown>(a, 2, 3);

        int n = 1;
        dummy(1);

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

    Folding_Seminar::test_folding_seminar_move_semantic();
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
