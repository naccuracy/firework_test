#include <iostream>
#include <fstream>

#include <GL/gl.h>
#include <GL/glu.h>

#include "png.h"
#include "mTexture.h"

using namespace std;

//установка чтения png из ifstream
void userReadData(png_structp pngPtr, png_bytep data, png_size_t length) {
    //Here we get our IO pointer back from the read struct.
    //This is the parameter we passed to the png_set_read_fn() function.
    //Our std::istream pointer.
    png_voidp a = png_get_io_ptr(pngPtr);
    //Cast the pointer to std::istream* and read 'length' bytes into 'data'
    ((std::istream*)a)->read((char*)data, length);
}

mTexture::mTexture(std::string mname)
:name(mname)
,id(0)
,width(0)
,height(0)
,scale(1.f)
,angle(0.f)
,alpha(1.f)
{
    cout<< "ctor mTextures: "<< mname << endl;
}

mTexture::~mTexture(){
    FreeGLTexture();
    id = 0;
}

void mTexture::FreeGLTexture(){
    if(glIsTexture(id)){
        glDeleteTextures(1, &id);    
        cout << "glDeleteTextures "<<name<<" id#" << id << endl;
    }else{
        cout << "error " << id << " is not valid!"<< endl;
    }
}

bool mTexture::isValid()
{
    return glIsTexture(id);
}

void mTexture::Load(std::string file){
    png_byte header[8];
    cout << "Loading..."<< file << endl;
    ifstream fp(file.c_str(), ifstream::binary);
    if (fp.is_open() == false)
    {
        perror(file.c_str());
        return;
    }

    // read the header
    fp.read((char*)header, 8);

    if (png_sig_cmp(header, 0, 8))
    {
        cout << "error: " << file <<" is not a PNG.\n";
        fp.close();
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr)
    {
        cout <<  "error: png_create_read_struct returned 0.\n";
        fp.close();
        return;
    }

    // create png info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
    {
        cout << "error: png_create_info_struct returned 0.\n";
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        fp.close();
        return;
    }

    // create png info struct
    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info)
    {
        cout << "error: png_create_info_struct returned 0.\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
        fp.close();
        return;
    }

    // the code in this if statement gets called if libpng encounters an error
    if (setjmp(png_jmpbuf(png_ptr))) {
        cout << "error from libpng\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fp.close();
        return;
    }

    // init png reading form istream
    png_set_read_fn(png_ptr,(png_voidp)&fp, userReadData);

    // let libpng know you already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);

    // read all the info up to the image data
    png_read_info(png_ptr, info_ptr);

    // variables to pass to get info
    int bit_depth, color_type;
    png_uint_32 temp_width, temp_height;

    // get info about png
    png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
        NULL, NULL, NULL);
        
    width = temp_width;
    height = temp_height;

    // Update the png info struct.
    png_read_update_info(png_ptr, info_ptr);

    // Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    // glTexImage2d requires rows to be 4-byte aligned
    rowbytes += 3 - ((rowbytes-1) % 4);
    
    // Allocate the image_data as a big block, to be given to opengl
    png_byte * image_data;
    image_data = new png_byte[rowbytes * temp_height * sizeof(png_byte)];
    if (image_data == NULL)
    {
        cout << "error: could not allocate memory for PNG image data\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        fp.close();
        return;
    }

    // row_pointers is for pointing to image_data for reading the png with libpng
    png_bytep * row_pointers = new png_bytep[temp_height * sizeof(png_bytep)];
    if (row_pointers == NULL)
    {
        cout << "error: could not allocate memory for PNG row pointers\n";
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        delete image_data;
        fp.close();
        return;
    }

    // set the individual row_pointers to point at the correct offsets of image_data
    unsigned int i;
    for (i = 0; i < temp_height; i++)
    {
        row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
    }

    // read the png into image_data through row_pointers
    png_read_image(png_ptr, row_pointers);
    
    
    // Generate the OpenGL texture object
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp_width, temp_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    cout << "Loaded tex# "<<id<<endl;
    cout << "Width "<<width<<endl;
    cout << "Height "<<height<<endl;
    // clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    delete image_data;
    delete row_pointers;
    fp.close();
    return;    
}

void mTexture::Draw(int x, int y){
    if(!glIsTexture(id)){
        return;
    }
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1);
    glScalef(scale,scale,scale);
    glBindTexture(GL_TEXTURE_2D, id);
    glBegin(GL_TRIANGLES);
    glColor4f(1.0, 1.0, 1.0, alpha);
    glTexCoord2f(0.0, 0.0); glVertex2f(0, 0);
    glTexCoord2f(0.0, 1.0); glVertex2f(0, height);
    glTexCoord2f(1.0, 0.0); glVertex2f(width, 0);
    glTexCoord2f(1.0, 1.0); glVertex2f(width, height);
    glTexCoord2f(1.0, 0.0); glVertex2f(width, 0);
    glTexCoord2f(0.0, 1.0); glVertex2f(0, height);
    glEnd();
    glFlush();
    glPopMatrix();
    //cout << "draw # "<<id<<endl;
}

mTexture::mTexture(const mTexture& mTex)//имя OpenGL текстуры не копируется
{
    *this = mTex;
}

mTexture& mTexture::operator=(const mTexture& mTex){
    if(id==mTex.id)
    {
        return *this;
    }
    name = mTex.name;
    id = mTex.id;
    width = mTex.width;
    height = mTex.height;
    scale = mTex.scale;
    angle = mTex.angle;
    alpha = mTex.alpha;
    return *this;
}
