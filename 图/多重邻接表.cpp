#include <iostream>
#include<fstream>
#include <streambuf>
#include<queue>
typedef char vex_type;
struct G_node
{
    int head, tail, info; //info权重
    G_node* h_link, * t_link;//h——link指向与左边顶点同顶点的其他边，t_link指向与右边边顶点同顶点的其他边
    G_node() : head(0), tail(0), info(0), h_link(nullptr), t_link(nullptr) {};
};
struct G_list
{
    vex_type data;
    G_node* out; //无向图，一个指向边的指针
    G_list() : data(0), out(nullptr) {};
};

class Graph
{
public:
    int vex_num, arc_num, * visited;
    std::queue<G_list> q;
    G_list* vexs;
    Graph();
    ~Graph();
    int get_num(vex_type& v);//通过数组名称返回节点的下标
    vex_type get_name(int index);//通过数组下标返回节点的名称
    void insert(int head, int tail, int info);
    void create();
    void show();
    void dfs(int index = 0);
    void bfs(int index = 0);
    void clean_visited() { for (int i = 0; i < vex_num; ++i) visited[i] = 0; };
};

Graph::Graph()
{

    std::cout << "请输入节点的个数和边的个数" << std::endl;
    std::cin >> vex_num >> arc_num;
    vexs = new G_list[vex_num];
    visited = new int[vex_num];
    for (int i = 0; i < vex_num; ++i)
        visited[i] = 0;
    for (int i = 0; i < vex_num; ++i)
    {
        std::cout << "请输入所有" << vex_num << "个节点的名称:";
        std::cin >> vexs[i].data;
    }
}

Graph::~Graph()
{
    delete[] vexs;
}

int Graph::get_num(vex_type& v)
{
    int i;
    for (i = 0; i < vex_num; ++i)
    {
        if (vexs[i].data == v)
            break;
    }
    return i;
}
vex_type Graph::get_name(int index)
{
    return vexs[index].data;
}
void Graph::insert(int head, int tail, int info) //给定下标，头插法
{
    G_node* p = new G_node();
    p->head = head;
    p->tail = tail;
    p->info = info;
    //头插进入出结点
    p->h_link = vexs[head].out;
    vexs[head].out = p;
    //头插进入入节点
    p->t_link = vexs[tail].out;
    vexs[tail].out = p;

}
void Graph::create()
{
    for (int i = 0; i < arc_num; ++i)
    {
        std::cout << "请输入两个节点名称及权重：" << std::endl;
        vex_type a, b;
        int weight;
        std::cin >> a >> b >> weight;
        a = get_num(a);
        b = get_num(b);
        insert(a, b, weight);
    }
}
void Graph::show()
{
    for (int i = 0; i < vex_num; ++i)
    {
        std::cout << "第" << i + 1 << "个节点边有：";
        for (G_node* p = vexs[i].out; p != nullptr;)
        {
            std::cout << get_name(p->head) << "->" << get_name(p->tail) << "  ";
            if (p->head == i)
                p = p->h_link;
            else
                p = p->t_link;
        }
        std::cout << "\n";


    }

}
void Graph::dfs(int index)
{
    std::cout << get_name(index) << std::endl;
    visited[index] = 1;
    G_node* p = vexs[index].out;
    while (p)//递归访问每个节点的第一条边
    {
        int index1 = (p->head == index) ? p->tail : p->head;//因为是无向图，不能直接使用tail和head,需要判断该节点对应边中的哪个节点，比如1号节点第一条边是 (0,1,h_link,t_link) （重复利用了0号节点的边）
        if (!visited[index1])								//则1号节点的下一条边是t_link指向的边，而0号节点的第一条边是h_link指向的边，也就是说要先判断当前顶点对应边中的哪个点，然后才能确定使用哪个指针
            dfs(index1);
        p = (p->head == index) ? p->h_link :p->t_link;
    }



}
void Graph::bfs(int index)
{
    if (!visited[index])
    {
        visited[index] = 1;
        q.push(vexs[index]);
        while (!q.empty())
        {
            G_node* tmp = q.front().out;
            int num = get_num(q.front().data);//获取节点在数组中的下标
            std::cout << q.front().data << std::endl;
            while (tmp)
            {
                int index1 = (tmp->head == num) ? tmp->tail : tmp->head;
                if (!visited[index1])
                {
                    q.push(vexs[index1]);
                    visited[index1] = 1;
                }
                    
                tmp = (index1 == tmp->head) ? tmp->t_link : tmp->h_link;

            }
            q.pop();

        }
    }

}

int main()
{
	//这里使用了C++的cin/cout重定向，直接读取文件并输出到文件，如果你不想使用，将下面的部分和结尾部分注释即可
     std::ifstream fin("多重邻接表测试.txt");//重定向输入为文件
     std::ofstream fout("多重邻接表测试__out.txt");//重定向输出
     std::streambuf *cin_backup = std::cin.rdbuf(fin.rdbuf());//cin.rdbuf(&)带参数返回原来的buffer,并备份
     std::streambuf *cout_backup = std::cout.rdbuf(fout.rdbuf());


	 //主程序
    Graph g;
    g.create();
    std::cout<<"--------------------------初始视图--------------------"<<std::endl;
    g.show();


    std::cout<<"--------------------------bfs--------------------"<<std::endl;
    g.clean_visited();
    g.bfs();

    std::cout<<"--------------------------dfs--------------------"<<std::endl;

    g.clean_visited();
    g.dfs();


	//主程序



	//恢复重定向
     std::cin.rdbuf(cin_backup);//不需要读取时，使用备份恢复cin cout到标准输入输出中
     std::cout.rdbuf(cout_backup);
	 //关闭文件读写
     fin.close();
     fout.close();
}
