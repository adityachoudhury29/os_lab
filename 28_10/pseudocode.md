***Pseudocode for First-Year Students:***

Initialize Shared Memory and Semaphores:
    Connect to shared memory containing SharedCounters.
    Initialize hod_room semaphore (1: allows one student in the room).
    Initialize turn semaphore (1: allows first-year students to start).

Define visit_hod_room function:
    For each of 5 visits:
        Increment first_year_count.
        If there are waiting final-year students:
            Wait on turn semaphore to allow alternating access.
        Wait on hod_room semaphore to enter the HOD’s room (critical section).
        Print message showing first-year student visiting.
        Simulate time spent in the room (e.g., sleep(1)).
        Release hod_room semaphore.
        Decrement first_year_count.
        If there are waiting final-year students:
            Signal turn to allow final-year access.
        Sleep before the next visit.

Cleanup Resources:
    Disconnect shared memory and close semaphores.

***Pseudocode for Final-Year Student:***

Initialize Shared Memory and Semaphores:
    Connect to shared memory containing SharedCounters.
    Initialize hod_room semaphore.
    Initialize turn semaphore (0: allows final-year students to start when first-year is done).

Define visit_hod_room function:
    For each of 5 visits:
        Increment final_year_count.
        If there are waiting first-year students:
            Wait on turn semaphore to allow alternating access.
        Wait on hod_room semaphore to enter the HOD’s room.
        Print message showing final-year student visiting.
        Simulate time spent in the room.
        Release hod_room semaphore.
        Decrement final_year_count.
        If there are waiting first-year students:
            Signal turn to allow first-year access.
        Sleep before the next visit.

Cleanup Resources:
    Disconnect shared memory and close semaphores.

