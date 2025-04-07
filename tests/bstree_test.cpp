#include "bst.h"
#include <algorithm>
#include <functional>
#include <gtest/gtest.h>
#include <iterator>
#include <memory_resource>
#include <print>
#include <set>

// Тестирование заголовка <set>, основанное на книге «The C++ Standard Template
// Library» P.J. Plauger, Alexander A. Stepanov,
//     Meng Lee, David R. Musser. Немного модифицировано, и разбито на отдельные
//     тесты.
//
//  Эти тесты могут быть использованы для тестирования бинарного дерева поиска,
//  написанного по аналогии с библиотечной реализацией STL, причём их можно
//  использовать как для тестирования шаблона set на основе RBT, так и на основе
//  AVL.
//
//  Следует иметь в виду, авторы тестов являются также и авторами реализации
//  контейнера std::set, так что тесты так себе, далеко не все проблемные случаи
//  покрывают, но хотя бы так.

namespace TreeTest
{
template<typename T> using Myal = std::allocator<T>;
template<typename T> using Mypred = std::less<T>;

//     //  Для того, чтобы выполнить тестирование одного из указанных
//     //  контейнеров (std::set или Binary_Tree_Search)
//     //    должна быть раскомментирована одна из следующих строк:
// template<typename T> using ContainerTemplate = std::set<T, Mypred<T>,
// Myal<T>>;
template<typename T>
using ContainerTemplate = BinarySearchTree<T, Mypred<T>, Myal<T>>;

TEST(BaseTest, TreeSizeTest)
{
    ContainerTemplate<int> T1;
    ASSERT_EQ(T1.size(), ContainerTemplate<int>::size_type(0))
        << "Пустое дерево имеет ненулевой размер!";
    ContainerTemplate<int> Tree = {40, 50, 30, 35, 10, 75, 23, 87, 68};
    ASSERT_EQ(Tree.size(), ContainerTemplate<int>::size_type(9))
        << "Неверно указывается размер дерева!";
    ContainerTemplate<int> Tree2(Tree);
    ASSERT_EQ(Tree.size(), Tree2.size())
        << "Неверно указывается размер после копирования!";
}

TEST(BaseTest, TreesEqualityTest)
{
    //  Тестирование оператора == для дерева
    ContainerTemplate<int> T1, T2;
    ASSERT_TRUE(T1 == T2) << "Неверно выполняется сравнение пустых деревьев!";
    ContainerTemplate<int> Tree = {40, 50, 30, 35, 10, 75, 23, 87, 68};
    ContainerTemplate<int> Tree2(Tree);
    ASSERT_TRUE(Tree == Tree2)
        << "Ошибка сравнения деревьев на равенство после "
           " копирования!";
    // ContainerTemplate<int> Tree3{Tree.rbegin(), Tree.rend()};
    // ASSERT_TRUE(Tree == Tree3)
    //     << "Ошибка сравнения деревьев на равенство после "
    //        "копирования в обратном порядке!";
}

TEST(BaseTest, TreeAssignmentTests)
{
    //  Тестирование оператора == для дерева
    ContainerTemplate<int> T1 = {40, 50, 30, 35, 10, 75, 23, 87, 68};
    ContainerTemplate<int> T2;
    T2 = T1;
    ASSERT_TRUE(T1 == T2) << "Ошибка в операторе присваивания!";
    T2.clear();
    T1 = T2;
    ASSERT_TRUE(T1 == T2)
        << "Ошибка в операторе присваивания пустых деревьев !";
    ContainerTemplate<int> T3 = {40, 50, 30, 35, 10, 75, 23, 87, 68};
    T1 = T3;
    T2 = std::move(T3);
    ASSERT_TRUE(T1 == T2) << "Ошибка при перемещающем операторе присваивания!";
}

// //		//  Тесты стандартного контейнера std::set, из книги
// //"The C++
// // Standard template library" Плаугера, Степанова и др.
// //
template<typename T> using Myal = std::allocator<T>;
template<typename T> using Mypred = std::less<T>;
// //
// //		//  Для того, чтобы выполнить тестирование одного из
// // указанных
// // контейнеров (std::set или Binary_Tree_Search)
// //		//    должна быть раскомментирована одна из следующих
// // строк:
// //		//template<typename T> using ContainerTemplate =
// // std::set<T,
// // Mypred<T>, Myal<T>>;
template<typename T>
using ContainerTemplate = BinarySearchTree<T, Mypred<T>, Myal<T>>;
using Mycont = ContainerTemplate<char>;



TEST(SetTests, SetSize)
{
    Mycont v0;
    Myal<char> al = v0.get_allocator();
    Mypred<char> pred;
    Mycont v0a(pred), v0b(pred, al);
    ASSERT_TRUE(v0.empty() && v0.size() == 0) << "Неверный размер пустого set";
    ASSERT_TRUE(v0a.size() == 0 && v0a.get_allocator() == al)
        << "Неверный размер или аллокатор";
    ASSERT_TRUE(v0b.size() == 0 && v0b.get_allocator() == al)
        << "Неверный размер или аллокатор";
}

TEST(SetTests, SetCreation)
{
    char carr[] = "abc", carr2[] = "def";
    Mycont v0;
    Myal<char> al = v0.get_allocator();
    Mypred<char> pred;
    Mycont v0a(pred);

    Mycont v1(carr, carr + 3);
    ASSERT_TRUE(v1.size() == 3 && *v1.begin() == 'a')
        << "Неверно создаётся set символов";

    Mycont v2(carr, carr + 3, pred);
    ASSERT_TRUE(v2.size() == 3 && *v2.begin() == 'a')
        << "Неверно создаётся set символов";

    Mycont v3(carr, carr + 3, pred, al);
    ASSERT_TRUE(v3.size() == 3 && *v3.begin() == 'a')
        << "Неверно создаётся set символов";

    const Mycont v4(carr, carr + 3);
    v0 = v4;
    ASSERT_TRUE(v1.size() == 3 && *v1.begin() == 'a')
        << "Неверно работает оператор присваивания для set";
}

TEST(SetTests, SetIterators)
{
    char ch('a'), carr[] = "abc", carr2[] = "def";
    Mycont v0;
    Myal<char> al = v0.get_allocator();
    Mypred<char> pred;
    Mycont v0a(pred);

    Mycont v1(carr, carr + 3);
    Mycont v2(carr, carr + 3, pred);
    Mycont v3(carr, carr + 3, pred, al);
    const Mycont v4(carr, carr + 3);
    v0 = v4;

    Mycont::iterator p_it(v1.begin());
    Mycont::const_iterator p_cit(v4.begin());
    Mycont::reverse_iterator p_rit(v1.rbegin());
    Mycont::const_reverse_iterator p_crit(v4.rbegin());

    ASSERT_TRUE(*p_it == 'a' && *--(p_it = v1.end()) == 'c')
        << "Декремент end() не корректен?";
    ASSERT_TRUE(*p_cit == 'a' && *--(p_cit = v4.end()) == 'c')
        << "Декремент для const iterator на end() не корректен? ";
    ASSERT_TRUE(*p_rit == 'c' && *--(p_rit = v1.rend()) == 'a')
        << "Reverse iterator не корректен?";
    ASSERT_TRUE(*p_crit == 'c' && *--(p_crit = v4.rend()) == 'a')
        << "Const reverse iterator не корректен? ";
}

TEST(SetTests, SetInsertEraseTests)
{
    char carr[] = "abc", carr2[] = "def";
    Mycont v0;
    Myal<char> al = v0.get_allocator();
    Mypred<char> pred;
    Mycont v0a(pred);

    Mycont v1(carr, carr + 3);
    Mycont v2(carr, carr + 3, pred);
    Mycont v3(carr, carr + 3, pred, al);
    const Mycont v4(carr, carr + 3);
    v0 = v4;

    Mycont::iterator p_it(v1.begin());
    Mycont::const_iterator p_cit(v4.begin());
    Mycont::reverse_iterator p_rit(v1.rbegin());
    Mycont::const_reverse_iterator p_crit(v4.rbegin());

    v0.clear();
    std::pair<Mycont::iterator, bool> pib = v0.insert('d');
    ASSERT_TRUE(*pib.first == 'd' && pib.second);
    ASSERT_TRUE(*--v0.end() == 'd');
    pib = v0.insert('d');
    ASSERT_TRUE(*pib.first == 'd' && !pib.second);
    ASSERT_TRUE(*v0.insert(v0.begin(), 'e') == 'e');
    v0.insert(carr, carr + 3);
    ASSERT_TRUE(v0.size() == 5 && *v0.begin() == 'a');
    v0.insert(carr2, carr2 + 3);
    ASSERT_TRUE(v0.size() == 6 && *--v0.end() == 'f');
    ASSERT_TRUE(*v0.erase(v0.begin()) == 'b' && v0.size() == 5);
    ASSERT_TRUE(*v0.erase(v0.begin(), ++v0.begin()) == 'c' && v0.size() == 4);
    ASSERT_TRUE(v0.erase('x') == 0 && v0.erase('e') == 1);
}


//	TEST_CLASS(SetTests)
//	{
//
//
//
//		TEST_METHOD(SetSwapAndCompTests)
//		{
//			char carr[] = "abc", carr2[] = "def";
//			Mycont v0;
//			Mycont v1(carr, carr + 3);
//
//			v0.clear();
//			std::pair<Mycont::iterator, bool> pib =
// v0.insert('d'); 			pib = v0.insert('d');
// v0.insert(v0.begin(), 'e'); 			v0.insert(carr, carr + 3);
// v0.insert(carr2, carr2
//+ 3); 			v0.erase(v0.begin());
// v0.erase(v0.begin(), ++v0.begin()); 			v0.erase('x');
//			v0.erase('e');
//			v0.clear();
//			ASSERT_TRUE(v0.empty());
//			v0.swap(v1);
//			ASSERT_TRUE(!v0.empty() && v1.empty());
//			std::swap(v0, v1);
//			ASSERT_TRUE(v0.empty() && !v1.empty());
//			ASSERT_TRUE(v1 == v1 && v0 < v1, L"Сравнение
// множеств
// некорректно!"); 			ASSERT_TRUE(v0 != v1 && v1 >
// v0, L"Сравнение множеств некорректно!"); ASSERT_TRUE(v0 <= v1 &&
// v1 >= v0, L"Сравнение множеств некорректно!");
//		}
//
//		TEST_METHOD(SetComparatorTests)
//		{
//			Mycont v0;
//			ASSERT_TRUE(v0.key_comp()('a', 'c') &&
//! v0.key_comp()('a', 'a'), L"Некорректный компаратор!");
//			ASSERT_TRUE(v0.value_comp()('a', 'c') &&
//! v0.value_comp()('a', 'a'), L"Некорректный компаратор!");
//		}
//
//		TEST_METHOD(SetAlgTests)
//		{
//			char carr[] = "abc";
//			const Mycont v4(carr, carr+3);
//
//			ASSERT_TRUE(*v4.find('b') == 'b');
//			ASSERT_TRUE(v4.count('x') == 0 &&
// v4.count('b') ==
// 1); 			ASSERT_TRUE(*v4.lower_bound('a') == 'a',
// L"Метод lower_bound");
// ASSERT_TRUE(*v4.upper_bound('a') == 'b', L"Метод upper_bound");
// std::pair<Mycont::const_iterator, Mycont::const_iterator> pcc =
// v4.equal_range('a'); ASSERT_TRUE(*pcc.first == 'a' && *pcc.second
//== 'b', L"Ошибка метода equal_range");
//		}
//	};
//
//	TEST_CLASS(MultiSetTests)
//	{
//		///  Тесты стандартного контейнера std::multiset, из
// книги "The
// C++ Standard template library" Плаугера, Степанова и др. 	public:
//
//		using Myal = std::allocator<char>;
//		using Mypred = std::less<char>;
//
//		//  Для того, чтобы выполнить тестирование одного из
// указанных
// контейнеров (std::set или Binary_Tree_Search)
//		//    должна быть раскомментирована одна из следующих
// строк:
//		//template<typename T> using ContainerTemplate =
// std::multiset<T, Mypred, Myal>; 		template<typename T>
// using ContainerTemplate = BinarySearchTree<T, Mypred, Myal>;
//
//		using Mycont = ContainerTemplate<char>;
//
//		TEST_METHOD(MultiSetSize)
//		{
//			Mycont v0;
//			Myal al = v0.get_allocator();
//			Mypred pred;
//			Mycont v0a(pred), v0b(pred, al);
//			ASSERT_TRUE(v0.empty() && v0.size() == 0,
// L"Неверный
// размер пустого set");
// ASSERT_TRUE(v0a.size() == 0
// && v0a.get_allocator() == al, L"Неверный размер или аллокатор");
//			ASSERT_TRUE(v0b.size() == 0 &&
// v0b.get_allocator() ==
// al, L"Неверный размер или аллокатор");
//		}
//
//		TEST_METHOD(MultiSetCreation)
//		{
//			char carr[] = "abc", carr2[] = "def";
//			Mycont v0;
//			Myal al = v0.get_allocator();
//			Mypred pred;
//			Mycont v0a(pred);
//
//			Mycont v1(carr, carr + 3);
//			ASSERT_TRUE(v1.size() == 3 && *v1.begin() ==
//'a',
// L"Неверно создаётся set символов");
//
//			Mycont v2(carr, carr + 3, pred);
//			ASSERT_TRUE(v2.size() == 3 && *v2.begin() ==
//'a',
// L"Неверно создаётся set символов");
//
//			Mycont v3(carr, carr + 3, pred, al);
//			ASSERT_TRUE(v3.size() == 3 && *v3.begin() ==
//'a',
// L"Неверно создаётся set символов");
//
//			const Mycont v4(carr, carr + 3);
//			v0 = v4;
//			ASSERT_TRUE(v1.size() == 3 && *v1.begin() ==
//'a',
// L"Неверно работает оператор присваивания для set");
//		}
//
//		TEST_METHOD(MultiSetIterators)
//		{
//			char ch('a'), carr[] = "abc", carr2[] = "def";
//			Mycont v0;
//			Myal al = v0.get_allocator();
//			Mypred pred;
//			Mycont v0a(pred);
//
//			Mycont v1(carr, carr + 3);
//			Mycont v2(carr, carr + 3, pred);
//			Mycont v3(carr, carr + 3, pred, al);
//			const Mycont v4(carr, carr + 3);
//			v0 = v4;
//
//			Mycont::iterator p_it(v1.begin());
//			Mycont::const_iterator p_cit(v4.begin());
//			Mycont::reverse_iterator p_rit(v1.rbegin());
//			Mycont::const_reverse_iterator
// p_crit(v4.rbegin());
//
//			ASSERT_TRUE(*p_it == 'a' && *--(p_it =
// v1.end()) == 'c', L"Декремент end() не корректен?");
// ASSERT_TRUE(*p_cit == 'a' &&
//*--(p_cit = v4.end()) == 'c', L"Декремент для const iterator на end()
// не
// корректен?"); 			ASSERT_TRUE(*p_rit == 'c' &&
// *--(p_rit = v1.rend()) == 'a', L"Reverse iterator не корректен?");
// ASSERT_TRUE(*p_crit == 'c' &&
//*--(p_crit = v4.rend()) == 'a', L"Const reverse iterator не
// корректен?");
//		}
//
//		TEST_METHOD(MultiSetInsertEraseTests)
//		{
//			char carr[] = "abc", carr2[] = "def";
//			Mycont v0;
//			Myal al = v0.get_allocator();
//			Mypred pred;
//			Mycont v0a(pred);
//
//			Mycont v1(carr, carr + 3);
//			Mycont v2(carr, carr + 3, pred);
//			Mycont v3(carr, carr + 3, pred, al);
//			const Mycont v4(carr, carr + 3);
//			v0 = v4;
//
//			Mycont::iterator p_it(v1.begin());
//			Mycont::const_iterator p_cit(v4.begin());
//			Mycont::reverse_iterator p_rit(v1.rbegin());
//			Mycont::const_reverse_iterator
// p_crit(v4.rbegin());
//		}
//
//		TEST_METHOD(MultiSetInsEraseTests)
//		{
//			char carr[] = "abc", carr2[] = "def";
//			Mycont v0;
//			Mycont v1(carr, carr + 3);
//			v0 = v1;
//
//			v0.clear();
//			ASSERT_TRUE(*v0.insert('d') == 'd');
//			ASSERT_TRUE(*--v0.end() == 'd');
//			ASSERT_TRUE(*v0.insert('d') == 'd',
// L"Повторная
// вставка не сработала"); 			ASSERT_TRUE(v0.size()
// == 2, L"Неправильный размер multiset");
// ASSERT_TRUE(*v0.insert(v0.begin(), 'e') == 'e', L"Вставка с
// указанием итератора"); 			v0.insert(carr, carr +
// 3);
//			ASSERT_TRUE(v0.size() == 6 && *v0.begin() ==
//'a'); 			v0.insert(carr2, carr2 + 3);
// ASSERT_TRUE(v0.size() == 9 &&
//*--v0.end() == 'f'); 			ASSERT_TRUE(*v0.erase(v0.begin()) ==
//'b' && v0.size()
//== 8); 			ASSERT_TRUE(*v0.erase(v0.begin(),
//++v0.begin()) == 'c' && v0.size() == 7);
// ASSERT_TRUE(v0.erase('x') == 0 && v0.erase('e') == 2);
//		}
//
//		TEST_METHOD(MultiSetSwapTests)
//		{
//			char carr[] = "abc", carr2[] = "def";
//			Mycont v0;
//			Mycont v1(carr, carr + 3);
//			v0.clear();
//			v0 = v1;
//			v0.insert('d'); v0.insert('d');
//			v0.insert(v0.begin(), 'e');
//			v0.insert(carr, carr + 3);
//			v0.insert(carr2, carr2 + 3);
//			v0.erase(v0.begin());
//			v0.erase(v0.begin(), ++v0.begin());
//			v0.erase('x');
//			v0.erase('e');
//			v0.clear();
//			ASSERT_TRUE(v0.empty(), L"А должно быть
// пустым!"); 			v0.swap(v1);
// ASSERT_TRUE(!v0.empty() && v1.empty(), L"Метод swap
// на multiset сработала неправильно!"); 			swap(v0,
// v1);
//			ASSERT_TRUE(v0.empty() && !v1.empty(),
// L"Операция
// swap на multiset сработала неправильно!");
// ASSERT_TRUE(v1 == v1 && v0 < v1, L"Сравнение некорректно");
// ASSERT_TRUE(v0 != v1 && v1 > v0, L"Сравнение некорректно");
// ASSERT_TRUE(v0 <= v1 && v1 >= v0, L"Сравнение некорректно");
//		}
//
//		TEST_METHOD(MultiSetAlgTests)
//		{
//			char carr[] = "abc", carr2[] = "def";
//			Mycont v0;
//			Mycont v1(carr, carr + 3);
//			Mycont v4(carr, carr + 3);
//			v0.clear();
//			v0 = v1;
//			v0.insert('d'); v0.insert('d');
//			v0.insert(v0.begin(), 'e');
//			v0.insert(carr, carr + 3);
//			v0.insert(carr2, carr2 + 3);
//			v0.erase(v0.begin());
//			v0.erase(v0.begin(), ++v0.begin());
//			v0.erase('x');
//			v0.erase('e');
//			v0.clear();
//			v0.swap(v1);
//			swap(v0, v1);
//
//			ASSERT_TRUE(v0.key_comp() ('a', 'c') &&
//! v0.key_comp() ('a', 'a'), L"Проблема с компаратором ключей");
//			ASSERT_TRUE(v0.value_comp() ('a', 'c') &&
//! v0.value_comp() ('a', 'a'), L"Проблема с компаратором значений");
//			ASSERT_TRUE(*v4.find('b') == 'b', L"Метод
// find");
//
//			ASSERT_TRUE(v4.count('x') == 0 &&
// v4.count('b') ==
// 1); 			ASSERT_TRUE(*v4.lower_bound('a') == 'a',
// L"Метод lower_bound");
// ASSERT_TRUE(*v4.upper_bound('a') == 'b', L"Метод upper_bound");
//
//			std::pair<Mycont::const_iterator,
// Mycont::const_iterator> pcc = v4.equal_range('a');
//			ASSERT_TRUE(*pcc.first == 'a' && *pcc.second
//== 'b',
// L"Ошибка метода equal_range");
// Logger::WriteMessage("Вот так оно и бывает: тесты говорят, что всё в
// норме. Но верить им нельзя!");
//		}
//	};
//
//	//-------------------------------------------------------------------------------------------------------------------
//	// Класс Elem для тестирования правильной работы с памятью
//	class Elem {
//		int data;
//		uint32_t watch;
//		static size_t elem_counter;
//		void check() const { ASSERT_TRUE(watch == 0xDEADBEEF,
// L"Повреждение памяти!! (Обращение к неинициализированному экземпляру
// класса Elem)"); } 	public: 		Elem(const Elem&) =
// delete; Elem& operator=(const Elem&) = delete; 		explicit
// Elem(int value) : data(value), watch(0xDEADBEEF) { ++elem_counter; }
// Elem(Elem&& e) noexcept { e.check(); data = e.data; watch = e.watch;
// ++elem_counter; } 		Elem& operator=(Elem&& e) noexcept {
// check(); e.check(); data = e.data; watch = e.watch; }
// bool operator<(const Elem& e) const { check(); e.check(); return data
// < e.data;
//} 		~Elem() { check(); watch = 0; --elem_counter; }
// static size_t count() { return elem_counter; }
//	};
//	size_t Elem::elem_counter = 0;
//
//	TEST_CLASS(ComplexTypeTests)
//	{
//		///  Немного отсебятины - тесты на сложных структурах
// данных 	public:
//
//		template<typename T> using Myal = std::allocator<T>;
//		template<typename T> using Mypred = std::less<T>;
//
//		//  Для того, чтобы выполнить тестирование одного из
// указанных
// контейнеров (std::set или Binary_Tree_Search)
//		//    должна быть раскомментирована одна из следующих
// строк:
//		//template<typename T> using ContainerTemplate =
// std::set<T,
// Mypred<T>, Myal<T>>; 		template<typename T> using
// ContainerTemplate = BinarySearchTree<T, Mypred<T>, Myal<T>>;
//
//
//		TEST_METHOD(StringTests)
//		{
//			ContainerTemplate<std::string> T1{ "abc", "cde",
//"123", "AAAAAAAA" }; 			std::vector<std::string> check1{
//"123", "AAAAAAAA", "abc", "cde" };
//
//			ASSERT_TRUE(T1.size() == 4, L"Неправильно
// считается
// количество строковых элементов");
// ASSERT_TRUE(std::equal(T1.begin(), T1.end(), check1.begin(),
// check1.end()), L"Неправильный порядок строковых элементов");
// ASSERT_TRUE(std::equal(T1.rbegin(), T1.rend(), check1.rbegin(),
// check1.rend()), L"Неправильный порядок строковых элементов");
//
//			for (const auto& str : check1)
//				T1.erase(str);
//
//			ASSERT_TRUE(T1.size() == 0, L"Сет не пуст
// после
// удаления всех элементов"); 			for (const auto& elem :
// T1)
//				Assert::Fail(L"Что-то лежит в сете после
// удаления всех элементов");
//
//			T1.insert("test-1");
//			T1.insert("test-4");
//			T1.insert("test-2");
//			std::vector<std::string> check2{ "test-1",
//"test-2", "test-4" };
// ASSERT_TRUE(std::equal(T1.begin(), T1.end(),
// check2.begin(), check2.end()), L"Неправильный порядок строковых
// элементов");
// ASSERT_TRUE(std::equal(T1.rbegin(), T1.rend(), check2.rbegin(),
// check2.rend()), L"Неправильный порядок строковых элементов");
//		}
//
//		TEST_METHOD(ElemTests)
//		{
//			size_t init_count = Elem::count();
//			{
//				ContainerTemplate<Elem> T1;
//				ASSERT_TRUE(Elem::count() -
// init_count == 0,
// L"Создан лишний экземпляр класса Elem");
//
//				for (const auto& elem : T1)
//					Assert::Fail(L"Что-то лежит в
// пустом
// сете");
//
//				T1.insert(Elem(40));
//				T1.insert(Elem(75));
//				T1.insert(Elem(50));
//				T1.insert(Elem(23));
//				T1.insert(Elem(87));
//				ASSERT_TRUE(Elem::count() -
// init_count == 5,
// L"Создан лишний экземпляр класса Elem");
// T1.erase(Elem(75)); ASSERT_TRUE(Elem::count() - init_count == 4,
// L"Неправильно работает удаление элементов дерева");
// T1.erase(Elem(100));
// ASSERT_TRUE(Elem::count() - init_count == 4, L"Неправильно
// работает удаление несуществующих элементов");
//			}
//			ASSERT_TRUE(Elem::count() - init_count == 0,
// L"Утечка
// памяти!!");
//		}
//	};
} // namespace TreeTest