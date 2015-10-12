#include <iostream>
#include <string>
#include <cctype>
#include <functional>
#include <algorithm>
#include <stack>
#include <deque>
#include <vector>


using namespace std;


int leafCount = 0;
int degree2Count = 0;

template <class DataType>
struct TreeNode
{
	TreeNode(DataType d) : data(d), lchild(NULL), rchild(NULL) {}
	DataType data;
	TreeNode *lchild, *rchild;
};

void trim( string &s )
{
	string::iterator it = s.end()-1;
	while( it >= s.begin() && isspace(*it) )
		*it-- = 0;
}


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
		if( p->lchild && p->rchild )
			++degree2Count;
		else if( !p->lchild && !p->rchild )
			++leafCount;
		s.pop();				//!!出栈一定要在访问数据之后
		if(p->rchild) s.push(p->rchild);	//!!不能颠倒
		if(p->lchild) s.push(p->lchild);
	}
}


template <class DataType>
void postorder_nonrecur(TreeNode<DataType> *p, vector<TreeNode<DataType>*> &path)
{
	deque<TreeNode<DataType>*> s1;
	stack<short> s2;
	int maxPath = 0;
	
	do{
		while(p)
			s1.push_back(p), s2.push(0), p=p->lchild;
		if(!s1.empty())
		{
			if(s2.top()==0) s2.top()=1, p=s1.back()->rchild;
			else {
				if( s1.size() > maxPath ) {
					maxPath = s1.size();
					path.assign(s1.begin(), s1.end());
				}
				s1.pop_back(); s2.pop();
			}
		}
	}while(p || !s1.empty());
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


bool fail = false;
template <class Iter, class DataType>
void create_tree(Iter postorder, Iter inorder, int n, TreeNode<DataType> *&t)
{
	if( fail ) return;
	if(n==0) { t=NULL; return; }
	t=new TreeNode<DataType>(*(postorder+n-1));
	Iter it=find(inorder, inorder+n, t->data);
	if( inorder+n == it ) {
		fail = true;
		return;
	}
	int k=it-inorder;
	create_tree(postorder, inorder, k, t->lchild);
	create_tree(postorder+k, inorder+1+k, n-k-1, t->rchild);
}


int main()
{
	string in, post;

	cout << "Input the in-order sequence of the tree:" << endl;
	getline(cin, in);
	cout << "Input the post-order sequence of the tree:" << endl;
	getline(cin, post);

	trim(in);
	trim(post);

	if( in.length() != post.length() || in.length() == 0 ) {
		cerr << "Invalid input!" << endl;
		exit(-1);
	}

	TreeNode<char> *root;
	create_tree(post.begin(), in.begin(), in.length(), root);
	if( fail ) {
		cerr << "Invalid input!" << endl;
		exit(-1);
	}
	cout << "The pre-order sequence of the tree is:" << endl;
	preorder_nonrecur( root );
	cout << endl;
	cout << "There are " << leafCount << " leaf nodes and " << degree2Count << " two-degree nodes in this tree." << endl;

	vector<TreeNode<char>*> path;
	postorder_nonrecur( root, path );
	cout << "The longest path from root to leaf in this tree is:" << endl;
	for( vector<TreeNode<char>*>::iterator it = path.begin(); it != path.end(); ++it )
		cout << (*it)->data << " ";
	cout << endl;

	destroy(root);
	return 0;
}


// test data
/*
dbgehacif
dghebifca
*/
