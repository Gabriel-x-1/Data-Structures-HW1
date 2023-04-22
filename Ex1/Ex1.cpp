#include <iostream>
#include <fstream>
#include "stack.h"

using namespace std;

int search(Stack<int> b, int n, int cond) // function to search for Homer in the stack of inputs , while checking the condition for finding homer given by the "cond" parameter
{
    int sum[n] = {0}; // vector for sums
    int x = 0;        // local var for the sum of the elements on the row
    int counter = 0;
    int i = 0;
    while (i < b.getTopLevel() + 2) // traveling the stack
    {
        if (i % n == 0 && i != 0) // travel each row and adding their sum to a different index of the vector
        {
            sum[counter] = x;
            counter++;
            x = 0; // resetting the sum
        }
        if (i == n * n - 1) // check if last row ends at 35; so that it paths from 0 to 35 which is O(n) complexity for n=36
        {
            sum[counter] = x;
            counter++;
            x = 0;
        }
        x = x + b.pop(); // calculating the sum using the stack's function
        i++;
    }
    for (i = 0; i < 6; i++) // checks for the row that verifies the condition of finding Homer and returns it
    {
        if (sum[i] == cond)
        {
            return n - i - 1;
        }
    }
    return -1; // returns -1 if we can't find homer
}

ifstream f("input.txt"); // file input stream

int main()
{
    int n;
    int i, j;
    f >> n; // reading the number of rows and cols from the file
    int a[n][n]; // initializing the matrix of people and who they talked to

    // read the matrix
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            f >> a[i][j]; // saving the values from the file into the matrix
        }
    }
    // create stacks
    Stack<int> rows;
    Stack<int> cols;

    // put data into the first stack
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (i == j)
                rows.push(0); // initializing the first diagonal with 0 (so that no one talks to themselves) to avoid a code error
            else
                rows.push(a[i][j]); // pushing the values into the stack
        }
    }

    // put data into the second stack
    for (j = 0; j < n; j++)
    {
        for (i = 0; i < n; i++)
        {
            if (i == j)
                cols.push(0); // initializing the first diagonal with 0 (so that no one talks to themselves) to avoid a code error
            else
                cols.push(a[i][j]); // pushing the values into the stack
        }
    }

    cout << endl;

    // verify the conditions and output the text
    if (search(rows, n, 0) == search(cols, n, n - 1) && search(rows, n, 0) != -1)
    {
        cout << "Homer is the person with id: " << search(rows, n, 0);
    }
    else
        cout << "Everyone has talked to at least one other person. Homer is not present";

    f.close();
    return 0;
}
