#include "mWidget.h"

#include <sstream>
#include <cmath>

using namespace std;

mWidget::mWidget(const string& name)
    : mname(name)
    , width(100)
    , height(100)
    , mx(0)
    , my(0)
    , fon(nullptr)
    , cursor(nullptr)
    , angle(0.f)
    , level(2)
    , count(20)
    , velocity(150.0)
    , lifeTime(5.0)
    , cx(0)
    , cy(0)
    , verticalCanon(true)
    , useRandom(true)
    , acceleration(30)
{
    Init();
}

mWidget::~mWidget()
{
    if(textures.size())
    {
        for(size_t i =0; i<textures.size(); ++i){
            delete textures[i];
        }
    }
    fon = nullptr;
    cursor = nullptr;
}
void mWidget::Input()
{
    ifstream fs("rc/input.txt");
    string str, name, value;

    fs >> str;
    size_t pos = str.find("=");
    if (pos!=string::npos)
    {
        name = str.substr(0,pos+1);
        value = str.substr(pos+1);
        istringstream ( value ) >> level;
    }

    fs >> str;
    pos = str.find("=");
    if (pos!=string::npos)
    {
        name = str.substr(0,pos+1);
        value = str.substr(pos+1);
        istringstream ( value ) >> count;
    }
    
    fs >> str;
    pos = str.find("=");
    if (pos!=string::npos)
    {
        name = str.substr(0,pos+1);
        value = str.substr(pos+1);
        istringstream ( value ) >> velocity;
    }
    
    
    fs >> str;
    pos = str.find("=");
    if (pos!=string::npos)
    {
        name = str.substr(0,pos+1);
        value = str.substr(pos+1);
        istringstream ( value ) >> lifeTime;
    }
    fs >> str;
    pos = str.find("=");
    if (pos!=string::npos)
    {
        name = str.substr(0,pos+1);
        value = str.substr(pos+1);
        istringstream ( value ) >> acceleration;
    }
    //level=3;
    //count=10;
    //velocity=200;
    //lifeTime=3;
    //acceleration=40;
    return;
}

void mWidget::Init()
{
    string dir = "rc/textures/";
    Input();
    fon = new mTexture("fon");
    textures.push_back(fon);
    fon->Load(dir + "fon.png");
    cursor = new mTexture("cursor");
    textures.push_back(cursor);
    cursor->Load(dir + "cursor.png");
    textures.push_back(new mTexture("dot"));
    textures[2]->Load(dir + "dot.png");
    
    if(cursor->isValid())cout << "cursor is valid" << endl;
    else cout << "cursor is NOT valid" << endl;
    
    if(fon->isValid())cout << "fon is valid" << endl;
    else cout << "fon is NOT valid" << endl;
    
    if(textures[2]->isValid())cout << "dot is valid" << endl;
    else cout << "dot is NOT valid" << endl;
    
    cx = width/2.0;
    cy = height/3.0*2.0;
}

void mWidget::Draw() 
{
    fon->angle = 0;
    fon->Draw(0, 0);
    cursor->angle = angle - 45.f ;
    cursor->Draw(mx - cursor->width/2, my - cursor->height/2);
    man.Draw();
}

void mWidget::Update(float dt)
{
    // Обновляем все фейрверки
    //
    man.Update(dt);
}

bool mWidget::MouseDown(int x, int y)
{
    float a = (angle+90.0)* M_PI / 180.0;
    FPoint pos = FPoint(x, height - y);
    FPoint speed = FPoint(cos(a),sin(a))*velocity;
    Fireball fbl(pos, 
                speed,
                acceleration,
                lifeTime,
                1, //первый уровень фейрверка
                textures[2]);
    Firework fwrk(level, count, useRandom, fbl, textures[2]);
    man.StartFirework(fwrk);
    return false;
}

void mWidget::MouseMove(int x, int y)
{
    mx = x;
    my = height-y;
    //cout<<"mWidget::MouseMove: x="<<mx<<" y="<<my<<endl;
    if(!verticalCanon)angle = atan((my - cy )/(mx - cx)) / M_PI * 180.0 + 90.0;
}
