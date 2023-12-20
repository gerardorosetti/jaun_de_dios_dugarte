#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

//test
#include <wchar.h>
#include <locale.h>
//test

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
    std::vector<std::string> answer;
};

std::vector<std::string> split(std::string string, char break_char = ' ')
{
    std::vector<std::string> result{};
    std::string temp = "";
    for (size_t i = 0; i < string.length(); ++i)
    {
        if (string[i] == ' ')
        {
            result.push_back(temp);
            temp = "";
            continue;
        }
        temp += string[i];
    }
    result.push_back(temp);
    return result;
}

int main ()
{
    setlocale(LC_ALL, "");

    std::vector<Question> questions;

    std::ifstream data_file{"data.txt"};

    bool menu_flag = true;

    bool reading_answers = false;

    Question q{"", "", std::vector<std::string>{}};

    while(!data_file.eof())
    {
        std::string line;
        std::getline(data_file, line);
        if (line.length() == 0)
        {
            if (q.title != "")
            {
                questions.push_back(q);
            }
            
            q = Question{"", "", std::vector<std::string>{}};
            reading_answers = false;
            continue;
        }
        if(menu_flag && line == "MENU FIN")
        {
            menu_flag = false;
            continue;
        }
        if(menu_flag)
        {
            std::cout << line << std::endl;
            continue;
        }

        std::vector<std::string> temp{split(line)};

        for (size_t i = 0; i < temp.size(); ++i)
        {
            std::string s = temp[i];
            if (s == "Pregunta:")
            {
                for (size_t j = i; j < temp.size(); ++j)
                {
                    q.title += temp[j];
                    q.title += " ";
                }
                q.title.pop_back();
                q.title.pop_back();
                break;
            }
            else if (s == "Pista:")
            {
                for (size_t j = i; j < temp.size(); ++j)
                {
                    q.hint += temp[j];
                    q.hint += " ";
                }
                q.hint.pop_back();
                break;
            }
            else if (s == "Respuesta:") reading_answers = true;
        }
        if (reading_answers)
        {
            continue;
        }
    }

    questions.push_back(q);

    data_file.close();

    std::cout << questions.size() <<std::endl;

    size_t index = 1;
    for (Question q: questions)
    {
        std::cout << index << ". ";
        std::cout /*<< "Pregunta: "*/ << q.title << std::endl;
        std::cout << index << ". ";
        std::cout /*<< "Pista: "*/ << q.hint  << std::endl  << std::endl;
        ++index;
    }
    

    // std::string temp[3];
    // size_t index = 0;
    // for (std::string line; std::getline(data_file, line); ++index)
    // {
    //     if (line.length() == 0)
    //     {
    //         index = -1;
    //         continue;
    //     }

    //     temp[index] = line;
    //     if (index == 2)
    //     {
    //         Question q{temp[0], temp[1], temp[2]};
    //         questions.push_back(q);
    //     }
    // }


    return EXIT_SUCCESS;
}