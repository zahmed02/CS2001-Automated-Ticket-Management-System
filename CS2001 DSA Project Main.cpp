#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
using namespace std;
using namespace chrono;
// REQUIREMENT 1 SERVICE REQUEST (TICKET) MANAGEMENT
class Ticket
{
public:
  int ticketholderid;
  string TicketholderName;
  int prioritylevel;
  string requeststatement;
  chrono::_V2::system_clock::time_point startTime;
  bool isOpen;

  chrono::_V2::system_clock::time_point endTime{};

  Ticket()
  {
  }

  Ticket(string TicketholderName, int prioritylevel, string requeststatement) : TicketholderName(TicketholderName), prioritylevel(prioritylevel), requeststatement(requeststatement), startTime(chrono::system_clock::now()), isOpen(true)
  {
    ticketholderid = rand() % 1000000;
  }

  time_t Timegetter()
  {
    return system_clock::to_time_t(startTime);
  }
  // SHOWING ALL INFORMATION PERTAINING TICKET(S)
  void print()
  {
    cout << "====================" << endl;
    cout << "    TICKET INFORMATION" << endl;
    cout << "====================" << endl;
    cout << "Ticket Holder ID: " << ticketholderid << endl;
    cout << "priortiy Level: " << prioritylevel << endl;
    cout << "Ticket Holder Name: " << TicketholderName << endl;
    cout << "Request Specification: " << requeststatement << endl;
    cout << "Time Of Creation: ";
    printCreationTime();
    if (!isOpen)
    {
      cout << "Closing Time: ";
      printClosingTime();
    }
    cout << "Is Open: " << isOpen << endl;
    cout << endl;
  }

  void printCreationTime()
  {
    time_t returnTime = system_clock::to_time_t(startTime);
    cout << ctime(&returnTime);
  }

  void printClosingTime()
  {
    time_t returnTime = system_clock::to_time_t(endTime);
    cout << ctime(&returnTime);
  }

