#include<iostream>
#include<fstream>
#include<math.h>
#include<cstdlib>


struct pixel {      //пиксель с тремя составляющими цвета
        unsigned char b;
        unsigned char g;
        unsigned char r;
};


class Picture {
    char signature[2]; // file signature
    unsigned int size; // file size
    unsigned short reserved_1; // reserved field
    unsigned short reserved_2; // reserved field
    unsigned int shift;
    char name[100]; // file name
    unsigned int size_header; //=40
    unsigned int width;  //ширина
    unsigned int height;  //высота
    unsigned short num; //число плоскостей =1
    unsigned short bit_per_pixel; //глубина цвета
    unsigned int compression;//тип сжатия
    unsigned int size_of_compression; //0 или размер сжатого изображения
    unsigned int horizontal_resolution; //0
    unsigned int vertical_resolution;  //0
    unsigned int used_color; //кол-во используемых цветов
    unsigned int main_color; //кол-во важных цветов
    pixel* pixArr;   //указатель не массив пикселей
    pixel color_of_pix; //цвет пикселя

    pixel set_color(int c,int r=0,int g=0,int b=0);  // установить цвет
    int tr_from_decXY(int x,int y); //перевод координат  x,y в индекс массива(координаты начинаются с нижнего левого угла рисунка точка (0,0))
    unsigned int img_size; //размер картинки
    unsigned int bit_img_size;//размер данных изображения

public:
    Picture(char *p="default",int w=200,int h=200,int color=1,int r=0,int g=0,int b=0);
        void draw();//создает рисунок (заносит всю информацию в файл)
    void put_pixel(int x=0,int y=0,int c=6,int r=0,int g=0,int b=0);//нарисовать пиксель
    void put_point(int x=0,int y=0,int c=6,int r=0,int g=0,int b=0);//нарисовать точку 2Х2 пикселей
    void line(int x1,int y1,int x2, int y2, int c=6,int r=0,int g=0,int b=0);//нарисовть линию
    void circle(int rad,int x0,int y0,int c=6,int r=0,int g=0,int b=0);//нарисовть круг с центром в точке х0,y0 и радиусом rad
    void rectangle(int x0,int y0,int w,int h,int c=6,int r=0,int g=0,int b=0);/*нарисовать прямоугольник ц центром в точке x0,y0  */



};


Picture::Picture(char *p,int w,int h,int color,int r,int g,int b)
{
    width=w/4; height=h/4;
        width*=4; height*=4;
    //----------------
    register int i;
    for (i=0;i<100;++i) name[i]=0;
    for (i=0;p[i];++i) {
        name[i]=p[i];
    }
    name[i]='.'; name[i+1]='b'; name[i+2]='m'; name[i+3]='p';
    //----------------
    signature[0]='B'; signature[1]='M';
    reserved1=0;
    reserved2=0;
    pos_of_start=54;
    size_header=40;
    used_color=0;
        compression=0;
    horizontal_resolution=0;
    bit_per_pixel=24;
    main_color=0;
    num=1;
    size_of_compression=0;
    vertical_resolution=0;
    //------------
    bit_img_size=width*height*3;//размер данных изображения в байтах
    size=bit_img_size+54;//размер файла в целом в байтах
    img_size=width*height;//разме картинки

    color_of_pix=set_color(color,r,g,b);
     pix=new pixel[img_size];  //массив пикселей самого изображения
     int j;
         for(j=0;j<img_size;j++)
     {
         pix[j]=color_of_pix;
     }
}

void Picture::draw()
{
    std::ofstream img(name,std::ios::out | std::ios::binary);
    if (!img) {
        std::cout<<"Can not open the file!!\n";
    }
                          //bitmap заголовок
    img.write((char *) &signature,2);
    img.write((char *) &size,4);
        img.write((char *) &reserved1,2);
    img.write((char *) &reserved2,2);
    img.write((char *) &pos_of_start,4);
                           ////////bitmap информация об изображении
    img.write((char *) &size_header,4);
    img.write((char *) &width,4);
    img.write((char *) &height,4);
    img.write((char *) &num,2);
    img.write((char *) &bit_per_pixel,2);
    img.write((char *) &compression,4);
    img.write((char *) &size_of_compression,4);
    img.write((char *) &horizontal_resolution,4);
    img.write((char *) &vertical_resolution,4);
    img.write((char *) &used_color,4);
    img.write((char *) &main_color,4);
        ///////
    img.write((char *) pix,bit_img_size);
    img.close();

}

