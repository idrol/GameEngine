//
// Created by idrol on 22/01/2020.
//
#pragma once

#include <stdexcept>

/**
 * Find and Exists operations are O(n)
 * Insert is O(1)
 * InsertAfter is O(n)
 * Remove is O(n)
 * Next, GetLength and ResetCursor is O(1)
 * @tparam T
 */
template <class T>
class LinkedList {
public:
    struct Node {
        T data;
        Node *prev = nullptr;
        Node *next = nullptr;
    };

    ~LinkedList() {
        while(auto node = Next()) {
            delete node;
        }
    }

    /**
     * Add a new element to the end of the list
     * @param data
     */
    void Insert(T data) {
        Node *node = new Node();
        if(start == nullptr) {
            start = node;
            cursor = start;
        }
        node->data = data;
        node->prev = head;
        if(head != nullptr) {
            head->next = node;
        }
        head = node;
        length++;
    }

    void InsertAfter(T afterThisData, T dataToInsert) {
        Node *afterThisNode = Find(afterThisData);
        if(!afterThisNode) {
            throw std::runtime_error("Cannot insert after data that does not exist in the linked list");
        }
        Node *nextNode = afterThisNode->next; // Can be nullptr
        Node *newNode = new Node();
        if(nextNode != nullptr) {
            nextNode->prev = newNode;
        }
        afterThisNode->next = newNode;

        newNode->prev = afterThisNode;
        newNode->next = nextNode;
        newNode->data = dataToInsert;
    }

    Node *Find(T data) {
        Node *foundNode = nullptr;
        while(auto node = Next()) {
            if (node->data == data) {
                foundNode = node;
                break;
            }
        }
        ResetCursor();
        return foundNode;
    }

    bool Exists(T data) {
        Node *foundNode = Find(data);
        return foundNode != nullptr;
    }

    /**
     * Remove node contain data this effectivly makes prev point to next and next point to prev and deletes this datas node
     * @param data
     */
    bool Remove(T data) {
        bool found = false;
        Node *nodeToDelete = nullptr;
        while(auto node = Next()) {
            if(node->data == data) {
                node->prev->next = node->next;
                if(node->next != nullptr) {
                    node->next->prev = node->prev;
                }
                nodeToDelete = node;
                found = true;
            }
        }
        ResetCursor();
        if(found) {
            delete nodeToDelete;
        }
        return found;
    }

    int GetLength() {
        return length;
    }

    /**
     * Retrieve the node at the cursor and advance the cursor one step
     */
    Node *Next() {
        Node *ret = cursor;
        if(cursor != nullptr) {
            cursor = cursor->next;
        }
        return ret;
    }

    /**
     * Reset the cursor to the start of the list
     */
    void ResetCursor() {
        cursor = start;
    }


    Node *cursor = nullptr;
    Node *start = nullptr;
    Node *head = nullptr;
    int length = 0;
};