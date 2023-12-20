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

#include <regex>

const std::regex regexp{"[a-zA-Z0-9áéíóúñĺ]"};
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

std::string to_lower(std::string s)
{
    std::string result = "";

    for (size_t i = 0; i < s.length(); ++i)
    {
        result += std::tolower(s[i]);
    }

    return result;
}

std::string only_letters(std::string s)
{
    std::string result = "";

    for (size_t i = 0; i < s.length(); ++i)
    {
        if (std::regex_match(std::string{s[i]}, regexp) || s[i] == ' ')
        {
            result += s[i];
        }
    }

    return result;
}

std::vector<std::string> split(std::string string, char break_char = ' ')
{
    std::vector<std::string> result{};
    std::string temp = "";
    for (size_t i = 0; i < string.length(); ++i)
    {
        if (string[i] == break_char)
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
            else if (s == "Respuesta:")
            {
                if (i != temp.size() - 1)
                {
                    std::string temp_ans = "";
                    for (size_t j = i; j < temp.size(); ++j)
                    {
                        temp_ans += temp[j];
                        temp_ans += " ";
                    }
                    temp_ans.pop_back();
                    temp_ans.pop_back();
                    q.answer.push_back(only_letters(to_lower(temp_ans)));
                    break;
                }
                reading_answers = true;
            }
        }
        if (reading_answers)
        {
            if (temp[1] == "Respuesta:")
            {
                continue;
            }

            std::string temp_ans = "";
            for (size_t i = 0; i < temp.size(); ++i)
            {
                if (temp[i][0] == '\t')
                {
                    continue;
                }
                temp_ans += temp[i];
                temp_ans += " ";
            }
            temp_ans.pop_back();
            temp_ans.pop_back();
            q.answer.push_back(only_letters(to_lower(temp_ans)));
        }
    }

    clear();

    questions.push_back(q);

    data_file.close();

    std::cout << "\nNumero de Preguntas: " << questions.size() << std::endl << std::endl;

    size_t index = 1;

    for (Question q: questions)
    {
        std::cout << index << ". ";
        std::cout /*<< "Pregunta: "*/ << q.title << std::endl;

        if (q.hint.length() > 0)
        {
            std::cout << index << ". ";
            std::cout /*<< "Pista: "*/ << q.hint  << std::endl;
        }

        if (q.answer.size() == 0)
        {
            ++index;
            continue;
        }

        if (q.answer.size() > 1)
        {
            std::cout << index << ". Respuesta: " << std::endl;
            char letter = 'a';
            for (std::string s : q.answer)
            {
                std::cout << "\t" << letter++ << ") " << s << std::endl;
            }
        }
        else
        {
            std::cout << index << ". " << q.answer[0]  << std::endl  << std::endl;
        }

        std::cout << std::endl;

        ++index;
    }

    return EXIT_SUCCESS;
}