  void close()
  {
    isOpen = false;
    endTime = chrono::system_clock::now();
  }
};
// HELPER NODE CLASS FOR LINKED LIST
class NodeTicket
{
public:
  Ticket *ticket;
  NodeTicket *next;
  NodeTicket *prev;
  NodeTicket(Ticket *ticket) : ticket(ticket), next(NULL), prev(NULL)
  {
  }
};
// LINKED LIST BLUEPRINT FOR TICKET CLASS
class TicketLinkedList
{
public:
  NodeTicket *head;
  NodeTicket *tail;
  int length;
  TicketLinkedList() : head(NULL), tail(NULL), length(0)
  {
  }
  // TICKET PRINT METHOD
  void print()
  {
    NodeTicket *temp = head;
    while (temp != NULL)
    {
      temp->ticket->print();
      temp = temp->next;
    }
  }
  // ALL (OPEN) TICKET PRINT FUNCTION
  void displayOpenTickets()
  {
    NodeTicket *temp = head;
    while (temp != NULL)
    {
      if (temp->ticket->isOpen)
      {
        temp->ticket->print();
      }
      temp = temp->next;
    }
  }
  // ALL (CLOSED) TICKET PRINT FUNCTION
  void displayClosedTickets()
  {
    NodeTicket *temp = head;
    while (temp != NULL)
    {
      if (!temp->ticket->isOpen)
      {
        temp->ticket->print();
      }
      temp = temp->next;
    }
  }
  // FUNCTION FOR ADDING TICKET TO LINKED LIST
  void addTicket(Ticket *ticket)
  {
    NodeTicket *newNode = new NodeTicket(ticket);
    length++;
    if (head == NULL)
    {
      head = newNode;
      tail = newNode;
      return;
    }
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
    return;
  }
  // FUNCTION FOR REMOVING A TICKET BY ID
  void removeTicket(int id)
  {
    NodeTicket *temp = head;
    while (temp != NULL)
    {
      if (temp->ticket->ticketholderid == id)
      {
        cout << "[+] Removing Ticket-" << id << endl;
        // REMOVE TICKET
        NodeTicket *prevNode = temp->prev;
        NodeTicket *nextNode = temp->next;
        // IF ONLY NODE PRESENT
        if (prevNode == NULL && nextNode == NULL)
        {
          head = NULL;
          tail = NULL;
        }
        else
          // IF HEAD PRESENT
          if (prevNode == NULL)
          {
            nextNode->prev = NULL;
            head = nextNode;
          }
          else
            // IF TAIL PRESENT
            if (nextNode == NULL)
            {
              prevNode->next = NULL;
              tail = prevNode;
            }
            else
              // IF NEITHER HEAD NOR TAIL
              if (nextNode != NULL && prevNode != NULL)
              {
                prevNode->next = nextNode;
                nextNode->prev = prevNode;
              }
        delete temp;
        break;
      }
      temp = temp->next;
    }
  }
  // RETRIEVES A POINTER TO A TICKET OBJECT BASED ON ITS TICKETHOLDER ID
  Ticket *getTicketById(int id)
  {
    NodeTicket *current = head;
    while (current != NULL)
    {
      if (current->ticket->ticketholderid == id)
      {
        return current->ticket;
      }
      current = current->next;
    }
    return NULL;
  }
  // RETRIEVES A POINTER TO A TICKET OBJECT BASED ON ITS TICKETHOLDER'S NAME
  Ticket *getTicketByTicketholderName(string Name)
  {
    NodeTicket *current = head;
    while (current != NULL)
    {
      if (current->ticket->TicketholderName == Name)
      {
        return current->ticket;
      }
      current = current->next;
    }
    return NULL;
  }
  // RETURNS A DYNAMIC ARRAY CONTAINING ELEMENTS OF LINKED LIST
  Ticket **toArray()
  {
    // INITIALIZING ARRAY
    if (length > 0)
    {
      Ticket **ticketsArray = new Ticket *[length];
      NodeTicket *current = head;
      int i = 0;
      while (current != NULL)
      {
        ticketsArray[i] = current->ticket;
        current = current->next;
        i++;
      }
      return ticketsArray;
    }
    return NULL;
  }
  // FUNCTION TO DELETE ALL NODES FROM LINKED LIST
  void deleteAll()
  {
    NodeTicket *current = tail;
    while (current != NULL)
    {
      NodeTicket *temp = current;
      current = current->prev;
      delete temp;
    }
    head = tail = NULL;
  }
  // REFILL THE LINKED LIST VIA AN ARRAY
  void fromArray(Ticket **array, int length)
  {
    deleteAll();
    for (int i = length - 1; i >= 0; i--)
    {
      addTicket(array[i]);
    }
  }
  // IMPLEMENTING SORTING ALGORITHMS
  // BUBBLE SORT
  void bubbleSort(string sortBy)
  {
    if (head == tail || head == NULL)
    {
      return;
    }
    // LENGTH CALCULATOR
    int length = 0;
    NodeTicket *current = head;
    while (current != NULL)
    {
      length++;
      current = current->next;
    }
    for (int i = 0; i < length; i++)
    {
      current = head->next;
      while (current != NULL)
      {
        // SWAP if (PREV > NEXT)
        bool condition = false;
        if (sortBy == "prioritylevel")
        {
          condition = current->prev->ticket->prioritylevel > current->ticket->prioritylevel;
        }
        else if (sortBy == "creationTime")
        {
          condition = current->prev->ticket->startTime > current->ticket->startTime;
        }
        else if (sortBy == "TicketholderName")
        {
          condition = current->prev->ticket->TicketholderName > current->ticket->TicketholderName;
        }
        if (condition)
        {
          Ticket *temp = current->prev->ticket;
          current->prev->ticket = current->ticket;
          current->ticket = temp;
        }
        current = current->next;
      }
    }
  }
  // INSERTION SORT
  void insertionSort(string sortBy)
  {
    NodeTicket *current = head;
    while (current != NULL)
    {
      Ticket *key = current->ticket;
      NodeTicket *j = current;
      bool condition = false;
      if (sortBy == "prioritylevel" && j->prev != NULL)
      {
        condition = j->prev->ticket->prioritylevel > key->prioritylevel;
      }
      else if (sortBy == "creationTime" && j->prev != NULL)
      {
        condition = j->prev->ticket->startTime > key->startTime;
      }
      else if (sortBy == "TicketholderName" && j->prev != NULL)
      {
        condition = j->prev->ticket->TicketholderName > key->TicketholderName;
      }
      while (j->prev != NULL && condition)
      {
        j->ticket = j->prev->ticket;
        j = j->prev;
        condition = false;
        if (sortBy == "prioritylevel" && j->prev != NULL)
        {
          condition = j->prev->ticket->prioritylevel > key->prioritylevel;
        }
        else if (sortBy == "creationTime" && j->prev != NULL)
        {
          condition = j->prev->ticket->startTime > key->startTime;
        }
        else if (sortBy == "TicketholderName" && j->prev != NULL)
        {
          condition = j->prev->ticket->TicketholderName > key->TicketholderName;
        }
      }
      j->ticket = key;
      current = current->next;
    }
  }
  // SELECTION SORT
  void selectionSort(string sortBy)
  {
    NodeTicket *i = head;
    while (i != NULL)
    {
      NodeTicket *current = i;
      NodeTicket *minimum = i;
      while (current != NULL)
      {
        bool condition = false;

        if (sortBy == "prioritylevel")
        {
          condition = current->ticket->prioritylevel > minimum->ticket->prioritylevel;
        }
        else if (sortBy == "creationTime")
        {
          condition = current->ticket->startTime > minimum->ticket->startTime;
        }
        else if (sortBy == "TicketholderName")
        {
          condition = current->ticket->TicketholderName > minimum->ticket->TicketholderName;
        }
        if (condition)
        {
          minimum = current;
        }
        Ticket *temp = minimum->ticket;
        minimum->ticket = current->ticket;
        current->ticket = temp;
        current = current->next;
      }
      i = i->next;
    }
  }
  // QUICK SORT HELPER FUNCTION 1
  int partition(Ticket **array, int left, int right, string sortBy)
  {
    Ticket pivot = *array[right];
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
      bool condition = false;
      if (sortBy == "prioritylevel")
      {
        condition = array[j]->prioritylevel < pivot.prioritylevel;
      }
      else if (sortBy == "creationTime")
      {
        condition = array[j]->startTime < pivot.startTime;
      }
      else if (sortBy == "TicketholderName")
      {
        condition = array[j]->TicketholderName < pivot.TicketholderName;
      }
      if (condition)
      {
        i++;
        swap(array[i], array[j]);
      }
    }
    swap(array[i + 1], array[right]);
    return i + 1;
  }
  // QUICK SORT HELPER FUNCTION 2
  void qs(Ticket **array, int left, int right, string sortBy)
  {
    if (left < right)
    {
      int pi = partition(array, left, right, sortBy);
      qs(array, left, pi - 1, sortBy);
      qs(array, pi + 1, right, sortBy);
    }
  }
  // FINAL QUICK SORT CALLING HELPER FUNCTIONS 1-2
  void quickSort(string sortBy)
  {
    // TRANSFORM LINKED LIST TO ARRAY
    Ticket **tickets = toArray();
    qs(tickets, 0, length - 1, sortBy);
    fromArray(tickets, length);
    delete[] tickets;
  }

  // MERGE SORT HELPER FUNCTION 1
  void merge(Ticket **tickets, int left, int mid, int right, string sortBy)
  {
    if (left == right)
    {
      return;
    }
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Ticket **leftArray = new Ticket *[n1];
    Ticket **rightArray = new Ticket *[n2];
    for (int i = 0; i < n1; i++)
    {
      leftArray[i] = tickets[left + i];
    }
    for (int i = 0; i < n2; i++)
    {
      rightArray[i] = tickets[mid + 1 + i];
    }
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
      bool condition = false;
      if (sortBy == "prioritylevel")
      {
        condition = leftArray[i]->prioritylevel <= rightArray[j]->prioritylevel;
      }
      else if (sortBy == "creationTime")
      {
        condition = leftArray[i]->startTime <= rightArray[j]->startTime;
      }
      else if (sortBy == "TicketholderName")
      {
        condition = leftArray[i]->TicketholderName <= rightArray[j]->TicketholderName;
      }
      if (condition)
      {
        tickets[k] = leftArray[i];
        i++;
        k++;
      }
      else
      {
        tickets[k] = rightArray[j];
        j++;
        k++;
      }
    }
    while (i < n1)
    {
      tickets[k] = leftArray[i];
      i++;
      k++;
    }
    while (j < n2)
    {
      tickets[k] = rightArray[j];
      j++;
      k++;
    }
    delete[] leftArray;
    delete[] rightArray;
  }
  // MERGE SORT HELPER FUNCTION 2
  void ms(Ticket **tickets, int left, int right, string sortBy)
  {
    if (left < right)
    {
      int mid = left + (right - left) / 2;
      ms(tickets, left, mid, sortBy);
      ms(tickets, mid + 1, right, sortBy);
      merge(tickets, left, mid, right, sortBy);
    }
  }
  // FINAL MERGE SORT CALLING ALL HELPER FUNCTIONS 1-2
  void mergeSort(string sortBy)
  {
    Ticket **tickets = toArray();
    ms(tickets, 0, length - 1, sortBy);
    fromArray(tickets, length);
    delete[] tickets;
  }
};

// =============================================
// AVL TREE IMPLEMENTATION FOR RANGE QUERIES
// =============================================
class AVLNode
{
public:
  int key;        // Ticket ID
  Ticket *ticket; // Pointer to ticket
  AVLNode *left;
  AVLNode *right;
  int height;

