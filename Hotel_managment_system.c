#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <conio.h>

typedef struct customerdetails {
    int roomNo;
    char name[50];
    char address[100];
    char checkInDate[20];
    char checkOutDate[20];
    float bill;
    float originalBill;
    bool isCancelled;
    bool isCheckedout;
    struct customerdetails* next;
} customerdetails;

typedef struct roomdetails {
    int roomNo;
    int status;
    int customerCount;
    struct roomdetails* next;
} roomdetails;

roomdetails* reserveroom = NULL;
customerdetails* customerentry = NULL;

// Function declarations
void greetings();
roomdetails* addroom(roomdetails* start, int roomNo);
customerdetails* addcustomer(customerdetails* start, int roomNo, char* name, char* address, char* checkInDate, char* checkOutDate);
void displaycustomer(customerdetails* start);
void checkoutcustomer(int roomNo);
int isempty();
float calculatebill(char* checkInDate, char* checkOutDate, bool isCancelled);
void displayroomstatus(roomdetails* startR);
void allocateinitialrooms(int totalRooms);
void cancelbooking(int roomNo);
void displaytotalearnings();
bool isDateValid(const char* date);

int main() {
    int choice, roomNo;
    char name[50], address[100], checkInDate[20], checkOutDate[20];
    int cancelRoomNo;

    customerdetails* temp_cus;
    greetings();

    int totalRooms = 20;
    allocateinitialrooms(totalRooms);

    do {
        printf("\n");

        printf("\n\n");
        printf("1. Check in customer\n");
        printf("2. Check out customer\n");
        printf("3. Cancel booking\n");
        printf("4. Display customer details\n");
        printf("5. Display room status\n");
        printf("6. Total revenue details\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter customer details:\n");
                printf("Name: ");
                scanf(" %[^\n]", name);
                printf("Address: ");
                scanf(" %[^\n]", address);

                do {
                    printf("Check-in date (yyyy-mm-dd): ");
                    scanf("%s", checkInDate);
                } while (!isDateValid(checkInDate));


                    while (1) {
                        printf("Check-out date (yyyy-mm-dd): ");
                        scanf("%s", checkOutDate);
                        if (!isDateValid(checkOutDate)) {
                            printf("Invalid date format. Please use the yyyy-mm-dd format.\n");
                        } else if (strcmp(checkOutDate, checkInDate) <= 0) {
                            printf("Invalid check-out date. Please enter a valid date later than the check-in date.\n");
                        } else {
                            break;
                        }
                    }


                int roomAllocated = 0;

                roomdetails* temp_room = reserveroom;
                while (temp_room != NULL) {
                    if (temp_room->status == 0) {
                        customerentry = addcustomer(customerentry, temp_room->roomNo, name, address, checkInDate, checkOutDate);
                        temp_room->status = 1;
                        temp_room->customerCount++;
                        customerentry->bill = calculatebill(checkInDate, checkOutDate, customerentry->isCancelled);
                        printf("Customer allocated to Room %d.\n", temp_room->roomNo);
                        roomAllocated = 1;
                        break;
                    }
                    temp_room = temp_room->next;
                }
                if (!roomAllocated) {
                    printf("No rooms available. Please check out customers to make rooms available.\n");
                }
                break;
            case 2:
                printf("Enter room number: ");
                scanf("%d", &roomNo);
                checkoutcustomer(roomNo);
                break;
            case 3:
                printf("Enter room number to cancel booking: ");
                scanf("%d", &cancelRoomNo);
                cancelbooking(cancelRoomNo);
                break;
            case 4:
                displaycustomer(customerentry);
                break;
            case 5:
                displayroomstatus(reserveroom);
                break;
            case 6:
                displaytotalearnings();
                break;
            case 7:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please choose again.\n");
        }
    } while (choice != 7);

    return 0;
}


void greetings()
{
    printf("\n\n");
    printf("\t\t\t     ****************************************\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     *     ----------------------------     *\n");
    printf("\t\t\t     *      WELCOME  TO   PESU  LODGE       *\n");
    printf("\t\t\t     *     ----------------------------     *\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     *     PES UNIVERSITY,BENGALURU,India   *\n");
    printf("\t\t\t     *     Email: pesulodgePPP@gmail.com    *\n");
    printf("\t\t\t     *     Contact:1233456781               *\n");
    printf("\t\t\t     *                                      *\n");
    printf("\t\t\t     ****************************************\n");
    printf("\t\t\t             Press any key to continue: ");
    getch();
}

