//
// Created by zjp on 18-12-26.
//
/**
 * 败者树
 */
#ifndef PARALLELPROGRAM_LOSERTREE_H
#define PARALLELPROGRAM_LOSERTREE_H

#include <assert.h>

#define MIN -1   //所有数据的可能最小值

class LoserTree{
private:
    int len;
    int *ls; //存放败者树中间节点
    int *buf; //存放叶子节点

    void adjust(int s){ //s是需要调整的buf的下标
        int t = (s+len)/2; //得到s在败者树上面的父节点
        while(t>0){
            if(buf[s]>buf[ls[t]]){ //当前节点大于父节点，小为胜利，记录败者，记录较大数的下标
                int temp = ls[t];
                ls[t] = s;
                s = temp;  //s记录当前胜者，继续下一轮比较
            }
            t/=2;
        }
        ls[0] = s;  //最终胜者
    }
    void build(){
        buf[len] = MIN; //最后一位放MIN
        for(int i=0;i<len;i++)
            ls[i] = len; //所有败者树初始化为MIN的下标

        for(int i=len-1;i>=0;i--)
            adjust(i);
    }

public:
    LoserTree(int length,int data[]):len(length){
        ls = new int[len+1]; //多出来一位用老存放MIN
        buf = new int[len+1]; //多1用来存放最终的胜者
        for(int i=0;i<len;i++)
            buf[i] = data[i];
        build();
    }
    ~LoserTree(){
        delete []ls;
        delete []buf;
    }
    int getLen() const{
        return len;
    }

    void setData(int index,int value){
        if(buf[index]!=value) {  ///如果相同，重复adjust会出现错误结果?????????????
            assert(index < len);
            assert(value >= 0);
            buf[index] = value;
            adjust(index);
        }
    }
    /**
     * 返回最小值下标和最小值
     * @return
     */
    int* getMin() const{
        int *map = new int[2];
        map[0] = ls[0];
        map[1] = buf[ls[0]];
        return map;
    }
};

#endif //PARALLELPROGRAM_LOSERTREE_H
