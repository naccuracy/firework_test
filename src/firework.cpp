#include "firework.h"
#include <algorithm>
#if defined(WINDOWS_PLATFORM)
    #define _USE_MATH_DEFINES
    #include <math.h>
#else
    #include <cmath>
#endif
#include <random>
#include <GLFW/glfw3.h>

using namespace std;

float mx=100.f;
default_random_engine generator;
uniform_int_distribution<int> distribution(1, (int)mx);
float dice()
{
    return (float)distribution(generator) / mx;
}

//унарный предикат для удаления взорвавшихся снарядов, используется в вызове if_remove
bool isExploded(Fireball const &fbl);
//унарный предикат для удаления завершившихся фейрверков, используется в вызове if_remove
bool isEmpty(Firework const& fw);

//класс Снаряд
//конструктор по умолчанию
Fireball::Fireball()
: v0{0.0f,1.0f}
, v{0.0f,1.0f}
, p0{}
, pos{}
, lifeTime{5.0}
, texPtr{nullptr}
, tailSize{10}
, level{1}
, t{0.0f}
, a{28.0f}
{
}
//конструктор  для инициализации переменных конкретными значениями
Fireball::Fireball(FPoint pos0, FPoint _v0, float accel, float lTime, int lev, mTexture *tPtr, int tSize)
: v0{_v0}
, v{_v0}
, p0{pos0}
, pos{pos0}
, lifeTime{lTime}
, texPtr{tPtr}
, tailSize{tSize}
, level{lev}
, t{0.0f}
, a{accel}
{
    tail = deque<FPoint>(tailSize, FPoint(-1.0f,-1.0f));
}
//Конструктор копирования
Fireball::Fireball(const Fireball &f)
:v0{f.v0}
,v{f.v}
,p0{f.pos}
,pos{f.pos}
,lifeTime{f.lifeTime}
,texPtr{f.texPtr}
,tailSize{f.tailSize}
,level{f.level}
,t{f.t}
,a{f.a}
{
    tail.assign(begin(f.tail), end(f.tail));
}
//деструктор
Fireball::~Fireball()
{
    texPtr = nullptr;//удаляем ссылку на партикловый эффект (шлейф)
}
//расчет траектории снаряда, если заряд взорван, то возвращает 0
int Fireball::Update(float dt)
{
    //движение равнозамедленное, ускорение направлено вертикально вверх, модуль задан в переменной а
    FPoint g(0.0f, -1.0f*a);//создаем вектор ускорения
    if(t < lifeTime)//если снаряд не взорван
    {
        t += dt;//увеличиваем время полета
        //v = v0 + g*t;
        //pos = p0 + v*t + g*t*t/2.0;
        //рассчитываем траекторию по уравнениям равноускоренного движения в векторной форме
        v = v + g*dt;//скорость изменяется линейно
        pos = pos + v*dt + g*dt*dt/2.0;//координаты по параболе
        tail.pop_back();
        tail.push_front(pos);
    }
    if(t >= lifeTime)return 0;//снаряд взорвался(время истекло)
    else return 1;//снаряд продолжает полет
}

