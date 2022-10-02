# week_12
## 线段树
### 创建（以求和为例）
在构造函数中创建线段树，可以利用递归buildSegment()，在treeindex处创建区间[l...r]的线段树。
```c++
template<typename T>
SegmentTree<T>::SegmentTree(T arr[], int length) {
	data.resize(length,0);//为数组预留n的空间
	for (int i = 0; i < length; i++) {
		data[i]=arr[i];
	}
	tree.resize(4 * length,0);//为树预留4*n的空间
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
```
线段树不只可以用于求和可以根据设定执行不同功能，可以设置融合函数merge（根节点通过融合函数保存左右节点融合后的值）对函数不同的定义就能实现不同的功能。
```c++
//合并左右子树，此处以求和为例，可以根据实际情况修改此函数来执行不同功能
template<typename T>
T SegmentTree<T>::merge(T left, T right)
{
	return left+right;
}
```
### 区间查询
在[l...r]内搜索区间[queryL...queryR]。
取mid=l+(r-l)/2

 - 若[l,r]==[queryL,queryR]；返回；
 - 若queryL>mid;区间在右孩子中查找
 - 若queryR<=mid;区间在左孩子中查找
 - 否则区间包含在根节点左右子树中；分别求出两边包含的目标区间，最后求和。
 
 
 ```c++
 template<typename T>
T SegmentTree<T>::Query(int queryL, int queryR)
{
	if (queryL < 0 || queryL >= data.size() || queryR < 0 || queryR >= data.size())
		throw"index error";

	return query(0,0,data.size()-1,queryL,queryR);
}
//在以Treeindex的树中[l,r]范围内搜索区间[queryL,queryR]的值
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
```

