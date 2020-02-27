#ifndef M_TEXTURE_H
#define M_TEXTURE_H

#include <string>

class mTexture{
    public:
    mTexture(std::string mname = "empty");
    mTexture(const mTexture& mTex);
    mTexture& operator=(const mTexture& mTex);
    ~mTexture();
    void Load(std::string file);
    
    std::string name;
    unsigned int id;
    unsigned int render_list;
    int width;
    int height;
    float scale;
    float angle;
    float alpha;
    void Draw(int x, int y);
    void FreeGLTexture();
    bool isValid();
    void createRenderList();
    void bind();
};
#endif
