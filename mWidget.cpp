#include "mWidget.h"

#include <sstream>
#include <cmath>

mWidget::mWidget(const std::string& name)
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
	/*ifstream fs("input.txt");
	std::string str, name, value;

	fs >> str;
	size_t pos = str.find("=");
	if (pos!=string::npos)
	{
		name = str.substr(0,pos+1);
		value = str.substr(pos+1);
		istringstream ( value ) >> level);
	}

	fs >> str;
	pos = str.find("=");
	if (pos!=string::npos)
	{
		name = str.substr(0,pos+1);
		value = str.substr(pos+1);
		istringstream ( value ) >> count);
	}
	
	fs >> str;
	pos = str.find("=");
	if (pos!=string::npos)
	{
		name = str.substr(0,pos+1);
		value = str.substr(pos+1);
		istringstream ( value ) >> velocity);
	}
	
	
	fs >> str;
	pos = str.find("=");
	if (pos!=string::npos)
	{
		name = str.substr(0,pos+1);
		value = str.substr(pos+1);
		istringstream ( value ) >> lifeTime);
	}
	fs >> str;
	pos = str.find("=");
	if (pos!=string::npos)
	{
		name = str.substr(0,pos+1);
		value = str.substr(pos+1);
		istringstream ( value ) >> acceleration);
	}*/
	level=3;
	count=10;
	velocity=200;
	lifeTime=3;
	acceleration=40;
	return;
}

void mWidget::Init()
{
	std::string dir = "rc/textures/";
	Input();
	fon = new mTexture("fon");
	textures.push_back(fon);
	fon->Load(dir + "fon.png");
	cursor = new mTexture("cursor");
	textures.push_back(cursor);
	cursor->Load(dir + "cursor.png");
	textures.push_back(new mTexture("c"));
	textures[2]->Load(dir + "dot.png");
	
	if(cursor->isValid())std::cout << "cursor is valid" << std::endl;
	else std::cout << "cursor is NOT valid" << std::endl;
	
	if(fon->isValid())std::cout << "fon is valid" << std::endl;
	else std::cout << "fon is NOT valid" << std::endl;
	
	if(textures[2]->isValid())std::cout << "dot is valid" << std::endl;
	else std::cout << "dot is NOT valid" << std::endl;
	
	cx = width/2.0;
	cy = height/3.0*2.0;
}

void mWidget::Draw() 
{
	//
	// Проталкиваем в стек текущее преобразование координат, чтобы в дальнейшем
	// можно было восстановить это преобразование вызовом PopMatrix.
	//
	fon->angle = 0;
	fon->Draw(0, 0);
	//Render::device.PushMatrix();
	cursor->angle = angle - 45.f ;
	cursor->Draw(mx - cursor->width/2, my - cursor->height/2);
	man.Draw();
	//
	// Воостанавливаем прежнее преобразование координат, снимая со стека изменённый фрейм.
	//
/*	Render::device.PopMatrix();
	//
	// Этот вызов отключает текстурирование при отрисовке.
	//
	Render::device.SetTexturing(false);
	//
	// Метод BeginColor() проталкивает в стек текущий цвет вершин и устанавливает новый.
	//
	Render::BeginColor(Color(128, 128, 128, 128));
	//
	// Метод DrawRect() выводит в графическое устройство квадратный спрайт, состоящий их двух
	// примитивов - треугольников, используя при этом текущий цвет для вершин и привязанную (binded) текстуру,
	// если разрешено текстурирование.
	//
	// Перед вызовом DrawRect() должен быть вызов Texture::Bind(), либо SetTexturing(false),
	// иначе визуальный результат будет непредсказуемым.
	//
	Render::DrawRect(824, 0, 200, 100);
	//
	// Метод EndColor() снимает со стека текущий цвет вершин, восстанавливая прежний.
	//
	Render::EndColor();
	//
	// Опять включаем текстурирование.
	//
	Render::device.SetTexturing(true);
	//
	// Рисуем все эффекты, которые добавили в контейнер (Update() для контейнера вызывать не нужно).
	//
	effCont.Draw();

	Render::BindFont("arial");
	Render::PrintString(824 + 200 / 2, 35, "Fireworks: " + utils::lexical_cast(man.fireworks.size()) +" UseRandom: "+utils::lexical_cast(useRandom), 1.f, CenterAlign);
	Render::PrintString(824 + 200 / 2, 50, "Level: " + utils::lexical_cast(level) + " Count: " + utils::lexical_cast(count), 1.f, CenterAlign);
	Render::PrintString(824 + 200 / 2, 65, "Speed: "+ utils::lexical_cast(velocity) + " Time:" + utils::lexical_cast(lifeTime), 1.f, CenterAlign);
	Render::PrintString(824 + 200 / 2, 80, "Acceleration: "+ utils::lexical_cast(acceleration), 1.f, CenterAlign);
*/
}

void mWidget::Update(float dt)
{
	// Обновляем все фейрверки
	//
	man.Update(dt);
}

bool mWidget::MouseDown(int x, int y)
{
	//std::cout<<"mWidget::MouseDown: x="<<x<<" y="<<y<<std::endl;
	//if (Core::mainInput.GetMouseLeftButton())
	//{
		//
		// При нажатии на правую кнопку мыши, создаём Фейрверк
		//
		/*(ParticleEffect *eff = effCont.AddEffect("rocket");
		eff->posX = mouse_pos.x + 0.f;
		eff->posY = mouse_pos.y + 0.f;
		eff->Reset();
		*/
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
	//}
	return false;
}

void mWidget::MouseMove(int x, int y)
{
	mx = x;
	my = height-y;
	//std::cout<<"mWidget::MouseMove: x="<<mx<<" y="<<my<<std::endl;
	if(!verticalCanon)angle = atan((my - cy )/(mx - cx)) / M_PI * 180.0 + 90.0;
}
