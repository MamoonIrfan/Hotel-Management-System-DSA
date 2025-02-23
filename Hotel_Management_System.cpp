// Project Name: Hotel Management system:

#include <iostream>
#include <ctime>
using namespace std;

// User defined structure base data type for room:
struct Room
{
    int Room_Id;
    string type;
    string status;
    time_t booking_date; // Stores the time when the room was booked
    Room *Next;
};

// User defined structure base data type for Floor that hold  pointer type object of Room structure:
// it will use for move one room to another on linklist:
struct FloorNode
{
    int Floor_Number;
    Room *room;
    FloorNode *Next_Floor;
};
// User defined structure base data type for Booking Request:
struct Booking_Request
{
    string Customer_Name;
    string Room_Type;
    int Night;
    bool High_Priority;
};
// it will use for move one request to next one in queue.
// it hold the object of Booking request.
struct QueueNode
{
    Booking_Request Request;
    QueueNode *Next;
};
// User defined structure base data type for Booking Record:
struct Booking_Record
{
    string Customer_Name;
    int Room_ID;
    int Floor_Number;
    int Nights;
};
// it will use for move one record to next one in stack.
// it hold the object of Booking recrod.
struct StackNode
{
    Booking_Record Record;
    StackNode *Next;
};
// This class will control Regular Requests.
class RegularQueue
{
private:
    QueueNode *Regular_Front;
    QueueNode *Regular_Rear;

public:
    RegularQueue()
    {
        Regular_Front = nullptr;
        Regular_Rear = nullptr;
    }
    bool Is_Empty()
    {
        if (Regular_Front == nullptr && Regular_Rear == nullptr)
        {
            return true;
        }
        return false;
    }
    void Enqueue(Booking_Request request)
    {
        QueueNode *new_node = new QueueNode;
        new_node->Request = request;
        new_node->Next = nullptr;
        if (Is_Empty())
        {
            Regular_Front = new_node;
            Regular_Rear = new_node;
        }
        else
        {
            Regular_Rear->Next = new_node;
            Regular_Rear = new_node;
        }
    }

    Booking_Request Dequeue()
    {
        if (Is_Empty())
        {
            cout << "Queue is empty. Cannot dequeue.";
        }
        else
        {
            if (Regular_Front == Regular_Rear)
            {
                QueueNode *temp;
                temp = Regular_Front;
                Regular_Front = Regular_Rear = nullptr;
                Booking_Request request = temp->Request;
                delete temp;
                return request;
            }
            else
            {
                QueueNode *temp;
                temp = Regular_Front;
                Regular_Front = Regular_Front->Next;
                Booking_Request request = temp->Request;
                delete temp;
                return request;
            }
        }
    }
    int Size_of_regular_request()
    {
        QueueNode *temp = Regular_Front;
        int size = 0;
        while (temp != nullptr)
        {
            temp = temp->Next;
            size++;
        }
        // cout << "size:"<<size << endl;
        return size;
    }
};
// This class will control Priority Requests.
class PriorityQueue
{
private:
    QueueNode *Priority_Front;
    QueueNode *Priority_Rear;

public:
    PriorityQueue()
    {
        Priority_Front = nullptr;
        Priority_Rear = nullptr;
    }
    bool Is_Empty()
    {
        if (Priority_Front == nullptr && Priority_Rear == nullptr)
        {
            return true;
        }
        return false;
    }
    void Enqueue(Booking_Request request)
    {
        QueueNode *new_node = new QueueNode;
        new_node->Request = request;
        new_node->Next = nullptr;
        if (Is_Empty())
        {
            Priority_Front = new_node;
            Priority_Rear = new_node;
        }
        else
        {
            Priority_Rear->Next = new_node;
            Priority_Rear = new_node;
        }
    }

    Booking_Request Dequeue()
    {
        if (Is_Empty())
        {
            cout << "Queue is Empty." << endl;
        }
        else
        {
            QueueNode *temp = Priority_Front;
            if (Priority_Front == Priority_Rear)
            {
                Priority_Front = Priority_Rear = nullptr;
                Booking_Request request = temp->Request;
                delete temp;
                return request;
            }
            else
            {
                Priority_Front = Priority_Front->Next;
                Booking_Request request = temp->Request;
                delete temp;
                return request;
            }
        }
    }

