#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <shellapi.h>
#include <string.h>
#include <map>
#include <unordered_map>

using namespace std;
#define KEY_UP 38
#define KEY_DOWN 0x28
#define VK_RETURN 0x0D
int id = 1;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

class Passwords
{
public:
    std::map<std::string, std::pair<std::string, std::string>> users;

    Passwords(std::map<std::string, std::pair<std::string, std::string>> users)
      : users(std::move(users))
    {
    }

    Passwords(int i)
    {
        std::cout << i << std::endl;
    }

    Passwords()
    {
        users = {
            {"user1", {"password1", "admin"}},
            {"user2", {"password2", "user"} }
  
        };
    }
    
    
    std::string authorization()
    {
        HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
        DWORD NumInputs = 0;
        DWORD InputsRead = 0;
        INPUT_RECORD irInput;
        system("cls");
        std::string inputUsername, inputPassword;

        while (true)
        {
            // Запрашиваем у пользователя логин и пароль
            std::cout << "Введите логин: ";
            std::cin >> inputUsername;
            std::cout << "Введите пароль: ";
            std::cin >> inputPassword;

            auto it = users.find(inputUsername);
            if (it != users.end() && it->second.first == inputPassword)
            {
                std::cout << "Авторизация прошла успешно" << std::endl;
                return it->second.second; // Возвращаем роль пользователя
            }
            else
            {
                std::cout
                    << "Логин или пароль неверные. Стрелка вниз попробуйте еще раз, стрелка вверх выход."
                    << std::endl;
                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                switch (irInput.EventType)
                {
                case KEY_EVENT:
                {
                    if (irInput.Event.KeyEvent.bKeyDown)
                    {
                        if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                        {
                            system("cls");
                            continue;
                        }
                        else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                        {
                            return "0";
                            system("cls");
                        }
                    }
                }
                }
                system("pause");
                system("cls");
            }
        }
    }

    void addUser()
    {
        std::string roles[5]{"mainEditor", "journalist", "photograph", "litEditor", "designer"};
        int roleNumber;
        std::string username, password, role;
        std::cout << "Введите логин нового пользователя: ";
        std::cin >> username;
        std::cout << "Введите пароль нового пользователя: ";
        std::cin >> password;
        std::cout
            << "Введите роль нового пользователя(1 главный редактор, 2 журналист, 3 фотограф, 4 лит. редактор, 5 дизайнер): ";
        std::cin >> roleNumber;
        role = roles[roleNumber - 1];
        users.insert({
            username, {password, role}
        });
        std::cout << "Пользователь записан" << std::endl;
    }

    void save(const std::string& filename)
    {
        std::ofstream out(filename, std::ios::binary);
        if (!out.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для записи");
        }
        

        // Записать размер карты
        uint32_t size = users.size();
        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        //std::cout << size << std::endl;
        // Записать каждую пару ключ-значение
        for (const auto& user : users)
        {
            // Записать размер ключа
            uint32_t key_size = user.first.size();
            out.write(reinterpret_cast<const char*>(&key_size), sizeof(key_size));
            //std::cout << user.first.size() << std::endl;
            // Записать ключ
            out.write(user.first.data(), key_size);
            //std::cout << key_size << std::endl;
            // Записать размер значения
            uint32_t value_size = user.second.first.size();
            out.write(reinterpret_cast<const char*>(&value_size), sizeof(value_size));

            // Записать значение (имя)
            out.write(user.second.first.data(), value_size);

            // Записать размер значения
            value_size = user.second.second.size();
            out.write(reinterpret_cast<const char*>(&value_size), sizeof(value_size));

            // Записать значение (пароль)
            out.write(user.second.second.data(), value_size);
        }

        out.close();
    }

    std::map<std::string, std::pair<std::string, std::string>> load(const std::string& filename)
    {
        //std::map<std::string, std::pair<std::string, std::string>> users;

        std::ifstream in(filename, std::ios::binary);
        if (!in.is_open())
        {
            throw std::runtime_error("Не удалось открыть файл для чтения");
        }

        // Прочитать размер карты
        uint32_t size;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));

        // Прочитать каждую пару ключ-значение
        for (uint32_t i = 0; i < size; ++i)
        {
            // Прочитать размер ключа
            uint32_t key_size;
            in.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));

            // Прочитать ключ
            std::string key(key_size, '0');
            in.read(&key[0], key_size);

            // Прочитать размер значения (имя)
            uint32_t value_size;
            in.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));

            // Прочитать значение (имя)
            std::string value_name(value_size, '0');
            in.read(&value_name[0], value_size);

            // Прочитать размер значения (пароль)
            in.read(reinterpret_cast<char*>(&value_size), sizeof(value_size));

            // Прочитать значение (пароль)
            std::string value_password(value_size, '0');
            in.read(&value_password[0], value_size);

            // Сохранить пару ключ-значение в карте
            users[key] = {value_name, value_password};
        }  
        in.close();

        
        return users;
    }
};