void Fireball::Draw()
{
    texPtr->Draw(pos.x, pos.y);
    for(const auto& p: tail){
        if(p.x!=-1.0)texPtr->Draw(p.x, p.y);
    }
}
//класс фейрверк
//конструктор по умолчанию
Firework::Firework()
{
    //заполнение переменных значениями по умолчанию
    level = 2;
    count = 20;
    ang = 0.0;
    texPtr = NULL;
    v = FPoint(0.0, 1.0);
    life = 5.0;
    useRandom = true;
}
//конструктор для копирования
Firework::Firework(const Firework &f)
{
    level = f.level;
    count = f.count;
    ang = M_PI * 2.0 / (float)count;
    fireballs.assign(f.fireballs.begin(),f.fireballs.end());
    texPtr = f.texPtr;
    v = f.v;
    life = f.life;
    useRandom = f.useRandom;
}
//конструктор для инициализации переменных фейрверка конкретыми значениями
Firework::Firework(int lev, int cnt, bool uR, const Fireball &fb, mTexture *tPtr)
{
    level = lev;
    count = cnt;
    ang = M_PI * 2.0 / (float)count;
    useRandom = uR;
    v= fb.v;
    life = fb.lifeTime;
    fireballs.push_back(fb);
    texPtr = tPtr;
}
//деструктор
Firework::~Firework(void)
{
    //очищаем память
    fireballs.clear();
    texPtr = NULL;//удаляем ссылку на контейнер эффектов
}
//обработка снарядов фейрверка
int Firework::Update(float dt)
{
    float narrow, rnd, koef = 1.0;
    if(fireballs.empty()) return 0;//если нет зарядов, то нечего обновлять
    for(unsigned int i = 0; i < fireballs.size(); i++)//обновляем все заряды
    {
        if(fireballs[i].Update(dt) == 0)//если один заряд взорвался
        {
            //завершаем его шлейф
            /*if(fireballs[i].effPtr!=NULL)
            {
                fireballs[i].effPtr->Finish();
                fireballs[i].effPtr = NULL;
            }*/
            //создаем эффект взрыва
            //eff = effContPtr->AddEffect("boom");
            //в текущей позиции
            //eff->posX = fireballs[i].pos.x;
            //eff->posY = fireballs[i].pos.y;
            //eff->Reset();
            //создаем новые заряды
            if(fireballs[i].level != level)
            {
                rnd = dice()*(float)(M_PI/2.0);    
                for(int j = 0; j<count;j++)
                {
                    //добавляем эффекты новых снарядов в контейнер эффектов ссылку на эффект
                    //сохраняем в новом заряде
                    //ссылка там нужна чтобы передвигать за снарядом позицию эфекта
                    narrow = (float)j*ang+rnd;
                    //eff = effContPtr->AddEffect("rocket");
                    //eff->posX = fireballs[i].pos.x;
                    //eff->posY = fireballs[i].pos.y;
                    //eff->Reset();
                    
                    if(useRandom)koef = dice() + 0.1;//случайный коэффициент для задания параметро новых снарядов
                    FPoint nv= FPoint(cos(narrow),sin(narrow))*fireballs[i].v0.Length()*0.7*koef;//новая скорость меньше ~ на 30%
                    Fireball fb0(fireballs[i].pos,nv, fireballs[i].a ,life *0.3f, fireballs[i].level + 1, texPtr);//новый снаряд время жизни уменьшено на 70%, уровень больше на 1
                    fireballs.push_back(fb0);
                }
            }
        }
    }
    //удаление взорвавшихся снарядов
    fireballs.erase(remove_if(begin(fireballs), end(fireballs), &isExploded),end(fireballs));
    //возвращаем количество живых снарядов
    return (int)fireballs.size();
}

void Firework::Draw(){
    if(fireballs.empty()){ 
	    return;
    }
    fireballs[0].texPtr->bind();
    for(auto& it: fireballs){
        it.Draw();
    }
    glFlush();
}
//реализация унарного предиката для удаления взорвавшихся снарядов
bool isExploded(Fireball const& fbl)
{
    return (fbl.t >= fbl.lifeTime);
}
//класс Piro - следит за состоянием фейрверков
Piro::Piro()
{
    //нет инициализации по умолчанию
}
Piro::~Piro()
{
    //удаление фейрверков
    fireworks.clear();
}
int Piro::Update(float dt)
{
    if(fireworks.empty()) return 0;//если нет фейрверков то и нечего обновлять
    for(unsigned int i=0; i< fireworks.size();i++)//иначе обновим все фейрверки
        fireworks[i].Update(dt);
    //удаляем закончившиеся фейрверки
    fireworks.erase(remove_if(begin(fireworks), end(fireworks), &isEmpty),end(fireworks));
    return (int)fireworks.size();//возвращаем количество активных фейрверков
}
void Piro::Draw(){
    for(auto& it: fireworks){
        it.Draw();
    }    
}
int Piro::StartFirework(const Firework &fw)
{
    fireworks.push_back(fw);//добавляем новый фейрверк в список
    return (int)fireworks.size();//возвращаем количество активных фейрверков
}
//реализация унарного предиката для удаления закончившихся фейрверков
bool isEmpty(Firework const &fw)
{
    return fw.fireballs.empty();
}
