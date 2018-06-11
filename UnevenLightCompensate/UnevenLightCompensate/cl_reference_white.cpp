#include "def.h"
using namespace std;
using namespace cv;

/*****************************************************************************
 函 数 名  : CompensateLightReferenceWhite
 功能描述  : 基于参考白的不均匀光照补偿
 输入参数  : Mat &image,   灰度图像
             INT blockSize 块大小
 输出参数  : Mat* dstImg,  输出图像
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年6月11日
    作    者   : ccy0739
    修改内容   : 新生成函数

*****************************************************************************/
ULONG CompensateLightReferenceWhite(Mat &srcImg, Mat* dstImg)
{

    if (!srcImg.data || CV_8UC1 != srcImg.type())
    {
        return HCOM_ERR;
    }

    const DOUBLE whiteLevel = 0.05;
    ULONG cols = srcImg.cols;
    ULONG rows = srcImg.rows;

    /* 统计直方图 */
    INT histogram[256] = {0};
    UCHAR* grayData = srcImg.data;
    for(ULONG i = 0; i < cols; i++)
    {     
        for(ULONG j = 0;j < rows; j++)
        { 
            histogram[grayData[i + j * cols]] ++;
        }
    }

    ULONG refNum =0;
    ULONG totalNum = rows * cols ;
    ULONG grayVal;

    //前whiteLevel的高灰度像素
    for(ULONG i = 0;i < 256; i++)
    {
        if((DOUBLE)(refNum / totalNum) < whiteLevel) 
        {
            refNum += histogram[255-i]; 
            grayVal = i;
        }
        else
            break;
    }

    //参考亮度平均值
    ULONG aveGray = 0;
    refNum =0;
    for(ULONG i = 255;i >= 255 - grayVal; i--)
    {
        aveGray += histogram[i] * i;  //总的像素的个数*灰度值
        refNum += histogram[i];     //总的像素数
    }
    aveGray /=refNum;

    //光线补偿的系数
    DOUBLE coe = 255.0 / (DOUBLE )aveGray;

    *dstImg = srcImg.clone();
    UCHAR* dstData = dstImg->data;
    for(ULONG i = 0; i < cols; i++)
    {     
        for(ULONG j = 0;j < rows; j++)
        {
            dstData[i + j * cols] = saturate_cast<UCHAR>(coe * dstData[i + j * cols] + 0.5);
        }
    }

    return HCOM_OK;
}