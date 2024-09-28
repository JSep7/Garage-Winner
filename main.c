/* COP 3502C Assignment 2
This program is written by: Jaime Sepulveda */

#include <stdio.h>
#include <stdlib.h>
#define MAX_Garage 10

typedef struct Student {
    int sequenceNumber;
    struct Student *next;
}Student;

typedef struct Queue {
    Student *front;
    Student *back;
    int nodeCount, k, th, garageNum;
}Queue;

int isEmpty(Queue *q) {
    if(q->front == NULL)
    {
        return 1;
    }
    return 0;
}

//Takes an integer, dynamically allocates a Student struct and returns a Student node
Student *createStudent(int sequence) {
    Student *student = malloc(sizeof(Student));
    student->sequenceNumber = sequence;
    student->next = NULL;
    return student;
}

//Makes the queue function for newStudent and creates the circular link
void enQueue(Queue *q, Student *newStudent) {
    //Checks if the front is empty and then fills the front and back with newStudent
    if (q->back == NULL)
    {
        q->front = newStudent;
        q->back = newStudent;
        //This creates the circular link
        newStudent->next = newStudent;
    }
    else
    {
        //Else if it's not empty add the new node to the back
        q->back->next = newStudent;
        //Change the back
        q->back = newStudent;
        //Connect back the circular link
        q->back->next = q->front;
    }
}

//Function to remove and return front node from Queue
Student *deQueue(Queue *q) {
    if (isEmpty(q)) //Calls isEmpty function and returns NULL if true
    {
        return NULL;
    }

    Student *temp = q->front;
    //If front = back sets both to NULL because is empty
    if (q->front == q->back)
    {
        //Only one node in the queue
        q->front = NULL;
        q->back = NULL;
    }
    else
    {
        //Moves front pointer to next then back to front to connect circular link
        q->front = q->front->next;
        q->back->next = q->front;
    }
    q->nodeCount--;
    temp->next = NULL; //Disconnect the node from the list
    return temp;
}

//Peek function to check if Queue is empty
Student *peekQueue(Queue *q) {
    if(isEmpty(q))
    {
        return NULL;
    }
    else
    {
        return q->front;
    }
}

//Initializes all variables to NULL or 0
void initQueue(Queue *q) {
    q->front = NULL;
    q->back = NULL;
    q->nodeCount = 0;
    q->k = 0;
    q->th = 0;
    q->garageNum = 0;
}

//Function that creates circular queue in reverse order
void createReverseCircle(Queue *q) {
    if(isEmpty(q))
    {
        //Loop to create and enQueue students in reverse order
        for (int i = q->nodeCount; i >= 1; i--)
        {
            Student *newStudent = createStudent(i);
            enQueue(q, newStudent);
        }
        //Connects the circular Node back if it's not NULL
        if (q->back != NULL)
        {
            q->back->next = q->front;
        }
    }
}

//Function to reverse amd rearrange the circle
void rearrangeCircle(Queue* q) {
    if(isEmpty(q))
    {
        return;
    }

    //Initializes the pointers to traverse the node
    Student *front = q->front;
    Student *next = front->next;
    Student *prev = front;

    //Reverse the link of the node
    front->next = prev;
    front = next;

    //This will traverse the Queue and reverse the links
    while(front != q->front)
    {
        next = front->next;
        front->next = prev;
        prev = front;
        front = next;
    }

    //Adjust pointers to connect everything
    q->front->next = q->back;
    q->back = q->front;
    q->front = prev;
}

//Displays all non empty queues
void display(Queue *garage) {
    for (int i = 0; i < MAX_Garage; i++)
    {
        if (!isEmpty(&garage[i]))
        {
            //Print garage number
            printf("%d", garage[i].garageNum);
            //Traverse the circular linked list and print sequence numbers
            Student *temp = garage[i].front;
            if (temp != NULL)
            {  //Check to ensure temp is not NULL
                printf(" %d", temp->sequenceNumber);
                temp = temp->next;
                while (temp != garage[i].front)
                {
                    printf(" %d", temp->sequenceNumber);
                    temp = temp->next;
                }
                printf("\n");
            }
        }
    }
}

