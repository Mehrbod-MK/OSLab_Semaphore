// OSLab_Semaphore.cpp : This file contains the 'main' function. Program execution begins and ends there.
// IAU-TNB
// Developer:  Mehrbod Molla Kazemi
// Professor:  Dr. ZamanPour

#include <iostream>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>

const int MAX_SEM_COUNT = 10;
const int MAX_THREAD_COUNT = 10;
const int MAX_RESOURCES = 7;

int resources[MAX_RESOURCES];

HANDLE semaphore;
DWORD WINAPI MyFunction(void*);

int main()
{
    // std::cout << "Hello World!\n";

    HANDLE myThreads[MAX_THREAD_COUNT];
    DWORD tid;
    int i;
    
    for (i = 0; i < MAX_RESOURCES; i++)
    {
        resources[i] = 1;
    }

    semaphore = CreateSemaphore(NULL, 3, MAX_SEM_COUNT, L"MMK");
    if (semaphore == NULL)
    {
        std::cout << "CANNOT CREATE SEMAPHORE!\n";
        system("PAUSE");
        return -1;
    }

    for (i = 0; i < MAX_THREAD_COUNT; i++)
    {
        myThreads[i] = CreateThread(NULL, 0,
            (LPTHREAD_START_ROUTINE)MyFunction, NULL, 0, &tid);

        if (myThreads[i] == NULL)
        {
            std::cout << "Failed to create THREAD!\n";
            system("PAUSE");
            return -1;
        }
    }

    WaitForMultipleObjects(MAX_THREAD_COUNT, myThreads, true, INFINITE);

    for (i = 0; i < MAX_THREAD_COUNT; i++)
    {
        CloseHandle(myThreads[i]);
    }
    CloseHandle(semaphore);

    return 0;
}

DWORD WINAPI MyFunction(void* params)
{
    UNREFERENCED_PARAMETER(params);

    srand(time(NULL));

    DWORD dwWaitResult;
    bool shouldRun = true;

    while (shouldRun)
    {
        dwWaitResult = WaitForSingleObject(semaphore, 0L);
        int waitTime = (rand() + 500) % 2000;
        int resource = rand() % MAX_RESOURCES;

        switch (dwWaitResult)
        {
        case WAIT_OBJECT_0:
            shouldRun = false;
            printf("Thread %d REACHED CRITICAL SECTION\n", GetCurrentThreadId());

            // Consume resource.
            if (resources[resource] >= 1)
            {
                resources[resource]--;
                printf("Thread %d consumed resource:  %d. Remaining: %d\n", GetCurrentThreadId(), resource, resources[resource]);
            }
            else
            {
                printf("RESOURCE %d NOT AVAILABLE FOR THREAD: %d\n", resource, GetCurrentThreadId());
            }
            Sleep(waitTime);

            printf("Thread %d EXITED CRITICAL SECTION\n", GetCurrentThreadId());

            if (!ReleaseSemaphore(semaphore, 1, NULL))
            {
                printf("Release SEM error!!!\n");
            }

            // Release resource.
            resources[resource]++;
            printf("Thread %d returned resource:  %d. Remaining: %d\n", GetCurrentThreadId(), resource, resources[resource]);

            break;

        case WAIT_TIMEOUT:

            // printf("THREAD TIMED OUT! %d\n", GetCurrentThreadId());

            break;
        }
    }

    return true;
}
 
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
