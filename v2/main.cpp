#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <thread>
#include <chrono>

//Especial Chars
#include <wchar.h>
#include <locale.h>

//Regular Expression
#include <regex>

const std::regex regexp{"[a-zA-Z0-9áéíóúñÑĺ]"};
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

bool check_only_numbers(std::string s)
{
    bool result = true;

    for (size_t i = 0; i < s.length(); ++i)
    {
        if (s[i] < 48 || s[i] > 57)
        {
            result = false;
            break;
        }
    }
    
    return result;
}

std::string only_letters(std::string s)
{
    std::string result = "";

    for (size_t i = 0; i < s.length(); ++i)
    {
        if ((std::regex_match(std::string{s[i]}, regexp) || s[i] == ' ') && s[i] != '-')
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

void print_questions_vector_data(std::vector<Question>& questions)
{
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
            std::cout << index << ". Respuesta: " << q.answer[0]  << std::endl  << std::endl;
        }

        std::cout << std::endl;

        ++index;
    }
}

int main ()
{
    // clear();

    //Allow especial chars
    setlocale(LC_ALL, "");

    std::vector<Question> questions;

    std::ifstream data_file{"data.txt"};

    bool menu_flag = true;

    bool reading_answers = false;

    Question q{"", "", std::vector<std::string>{}};

    std::vector<std::string> initial_menu_lines{""};

    //Getting the questions
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
            // std::cout << line << std::endl;
            initial_menu_lines.push_back(line);
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

                if (q.hint == "Pista:")
                {
                    q.hint = "";
                }
                
                break;
            }
            else if (s == "Respuesta:")
            {
                if (i != temp.size() - 1)
                {
                    std::string temp_ans = "";
                    for (size_t j = i + 1; j < temp.size(); ++j)
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

    questions.push_back(q);

    data_file.close();

    auto print_initial_menu = [&initial_menu_lines] ()
    {
        clear();

        std::cout << "Bienvenido al Asistente de Estudio de\n\t    Juan Dugarte" << std::endl;
        std::cout << std::endl;
        for (std::string line : initial_menu_lines)
        {
            std::cout << line << std::endl;
        }
        std::cout << std::endl;
    };


    // print_questions_vector_data(questions);
    auto print_no_valid_option_message = [] ()
    {
        clear();
        std::cout << "\n\n\tOpcion no valida, intente nuevamente\n\n";
        std::this_thread::sleep_for(2s);
    };

    do
    {
        print_initial_menu();
        std::cout << "Seleccione de que manera le gustaria trabajar:" << std::endl;
        std::cout << "1) Modo Ordenado\n2) Modo Aleatorio\n3) Salir\n\nIngrese su opcion [1, 2 o 3]: ";
        std::string option;
        std::cin >> option;
        if (option == "1")
        {
            clear();
            // print_initial_menu();
            std::getchar();
            std::string start{""}, end{""};

            do
            {
                print_initial_menu();
                std::cout << "Seleccione con cual pregunta desea empezar: ";
                std::cin >> start;
                if (check_only_numbers(start))
                {
                    break;
                }
                print_no_valid_option_message();
            } while (true);

            clear();

            do
            {
                print_initial_menu();
                std::cout << "Seleccione con cual pregunta desea terminar: ";
                std::cin >> end;
                if (check_only_numbers(end))
                {
                    break;
                }
                print_no_valid_option_message();
            } while (true);

            size_t start_index = std::stoi(start) - 1;
            size_t end_index = std::stoi(end);

            std::getchar();

            for (size_t index = start_index; index < end_index; ++index)
            {
                clear();
                Question q = questions[index];
                std::cout << index + 1 << ") " << q.title << std::endl << q.hint << std::endl << "\nTu Respuesta: ";
                std::string ans;
                std::getline(std::cin, ans);
                // std::vector<std::string> ans_vec{split(only_letters(to_lower(ans)))};
                
                size_t words_match_counter = 0;

                auto check_sentences_match = [&words_match_counter] (std::string s1, std::string s2)
                {
                    std::vector<std::string> v1{split(only_letters(to_lower(s1)))};
                    std::vector<std::string> v2{split(only_letters(to_lower(s2)))};
                    bool result = true;
                    for (std::string s1_temp : v1)
                    {
                        if (std::any_of(v2.begin(), v2.end(), [&s1_temp] (std::string s2_temp) { return s1_temp == s2_temp; }))
                        {
                            // result = false;
                            // break;
                            ++words_match_counter;
                        }
                    }
                    return words_match_counter == v2.size();
                };

                std::string big_ans = "";

                for (std::string ans_item : q.answer)
                {
                    big_ans += ans_item + ' ';
                }

                big_ans.pop_back();

                if (!check_sentences_match(ans, big_ans))
                {
                    std::cout << "\n\nIncorrecto... Numero de palabras acertadas: " << words_match_counter << " / " << split(big_ans).size() << std::endl;
                    std::cout << "\n\nRespuesta esperada: " << big_ans << std::endl;
                }
                else std::cout << "\n\nExcelente!!! Numero de palabras acertadas: " << words_match_counter << " / " << split(big_ans).size() << std::endl;


                std::string xd;
                // std::this_thread::sleep_for(3s);
                std::cin>> xd;
                break;
                

                // if (std::all_of(q.answer.begin(), q.answer.end(), [&ans_vec] (std::string value) { return std::any_of(ans) }))
                // {
                //     /* code */
                // }
                

            }
        }
        else if(option == "2")
        {

            std::random_device rd{};
            std::mt19937_64 rng(rd());

            std::getchar();

            std::vector<Question> questions_temp{questions};
            bool again = false;
            Question q;
            for (size_t index = 0; questions_temp.size() > 0; ++index)
            {
                clear();
                std::uniform_int_distribution<size_t> dis{0, questions_temp.size() - 1};
                size_t num = dis(rng);
                if (!again)
                {
                    q = questions_temp[num];
                    std::vector<Question> q_temp_vec{};
                    Question q_aux;
                    while (true)
                    {
                        Question q_aux = questions_temp[questions_temp.size() - 1];
                        questions_temp.pop_back();
                        if (q_aux.title == q.title && q_aux.answer == q.answer && q.hint == q_aux.hint)
                        {
                            for (Question temp : q_temp_vec)
                            {
                                questions_temp.push_back(temp);
                            }
                            break;
                        }
                        q_temp_vec.push_back(q_aux);
                    }
                }
                std::cout << index + 1 << ") " << q.title << std::endl << q.hint << std::endl << "\nTu Respuesta: ";
                std::string ans;
                std::getline(std::cin, ans);
               
            }
        }
        else if(option == "3")
        {
            break;
        }
        else
        {
            // clear();
            // std::cout << "\n\n\tOpcion no valida, intente nuevamente\n\n";
            // std::this_thread::sleep_for(2s);
            print_no_valid_option_message();
        }
    } while (true);
    

    return EXIT_SUCCESS;
}