  AVLNode(int k, Ticket *t) : key(k), ticket(t), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree
{
private:
  AVLNode *root;

  // Helper functions
  int height(AVLNode *node)
  {
    return (node == nullptr) ? 0 : node->height;
  }

  int getBalance(AVLNode *node)
  {
    return (node == nullptr) ? 0 : height(node->left) - height(node->right);
  }

  int max(int a, int b)
  {
    return (a > b) ? a : b;
  }

  AVLNode *rightRotate(AVLNode *y)
  {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
  }

  AVLNode *leftRotate(AVLNode *x)
  {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
  }

  AVLNode *insertNode(AVLNode *node, int key, Ticket *ticket)
  {
    if (node == nullptr)
      return new AVLNode(key, ticket);

    if (key < node->key)
      node->left = insertNode(node->left, key, ticket);
    else if (key > node->key)
      node->right = insertNode(node->right, key, ticket);
    else
      return node;

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && key < node->left->key)
      return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
      return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
      node->left = leftRotate(node->left);
      return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
      node->right = rightRotate(node->right);
      return leftRotate(node);
    }

    return node;
  }

  void rangeQueryHelper(AVLNode *node, int low, int high)
  {
    if (node == nullptr)
      return;

    if (low < node->key)
      rangeQueryHelper(node->left, low, high);

    if (node->key >= low && node->key <= high)
      node->ticket->print();

    if (high > node->key)
      rangeQueryHelper(node->right, low, high);
  }

public:
  AVLTree() : root(nullptr) {}

  void insert(int key, Ticket *ticket)
  {
    root = insertNode(root, key, ticket);
  }

  void queryRange(int low, int high)
  {
    rangeQueryHelper(root, low, high);
  }
};

// REQUIREMENT 2 AGENT MANAGEMENT
class Agent
{
public:
  int agentid;
  string agentname;
  string type;
  int numTicketsAssigned;
  Ticket *assignedTickets[5];
  bool available;

  Agent()
  {
  }

  Agent(string agentname, string type) : agentname(agentname), type(type), numTicketsAssigned(0), available(true)
  {
    agentid = rand() % 1000000;
  }

  // SHOW AGENT INFORMATION
  void print()
  {
    cout << "========================" << endl;
    cout << "   AGENT ANALYSIS" << endl;
    cout << "========================" << endl;
    cout << "Agent ID: " << agentid << endl;
    cout << "Agent Name: " << agentname << endl;
    cout << "Agent Type: " << type << endl;
    cout << "Total Tickets Assigned: " << numTicketsAssigned << endl;
    for (int i = 0; i < numTicketsAssigned; i++)
    {
      cout << "--> Ticket " << i + 1 << ":" << endl;
      assignedTickets[i]->print();
    }
  }

  // ASSIGN A GIVEN TICKET TO AN AGENT
  void assignTickets(Ticket *ticket)
  {
    if (available)
    {
      assignedTickets[numTicketsAssigned] = ticket;
      numTicketsAssigned++;
      if (numTicketsAssigned >= 5)
      {
        available = false;
      }
    }
  }

  // RESOLVES AN ALREADY ASSIGNED TICKET TO AN AGENT
  Ticket *resolveTicket()
  {
    if (numTicketsAssigned > 0)
    {
      Ticket *resolvedTicket = assignedTickets[0];
      numTicketsAssigned--;
      // PUSH ALL TICKETS A STEP IN REVERSE
      for (int i = 1; i < 5; i++)
      {
        assignedTickets[i - 1] = assignedTickets[i];
      }
      assignedTickets[4] = NULL;
      available = true;
      return resolvedTicket;
    }
    return NULL;
  }
};
// DYNAMIC ARRAY BLUEPRINT FOR AGENT CLASS
class AgentDynamicArray
{
public:
  Agent **data;
  int capacity;
  int length;

  AgentDynamicArray() : capacity(1), length(0)
  {
    data = new Agent *[capacity];
  }

  // SHOW ALL AGENTS
  void print()
  {
    for (int i = 0; i < length; i++)
    {
      data[i]->print();
      cout << endl;
    }
  }

  // ADD NEW AGENT
  void addAgent(Agent *agent)
  {
    length++;
    if (length > capacity)
    {
      // UPDATE ARRAY CAPACITY AS PER ADDITION
      capacity *= 2;
      Agent **newData = new Agent *[capacity];
      // COPY EXISTING OLD DATA
      for (int i = 0; i < length - 1; i++)
      {
        newData[i] = data[i];
      }
      delete[] data;
      data = newData;
    }
    data[length - 1] = agent;
  }

  // Assigning ticket to the agent that has the lowest workload
  bool assignTickets(Ticket *ticket)
  {
    int mostFree = -1;
    bool assigned = false;

    for (int i = 0; i < length; i++)
    {
      if (data[i]->available && data[i]->type == ticket->requeststatement)
      {
        if (mostFree == -1)
        {
          mostFree = i;
        }
        else if (data[i]->numTicketsAssigned < data[mostFree]->numTicketsAssigned)
        {
          mostFree = i;
        }
      }
    }

    if (mostFree != -1)
    {
      data[mostFree]->assignTickets(ticket);
      cout << "[+] Assigned Ticket ID " << ticket->ticketholderid
           << " (Priority: " << ticket->prioritylevel << ") to Agent "
           << data[mostFree]->agentname << endl;
      assigned = true;
    }
    else
    {
      cout << "[X] No available " << ticket->requeststatement
           << " agents for Ticket ID " << ticket->ticketholderid << endl;
    }

    return assigned;
  }

  // SINGLE TICKET RESOLVE METHOD
  Ticket *resolveTicket()
  {
    for (int i = 0; i < length; i++)
    {
      if (data[i]->numTicketsAssigned > 0)
      {
        return data[i]->resolveTicket();
      }
    }
    return NULL;
  }

