#include<bmp.h>
#include<cctype>
#include<math.h>
#include<iostream>

int main()
{
    Picture image("D:\\image",1200,1200,1);
   // image.put_pixel(300,300,2);
  // image.put_point(300,300,2);
    image.line(0,0,1199,1199,6);
    image.circle(200,400,600,4);
    image.line(0,600,1199,600,2);
    image.line(592,0,592,1199,2);
    image.rectangle(800,300,100,400,7,255,145,24);


    for(int x=0;x<=1198;x++)
    {
        int y;

        image.put_point(x,(int)(sin(x*20*3.14)*70+600),5);
    }
    for(int x=0;x<=1198;x++)
    {
        int y;

        image.put_point(x,(int)(cos(x*20*3.14)*60+600),3);
    }

    image.draw();
    std::cout<<"ready";
    return 1;
}
