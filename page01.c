//page01.c 页面置换算法LRU、FIFO、OPT，程序模板，由学生完成缺失代码
#include <stdio.h>
#include <stdlib.h>
#include<string.h>
const int StartPos = 90;
int  AccessSeq[] = { 7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1 };   //20个序号


typedef struct _PageNum
{
    int        iNum;
    int       iDistance;//距离当前的距离
    struct    _PageNum *next;
}PageNum;


int BlockNum = 3;//物理块数量会影响算法产生的页面置换次数

PageNum* headOriPage;//链表头指针
PageNum* InitPageSeq()
{
    PageNum* onePage = NULL;
    PageNum* newPage = NULL;
    int i = 0;
    for (; i < 20; i++)
    {
        newPage = (PageNum*)malloc(sizeof(PageNum));
        newPage->iNum=AccessSeq[19 - i];//采用逆序进入队首
        newPage->iDistance = 19 - i;//距离属性用于OPT算法
        newPage->next = onePage;
        onePage = newPage;
    }
    return onePage;
}
//释放链表空间
void ClearPageSeq(PageNum* hPage)
{
    PageNum* onePage = hPage;
    PageNum* fPage = NULL;

    while (onePage != NULL)
    {
        fPage = onePage->next;
        free(onePage);
        onePage = fPage;
    }
    return;
}
//输出链表信息
void PrintPageSeq(PageNum* hPage)
{
    PageNum* onePage = hPage;
    while (onePage != NULL)
    {
        printf("%d,", onePage->iNum);
        onePage = onePage->next;
    }
    printf("\n");
    return;
}


PageNum* headBlockPage;//块链表头指针
PageNum* rearBlockPage;//块链表头指针
PageNum* pExistBlockPage = NULL;//存在页面指针
PageNum* pPreExistBlockPage = NULL;//存在页面指针前驱
//检查当前页面号是否在块链表中
//存在返回1，同时pExistBlockPage指向存在的页面，pProExistBlockPage指向存在页面的前驱
int CheckPageInBlock(PageNum*thePage)
{
    int iResult = 0;
    PageNum* onePage = headBlockPage;
    PageNum* pPreonePage = NULL;
/*  begin ************************************************************** */
    pExistBlockPage = NULL;
    pPreExistBlockPage = NULL;
    while (onePage != NULL)
    {
        if (onePage->iNum == thePage->iNum)
        {
            pExistBlockPage = onePage;
            pPreExistBlockPage = pPreonePage;
            iResult = 1;
            break;
        }
        pPreonePage = onePage;
        onePage = onePage->next;
    }
/*   end  ************************************************************** */
    return iResult;
}


int LRU()
{ //最近最少使用LRU页面置换算法
    int iPageCount = 0;
    int iEliTime = 0;
    PageNum* onePage = headOriPage;
    PageNum* oneBPage = NULL;
    PageNum* thePage = NULL;
    //块链表首个节点
    headBlockPage = (PageNum*)malloc(sizeof(PageNum));
    memcpy(headBlockPage, onePage, sizeof(PageNum));
    rearBlockPage = headBlockPage;
    headBlockPage->next = NULL;
    iPageCount = 1;//块链表节点初始节点为１个
    onePage = onePage->next;
/*  begin ************************************************************** */
    while (onePage != NULL)
    {
        if (CheckPageInBlock(onePage))
        {
            if (pExistBlockPage != rearBlockPage)
            {
                if (pPreExistBlockPage == NULL)
                    headBlockPage = pExistBlockPage->next;
                else
                    pPreExistBlockPage->next = pExistBlockPage->next;
                pExistBlockPage->next = NULL;
                rearBlockPage->next = pExistBlockPage;
                rearBlockPage = pExistBlockPage;
            }
        }
        else
        {
            thePage = (PageNum*)malloc(sizeof(PageNum));
            memcpy(thePage, onePage, sizeof(PageNum));
            thePage->next = NULL;
            if (iPageCount < BlockNum)
            {
                rearBlockPage->next = thePage;
                rearBlockPage = thePage;
                iPageCount++;
            }
            else
            {
                PageNum* temp = headBlockPage;
                headBlockPage = headBlockPage->next;
                free(temp);
                rearBlockPage->next = thePage;
                rearBlockPage = thePage;
                iEliTime++;
            }
        }
        onePage = onePage->next;
    }
/*   end  ************************************************************** */
    return iEliTime;
}