### 线段树的更新
更新index的元素为e。
更新时需注意对应父节点的值也会发生改变。
```c++
template<typename T>
void SegmentTree<T>::set(int index, T e)
{
	if(index < 0 || index >= data.size())
		throw "index error";
	data[index] = e;
	set(0, 0, data.size() - 1, index, e);
}
//在以Treeindex为根节点的线段树中更新index处值为e
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
	//更新根节点值
	tree[Treeindex] = merge(tree[left], tree[right]);
}

```
### leetcode 303区域检索_不可变
可以用线段树来解，但元素值不变，直接用sum数组来保存前i个元素的和即可，不用线段树就行。
```c++
class NumArray {
public:
    vector<int> sum;
    NumArray(vector<int>& nums) {
        sum.push_back(0);
        for(int i=0;i<nums.size();i++){
            sum.push_back(sum.back()+nums[i]);
        }
    }   
    int sumRange(int left, int right) {
        return (sum[right+1]-sum[left]);
    }
};
```
### leetcode307 区域检索_可变
需要更新元素的值使用sum很麻烦而且会超时，这时可以用线段树结构。
搜索和更新只需要修改前面的query和set函数。
```c++
class NumArray {
public:
    NumArray(vector<int>& nums) {
        data=nums;
        tree.resize(4*nums.size(),0);
        buildSegmentTree(0,0,nums.size()-1);
    }
    //创建线段树
    void buildSegmentTree(int Treeindex, int l, int r)
        {
            if (l == r) {
                tree[Treeindex]=data[l];
                return;
            }
            int leftindex = Treeindex*2+1;
            int rightindex = Treeindex*2+2;
            int mid = l + (r - l) / 2;
            buildSegmentTree(leftindex,l,mid);
            buildSegmentTree(rightindex,mid+1,r);
            tree[Treeindex]= tree[leftindex]+tree[rightindex];
        }
    void update(int index, int val) {
        data[index] = val;
	    set(0, 0, data.size() - 1, index, val);
    }
    //在以Treeindex为根节点的线段树中更新index处值为e
    void set(int Treeindex, int l, int r, int index, int e)
    {
        if (l == r) {
            tree[Treeindex] = e;
            return;
        }
        int mid = l + (r - l) / 2;
        int left = Treeindex*2+1;
        int right =Treeindex*2+2;
        if (index <= mid)
            set(left, l, mid, index, e);
        else
            set(right, mid + 1, r, index, e);
        //更新根节点值
        tree[Treeindex] = tree[left]+ tree[right];
    }
    int sumRange(int left, int right) {
        return query(0,0,data.size()-1,left,right);
    }
    //在以Treeindex的树中[l,r]范围内搜索区间[queryL,queryR]的值
    int query(int Treeindex, int l, int r, int queryL, int queryR)
    {
        if (l == queryL && r == queryR)
            return tree[Treeindex];
        int mid = l + (r - l) / 2;
        int left=Treeindex*2+1;
        int right=Treeindex*2+2;
        if (queryL > mid)
            return query(right, mid + 1, r, queryL, queryR);
        else if(queryR<=mid)
            return query(left,l, mid , queryL, queryR);
        else {
            int leftresult = query(left, l, mid, queryL, mid);
            int rightresult = query(right, mid+1,r, mid+1,queryR);
            return leftresult+rightresult;
        }  
    }
private:
    vector<int> data;
    vector<int> tree;
};
```
## Trie字典树
### 构造
node节点结构包括：TreeMap<Character,Node> next(用Map映射表示的指向下一节点的指针)，bool isWord(作为是否该节点是单词结尾的标志)。
Trie结构中包括Node根节点和size大小(Trie中存储的单词数量)。
### 向Trie中添加一个新单词
令cur=root根节点，从根节点开始获取添加单词的每个字母，找到该字母的下一节点若为空则添加新字母，否则cur指针指向该字母的下一个节点，遍历完单词的所有字母。需要判断字典中是否已有该单词，只需看当前cur的isword是否为true，若不为true则不存在该字母更新isword=true,size++。
### 查询单词word是否在Trie中
与添加相似，cur遍历word,next为空则说明不含该单词直接返回空，但最后返回isword。
### 测试
比较Trie与二分搜索树的结构实现保存一部英文小说，添加单词和查询单词所用时间，Trie更具性能优势。Trie与单词量无关，只与目标单词长度有关，在一个很大的集合中对很短的单词操作，优势更加明显。
### 前缀查询
查询是否存在单词以prefix为前缀。
照搬查询单词的逻辑，cur遍历prefix的每个字母，判断cur的next是否为空，是则返回false，否则遍历下一个字母，直到遍历完，最后返回true。
### leetcode 208实现Trie(前缀树)
```c++
class Trie {
public:
    struct Node{
        bool isWord;
        map<char,Node*> next;
        Node(){isWord=false;}
    };
    Node *root=new Node() ;
    Trie() {
        this->size=0;
    }
    
    void insert(string word) {
        Node *cur=root;
        for(int i=0;i<word.length();i++){
            char c=word.at(i);
            map<char,Node*>::iterator pos=cur->next.find(c);
            if(pos==cur->next.end()){
                Node* found=new Node();
                cur->next.insert(make_pair(c,found));
            }
            map<char,Node*>::iterator iter=cur->next.find(c);
            cur=iter->second;
        }
        if(!cur->isWord){
            cur->isWord=true;
            size++;
        }
    }
    
    bool search(string word) {
       Node *cur=root;
        for(int i=0;i<word.length();i++){
            char c=word.at(i);
            map<char,Node*>::iterator pos=cur->next.find(c);
            if(pos==cur->next.end())
           return false;
            cur=pos->second;
        }
        return cur->isWord;
    }
    
    bool startsWith(string prefix) {
        Node *cur=root;
        for(int i=0;i<prefix.length();i++){
            char c=prefix.at(i);
             map<char,Node*>::iterator pos=cur->next.find(c);
            if(pos==cur->next.end())
            return false;
            cur=pos->second;
        }
        return true;
    }
private:
   
    int size;
};
```
c++通过map实现比java麻烦好多，要用指针。难为死我了[裂开]。
