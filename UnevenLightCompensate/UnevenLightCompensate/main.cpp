/*****************************************************************************
 函 数 名  : HSEG_GetBindImgWithLocalThreshold
 功能描述  : 局部阈值 *使用均值和标准差作为判定依据 *输入参数包括邻域大小，均值系数，以及标准差系数
 输入参数  : 
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年6月5日
    作    者   : ccy0739
    修改内容   : 新生成函数

*****************************************************************************/
ULONG HSEG_GetBindImgWithLocalThreshold(Mat& srcImg, Mat* bindImg, INT windownSize, DOUBLE meanParam, DOUBLE stadevPara)
{

    if (!srcImg.data)
    {
        PRINT_ERROR(HSEG_ERROR_NO_IMAGE);
        return HSEG_ERROR_NO_IMAGE;
    }
    
    *bindImg = cv::Mat::zeros(srcImg.size(), srcImg.type());

    UCHAR* srcImgData = srcImg.data;
    UCHAR* bindImgData = bindImg->data;
    DOUBLE deta = 0.0;
    DOUBLE mean = 0.0;
    DOUBLE data = 0.0;

    /* 跳过首尾行列 */
    for (INT col = windownSize / 2; col < (srcImg.cols - windownSize / 2); col++)
    {
        for (INT row = windownSize / 2; row < (srcImg.rows - windownSize / 2); row++)
        {
            deta = 0.0;
            mean = 0.0; 
            data = (DOUBLE)(srcImgData[col + row * srcImg.cols]);

            /* 求邻域内均值和标准差 */
            for (INT i = -(windownSize / 2); i < windownSize / 2 + 1; i++)
            {
                for (INT j = -(windownSize / 2); j < windownSize / 2 + 1; j++)
                {
                    mean += srcImgData[(row + i) * srcImg.cols + col + j];
                }
            }
            mean /= (DOUBLE)(windownSize * windownSize);
            
            for (INT i = -(windownSize / 2); i <= windownSize / 2; i++)
            {
                for (INT j = -(windownSize / 2); j <= windownSize / 2; j++)
                {
                    deta += (srcImgData[(row + i) * srcImg.cols + col + j] - mean)
                        * (srcImgData[(row + i) * srcImg.cols + col + j] - mean);
                }
            }

            deta /= (DOUBLE)(windownSize * windownSize);
            deta = sqrt(deta);

            if (data> meanParam * mean && data > stadevPara * deta)
            {

                bindImgData[col + row * srcImg.cols] = 255;
            }
        
        }
    }

    return HCOM_OK;
}