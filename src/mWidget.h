#ifndef M_WIDGET_H
#define M_WIDGET_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include "firework.h"
///
/// Виджет - основной визуальный элемент на экране.
/// Он отрисовывает себя, а также может содержать другие виджеты.
///
class mWidget
{
public:
    mWidget(const std::string& name);
    ~mWidget();
    void Draw();
    void Update(float dt);
    
    bool MouseDown(int x, int y);
    void MouseMove(int x, int y);

    void Init();
    void Input();//чтение настроек
    
    //Widget info
    std::string mname;
    int width;
    int height;
    int mx, my;//mouse_pos
    mTexture *fon, *cursor;
    std::vector<mTexture*> textures;
    float angle;//угол, навпралвение полета новых снарядов
    int level;//уровень фейрверков
    int count;//количество осколков
    float velocity;//модуль начальной скорости
    float lifeTime;//время жизни снарядов
    
    int cx, cy;//координаты цели
    bool verticalCanon;//флаг вертикального запуска снарядов
    bool useRandom;//флаг ипользования случайных коэффициентов
    float acceleration;// ускорение
    Piro man;//Пироман)
};
#endif
