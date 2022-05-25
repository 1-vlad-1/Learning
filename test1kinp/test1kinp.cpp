﻿#include <stdio.h>
#include <iostream>
#include <fstream>
#include "Header.h"

#pragma warning(disable : 4996)
using namespace std;
 
int main()
{
    //инициализация и ввод данных
    setlocale(LC_ALL ,"rus");
    int size = 0;
    point p;
    int pointX = 0;
    int pointY = 0;

    string path;
    cin >> noskipws >>path;
    if (path.empty())
    {
        cout << "File not specified!";
        return 0;
    }

    ifstream fin;
    fin.open(path);
    if (!fin.is_open())
    {
        cout << "File Was Not Opened!";
        return 0;
    }
    fin >> size;
   
    fin >> pointX;

    fin >> pointY;
   
    for (int i = 0; i < size; i++)
    {
        fin >> p.x[i];
        fin >> p.y[i];
    }

    fin.close();

    try
    {
        //условия верности введенных данных
    //условие проверки количества точек
        if (size < 3 || size > 100)
        {
            throw "Input data is not correct. Number of points out of range [3; 100]";
        }
        //условие проверки совпадений точек
        for (int i = 0; i < size - 1; i++)
        {
            for (int j = i + 1; j <= size - 1; j++)
            {
                if (p.x[i] == p.x[j] && p.y[i] == p.y[j])
                {
                    throw "Error: vertices of the polygon match";
                }
            }
        }
        //условия коректного ввода координат от нуля до тысячи
        for (int i = 0; i < size; i++)
        {
            if (p.x[i] < 0 || p.x[i]>1000 || p.y[i] < 0 || p.y[i]>1000)
            {
                throw "Error: parameter is not included the range [0,1000]";
            }
        }
        //условия вырождения в отрезок
        for (int i = 0; i < size - 2; i++)
        {
            if (point_on_segment(p.x[i], p.y[i], p.x[i + 2], p.y[i + 2], p.x[i + 1], p.y[i + 1]))
            {
                throw "Error: points degenerate into segment";
            }
        }
        for (int i = 0; i < size - 2; i++)
        {
            if (point_on_segment(p.x[i], p.y[i], p.x[i + 1], p.y[i + 1], p.x[i + 2], p.y[i + 2]))
            {
                throw "Error: points degenerate into segment";
            }
        }
        if (point_on_segment(p.x[size - 1], p.y[size - 1], p.x[1], p.y[1], p.x[0], p.y[0]))
        {
            throw "Error: points degenerate into segment";
        }
        if (point_on_segment(p.x[size - 2], p.y[size - 2], p.x[0], p.y[0], p.x[size - 1], p.y[size - 1]))
        {
            throw "Error: points degenerate into segment";
        }
        //условие пересечения сторон
        if (size >= 4)
        {
            for (int i = 0; i < size - 3; i++)
            {
                for (int j = i + 2; j < size - 1; j++)
                {
                    if (intersect(p.x[i], p.y[i], p.x[i + 1], p.y[i + 1], p.x[j], p.y[j], p.x[j + 1], p.y[j + 1]))
                    {
                        throw "Error: lines intersect";
                    }
                }
            }
        }


        //вычислительная часть программы
        //--------------------------------------------
        double k[100];
        for (int i = 0; i < size; i++)  
        {
            if (pointX != p.x[i])
            {
                k[i] = ((double)pointY - (double)p.y[i]) / ((double)pointX - (double)p.x[i]);
            }
            else
            {
                k[i] = 0;
            }
        }
        for (int i = 0; i < size; i++)
        {
            for (int j = i + 1; j < size - i - 1; j++)
            {
                if (k[i] == k[j])
                    k[i] = 0;
            }
        }
        
        double angular_coefficient = 0;
        for (int i = 0; i < size - 1 && angular_coefficient == 0; i++)
        {
            if (k[i] != 0 && k[i + 1] != 0)
            {
                angular_coefficient = ((double)k[i] + (double)k[i + 1]) / 2;
            } 
        }
        double coeff_b = 0;
        coeff_b = pointY - angular_coefficient * pointX;

        bool result = false;
        int j = size - 1;
        for (int i = 0; i < size; i++)
        {
             if (ray_intersection_segment((double)p.x[j], (double)p.y[j], (double)p.x[i], (double)p.y[i], angular_coefficient, coeff_b, (double)pointY, (double)pointX))
                result = !result;
            j = i;
        }
        //--------------------------------------------
        /*bool result = false;
        int j = size - 1;
        for (int i = 0; i < size; i++)
        {
            if ((p.y[i] < pointY && p.y[j] >= pointY || p.y[j] < pointY && p.y[i] >= pointY) &&
                (p.x[i] + (pointY - p.y[i]) / (p.y[j] - p.y[i]) * (p.x[j] - p.x[i]) < pointX))
                result = !result;
            j = i;
        }
        */
        //-----------------------------------------------------------------------------------------
        for (int i = 0; i < size; i++)
        {
            if (point_on_segment(p.x[i], p.y[i], p.x[i + 1], p.y[i + 1], pointX, pointY))
            {
                result = true;
            }
        }
        if (point_on_segment(p.x[0], p.y[0], p.x[size - 1], p.y[size - 1], pointX, pointY))
        {
            result = true;
        }

        ofstream fout;

        fout.open("output.txt");
        if (!fout.is_open())
        {
            throw "Output File Was Not Opened!";
        }

        if (result)
        {
            fout << "Принадлежит";
            cout << "Принадлежит";
        }
        else
        {
            fout << "Не принадлежит";
            cout << "Не принадлежит";
        }

        fout.close();
    }
    catch (const char *str)
    {
        cout << str << endl;
    }
    catch (exception ex)
    {
        cout << ex.what();
    }
}