  // SORTING ALGORITHMS
  // BUBBLE SORT
  void bubbleSort()
  {
    for (int i = 0; i < length; i++)
    {
      for (int j = 0; j < length - 1; j++)
      {
        if (data[j]->numTicketsAssigned > data[j + 1]->numTicketsAssigned)
        {
          Agent *temp = data[j];
          data[j] = data[j + 1];
          data[j + 1] = temp;
        }
      }
    }
  }
  // SELECTION SORT
  void selectionSort()
  {
    for (int i = 0; i < length - 1; i++)
    {
      int smallest = i;
      for (int j = i + 1; j < length; j++)
      {
        if (data[j]->numTicketsAssigned < data[smallest]->numTicketsAssigned)
        {
          smallest = j;
        }
      }
      if (i != smallest)
      {
        Agent *temp = data[smallest];
        data[smallest] = data[i];
        data[i] = temp;
      }
    }
  }
  // INSERTION SORT
  void insertionSort()
  {
    for (int i = 1; i < length; i++)
    {
      Agent *key = data[i];
      int j = i;
      while (j > 0 && data[j - 1]->numTicketsAssigned > key->numTicketsAssigned)
      {
        data[j] = data[j - 1];
        j--;
      }
      data[j] = key;
    }
  }
  // MERGE SORT HELPER FUNCTION 1
  void merge(int left, int mid, int right)
  {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    Agent **leftArray = new Agent *[n1];
    Agent **rightArray = new Agent *[n2];
    for (int i = 0; i < n1; i++)
    {
      leftArray[i] = data[left + i];
    }
    for (int i = 0; i < n2; i++)
    {
      rightArray[i] = data[mid + i + 1];
    }
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
      if (leftArray[i]->numTicketsAssigned <= rightArray[j]->numTicketsAssigned)
      {
        data[k] = leftArray[i];
        i++;
        k++;
      }
      else
      {
        data[k] = rightArray[j];
        j++;
        k++;
      }
    }
    while (i < n1)
    {
      data[k] = leftArray[i];
      i++;
      k++;
    }
    while (j < n2)
    {
      data[k] = rightArray[j];
      j++;
      k++;
    }
    delete[] leftArray;
    delete[] rightArray;
  }
  // MERGE SORT HELPER FUNCTION 2
  void ms(int left, int right)
  {
    if (left < right)
    {
      int mid = left + (right - left) / 2;
      ms(left, mid);
      ms(mid + 1, right);
      merge(left, mid, right);
    }
  }
  // FINAL MERGE SORT CALLING ALL HELPER FUNCTION 1-2
  void mergeSort()
  {
    ms(0, length - 1);
  }

  // QUICK SORT HELPER FUNCTION 1
  int partition(int left, int right)
  {
    int pivot = right;
    if (right <= left)
    {
      return left;
    }
    int i = left - 1;
    for (int j = left; j < right; j++)
    {
      if (data[j]->numTicketsAssigned > data[pivot]->numTicketsAssigned)
      {
        i++;
        Agent *temp = data[i];
        data[i] = data[j];
        data[j] = temp;
      }
    }
    Agent *temp = data[pivot];
    data[pivot] = data[i + 1];
    data[i + 1] = temp;
    return i + 1;
  }
  // QUICK SORT HELPER FUNCTION 2
  void qs(int left, int right)
  {
    if (left < right)
    {
      int pi = partition(left, right);
      qs(left, pi - 1);
      qs(pi + 1, right);
    }
  }
  // FINAL QUICK SORT CALLING ALL HELPER FUNCTION 1-2
  void quickSort()
  {
    qs(0, length - 1);
  }
};

// REQUIREMENT 3.1 TICKET RESOLUTION LOGS (TRANSACTION LOG)
// HELPER CLASS NODE FOR STACK BLUEPRINT OF CLASS RESOLUTIONLOGISTICSSTACK
class NodeTicketResolution
{
public:
  Ticket *ticket; // TICKET CLASS FORWARD DECLARATION INTEGRATED IN TICKET RESOLUTION NODE
  NodeTicketResolution *next;
  chrono::_V2::system_clock::time_point closingTime;

  NodeTicketResolution(Ticket *ticket) : ticket(ticket), next(NULL), closingTime(ticket->endTime)
  {
  }
};
// STACK BLUEPRINT FOR RESOLUTIONLOGISTICS
class ResolutionLogisticsStack
{
  NodeTicketResolution *top;

public:
  ResolutionLogisticsStack() : top(NULL)
  {
  }
  // DISPLAY ALL COMPLETE LOGS
  void print()
  {
    cout << "======================================" << endl;
    cout << "      DISPLAYING RESOLVED TICKETS" << endl;
    cout << "======================================" << endl;
    NodeTicketResolution *current = top;
    while (current != NULL)
    {
      current->ticket->print();
      current = current->next;
    }
  }

  // ADD NEW LOG TO STACK
  void addTicket(Ticket *ticket)
  {
    if (ticket->isOpen)
    {
      cout << "[-] CANNOT ADD to resolution stack, CURRENT TICKET IS OPEN." << endl;
      return;
    }
    NodeTicketResolution *newNode = new NodeTicketResolution(ticket);
    if (top == NULL)
    {
      top = newNode;
      return;
    }
    newNode->next = top;
    top = newNode;
  }

  // [PEEK METHOD] GETTER METHOD FOR MOST RECENT LOG
  Ticket *peek()
  {
    if (top == NULL)
    {
      return NULL;
    }
    return top->ticket;
  }
};

// REQUIREMENT 3.2 PENDING TICKET QUEUE
// QUEUE BLUEPRINT FOR PENDINGTICKETSQUEUE (HEAP-BASED)
class PendingTicketsQueue
{
private:
  Ticket **heap; // Heap array
  int capacity;  // Maximum capacity
  int size;      // Current size

  // Helper functions
  int parent(int i) { return (i - 1) / 2; }
  int left(int i) { return 2 * i + 1; }
  int right(int i) { return 2 * i + 2; }

  void swap(Ticket *&a, Ticket *&b)
  {
    Ticket *temp = a;
    a = b;
    b = temp;
  }

  void heapifyUp(int i)
  {
    while (i > 0 && heap[parent(i)]->prioritylevel < heap[i]->prioritylevel)
    {
      swap(heap[i], heap[parent(i)]);
      i = parent(i);
    }
  }

  void heapifyDown(int i)
  {
    int largest = i;
    int l = left(i);
    int r = right(i);

    if (l < size && heap[l]->prioritylevel > heap[largest]->prioritylevel)
      largest = l;

    if (r < size && heap[r]->prioritylevel > heap[largest]->prioritylevel)
      largest = r;

    if (largest != i)
    {
      swap(heap[i], heap[largest]);
      heapifyDown(largest);
    }
  }

public:
  PendingTicketsQueue() : capacity(10), size(0)
  {
    heap = new Ticket *[capacity];
  }

  ~PendingTicketsQueue()
  {
    delete[] heap;
  }

  bool isEmpty()
  {
    return size == 0;
  }

  void enqueueTicket(Ticket *ticket)
  {
    if (!ticket->isOpen)
    {
      cout << "CANNOT ENQUEUE, CURRENT QUEUE is for OPEN TICKETS ONLY." << endl;
      return;
    }

    if (size == capacity)
    {
      // Double the capacity
      capacity *= 2;
      Ticket **newHeap = new Ticket *[capacity];
      for (int i = 0; i < size; i++)
      {
        newHeap[i] = heap[i];
      }
      delete[] heap;
      heap = newHeap;
    }

    heap[size] = ticket;
    size++;
    heapifyUp(size - 1);
  }

  Ticket *dequeueTicket()
  {
    if (isEmpty())
    {
      return NULL;
    }

    Ticket *top = heap[0];
    heap[0] = heap[size - 1];
    size--;
    heapifyDown(0);
    return top;
  }

  void displayQueue()
  {
    cout << "================================" << endl;
    cout << "     PENDING TICKETS QUEUE" << endl;
    cout << "================================" << endl;

    if (isEmpty())
    {
      cout << "Queue is empty." << endl;
      return;
    }

    // Create a temporary array for sorting
    Ticket **temp = new Ticket *[size];
    for (int i = 0; i < size; i++)
    {
      temp[i] = heap[i];
    }

    // Sort temporary array by priority (descending)
    for (int i = 0; i < size - 1; i++)
    {
      for (int j = 0; j < size - i - 1; j++)
      {
        if (temp[j]->prioritylevel < temp[j + 1]->prioritylevel)
        {
          swap(temp[j], temp[j + 1]);
        }
      }
    }

    // Display sorted tickets
    for (int i = 0; i < size; i++)
    {
      temp[i]->print();
    }

    delete[] temp;
  }

