#include"common_def.h"

ULONG compensateLightInBlocks(Mat &image, Mat* dstImg, INT blockSize);
ULONG compensateLightReferenceWhite(Mat &srcImg, Mat* dstImg);

int main()
{
	
	Mat srcImg;
	Mat grayImg;
	Mat blocksImg;
	Mat referenceWhite;

	srcImg = imread("timg.jpg");
	cvtColor(srcImg, grayImg, CV_RGB2GRAY);

	compensateLightInBlocks(grayImg, &blocksImg, 10);
	imshow("blocksImg", blocksImg);

	compensateLightReferenceWhite(grayImg, &referenceWhite);
	imshow("referenceWhite", referenceWhite);

	while (true)
	{
		waitKey(0);
	}

	return 0;
}

/*****************************************************************************
 函 数 名  : compensateLightInBlocks
 功能描述  : 不均匀光照补偿，基于领域
 输入参数  : Mat &image,   灰度图像
             INT blockSize 块大小
 输出参数  : Mat* dstImg,  输出图像

 修改历史      :
  1.日    期   : 2018年6月5日
    作    者   : chengcy
    修改内容   : 新生成函数

*****************************************************************************/
ULONG compensateLightInBlocks(Mat &image, Mat* dstImg, INT blockSize)
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

/*****************************************************************************
 函 数 名  : CompensateLightReferenceWhite
 功能描述  : 基于参考白的不均匀光照补偿
 输入参数  : Mat &image,   灰度图像
             INT blockSize 块大小
 输出参数  : Mat* dstImg,  输出图像

 修改历史      :
  1.日    期   : 2018年6月11日
    作    者   : chengcy
    修改内容   : 新生成函数

*****************************************************************************/
ULONG compensateLightReferenceWhite(Mat &srcImg, Mat* dstImg)
{

    if (!srcImg.data || CV_8UC1 != srcImg.type())
    {
        return HCOM_ERR;
    }

    const DOUBLE whiteLevel = 0.01;
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