bool point_on_segment(int x1, int y1, int x2, int y2, int x, int y)
{
    double k, b;

    //проверяем вертилкальный ли отрезок 
    if ((double)x2 == (double)x1)
    {
        //возвращаем значение в соответствии с расположением искомой точки
        return (x == x1 && y >= min(y1, y2) && y <= max(y1, y2));
    }
    
    //уравнение прямой y=kx+b
    //определяем коэффициент k
    k = ((double)y2 - (double)y1) / ((double)x2 - (double)x1);
    //определяем коэффициент b
    b = (double)y1 - k * (double)x1;

    //если искомая точка лежит на прямой
    if ((double)y == (double)x * k + b)
    {
        //если искомая точка лежит на отрезке
        if (y >= min(y1, y2) && y <= max(y1, y2) && x >= min(x1, x2) && x <= max(x1, x2))
            //точка принадлежит отрезку
            return 1;
    }
    //точка не принадлежит отрезку
    return 0;
}

bool intersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    double dot[2];
    //уравнение прямой через векторы ax + by + c = 0
    //вычисление векторов
    double a1 = y1 - y2;
    double b1 = x2 - x1;
    double c1 = x1 * y2 - x2 * y1;
    double a2 = y3 - y4;
    double b2 = x4 - x3;
    double c2 = x3 * y4 - x4 * y3;

    //если прямые параллельны
    if ((a1 / a2) == (b1 / b2))
    {
        //пересечений нет
        return 0;
    }
    //вычисление точки пересечения прямых
    double det = a1 * b2 - a2 * b1;
    dot[0] = (b1 * c2 - b2 * c1) / det;
    dot[1] = (a2 * c1 - a1 * c2) / det;

    //если точка пересечения лежит не на отрезке
    if (dot[0] > max(x1, x2) || dot[0] < min(x1, x2) || dot[0] > max(x3, x4) || dot[0] < min(x3, x4) || dot[1] > max(y1, y2) || dot[1] < min(y1, y2) || dot[1] > max(y3, y4) || dot[1] < min(y3, y4))
    {
        //пересечений нет
        return 0;
    }
    //пересечение есть
    return 1;

}

int ray_intersection_segment(double x1, double y1, double x2, double y2, double k, double b,double pointY,double pointX)
{
    double k1;
    double b1;
    double yIntersect;
    //определяем уравнение прямой на которой лежит отрезок (x1,y1),(x2,y2)
    //опредлеление коэффициента k
    
    if (x1 == x2)
    {
        k1 = 0;

        b1 = x1;

        yIntersect = k * x1 + b;
        
        double xIntersect = b1;
         
        //если точка пересечения лежит на отрезке
        if (xIntersect >= min(x1, x2) && xIntersect <= max(x1, x2) && yIntersect >= min(y1, y2) && yIntersect <= max(y1, y2))
        {
            //если стороны лежат по одну сторону от луча
            if (yIntersect >= pointY)
                //пересечение есть
                return 1;
            //иначе
            else
                //пересечений нет
                return 0;
        }
        //пересечений нет
        return 0;
    }
    else
    {
        //определение коэффициента k
        k1 = (y1 - y2) / (x1 - x2);
        //определение коэффициента b
        b1 = y1 - k1 * x1;
        //если уголвые коэффициенты прямых равны
        if (k1 == k)
        {
            //пересечений нет
            return 0;
        }

        //поиск точки персечения
        double xIntersect = (b - b1) / (k1 - k);
        double yIntersect = k * xIntersect + b;

        //если точка пересечения лежит на отрезке
        if (xIntersect >= min(x1, x2) && xIntersect <= max(x1, x2) && yIntersect >= min(y1, y2) && yIntersect <= max(y1, y2))
        {
            //если стороны лежат по одну сторону от луча
            if (y1 == y2)
            {
                if (xIntersect < pointX)
                    return 1;
                else
                    return 0;
            }
            else
            {
                if (yIntersect >= pointY)
                    //пересечение есть
                    return 1;
                //иначе
                else
                    //пересечений нет
                    return 0;
            }
        }
        //пересечений нет
        return 0;
    }
}
/*
bool cross(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    float dot[2];  // точка пересечения
    float n;
    if (y2 - y1 != 0) // a(y) != 0
    {  
        float q = (x2 - x1) / (y1 - y2); // -a(x)/a(y)
        float sn = (x3 - x4) + (y3 - y4) * q; // c(x) + c(y)*q
        if (!sn)  
        { return 0; } 
        float fn = (x3 - x1) + (y3 - y1) * q;   // b(x) + b(y)*q
        n = fn / sn;
    }
    else {
        if (!(y3 - y4)) // b(y)
        { return 0; }  
        n = (y3 - y1) / (y3 - y4);   // c(y)/b(y)
    }

    dot[0] = x3 + (x4 - x3) * n;  // x3 + (-b(x))*n
    dot[1] = y3 + (y4 - y3) * n;  // y3 +(-b(y))*n
    if (dot[0] > max(x1, x2) || dot[0] < min(x1, x2) || dot[0] > max(x3, x4) || dot[0] < min(x3, x4) || dot[1] > max(y1, y2) || dot[1] < min(y1, y2) || dot[1] > max(y3, y4) || dot[1] < min(y3, y4))
    {
        return 0;
    }
    return 1;
}
*/