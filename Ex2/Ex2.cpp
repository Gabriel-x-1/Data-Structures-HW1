#include <iostream>
#include <fstream>
#include "queue1.h"

using namespace std;
class Order
{
public:
    int t; // arrival time of the order
    int d; // duration to prepare the order
    Order(int t, int d)
    {
        this->d = d;
        this->t = t;
    }
    Order()
    {
        this->d = 0;
        this->t = 0;
    }
    ~Order()
    {
    }
};

ifstream f("input.txt"); // declaring file input stream

int main()
{

    Queue<Order> queue;               // initializing the queue
    int time = 0, N, T, ordermax = 0; // initializing the variables ; N is the number of orders and T is the closing time for the restaurant ; time is used to determine the current time
    f >> N >> T;                      // reading the values for N and T
    for (int i = 0; i < N; i++)       // generating all orders
    {
        int x, y;
        f >> x >> y; // reads the time the order was placed at in "x" and the time required for its completion in "y"
        Order b = Order(x, y);
        queue.enqueue(b); // adding all orders to the queue
    }
    // processing the first order
    Order aux;       // initializing an auxiliary variable of type "Order"
    int counter = 0; // counter for the orders
    ordermax = 0;    // initializing the value for the longest to make order

    do // processing the orders while the queue is not empty
    {
        aux = queue.peek(); // saving the first order of the queue to aux
        queue.dequeue();    // removing the first element of the queue
        if (aux.t < T)      // processing only orders placed before the closing time
        {
            if (aux.t > time) // if the time the order was placed at exceeds the current time
            {
                counter++;
                int time_aux = time;          // variable to save current time
                time += aux.t - time + aux.d; // computing the time till the order starts
                if (time <= T)                // if the order was placed before closing time or when the restaurant closes
                    cout << "The chef takes a break (the queue is empty) from: " << time_aux << " to " << aux.t << endl;
                cout << "Order " << counter << ": expected completion time = " << aux.t << ", actual completion time = " << time << endl;
            }
            else // if the time of the order doesnt exceed the current time
            {
                counter++;
                time += aux.d; // we add the completion time to the current
                // we check if the order completion time is smaller than the closing time
                if (time < T)
                    cout << "Order " << counter << ": expected completion time = " << aux.t << ", actual completion time = " << time << endl;
                // in case that it is higher we explain that it is finished after the closing time
                else
                    cout << "Order " << counter << ": expected completion time = " << aux.t << ", actual completion time = " << time << "   <--- was completed after the closing time " << endl;
            }
        }
        if (aux.d > ordermax) // checking for maximum completion time
            ordermax = aux.d;
    } while (!queue.isEmpty());
    cout << endl
         << "The max time to complete an order was : " << ordermax;
    f.close(); // closing the file input stream
    return 0;
}