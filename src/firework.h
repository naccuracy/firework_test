#ifndef M_FIREWORK_H
#define M_FIREWORK_H
#include "fpoint.h"
#include "mTexture.h"
#include <vector>
#include <deque>
//Класс Fireball - расчет полета заряда
class Fireball
{
public:
    //конструктор по умолчанию
    Fireball();
    //конструктор для инициализации
    Fireball(FPoint pos0, FPoint v0, float accel, float lTime, int lev, mTexture *tPtr, int tSize = 10);
    //конструктор копирования
    Fireball(const Fireball &f);
    ~Fireball();
    //расчет текущей скорости и координат, вызывается из метода Firework::Update(dt)
    int Update(float dt);
    void Draw();
    FPoint v0;            //начальная скорость
    FPoint v;            //скорость, движение равноускоренное, задает направление
    FPoint p0;            //начальная точка запуска снаряда
    FPoint pos;            //координаты
    float lifeTime;        //время жизни снаряда
    mTexture *texPtr;   //указатель на партикловый эффект(системы частиц)
    std::deque<FPoint> tail;
    int tailSize;
    int level;            //уровень снаряда
    float t;            //счетчик, содержит "возраст" снаряда
    float a;            //ускорение свободного падения
};
//Класс Firework - запуск новых зарядов при взрыве старых
class Firework
{
public:
    //конструктор по умолчанию
    Firework();
    //конструктор копирования
    Firework(const Firework &f);
    //конструктор для инициализации
    Firework(int lev, int cnt, bool uR, const Fireball& fb, mTexture *tPtr);
    ~Firework();
    //вызывается методом Piro::Update(dt)
    int Update(float dt);//вызывает расчет траектории каждого снаряда, удаляет взорванные
    void Draw();
    int level;//уровень фейрверка
    int count;//количество снарядов - осколков
    std::vector<Fireball> fireballs;//список не разорвавшихся снарядов
    float life;//время жизни снаряда
    FPoint v;//скорость
    bool useRandom;//флаг использования случайных чисел при создании новых снарядов
private:
    float ang;//вспомогательная переменная(угол между осколками)
    mTexture *texPtr;
};
//класс Piro отвечает за создание новых фейрверков и их обновление.
class Piro
{
public:
    //конструктор по умолчанию
    Piro();
    ~Piro();
    //функция обовления всех фейрверков
    int Update(float dt);
    void Draw();
    //запуск нового фейрверка
    int StartFirework(const Firework& fw);
    //список фейрверков
    std::vector<Firework> fireworks;
};
#endif
