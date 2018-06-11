#include "def.h"
using namespace std;
using namespace cv;

/*****************************************************************************
 函 数 名  : CompensateLightInBlocks
 功能描述  : 不均匀光照补偿
 输入参数  : Mat &image,   灰度图像
             INT blockSize 块大小
 输出参数  : Mat* dstImg,  输出图像
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年6月5日
    作    者   : ccy0739
    修改内容   : 新生成函数

*****************************************************************************/
ULONG CompensateLightInBlocks(Mat &image, Mat* dstImg, INT blockSize)
{
    if (!image.data)
    {
        return HCOM_ERR;
    }

    /* 图像整体灰度均值 */
    DOUBLE average = mean(image)[0];

    /* 分块 */
    INT newRows = (INT)ceil(DOUBLE(image.rows) / DOUBLE(blockSize));
    INT newCols = (INT)ceil(DOUBLE(image.cols) / DOUBLE(blockSize));
    Mat blockImage;
    blockImage = Mat::zeros(newRows, newCols, CV_64FC1);

    INT minRow;
    INT maxRow;
    INT minCol;
    INT maxCol;
    DOUBLE temaver;
    Mat imageROI;

    /* 求每个块内的均值 */
    for (INT i = 0; i < newRows; i++)
    {
        for (INT j = 0; j < newCols; j++)
        {
            minRow = i * blockSize;
            maxRow = (i + 1) * blockSize;
            if (maxRow > image.rows)
            {
                maxRow = image.rows;
            }

            minCol = j * blockSize;
            maxCol = (j + 1) * blockSize;
            if (maxCol > image.cols)
            {
                maxCol = image.cols;
            }

            imageROI = image(Range(minRow, maxRow), Range(minCol, maxCol));
            temaver = mean(imageROI)[0];
            blockImage.at<DOUBLE>(i, j) = temaver;
        }
    }

    blockImage = blockImage - average;

    Mat resizeBlockImage;
    resize(blockImage, resizeBlockImage, image.size(), INTER_CUBIC);

    Mat doubleImage;
    image.convertTo(doubleImage, CV_64FC1);

    *dstImg = doubleImage - resizeBlockImage;
    (*dstImg).convertTo(*dstImg, CV_8UC1);  

    return HCOM_OK;
}