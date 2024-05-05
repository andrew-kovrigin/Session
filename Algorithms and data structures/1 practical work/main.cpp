#include <iostream>
#include <cmath>
#include <limits>
#include <fstream>
#include <windows.h>

using namespace std;

struct Point
{
    double x, y, z, p;
    Point* next;
};

struct Space
{
    Point* head;
    int size;
};

Space createSpace()
{
    Space space;
    space.head = nullptr;
    space.size = 0;
    return space;
}

void addPoint(Space& space, double x, double y, double z, double p)
{
    Point* newPoint = new Point;
    newPoint->x = x;
    newPoint->y = y;
    newPoint->z = z;
    newPoint->p = p;
    newPoint->next = nullptr;

    if (space.head == nullptr)
    {
        space.head = newPoint;
    }
    else
    {
        Point* temp = space.head;
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newPoint;
    }

    space.size++;
}

void readPointsFromFile(Space& space, const string& filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Ошибка открытия файла." << endl;
        return;
    }

    double x, y, z, p;
    while (file >> x >> y >> z >> p)
    {
        addPoint(space, x, y, z, p);
    }

    file.close();
}

Point* selectPoint(const Space& space, int index)
{
    if (index < 0 || index >= space.size)
    {
        cerr << "Ошибка: индекс точки выходит за границы пространства" << endl;
        return nullptr;
    }

    Point* temp = space.head;
    for (int i = 0; i < index; ++i)
    {
        temp = temp->next;
    }

    return temp;
}

double distance(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                (p1.y - p2.y) * (p1.y - p2.y) +
                (p1.z - p2.z) * (p1.z - p2.z));
}

bool hasPoints(const Space& space)
{
    return space.head != nullptr;
}

int countPoints(const Space& space)
{
    return space.size;
}

void clearSpace(Space& space)
{
    Point* temp = space.head;
    while (temp != nullptr)
    {
        Point* next = temp->next;
        delete temp;
        temp = next;
    }
    space.head = nullptr;
    space.size = 0;
}

Point centerOfGravity(const Space& space)
{
    Point center;
    center.x = 0.0;
    center.y = 0.0;
    center.z = 0.0;
    center.p = 0.0;

    Point* temp = space.head;
    while (temp != nullptr)
    {
        center.x += temp->x * temp->p;
        center.y += temp->y * temp->p;
        center.z += temp->z * temp->p;
        center.p += temp->p;
        temp = temp->next;
    }

    center.x /= center.p;
    center.y /= center.p;
    center.z /= center.p;

    return center;
}

void printSpaceInfo(const Space& space)
{
    Point center = centerOfGravity(space);
    cout << "\nКоординаты центра тяжести системы: "
         << center.x << ", "
         << center.y << ", "
         << center.z << endl;

    Point* temp = space.head;
    while (temp != nullptr)
    {
        double dist = distance(center, *temp);
        cout << "Адрес: " << temp << " | Координаты: "
             << temp->x << ", "
             << temp->y << ", "
             << temp->z << ", "
             << temp->p << " | Расстояние от центра тяжести до точки: "
             << dist << endl;
        temp = temp->next;
    }
}

void removePoint(Space& space, int index)
{
    if (index < 0 || index >= space.size)
    {
        cerr << "Ошибка: индекс точки выходит за границы пространства" << endl;
        return;
    }

    if (index == 0)
    {
        Point* temp = space.head;
        space.head = space.head->next;
        delete temp;
    }
    else
    {
        Point* prev = space.head;
        for (int i = 0; i < index - 1; ++i)
        {
            prev = prev->next;
        }

        Point* temp = prev->next;
        prev->next = temp->next;
        delete temp;
    }

    space.size--;
}

string get_path()
{
    char buffer[MAX_PATH] = {};
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    char *lstChr = strrchr(buffer, '\\');
    *lstChr = '\0';
    return buffer;
}


int main()
{
    SetConsoleOutputCP(CP_UTF8);
    int choice;
    Space space = createSpace();
    Point* temp;
    while(true)
    {
        cout << "Выберите способ ввода точек:" << endl;
        cout << "1. Ввод из файла (points.txt)" << endl;
        cout << "2. Ручной ввод" << endl;
        if (hasPoints(space))
        {
            int numOfPoints = countPoints(space);
           cout << "3. Выбор любой точки в доступном пространстве | всего их : "
                << numOfPoints << endl;
        }
        if (hasPoints(space))
        {
            cout << "4. Удалить любую точку из пространства" << endl;
        }
        if (hasPoints(space))
        {
            cout << "5. очистить пространство" << endl;
        }
        if (hasPoints(space))
        {
            cout << "6. вывести сведение о пространстве" << endl;
        }
        cout << "7. Выход." << endl;
        cin >> choice;

        if(choice == 1)
        {
            readPointsFromFile(space, get_path()+"\\"+"points.txt");
        }
        else if(choice == 2)
        {
            int n;
            int x, y ,z, p;
            cout << "Введите количество точек:\n";
            while (!(cin >> n) || n <= 0 || n >= 26)
            {
                cout << "Ошибка ввода. " << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cout << "Введите координаты точек в формате x y z weight:" << endl;
            for (int i = 0; i < n; ++i)
            {
                while (!(cin >> x >> y >> z >> p))
                {
                    cout << "Ошибка ввода. в этом блоке только числа\n";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                    addPoint(space, x, y, z, p);
            }
            printSpaceInfo(space);
        }
        else if(choice == 3)
        {
            system("CLS");
            printSpaceInfo(space);
            int index;
            cout << "Введите индекс выбранной точки: ";
            cin >> index;
            Point* selectedPoint = selectPoint(space, index);
            if (selectedPoint != nullptr)
            {
                cout << "Выбранная точка: Адрес: " << selectedPoint << " | Координаты: "
                << selectedPoint->x << ", "
                << selectedPoint->y << ", "
                << selectedPoint->z << ", "
                << selectedPoint->p << endl;
            }
        }
        else if(choice == 4)
        {
            system("CLS");
            int i;
            cout << "какую точку будем удалять?\n";
            cin >> i;
            removePoint(space, i);
        }
        else if(choice == 5)
        {
            system("CLS");
            clearSpace(space);
        }
        else if(choice == 6)
        {
            system("CLS");
            printSpaceInfo(space);
        }
        else if(choice == 7)
        {
            system("CLS");
            temp = space.head;
            while (temp != nullptr)
            {
                Point* next = temp->next;
                delete temp;
                temp = next;
            }
            break;
        }
        else
        {
            cout << "Ошибка: некорректный выбор." << endl;
        }
    }
    cout << "Нажмите клавишу Ввод для завершения...";
    cin.sync();
    cin.ignore( numeric_limits <streamsize> ::max(), '\n' );
    return 0;
}
