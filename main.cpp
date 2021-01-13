//Калугин Кирилл Алексеевич

/*Создать шаблон динамической коллекцию, согласно варианту задания:
1. Коллекция должна быть реализована с помощью умных указателей (std::shared_ptr, std::weak_ptr). Опционально использование std::unique_ptr;
2. В качестве параметра шаблона коллекция должна принимать тип данных - фигуры;
3. Реализовать forward_iterator по коллекции;
4. Коллекция должны возвращать итераторы begin() и  end();
5. Коллекция должна содержать метод вставки на позицию итератора insert(iterator);
6. Коллекция должна содержать метод удаления из позиции итератора erase(iterator);
7. При выполнении недопустимых операций (например выход аз границы коллекции или удаление несуществующего элемента) необходимо генерировать исключения;
8. Итератор должен быть совместим со стандартными алгоритмами (например, std::count_if)
9.   	Коллекция должна содержать метод доступа:
стек – pop, push, top;
очередь – pop, push, top;
список, Динамический массив – доступ к элементу по оператору [].
10.	Реализовать программу, которая:
позволяет вводить с клавиатуры фигуры (с типом int в качестве параметра шаблона фигуры) и добавлять в коллекцию;
позволяет удалять элемент из коллекции по номеру элемента;
выводит на экран введенные фигуры c помощью std::for_each;
выводит на экран количество объектов, у которых площадь меньше   заданной (с помощью  std::count_if).*/
/*Вариант: Пятиугольник, Стэк*/


#include <iostream>
#include <algorithm>
#include <math.h>
#include <memory>
#include <windows.h>

using namespace std;

const double PI = 3.141592653589793238462643383279502884197169399375105820974944;
double Square;//глобальная переменная для хранения площади

template <typename T>
class Five {//класс пятиугольника
	public:
		using type = T;
		pair<T, T> O;
		T R;
	Five () {};//конструктор класса
	Five (T x, T y, T r): O(x, y), R(r) {};//конструктор класса
};

double square (Five <double> x) {//функция подсчета площади пятиугольника
	return (2.5 * pow (x.R, 2) * sin (72.0 / 180.0 * PI));
}

template <typename T>//шаблон функции поиска вершин пятиугольника
void points (Five <T> x) {
    cout << "----------\n"<< "A = (" << (x.O).first << "," << (x.O).second + x.R << ")\n" << "B = (" 
         << (x.O).first + x.R * cos (28.0 / 180.0 * PI) << "," << (x.O).second + x.R * sin (28.0  / 180.0 * PI) 
         << ")\n" << "C = (" << (x.O).first + x.R * cos (- 44.0  / 180.0 * PI) << "," 
         << (x.O).second + x.R * sin (- 44.0  / 180.0 * PI) << ")\n" << "D = (" 
         << (x.O).first + x.R * cos (- 116.0  / 180.0 * PI) << "," << (x.O).second + x.R * sin (- 116.0  / 180.0 * PI) 
         << ")\n" << "E = (" << (x.O).first + x.R * cos (- 188.0  / 180.0 * PI) 
         << "," << (x.O).second + x.R * sin (- 188.0  / 180.0 * PI) << ")\n" << "----------\n";
}

template <typename T>
class Elem {//класс элемента стека
    public:
        Five <T> vault;
        shared_ptr <Elem <T>> prev;
        Elem () {};//конструктор класса
        Elem (T x, T y, T r, shared_ptr <Elem <T>> p) {//конструктор класса
                vault.O.first = x;
                vault.O.second = y;
                vault.R = r;
                prev = p;
            };
};

template <typename T>
class Iter {//класс итератора
    public:
        weak_ptr <Elem <T>> iter;
        Iter () {};//конструктор класса
        Iter (weak_ptr <Elem <T>> c) {//конструктор класса
            iter = c;
        }

        Iter <T>* operator++ () {//перегруженный оператор ++
            shared_ptr <Elem <T>> buffer = iter.lock();
            if (buffer->prev) {
                iter = buffer->prev;
            } else {
                throw "Iterator out of range";
            }
            return (this);            
        }

        Five <T> operator* () {//перегруженный оператор разименования
            shared_ptr <Elem <T>> buffer = iter.lock();
            return (buffer->prev->vault);
        }

        bool operator!= (Iter <T> sec) {//перегруженный оператор неравенства
            shared_ptr <Elem <T>> b1 = iter.lock();
            shared_ptr <Elem <T>> b2 = sec.iter.lock();
            bool f;
            if (b1.get() != b2.get()) {
                f = 1;
            }
            else {
                f = 0;
            }
            return (f);
        }
};

template <typename T>
class Stack {//класс стэка
    public:
        shared_ptr <Elem <T>> first;
        weak_ptr <Elem <T>> last;
        Iter <T> point;
        shared_ptr <Elem <T>> null;

        void push () {//функция добавления элемента в начало стэка
            T x;
            T y;
            T r;
            cout << "Insert center coordinates: ";
            cin >> x;
            cin >> y;
            cout << "Insert radius: ";
            cin >> r;
            
            if (!first) {
                shared_ptr <Elem <T>> curr (new Elem <T> (x, y, r, first));
                shared_ptr <Elem <T>> n (new Elem <T> ());
                null = n;
                null->prev = curr;
            } else {
                shared_ptr <Elem <T>> curr (new Elem <T> (x, y, r, first->prev));
                null->prev = curr;
            }
            point.iter = null->prev;
            first = null;
            if (!last.lock()) {
                last = first->prev;
            }
                        
        }