class Employee
{
public:
    Employee(const std::string& name, int age, const std::string& department)
      : name(name)
      , age(age)
      , department(department)
    {
    }

    void work()
    {
        std::cout << "Выполняет обязанности сотрудника отдела " << department << std::endl;
    }

protected:
    std::string name;
    int age;
    std::string department;
};

class Article
{
public:
    int id;                  
    std::string title;       
    std::string description; 

    Article()
    {
        
    }
    Article(int id, const std::string& title, const std::string& description)
      : id(id), title(title), description(description)
    {
    } 

    static std::vector<Article> articles;

   void saveArticlesToBinaryFile(const std::string& filename) const
    {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (file.is_open())
        {
            for (const auto& article : articles)
            {
                file.write(reinterpret_cast<const char*>(&article), sizeof(article));
            }
            file.close();
        }
        else
        {
            std::cout << "not good" << std::endl;
        }
    }

  void loadArticlesFromBinaryFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            Article article;
            while (file.read(reinterpret_cast<char*>(&article), sizeof(article)))
            {
                articles.push_back(article);
            }
            file.close();
        }
        else
        {
            std::cout << "Failed to open file for reading." << std::endl;
        }
    }

private:
    struct ArticleData
    {
        char title[256];
        char author[256];
        char content[1024];
    };

};

std::vector<Article> Article::articles; 

class ChiefEditor
{
public:
    static void createArticleFromInput() 
    {
        int id = Article::articles.size() + 1;
        std::string title, description; 
        std::cout << "Введите заголовок статьи: ";
        std::getline(std::cin >> std::ws, title); 
        std::cout << "Введите описание статьи: ";
        std::getline(std::cin >> std::ws, description); 
        Article::articles.emplace_back(
            id, title, description
        ); 
        std::cout << "Статья успешно создана!"
                  << std::endl;
    }

    static void viewArticles() 
    {
        if (Article::articles.empty()) 
        {
            std::cout << "Нет доступных статей для просмотра."
                      << std::endl; 
        }
        else
        {
            std::cout << "Список статей:" << std::endl; 
            for (const auto& article : Article::articles)
            {
                std::cout << "ID: " << article.id << ", Заголовок: " << article.title
                          << ", Описание: " << article.description
                          << std::endl; 
            }
        }
    }
};


class Journalist : public Employee
{
public:
    Journalist(const std::string& name, int age)
      : Employee(name, age, "Журналистика")
    {
    }

    /*void writeArticle() {
        std::cout << name << " пишет статью" << std::endl;
    }*/
};

class LiteraryEditor : public Employee
{
public:
    LiteraryEditor(const std::string& name, int age)
      : Employee(name, age, "Литература")
    {
    }

    /*void editText() {
        std::cout << name << " редактирует текст" << std::endl;
    }*/
};

class Photographer : public Employee
{
public:
    Photographer(const std::string& name, int age)
      : Employee(name, age, "Фотография")
    {
    }

    /*void takePhoto() {
        std::cout << name << " делает фотографию" << std::endl;
    }*/
};

class Designer : public Employee
{
public:
    Designer(const std::string& name, int age)
      : Employee(name, age, "Дизайн")
    {
    }

    /*void designLayout() {
        std::cout << name << " разрабатывает дизайн макета" << std::endl;
    }*/
};

class Document
{
public:
    Document(const std::string& title, const std::string& author)
      : title(title)
      , author(author)
    {
    }

    /*void displayInfo() {
        std::cout << "Документ: " << title << ", Автор: " << author << std::endl;
    }*/

protected:
    std::string title;
    std::string author;
};

class MagazineLayout : public Document
{
public:
    MagazineLayout(const std::string& title, const std::string& author)
      : Document(title, author)
    {
    }

    /*void createLayout() {
        std::cout << "Создается макет для журнала: " << title << std::endl;
    }*/
};

class PhotoReportage : public Document
{
public:
    PhotoReportage(const std::string& title, const std::string& author)
      : Document(title, author)
    {
    }

    /*void capturePhotos() {
        std::cout << "Создается фото-репортаж: " << title << " авторства " << author << std::endl;
    }*/
};

struct Abonent
{
    int id;
    int cost;
    string name;
    string author;
    string date;
    string book_name;
    string publisher;
    int deleted = 0;
};

struct User
{
    std::string username;
    std::string password;
    std::string role;
};

struct Saver
{
    int id;
    int cost;
    char name[256];
    char author[256];
    char date[256];
    char book_name[256];
    char publisher[256];
    int deleted;
};

struct Debtor
{
    string name;
    int count = 0;
};

struct double_list
{
    Abonent abonent;
    double_list* next = nullptr;
    double_list* prev = nullptr;
};