  Ticket *peekFront()
  {
    if (isEmpty())
      return NULL;
    return heap[0];
  }

  Ticket *peekBack()
  {
    if (isEmpty())
      return NULL;

    // Find the ticket with the smallest priority
    Ticket *minTicket = heap[0];
    for (int i = 1; i < size; i++)
    {
      if (heap[i]->prioritylevel < minTicket->prioritylevel)
      {
        minTicket = heap[i];
      }
    }
    return minTicket;
  }
};

// COMPREHENSIVE CONFIGURATION FILING
class ConfigurationAnalysis
{
public:
  string sortHigh;
  string sortLow;
  int threshold;

  ConfigurationAnalysis()
  {
    // LOAD/EXTRACT DATA FROM FILE
    loadFromFile();
  }

  // METHOD SETS tTHRESHOLD AND UPDATES IT IN FILES
  void Thresholdsetter(int value)
  {
    threshold = value;
    writeToFile();
  }

  // SETS SORTING ALGORTHMS FOR LARGER DATA SETS
  void SortHighsetter(string value)
  {
    sortHigh = value;
    writeToFile();
  }

  // SETS SORTING ALGORTHMS FOR SMALLER DATA SETS
  void SortLowsetter(string value)
  {
    sortLow = value;
    writeToFile();
  }

  // SHOWS SET CONFIGURATOIN
  void display()
  {
    cout << "==================================================" << endl;
    cout << "    DISPLAYING SORTING CONFIGURATION & ANALYSIS" << endl;
    cout << "==================================================" << endl;
    cout << "Above THRESHOLD Sort: " << sortHigh << endl;
    cout << "Below THRESHOLD Sort: " << sortLow << endl;
    cout << "Threshold: " << threshold << endl;
  }

  // SETS DEFAULT CONFIGURATION AS IN METHOD BODY
  void setterDefaultData()
  {
    sortHigh = "quick";
    sortLow = "insertion";
    threshold = 10;
  }

  // WRITES CONFIGURATION DATA TO FILE
  void writeToFile()
  {
    ofstream configurationFile("ONESTOPMS.conf");
    configurationFile << sortHigh << endl;
    configurationFile << sortLow << endl;
    configurationFile << threshold << endl;
  }

  // LOADS CONFUIGURATION DATA FROM FILE
  void loadFromFile()
  {
    ifstream configurationFile("ONESTOPMS.conf");
    if (!configurationFile)
    {
      setterDefaultData();
      writeToFile();
      return;
    }

    string rawThreshold;
    getline(configurationFile, sortHigh);
    getline(configurationFile, sortLow);
    getline(configurationFile, rawThreshold);
    threshold = stoi(rawThreshold);
  }
};

// =============================================
// HASH TABLE IMPLEMENTATIONS
// =============================================

// Hash table for ticket ID lookups (integer keys)
class HashTableByID
{
private:
  struct Node
  {
    int key;
    Ticket *value;
    Node *next;
    Node(int k, Ticket *v) : key(k), value(v), next(nullptr) {}
  };
  static const int TABLE_SIZE = 100000; // Size of hash table
  Node **table;                         // Array of pointers to Nodes

public:
  HashTableByID()
  {
    table = new Node *[TABLE_SIZE](); // Initialize to nullptr
  }

  ~HashTableByID()
  {
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
      Node *current = table[i];
      while (current)
      {
        Node *temp = current;
        current = current->next;
        delete temp;
      }
    }
    delete[] table;
  }

  // Hash function for integer keys
  int hashFunction(int key)
  {
    return key % TABLE_SIZE;
  }

  // Insert key-value pair
  void insert(int key, Ticket *value)
  {
    int index = hashFunction(key);
    Node *newNode = new Node(key, value);
    // Insert at head of chain
    newNode->next = table[index];
    table[index] = newNode;
  }

  // Search by key
  Ticket *search(int key)
  {
    int index = hashFunction(key);
    Node *current = table[index];
    while (current)
    {
      if (current->key == key)
      {
        return current->value;
      }
      current = current->next;
    }
    return nullptr;
  }
};

// Hash table for customer name lookups (string keys)
class HashTableByName
{
private:
  struct Node
  {
    string key;
    Ticket *value;
    Node *next;
    Node(string k, Ticket *v) : key(k), value(v), next(nullptr) {}
  };
  static const int TABLE_SIZE = 100000; // Size of hash table
  Node **table;                         // Array of pointers to Nodes

public:
  HashTableByName()
  {
    table = new Node *[TABLE_SIZE](); // Initialize to nullptr
  }

  ~HashTableByName()
  {
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
      Node *current = table[i];
      while (current)
      {
        Node *temp = current;
        current = current->next;
        delete temp;
      }
    }
    delete[] table;
  }

  // Hash function for string keys
  int hashFunction(string key)
  {
    int hash = 0;
    for (char c : key)
    {
      hash = (hash * 31 + c) % TABLE_SIZE;
    }
    return hash % TABLE_SIZE;
  }

  // Insert key-value pair
  void insert(string key, Ticket *value)
  {
    int index = hashFunction(key);
    Node *newNode = new Node(key, value);
    // Insert at head of chain
    newNode->next = table[index];
    table[index] = newNode;
  }

  // Search by key
  Ticket *search(string key)
  {
    int index = hashFunction(key);
    Node *current = table[index];
    while (current)
    {
      if (current->key == key)
      {
        return current->value;
      }
      current = current->next;
    }
    return nullptr;
  }
};

// CLASS ONE STOP MANAGEMENT SYSYEM (DIRECTLY CALLED IN INT MAIN)
class OneStopManagementSystem
{
private:
public:
  // SEPERATE CLASS INTEGRATION INTO ONESTOPTICKETMANAGEMENT (CALLED IN INT MAIN)
  TicketLinkedList allTickets;
  AgentDynamicArray agents;
  ResolutionLogisticsStack resolvedTickets;
  ConfigurationAnalysis configuration;

  // PENDINGTICKETSQUEUE INSTANTIATION / FORWARD DECLARATION
  PendingTicketsQueue pendingTicketsIT;
  PendingTicketsQueue pendingTicketsAdmin;
  PendingTicketsQueue pendingTicketsAccounts;
  PendingTicketsQueue pendingTicketsAcademics;

  // ADDED: Hash tables for O(1) lookups
  HashTableByID hashTableByID;
  HashTableByName hashTableByName;

  AVLTree avlTree;

