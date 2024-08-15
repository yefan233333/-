# rune_detect
这是一个实现神符自瞄的测试

### 整体思路：

1. 先对视频图像进行预处理、得到其中的有效图像。

2. 从有效图像中分离出扇叶轮廓、扇叶末端的圆的圆心。（不必找出所有圆的圆心，一个就够）

3. 通过扇叶轮廓的最小面积矩形得到连接扇叶中心与旋转中心的直线方程（五个扇叶即可得到五个直线方程）

4. 求出五个直线两两的交点（多个交点），再对这多个交点取平均值，得到旋转中心的坐标。

5. 利用子轮廓数，对已击打扇叶和未击打扇叶进行区分，并用不同颜色的矩形框出。

6. 创建一个类用来装载每片扇叶的各项属性。

7. 实现两张相邻视频帧图像中扇叶、圆的位置匹配。

8. 通过圆心与旋转中心的相对位置的变化规律、求出相邻几帧内的神符旋转速度。

9. 将一段时间内收集到的数据集进行处理、拟合旋转方程。

10. 根据当前帧的已知圆心位置以及旋转中心位置得到其余所有的圆心位置。

11. 根据拟合得到的旋转方程预测未来的圆心位置。

    