pixel Picture::set_color(int c,int r,int g,int b) //для установления своего цвета нужно написать 7 и три компонента цвета
{
    pixel color;

    switch(c) {
        case 1: { color.r=255;color.g=255; color.b=255; break; } //white
        case 2: { color.r=255;color.g=0; color.b=0; break; } //red
        case 3: { color.r=0; color.g=255; color.b=0; break; } //green
        case 4: {color.r=255; color.g=255; color.b=0; break; } //yellow
                case 5: {color.r=0; color.g=0; color.b=255; break; } //blue
        case 6: {color.r=0; color.g=0; color.b=0; break; } //black
        case 7: {color.r=r; color.g=g; color.b=b; break; }
    default :
    { color.r=0; color.g=0; color.b=0; break; }
    }
    return color;
}



int Picture::tr_from_decXY(int x, int y)
{
    int loc;
   loc=x+width*y;
        return loc;
}

void Picture::put_pixel(int x, int y, int c, int r, int g, int b)
{
    int loc;
    loc=tr_from_decXY(x,y);
    color_of_pix=set_color(c,r,g,b);
    pix[loc]=color_of_pix;
}


void Picture::put_point(int x, int y, int c,int r,int g,int b)
{
    int loc;
        loc=tr_from_decXY(x,y);
    color_of_pix=set_color(c,r,g,b);
    pix[loc]=color_of_pix; pix[loc+1]=color_of_pix;
    pix[loc+width]=color_of_pix; pix[loc+width+1]=color_of_pix;

}

void Picture::line(int x1, int y1, int x2, int y2, int c,int r,int g,int b)
{
    int dx,dy,y,x;
    register int i;
    dx=abs(x2-x1); dy=abs(y2-y1);
    if (dx>=dy) {
                if(x1<x2) {
            for(i=x1;i<=x2;i++) {
                y=(int) (((y2-y1)/(x2-x1))*i + (y1-x1*((y2-y1)/(x2-x1)))) ;
                put_pixel(i,y,c,r,g,b);
            }
        }
        if(x1>x2) {
            for(i=x2;i<=x1;i++) {
                y=(int)(((y2-y1)/(x2-x1))*i + (y1-x1*((y2-y1)/(x2-x1))));
                put_pixel(i,y,c,r,g,b);
            }
        }
        }
    if (dy>dx) {
        if(y2>y1) {
                        for(i=y1;i<=y2;i++) {
                x=(int)(((x2-x1)/(y2-y1))*i + (x1-y1*((x2-x1)/(y2-y1))));
                put_pixel(x,i,c,r,g,b);
            }
        }
        if(y1>y2) {
            for(i=y2;i<=y1;i++) {
                x=(int)(((x2-x1)/(y2-y1))*i + (x1-y1*((x2-x1)/(y2-y1))));
                put_pixel(x,i,c,r,g,b);
            }
        }
    }
}

void Picture::circle(int rad, int x0, int y0, int c,int r,int g,int b)
{
    int lenght,x,y;
    int fi=0; int j;
    for(j=0;j<=rad;j++) {
    lenght=(int)3.1415*2*j;
    for(int i=0;i<=lenght*70;i++)
    {
        fi++;
                x=(int)(x0+j*cos((float)fi));
                y=(int)(y0+j*sin((float)fi));
        put_pixel(x,y,c,r,g,b);


    } }
}

void Picture::rectangle(int x0, int y0, int w, int h, int c,int r,int g,int b)
{

    for(int i=0;i<h/2;i++) {
      line(x0+w/2-width*i,y0,x0-w/2-width*i,y0,c,r,g,b);
    }
    for(int i=0;i<h/2-1;i++) {
      line(x0+w/2+width*i,y0,x0-w/2+width*i,y0,c,r,g,b);
    }
}