//Gets the students eliminates using their for every number of k until th is left
void phase1(Queue *q){

    //Initializes front and back
    Student *front = q->front;
    Student *back = q->back;

    //While loop to start elimination process until Students nodeCount greater than thresh
    while (q->nodeCount > q->th)
    {
        //Move front and back k-1 to find the kth student and eliminate them
        for (int i = 1; i < q->k; i++)
        {
            back = front;
            front = front->next;
        }

        printf("Student# %d eliminated\n", front->sequenceNumber);
        back->next = front->next;

        //If student eliminated move front pointer
        if (front == q->front)
        {
            q->front = front->next;
        }

        //If eliminated student is at the back update back pointer
        if (front == q->back)
        {
            q->back = back;
        }

        //Free the memory
        Student *temp = front;
        front = back->next;
        free(temp);
        q->nodeCount -= 1;
    }
}

//Phase 2 elimination: Eliminate students across groups until one student remains
void phase2(Queue garages[], int totalGarages) {
    int remainingStudents = 0;

    //Calculate total remaining students
    for (int i = 0; i < totalGarages; i++)
    {
        if (!isEmpty(&garages[i]))
        {
            remainingStudents += garages[i].nodeCount;
        }
    }

    while (remainingStudents > 1)
    {
        int maxNum = -1;
        int garageIndexToEliminate = -1;

        //Find the garage with the highest front sequence number
        //Tie-breaker: smallest garage number
        for (int i = 0; i < totalGarages; i++)
        {
            if (!isEmpty(&garages[i]))
            {
                int seqNum = garages[i].front->sequenceNumber;
                if (seqNum > maxNum ||
                   (seqNum == maxNum && (garageIndexToEliminate == -1 || garages[i].garageNum < garages[garageIndexToEliminate].garageNum)))
                {
                    maxNum = seqNum;
                    garageIndexToEliminate = i;
                }
            }
        }

        if (garageIndexToEliminate == -1)
        {
            break; //No group to eliminate from
        }

        //Eliminate the student from the selected garage
        Student *eliminatedStudent = deQueue(&garages[garageIndexToEliminate]);
        if (eliminatedStudent != NULL)
        {
            printf("Eliminated student %d from group for garage %d\n", eliminatedStudent->sequenceNumber, garages[garageIndexToEliminate].garageNum);
            free(eliminatedStudent);
            remainingStudents--;
        }
    }

    //Find and declare the winner
    int winnerFound = 0;
    for (int i = 0; i < totalGarages; i++)
    {
        if (!isEmpty(&garages[i]))
        {
            Student *winner = peekQueue(&garages[i]);
            if (winner != NULL)
            {
                printf("\nStudent %d from the group for garage %d is the winner!\n", winner->sequenceNumber, garages[i].garageNum);
                winnerFound = 1;
                break;
            }
        }
    }

    if (!winnerFound)
    {
        printf("No winner found.\n");
    }
}


int main() {
    //Array of Queue structures for each garage
    Queue garage[MAX_Garage];
    int garageNum, k, th, studentCount, garages;
    scanf("%d", &garages);
    for (int i = 0; i < 10; i++)
    {
        initQueue(&garage[i]);
    }

    //Initialize each garage queue
    for (int i = 0; i < garages; i++)
    {
        scanf("%d %d %d %d", &garageNum , &studentCount, &k, &th);
        //Assign the values
        garage[garageNum - 1].garageNum = garageNum;
        garage[garageNum - 1].k = k;
        garage[garageNum - 1].th = th;
        garage[garageNum - 1].nodeCount = studentCount;

        //Creates reverse circular queue
        createReverseCircle(&garage[garageNum - 1]);
    }
    printf("Initial status of nonempty queues\n");
    display(garage);

    //Rearrange the order
    for (int i = 0; i < 10; i++)
    {
        rearrangeCircle(&garage[i]);
    }

    printf("\nAfter ordering status of nonempty queues\n");
    display(garage);

    printf("\nPhase1 elimination\n");
    //Preforms the Phase1 elimination for garages
    for (int i = 0; i < 10; i++)
    {
        if(!isEmpty(&garage[i]))
        {
            printf("\nGroup for Garage# %d\n", i + 1);
            phase1(&garage[i]);
        }
    }

    //Phase 2 elimination
    printf("\nPhase2 elimination\n");
    phase2(garage, MAX_Garage);

    //Free remaining students in all garages
    for (int i = 0; i < MAX_Garage; i++)
    {
        //Dequeues and frees until everything is empty
        while (!isEmpty(&garage[i]))
        {
            Student *student = deQueue(&garage[i]);
            if (student != NULL)
            {
                free(student);
            }
        }
    }
    return 0;
}

