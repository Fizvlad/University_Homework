//******************************************************
//
//2012 year
//create by Xavier (Alexandr)  email: Alex1993.10@mail.ru
//
//код предназанчен для свободного использования
//билиотека создана мною для моих личных целей она не совершенна и вы можете ее улучшать
//ее можно изменять,дополнять в соответствии со своими нуждами, передовать, копировать
//(!!!!!!!!запрещено закрытие исходного кода!!!!!!!!!!!!!)
//*****************************************************




struct pixel {      //пиксель с тремя составляющими цвета
        unsigned char b;
        unsigned char g;
        unsigned char r;
};


class Picture {
        char signature[2];                                  //тип файла, сигнатура
        unsigned int size;                                   //размер файла в байтах
        unsigned short reserved1;                      //зарезервировано
        unsigned short reserved2;                     //зарезервировано
        unsigned int pos_of_start;                    //смещение
        char name[100];
        unsigned int size_header;//=40
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
    pixel *pix;   //указатель не массив пикселей
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



// библиотека скорее всего далека от совершенства но не судите строго я еще только учусь
//вы сами можете улучшать/оптимизировать код или писать мне предложения по улучшению/оптимизации библиотеки
//надеюсь кому то еще она будет полезна кроме меня =)
