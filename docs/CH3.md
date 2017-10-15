# CH3

## 线框模型

一组边表+顶点表+点边关系

优点：简单，易处理 

功能：物体显示，物体变换，视图生成

缺点：不能保证有效性，有二义性，不完备

## 边界表示法及其数据结构

1. Brap组成：拓扑结构，几何信息
    * 拓扑结构：几何元素的种类和数目，之间的关系
        * 几何元素：点边面体环
        * 关系：VEF全连接
            * 存储效率
            * 查找效率
            * 应用需求
    * 几何信息：描述边界元素形状大小与位置信息
        * 点point：坐标
        * 曲线curve：曲线方程
        * 曲面surface：曲面方程
1. 翼变数据结构(wing-edge structure)
    * 边为中心表示三维物体的拓扑结构：EV EE EF VE(partial)，FE VE
    * 边表，环表
1. 半边数据结构(half-edge structure)
    * 将一条物理边当作两条边表示，其中每条边只与一个面相关

## 数据结构关系

solid face loop half-edge vertex

其中Half-Edge为属于Loop的边集且有序

```
graph TD
Solid-->prevSolid
Solid-->nextSolid
Solid-->Face
Face-->Solid
Face-->prevFace
Face-->nextFace
Face-->Loop
Loop-->Face
Loop-->prevLoop
Loop-->nextLoop
Loop-->Half-Edge
Half-Edge-->Loop
Half-Edge-->prevHalf-Edge
Half-Edge-->nextHalf-Edge
Half-Edge-->Vertex
Vertex-->Half-Edge
Vertex-->prevVertex
Vertex-->nextVertex
Half-Edge-->Edge
Edge-->prevEdge
Edge-->nextEdge
Solid-->Edge
```