  // METHOD TO DISPLAY USER INTERFACE
  // METHOD to BOOT the MANAGEMENT SYSTEM & USER interaction
  void run()
  {
    bool running = true;
    int option;
    while (running)
    {
      // MAIN MENU
      cout << " ========================================" << endl;
      cout << " ||  ONE STOP MANAGEMENT SYSTEM        ||" << endl;
      cout << " ||                                    ||" << endl;
      cout << " ||  OWNED AND OPERATED BY FAST-NUCES  ||" << endl;
      cout << " ========================================" << endl;
      cout << endl;
      cout << "Welcome to the One Stop Ticket Management System!" << endl;
      cout << "This system manages service requests, agents, and ticket resolutions." << endl;
      cout << endl;
      cout << "----------------------!! WARNING !!----------------------------" << endl;
      cout << "CAREFULLY READ THE PROGRAM TUTORIAL & INSTRUCTIONS TO START: " << endl
           << endl;
      cout << "Below are given instructions listed specifically for system operator: " << endl
           << endl;
      cout << "[0] Exit System - Quit the application." << endl;
      cout << "[1] ADMIN MENU - Agent Management - Manage agents assigned to tickets. " << endl;
      cout << "[2] USER MENU - Ticket Management - Manage service requests and tickets." << endl;
      cout << endl;
      cout << "Input number of listed choice above: ";
      cin >> option;
      string agentName, agentType, inputStr, TicketholderNameInput, ticketDetailsInput;
      int inputInt;
      switch (option)
      {
      case 0:
        // EXIT
        running = false;
        break;
      case 1:;
        // ADMIN MENU COMMAND LINES
        cout << "========================================" << endl;
        cout << "        COMPREHENSIVE ADMIN MENU" << endl;
        cout << "========================================" << endl;
        cout << "[0] EXIT" << endl;
        cout << "[1] Add an Agent" << endl;
        cout << "[2] Assign Ticket to an Agent" << endl;
        cout << "[3] Resolve a Ticket" << endl;
        cout << "[4] Print Tickets with Status (Open/Closed)" << endl;
        cout << "[5] Print Configuration" << endl;
        cout << "[6] Alter Configuration" << endl;
        cout << "[7] Search Ticket with Analysis Report" << endl;
        cout << "[8] (Print/Display) Queue" << endl;
        cout << "[9] (Print/Display) Resolved Tickets" << endl;
        cout << "[10] Show Agents Summary" << endl;
        cout << "[11] Sort Agents with Analysis Report" << endl;
        cout << "[12] Sort Tickets with Analysis Report" << endl;
        cout << endl;
        cout << "Enter your Choice: ";

        cin >> option;
        switch (option)
        {
        case 0:
          // END PROGRAM
          running = false;
          break;
        case 1:
          // ADD AGENT TO SYSTEM
          cout << "-> Input Agent name: ";
          cin >> agentName;

          cout << "-> Input Agent type (IT / Admin / Accounts / Academics) :";
          cin >> agentType;
          addNewAgent(agentName, agentType);

          cout << "[+] Agent SUCCESSFULLY added." << endl;
          char c;
          cout << "Enter any key to resume program...";
          getchar();
          getchar();
          cout << endl;
          break;
        case 2:
          // ASSIGN TICKETS TO AGENT
          assignTickets();
          cout << "[+] Summary Of Assigned tickets to ALL AVAILABLE agents." << endl;
          cout << "Enter any key to resume program...";
          getchar();
          getchar();
          cout << endl;
          break;
        case 3:
          // RESOLVE TICKETS
          cout << "[+] Attempting to resolve a ticket assigned to agents." << endl;
          resolveTicket();
          cout << "Enter any key to resume program...";
          getchar();
          getchar();
          cout << endl;
          break;
        case 4:
          cout << "[0] Quit" << endl;
          cout << "[1] Show ALL OPEN Tickets" << endl;
          cout << "[2] Show ALL CLOSED Tickets" << endl;
          cout << "[3] Show ALL Tickets (OPEN & CLOSED)" << endl;
          cout << endl;
          cout << "Input your choice: ";
          cin >> option;
          switch (option)
          {
          case 0:
            running = false;
            break;
          case 1:
            allTickets.displayOpenTickets();
            break;
          case 2:
            allTickets.displayClosedTickets();
            break;
          case 3:
            allTickets.print();
            break;
          default:
            break;
          }
          cout << "Enter any key to resume program...";
          getchar();
          getchar();
          cout << endl;
          break;
          break;
        case 5:
          configuration.display();
          break;
        case 6:
          cout << "[0] EXIT" << endl;
          cout << "[1] Set Threshold" << endl;
          cout << "[2] Set High Sorting Algorithm" << endl;
          cout << "[3] Set Low Sorting Algorithm" << endl;
          cout << endl;
          cout << "Input your option: ";
          cin >> option;
          switch (option)
          {
          case 0:
            running = false;
            break;
          case 1:
            cout << "-> Enter new threshold: ";
            cin >> inputInt;
            configuration.Thresholdsetter(inputInt);
            break;
          case 2:
            cout << " --- BUBBLE SORT ---" << endl;
            cout << " --- INSERTION SORT ---" << endl;
            cout << " --- SELECTION SORT ---" << endl;
            cout << " --- QUICK SORT ---" << endl;
            cout << " --- MERGE SORT ---" << endl;
            cout << " -> CHOOSE ANY ONE FROM the above algorithm(s): ";
            cin >> inputStr;
            configuration.SortHighsetter(inputStr);
            break;
          case 3:
            cout << " --- BUBBLE SORT ---" << endl;
            cout << " --- INSERTION SORT ---" << endl;
            cout << " --- SELECTION SORT ---" << endl;
            cout << " --- QUICK SORT ---" << endl;
            cout << " --- MERGE SORT ---" << endl;
            cout << " -> CHOOSE ANY ONE FROM the above algorithm(s): ";
            cin >> inputStr;
            configuration.SortLowsetter(inputStr);
            break;
          default:
            break;
          }
          break;
        case 7:
          cout << "Input your restrictions for searching: " << endl;
          cout << "[0] Search by ID" << endl;
          cout << "[1] Search by Customer Name" << endl;
          cout << "[2] Search by ID Range" << endl; // NEW OPTION
          cout << endl;
          cout << "Your choice: ";
          cin >> option;

          if (option == 0)
          {
            // Search by ID
            cout << "Enter Ticket ID: ";
            cin >> inputInt;
            searchTicketByID(inputInt);
          }
          else if (option == 1)
          {
            // Search by Name
            cout << "Enter Customer Name: ";
            cin >> inputStr;
            searchTicketByName(inputStr);
          }
          else if (option == 2)
          {
            // NEW: Search by ID Range
            int lowID, highID;
            cout << "Enter starting ID (low range): ";
            cin >> lowID;
            cout << "Enter ending ID (high range): ";
            cin >> highID;

            if (lowID > highID)
            {
              cout << "[X] Invalid range! Starting ID must be less than ending ID." << endl;
            }
            else
            {
              cout << "======================================" << endl;
              cout << "   TICKETS IN RANGE [" << lowID << "-" << highID << "]" << endl;
              cout << "======================================" << endl;

              // Use AVL tree for range query
              avlTree.queryRange(lowID, highID);

              cout << "--------------------------------------" << endl;
              cout << "Found all tickets in specified range." << endl;
            }
          }
          else
          {
            cout << "[X] Invalid search option!" << endl;
          }

          cout << "Enter any key to resume program...";
          getchar();
          getchar();
          cout << endl;
          break;
        case 8:
          cout << "[0] IT" << endl;
          cout << "[1] Admin" << endl;
          cout << "[2] Accounts" << endl;
          cout << "[3] Academics" << endl;
          cout << endl;
          cout << "Input type from above:" << endl;
          cin >> option;
          switch (option)
          {
          case 0:
            pendingTicketsIT.displayQueue();
            break;
          case 1:
            pendingTicketsAdmin.displayQueue();
            break;
          case 2:
            pendingTicketsAccounts.displayQueue();
            break;
          case 3:
            pendingTicketsAcademics.displayQueue();
            break;
          default:
            break;
          }
          break;
        case 9:
          resolvedTickets.print();
          break;
        case 10:
          agents.print();
          break;
        case 11:
          sortAgents();
          cout << "[+] Agents SUCCESSFULLY Sorted." << endl;
          break;
        case 12:
          cout << "Input your restrictions for sorting" << endl;
          cout << "[0] prioritylevel" << endl;
          cout << "[1] Creation Time" << endl;
          cout << "[2] Customer Name" << endl;
          cout << endl;
          cout << "Input your choice: ";
          cin >> option;
          switch (option)
          {
          case 0:
            sortTickets("prioritylevel");
            break;
          case 1:
            sortTickets("creationTime");
            break;
          case 2:
            sortTickets("TicketholderName");
            break;
          default:
            break;
          }
          cout << "[+] Tickets SUCCESSFULLY Sorted." << endl;
          break;
        default:
          break;
        }
        break;
      case 2:
        // USER MENU
        cout << "[0] Quit" << endl;
        cout << "[1] Add Ticket" << endl;
        cout << "[2] Search Ticket" << endl;
        cout << "[3] See ALL Tickets Pending" << endl;
        cout << endl;
        cout << "Your option: ";
        cin >> option;
        switch (option)
        {
        case 0:
          running = false;
          break;
        case 1:
          cout << "-> Enter Customer Name: ";
          cin >> TicketholderNameInput;

          cout << "-> Enter Ticket prioritylevel: ";
          cin >> inputInt;

          cout << "-> Enter ticket type/description (IT, Admin, Accounts, Academics): ";
          cin >> ticketDetailsInput;

          inputInt = addNewTicket(TicketholderNameInput, inputInt, ticketDetailsInput);
          cout << "[+] Ticket SUCCESSFULLY added." << endl;
          cout << "[+] Your Ticket ID: " << inputInt << endl;
          char c;
          cout << "Enter any key to resume program...";
          getchar();
          getchar();
          cout << endl;
          break;
        case 2:
          cout << "Input your restrictions for searching: " << endl;
          cout << "[0] Search by ID" << endl;
          cout << "[1] Search by Customer Name" << endl;
          cout << "[2] Search by ID Range" << endl; // NEW OPTION
          cout << endl;
          cout << "Input your choice: ";
          cin >> option;

          if (option == 0)
          {
            cout << "Enter ID: ";
            cin >> inputInt;
            searchTicketByID(inputInt);
          }
          else if (option == 1)
          {
            cout << "Enter Customer Name: ";
            cin >> inputStr;
            searchTicketByName(inputStr);
          }
          else if (option == 2)
          {
            // NEW: Search by ID Range
            int lowID, highID;
            cout << "Enter starting ID (low range): ";
            cin >> lowID;
            cout << "Enter ending ID (high range): ";
            cin >> highID;

            if (lowID > highID)
            {
              cout << "[X] Invalid range! Starting ID must be less than ending ID." << endl;
            }
            else
            {
              cout << "======================================" << endl;
              cout << "   YOUR TICKETS IN RANGE [" << lowID << "-" << highID << "]" << endl;
              cout << "======================================" << endl;

              // Use AVL tree for range query
              avlTree.queryRange(lowID, highID);

              cout << "--------------------------------------" << endl;
              cout << "Found all your tickets in specified range." << endl;
            }
          }
          else
          {
            cout << "[X] Invalid search option!" << endl;
          }
          break;
        case 3:
          cout << "[0] IT" << endl;
          cout << "[1] Admin" << endl;
          cout << "[2] Accounts" << endl;
          cout << "[3] Academics" << endl;
          cout << endl;
          cout << "Select a type:";
          cin >> option;
          switch (option)
          {
          case 0:
            pendingTicketsIT.displayQueue();
            break;
          case 1:
            pendingTicketsAdmin.displayQueue();
            break;
          case 2:
            pendingTicketsAccounts.displayQueue();
            break;
          case 3:
            pendingTicketsAcademics.displayQueue();
            break;
          default:
            break;
          }
          break;
        default:
          break;
        }
        break;
      default:
        break;
      }
    }
  }

