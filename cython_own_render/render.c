#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h> 
#include <math.h>


void get_mat_A_B(float coor[3][5], float A[2][2], float*B)
{
    /*由坐标得到三角仿射变换的矩阵*/
    float x1_A, y1_A, x2_A, y2_A, x3_A, y3_A;
    float x1_B, y1_B, x2_B, y2_B, x3_B, y3_B; 
    
    x1_B = coor[0][0];
    y1_B = coor[0][1];
    x1_A = coor[0][2];
    y1_A = coor[0][3];

    x2_B = coor[1][0];
    y2_B = coor[1][1];
    x2_A = coor[1][2];
    y2_A = coor[1][3];

    x3_B = coor[2][0];
    y3_B = coor[2][1];
    x3_A = coor[2][2];
    y3_A = coor[2][3];

    float mat_1[2][2];
    float mat_2[2][2];
    mat_1[0][0] = x1_B - x2_B;mat_1[0][1] = x1_B - x3_B;
    mat_1[1][0] = y1_B - y2_B;mat_1[1][1] = y1_B - y3_B;

    mat_2[0][0] = x1_A - x2_A;mat_2[0][1] = x1_A - x3_A;
    mat_2[1][0] = y1_A - y2_A;mat_2[1][1] = y1_A - y3_A;
    

    float a,b,c,d;
    a = mat_2[0][0];b=mat_2[0][1];
    c = mat_2[1][0];d=mat_2[1][1];

    float fm = a*d - b*c;

    /*保证分母不为0*/
    if(fm == 0)
        fm = 0.001;
    
    mat_2[0][0]=d/fm;mat_2[0][1]=-b/fm;
    mat_2[1][0]=-c/fm;mat_2[1][1]=a/fm;


    for(int i=0;i<2;i++)
        for(int j=0;j<2;j++)
    {
        A[i][j] = 0;
        for(int k=0;k<2;k++)
            A[i][j] = A[i][j] + mat_1[i][k]*mat_2[k][j];
        
    }

    float Y[2],X[2];
    Y[0] = x1_B;Y[1] = y1_B;
    X[0] = A[0][0]*x1_A + A[0][1]*y1_A;
    X[1] = A[1][0]*x1_A + A[1][1]*y1_A;
    B[0] = Y[0] - X[0];
    B[1] = Y[1] - X[1];
    return;
}

float dist(float x1, float y1, float x2, float y2)
{
    float l;
    l = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
    l = sqrtf(l);
    return l;
}

float get_area(float x1, float y1, float x2, float y2, float x3, float y3)
{
    float l1,l2,l3;
    l1 = dist(x1, y1, x2, y2);
    l2 = dist(x2, y2, x3, y3);
    l3 = dist(x1, y1, x3, y3);

    float ll = (l1 + l2 + l3) / 2;
    float area;

    area = sqrt((ll)*(ll-l1)*(ll-l2)*(ll-l3));
    return area;
}


int in_tri(float x, float y, float x1, float y1, float x2, float y2, float x3, float y3)
{
    float area, area1, area2, area3;    
    area = get_area(x1, y1, x2, y2, x3, y3);    
    area1 = get_area(x, y, x1, y1, x2, y2);    
    area2 = get_area(x, y, x1, y1, x3, y3);    
    area3 = get_area(x, y, x2, y2, x3, y3);
    //printf("%f\n", area1 + area2 + area3 - area);
    if (abs(area1 + area2 + area3 - area)<=1)        
        return 1;    

    // float tmp = (y1-y2)*x3 + (x2-x1)*y3 + x1*y2-x2*y1;    
    // if (tmp<=0.001)    
    // {        
    //     float tmp1 = (y1-y2)*x+(x2-x1)*y3+x1*y2-x2*y1;        
    //     if (tmp1<=0.001)        
    //         return 1;
    // }        
    // else
    //     return 0;

    return 0;
}


float coor_cc[1000][3][5];

void render(float* img_A, float* img_B, float* coor, float* black_pic, float* Map, int img_A_h, int img_A_w, int img_B_h, int img_B_w, int coor_len)
{
    for(int y=0;y<coor_len;y++)
        for(int x=0;x<3;x++)
            for(int k=0;k<5;k++)
                coor_cc[y][x][k] = coor[y*(5*3) + x* 5 + k];
    /*赋值完成*/

    /*渲染*/
    for(int k=0;k<coor_len;k++)
    {
        float A[2][2];
        float B[2];
        //printf("get_mat\n");
        get_mat_A_B(coor_cc[k], A, B); //获取三角仿射变换方程 矩阵已经验证过，是对的
        
        //printf("render\n");
        float x1,y1,x2,y2,x3,y3;
        x1 = coor_cc[k][0][2];x2 = coor_cc[k][1][2];x3 = coor_cc[k][2][2];
        y1 = coor_cc[k][0][3];y2 = coor_cc[k][1][3];y3 = coor_cc[k][2][3];
        //获取坐标 正确
        float x_min, y_min, x_max, y_max;
        x_min = x1;
        if (x_min > x2)
            x_min = x2;
        if (x_min > x3)
            x_min = x3;

        x_max = x1;
        if (x_max < x2)
            x_max = x2;
        if (x_max < x3)
            x_max = x3;

        y_min = y1;
        if (y_min > y2)
            y_min = y2;
        if (y_min > y3)
            y_min = y3;

        y_max = y1;
        if (y_max < y2)
            y_max = y2;
        if (y_max < y3)
            y_max = y3;

        for(int x=(int)x_min;x<=(int)x_max;x++)
            for(int y=(int)y_min;y<=(int)y_max;y++)
        {
            if (in_tri((float)x,(float)y,x1,y1,x2,y2,x3,y3) == 1)//如果在三角形中
            {
                
                int xx,yy;
                xx = (int)(A[0][0] * x + A[0][1] * y + B[0]);
                yy = (int)(A[1][0] * x + A[1][1] * y + B[1]);
                if(xx >=0 && xx < img_A_w && yy >= 0 && yy < img_A_h && x >= 0 && x < img_B_w && y>=0 && y < img_B_h)
                {
                    for (int c=0;c<3;c++)
                    {
                        int num_black_pic = y*img_B_w*3 + x*3 +c;
                        int num_img_A = yy*img_A_w*3 + xx*3 +c;
                        black_pic[num_black_pic] = img_A[num_img_A];
                    }
                }
                int map_num = y*img_B_w + x;
                Map[map_num] = 1;
            }
        }

    }
    return;//img_A_c[5][10][2];
}