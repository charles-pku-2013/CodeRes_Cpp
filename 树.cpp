#include <iostream>
#include <string>
#include <functional>
#include <algorithm>
#include <ctime>
#include <stack>
#include <queue>

using namespace std;

template <class DataType>
struct TreeNode
{
    TreeNode(const DataType &d) : data(d), lchild(NULL), rchild(NULL) {}
    DataType data;
    TreeNode *lchild, *rchild;
};

template <class DataType>
bool search(TreeNode<DataType> *t, const DataType &key)
{
    if(t==NULL) return false;
    else if(key==t->data) return true;
    else if(key<t->data) return search(t->lchild, key);
    else return search(t->rchild, key);
}

template <class DataType>
TreeNode<DataType> *insert_node(TreeNode<DataType> *t, DataType key)
{
    if(t==NULL) return new TreeNode<int>(key);
    if(key<t->data) t->lchild=insert_node(t->lchild, key);
    else if(key>t->data) t->rchild=insert_node(t->rchild, key);
    return t;
}

template <class DataType>
void inorder(TreeNode<DataType> *t)
{
    if(t)
    {
        inorder(t->lchild);
        cout<<t->data<<" ";
        inorder(t->rchild);
    }
}

//非递归先序遍历
template <class DataType>
void preorder_nonrecur(TreeNode<DataType> *t)
{
    stack<TreeNode<DataType>*> s;
    TreeNode<DataType> *p;
    if(t) s.push(t);
    while(!s.empty())
    {
        p=s.top();
        cout<<p->data<<" ";
        s.pop();                //!!出栈一定要在访问数据之后
        if(p->rchild) s.push(p->rchild);    //!!不能颠倒
        if(p->lchild) s.push(p->lchild);
    }
}

//中序非递归
template <class DataType>
void inorder_nonrecur(TreeNode<DataType> *p)
{
    stack<TreeNode<DataType>*> s;
    while(p || !s.empty())
    {
        while(p)
            s.push(p), p=p->lchild;
        if(!s.empty())
        {
            p=s.top();
            cout<<p->data<<" ";
            s.pop();        //!!出栈一定要在访问数据之后
            p=p->rchild;
        }
    }
}

template <class DataType>
void inorder_nonrecur1(TreeNode<DataType> *t)
{
    stack<TreeNode<DataType>*> s;
    TreeNode<DataType> *p;
    for(p=t; p; p=p->lchild)
        s.push(p);
    while(!s.empty())
    {
        p=s.top();
        cout<<p->data<<" ";
        s.pop();
        for(p=p->rchild; p; p=p->lchild)
            s.push(p);
    }
}

template <class DataType>
void postorder_nonrecur(TreeNode<DataType> *p)
{
    stack<TreeNode<DataType>*> s1;
    stack<short> s2;
    do{
        while(p)
            s1.push(p), s2.push(0), p=p->lchild;
        if(!s1.empty())
        {
            if(s2.top()==0) s2.top()=1, p=s1.top()->rchild;
            else {
                cout<<s1.top()->data<<" ";
                s1.pop(); s2.pop();
            }
        }
    }while(p || !s1.empty());
}

//习题集6.38
template <class DataType>
void postorder_nonrecur1(TreeNode<DataType> *p)
{
    stack<TreeNode<DataType>*> s1;
    stack<short> s2;
    if(p) s1.push(p), s2.push(0);
    while(!s1.empty())
    {
        if(s2.top()==0) //未走过
        {
            s2.top()=1;
            if(s1.top()->lchild)
                s1.push(s1.top()->lchild), s2.push(0);
        }
        else if(s2.top()==1)    //从左边回来，再走右边
        {
            s2.top()=2;
            if(s1.top()->rchild)
                s1.push(s1.top()->rchild), s2.push(0);
        }
        else    //左右都走完了
        {
            cout<<s1.top()->data<<" ";
            s1.pop(), s2.pop();
        }
    }
}

