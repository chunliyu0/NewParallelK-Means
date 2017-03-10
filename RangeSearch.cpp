#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct IntervalWithIndex
{
    IntervalWithIndex(int b, int e, int i)
    {
        begin = b;
        end = e;
        index = i;
    }
    int begin;
    int end;
    int index;
};

struct IntervalTreeNode
{
    IntervalTreeNode(int v)
    {
        value = v;
        left = 0, right = 0;
    }
    vector<IntervalWithIndex> mid_interval;
    int value;
    IntervalTreeNode* left;
    IntervalTreeNode* right;
};

IntervalTreeNode* buildTree(vector<IntervalWithIndex>& arr, int left, int right)
{
	if(left > right)
        return 0;
    else if(left == right)
    {
        IntervalTreeNode* root = new IntervalTreeNode((arr[left].begin + arr[right].end) / 2);
        root->mid_interval.push_back(arr[left]);
        return root; 
    }

    int minv = INT_MAX, maxv = INT_MIN;
    for(int i = left; i <= right; i++)
    {
        minv = min(minv, arr[i].begin);
        maxv = max(maxv, arr[i].end);
    }
    int midv = (minv + maxv) / 2;
    IntervalTreeNode* root = new IntervalTreeNode(midv);
    int l = left;
    int cur = left;
    int r = right;
    while(cur <= r)
    {
        if(arr[cur].end < midv)
            swap(arr[cur++], arr[l++]);
        else if(arr[cur].begin > midv)
            swap(arr[cur], arr[r--]);
        else
        {
            root->mid_interval.push_back(arr[cur]);
            cur++;
        }
    }
    root->left = buildTree(arr, left, l - 1);
    root->right = buildTree(arr, r + 1, right);
    return root;
}

void search(vector<IntervalWithIndex>& ret, IntervalTreeNode* root, int value)
{
    if(root == 0)
        return;
    if(value == root->value)
    {
        ret.insert(ret.end(), root->mid_interval.begin(), root->mid_interval.end());
    }
    else
    {
        for(auto& interval : root->mid_interval)
        {
            if(interval.begin <= value && interval.end >= value)
                ret.push_back(interval);
        }
        if(value < root->value)
            search(ret, root->left, value);
        else
            search(ret, root->right, value);
    }
}

class IntervalSet
{
public:
    
    vector<IntervalWithIndex> tree;
    IntervalTreeNode* root;
    IntervalSet(vector<pair<int, int> >& arr)
    {
        for(int i = 0; i < arr.size(); i++)
            tree.push_back(IntervalWithIndex(arr[i].first, arr[i].second, i));
        root = buildTree(tree, 0, (int)tree.size() - 1);
    }
    vector<int> search(int value)
    {
        vector<IntervalWithIndex> ret;
        ::search(ret, root, value);
        vector<int> indices;
        for(auto& interval : ret)
            indices.push_back(interval.index);
        return indices;
    }
    
};

int main(){
    vector<pair<int, int> > pairs = {{4, 10}, {1, 6}, {20, 30}, {7, 12}};
    IntervalSet intset(pairs);
    vector<int> ret = intset.search(5);
    for(auto n : ret)
        cout << n << endl;
}
