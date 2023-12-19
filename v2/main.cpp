#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

using namespace std::literals::chrono_literals;

#ifdef _WIN32
    #include <windows.h>
    void clear() {
        system("cls");
    }
    #define LINUX false
#else
    #include <unistd.h>
    void clear() {
        system("clear");
    }
    #define LINUX true
#endif

struct Question
{
    std::string title;
    std::string hint;
    std::string answer;
};

int main ()
{
    std::vector<Question> questions;

    std::ifstream data_file{"data.txt"};

    std::string temp[3];
    size_t index = 0;
    for (std::string line; std::getline(data_file, line); ++index)
    {
        if (line.length() == 0)
        {
            index = -1;
            continue;
        }

        temp[index] = line;
        if (index == 2)
        {
            Question q{temp[0], temp[1], temp[2]};
            questions.push_back(q);
        }
    }

    return EXIT_SUCCESS;
}