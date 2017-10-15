# CH4

## 以面为中心

FAG(Face Adjacency [Hyper]graph)不带Hyper的用于特征识别

## 基于Brep的造型操作及其算法

难点：**拓扑关系**的构建

### 欧拉操作

1. 设计思想：
    - 设计一组通用的拓扑结构生成操作
    - 基于欧拉公式**尽量**保证操作的拓扑有效性
1. 欧拉公式：
    - 凸多面体`V - E + F = 2`
    - 扩展`V - E + F = 2 * (S - H) + R`
1. 欧拉操作的选择

    |v|e|f|h|r|s||
    |-|-|-|-|-|-|-|
    |1|1|0|0|0|0|Make EV|
    |0|1|1|0|0|0|Make EF|
    |1|0|1|0|0|1|Make VFS|
    |0|-1|0|0|1|0|Kill E Make R|
    |0|0|-1|1|1|0|Kill F Make HR|
    - 额外操作Split Edge Make Vertex
    - 操作可逆
1. 欧拉操作的功能与实现
    1. `MVFS()`：从无到有构造一个点、环、面、体
        - 输入：几何点P
        - 输出：体与点
    1. `MEV()`：在给定环上构造一个新点，一条连接新点与给定点的新边
        - 输入：给定环，给定点
        - 输出：新边其中一条半边

            ```c++
            HE* MEV(LP* lp, V* v1)
            {
                HE *he1, *he2, *he;
                V *v2;
                E *e;
                /* Alloc Memory Here */
                he1->startV = v1;
                he2->startV = v2;
                he1->wloop = he2->wloop = lp;
                he1->edge = he2->edge = e;
                e->he1 = he1;
                e->he2 = he2;
            
                if(lp->ledge == nullptr)
                {
                    he1->nxt = he2;
                    he2->nxt = he1;
                }
                else
                {
                    he1->nxt = he2;
                    for(he = lp->ledge; he->nxt->startV != v1; he = he->nxt)
                    {
                        he2->nxt = he->nxt;
                        he->nxt = he1;
                    }
                }
                return he1;
            }
            ```
    1. `MEF()`：给定外环，两个给定点；构造一个新面，新环
    1. `KEMR()`：给定环，待删[半]边；消去一条边，构造一个内环
    1. `KFMHR()`：给定体，两个贴合面；删除一个面，将其定义为另一个面的内环，在体中构造柄或合并两个物体
    1. `SEMV()`：给定边，
