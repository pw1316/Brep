# BRep 欧拉操作

## 欧拉操作

### void mvfs(float x, float y, float z)

- x, y, z分别为新定点的坐标

### BHalfEdge *mev(BLoop *loop, BVertex *vertex, float x, float y, float z);

- loop为要待添加顶点所在的环
- vertex为与新顶点构成新边的已有顶点
- x, y, z分别为新顶点的坐标

### void mef(BLoop *outLoop, BVertex *vertex1, BVertex *vertex2);

mef的重载1

- outLoop为两顶点所在的环
- vertex1为起始顶点，需手动保证vertex1->vertex2的走向为使outLoop法向向外的走向，即vertex1->vertex2走向的半边属于原环
- vertex2为终止顶点，需手动保证vertex1->vertex2的走向为使outLoop法向向外的走向，即vertex1->vertex2走向的半边属于原环

### void mef(BLoop *outLoop, BVertex *vertex1, BVertex *vertex2, BVertex *vertex3, BVertex *vertex4);

mef的重载2

- outLoop为两顶点所在的环
- vertex1为起始顶点，需手动保证vertex1->vertex3的走向为使outLoop法向向外的走向，即vertex1->vertex3走向的半边属于原环
- vertex2为vertex1->halfEdge->next->vertex，由于vertex对应的halfEdge可能不唯一，此处要显示指定
- vertex3为终止顶点，需手动保证vertex1->vertex3的走向为使outLoop法向向外的走向，即vertex1->vertex3走向的半边属于原环
- vertex4为vertex3->halfEdge->next->vertex，由于vertex对应的halfEdge可能不唯一，此处要显示指定

### void kemr(BLoop *outLoop, BEdge *edge, BVertex *vertexOnOutLoop);

- outLoop为待处理的外环
- edge为外环上待删除的边
- vertexOnOutLoop为分成两个环后显示指定该顶点所在的环为外环

### void kfmhr(BFace *outFace, BFace *innerFace);

- outFace为外部面，删除贴合面时，内部面被删除，原内部面的外环变为外部面的内环
- innerFace为内部面，删除贴合面时，内部面被删除，原内部面的外环变为外部面的内环
- innerFace暂不支持内环，且需和outFace完全贴合

## 扫成

### void sweep(BSolid *solid, BFace *face, float x, float y, float z);

- solid为对应体
- face为待扫成面，选取的面的外法向应当与扫成方向相反，或应当按扫成方向构建扫成面
- x, y, z分别为扫成方向与距离

## 对应数据结构的搜索

每个Solid包含了Face，Edge，Vertex的三个链表，欧拉操作需要的参数通过索引的方式搜索，索引为某节点在链表中的位置，索引下标从0开始。

__注意__：对于kemr与kfmhr操作，由于会删除边，删除面，这些操作之后对边和面的索引要注意，索引永远是其在链表中的位置。比如现有三条边e0，e1，e2，通过kemr删除e1构建内环后，原e2的索引变为1，链表变为e0，e1(原e2)

## Dump

Dump可将BRep信息输出为文件，按格式

```c++
Solid0
    Face0
        outLoop
        loop0
            he0
            he1
            ...
        ...
    ...
    Edge0
        he0
        he1
    ...
    Vertex0
    ...
...
```

输出

## 显示

显示采用了OpenGL框架，由MSDN提供的Demo修改而来，显示分为两种，边界显示和着色显示。默认采用着色显示，白色Lambert材质单光源，使用键盘`z`键可切换显示模式

### 显示操作

`w`, `s`, `a`, `d`控制摄像机前后左右移动

按下`鼠标右键`并拖动可更改视角

## 数据修改

线框模型的定义在`app.cpp`的`GLvoid gl_init_cb(GLsizei width, GLsizei height)`中

预提供了两个例子

1. 两个立方体的拼合
1. 带有两个孔的扫成体
