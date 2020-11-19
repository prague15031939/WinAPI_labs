#pragma once

#include "phonebook.h"
#include <sstream>

struct TreeNode
{
    std::vector<PhonebookRecord*> data;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;

    TreeNode(PhonebookRecord* value, TreeNode* parent)
    {
        this->left = this->right = nullptr;
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

    TreeNode* root = nullptr;

    Index(std::vector<PhonebookRecord*> phoneBook, ::size_t offset)
    {
        this->offset = offset;
        for (int i = 0; i < phoneBook.size(); i++) {
            root = Insert(root, phoneBook[i]);
        }
    }

    TreeNode* Insert(TreeNode* ptr, PhonebookRecord* value)
    {
        if (ptr == nullptr) {
            ptr = new TreeNode(value, nullptr);
        }
        else {
            std::wstring first = ConvertTowString(*(T*)((::size_t)value + offset));
            std::wstring second = ConvertTowString(*(T*)((::size_t)ptr->data[0] + offset));

            if (first < second)
                ptr->left = Insert(ptr->left, value);
            else if (first > second)
                ptr->right = Insert(ptr->right, value);
            else
                ptr->data.push_back(value);
        }
        return ptr;
    }

    std::vector<PhonebookRecord*> Search(TreeNode* ptr, T value)
    {
        if (ptr != nullptr) {
            std::wstring first = ConvertTowString(value);
            std::wstring second = ConvertTowString(*(T*)((::size_t)ptr->data[0] + offset));

            if (first == second)
                return ptr->data;
            else if (first < second)
                return Search(ptr->left, value);
            else
                return Search(ptr->right, value);
        }

        return {};
    }

private:
    ::size_t offset;

};