  // METHOD ASSIGNING MOST URGENT TICKET FROM ALL TYPES TO ALL AVAILABLE AGENTS
  void assignTickets()
  {
    cout << "======================" << endl;
    cout << "  TICKET ASSIGNMENT   " << endl;
    cout << "======================" << endl;

    // IT Department
    Ticket *pendingTicket = pendingTicketsIT.peekFront();
    if (pendingTicket != NULL)
    {
      bool assigned = agents.assignTickets(pendingTicket);
      if (assigned)
      {
        pendingTicketsIT.dequeueTicket();
      }
    }
    else
    {
      cout << "[i] No pending IT tickets" << endl;
    }

    // Admin Department
    pendingTicket = pendingTicketsAdmin.peekFront();
    if (pendingTicket != NULL)
    {
      bool assigned = agents.assignTickets(pendingTicket);
      if (assigned)
      {
        pendingTicketsAdmin.dequeueTicket();
      }
    }
    else
    {
      cout << "[i] No pending Admin tickets" << endl;
    }

    // Accounts Department
    pendingTicket = pendingTicketsAccounts.peekFront();
    if (pendingTicket != NULL)
    {
      bool assigned = agents.assignTickets(pendingTicket);
      if (assigned)
      {
        pendingTicketsAccounts.dequeueTicket();
      }
    }
    else
    {
      cout << "[i] No pending Accounts tickets" << endl;
    }

    // Academics Department
    pendingTicket = pendingTicketsAcademics.peekFront();
    if (pendingTicket != NULL)
    {
      bool assigned = agents.assignTickets(pendingTicket);
      if (assigned)
      {
        pendingTicketsAcademics.dequeueTicket();
      }
    }
    else
    {
      cout << "[i] No pending Academics tickets" << endl;
    }
  }

  // METHOD THAT RESOLVES TICKETS EVERYTIME IT'S CALLED
  void resolveTicket()
  {
    Ticket *resolvedTicket = agents.resolveTicket();
    if (resolvedTicket == NULL)
    {
      cout << "[X] NO tickets to resolve." << endl;
      return;
    }
    resolvedTicket->close();
    resolvedTickets.addTicket(resolvedTicket);
  }

