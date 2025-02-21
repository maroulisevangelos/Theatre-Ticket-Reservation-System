# Theatre-Ticket-Reservation-System
This project implements a theatre ticket reservation system using POSIX threads (pthreads). The system allows multiple clients to reserve seats, make payments with credit cards, and manage transactions concurrently. The program ensures mutual exclusion with mutexes and synchronization with condition variables.


Features:

    Seat Reservation: Clients can reserve seats in different zones (Zone A and Zone B) of the theatre.
    
    Payment Processing: Payments are processed by cashiers, and transactions are managed securely.
    
    Concurrency Management: The system uses threads to handle multiple clients simultaneously.
    
Implementation Details:

    The theatre has a rectangular layout with seats organized in rows.
    
    Zone A consists of 10 rows, and Zone B consists of 20 rows.
    
    Each client randomly selects a zone and a number of seats to reserve.
    
    The system checks for continuous available seats and reserves them if possible.
    
    If the payment is successful, the seats are confirmed, and the client's credit card is charged.
    
    If the payment fails, the seats are released back to the theatre plan.
    
    The system prints the reservation results, total revenue, transaction statistics, average waiting time, and average service time.

Files Included:

    Header File: Contains constant declarations.
    
    C Code File: Contains the implementation code.
    
    Test Script: A script to compile and run the program with specific parameters.

All necessary code and assets required for the system are included in this repository.
