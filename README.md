# Traffic Fine & Violation Management System (C++)

This was my Object Oriented Programming semester project. Honestly this semester had been pretty hectic for me because I had to deal with some big responsibilities in life while also trying to keep up with studies.

While making this project there were many times where it almost broke me 😅. I would write one function and everything would work perfectly, and then after adding another function suddenly nothing would work together anymore. So debugging this project took a lot of time and patience.

In the end though, it was actually pretty fun to build and I learned a lot of things about how different parts of a program interact with each other.

The main goal of this project was to make a Traffic Fine & Violation Management System that can generate tickets, keep track of violations, and allow an admin to manage the whole system.

The thing I was most excited about (and also the thing that took most of my time) was building the admin panel.

# What the System Can Do

The system allows a few main operations.

- Generate Traffic Tickets

  A traffic officer can generate a ticket for a violation. The ticket stores things like:

  - Ticket ID
  - Driver information
  - Vehicle type
  - Violation committed
  - Fine amount
  - Payment status

Each ticket is also saved in its own file using the ticket ID.

- Violation History

  - Every driver has a record of their violations.
  - This makes it possible to check previous violations for a driver.

- Payment Management

  - Tickets can later be marked as paid.

  • When a ticket is paid, the system updates:
    - The individual ticket file
    - The main ticket record file


- Admin Panel

  • The admin panel is where most of the management features exist.

From here the admin can:

  - Add new drivers
  - Remove drivers
  - Add traffic officers
  - Remove officers
  - View all registered drivers
  - View all officers
  - Add new traffic violations
  - Remove violations
  - Generate reports of tickets

This was honestly the part where I spent most of my time while building the project.

- Admin Password System

  The admin panel is protected by a password system.

  Before accessing the admin functions, the user has to enter the correct password. The password itself is stored in a file and can also be changed from inside the admin panel.

  Implementing this properly was actually a bit tricky because I had to:

  - Read the stored password from the file
  - Compare it manually with user input
  - Update the password file when it is changed

Since we were working with character arrays instead of strings, this required writing extra logic for comparing and updating passwords.

# Concepts Used

This project was mainly built to practice Object Oriented Programming concepts, including:

- Classes and Objects
- Inheritance
- Abstract Classes
- Function modularity
- Dynamic memory allocation
- File handling
- Custom string handling using character arrays

The program is divided into multiple classes such as:

  - Person (base class)
  - Driver
  - Officer
  - Vehicle
  - Car
  - Bike
  - Violation
  - Ticket
  - Payment
  - TrafficDepartment

Each class handles a specific part of the system which helped keep the code more organized.

# Data Storage

All the information used by the system is stored in text files, including:

  - Drivers
  - Officers
  - Violations
  - Tickets
  - Payment status
  - Admin password

This makes sure the data stays saved even after the program closes.

Each ticket is also stored in a separate file generated using the ticket ID.

# Some Extra Things Implemented

While building this project I also added a few things to make the system more consistent.

- Input Validation
  Most user inputs like menu options and IDs are validated to avoid invalid entries.
  
- String Normalization
  Names of drivers, officers, and violations are formatted consistently before being stored.

- Custom String Functions
  Since we were restricted in using built-in utilities, I implemented my own helper functions such as:
  - Copying character arrays
  - Appending arrays
  - Normalizing strings

# Challenges While Making This

There were quite a few challenges while writing this project.

- Dynamic Memory Handling
  - Since many things used dynamic memory allocation, I had to be careful to avoid memory leaks and incorrect memory usage.

- Manual String Handling
  - Working with character arrays instead of strings made even simple operations like comparisons and formatting more complicated.

- File Handling
  - Managing multiple files and updating specific records required careful reading and rewriting of files.

- Password System
  - Implementing the admin password system without built-in utilities required writing manual comparison logic and handling password updates safely.

# What I Learned

This project helped me understand how a bigger program is structured and how different parts of the system interact with each other.

It also helped me get better at:

- Structuring classes
- Handling files
- Managing memory
- Debugging larger programs

Even though it was frustrating at times, it was still a pretty interesting project to work on.

# Project Structure

- main.cpp → Main program file
- drivers.txt → Stores driver information
- officers.txt → Stores officer records
- violations.txt → Stores traffic violations and fines
- all_tickets.txt → Stores all generated tickets
- password.txt → Stores admin password
# How to Run

1. Download or clone the repository
2. Open the project in any C++ compiler (Visual Studio / CodeBlocks / Dev-C++)
3. Compile the main source file
4. Make sure the required text files are in the same directory as the executable
5. Run the program