  // METHOD THAT ADDS A NEW TICKET TO THE SYSTEM
  int addNewTicket(string TicketholderName, int prioritylevel, string requestDescription)
  {
    Ticket *newTicket = new Ticket(TicketholderName, prioritylevel, requestDescription);
    allTickets.addTicket(newTicket);

    // ADDED: Insert into hash tables
    hashTableByID.insert(newTicket->ticketholderid, newTicket);
    hashTableByName.insert(newTicket->TicketholderName, newTicket);

    // ADDED: Insert into AVL tree
    avlTree.insert(newTicket->ticketholderid, newTicket);

    if (requestDescription == "IT")
    {
      pendingTicketsIT.enqueueTicket(newTicket);
    }
    else if (requestDescription == "Admin")
    {
      pendingTicketsAdmin.enqueueTicket(newTicket);
    }
    else if (requestDescription == "Accounts")
    {
      pendingTicketsAccounts.enqueueTicket(newTicket);
    }
    else if (requestDescription == "Academics")
    {
      pendingTicketsAcademics.enqueueTicket(newTicket);
    }
    return newTicket->ticketholderid;
  }

  // ADDED: Range query method
  void queryTicketsByIDRange(int low, int high)
  {
    cout << "======================================" << endl;
    cout << "   TICKETS IN RANGE [" << low << "-" << high << "]" << endl;
    cout << "======================================" << endl;
    avlTree.queryRange(low, high);
  }

  // METHOD THAT ADDS A NEW AGENT TO THE SYSTEM (DIRECTLY CALLED IN INT MAIN) AFTER OSMS INSTANTIATION
  void addNewAgent(string name, string type)
  {
    Agent *newAgent = new Agent(name, type);
    agents.addAgent(newAgent);
  }

  // SEARCH FOR A TICKET BY IT'S ID
  void searchTicketByID(int id)
  {
    // MODIFIED: Use hash table instead of linear search
    Ticket *ticket = hashTableByID.search(id);
    if (ticket)
    {
      cout << "[+] YOUR SEARCHED BY ID/REQUESTED TICKET." << endl;
      ticket->print();
    }
    else
    {
      cout << "[X] Ticket not found!" << endl;
    }
  }

  // SEARCH FOR A TICKET BY IT'S CUSTOMER NAME
  void searchTicketByName(string TicketholderName)
  {
    // MODIFIED: Use hash table instead of linear search
    Ticket *ticket = hashTableByName.search(TicketholderName);
    if (ticket)
    {
      cout << "[+] YOUR SEARCHED BY NAME/REQUESTED TICKET." << endl;
      ticket->print();
    }
    else
    {
      cout << "[X] Ticket not found!" << endl;
    }
  }

  // DISPLAY ALL TICKETS ENTERED INTO SYSTEM FROM THE START
  void displayAllTickets()
  {
    cout << "======================================" << endl;
    cout << "||      SHOWING (ALL) Tickets       ||" << endl;
    cout << "======================================" << endl;
    allTickets.print();
  }

  // DISPLAY ALL OPEN TICKETS ENTERED INTO SYSTEM FROM THE START
  void displayAllOpenTickets()
  {
    cout << "======================================" << endl;
    cout << "||     SHOWING ALL (OPEN) Tickets   ||" << endl;
    cout << "======================================" << endl;
    allTickets.displayOpenTickets();
  }

  // DISPLAY ALL CLOSED TICKETS ENTERED INTO SYSTEM FROM THE START
  void displayAllClosedTickets()
  {
    cout << "======================================" << endl;
    cout << "||   SHOWING ALL (CLOSED) Tickets   ||" << endl;
    cout << "======================================" << endl;
    allTickets.displayClosedTickets();
  }

  // SORT AGENTS RESTRICTED AS PER CONFIGURATION DATA SET
  void sortAgents()
  {
    string sortAlgorithm;
    if (agents.length <= configuration.threshold)
    {
      sortAlgorithm = configuration.sortLow;
    }
    else
    {
      sortAlgorithm = configuration.sortHigh;
    }

    chrono::_V2::system_clock::time_point sortStart = chrono::_V2::system_clock::now();

    if (sortAlgorithm == "bubble")
    {
      agents.bubbleSort();
    }
    else if (sortAlgorithm == "insertion")
    {
      agents.insertionSort();
    }
    else if (sortAlgorithm == "selection")
    {
      agents.selectionSort();
    }
    else if (sortAlgorithm == "quick")
    {
      agents.quickSort();
    }
    else if (sortAlgorithm == "merge")
    {
      agents.mergeSort();
    }

    chrono::_V2::system_clock::time_point sortEnd = chrono::_V2::system_clock::now();

    cout << "--> Agents Sorting Summary" << endl;
    cout << "Algorithm: " << sortAlgorithm << endl;
    cout << "n: " << agents.length << endl;
    cout << "Time Interval (milliseconds): " << difftime(system_clock::to_time_t(sortEnd), system_clock::to_time_t(sortStart)) << endl;
  }

  // SORT TICKETS RESTRICTED AS PER CONFIGURATION DATA SET
  void sortTickets(string sortBy)
  {
    string sortAlgorithm;
    if (allTickets.length <= configuration.threshold)
    {
      sortAlgorithm = configuration.sortLow;
    }
    else
    {
      sortAlgorithm = configuration.sortHigh;
    }

    chrono::_V2::system_clock::time_point sortStart = chrono::_V2::system_clock::now();

    if (sortAlgorithm == "bubble")
    {
      allTickets.bubbleSort(sortBy);
    }
    else if (sortAlgorithm == "insertion")
    {
      allTickets.insertionSort(sortBy);
    }
    else if (sortAlgorithm == "selection")
    {
      allTickets.selectionSort(sortBy);
    }
    else if (sortAlgorithm == "quick")
    {
      allTickets.quickSort(sortBy);
    }
    else if (sortAlgorithm == "merge")
    {
      allTickets.mergeSort(sortBy);
    }

    chrono::_V2::system_clock::time_point sortEnd = chrono::_V2::system_clock::now();

    cout << "--> Ticket Sorting Summary" << endl;
    cout << "Sorting Algorithm: " << sortAlgorithm << endl;
    cout << "n: " << allTickets.length << endl;
    cout << "Time Interval (milliseconds): " << system_clock::to_time_t(sortEnd) - system_clock::to_time_t(sortStart) << endl;
  }
};
// INT MAIN/DRIVER CODE
int main()
{
  // CTIME LIBRARY SRAND METHOD
  srand(time(NULL));
  OneStopManagementSystem OSMS;
  // ADDING AGENTS MAIN AS PER REQUIREMENT 2
  OSMS.addNewAgent("Bruce", "IT");
  OSMS.addNewAgent("Clark", "Admin");
  OSMS.addNewAgent("Diana", "Accounts");
  OSMS.addNewAgent("Carol", "IT");
  OSMS.addNewAgent("Alice", "Accounts");
  OSMS.addNewAgent("Sarah", "IT");
  OSMS.addNewAgent("Hal", "Admin");
  OSMS.addNewAgent("Alex", "IT");
  OSMS.addNewAgent("Rovers", "Admin");
  OSMS.addNewAgent("Turner", "Accounts");
  OSMS.addNewAgent("Duela", "Academics");
  OSMS.addNewAgent("Harper", "Academics");
  OSMS.addNewAgent("Cullen", "Academics");
  // INITIATING PROGRAM RUN -- USER INTERACTION.
  OSMS.run();
  exit(0);
}