roomdetails* addroom(roomdetails* start, int roomNo) {
    roomdetails* new_room = (roomdetails*)malloc(sizeof(roomdetails));
    new_room->roomNo = roomNo;
    new_room->status = 0;
    new_room->customerCount = 0;
    new_room->next = NULL;

    if (start == NULL) {
        return new_room;
    }
    roomdetails* temp = start;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_room;

    return start;
}

customerdetails* addcustomer(customerdetails* start, int roomNo, char* name, char* address, char* checkInDate, char* checkOutDate) {
 
    if (!isDateValid(checkInDate) || !isDateValid(checkOutDate) || strcmp(checkOutDate, checkInDate) <= 0) {
        printf("Invalid check-in or check-out date. Please enter valid dates.\n");
        return start;
    }

    customerdetails* new_customer = (customerdetails*)malloc(sizeof(customerdetails));
    new_customer->roomNo = roomNo;
    strcpy(new_customer->name, name);
    strcpy(new_customer->address, address);
    strcpy(new_customer->checkInDate, checkInDate);
    strcpy(new_customer->checkOutDate, checkOutDate);
    new_customer->bill = 0.0;
    new_customer->isCancelled = false;
    new_customer->originalBill = 0.0;
    new_customer->isCheckedout = false;
    new_customer->next = start;
    return new_customer;
}


void displaycustomer(customerdetails* start) {
    customerdetails* temp_cus = start;
    printf("\nCustomer Details:\n");
    printf("Room No   Name                          Address                       Check-in Date   Check-out Date   Bill\n");
    while (temp_cus != NULL) {
        if (temp_cus->isCancelled) {
            printf("%-9d%-30s%-30s%-15s%-16s200.00 (Fine)\n", temp_cus->roomNo, temp_cus->name, temp_cus->address, temp_cus->checkInDate, temp_cus->checkOutDate);
        } else {
            printf("%-9d%-30s%-30s%-15s%-16s%.2f\n", temp_cus->roomNo, temp_cus->name, temp_cus->address, temp_cus->checkInDate, temp_cus->checkOutDate, temp_cus->bill);
        }
        temp_cus = temp_cus->next;
    }
}


void checkoutcustomer(int roomNo) {
    customerdetails* temp_cus = customerentry;
    int roomFound = 0;

    while (temp_cus != NULL) {
        if (temp_cus->roomNo == roomNo) {
            roomFound = 1;

            if (!temp_cus->isCancelled) {

                roomdetails* temp_room = reserveroom;
                while (temp_room != NULL) {
                    if (temp_room->roomNo == roomNo) {
                        temp_room->customerCount--;
                        if (temp_room->customerCount == 0) {
                            temp_room->status = 0;
                        }
                        break;
                    }
                    temp_room = temp_room->next;
                }

                temp_cus->isCheckedout = true;

                printf("Customer in Room %d has been checked out.\n", roomNo);
                return;
            } else {
                printf("Room %d has already been cancelled. Cannot check out.\n", roomNo);
            }
            break;
        }
        temp_cus = temp_cus->next;
    }

    if (!roomFound) {
        printf("Room %d is not allocated.\n", roomNo);
    }
}

int isempty() {
    roomdetails* temp_room = reserveroom;
    while (temp_room != NULL) {
        if (temp_room->status == 0) {
            return 0; 
        }
        temp_room = temp_room->next;
    }
    return 1; 
}

float calculatebill(char* checkInDate, char* checkOutDate, bool isCancelled) {
    struct tm tm1 = {0};
    struct tm tm2 = {0};

    if (sscanf(checkInDate, "%d-%d-%d", &tm1.tm_year, &tm1.tm_mon, &tm1.tm_mday) != 3 ||
        sscanf(checkOutDate, "%d-%d-%d", &tm2.tm_year, &tm2.tm_mon, &tm2.tm_mday) != 3 ||
        tm1.tm_year < 0 || tm2.tm_year < 0 || tm1.tm_mon < 1 || tm1.tm_mon > 12 || tm2.tm_mon < 1 || tm2.tm_mon > 12 ||
        tm1.tm_mday < 1 || tm1.tm_mday > 31 || tm2.tm_mday < 1 || tm2.tm_mday > 31) {
        printf("Invalid date format or out-of-range values. Please use the yyyy-mm-dd format and ensure valid date components.\n");
        return -1.0; 
    }
 
    tm1.tm_year -= 1900;
    tm1.tm_mon -= 1;
    tm2.tm_year -= 1900;
    tm2.tm_mon -= 1;

    time_t t1 = mktime(&tm1);
    time_t t2 = mktime(&tm2);

    if (t1 == -1 || t2 == -1 || t2 <= t1) {
        printf("Invalid dates. Please make sure the check-out date is later than the check-in date.\n");
        return -1.0; 
    }

    double diff = difftime(t2, t1);
    int totalDays = (int)(diff / (60 * 60 * 24));

    return totalDays * 750.0; 
}

