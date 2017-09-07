#include"iostream"
#include"fstream"
#include"vector"
#include"windows.h"
using namespace std;

vector<int> have[500];   //ÿ�������Ѿ��е���Դ
vector<int> want[500];   //����������Ҫ����Դ
int pro_size;            //������
int res_size;            //��Դ������
int res_max[500];        //ÿ����Դ�ĸ���
int res_available[500];  //��ǰ������Դ��
vector<int> Deadlock;
bool* visited;
bool* Dead_res;

void DeadlockChain(int beg);
void read_data();
void predigest();

int main()
{
    read_data();
    predigest();
    return 0;
}

//�ļ���ʽ��
void read_data()
{
    cout<<"��ʼ��ȡ��Դ�����ļ�......"<<endl;

    ifstream in;
    in.open("C://Users//86739//Desktop//data.txt");
    if(!in)
        cout << "FILE OPEN FAILED!" << endl;
    memset(res_max,0,500);
    memset(res_available,0,500);
//**********************************************
//Read res
    in >> res_size; //��ȡ��Դ�����ʽ����
    if(res_size > 500)    //�����Դ����500�����˳�
    {
        cout << "Number of res should be small than 500" << endl;
        exit(-1);
    }

    for(int i = 0; i < res_size; i++) //��ȡÿ����Դ����Ŀ
    {
        int temp;
        in >> temp;
        if(temp < 0)
        {
            cout << "Illegal No. of res!" << endl;
            cout <<"Please check your input data!" << endl;
            exit(-1);
        }
        res_max[i] = res_available[i] = temp;//��ʼ��Դ��������Ϳ���������ͬ
    }
//************************************************
//Read pro
    in >> pro_size;      //��ȡ��������
    if(pro_size > 500)   //������̴���500�����˳�
    {
        cout << "Number of pro should be small than 500" << endl;
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
                cout << "illegal res No.!" << endl;
                cout <<"Please check your input data!" << endl;
                exit(-1);
            }
            res_available[temp]--;

            if(res_available[temp] < 0)
            {
                cout << "Impossible res dispatch situation!" << endl;
                cout <<"Please check your input data!" << endl;
                exit(-1);
            }

            have[num].push_back(temp);
        }
//--------------------------------------------------------
        int want_size;
        in >> want_size;

        for(int j = 0; j < want_size; j++)
        {
            int temp;
            in >> temp;
            if(temp >= res_size)
            {
                cout << "illegal res No.!" << endl;
                cout <<"Please check your input data!" << endl;
                exit(-1);
            }
            want[num].push_back(temp);
        }
    }

    cout<<"��ȡ�ļ�����......"<<endl;
}


void predigest()
{
    cout<<"begin simplify resource allocation graph......"<<endl;
    //  Draw();
    bool finish[500];           //��¼�����Ƿ����

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
                res_available[ have[index][i] ]++;//��ʱ�Ϳ��Ի�����Դ
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
                    }//���Է��䣿
                }

                if(ok)//�������i�����������Դ����С��ϵͳ�ܹ��ṩ����Ը��Ŀ����ɾ���ý���������Դ��
                {
                    //       ers(i);
                    for(int t = 0; t < have[i].size(); t++)
                    {
                        res_available[have[i][t]]++;//�����ͷ�֮ǰ�����������Դ
                    }        //������Դ
                    cout<<"process "<<i<<" all resuorce edge have deleted"<<endl;
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
        cout << "DeadLock has happened!" << endl << endl;
        cout << "The processes & resources related to the deadlock is: " << endl;
        visited = new bool[Deadlock.size()];             //use to id wheather the pro is visited DFS
        memset(visited,0,Deadlock.size());
        Dead_res = new bool[res_size];

        for(int i = 0; i < Deadlock.size(); i++)
        {
            if(!visited[i])
            {
                memset(Dead_res,0,res_size);
                DeadlockChain(i);
                cout << "Resource: ";
                for(int j = 0; j < res_size; j++)
                {
                    if(Dead_res[j])
                    {
                        cout << j << " ";
                    }

                }

            }
            cout << endl;
        }
    }
    else{
        cout << "All process had execued!" << endl;
    }
}

void DeadlockChain(int beg)
{
    visited[beg] = true;
    cout << "Process:" << beg << endl;

    for(int i = 0; i < want[beg].size(); i++){
        for(int j = 0; j < Deadlock.size(); j++){
           for(int k = 0; k < have[ Deadlock[j] ].size(); k++){
               if(have[ Deadlock[j] ][k] == want[beg][i])//�������jӵ�е���Դ�ͽ���i�������Դ�����ȣ���������ҽ���j��Դ���
                {
                    Dead_res[want[beg][i]] = true;//��want[beg][i]��ʾ����ԴΪ�����������Դ
                    if(!visited[j]){
                        DeadlockChain(j);
                    }

                }
           }

        }

    }

    for(int t = 0; t < have[beg].size(); t++){
        for(int j = 0; j < Deadlock.size(); j++){
            for(int k = 0; k < want[ Deadlock[j] ].size(); k++){
               if(want[ Deadlock[j] ][k] == have[beg][t]){//�������j�������Դ�ͽ���iӵ�е���Դ�����ȣ���������ҽ���j��Դ���
                    Dead_res[have[beg][t]] = true;//��have[beg][t]��ʾ����ԴΪ�����������Դ
                    if(!visited[j]){
                        DeadlockChain(j);
                    }

                }
            }

        }

    }

}
