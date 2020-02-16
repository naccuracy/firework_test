#ifndef M_FPOINT_H
#define M_FPOINT_H
#define M_ACCURACY 1e-3
class FPoint
{
    public:
    FPoint(float x0 = 0.f, float y0 = 0.f);
    FPoint(const FPoint& p);
    FPoint& operator=(const FPoint& p);
    FPoint operator+(const FPoint& p);
    FPoint operator-(const FPoint& p);
    FPoint operator*(float val);
    FPoint operator/(float val);
    bool operator==(const FPoint& p);
    ~FPoint();
    
    float x,y;
    float Length();
};
#endif