        void pop () {//функция удаления элемента из начала стэка
            if (!first) {
                throw "Stack is empty!\n";
            } else {
                if (!first->prev) {
                    throw "Stack is empty!\n";
                } else {
                    first->prev = first->prev->prev;

                }
            }
            
        }

        void top () {//функция просмотра элемента из начала стэка
            if (!first) {
                cout << "Stack is empty!\n";
            } else {
                if (!first->prev) {
                    cout << "Stack is empty!\n";
                } else {
                    points (first->prev->vault);
                }
            }
            
            
        }

        Iter <T> begin () {//функция возврата в начало стэка
            return (Iter <T> (first));
        }

        Iter <T> end () {//функция перемещения в конец стэка
            Iter <T> j; 
            j.iter = last;
            return (j);
        }

        void insert () {//функция вставки элемента по расположению итератора
            T x;
            T y;
            T r;
            cout << "Insert center coordinates: ";
            cin >> x;
            cin >> y;
            cout << "Insert radius: ";
            cin >> r;
            
            if (first) {
                shared_ptr <Elem <T>> curr (new Elem <T> (x, y, r, point.iter.lock()->prev));
                point.iter.lock()->prev = curr;
                point.iter = curr;
                if (!curr->prev) {
                    last = curr;
                }
            } else {
                shared_ptr <Elem <T>> curr (new Elem <T> (x, y, r, first));
                shared_ptr <Elem <T>> n (new Elem <T> ());
                null = n;
                null->prev = curr;
                first = null;
                if (!last.lock()) {
                    last = first->prev;
                } 
                point.iter = curr;
            }
            
        }
    
        void erase () {//функция удаления элемента по расположению итератора
            if (first && first->prev) {
                Iter <T> e;
                e.iter = first;
                while ((e.iter.lock()->prev).get() != point.iter.lock().get()) {
                    ++ e;
                }
                e.iter.lock()->prev = point.iter.lock()->prev;
                point.iter = e.iter.lock()->prev;
            } else {
                throw "Stack is empty!\n";
            }
            
        }

        Iter <T> r () {//функция нахождения элемента, стоящего перед итератором
            Iter <T> p;
            p.iter = first;
            while ((p.iter.lock()->prev).get() != (point.iter.lock()).get()) {
                ++ p;
            }
            return (p);
        }

        Iter <T> r2 () {//функция нахождения итератора
            Iter <T> p2 = point;
            return (p2);
        }
};

template <typename T>
bool s (Five <T> f) {//функция сравнения площади с максимальной
    bool ret;
    if (square (f) < Square) {
        ret = 1;
    } else {
        ret = 0;
    }
    return (ret);
}

template <class InputIt, class UnaryPredicate>
int Count_if (InputIt first, InputIt last, UnaryPredicate p)//стандартная функция count_if
{
    int ret = 0;
    for (; first != last; ++ first) {
        if (p (*first)) {
            ret ++;
        }
    }
    return ret;
}

int main() {
    Stack <double> S;

    int m = 99;
    cout << "1.Execute command \"push\"\n";
    cout << "2.Execute command \"pop\"\n";
    cout << "3.Execute command \"top\"\n";
    cout << "4.Show stack\n";
    cout << "5.Move iterator to the next element\n";
    cout << "6.Move iterator to the first element\n";
    cout << "7.Move iterator to the last element\n";
    cout << "8.Add element in iterator position\n";
    cout << "9.Erase element in iterator position\n";
    cout << "10.Print the number of objects with an area less than the specified one\n";
    cout << "0.Exit\n";
    while (m != 0) {
        cout << "Choose option: ";
        cin >> m;
        switch (m) {//меню
            case 1: {
                try {
                    S.push ();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;              
            }

            case 2: {
                try {
                    S.pop ();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 3: {
                try {
                    S.top ();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 4: {
                try {
                    if ((S.first) && (S.first->prev)) {
                        for_each (S.begin(), S.r(), points <double>);
                        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                        cout << "vvvvvvvvvv\n";
                        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                        points <double> (*(S.r()));
                        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                        cout << "^^^^^^^^^^\n";
                        SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                        for_each (S.r2(), S.end(), points <double>);
                    } else {
                        cout << "Stack is empty!\n";
                    } 
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }                
                break;
            }

            case 5: {
                try {
                    ++ S.point; 
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 6: {
                try {
                    S.point = S.begin();
                    ++ S.point; 
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 7: {
                try {
                    S.point = S.end();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }
            
            case 8: {
                try {
                    S.insert ();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 9: {
                try {
                    S.erase ();
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 10: {
                try {
                    cout << "Insert max square: ";
                    cin >> Square;
                    int k;
                    k = Count_if (S.begin(), S.end(), s <double>);
                    cout << "There are ";
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 2));
                    cout << k;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                    cout << " pentagons with square less than " << Square << "\n";
                }
                catch (const char* exc) {
                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 4));
                    cerr << exc << endl;
                    SetConsoleTextAttribute(hConsole, (WORD) ((0 << 4) | 7));
                }
                break;
            }

            case 0: {
                break;
            }
            default: {
                cout << "Wrong command!\n";
                break;
            }
        }
    }

    return 0;
}