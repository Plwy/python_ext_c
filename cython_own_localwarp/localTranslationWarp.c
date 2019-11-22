#include <stdio.h>
#include <stdlib.h> 
#include <malloc.h> 
#include <math.h>


float BilinearInsert(float f1, float f2, float f3, float f4, float x1, float y1, float x2, float y2, float UX, float UY)
{
    float p1,p2,p3,p4;
    p1 = f1*(x2-UX)*(y2-UY);
    p2 = f2*(UX-x1)*(y2-UY);
    p3 = f3*(x2-UX)*(UY-y1);
    p4 = f4*(UX-x1)*(UY-y1);
    return p1+p2+p3+p4;
}

float srcImg_c[2000][2000][3];
float copyImg_c[2000][2000][3];

void localTranslationWarp(float* srcImg, float* parma_lis)
{
    

    float startX, startY, endX, endY, radius, point_x, point_y;
    startX = parma_lis[0];
    startY = parma_lis[1];
    endX = parma_lis[2];
    endY = parma_lis[3];
    radius = parma_lis[4];
    point_x = parma_lis[5];
    point_y = parma_lis[6];

    /*将一维数组转为图像的三维数组方便处理*/
    // srcImg_c = CreateGrid_float((int)parma_lis[7], (int)parma_lis[8], 3);
    // copyImg_c = CreateGrid_float((int)parma_lis[7], (int)parma_lis[8], 3);


    for(int y=0;y<parma_lis[7];y++)
        for(int x=0;x<parma_lis[8];x++)
            for(int k=0;k<3;k++)
            {
                srcImg_c[y][x][k] = srcImg[y*((int)parma_lis[8]*3) + x*3 + k];
                copyImg_c[y][x][k] = srcImg_c[y][x][k];
            }

    
    float ddradius;
    ddradius = radius * radius;
    float ddmc;
    ddmc = (endX - startX) * (endX - startX) + (endY - startY) * (endY - startY);
    
    for (int j=point_y - radius; j<point_y + radius; j++)
        for (int i=point_x - radius; i<point_x + radius; i++)
        {
            if (j<0 || j>=parma_lis[7])
                continue;

            if (i<0 || i>=parma_lis[8])
                continue;

            float distance;
            distance = (i - point_x) * (i - point_x) + (j - point_y) * (j - point_y);
            if (distance > ddradius)
                continue;

            float ratio;
            ratio = (ddradius - distance) / ( ddradius - distance + ddmc);
            ratio = ratio * ratio;

            float UX, UY;

            UX = i - ratio*(endX - startX);
            UY = j - ratio*(endY - startY);

            if (UY<0 || UY>=parma_lis[7])
                continue;

            if (UX<0 || UX>=parma_lis[8])
                continue;

            //printf("%d %d %f %f\n", i, j, UX, UY);


            for (int k=0;k<3;k++)
            {
                int x1,y1,x2,y2;
                x1 = floor(UX);x2 = x1 + 1;
                y1 = floor(UY);y2 = y1 + 1;
                float f1,f2,f3,f4;
                f1 = srcImg_c[y1][x1][k];f2 = srcImg_c[y1][x2][k];
                f3 = srcImg_c[y2][x1][k];f4 = srcImg_c[y2][x2][k];              

                float value;
                value = BilinearInsert(f1, f2, f3, f4, (float)x1, (float)y1, (float)x2, (float)y2, UX, UY);
                copyImg_c[j][i][k] = value;
            }
        }
    

    //printf("444444\n");
    for(int y=0;y<parma_lis[7];y++)
        for(int x=0;x<parma_lis[8];x++)
            for(int k=0;k<3;k++)
            {
                srcImg[y*((int)parma_lis[8]*3) + x*3 + k] = copyImg_c[y][x][k];
            }

    return;
}
