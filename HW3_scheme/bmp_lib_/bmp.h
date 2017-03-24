//******************************************************
//
//2012 year
//create by Xavier (Alexandr)  email: Alex1993.10@mail.ru
//
//��� ������������ ��� ���������� �������������
//��������� ������� ���� ��� ���� ������ ����� ��� �� ���������� � �� ������ �� ��������
//�� ����� ��������,��������� � ������������ �� ������ �������, ����������, ����������
//(!!!!!!!!��������� �������� ��������� ����!!!!!!!!!!!!!)
//*****************************************************




struct pixel {      //������� � ����� ������������� �����
        unsigned char b;
        unsigned char g;
        unsigned char r;
};


class Picture {
        char signature[2];                                  //��� �����, ���������
        unsigned int size;                                   //������ ����� � ������
        unsigned short reserved1;                      //���������������
        unsigned short reserved2;                     //���������������
        unsigned int pos_of_start;                    //��������
        char name[100];
        unsigned int size_header;//=40
        unsigned int width;  //������
        unsigned int height;  //������
        unsigned short num; //����� ���������� =1
        unsigned short bit_per_pixel; //������� �����
        unsigned int compression;//��� ������
        unsigned int size_of_compression; //0 ��� ������ ������� �����������
    unsigned int horizontal_resolution; //0
    unsigned int vertical_resolution;  //0
    unsigned int used_color; //���-�� ������������ ������
    unsigned int main_color; //���-�� ������ ������
    pixel *pix;   //��������� �� ������ ��������
    pixel color_of_pix; //���� �������

    pixel set_color(int c,int r=0,int g=0,int b=0);  // ���������� ����
    int tr_from_decXY(int x,int y); //������� ���������  x,y � ������ �������(���������� ���������� � ������� ������ ���� ������� ����� (0,0))
    unsigned int img_size; //������ ��������
    unsigned int bit_img_size;//������ ������ �����������

public:
    Picture(char *p="default",int w=200,int h=200,int color=1,int r=0,int g=0,int b=0);
        void draw();//������� ������� (������� ��� ���������� � ����)
    void put_pixel(int x=0,int y=0,int c=6,int r=0,int g=0,int b=0);//���������� �������
    void put_point(int x=0,int y=0,int c=6,int r=0,int g=0,int b=0);//���������� ����� 2�2 ��������
    void line(int x1,int y1,int x2, int y2, int c=6,int r=0,int g=0,int b=0);//��������� �����
    void circle(int rad,int x0,int y0,int c=6,int r=0,int g=0,int b=0);//��������� ���� � ������� � ����� �0,y0 � �������� rad
    void rectangle(int x0,int y0,int w,int h,int c=6,int r=0,int g=0,int b=0);/*���������� ������������� � ������� � ����� x0,y0  */



};



// ���������� ������ ����� ������ �� ������������ �� �� ������ ������ � ��� ������ �����
//�� ���� ������ ��������/�������������� ��� ��� ������ ��� ����������� �� ���������/����������� ����������
//������� ���� �� ��� ��� ����� ������� ����� ���� =)