int OPT()
{    //OPT算法思想是查看块链表中页面号在未来访问序列中距离当前最近的距离，不重复出现的页面号则距离为非常远
    //在计算时，对块链表中的页面号进行距离统计，选中距离最远的页面号进行淘汰
    //虽然本算法能够计算出结果，但实际运行中未来访问序列无法预知，因此仅是理论算法

    int iPageCount = 0;
    int iEliTime = 0;
    PageNum* onePage = headOriPage;
    PageNum* oneBPage = NULL;
    PageNum* thePage = NULL;
    //块链表首个节点
    headBlockPage = (PageNum*)malloc(sizeof(PageNum));
    memcpy(headBlockPage, onePage, sizeof(PageNum));
    rearBlockPage = headBlockPage;
    headBlockPage->next = NULL;
    iPageCount = 1;//块链表节点初始节点为１个
    onePage = onePage->next;
    /*  begin ************************************************************** */
    while (onePage != NULL)
    {
        if (CheckPageInBlock(onePage))
        {
            // hit, do nothing
        }
        else
        {
            if (iPageCount < BlockNum)
            {
                thePage = (PageNum*)malloc(sizeof(PageNum));
                memcpy(thePage, onePage, sizeof(PageNum));
                thePage->next = NULL;
                rearBlockPage->next = thePage;
                rearBlockPage = thePage;
                iPageCount++;
            }
            else
            {
                PageNum* bp = headBlockPage;
                PageNum* prevBp = NULL;
                PageNum* victim = NULL;
                PageNum* prevVictim = NULL;
                int maxDist = -1;
                while (bp != NULL)
                {
                    int dist = 9999;
                    PageNum* future = onePage->next;
                    int d = 1;
                    while (future != NULL)
                    {
                        if (future->iNum == bp->iNum)
                        {
                            dist = d;
                            break;
                        }
                        future = future->next;
                        d++;
                    }
                    if (dist > maxDist)
                    {
                        maxDist = dist;
                        victim = bp;
                        prevVictim = prevBp;
                    }
                    prevBp = bp;
                    bp = bp->next;
                }
                if (prevVictim == NULL)
                    headBlockPage = victim->next;
                else
                    prevVictim->next = victim->next;
                if (victim == rearBlockPage)
                    rearBlockPage = prevVictim;
                free(victim);
                iEliTime++;

                thePage = (PageNum*)malloc(sizeof(PageNum));
                memcpy(thePage, onePage, sizeof(PageNum));
                thePage->next = NULL;
                rearBlockPage->next = thePage;
                rearBlockPage = thePage;
            }
        }
        onePage = onePage->next;
    }
　　/*   end  ************************************************************** */

    return iEliTime;
}
int FIFO()
{ //先进先出算法
    int iPageCount = 0;
    int iEliTime = 0;
    PageNum* onePage = headOriPage;
    PageNum* oneBPage = NULL;
    PageNum* thePage = NULL;
    //块链表首个节点
    headBlockPage = (PageNum*)malloc(sizeof(PageNum));
    memcpy(headBlockPage, onePage, sizeof(PageNum));
    rearBlockPage = headBlockPage;
    headBlockPage->next = NULL;
    iPageCount = 1;//块链表节点初始节点为１个
    onePage = onePage->next;
    /*  begin ************************************************************** */
    while (onePage != NULL)
    {
        if (!CheckPageInBlock(onePage))
        {
            thePage = (PageNum*)malloc(sizeof(PageNum));
            memcpy(thePage, onePage, sizeof(PageNum));
            thePage->next = NULL;
            if (iPageCount < BlockNum)
            {
                rearBlockPage->next = thePage;
                rearBlockPage = thePage;
                iPageCount++;
            }
            else
            {
                PageNum* temp = headBlockPage;
                headBlockPage = headBlockPage->next;
                free(temp);
                rearBlockPage->next = thePage;
                rearBlockPage = thePage;
                iEliTime++;
            }
        }
        onePage = onePage->next;
    }
　　/*   end  ************************************************************** */
    return iEliTime;
}

int main()
{
    headOriPage = InitPageSeq();
    printf("LRU:%d,FIFO:%d,OPT:%d", LRU(), FIFO(), OPT());
    ClearPageSeq(headOriPage);
    return 0;
}
