#include <iostream>
using namespace std;

struct linkNode
{
    int id;
    linkNode *next;
};

int josephus(linkNode *head)
{
    int round = 1; // 第几轮
    linkNode *last_live = head; // 最后一个存活节点
    linkNode *last_live_copy = last_live; // 最后一个存活节点的copy

    while (head->next != head) // 当链表长度大于1时
    {
        int count = 1;           // 报数器

        // 从最后一个报数且存活的节点开始
        linkNode *curr = last_live;

        // cout << "Round " << round << ":\n";
        // cout << "Last live node: " << last_live->id << endl;
        // cout << "Last live node's copy: " << last_live_copy->id << endl;

        // 遍历一次链表，标记待删除节点
        do
        {
            if (count < round + 1 && count != 1)
            {
                // cout << curr->id << " switched to -1\n";
                curr->id = -1; // 标记为待删除
                count++;
            }
            else if (count == round + 1)
            {
                // cout << curr->id << " switched to -1\n";
                curr->id = -1; // 标记为待删除
                count = 1; // 重置报数器
            }
            else
            {
                count++;
                last_live = curr; // 更新最后一个存活节点
            }
            curr = curr->next; // 移动到下一个节点
        } while (curr != last_live_copy);

        // 遍历一次链表，删除标记过的节点
        curr = last_live_copy;
        do
        {
            if (curr->next->id == -1)
            {
                linkNode *toDelete = curr->next;

                // cout << "Node " << toDelete->id << " deleted\n";
                // cout << "prev: " << curr->id << endl;

                curr->next = curr->next->next; // 删除节点

                if (toDelete == head) // 如果头节点被删除
                {
                    head = curr->next; // 更新头节点
                }
                delete toDelete;
            }
            else
            {
                curr = curr->next;
            }
        } while (curr->next != last_live_copy);

        last_live_copy = last_live; // 更新最后一个存活节点的copy
        round++; // 进入下一轮
    }

    return head->id; // 返回最后一个节点的编号
}

int main()
{
    int n;
    cin >> n; 

    linkNode *head = new linkNode();
    head->id = 1;
    linkNode *tail = head;

    for (int i = 2; i <= n; i++)
    {
        linkNode *node = new linkNode();
        node->id = i;
        tail->next = node;
        tail = node;
    }
    tail->next = head; 

    cout << josephus(head);
    return 0;
}
