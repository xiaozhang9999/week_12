using namespace std;
#include<iostream>
#include<vector>
#include<string>
template <typename T>
class SegmentTree 
{
public:
	SegmentTree(T arr[], int length); 
	int getSize() { return data.size(); }
	T get(int index);//�����������Ӧ������Ӧ��Ԫ��
	void buildSegmentTree(int Treeindex, int l, int r);//��Treeindex����������[l...r]���߶���
	T Query(int queryL, int queryR);//�����ѯ
	void set(int index, T e);//����index��Ԫ��Ϊe
	string tostring();//�ַ�����ʽ���
private:
	vector<T> data;
	vector<T> tree;

	int leftchild(int index) { return index * 2 + 1; }//���ӵ�����
	int rightchild(int index) { return index * 2 + 2; }//�Һ��ӵ�����

	T merge(T left, T right);//�ϲ������������˴������Ϊ�������Ը���ʵ������޸Ĵ˺�����ִ�в�ͬ����
	T query(int Treeindex, int l, int r, int queryL, int queryR);
	void set(int Treeindex, int l, int r,int index, T e);
};

template<typename T>
SegmentTree<T>::SegmentTree(T arr[], int length) {
	data.resize(length,0);//Ϊ����Ԥ��n�Ŀռ�
	for (int i = 0; i < length; i++) {
		data[i]=arr[i];
	}
	tree.resize(4 * length,0);//Ϊ��Ԥ��4*n�Ŀռ�
	buildSegmentTree(0, 0, data.size() - 1);
}

template<typename T>
void SegmentTree<T>::buildSegmentTree(int Treeindex, int l, int r)
{
	if (l == r) {
		tree[Treeindex]=data[l];
		return;
	}
	int leftindex = leftchild(Treeindex);
	int rightindex = rightchild(Treeindex);
	int mid = l + (r - l) / 2;
	buildSegmentTree(leftindex,l,mid);
	buildSegmentTree(rightindex,mid+1,r);
	tree[Treeindex]= merge(tree[leftindex], tree[rightindex]);
}
template<typename T>
T SegmentTree<T>::merge(T left, T right)
{
	return left+right;
}
template<typename T>
T SegmentTree<T>::get(int index)
{
	if (index < 0 || index >= data.size())
		throw "index error";
	return data[index];
}

template<typename T>
T SegmentTree<T>::Query(int queryL, int queryR)
{
	if (queryL < 0 || queryL >= data.size() || queryR < 0 || queryR >= data.size())
		throw"index error";

	return query(0,0,data.size()-1,queryL,queryR);
}
//����Treeindex������[l,r]��Χ����������[queryL,queryR]��ֵ
template<typename T>
T SegmentTree<T>::query(int Treeindex, int l, int r, int queryL, int queryR)
{
	if (l == queryL && r == queryR)
		return tree[Treeindex];
	int mid = l + (r - l) / 2;
	if (queryL > mid)
		return query(rightchild(Treeindex), mid + 1, r, queryL, queryR);
	else if(queryR<=mid)
		return query(leftchild(Treeindex),l, mid , queryL, queryR);
	else {
		T leftresult = query(leftchild(Treeindex), l, mid, queryL, mid);
		T rightresult = query(rightchild(Treeindex), mid+1,r, mid+1,queryR);
		return merge(leftresult, rightresult);
	}
	
}

template<typename T>
void SegmentTree<T>::set(int index, T e)
{
	if(index < 0 || index >= data.size())
		throw "index error";
	data[index] = e;
	set(0, 0, data.size() - 1, index, e);
}
//����TreeindexΪ���ڵ���߶����и���index��ֵΪe
template<typename T>
void SegmentTree<T>::set(int Treeindex, int l, int r, int index, T e)
{
	if (l == r) {
		tree[Treeindex] = e;
		return;
	}
	int mid = l + (r - l) / 2;
	int left = leftchild(Treeindex);
	int right = rightchild(Treeindex);
	if (index <= mid)
		set(left, l, mid, index, e);
	else
		set(right, mid + 1, r, index, e);
	//���¸��ڵ�ֵ
	tree[Treeindex] = merge(tree[left], tree[right]);
}

template<typename T>
string SegmentTree<T>::tostring()
{
	string str="the tree is:";
	for (int i = 0; i < tree.size(); i++) {
		if (tree[i] == 0)
			str = str + " NULL";
		else
			str = str + " " + to_string(tree[i]);
	}
	return str;
}

int main()
{
	int arr[] = { 4,6,2,3,5,1 };
	SegmentTree<int>tree(arr,6);
	cout << tree.tostring() << endl;
	cout<<tree.Query(2, 3)<<endl;
	tree.set(1, 10);
	cout << tree.tostring() << endl;
}