    int Size_of_priority_request()
    {
        QueueNode *temp = Priority_Front;
        int size = 0;
        while (temp != nullptr)
        {
            temp = temp->Next;
            size++;
        }
        // cout << "size:"<<size << endl;
        return size;
    }
};
// After processing the requests either it is regular or priority store in the stack for history purposes.
class Booking_Stack
{
private:
    StackNode *Top;

public:
    Booking_Stack()
    {
        Top = nullptr;
    }
    bool Is_Empty()
    {
        if (Top == nullptr)
        {
            return true;
        }
        return false;
    }
    void Is_Push(Booking_Record Record)
    {
        StackNode *new_node = new StackNode;
        new_node->Record = Record;
        new_node->Next = nullptr;
        if (Top == nullptr)
        {
            Top = new_node;
        }
        else
        {
            new_node->Next = Top;
            Top = new_node;
        }
    }
    Booking_Record Is_Pop()
    {
        if (Is_Empty())
        {
            cout << "Stack Is Empty";
        }
        else
        {
            StackNode *temp = Top;
            Booking_Record record = Top->Record;
            Top = Top->Next;
            delete temp;
            return record;
        }
    }
    void Display()
    {
        StackNode *Current = Top;
        while (Current != nullptr)
        {
            cout << "Customer: " << Current->Record.Customer_Name << ", Room ID: " << Current->Record.Room_ID << ", Floor: " << Current->Record.Floor_Number << ", Nights: " << Current->Record.Nights << endl;
            Current = Current->Next;
        }
    }
};

class Galaxy_Hotel
{
private:
    FloorNode *Floor_Head;
    FloorNode *Floor_Tail;
    RegularQueue regularQueue;    // Regular booking requests
    PriorityQueue priorityQueue;  // High priority requests
    Booking_Stack BookingHistory; // Booking history

    // Helper function to check if 30 days have passed since booking
    bool Is_Booking_Expired(time_t booking_date)
    {
        time_t now = time(0);
        double seconds_diff = difftime(now, booking_date);
        const int SECONDS_IN_30_DAYS = 30 * 24 * 60 * 60;
        return seconds_diff > SECONDS_IN_30_DAYS;
    }