template <class DataType>
void level_order(TreeNode<DataType> *t)
{
    queue<TreeNode<DataType>*> q;
    TreeNode<DataType> *p;
    if(t) q.push(t);
    while(!q.empty())
    {
        p=q.front();    //!!注意是front不是back
        cout<<p->data<<" ";
        q.pop();
        if(p->lchild) q.push(p->lchild);
        if(p->rchild) q.push(p->rchild);
    }
}

//求各层宽度
template <class DataType>
void width( TreeNode<DataType> *p, int *w, int level)
{
    if(!p) return;
    ++w[level];
    width(p->lchild, w, level+1);
    width(p->rchild, w, level+1);
}

//计算繁茂度 习题集6.52
template <class T>
int fanmao( TreeNode<T> *t )
{
    if(!t) return 0;
    queue< TreeNode<T>* > q;
    vector<int> n_of_nodes;
    int c=1; n_of_nodes.push_back(c);
    TreeNode<T> *p;
    q.push(t);
    while( !q.empty() )     //非递归求各层节点数目
    {
        p=q.front();
        q.pop();
        if(p->lchild) q.push(p->lchild);
        if(p->rchild) q.push(p->rchild);
        if(--c==0) { c=q.size(); n_of_nodes.push_back(c);}      //新的一层}
    }//while

    int degree=0;
    for(int i=1, n=n_of_nodes.size(); i<=n; ++i)
        degree+=i*n_of_nodes[i-1];
    return degree;
}

template <class DataType>
void destroy(TreeNode<DataType> *t)
{
    if(t)
    {
        destroy(t->lchild);
        destroy(t->rchild);
        delete t;
    }
}

template <class DataType>
int count_nodes(TreeNode<DataType> *t)
{
    if(t==NULL) return 0;
    else return 1+count_nodes(t->lchild)+count_nodes(t->rchild);
}

template <class DataType>
int height(TreeNode<DataType> *t)
{
    if(t==NULL) return 0;
    else return 1+max(height(t->lchild), height(t->rchild));
}

//根据先序和中序建立二叉树
//template <class Iter, class DataType>
//TreeNode<DataType> *create_tree(Iter preorder_beg, Iter preorder_end,
//                              Iter inorder_beg, Iter inorder_end, const DataType &c)
//{
//  if(preorder_beg>=preorder_end) return NULL;
//  TreeNode<DataType> *t=new TreeNode<DataType>(*preorder_beg);
//  Iter pos=find(inorder_beg, inorder_end, *preorder_beg);
//  int k=pos-inorder_beg;
//  t->lchild=create_tree(preorder_beg+1, preorder_beg+1+(pos-inorder_beg),
//                          inorder_beg, pos, c);
//  t->rchild=create_tree(preorder_beg+1+(pos-inorder_beg), preorder_end,
//                          pos+1, inorder_end, c);
//  return t;
//}

//照顾以前的数组习惯
template <class Iter, class DataType>
void create_tree(Iter preorder, Iter inorder, int n, TreeNode<DataType> *&t)
{
    if(n==0) { t=NULL; return; }
    t=new TreeNode<DataType>(*preorder);
    Iter it=find(inorder, inorder+n, *preorder);
    int k=it-inorder;
    create_tree(preorder+1, inorder, k, t->lchild);
    create_tree(preorder+1+k, inorder+1+k, n-k-1, t->rchild);
}

//根据广义表建立二叉树
TreeNode<char> *create_tree(const string &s)
{
    stack<TreeNode<char>*> stk;
    short branch=0;
    TreeNode<char> *root=NULL, *p=NULL;
    for(string::const_iterator it=s.begin(); it!=s.end(); ++it)
    {
        switch(*it)
        {
        case '(':
            stk.push(p); branch=0; break;
        case ',':
            branch=1; break;
        case ')':
            stk.pop(); branch=0; break;
        default:
            p=new TreeNode<char>(*it);
            if(!root) root=p;
            if(!stk.empty())
                if(branch==0) stk.top()->lchild=p;
                else stk.top()->rchild=p;
        }
    }
    return root;
}

