#include <iostream>
#include <fstream>
#include "list1.h"
#include <math.h>

using namespace std;

template <typename T>
class Term
{
public:
    int exp;
    T coef;

    Term(T coef, int exp)
    {
        this->coef = coef;
        this->exp = exp;
    }

    Term()
    {
        this->coef = 0;
        this->exp = 0;
    }

    ~Term() {}

    void TermDisplay()
    {
        if (this->coef > 0) // only display terms that have positive coefficients
            cout << this->coef << "*X^" << this->exp << " ";
    }
};

template <typename T>
void valuePoly(int x, LinkedList<T> a) // calculates the sum of the polynom for a given value of X given by the "x" parameter
{
    int sum = 0;
    Node<T> *p = a.pfirst;  // we give the pointer the adress of the first term in the list
    while (p != NULL)  // while there are still terms in the list do the following
    {
        Term<int> term = p->info;   
        sum += term.coef * (pow(x, term.exp)); // calculate the sum of each term
        p = p->next; // move to the next term
    }
    cout << "The sum of the polynom when x = "<<x<<" is: " << sum; // display the sum
}

template <typename T>
LinkedList<Term<T>> sum(LinkedList<Term<T>> poly1, LinkedList<Term<T>> poly2)
{
    LinkedList<Term<T>> result;  // auxiliary list to store result
    Node<Term<T>> *p1 = poly1.pfirst;  // pointer to the first adress in the first polynom list
    Node<Term<T>> *p2 = poly2.pfirst;  // pointer to the first adress in the second polynom list
    while (p1 != nullptr && p2 != nullptr) // while there are still terms in both of the lists do the following
    {
        if (p1->info.exp > p2->info.exp) // if the first list has the higher exponent (because its degree is higher)
        {
            if (p1->info.coef != 0) // add to the result list only the non-zero terms
                result.addLast(p1->info);
            p1 = p1->next; // go to the next term of the higher exponent list
        }
        else if (p2->info.exp > p1->info.exp)
        {
            if (p2->info.coef != 0) // add to the result list only the non-zero terms
                result.addLast(p2->info);
            p2 = p2->next; // go to the next term of the higher exponent list
        }
        else
        {
            Term<T> term(p1->info.coef + p2->info.coef, p1->info.exp); // if both terms have the same coefficient
            if (term.coef != 0) // add to the result list only the non-zero terms
                result.addLast(term);

            // go to the next term of both lists
            p1 = p1->next; 
            p2 = p2->next;
        }
    }
    while (p1 != nullptr) //if there are terms only in the first list
    {
        result.addLast(p1->info); // add the term to the result list
        p1 = p1->next; // go to the next term of the list
    }
    while (p2 != nullptr) //if there are terms only in the second list
    {
        result.addLast(p2->info); // add the term to the result list
        p2 = p2->next; // go to the next term of the list
    }
    return result;
}

template <typename T>
LinkedList<Term<T>> simplify(LinkedList<Term<T>> poly) // function to help simplify the multiplication function by adding all terms with the same exponent
{
    LinkedList<Term<T>> result; // the list that we use to write the result in

    Term<T> vect[getLength(poly)]; // initializing the vector with the lenght equal to the amount of terms resulted after the multiplication
    int count = 0; // initializing the counter used to index the vector

    while (poly.isEmpty() == 0) // while there are terms in the list
    {
        if (poly.getFirst().coef != 0) // if the coefficient of the first term in the list is different from zero
            vect[count] = poly.getFirst(); // we add the first element of the list in the vector

        count++; // we increase the counter
        poly.removeFirst(); // we remove the first term in the list so that we can linearly progress the list
    }

    for (int i = 0; i < count; i++) // going through the vector from index 0 to counter-1
    {
        Term<T> aux(vect[i].coef, vect[i].exp); // initializing an auxiliary vector that has all the information from the first term of the vector

        for (int j = i + 1; j < count; j++) // checking the vector for more terms with the same coefficient but going through all indexes bigger than "i" but smaller than count
        {

            if (vect[j].coef != 0) // if coefficient of the term at index j is different from 0
            {
                if (vect[i].exp == vect[j].exp) // if both exponents of the terms at index "i" and "j" are equal
                {
                    aux.coef += vect[j].coef; // add the coefficient of term at index "j" to the auxiliary term
                    vect[j].coef = 0; // changing the coefficient of the found term to "0"; basically deleting the term
                }
            }
        }

        if (aux.coef != 0) 
            result.addLast(aux); // adding the resulting auxiliary term to the result list only if its coefficient is different from 0
    }

    return result;
}

