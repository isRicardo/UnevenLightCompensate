不均匀光照补偿
两种方法:
一. 基于参考白；
  (1)  统计每个灰度值的像素数，通过循环得到排列前5%的像素灰度值作为参考白，则参考白像素的亮度平均值aveGray为：
      aveGray = Grayref/GrayrefNum

              式中Grayref为参考白灰度值；GrayrefNum用为参考白像素数；
  (2)  计算光照补偿系数coe: coe=255/aveGray

  (3)  原像素值分别乘以光照补偿系数coe，得到光照补偿后的像素值。
  
二. 基于领域内灰度均值
  (1) 求取源图I的平均灰度，并记录rows和cols；
  
  (2)按照一定大小，分为N*M个方块，求出每块的平均值，得到子块的亮度矩阵D；
  
  (3) 用矩阵D的每个元素减去源图的平均灰度，得到子块的亮度差值矩阵E；

  (4) 用双立方差值法，将矩阵E差值成与源图一样大小的亮度分布矩阵R；

  (5) 矫正后的图像result=I-R；
