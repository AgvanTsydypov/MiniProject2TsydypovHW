#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <string>
#include <omp.h>
#include <time.h>
size_t threads;

std::mutex mtx;

int GetNumber(int left, int right, std::string str)
{
	int temp;
	while (true)
	{
		std::cout << "введите число " << str << " [" << left << ";" << right << "]" << std::endl;
		std::cin >> temp;
		if (temp < left || temp > right) 
			continue;
		else
			break;
	}
	return temp;
}

void Bear(int& a)
{
	do {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "Медведь делает глоток " << "\n";
		std::lock_guard<std::mutex> guard(mtx);
		a -= 1;
	} while (true);
}

int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "ru");
	int h = 0;
	std::cout << "Количество пчел = n\n";
	threads = GetNumber(1,10, "n =");
	std::cout << "n = " << threads << std::endl;
	int H;

	std::cout << "Количество глотков в чаше = H\n";
	H =  GetNumber(1,100, "H =");
	std::cout << "H = " << H << std::endl;
	int r = rand();
	omp_set_num_threads(threads);
#pragma omp parallel for num_threads(threads)
	for (int i = 0; i < H; i++)
	{
		int time = rand() * r % 10000;
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
		double t = time;
		std::cout << "Пчела_" << std::to_string(omp_get_thread_num()) << " нашла глоток меда за " << t / 1000 << " секунд\n";
		std::lock_guard<std::mutex> guard(mtx);
		h++;;
		std::cout << "глотков в чаше =  " << h << std::endl;;
	}

		std::cout << "Медведь проснулся\n";
		std::cout << "Пчелы спрятались\n";
		std::cout << "глотков в чаше =  " << h << std::endl;
	std::thread bear(Bear, std::ref(h));
	while (true)
	{
		if (h == 0)
			break;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		std::cout << "глотков в чаше =  " << h << std::endl;
	}
	bear.detach();
	std::cout << "медведь засыпает";

	return 0;
}