Abonent Init(
    string firstname,
    string secondname,
    string surname,
    int cost,
    string author,
    string book_name,
    string publisher,
    string date
)
{
    Abonent new_abonent;
    new_abonent.id = id;
    id++;
    new_abonent.cost = cost;
    ostringstream oss1;

    new_abonent.date = date;
    new_abonent.author = author;
    oss1 << secondname << " " << firstname << " " << surname;
    new_abonent.name = (string) oss1.str().c_str();
    new_abonent.publisher = publisher;
    new_abonent.book_name = book_name;
    return new_abonent;
}

void addUser(std::map<std::string, std::pair<std::string, std::string>>& users);
std::string authorization(std::map<std::string, std::pair<std::string, std::string>>& users);
Abonent calculateAbonentBookCount(double_list& list);
double_list* Init_double_list(double_list* list, Abonent abonent1);
double_list* Sort_list_name(double_list* list, int a, double_list* beg);
double_list* Sort_list_cost(double_list* list, int a, double_list* beg);
double_list* Sort_list_id(double_list* list, int a, double_list* beg);
void Show_list(double_list* list, int n, int page);
void Show_abonent(Abonent people, int n);
void processDoubleList(double_list* list);
Abonent Get_param_from_keyboard();
void Save_txt(double_list* list);
void Save_bin(double_list* list);
double_list* Load_bin(double_list* list, double_list* beg);
double_list* find_abonent(double_list* list, int id);
void edit_abonent(double_list* pClient);
void delete_abonent(double_list* pClient);
int Age(string str1);
long avarage_age(double_list* list);
//авторизация
vector<string> menu1 = {"Авторизация", "Выход", ""};
// админ
vector<string> menu2 = {"Добавить нового пользователя", "Выход", ""};
// главный редактор
vector<string> menu3 = {"Создать статью", "Просмотр текущих статей в редакции", "Статьи на утверждение", "Выход", ""};
// журналист
vector<string> menu4 = {"Просмотр статей в работе", "Выход", ""};
// журналист/Просмотр статей в работе
vector<string> menu4_1 = {
    "Добавить статью",
    "редактировать статью",
    "удалить статью ", "Выход", ""};
    //лит редактор
vector<string> menu5 = {"Просмотр статей в редакции", "Выход", ""};
// лит редактор/Просмотр статей в редакции
vector<string> menu5_1 =
    {"Редактировать статью", "Удалить статью ", "Выход", ""};
//фотограф
vector<string> menu6 = {"Загрузить фото", "Редактировать фото", "", "Выход", ""};
void print_menu(int switcher, vector<string> menu);