    // Function to update room statuses
    void Update_Room_Statuses()
    {
        FloorNode *Current = Floor_Head;
        while (Current != nullptr)
        {
            Room *Room_Current = Current->room;
            while (Room_Current != nullptr)
            {
                if (Room_Current->status == "Booked" && Is_Booking_Expired(Room_Current->booking_date))
                {
                    Room_Current->status = "Ready";
                    Room_Current->booking_date = 0; // Reset booking date
                }
                Room_Current = Room_Current->Next;
            }
            Current = Current->Next_Floor;
        }
    }

public:
    Galaxy_Hotel()
    {
        Floor_Head = nullptr;
        Floor_Tail = nullptr;
    }
    // Build Floor and Room in Hotel:
    // using the link list make required number of floors in which make required number of rooms using link list:
    void Build_Hotel(int Num_of_Floor, int Num_of_Rooms)
    {
        for (int i = 1; i <= Num_of_Floor; i++)
        {
            FloorNode *New_Floor = new FloorNode;
            New_Floor->Floor_Number = i;
            New_Floor->room = nullptr;
            New_Floor->Next_Floor = nullptr;

            Room *Room_Head = nullptr;
            Room *Room_Tail = nullptr;
            for (int j = 0; j < Num_of_Rooms; j++)
            {
                Room *New_Room = new Room;
                New_Room->Room_Id = (i * 100) + (j + 1);
                New_Room->Next = nullptr;
                if ((j + 1) % 3 == 0)
                {
                    New_Room->type = "Suite";
                }
                else if ((j + 1) % 2 == 0)
                {
                    New_Room->type = "Double";
                }
                else
                {
                    New_Room->type = "Single";
                }

                New_Room->status = "Ready";
                if (Room_Head == nullptr && Room_Tail == nullptr)
                {
                    Room_Head = New_Room;
                    Room_Tail = New_Room;
                }
                else
                {
                    Room_Tail->Next = New_Room;
                    Room_Tail = New_Room;
                }
            }
            New_Floor->room = Room_Head; // Attach room list to the each floor

            if (Floor_Head == nullptr && Floor_Tail == nullptr)
            {
                Floor_Head = New_Floor;
                Floor_Tail = New_Floor;
            }
            else
            {
                Floor_Tail->Next_Floor = New_Floor;
                Floor_Tail = New_Floor;
            }
        }
    }
    // Display Floor and Rooms with status and Type:
    void Display_Hotel()
    {
        Update_Room_Statuses(); // Ensure statuses are up to date
        FloorNode *Current = Floor_Head;
        while (Current != nullptr)
        {
            cout << " Floor " << Current->Floor_Number << ":" << endl;
            Room *Room_Current = Current->room;
            while (Room_Current != nullptr)
            {
                cout << "  Room  " << Room_Current->Room_Id << ", Type: " << Room_Current->type << ", Status: " << Room_Current->status << endl;
                Room_Current = Room_Current->Next;
            }
            Current = Current->Next_Floor;
        }
    }
    // Use for Enqueue requests in Regular and Priority Queue according to user request:
    void Add_Booking_Request(string Customer_Name, string Room_Type, int Nights, bool High_Priority)
    {
        Booking_Request request{Customer_Name, Room_Type, Nights, High_Priority};
        if (request.High_Priority)
        {
            priorityQueue.Enqueue(request);
        }
        else
        {
            regularQueue.Enqueue(request);
        }
    }
    // Use for proccessing each request first it will dequeue the request from Queue and then put on the top of the stack for History Purpose:
    void processRequests()
    {
        Update_Room_Statuses(); // Update statuses before processing requests

        if ((regularQueue.Size_of_regular_request() + priorityQueue.Size_of_priority_request()) <= 10)
        {
            while (!priorityQueue.Is_Empty())
            {
                processRequest(priorityQueue.Dequeue());
            }
            while (!regularQueue.Is_Empty())
            {
                processRequest(regularQueue.Dequeue());
            }
        }
        else
        {
            cout << "Your number of reuqest must be less then ten!." << endl;
        }
    }
    // Use for linearly Search Floor and room from the hotel. If the Room is avialiable the request will push in the stack for history purposes:
    void processRequest(const Booking_Request &request)
    {
        FloorNode *Current = Floor_Head;
        while (Current != nullptr)
        {
            Room *Room_Current = Current->room;
            while (Room_Current != nullptr)
            {
                if (Room_Current->type == request.Room_Type && Room_Current->status == "Ready")
                {
                    Room_Current->status = "Booked";
                    Room_Current->booking_date = time(0); // Set booking date
                    BookingHistory.Is_Push(Booking_Record{request.Customer_Name, Room_Current->Room_Id, Current->Floor_Number, request.Night});
                    cout << "Booking successful for " << request.Customer_Name << " in Room ID " << Room_Current->Room_Id << endl;
                    return;
                }
                Room_Current = Room_Current->Next;
            }
            Current = Current->Next_Floor;
        }

        cout << "No available room for " << request.Customer_Name << endl;
    }
    // Display All the proccessed request from the stack:
    void Display_Booking_History()
    {
        BookingHistory.Display();
    }
    // Use for cancel most recent booking it will pop out the most recent booking and then change the room status:
    void Cancel_Most_Recent_Booking()
    {
        if (!BookingHistory.Is_Empty())
        {
            Booking_Record record = BookingHistory.Is_Pop();
            FloorNode *Current = Floor_Head;
            while (Current != nullptr)
            {
                Room *Room_Current = Current->room;
                while (Room_Current != nullptr)
                {
                    if (record.Room_ID == Room_Current->Room_Id)
                    {
                        Room_Current->status = "Ready";
                        Room_Current->booking_date = 0; // Reset booking date
                        cout << "Booking canceled for Room ID " << Room_Current->Room_Id << endl;
                        return;
                    }
                    Room_Current = Room_Current->Next;
                }
                Current = Current->Next_Floor;
            }
        }
        else
        {
            cout << "No bookings to cancel." << endl;
        }
    }
};

int main()
{
    Galaxy_Hotel galaxy_Hotel;
    galaxy_Hotel.Build_Hotel(5, 10);
    galaxy_Hotel.Display_Hotel();

    cout << endl
         << "Adding Booking Requests:" << endl;
    galaxy_Hotel.Add_Booking_Request("Ahmad", "Single", 3, true);
    galaxy_Hotel.Add_Booking_Request("Ali", "Suite", 2, false);
    galaxy_Hotel.Add_Booking_Request("Sara", "Double", 4, false);
    galaxy_Hotel.Add_Booking_Request("Hammad", "Single", 3, true);
    galaxy_Hotel.Add_Booking_Request("Talha", "Suite", 2, false);
    galaxy_Hotel.Add_Booking_Request("Mamoon", "Double", 4, false);

    galaxy_Hotel.processRequests();

    cout << endl
         << "Booking History:" << endl;
    galaxy_Hotel.Display_Booking_History();

    cout << endl
         << "Canceling Most Recent Booking:" << endl;
    galaxy_Hotel.Cancel_Most_Recent_Booking();

    cout << endl
         << "Booking History After Cancellation:" << endl;
    galaxy_Hotel.Display_Booking_History();

    return 0;
}
