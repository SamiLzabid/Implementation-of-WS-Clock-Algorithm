#include <iostream>
#include <cstdlib>
#include <ctime>
#define MAX_PAGE 100
#define MAX_RAM 50
#define TIME 200
#define TAU 50
#define T_MES 50

struct Page {
    int id;
    int Ref_bit;
    long time;
};
struct Node;
typedef Node* Pointer;

struct Node {
    Page page;
    Pointer next;
    Pointer prev;
};

struct LinkedList {
    int size;
    Pointer front;
};

void initializeList(LinkedList *list) {
    list->size = 0;
    Node* head = new Node;
    list->front = head;
    head->next = list->front;
    head->prev = list->front;
}

int getSize(LinkedList *list) {
    return list->size;
}

bool isEmpty(LinkedList *list) {
    return list->size == 0;
}

Pointer searchPage(LinkedList *list, int x) {
    Pointer current = list->front->next;
    while (current != list->front) {
        if (current->page.id == x) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

bool insertPage(LinkedList *list, Pointer p, Page x) {
    if (getSize(list) == MAX_RAM) {
        return false;
    } else {
        Node* newNode = new Node;
        newNode->page = x;
        newNode->next = p;
        newNode->prev = p->prev;
        p->prev->next = newNode;
        p->prev = newNode;
        list->size++;
        return true;
    }
}

bool removePage(LinkedList *list, Pointer p, Page *x) {
    if (isEmpty(list)) {
        return false;
    } else {
        p->prev->next = p->next;
        p->next->prev = p->prev;
        *x = p->page;
        delete p;
        list->size--;
        return true;
    }
}

void printList(LinkedList *list) {
    Pointer current = list->front->next;
    while (current != list->front) {
        std::cout << "id_page: " << current->page.id << "\t";
        std::cout << "page_Ref_bit: " << current->page.Ref_bit << "\t";
        std::cout << "time_access: " << current->page.time << std::endl;
        current = current->next;
    }
}

void randomPage(Page *x, int clock) {
    x->id = rand() % MAX_PAGE;
    x->Ref_bit = 1;
    x->time = clock;
}

void resetRef_bit(LinkedList *list) {
    Pointer current = list->front->next;
    while (current != list->front) {
        current->page.Ref_bit = 0;
        current = current->next;
    }
}

Pointer ReplacePage(LinkedList *list, long clk) {
    Pointer current = list->front->next;
    Pointer maxTime;
    long t = 9999999;
    while (current != list->front) {
        if (current->page.Ref_bit == 1) {
            current->page.Ref_bit = 0;
        } else if (current->page.Ref_bit == 0) {
            if ((clk - current->page.time) > TAU) {
                return current;
            } else if (current->page.time < t) {
                t = current->page.time;
                maxTime = current;
            }
        }
        current = current->next;
    }
    return maxTime;
}

void workingSetClock(LinkedList *list) {
    int count_hit = 0, cont = 1;
    long clock = 0;
    Page x;
    Pointer position = list->front->next;
    Pointer current;
    for (int i = 0; i < TIME; i++) {
        clock++;
        randomPage(&x, clock);
        current = searchPage(list, x.id);
        if (current == nullptr) {
            if (getSize(list) < MAX_RAM) {
                if (insertPage(list, position, x)) {
                    std::cout << "n- " << cont << "\t Inserted_id: " << x.id << "\tclock: " << clock << std::endl;
                    cont++;
                } else {
                    std::cerr << "ERROR" << std::endl;
                }
            } else {
                Pointer pageOut;
                Page x1;
                pageOut = ReplacePage(list, clock);
                if (removePage(list, pageOut, &x1)) {
                    if (insertPage(list, position, x)) {
                        std::cout << "Removed_id: " << x1.id << " Inserted_id: " << x.id << "\tclock: " << clock
                                  << std::endl;
                    } else {
                        std::cerr << "ERROR" << std::endl;
                    }
                } else {
                    std::cerr << "ERROR" << std::endl;
                }
            }
        } else {
            current->page.Ref_bit = 1;
            current->page.time = clock;
            count_hit++;
        }
        if (clock >= T_MES) {
            resetRef_bit(list);
            clock = 0;
            std::cout << "\t\t\t\t\tReset CLOCK" << std::endl;
        }
    }
    std::cout << "count hit: " << count_hit << std::endl;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));
    LinkedList list;
    initializeList(&list);
    workingSetClock(&list);
     printList(&list);
    return 0;
}