//判断是否是排序树 习题集9.31
template <class DataType>
bool is_BSTree(TreeNode<DataType> *p)
{
    if(!p) return true;
    if(p->lchild && p->lchild->data>p->data) return false;
    if(p->rchild && p->rchild->data<p->data) return false;
    return is_BSTree(p->lchild) && is_BSTree(p->rchild);
}

//!!这样的递归是绝对错误的!!!处理左边找不到情况而不是返回
//template <class DataType>
//pair< TreeNode<DataType>*, TreeNode<DataType>* >
//equal_range(TreeNode<DataType> *t, const DataType &key)
//{
//  static TreeNode<DataType> *pre=NULL;
//  if(t)
//  {
//      if(t->lchild) return equal_range(t->lchild, key);
//      if(pre && pre->data<key && t->data>=key)
//          return make_pair(pre, t);
//      pre=t;
//      if(t->rchild) return equal_range(t->rchild, key);
//  }
//}

//习题集p44 6.69
template <class T>
void print_tree(TreeNode<T> *p, int level)
{
    if(p)
    {
        print_tree(p->rchild, level+1);
        for(int i=0; i<level; ++i)
            cout<<'\t';
        cout<<p->data<<endl;
        print_tree(p->lchild, level+1);
    }
}

int main()
{
    //TreeNode<int> *root=NULL;
    //srand((unsigned)time(NULL));
    //int n=10;
    //for(int i=0; i<n; ++i)
    //  root=insert_node(root, rand()%100+1);

    ////string s("4213657"), t("1234567");
    //string s("abcdefghij"), t("bcdafehjig");
    //TreeNode<char> *root;
    //create_tree(s.begin(), t.begin(), s.length(), root);
    ////TreeNode<char> *root=create_tree(s.begin(), s.end(), t.begin(), t.end(), string::value_type());

    //int a[]={8, 4, 2, 6, 10, 12};
    //int b[]={2, 4, 6, 8, 10, 12};
    //TreeNode<int> *root=create_tree(a, a+sizeof(a)/sizeof(*a), 
    //  b, b+sizeof(b)/sizeof(*b), 0);

    //string s("a(b(c,d),e(,f))");
    string s("A(B(,D),C(E(,F))");
    TreeNode<char> *root=create_tree(s);

    print_tree(root, 0);
    cout<<endl;

    cout<<"inorder:"<<endl;
    inorder(root);
    cout<<endl;
    //cout<<"inorder_nonrecur:"<<endl;
    //inorder_nonrecur(root);
    //cout<<endl;
    //cout<<"inorder_nonrecur1:"<<endl;
    //inorder_nonrecur1(root);
    //cout<<endl;

    //cout<<"preorder_nonrecur:"<<endl;
    //preorder_nonrecur(root);
    //cout<<endl;
    //cout<<"postorder_nonrecur:"<<endl;
    //postorder_nonrecur(root);
    //cout<<endl;

    //cout<<"postorder_nonrecur1:"<<endl;
    //postorder_nonrecur1(root);
    //cout<<endl;

    //cout<<"level_order:"<<endl;
    //level_order(root);
    //cout<<endl;

    //cout<<"count_nodes:"<<count_nodes(root)<<endl;
    //int ht=height(root);
    //cout<<"height:"<<ht<<endl;

    ////cout<<"is_BSTree:"<<is_BSTree(root)<<endl;

    //cout<<"width in each level:"<<endl;
    //vector<int> w(ht, 0);
    //width(root, &w[0], 0);
    //for(vector<int>::iterator it=w.begin(); it!=w.end(); ++it)
    //  cout<<it-w.begin()<<":"<<*it<<endl;

    int fm=fanmao(root);
    cout<<"繁茂度："<<fm<<endl;

    destroy(root);
    return 0;
}