void displayroomstatus(roomdetails* startR) {
    int totalRooms = 0;
    int occupiedRooms = 0;
    int vacantRooms = 0;

    roomdetails* temp_room = startR;

    while (temp_room != NULL) {
        totalRooms++;
        if (temp_room->status == 1) {
            occupiedRooms++;
        } else {
            vacantRooms++;
        }
        temp_room = temp_room->next;
    }

    printf("\nHotel Room Status Report:\n");
    printf("Total Rooms: %d\n", totalRooms);
    printf("Occupied Rooms: %d\n", occupiedRooms);
    printf("Vacant Rooms: %d\n", vacantRooms);
    printf("\nRoom Status Visualization:\n");

    temp_room = startR;
    while (temp_room != NULL) {
        printf("Room %d: ", temp_room->roomNo);
        if (temp_room->status == 1) {
            printf("[Occupied] - ");
            for (int i = 0; i < temp_room->customerCount; i++) {
                printf("C%d ", i + 1);
            }
        } else {
            printf("[Vacant]");
        }
        printf("\n");
        temp_room = temp_room->next;
    }
}

void allocateinitialrooms(int totalRooms) {
    for (int roomNo = 1; roomNo <= totalRooms; roomNo++) {
        reserveroom = addroom(reserveroom, roomNo);
    }
}

void cancelbooking(int roomNo) {
    customerdetails* temp_cus = customerentry;
    int roomFound = 0;

    while (temp_cus != NULL) {
        if (temp_cus->roomNo == roomNo) {
            roomFound = 1;

            if (temp_cus->isCheckedout) {
                printf("Room %d is currently empty. Cannot cancel booking.\n", roomNo);
                return;
            }

            if (temp_cus->isCancelled) {
                printf("Booking for Room %d has already been cancelled.\n", roomNo);
                return;
            }


            temp_cus->isCancelled = true;
            temp_cus->bill += 200.0;
            temp_cus->originalBill = temp_cus->bill;


            roomdetails* temp_room = reserveroom;
            while (temp_room != NULL) {

                if (temp_room->roomNo == roomNo) {
                    temp_room->status = 0; 
                    temp_room->customerCount = 0; 
                    break;
                }
                temp_room = temp_room->next;
            }

            printf("Booking for Room %d has been cancelled. Fine of $200 applied.\n", roomNo);
            return;
        }
        temp_cus = temp_cus->next;
    }

    printf("No booking found for Room %d.\n", roomNo);
}

void displaytotalearnings() {
    customerdetails* temp_cus = customerentry;
    float totalEarnings = 0.0;
    while (temp_cus != NULL) {
        if (!temp_cus->isCancelled) {
            totalEarnings += temp_cus->bill; 
        } else {
            totalEarnings += 200.0; 
        }
        temp_cus = temp_cus->next;
    }
    printf("\nTotal Earnings: $%.2f\n", totalEarnings);
}

bool isDateValid(const char* date) {
    struct tm tm = {0};
    if (sscanf(date, "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) == 3) {
        
        tm.tm_year -= 1900;
        tm.tm_mon -= 1;


        if ((tm.tm_year >= 0) && (tm.tm_mon >= 0) && (tm.tm_mon < 12) && (tm.tm_mday > 0)) {
            
            int maxDaysInMonth = 31;
            if (tm.tm_mon == 3 || tm.tm_mon == 5 || tm.tm_mon == 8 || tm.tm_mon == 10) {
                maxDaysInMonth = 30;
            } else if (tm.tm_mon == 1) {
                
                maxDaysInMonth = ((tm.tm_year % 4 == 0 && tm.tm_year % 100 != 0) || (tm.tm_year % 400 == 0)) ? 29 : 28;
            }


            if (tm.tm_mday <= maxDaysInMonth) {
                
                time_t t = mktime(&tm);
                if (t != -1) {
                    return true;
                }
            }
        }
    }
    printf("Invalid date format or out-of-range values. Please use the yyyy-mm-dd format and ensure valid date components.\n");
    return false;
}
