#pragma once

#include "phonebook.h"
#include <sstream>

#define CMP_EQ(a, b) ((a) == (b))
#define CMP_LT(a, b) ((a) < (b))
#define CMP_GT(a, b) ((a) > (b))

struct TreeNode
{
    std::vector<PhonebookRecord*> data;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;

    TreeNode(PhonebookRecord* value, TreeNode* parent)
    {
        this->left = this->right = NULL;
        this->data.push_back(value);
        this->parent = parent;
    }
};

template <typename T>
std::wstring ConvertTowString(T value)
{
    std::wostringstream wss;
    wss << value;
    return wss.str();
}

template <typename T>
class Index
{

public:

    TreeNode* root = NULL;

    Index(std::vector<PhonebookRecord*> phoneBook, ::size_t offset)
    {
        this->offset = offset;
        for (int i = 0; i < phoneBook.size(); i++) {
            Insert(root, phoneBook[i]);
        }
    }

    TreeNode* Insert(TreeNode* ptr, PhonebookRecord* value)
    {
        std::wstring first = ConvertTowString(*(T*)((::size_t)value + offset));
        std::wstring second = ConvertTowString(*(T*)((::size_t)ptr->data[0] + offset));

        if (ptr == NULL) {
            ptr = new TreeNode(value, NULL);
        }
        else {
            if (first < second)
                ptr->left = Insert(ptr->left, value);
            else if (first > second)
                ptr->right = Insert(ptr->right, value);
            else if (first == second)
                ptr->data.push_back(value);
        }
        return ptr;
    }

    std::vector<PhonebookRecord*> Search(TreeNode* ptr, T value)
    {
        std::wstring first = ConvertTowString(value);
        std::wstring second = ConvertTowString(*(T*)((::size_t)ptr->data[0] + offset));

        if (first == second)
            return ptr->data;
        Search(ptr->left, value);
        Search(ptr->right, value);
    }

private:
    ::size_t offset;

};