int main()
{   
    Passwords Passwords1;
    ifstream file("passwords.bin");
    if ((file.is_open()))
    {
        Passwords1.load("passwords.bin");
    }
    setlocale(LC_ALL, "Russian");
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumInputs = 0;
    DWORD InputsRead = 0;
    bool running = true;
    int switcher = 2;
    std::string role;
    bool autorisation = true;
    string login;
    string password;
    INPUT_RECORD irInput;
    GetNumberOfConsoleInputEvents(hInput, &NumInputs);
    
    
    //double_list* list = new double_list;
    //list = NULL;
    //Abonent a;
    //Saver* s = new Saver;
    /*std::map<std::string, std::pair<std::string, std::string>> users = {
        {"user1", {"password1", "admin"}},
        {"user2", {"password2", "user"}}
  // Добавьте других пользователей здесь
    };*/

    /* OPENFILENAMEA ofn = {0};
    char Buffer[300];
    std::fill(Buffer, Buffer + 300, '\0');
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetForegroundWindow();
    ofn.lpstrFile = Buffer;
    ofn.nMaxFile = 300;
    ofn.Flags = OFN_EXPLORER;
    ofn.lpstrFilter = NULL;
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 0;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrTitle = NULL;
    if (GetOpenFileNameA(&ofn))
    {
        cout << ofn.lpstrFile << std::endl;
        system("pause");
    }*/
    bool miganie = true;
    while (running)
    {
        if (miganie)
        {
            system("cls");
            print_menu(switcher, menu1);
        }
        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
        
        switch (irInput.EventType)
        {

        case KEY_EVENT:
            if (irInput.Event.KeyEvent.bKeyDown)
            {
                if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                {
                    if (switcher > 2)
                    {
                        switcher--;
                    }
                    system("cls");
                    print_menu(switcher, menu1);
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                {
                    if (switcher < menu1.size())
                    {
                        switcher++;
                    }
                    system("cls");
                    print_menu(switcher, menu1);
                }
                else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                {
                    switch (switcher - 1)
                    {
                    case 1:
                        system("cls");
                        role = Passwords1.authorization();
                        if (role == "0")
                        {
                            system("cls");
                            break;
                            
                        }
                        std::cout << "ваша роль определена как: " << role << endl;
                        system("pause");
                        if (role == "admin")
                        {
                            bool ans = 1;
                            switcher = 2;
                            miganie = true;
                            while (ans)
                            {
                                
                                if (miganie)
                                {
                                    system("cls");
                                    print_menu(switcher, menu2);
                                }
                                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                                switch (irInput.EventType)
                                {
                                case KEY_EVENT:
                                    if (irInput.Event.KeyEvent.bKeyDown)
                                    {
                                        if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                                        {
                                            if (switcher > 2)
                                            {
                                                switcher--;
                                            }
                                            system("cls");
                                            print_menu(switcher, menu2);
                                        }
                                        else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                        {
                                            if (switcher < menu2.size())
                                            {
                                                switcher++;
                                            }
                                            system("cls");
                                            print_menu(switcher, menu2);
                                        }
                                        else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                        {
                                            switch (switcher - 1)
                                            {
                                            case 1:
                                            {
                                                system("cls");
                                                Passwords1.addUser();
                                                system("pause");
                                                break;
                                            }
                                            case 2:
                                                ans = 0;
                                                system("cls");
                                                break;
                                            default:
                                                ans = 0;
                                                system("cls");
                                                // system("pause");
                                                break;
                                            }
                                        }
                                    }
                                }
                                miganie = false;
                            }
                        }
                        else if (role == "mainEditor")
                            {
                                
                            //menu mainEditor
                                Article myArticle;
                            myArticle.loadArticlesFromBinaryFile("articles.bin");
                                ChiefEditor chief_editor;
                                bool ans = 1;
                                switcher = 2;
                                miganie = true;
                                while (ans)
                                {
                                    if (miganie)
                                    {
                                        system("cls");
                                        print_menu(switcher, menu3);
                                    }
                                    ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                                    switch (irInput.EventType)
                                    {
                                    case KEY_EVENT:
                                        if (irInput.Event.KeyEvent.bKeyDown)
                                        {
                                            if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                                            {
                                                if (switcher > 2)
                                                {
                                                    switcher--;
                                                }
                                                system("cls");
                                                print_menu(switcher, menu3);
                                            }
                                            else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                            {
                                                if (switcher < menu3.size())
                                                {
                                                    switcher++;
                                                }
                                                system("cls");
                                                print_menu(switcher, menu3);
                                            }
                                            else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                            {
                                                switch (switcher - 1)
                                                {
                                                case 1:
                                                {
                                                    system("cls");
                                                    ChiefEditor::createArticleFromInput();
                                                    system("pause");
                                                    break;
                                                }
                                                case 2:
                                                {
                                                    system("cls");
                                                    ChiefEditor::viewArticles();
                                                    system("pause");
                                                    break;
                                                }
                                                case 3:
                                                {
                                                    system("cls");
                                                    // addUser(users);
                                                    system("pause");
                                                    break;
                                                }

                                                case 4:
                                                    ans = 0;
                                                    system("cls");
                                                    switcher = 2;
                                                    myArticle.saveArticlesToBinaryFile("articles.bin");
                                                    break;
                                                default:
                                                    ans = 0;
                                                    system("cls");
                                                    // system("pause");
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                    miganie = false;
                                }
                            }
                            else if (role == "journalist")
                                {
                            //menu journalist
                                    bool ans = 1;
                                    switcher = 2;
                                    miganie = true;
                                    while (ans)
                                    {
                                        if (miganie)
                                        {
                                            system("cls");
                                            print_menu(switcher, menu4);
                                        }
                                        ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                                        switch (irInput.EventType)
                                        {
                                        case KEY_EVENT:
                                            if (irInput.Event.KeyEvent.bKeyDown)
                                            {
                                                if (irInput.Event.KeyEvent.wVirtualKeyCode
                                                    == KEY_UP)
                                                {
                                                    if (switcher > 2)
                                                    {
                                                        switcher--;
                                                    }
                                                    system("cls");
                                                    print_menu(switcher, menu4);
                                                }
                                                else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                                {
                                                    if (switcher < menu4.size())
                                                    {
                                                        switcher++;
                                                    }
                                                    system("cls");
                                                    print_menu(switcher, menu4);
                                                }
                                                else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                                {
                                                    switch (switcher - 1)
                                                    {
                                                    case 1:
                                                    {
                                                        system("cls");
                                                        
                                                        system("pause");
                                                        break;
                                                    }
                                                    case 2:
                                                        ans = 0;
                                                        system("cls");
                                                        break;
                                                    default:
                                                        ans = 0;
                                                        system("cls");
                                                        // system("pause");
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                        miganie = false;
                                    }
                                }
                                else if (role == "litEditor")
                                    {
                                        // menu litEditor
                                        bool ans = 1;
                                        switcher = 2;
                                        miganie = true;
                                        while (ans)
                                        {
                                            if (miganie)
                                            {
                                                system("cls");
                                                print_menu(switcher, menu5);
                                            }
                                            ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                                            switch (irInput.EventType)
                                            {
                                            case KEY_EVENT:
                                                if (irInput.Event.KeyEvent.bKeyDown)
                                                {
                                                    if (irInput.Event.KeyEvent.wVirtualKeyCode
                                                        == KEY_UP)
                                                    {
                                                        if (switcher > 2)
                                                        {
                                                            switcher--;
                                                        }
                                                        system("cls");
                                                        print_menu(switcher, menu5);
                                                    }
                                                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                                    {
                                                        if (switcher < menu5.size())
                                                        {
                                                            switcher++;
                                                        }
                                                        system("cls");
                                                        print_menu(switcher, menu5);
                                                    }
                                                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                                    {
                                                        switch (switcher - 1)
                                                        {
                                                        case 1:
                                                        {
                                                            system("cls");

                                                            system("pause");
                                                            break;
                                                        }
                                                        case 2:
                                                            ans = 0;
                                                            system("cls");
                                                            break;
                                                        default:
                                                            ans = 0;
                                                            system("cls");
                                                            // system("pause");
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                            miganie = false;
                                        }
                                    }
                                    else if (role == "photograph")
                                    {//menu photograph
                                        bool ans = 1;
                                        switcher = 2;
                                        miganie = true;
                                        while (ans)
                                        {
                                            if (miganie)
                                            {
                                                system("cls");
                                                print_menu(switcher, menu6);
                                            }
                                            ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                                            switch (irInput.EventType)
                                            {
                                            case KEY_EVENT:
                                                if (irInput.Event.KeyEvent.bKeyDown)
                                                {
                                                    if (irInput.Event.KeyEvent.wVirtualKeyCode
                                                        == KEY_UP)
                                                    {
                                                        if (switcher > 2)
                                                        {
                                                            switcher--;
                                                        }
                                                        system("cls");
                                                        print_menu(switcher, menu6);
                                                    }
                                                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                                    {
                                                        if (switcher < menu6.size())
                                                        {
                                                            switcher++;
                                                        }
                                                        system("cls");
                                                        print_menu(switcher, menu6);
                                                    }
                                                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                                    {
                                                        switch (switcher - 1)
                                                        {
                                                        case 1:
                                                        {
                                                            system("cls");
                                                            // addUser(users);
                                                            system("pause");
                                                            break;
                                                        }
                                                        case 2:
                                                        {
                                                            system("cls");
                                                            // addUser(users);
                                                            system("pause");
                                                            break;
                                                        }
                                                        case 3:
                                                            ans = 0;
                                                            system("cls");
                                                            break;
                                                        default:
                                                            ans = 0;
                                                            system("cls");
                                                            // system("pause");
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                            miganie = false;
                                        }
                                    }
                    break;
                    case 2:
                        running = 0;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        miganie = false;    
    }
   
    Passwords1.save("passwords.bin");
}

                            //bool ans = 1;
                            /*switcher = 2;
                            while (ans)
                            {
                                ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                                switch (irInput.EventType)
                                {
                                case KEY_EVENT:
                                    if (irInput.Event.KeyEvent.bKeyDown)
                                    {
                                        if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                                        {
                                            if (switcher > 2)
                                            {
                                                switcher--;
                                            }
                                            system("cls");
                                            print_menu(switcher, menu2);
                                        }
                                        else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                        {
                                            if (switcher < menu2.size())
                                            {
                                                switcher++;
                                            }
                                            system("cls");
                                            print_menu(switcher, menu2);
                                        }

                                        else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                        {
                                            switch (switcher - 1)
                                            {
                                            case 1:
                                            {
                                                page++;
                                                system("cls");
                                                printf(
                                                    "|%-7s| %-40s | %-20s | %-10s | %-6s |\n",
                                                    "ID",
                                                    "Цена",
                                                    "ФИО",
                                                    "Даты выдачи",
                                                    "Книга",
                                                    "Издательство"
                                                );
                                                printf(
                                                    "|-------+-------+-----------------------------------------+----------------------+------------+--------------|\n"
                                                );
                                                Show_list(list, 0, page);
                                                print_menu(switcher, menu2);
                                                system("pause");
                                                break;
                                            }
                                            case 2:
                                            {
                                                if (page != 1)
                                                {
                                                    page--;
                                                }
                                                system("cls");
                                                printf(
                                                    "|%-7s| %-40s | %-20s | %-10s | %-6s |\n",
                                                    "ID",
                                                    "ID",
                                                    "Цена",
                                                    "ФИО",
                                                    "Даты выдачи",
                                                    "Книга",
                                                    "Издательство"
                                                );
                                                printf(
                                                    "|-------+-------+-----------------------------------------+----------------------+------------+--------------|\n"
                                                );
                                                Show_list(list, 0, page);
                                                print_menu(switcher, menu2);
                                                system("pause");
                                                break;
                                            }
                                            default:
                                                ans = 0;
                                                system("cls");
                                                system("pause");
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    case 3:
                    {
                        system("cls");
                        Save_txt(list);
                        Save_bin(list);
                        break;
                    }
                    case 4:
                        list = NULL;
                        system("cls");
                        list = Load_bin(list, list);
                        break;
                    case 5:
                    {
                        system("cls");
                        int ans = 1;
                        int poriador = 1;
                        switcher = 1;
                        while (ans)
                        {
                            ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
                            switch (irInput.EventType)
                            {
                            case KEY_EVENT:
                                if (irInput.Event.KeyEvent.bKeyDown)
                                {
                                    if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                                    {
                                        if (switcher > 2)
                                        {
                                            switcher--;
                                        }
                                        system("cls");
                                        print_menu(switcher, menu3);
                                    }
                                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                                    {
                                        if (switcher < menu3.size())
                                        {
                                            switcher++;
                                        }
                                        system("cls");
                                        print_menu(switcher, menu3);
                                    }
                                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == VK_RETURN)
                                    {
                                        switch (switcher - 1)
                                        {
                                        case 1:
                                        {
                                            Sort_list_id(list, poriador, list);
                                            ans = 0;
                                            break;
                                        }
                                        case 2:
                                        {
                                            Sort_list_name(list, poriador, list);
                                            ans = 0;
                                            break;
                                        }
                                        case 3:
                                        {
                                            Sort_list_cost(list, poriador, list);
                                            ans = 0;
                                            break;
                                        }
                                        default:
                                            ans = 0;
                                            system("cls");
                                            system("pause");
                                            break;
                                        }
                                    }
                                }
                            default:
                                break;
                            }
                        }
                    }
                    break;
                    case 6:
                    {
                        int id_s;
                        system("cls");
                        cout << "Введите id\n", cin >> id_s;
                        edit_abonent(find_abonent(list, id_s));
                        break;
                    }
                    case 7:
                    {
                        int id_s;
                        system("cls");
                        cout << "Введите id\n", cin >> id_s;
                        delete_abonent(find_abonent(list, id_s));
                        break;
                    }
                    case 8:
                    {
                        system("cls");
                        processDoubleList(list);
                        system("cls");
                        break;
                    }
                    case 9:
                    {
                        int id_ss;
                        cout << "Введите искомое id" << endl, cin >> id_ss;
                        if (find_abonent(list, id_ss) == nullptr)
                        {
                            break;
                        }
                        Show_abonent(find_abonent(list, id_ss)->abonent, 1);
                        break;
                    }
                    case 10:
                        running = 0;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
}*/ 

void print_menu(int switcher, vector<string> menu)
{
    for (int i = 0; i < menu.size(); i++)
    {
        if (i + 2 == switcher)
        {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
            GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
            WORD saved_attributes = consoleInfo.wAttributes;
            SetConsoleTextAttribute(
                hConsole,
                FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE
                    | BACKGROUND_INTENSITY
            );
            cout << ">>" << menu[i] << endl;
            SetConsoleTextAttribute(hConsole, saved_attributes);
        }
        else
        {
            cout << "  " << menu[i] << endl;
        }
    }
}

double_list* Init_double_list(double_list* list, Abonent abonent1)
{
    if (list == NULL)
    {
        list = new double_list;
        list->next = NULL;
        list->prev = NULL;
        list->abonent = abonent1;
        return list;
    }
    else
    {
        double_list* p = list;
        list->next = Init_double_list(list->next, abonent1);
        list->next->prev = list;
    }
    return list;
}

void Show_abonent(Abonent people, int n)
{
    setlocale(LC_ALL, "ru");
    if (people.deleted != 1)
    {
        printf(
            "|%-7s|%-7s|%-40s | %-20s | %-10s | %-10s |\n",
            to_string(people.id).c_str(),
            to_string(people.cost).c_str(),
            people.name.c_str(),
            people.author.c_str(),
            people.book_name.c_str(),
            people.publisher.c_str()
        );
        printf(
            "|-------+-------+-----------------------------------------+----------------------+------------+--------------|\n"
        );
    }
}

Abonent Get_param_from_keyboard()
{
    string firstname, secondname, surname, date, book_name, author, publisher;
    int cost;
    cout << "\nИмя:\n";
    cin >> firstname;
    cout << "\nОтчество:\n";
    cin >> secondname;
    cout << "\nФамилия:\n";
    cin >> surname;
    cout << "\nДата выдачи:\n";
    cin >> date;
    cout << "\nИздатель:\n";
    cin >> publisher;
    cout << "\nНазвание книги\n";
    cin >> book_name;
    cout << "\nАвтор:\n";
    cin >> author;
    cout << "\nЦена:\n";
    cin >> cost;
    Abonent ne = Init(firstname, secondname, surname, cost, author, book_name, publisher, date);
    return ne;
}

void Save_txt(double_list* list)
{
    system("cls");
    string name;
    cout << "Введите имя файла для сохранения txt(вписать без .txt)\n", cin >> name;
    name.append(".txt");
    ofstream saver(name);
    if (!saver)
    {
        cout << "Error" << endl;
    }
    double_list* beg = list;
    int counter = 0;
    saver << "--------------------------------------------------------------" << endl;
    while (list)
    {
        counter++;
        saver << "ID: " << list->abonent.id << endl;
        saver << "COST:" << list->abonent.cost << endl;
        saver << "NAME: " << list->abonent.name << endl;
        saver << "BOOK_NAME: " << list->abonent.book_name << endl;
        saver << "AUTHOR: " << list->abonent.author << endl;
        saver << "DATE: " << list->abonent.date << endl;
        saver << "PUBLISHER: " << list->abonent.publisher << endl;
        saver << "DELETED: " << list->abonent.deleted << endl;
        saver << "--------------------------------------------------------------" << endl;
        list = list->next;
    }
    list = beg;
    saver.close();
    cout << "Сохранено";
}

void Save_bin(double_list* list)
{
    Saver element;
    system("cls");
    string name;
    cout << "Введите имя файла для сохранения bin(вписать без .bin)\n", cin >> name;
    name.append(".bin");
    ofstream saver(name, ios::binary);
    if (!saver)
    {
        cout << "Ошибка" << endl;
        return;
    }
    double_list* beg = list;
    while (list)
    {
        element.id = list->abonent.id;
        strncpy_s(element.name, const_cast<char*>(list->abonent.name.c_str()), 256);
        strncpy_s(element.author, const_cast<char*>(list->abonent.author.c_str()), 256);
        strncpy_s(element.date, const_cast<char*>(list->abonent.date.c_str()), 256);
        strncpy_s(element.book_name, const_cast<char*>(list->abonent.book_name.c_str()), 256);
        strncpy_s(element.publisher, const_cast<char*>(list->abonent.publisher.c_str()), 256);
        element.deleted = list->abonent.deleted;
        saver.write((char*) &element, sizeof(Saver));
        list = list->next;
    }
    list = beg;
    saver.close();
    cout << "Сохранено";
}

double_list* Load_bin(double_list* list, double_list* beg)
{
    id = 1;
    system("cls");
    string name;
    cout << "Введите имя файла для загрузки\n", cin >> name;
    name.append(".bin");
    ifstream loader(name);
    if (!loader)
    {
        cout << "Ошибка";
        return beg;
    }
    Saver element;
    Abonent el;
    loader.seekg(0);
    while (loader.read((char*) &element, sizeof(Saver)))
    {
        el.id = element.id;
        el.name = element.name;
        el.date = element.date;
        el.book_name = element.book_name;
        el.author = element.author;
        el.publisher = element.publisher;
        el.cost = element.cost;
        el.deleted = element.deleted;
        list = Init_double_list(list, el);
        id++;
    }
    return list;
}

void Show_list(double_list* list, int n, int page)
{
    if (list)
    {
        n += 1;
        if (n <= page * 20 && n > page * 20 - 20)
        {
            Show_abonent(list->abonent, n);
            Show_list(list->next, n, page);
        }
        else
        {
            if (n > page * 20)
            {
                return;
            }
            Show_list(list->next, n, page);
        }
    }
}

double_list* Sort_list_name(double_list* list, int a, double_list* beg)
{
    Abonent temp;
    if (list != NULL)
    {
        if (list->next != NULL)
        {
            if (list->abonent.name > list->next->abonent.name)
            {
                temp = list->abonent;
                list->abonent = list->next->abonent;
                list->next->abonent = temp;
                return Sort_list_name(beg, a, beg);
            }
            else
            {
                return Sort_list_name(list->next, a, beg);
            }
        }
        else
        {
            return list;
        }
    }
    else
    {
        return list;
    }
    return list;
}

double_list* Sort_list_cost(double_list* list, int a, double_list* beg)
{
    Abonent temp;
    if (list != NULL)
    {
        if (list->next != NULL)
        {
            if (list->abonent.cost > list->next->abonent.cost)
            {
                temp = list->abonent;
                list->abonent = list->next->abonent;
                list->next->abonent = temp;
                return Sort_list_cost(beg, a, beg);
            }
            else
            {
                return Sort_list_cost(list->next, a, beg);
            }
        }
        else
        {
            return list;
        }
    }
    else
    {
        return list;
    }
    return list;
}

double_list* Sort_list_id(double_list* list, int a, double_list* beg)
{
    Abonent temp;
    if (list != NULL)
    {
        if (list->next != NULL)
        {
            if (list->abonent.id > list->next->abonent.id)
            {
                temp = list->abonent;
                list->abonent = list->next->abonent;
                list->next->abonent = temp;
                return Sort_list_id(beg, a, beg);
            }
            else
            {
                return Sort_list_id(list->next, a, beg);
            }
        }
        else
        {
            return list;
        }
    }
    else
    {
        return list;
    }
    return list;
}

double_list* find_abonent(double_list* list, int id)
{
    double_list* list1 = list;
    while (list1)
    {
        if (list1->abonent.id == id)
        {
            return list1;
        }
        list1 = list1->next;
    }
    cout << "Не найден\n";
    return nullptr;
}

void edit_abonent(double_list* pAbonent)
{
    if (pAbonent)
    {
        pAbonent->abonent = Get_param_from_keyboard();
    }
    else
    {
        cout << "Не найден";
    }
}

void delete_abonent(double_list* pAbonent)
{
    if (pAbonent)
    {
        pAbonent->abonent.deleted = 1;
        cout << "Удален" << endl;
    }
}

void processDoubleList(double_list* list)
{
    std::unordered_map<std::string, int> debtorCount;
    double_list* current = list;

    while (current != nullptr)
    {
        if (current->abonent.deleted == 0)
        {
            debtorCount[current->abonent.name]++;
        }
        current = current->next;
    }

    std::vector<Debtor> debtors;
    for (const auto& entry : debtorCount)
    {
        debtors.push_back({entry.first, entry.second});
    }

    std::sort(
        debtors.begin(),
        debtors.end(),
        [](const Debtor& a, const Debtor& b)    
        {
            return a.count > b.count;
        }
    );
    for (int i = 0; i < 3 && i < debtors.size(); ++i)
    {
        std::cout << debtors[i].name << ": " << debtors[i].count << " задолженностей" << std::endl;
    }
    std::string filename;
    std::cout << "Введите имя файла для сохранения списка должников: ";
    std::cin >> filename;

    if (filename.find(".txt") == std::string::npos)
    {
        filename += ".txt";
    }
    std::ofstream outfile(filename);
    if (outfile.is_open())
    {
        for (const auto& debtor : debtors)
        {
            outfile << debtor.name << ": " << debtor.count << " debts" << std::endl;
        }
        outfile.close();
        std::cout << "Список должников сохранен в файле '" << filename << "'" << std::endl;
    }
    else
    {
        std::cerr << "Не удалось открыть файл для записи" << std::endl;
    }
}

    
    /* std::string authorization(std::map<std::string, std::pair<std::string, std::string>>& users)
{
     
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD NumInputs = 0;
    DWORD InputsRead = 0;
    INPUT_RECORD irInput;
    system("cls");
    std::string inputUsername, inputPassword;

    while (true)
    {
        // Запрашиваем у пользователя логин и пароль
        std::cout << "Введите логин: ";
        std::cin >> inputUsername;
        std::cout << "Введите пароль: ";
        std::cin >> inputPassword;

        auto it = users.find(inputUsername);
        if (it != users.end() && it->second.first == inputPassword)
        {
            std::cout << "Авторизация прошла успешно" << std::endl;
            return it->second.second; // Возвращаем роль пользователя
        }
        else
        {
            std::cout << "Логин или пароль неверные. Стрелка вниз попробуйте еще раз, стрелка вверх выход." << std::endl;
            ReadConsoleInput(hInput, &irInput, 1, &InputsRead);
            switch (irInput.EventType)
            {
            case KEY_EVENT:
            {
                if (irInput.Event.KeyEvent.bKeyDown)
                {
                    if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_UP)
                    {
                        system("cls");
                        continue;

                    }
                    else if (irInput.Event.KeyEvent.wVirtualKeyCode == KEY_DOWN)
                    {
                        return "0";

                        system("cls");
                    }
                }
            }
            }
            system("pause");
            system("cls");
        }
    }
}*/

    /* void addUser(std::map<std::string, std::pair<std::string, std::string>>& users)
{
    std::string roles[5]{"
        Editor", "journalist", "photograph", "litEditor", "designer"};
    int roleNumber;
    std::string username, password, role;
    std::cout << "Введите логин нового пользователя: ";
    std::cin >> username;
    std::cout << "Введите пароль нового пользователя: ";
    std::cin >> password;
    std::cout
        << "Введите роль нового пользователя(1 главный редактор, 2 журналист, 3 фотограф, 4 лит. редактор, 5 дизайнер): ";
    std::cin >> roleNumber;
    role = roles[roleNumber - 1];
    users.insert({
        username, {password, role}
    });
    std::cout << "Пользователь записан" << std::endl;
    }*/
    
    