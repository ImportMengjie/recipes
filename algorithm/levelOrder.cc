/*
 * 给你一个二叉树，请你返回其按层序遍历得到的节点值。（即逐层地，从左到右访问所有节点）。
 *
 * 示例：
 * ⁠   3
 * ⁠  / \
 * ⁠ 9  20
 * ⁠   /  \
 * ⁠  15   7
 *
 * 返回其层次遍历结果：
 *
 * [
 * ⁠ [3],
 * ⁠ [9,20],
 * ⁠ [15,7]
 * ]
 */

#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

vector<vector<int>> levelOrder(TreeNode *root) {
    vector<vector<int>> ret;
    if (!root) {
        return ret;
    }

    queue<TreeNode *> q;
    q.push(root);
    while (!q.empty()) {
        int currentLevelSize = q.size();
        ret.push_back(vector<int>());
        for (int i = 1; i <= currentLevelSize; ++i) {
            auto node = q.front();
            q.pop();
            ret.back().push_back(node->val);
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return ret;
}

vector<vector<int>> levelOrder2(TreeNode *root) {
    vector<vector<int>> ans;
    if (!root) return ans;
    queue<pair<TreeNode *, int>> q;
    q.push({root, 0});
    int depth = -1;
    while (!q.empty()) {
        TreeNode *pNode = q.front().first;
        if (depth < q.front().second) {
            ans.push_back(vector<int>());
            depth++;
        }
        ans[depth].push_back(pNode->val);
        if (pNode->left) q.push({pNode->left, depth + 1});
        if (pNode->right) q.push({pNode->right, depth + 1});
        q.pop();
    }
    return ans;
}

// ----- TEST -----

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

int main() {
    TreeNode arr[] = {3, 9, 20, 15, 7};
    arr[0].left = &arr[1];
    arr[0].right = &arr[2];
    arr[2].left = &arr[3];
    arr[2].right = &arr[4];
    std::cout << levelOrder(arr) << std::endl;
}