template <typename T>
LinkedList<Term<T>> multiply(LinkedList<Term<T>> poly1, LinkedList<Term<T>> poly2) // function to multiply 2 polynoms given through lists
{
    LinkedList<Term<T>> result; // list used to return result
    Node<Term<T>> *p1 = poly1.pfirst; // giving pointer p1 the adress of the first term in the first polynom
    while (p1 != nullptr) // while there are terms in the first polynom
    {
        Node<Term<T>> *p2 = poly2.pfirst;  // giving pointer p2 the adress of the first term in the second polynom
        while (p2 != nullptr) // while there are terms in the second polynom
        {
            Term<T> term(p1->info.coef * p2->info.coef, p1->info.exp + p2->info.exp); // multiplying the terms of the polynoms
            if (term.coef != 0) // if coefficient is different from 0
                result.addLast(term); // add it to the result list
            p2 = p2->next; // go to the next element in the second polynom
        }
        p1 = p1->next; // go to the next element in the first polynom
    }

    return simplify(result); // display the simplified result
}

template <typename T>
void display(LinkedList<T> list) // function to display the polynom
{
    Node<T> *p = list.pfirst; // pointer with the value of the first element of the list
    int c = getLength(list);
    c -= getZeroCoefs(list); // counter for how many coefficients in the list are 0 
    int counter = 1; // initializing the counter for "+" signs
    while (p != NULL) // while there are terms in the list
    {
        p->info.TermDisplay(); // display their information
        if (counter < c) // if counter is smaller than length of the list
        {
            cout << " + ";
            counter++;
        }
        p = p->next; // go to the next element of the list
    }
}

template <typename T>
int getLength(LinkedList<T> list) // function to get length of the list
{
    Node<T> *p = list.pfirst;
    int counter = 0;
    while (p != NULL)
    {
        counter++;
        p = p->next;
    }
    return counter;
}

template <typename T>
int getZeroCoefs(LinkedList<T> list) // function to get how many coefficients are 0 or less 
{
    Node<T> *p = list.pfirst; // get the pointer to the first node of the list
    int counter = 0;
    while (p != NULL)
    { // traverse the list using the next pointers of each node
        if (p->info.coef == 0 || p->info.exp == -1)
            counter++;
        p = p->next;
    }
    return counter;
}

struct complex{
    float re;
    float im;
};

complex sum_complex(complex a, complex b)
{
    complex c;
    c.re=a.re+b.re;
    c.im=a.im+b.im;
    return c;
}
void display_complex(complex a)
{
    cout<<a.re<<"+"<<a.im<<"i"<<" ";
}

int main()
{
    int degree1, degree2;
    cout << "Give a degree for the first polynomial: ";
    cin >> degree1;
    cout << "Give a degree for the second polynomial: ";
    cin >> degree2;

    LinkedList<Term<int>> poly1, poly2;
    Term<int> aux;

    // creating the first polynom
    for (int i = 0; i <= degree1; i++)
    {
        cout << "Give the coefficient for the " << i << " term : ";
        cin >> aux.coef;
        aux.exp = i;
        poly1.addFirst(aux);
    }

    // creating the second polynom
    for (int i = 0; i <= degree2; i++)
    {
        cout << "Give the coefficient for the " << i << " term : ";
        cin >> aux.coef;
        aux.exp = i;
        poly2.addFirst(aux);
    }
    
    display(poly1);
    cout << endl;
    display(poly2);
    cout << endl;
    cout<<"The sum of the 2 polynoms is : "<<endl;
    display(sum(poly1, poly2));
    cout << endl;
    valuePoly(2, poly1);
    cout << endl;
    cout<<"The multiplication of the 2 polynomials is: "<<endl;
    display(multiply(poly1, poly2));
    complex c1, c2;
    c1.im = 2;
    c1.re = 3.2;
    c2.im = 3;
    c2.re = 1.2;
    cout << endl;
    display_complex(c1);
    display_complex(c2);
    return 0;
}