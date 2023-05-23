#include <iostream>
#include <time.h>
#include <windows.h>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

class Fork {
public:
    int name;
    mutable std::mutex mtx;

    Fork(int v = 0) : name(v) {}

    Fork(Fork&& other) {
        std::lock_guard<std::mutex> lock(other.mtx);
        name = std::move(other.name);
        other.name = 0;
    }
};

class Philosopher {
public:
    int name;
    int count = 0;

    Philosopher(int v = 0) : name(v) {}

    void eat(Fork& left_fork, Fork& right_fork) {
        // пробует взять 1 вылку
        if (left_fork.mtx.try_lock()) {
            // если взял 1, то пробует взять 2 вылку несколько раз(1-3)
            for (int i = 0; i <= rand() % 3 + 1; i++) {
                if (right_fork.mtx.try_lock()) {
                    std::cout << "P " << name << " eat" << endl;
                    Sleep(rand() % (21 - 10) + 10);
                    count++;
                    right_fork.mtx.unlock();
                    left_fork.mtx.unlock();
                    return;
                }
                Sleep(1);
            }
            left_fork.mtx.unlock();
        }
    }

    void think(int time) {
        std::cout << "P " << name << " thinks" << endl;
        Sleep(time);
    }
};

void simulation(Philosopher& philosopher, Fork& left_fork, Fork& right_fork) {
    for (int i = 0; i <= 150; i++)
    {
        philosopher.think(rand() % (21 - 10) + 10);
        philosopher.eat(left_fork, right_fork);
    }
}

int main() {
    int sum = 0;
    int count_philosopher;
    vector <thread> threads;
    vector<Fork> forks;
    vector<Philosopher> philosophers;

    std::cout << "Enter the number of philosophers: ";
    std::cin >> count_philosopher;

    for (int i = 0; i < count_philosopher; i++) {
        philosophers.push_back(Philosopher(i));
        forks.push_back(Fork(i));
    }

    for (int i = 0; i < count_philosopher; i++)
        threads.push_back(thread(simulation, ref(philosophers[i]), ref(forks[i]), ref(forks[(i+1)% count_philosopher])));

    for (thread& t : threads)
        t.join();

    std::cout << endl << "Resulte:" << endl;
    for (int i = 0; i < count_philosopher; i++)
        std::cout << "Philosopher " << philosophers[i].name << " eat count " << philosophers[i].count << endl;
    std::cout << sum << endl;
}


//#include <iostream>
//#include <vector>
//#include <future>
//using namespace std;
//int compute(int const x) {
//    return x * x;
//}
//int main() {
//    int sum = 0;
//    int THREAD_NUM = 4;
//    vector<future<int>> futures;
//    for (int i = 0; i <= THREAD_NUM; i++) {
//        std::cout << "hello world!\n" << i << endl;
//        futures.push_back(async(&compute, i));
//    }
//    for (future<int>& f : futures) {
//        sum += f.get();
//        std::cout << sum << endl;
//    }
//    std::cout << sum << endl;
//}

//int main()
//{
//    std::cout << "hello world!\n";
//}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
