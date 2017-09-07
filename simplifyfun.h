#ifndef SIMPLIFYFUN_H
#define SIMPLIFYFUN_H

#include<vector>
#include<string>
#include"fstream"
#include<sstream>
using namespace std;

void DeadlockChain(int beg,vector<string> &DeadlockChain_string, vector<int> *have, vector<int> *want, vector<int> Deadlock,bool *visited, bool *Dead_res)
{
    visited[beg] = true;

    stringstream ssTemp;
    ssTemp<<beg;

    DeadlockChain_string.push_back("Process:" + ssTemp.str());

    for(int i = 0; i < want[beg].size(); i++){
        for(int j = 0; j < Deadlock.size(); j++){
           for(int k = 0; k < have[ Deadlock[j] ].size(); k++){
               if(have[ Deadlock[j] ][k] == want[beg][i])//如果进程j拥有的资源和进程i请求的资源类别相等，则继续查找进程j资源情况
                {
                    Dead_res[want[beg][i]] = true;//置want[beg][i]表示的资源为造成死锁的资源
                    if(!visited[j]){
                        DeadlockChain(j,DeadlockChain_string,have,want, Deadlock, visited, Dead_res);

                    }

                }
           }

        }

    }

    for(int t = 0; t < have[beg].size(); t++){
        for(int j = 0; j < Deadlock.size(); j++){
            for(int k = 0; k < want[ Deadlock[j] ].size(); k++){
               if(want[ Deadlock[j] ][k] == have[beg][t]){//如果进程j请求的资源和进程i拥有的资源类别相等，则继续查找进程j资源情况
                    Dead_res[have[beg][t]] = true;//置have[beg][t]表示的资源为造成死锁的资源
                    if(!visited[j]){
                        DeadlockChain(j,DeadlockChain_string,have,want, Deadlock, visited, Dead_res);

                    }

                }
            }

        }

    }

}


//文件格式，
vector<string> simplify(string fileName)
{
    vector<int> have[500];   //每个进程已经有的资源
    vector<int> want[500];   //各个进程想要的资源
    int pro_size;            //进程数
    int res_size;            //资源种类数
    int res_max[500];        //每类资源的个数
    int res_available[500];  //当前可用资源数

    vector<int> Deadlock;

    bool* visited;
    bool* Dead_res;



    vector<string> simplify_msg;
    simplify_msg.push_back("begin read resource allocation graph file......");
    ifstream in;
    in.open(fileName);
    if(!in){
        simplify_msg.push_back("FILE OPEN FAILED!");
    }
    memset(res_max,0,500);
    memset(res_available,0,500);

    in >> res_size; //读取资源种类格式个数
    if(res_size > 500)    //如果资源大于500，则退出
    {
        simplify_msg.push_back("Number of res should be small than 500");
        exit(-1);
    }

    for(int i = 0; i < res_size; i++) //读取每类资源的数目
    {
        int temp;
        in >> temp;
        if(temp < 0)
        {
            simplify_msg.push_back("Illegal No. of res!");
            simplify_msg.push_back("Please check your input data!");
            exit(-1);
        }
        res_max[i] = res_available[i] = temp;//初始资源最大数量和可用数量相同
    }

    in >> pro_size;      //读取进程数量
    if(pro_size > 500)   //如果进程大于500，则退出
    {
       simplify_msg.push_back("Number of pro should be small than 500");
        exit(-1);
    }

    for(int k = 0; k < pro_size; k++)
    {

        int num;
        in>> num;

        int have_size;
        in >> have_size;
        for(int i = 0; i < have_size; i++)
        {
            int temp;
            in >> temp;

            if(temp >= res_size)
            {
                simplify_msg.push_back("illegal res No.!");
                simplify_msg.push_back("Please check your input data!");
                exit(-1);
            }
            res_available[temp]--;

            if(res_available[temp] < 0)
            {
                simplify_msg.push_back("Impossible res dispatch situation!");
                simplify_msg.push_back("Please check your input data!");
                exit(-1);
            }

            have[num].push_back(temp);
        }

        int want_size;
        in >> want_size;

        for(int j = 0; j < want_size; j++)
        {
            int temp;
            in >> temp;
            if(temp >= res_size)
            {
               simplify_msg.push_back("illegal res No.!" );
               simplify_msg.push_back("Please check your input data!");
                exit(-1);
            }
            want[num].push_back(temp);
        }
    }

    simplify_msg.push_back("read file end......");

    simplify_msg.push_back("begin simplify resource allocation graph......");
    //  Draw();
    bool finish[500];           //记录进程是否完成

    for(int index = 0; index < pro_size; index++)
    {
        if(want[index].size() != 0)
        {
            finish[index] = false;
        }
        else
        {
            finish[index] = true;
            //ers(index);
            for(int i = 0; i < have[index].size(); i++)
                res_available[ have[index][i] ]++;//此时就可以回收资源
        }
    }

    bool flag;
    do{
        flag = false;
        for(int i = 0; i < pro_size; i++)
        {
            if(!finish[i])
            {
                int temp[500];
                memset(temp,0,500);

                for(int k = 0; k < want[i].size(); k++)
                {
                    temp[want[i][k]]++;
                }

                bool ok = true;
                for(int j = 0; j < res_size; j++)
                {
                    if(temp[j] > res_available[j])
                    {
                        ok = false;
                        break;
                    }//可以分配？
                }

                if(ok)//如果进程i请求的所有资源数量小于系统能够提供的自愿数目，则删除该进程所有资源边
                {
                    //       ers(i);
                    for(int t = 0; t < have[i].size(); t++)
                    {
                        res_available[have[i][t]]++;//进程释放之前请求的所有资源
                    }        //回收资源

                    stringstream ssTemp;
                    ssTemp<<i;
                    simplify_msg.push_back("process " + ssTemp.str() + " all resuorce edges have deleted");
                    have[i].clear();
                    finish[i] = true;
                    flag = true;
                }
            }
        }

    }
    while(flag);

    for(int i = 0; i < pro_size; i++)
    {
        if(finish[i] == false)
        {
            Deadlock.push_back(i);
        }

    }


    if(Deadlock.size() != 0)
    {
        simplify_msg.push_back("DeadLock has happened!");
        simplify_msg.push_back("The processes & resources related to the deadlock is: ");
        visited = new bool[Deadlock.size()];             //use to id wheather the pro is visited DFS
        memset(visited,0,Deadlock.size());
        Dead_res = new bool[res_size];

        for(int i = 0; i < Deadlock.size(); i++)
        {
            if(!visited[i]) {
                vector<string> DeadlockChain_string;
                memset(Dead_res,0,res_size);
                DeadlockChain(i,DeadlockChain_string,have,want, Deadlock, visited, Dead_res);

                simplify_msg.insert(simplify_msg.end(), DeadlockChain_string.begin(),DeadlockChain_string.end());
                simplify_msg.push_back("Resource: ");

                string resDeadlockTemp = "";
                for(int j = 0; j < res_size; j++)
                {
                    stringstream ssTemp;
                    ssTemp<<j;
                    if(Dead_res[j])
                    {
                       resDeadlockTemp += ssTemp.str() + " ";
                    }

                }
                simplify_msg.push_back(resDeadlockTemp);

            }
        }
    }
    else{
         simplify_msg.push_back("All process had execued!");
    }
    return simplify_msg;
}

#endif // SIMPLIFYFUN_H
