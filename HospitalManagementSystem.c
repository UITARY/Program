#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_PATIENTS 50
#define MAX_ITEMS 50
#define MAX_DOCTORS 50
#define MAX_ROOMS 20
#define MAX_STAFF 50
#define MAX_ACCOUNTS 50

typedef struct {
	char username[20];
	char password[20];
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

typedef struct
{
	int id;
	char name[50];
	int age;
	char gender[10];
	char disease[50];
	char medicalResults[200];
	char admitDate[30];
	char dischargeDate[30];
	int assignedRoom;

	float billAmount;
	float totalPaid;
	float balance;

	float payments[50];
	char paymentDates[50][50];
	int paymentCount;

	int isPaid;
	int isDischarged;
} Patient;

Patient patients[MAX_PATIENTS];
int patientID = 0;
int patientCount = 0;
int patientDischarge = 0;
int patientPaymentPaid = 0;
int transactionCount = 0;
int patientTransactionCount = 0;
int ID;

typedef struct
{
	char name[50];
	float price;
	int quantity;
	float total;
} Item;

char DoctorName[MAX_DOCTORS][50];
char DoctorSpec[MAX_DOCTORS][50];
int doctorCount = 0;

int RoomNumber[MAX_ROOMS];
int selectedRoom;

char StaffName[MAX_STAFF][50];
char StaffRole[MAX_STAFF][50];
int staffCount = 0;

void getCurrentDateTime(char *buffer, size_t size)
{
	time_t t = time(NULL);
	struct tm *tmp = localtime(&t);
	if (tmp)
	{
		strftime(buffer, size, "%Y-%m-%d %H:%M:%S", tmp);
	}
	else
	{
		strncpy(buffer, "0000-00-00 00:00:00", size - 1);
		buffer[size - 1] = '\0';
	}
}

void clearInputBuffer(void)
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
	{
	}
}

void clearScreen()
{
	system("cls");
}

void viewRooms()
{
	printf("\n==================== ROOM LIST ======================\n");
	for (int i = 0; i < MAX_ROOMS; i++)
	{
		printf("Room %d\t\t%s\n", i + 1, (RoomNumber[i] == 0) ? "Available" : "Occupied");
	}
}

void patientBills(Patient *p)
{
	Item items[MAX_ITEMS];
	int n;
	float grandTotal = 0.0f;

	FILE *fp = fopen("Bills.txt", "a");

	while (1)
	{
		clearScreen();
		printf("\n====== PATIENT BILLING ======\n");
		printf("Patient Name: %s\n", p->name);
		printf("Patient ID  : %d\n", p->id);
		printf("==============================\n");

		printf("Number of services/items: ");
		if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_ITEMS)
		{
			clearInputBuffer();
			printf("Invalid number.\n");
			fclose(fp);
			sleep(1);
			continue;
		}
		clearInputBuffer();
		break;
	}


	for (int i = 0; i < n; i++)
	{
		while (1)
		{
			clearScreen();
			printf("\n====== PATIENT BILLING ======\n");
			printf("Patient Name: %s\n", p->name);
			printf("Patient ID  : %d\n", p->id);
			printf("==============================\n");
			printf("\nItem %d:\n", i + 1);

			printf("Service Name: ");
			if (fgets(items[i].name, sizeof(items[i].name), stdin) == NULL)
			{
				clearInputBuffer();
				sleep(1);
				continue;
			}
			items[i].name[strcspn(items[i].name, "\n")] = '\0';

			fprintf(fp, "%d|%s|%.2f|%d|%.2f\n",
			        i + 1, items[i].name, items[i].price, items[i].quantity, items[i].total);

			printf("Price: ");
			if (scanf("%f", &items[i].price) != 1 || items[i].price <= 0)
			{
				clearInputBuffer();
				printf("Invalid quantity.\n");
				sleep(1);
				continue;
			}
			clearInputBuffer();

			printf("Quantity: ");
			if (scanf("%d", &items[i].quantity) != 1 || items[i].quantity <= 0)
			{
				clearInputBuffer();
				printf("Invalid quantity.\n");
				sleep(1);
				continue;
			}
			clearInputBuffer();

			items[i].total = items[i].price * items[i].quantity;
			grandTotal += items[i].total;

			break;
		}
	}

	p->billAmount = grandTotal;

	printf("\n===================== HOSPITAL BILL ======================\n");
	printf("%-5s %-20s %-10s %-10s %-10s\n", "No.", "Service", "Price", "Qty", "Total");
	printf("==========================================================\n");
	for (int i = 0; i < n; i++)
	{
		printf("%-5d %-20s %-10.2f %-10d %-10.2f\n",
		       i + 1, items[i].name, items[i].price, items[i].quantity, items[i].total);
	}
	printf("==========================================================\n");
	printf("TOTAL: %.2f\n", p->billAmount);
	printf("==========================================================\n");


	time_t now = time(NULL);
	fprintf(fp, "\n============================= NEW BILL =============================\n");
	fprintf(fp, "DATE: %s", ctime(&now));
	fprintf(fp, "PATIENT: %s (ID: %d)\n", p->name, p->id);
	fprintf(fp, "TOTAL BILL: %.2f\n", p->billAmount);
	fprintf(fp, "==========================================================\n");

	fclose(fp);
}

void patientInfo(Patient p)
{
	clearScreen();
	printf("\n=========================================\n");
	printf("      PATIENT REGISTERED SUCCESSFULLY\n");
	printf("=========================================\n");
	printf("Patient ID     : %d\n", p.id);
	printf("Name           : %s\n", p.name);
	printf("Age            : %d\n", p.age);
	printf("Gender         : %s\n", p.gender);
	printf("Disease        : %s\n", p.disease);
	printf("Medical Result : %s\n", p.medicalResults);
	printf("Total Bill     : %.2f\n", p.billAmount);
	printf("Admit Date     : %s\n", p.admitDate);
	printf("Assigned Room  : %d\n", p.assignedRoom);
	printf("=========================================\n");
}

void registerPatient()
{
	FILE *fp = fopen("registerPatients.txt", "a");
	if (fp == NULL)
	{
		printf("\nError: Could not open file for writing.\n");
		return;
	}

	int genderChoice;
	int resultChoice;
	int roomChoice;

	if (patientCount >= MAX_PATIENTS)
	{
		printf("\n=========================================\n");
		printf("  Patient list is full! Cannot register more.\n");
		printf("=========================================\n");
		fclose(fp);
		return;
	}

	Patient p;
	p.id = patientID + 1;
	p.assignedRoom = 0;
	p.isPaid = 0;
	p.isDischarged = 0;
	strcpy(p.dischargeDate, "N/A");
	getCurrentDateTime(p.admitDate, sizeof(p.admitDate));

	while (1)
	{
		clearScreen();
		printf("\n=========================================\n");
		printf("         PATIENT REGISTRATION FORM\n");
		printf("=========================================\n");

		printf("\nEnter patient name: ");
		if (fgets(p.name, sizeof(p.name), stdin) == NULL)
			continue;
		p.name[strcspn(p.name, "\n")] = '\0';

		if (strlen(p.name) == 0)
		{
			printf("Name cannot be empty.\n");
			sleep(1);
			continue;
		}

		printf("Enter age: ");
		if (scanf("%d", &p.age) != 1 || p.age <= 0)
		{
			clearInputBuffer();
			printf("\nInvalid Age.\n");
			sleep(1);
			continue;
		}
		clearInputBuffer();

		printf("\nSelect Gender:\n");
		printf("[1] Male\n");
		printf("[2] Female\n");
		printf("Enter choice: ");
		if (scanf("%d", &genderChoice) != 1 || genderChoice < 1 || genderChoice > 2)
		{
			clearInputBuffer();
			printf("\nInvalid gender selection.\n");
			sleep(1);
			continue;
		}
		clearInputBuffer();

		strcpy(p.gender, (genderChoice == 1) ? "Male" : "Female");

		printf("\nEnter disease or diagnosis: ");
		if (fgets(p.disease, sizeof(p.disease), stdin) == NULL)
			continue;
		p.disease[strcspn(p.disease, "\n")] = '\0';

		if (strlen(p.disease) == 0)
		{
			printf("Disease cannot be empty.\n");
			sleep(1);
			continue;
		}

		printf("\nMedical Result:\n");
		printf("[1] Positive\n");
		printf("[2] Negative\n");
		printf("Enter choice: ");
		if (scanf("%d", &resultChoice) != 1 || resultChoice < 1 || resultChoice > 2)
		{
			clearInputBuffer();
			printf("\nInvalid input for result.\n");
			sleep(1);
			continue;
		}
		clearInputBuffer();
		strcpy(p.medicalResults, (resultChoice == 1) ? "Positive" : "Negative");
		break;
	}

	while (1)
	{
		clearScreen();
		printf("\n==========================================================\n");
		printf("                      AVAILABLE ROOMS\n");
		viewRooms();
		printf("==========================================================\n");

		printf("Select Room: ");
		if (scanf("%d", &roomChoice) != 1)
		{
			clearInputBuffer();
			printf("Invalid room selection.\n");
			sleep(1);
			continue;
		}
		clearInputBuffer();

		if (roomChoice < 1 || roomChoice > MAX_ROOMS)
		{
			printf("Invalid room number! Please select between 1 and %d\n", MAX_ROOMS);
			sleep(1);
			continue;
		}

		int index = roomChoice - 1;

		if (RoomNumber[index] == 0)
		{
			RoomNumber[index] = 1;
			p.assignedRoom = roomChoice;
			printf("Room %d assigned successfully!\n", roomChoice);
			sleep(1);
		}
		else
		{
			printf("Sorry, that room is already occupied.\n");
			sleep(1);
			continue;
		}

		patientBills(&p);
		patients[patientCount++] = p;
		patientInfo(p);
		break;
	}

	fprintf(fp, "==========================================================\n");
	fprintf(fp, "%-20s: %d\n", "ID", p.id);
	fprintf(fp, "%-20s: %s\n", "Name", p.name);
	fprintf(fp, "%-20s: %d\n", "Age", p.age);
	fprintf(fp, "%-20s: %s\n", "Gender", p.gender);
	fprintf(fp, "%-20s: %s\n", "Disease", p.disease);
	fprintf(fp, "%-20s: %s\n", "Medical Results", p.medicalResults);
	fprintf(fp, "%-20s: %d\n", "Assigned Room", p.assignedRoom);
	fprintf(fp, "%-20s: %s\n", "Admit Date", p.admitDate);
	fprintf(fp, "%-20s: %.2f\n", "Bill Amount", p.billAmount);
	fprintf(fp, "%-20s: %s\n", "Payment", p.isPaid ? "PAID" : "UNPAID");
	fprintf(fp, "%-20s: %s\n", "Discharge", p.isDischarged ? p.dischargeDate : "Pending");
	fprintf(fp, "==========================================================\n\n");

	fclose(fp);
	patientID++;

	printf("\nPatient registered successfully!\n");
	sleep(1);
}

void viewAllPatients()
{
	clearScreen();
	printf("=======================================================================================================================================================================\n");
	printf("                                                               PATIENT'S INFORMATION\n");
	printf("=======================================================================================================================================================================\n");

	if (patientCount == 0)
	{
		printf("No patients registered yet.\n");
		return;
	}


	printf("%-5s %-30s %-5s %-8s %-20s %-10s %-8s %-20s %-15s %-10s %-12s\n",
	       "ID", "Patient Name", "Age", "Gender", "Disease", "Result",
	       "Room", "Admit Date", "Patient Bill", "Payment", "Discharge");

	printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < patientID; i++)
	{
		Patient p = patients[i];

		printf("%-5d %-30s %-5d %-8s %-20s %-10s %-8d %-20s %-15.2f %-10s %-12s\n",
		       p.id,
		       p.name,
		       p.age,
		       p.gender,
		       p.disease,
		       p.medicalResults,
		       p.assignedRoom,
		       p.admitDate,
		       p.billAmount,
		       p.isPaid ? "PAID" : "UNPAID",
		       p.isDischarged ? p.dischargeDate : "Pending");
	}

	printf("============================================================================================================================================================================\n");
}

void dischargePatient()
{
	FILE *fp = fopen("dischargePatients.txt", "a");
	if (!fp)
	{
		printf("Error opening file!\n");
		return;
	}

	int id;

	clearScreen();
	printf("\n=========================================\n");
	printf("          PATIENT DISCHARGE MENU\n");
	printf("=========================================\n");
	printf("Enter Patient ID for discharge: ");

	if (scanf("%d", &id) != 1)
	{
		clearInputBuffer();
		printf("\n Invalid input. Please enter a valid numeric ID.\n");
		printf("=========================================\n");
		return;
	}
	clearInputBuffer();

	if (id <= 0 || id > patientCount)
	{
		printf("\n Invalid ID! No patient found with that number.\n");
		printf("=========================================\n");
		return;
	}

	Patient *p = &patients[id - 1];

	printf("\n=============================================================\n");
	printf("Checking patient record...\n");
	printf("=============================================================\n");

	if (p->isDischarged)
	{
		printf("Patient '%s' has already been discharged on %s.\n", p->name, p->dischargeDate);
		printf("===========================================================\n");
		return;
	}

	if (!p->isPaid)
	{
		printf("Cannot discharge patient '%s' until the bill is fully paid.\n", p->name);
		printf("Outstanding Balance: %.2f\n", p->billAmount);
		printf("=============================================================\n");
		return;
	}

	getCurrentDateTime(p->dischargeDate, sizeof(p->dischargeDate));
	p->isDischarged = 1;

	printf("\n Patient Discharge Successful!\n");
	printf("==========================================================\n");
	printf("Patient ID     : %d\n", p->id);
	printf("Name           : %s\n", p->name);
	printf("Disease        : %s\n", p->disease);
	printf("Gender         : %s\n", (p->gender == 0) ? "Female" : "Male");
	printf("Assigned Room  : %d\n", p->assignedRoom);
	printf("Bill Paid      : %.2f\n", p->billAmount);
	printf("Discharge Date : %s\n", p->dischargeDate);
	printf("==========================================================\n");

	fprintf(fp,"==========================================================\n");
	fprintf(fp,"Patient ID     : %d\n", p->id);
	fprintf(fp,"Name           : %s\n", p->name);
	fprintf(fp,"Disease        : %s\n", p->disease);
	fprintf(fp,"Gender         : %s\n", (p->gender == 0) ? "Female" : "Male");
	fprintf(fp,"Assigned Room  : %d\n", p->assignedRoom);
	fprintf(fp,"Bill Paid      : %.2f\n", p->billAmount);
	fprintf(fp,"Discharge Date : %s\n", p->dischargeDate);
	fprintf(fp,"==========================================================\n");

	patientDischarge++;
	patientCount--;

	int index = p->assignedRoom - 1;
	if (index >= 0 && index < MAX_ROOMS)
	{
		RoomNumber[index] = 0;
		printf("Room %d is now available.\n", p->assignedRoom);
	}
	fclose(fp);
}

void viewDischargePatient()
{
	clearScreen();
	printf("=======================================================================================================================================================================\n");
	printf("                                                               PATIENT'S DISCHARGE SUMMARY\n");
	printf("=======================================================================================================================================================================\n");

	if (patientDischarge == 0)
	{
		printf("No patients discharged yet.\n");
		return;
	}


	printf("%-5s %-30s %-5s %-8s %-20s %-10s %-8s %-20s %-15s %-10s %-12s\n",
	       "ID", "Patient Name", "Age", "Gender", "Disease", "Result",
	       "Room", "Admit Date", "Patient Bill", "Payment", "Discharge");

	printf("----------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < patientDischarge; i++)
	{
		Patient p = patients[i];

		printf("%-5d %-30s %-5d %-8s %-20s %-10s %-8d %-20s %-15.2f %-10s %-12s\n",
		       p.id,
		       p.name,
		       p.age,
		       p.gender,
		       p.disease,
		       p.medicalResults,
		       p.assignedRoom,
		       p.admitDate,
		       p.billAmount,
		       p.isPaid ? "PAID" : "UNPAID",
		       p.isDischarged ? p.dischargeDate : "Pending");
	}

	printf("============================================================================================================================================================================\n");
}

void patientPayment()
{
	FILE *fp = fopen("Payment.txt", "a");
	if (!fp)
	{
		printf("Error opening Payment.txt\n");
		return;
	}

	int id;
	float payment;

	clearScreen();
	printf("\n=========================================\n");
	printf("            PATIENT PAYMENT MENU\n");
	printf("=========================================\n");
	printf("Enter Patient ID for payment: ");

	if (scanf("%d", &id) != 1)
	{
		clearInputBuffer();
		printf("\nInvalid input. Please enter a Patient ID.\n");
		printf("=========================================\n");
		return;
	}
	clearInputBuffer();

	if (id <= 0 || id > patientID)
	{
		printf("\nInvalid ID!\n");
		printf("=========================================\n");
		return;
	}

	Patient *p = &patients[id - 1];

	if (p->isPaid)
	{
		printf("\nPayment already marked as PAID.\n");
		printf("Thank you, no further payment is required.\n");
		printf("=========================================\n");
		return;
	}

	float balance = p->billAmount - p->totalPaid;

	printf("Total Bill Amount     : %.2f\n", p->billAmount);
	printf("Amount Previously Paid: %.2f\n", p->totalPaid);
	printf("Remaining Balance     : %.2f\n", balance);
	printf("-----------------------------------------\n");
	printf("Enter payment amount: ");

	if (scanf("%f", &payment) != 1)
	{
		clearInputBuffer();
		printf("\nInvalid input. Please enter a numeric amount.\n");
		printf("=========================================\n");
		return;
	}
	clearInputBuffer();

	if (payment <= 0)
	{
		printf("\nPayment must be greater than 0.\n");
		printf("=========================================\n");
		return;
	}

	if (payment > balance)
	{
		printf("\nPayment exceeds balance! Please pay only %.2f or less.\n", balance);
		printf("=========================================\n");
		return;
	}

	char date[50];
	getCurrentDateTime(date, sizeof(date));

	p->payments[p->paymentCount] = payment;
	strcpy(p->paymentDates[p->paymentCount], date);
	p->paymentCount++;

	p->totalPaid += payment;
	p->balance = p->billAmount - p->totalPaid;

	if (p->balance <= 0)
	{
		p->isPaid = 1;
		p->balance = 0;
		printf("\nPayment Complete! The bill is fully paid.\n");
	}
	else
	{
		printf("\nPartial payment accepted.\n");
		printf("Remaining Balance: %.2f\n", p->balance);
	}

	fprintf(fp, "Date: %s\n", date);
	fprintf(fp, "Patient ID: %d\n", p->id);
	fprintf(fp, "Name: %s\n", p->name);
	fprintf(fp, "Disease: %s\n", p->disease);
	fprintf(fp, "Amount Paid This Time: %.2f\n", payment);
	fprintf(fp, "Total Paid: %.2f\n", p->totalPaid);
	fprintf(fp, "Remaining Balance: %.2f\n", p->balance);
	fprintf(fp, "Status: %s\n", p->isPaid ? "PAID" : "INSTALLMENT (UNPAID)");
	fprintf(fp, "-----------------------------------------\n");

	fclose(fp);

	printf("-----------------------------------------\n");
	printf("              PAYMENT RECEIPT\n");
	printf("-----------------------------------------\n");
	printf("Patient ID     : %d\n", p->id);
	printf("Name           : %s\n", p->name);
	printf("Disease        : %s\n", p->disease);
	printf("Amount Due     : %.2f\n", p->billAmount);
	printf("Paid This Time : %.2f\n", payment);
	printf("Total Paid     : %.2f\n", p->totalPaid);
	printf("Balance        : %.2f\n", p->balance);
	printf("Status         : %s\n", p->isPaid ? "PAID" : "INSTALLMENT");
	printf("Date Processed : %s\n", date);
	printf("=========================================\n");

	transactionCount++;

	if (p->isPaid)
		patientPaymentPaid++;

	printf("\nPress Enter to return to the main menu...");
	getchar();
}

void viewTransactionHistory()
{
	clearScreen();
	printf("===============================================================================================================================\n");
	printf("                                           PATIENT TRANSACTION HISTORY\n");
	printf("===============================================================================================================================\n");

	if (transactionCount == 0)
	{
		printf("No transactions yet.\n");
		printf("===============================================================================================================================\n");
		return;
	}

	for (int i = 0; i < patientID; i++)
	{
		Patient p = patients[i];

		printf("\n---------------------------------------------------------------------------------------------------------------\n");
		printf("%-12s: %-5d  %-10s: %-15s  %-10s: %-15s  %-12s: %-10.2f  %-12s: %-10.2f  %-10s: %-10.2f  %-10s: %s\n",
       "Patient ID", p.id,
       "Name", p.name,
       "Disease", p.disease,
       "Total Bill", p.billAmount,
       "Total Paid", p.totalPaid,
       "Balance", p.balance,
       "Status", p.isPaid ? "PAID" : "UNPAID");
		printf("---------------------------------------------------------------------------------------------------------------\n");

		printf("%-5s %-15s %-25s\n", "No.", "Amount Paid", "Date");
		printf("---------------------------------------------------------------------------------------------------------------\n");

		if (p.paymentCount == 0)
		{
			printf("%-5s %-15s %-25s\n", "-", "No payments yet", "-");
		}
		else
		{
			for (int j = 0; j < p.paymentCount; j++)
			{
				printf("%-5d %-15.2f %-25s\n", j + 1, p.payments[j], p.paymentDates[j]);
			}
		}

		if (p.isDischarged)
		{
			printf("---------------------------------------------------------------------------------------------------------------\n");
			printf("Discharge Date: %s\n", p.dischargeDate);
		}

		printf("===============================================================================================================\n");
	}
}

void addDoctor()
{
	FILE *fp = fopen("Doctor.txt", "a");
	if (!fp)
	{
		printf("Error Opening This File\n");
		return;
	}
	int choice;

	if (doctorCount >= MAX_DOCTORS)
	{
		printf("Doctor list full.\n");
		return;
	}

	printf("\n=====================================\n");
	printf("            SPECIALTIES               \n");
	printf("=====================================\n");
	printf("[ 1 ]  Emergency Medicine\n");
	printf("[ 2 ]  Internal Medicine\n");
	printf("[ 3 ]  General Surgeon\n");
	printf("[ 4 ]  Anesthesiologist\n");
	printf("[ 5 ]  Pediatrician\n");
	printf("[ 6 ]  Obstetrics and Gynecology (OB-GYN)\n");
	printf("[ 7 ]  Cardiologist\n");
	printf("[ 8 ]  Radiologist\n");
	printf("[ 9 ]  Critical Care (ICU Specialist)\n");
	printf("[10 ]  Orthopedic Surgeon\n");
	printf("=====================================\n");
	printf("Enter your choice (1-10): ");

	if (scanf("%d", &choice) != 1 || choice < 1 || choice > 10)
	{
		clearInputBuffer();
		printf("Invalid input. Please enter a number between 1 and 10.\n");
		return;
	}
	clearInputBuffer();

	printf("\nName: ");
	if (fgets(DoctorName[doctorCount], sizeof(DoctorName[doctorCount]), stdin) == NULL)
		return;
	DoctorName[doctorCount][strcspn(DoctorName[doctorCount], "\n")] = '\0';

	const char *specialties[] = {
		"Emergency Medicine",
		"Internal Medicine",
		"General Surgeon",
		"Anesthesiologist",
		"Pediatrician",
		"Obstetrics and Gynecology (OB-GYN)",
		"Cardiologist",
		"Radiologist",
		"Critical Care (ICU Specialist)",
		"Orthopedic Surgeon"
	};

	strcpy(DoctorSpec[doctorCount], specialties[choice - 1]);

	fprintf(fp, "Name: %s\n", DoctorName[doctorCount]);
	fprintf(fp, "Specialty: %s\n", DoctorSpec[doctorCount]);
	fprintf(fp, "-----------------------------------\n");

	doctorCount++;
	printf("Doctor added successfully!\n");
	fclose(fp);
}

void viewDoctors()
{
	printf("\n==============================================================\n");
	printf("                        DOCTOR LIST\n");
	printf("==============================================================\n");

	if (doctorCount == 0)
	{
		printf("No doctors have been added yet.\n");
		printf("=========================================================\n");
		return;
	}

	printf("%-5s %-28s %-30s\n", "No.", "Doctor Name", "Specialization");
	printf("-------------------------------------------------------------\n");

	for (int i = 0; i < doctorCount; i++)
	{
		printf("%-5d Dr.%-25s %-30s\n", i + 1, DoctorName[i], DoctorSpec[i]);
	}

	printf("==============================================================\n");
}

void addStaff()
{
	FILE *fp = fopen("AddStaff.txt", "a");
	if (!fp)
	{
		printf("Error Opening This File\n");
		return;
	}

	int choice;

	if (staffCount >= MAX_STAFF)
	{
		printf("Staff list full.\n");
		return;
	}

	clearScreen();
	printf("\n=======================================\n");
	printf("           ADD STAFF MEMBER\n");
	printf("========================================\n");
	printf("Medical Staff\n");
	printf("[ 1 ]  General Practitioner (GP)\n");
	printf("[ 2 ]  Pharmacist\n");
	printf("[ 3 ]  Phlebotomists\n");
	printf("[ 4 ]  Physical Therapist\n");
	printf("[ 5 ]  Registered Nurses (RNs)\n\n");

	printf("Technical Staff\n");
	printf("[ 6 ]  ECG Technician\n");
	printf("[ 7 ]  Laboratory Technician\n");
	printf("[ 8 ]  Medical Technologist\n");
	printf("[ 9 ]  Radiologic Technologist\n");
	printf("[ 10 ]  Ultrasound Technologist\n\n");

	printf("Administrative Staff\n");
	printf("[ 11 ]  Administrative Assistant\n");
	printf("[ 12 ]  Billing Clerk\n");
	printf("[ 13 ]  Hospital Clerk\n");
	printf("[ 14 ]  Records Officer\n");
	printf("[ 15 ]  Receptionist / Front Desk Officer\n");
	printf("[ 16 ]  Medical Receptionist\n\n");

	printf("Support Staff\n");
	printf("[ 17 ]  Ambulance Driver\n");
	printf("[ 18 ]  Cook / Food Service Worker\n");
	printf("[ 19 ]  Electrician / Maintenance Worker\n");
	printf("[ 20 ]  Housekeeper\n");
	printf("[ 21 ]  Janitor\n");
	printf("[ 22 ]  Laundry Staff\n");
	printf("[ 23 ]  Security Guard\n\n");
	printf("==========================================\n");
	printf("Enter your choice (1-23): ");

	if (scanf("%d", &choice) != 1 || choice < 1 || choice > 23)
	{
		clearInputBuffer();
		printf("Invalid input. Please enter a number between 1 and 23.\n");
		return;
	}
	clearInputBuffer();

	clearScreen();
	printf("\n=======================================\n");
	printf("           STAFF INFORMATION\n");
	printf("========================================\n");
	printf("\nName: ");
	if (fgets(StaffName[staffCount], sizeof(StaffName[staffCount]), stdin) == NULL)
		return;
	StaffName[staffCount][strcspn(StaffName[staffCount], "\n")] = '\0';

	const char *roles[] = {
		"General Practitioner (GP)",
		"Pharmacist",
		"Phlebotomists",
		"Physical Therapist",
		"Registered Nurses (RNs)",
		"ECG Technician",
		"Laboratory Technician",
		"Medical Technologist",
		"Radiologic Technologist",
		"Ultrasound Technologist",
		"Administrative Assistant",
		"Billing Clerk",
		"Hospital Clerk",
		"Records Officer",
		"Receptionist / Front Desk Officer",
		"Medical Receptionist",
		"Ambulance Driver",
		"Cook / Food Service Worker",
		"Electrician / Maintenance Worker",
		"Housekeeper",
		"Janitor",
		"Laundry Staff",
		"Security Guard"
	};

	strcpy(StaffRole[staffCount], roles[choice - 1]);

	fprintf(fp, "Name: %s\n", StaffName[staffCount]);
	fprintf(fp, "Role: %s\n", roles[choice - 1]);
	fprintf(fp, "---------------------------------------------\n");

	staffCount++;
	printf("\nStaff member added successfully!\n");
	printf("=================================================\n");
	fclose(fp);
}

void viewStaff()
{
	printf("\n==============================================================\n");
	printf("                          STAFF LIST\n");
	printf("==============================================================\n");

	if (staffCount == 0)
	{
		printf("No staff members have been added yet.\n");
		printf("==========================================================\n");
		return;
	}

	printf("%-5s %-25s %-30s\n", "No.", "Staff Name", "Role/Position");
	printf("-------------------------------------------------------------\n");

	for (int i = 0; i < staffCount; i++)
	{
		printf("%-5d %-25s %-30s\n", i + 1, StaffName[i], StaffRole[i]);
	}

	printf("=============================================================\n");
}

void userViewPatientInfo()
{
	clearScreen();
	printf("\n================ PATIENT'S INFORMATION ================\n");

	int index = ID - 1;

	Patient p = patients[index];

	printf("==========================================================\n");
	printf("%-20s: %d\n", "ID", p.id);
	printf("%-20s: %s\n", "Name", p.name);
	printf("%-20s: %d\n", "Age", p.age);
	printf("%-20s: %s\n", "Gender", p.gender);
	printf("%-20s: %s\n", "Disease", p.disease);
	printf("%-20s: %s\n", "Medical Results", p.medicalResults);
	printf("%-20s: %d\n", "Assigned Room", p.assignedRoom);
	printf("%-20s: %s\n", "Admit Date", p.admitDate);
	printf("==========================================================\n");
}

void userViewDischargeSummary()
{
	clearScreen();
	printf("\n========== PATIENTS DISCHARGE SUMMARY ===============\n");

	if (patientPaymentPaid == 0)
	{
		printf("Patients Cannot Be Discharged Yet. Payment Muna\n");
		return;
	}

	int index = ID - 1;

	Patient p = patients[index];

	printf("%-20s: %d\n", "Patient ID", p.id);
	printf("%-20s: %s\n", "Name", p.name);
	printf("%-20s: %d\n", "Age", p.age);
	printf("%-20s: %s\n", "Gender", p.gender);
	printf("%-20s: %s\n", "Disease", p.disease);
	printf("%-20s: %s\n", "Medical Results", p.medicalResults);
	printf("%-20s: %d\n", "Assigned Room", p.assignedRoom);
	printf("%-20s: %s\n", "Admit Date", p.admitDate);
	printf("%-20s: %s\n", "Discharge Date", p.dischargeDate);
	printf("%-20s: %.2f\n", "Bill Amount", p.billAmount);
	printf("%-20s: %s\n", "Payment Status", p.isPaid ? "PAID" : "UNPAID");
	printf("====================================================\n");
}

void userMakePayment()
{
    float payment;

    int index = ID - 1;

    if (index < 0 || index >= patientID)
    {
        printf("\nInvalid Patient ID!\n");
        printf("=========================================\n");
        return;
    }

    Patient *p = &patients[index];

    clearScreen();
    printf("\n=========================================\n");
    printf("            PATIENT PAYMENT MENU\n");
    printf("=========================================\n");
    printf("%-20s: %d\n", "ID", p->id);
    printf("%-20s: %s\n", "Name", p->name);
    printf("=========================================\n");

    if (p->isPaid)
    {
        printf("\nPayment already marked as PAID.\n");
        printf("Thank you, no further payment is required.\n");
        printf("=========================================\n");
        return;
    }

    float balance = p->billAmount - p->totalPaid;

    printf("Total Bill Amount     : %.2f\n", p->billAmount);
    printf("Amount Previously Paid: %.2f\n", p->totalPaid);
    printf("Remaining Balance     : %.2f\n", balance);
    printf("-----------------------------------------\n");

    printf("Enter Payment Amount: ");
    if (scanf("%f", &payment) != 1 || payment <= 0)
    {
        printf("\nPayment must be greater than 0.\n");
        printf("=========================================\n");
        clearInputBuffer();
        return;
    }

    if (payment > balance)
    {
        printf("\nPayment exceeds balance! Please pay only %.2f or less.\n", balance);
        printf("=========================================\n");
        return;
    }

    char date[50];
    getCurrentDateTime(date, sizeof(date));

    p->payments[p->paymentCount] = payment;
    strcpy(p->paymentDates[p->paymentCount], date);
    p->paymentCount++;

    p->totalPaid += payment;
    p->balance = p->billAmount - p->totalPaid;

    if (p->balance <= 0)
    {
        p->isPaid = 1;
        p->balance = 0;
        printf("\nPayment Complete! The bill is fully paid.\n");
    }
    else
    {
        printf("\nPartial payment accepted.\n");
        printf("Remaining Balance: %.2f\n", p->balance);
    }

    printf("-----------------------------------------\n");
    printf("              PAYMENT RECEIPT\n");
    printf("-----------------------------------------\n");
    printf("%-15s: %d\n", "Patient ID", p->id);
    printf("%-15s: %s\n", "Name", p->name);
    printf("%-15s: %s\n", "Disease", p->disease);
    printf("%-15s: %.2f\n", "Amount Due", p->billAmount);
    printf("%-15s: %.2f\n", "Paid This Time", payment);
    printf("%-15s: %.2f\n", "Total Paid", p->totalPaid);
    printf("%-15s: %.2f\n", "Balance", p->balance);
    printf("%-15s: %s\n", "Status", p->isPaid ? "PAID" : "INSTALLMENT");
    printf("%-15s: %s\n", "Date Processed", date);
    printf("=========================================\n");

    patientTransactionCount++;

    if (p->isPaid)
        patientPaymentPaid++;

    printf("\nPress Enter to return to the main menu...");
    clearInputBuffer();
    getchar();
}

void userViewTransaction()
{
    clearScreen();
	printf("===============================================================================================================================\n");
	printf("                                           PATIENT TRANSACTION HISTORY\n");
	printf("===============================================================================================================================\n");

	if (patientTransactionCount == 0)
	{
		printf("No Transaction yet.\n");
		printf("===============================================================================================================================\n");
		return;
	}

		int index = ID - 1;

	    Patient p = patients[index];

		printf("\n---------------------------------------------------------------------------------------------------------------\n");
		printf("%-12s: %-5d  %-10s: %-15s  %-10s: %-15s  %-12s: %-10.2f  %-12s: %-10.2f  %-10s: %-10.2f  %-10s: %s\n",
       "Patient ID", p.id,
       "Name", p.name,
       "Disease", p.disease,
       "Total Bill", p.billAmount,
       "Total Paid", p.totalPaid,
       "Balance", p.balance,
       "Status", p.isPaid ? "PAID" : "UNPAID");
		printf("---------------------------------------------------------------------------------------------------------------\n");

		printf("%-5s %-15s %-25s\n", "No.", "Amount Paid", "Date");
		printf("---------------------------------------------------------------------------------------------------------------\n");

		if (p.paymentCount == 0)
		{
			printf("%-5s %-15s %-25s\n", "-", "No payments yet", "-");
		}
		else
		{
			for (int i = 0; i < p.paymentCount; i++)
			{
				printf("%-5d %-15.2f %-25s\n", i + 1, p.payments[i], p.paymentDates[i]);
			}
		}

		if (p.isDischarged)
		{
			printf("---------------------------------------------------------------------------------------------------------------\n");
			printf("Discharge Date: %s\n", p.dischargeDate);
		}

		printf("===============================================================================================================\n");
}

void adminMenu()
{
	int choice;
	while (1)
	{
		clearScreen();

		printf("\n==================================================== ");
		printf("\n              Hospital Management                     ");
		printf("\n====================================================\n");
		printf("[ 1 ]  Register New Patient\n");
		printf("[ 2 ]  View All Patients\n");
		printf("[ 3 ]  Discharge Patient\n");
		printf("[ 4 ]  View Discharge Patients\n");
		printf("[ 5 ]  Patient Payment\n");
		printf("[ 6 ]  View Transaction History\n");
		printf("[ 7 ]  View Rooms\n");
		printf("[ 8 ]  Add Doctor\n");
		printf("[ 9 ]  View Doctors\n");
		printf("[ 10]  Add Staff\n");
		printf("[ 11]  View Staff\n");
		printf("[ 12]  Logout\n");
		printf("\nSelection: ");

		if (scanf("%d", &choice) != 1)
		{
			clearInputBuffer();
			printf("Invalid input.\n");
			sleep(2);
			continue;
		}
		clearInputBuffer();

		switch (choice)
		{
		case 1:
			registerPatient();
			break;
		case 2:
			viewAllPatients();
			break;
		case 3:
			dischargePatient();
			break;
		case 4:
			viewDischargePatient();
			break;
		case 5:
			patientPayment();
			break;
		case 6:
			viewTransactionHistory();
			break;
		case 7:
			viewRooms();
			break;
		case 8:
			addDoctor();
			break;
		case 9:
			viewDoctors();
			break;
		case 10:
			addStaff();
			break;
		case 11:
			viewStaff();
			break;
		case 12:
			return;
		default:
			printf("Invalid Choice!\n");
		}

		printf("\nPress Enter to continue...");
		getchar();
	}
}

void userMenu()
{
	int choice;
	while (1)
	{
		clearScreen();
		printf("\n==================================================== ");
		printf("\n              Hospital Management                     ");
		printf("\n====================================================\n");
		printf("[1] Patient Information\n");
		printf("[2] View Discharge Summary\n");
		printf("[3] Make Payment\n");
		printf("[4] View Transaction History\n");
		printf("[5] Logout\n");
		printf("\nSelections: ");

		if (scanf("%d", &choice) != 1)
		{
			clearInputBuffer();
			printf("Invalid Input.\n");
			sleep(1);
			continue;
		}
		clearInputBuffer();

		switch (choice)
		{
		case 1:
			userViewPatientInfo();
			break;
		case 2:
			userViewDischargeSummary();
			break;
		case 3:
			userMakePayment();
			break;
		case 4:
			userViewTransaction();
			break;
		case 5:
			return;
		default:
			printf("Invalid Choice\n");
		}

		printf("\nPress Enter to continue...");
		getchar();
	}
}

void createAccount()
{
	if (accountCount >= MAX_ACCOUNTS) {
		printf("\nAccount list full!\n");
		return;
	}

	char username[20], password[20];

	clearScreen();
	printf("\n==================================================== ");
	printf("\n              CREATE ACCOUNT                         ");
	printf("\n====================================================\n");

	printf("\nEnter Username: ");
	scanf("%49s", username);
	printf("Enter Password: ");
	scanf("%49s", password);

	if (patientID == 0)
	{
		printf("\nNo Patient Yet\n");
		sleep(1);
		return;
	}

	printf("\nAccount created successfully!\n");

	strcpy(accounts[accountCount].username, username);
	strcpy(accounts[accountCount].password, password);
	accountCount++;
	sleep(1);
}

void userLogin()
{
	char username[20], password[20];
	int found = 0;

	clearScreen();
	printf("\n==================================================== ");
	printf("\n                    LOGIN                            ");
	printf("\n====================================================\n");

	printf("\nEnter Username: ");
	scanf("%19s", username);
	printf("Enter Password: ");
	scanf("%19s", password);
	printf("Enter Patient ID: ");
	if (scanf("%d", &ID) != 1)
	{
		clearInputBuffer();
		printf("\nInvalid input! Please enter a number.\n");
		return;
	}
	clearInputBuffer();

	if (patientID <= 0 || patientID > patientCount)
	{
		printf("\nThe Patient ID you entered is not available.\n");
		return;
	}

	for (int i = 0; i < accountCount; i++) {
		if (strcmp(username, accounts[i].username) == 0 &&
		        strcmp(password, accounts[i].password) == 0) {
			found = 1;
			break;
		}
	}

	if (found)
	{
		userMenu();
	}
	else
	{
		printf("\nInvalid username or password.\n");
	}
}

int main()
{
	int choice;
	char username[20], password[20];

	while (1)
	{
		clearScreen();
		printf("\n==================================================== ");
		printf("\n              Hospital Management System              ");
		printf("\n====================================================\n");
		printf("\n[ 1 ] USER");
		printf("\n[ 2 ] ADMIN\n");
		printf("\nEnter your choice: ");

		if (scanf("%d", &choice) != 1)
		{
			clearInputBuffer();
			printf("Invalid input! Please enter a number.\n");
			continue;
		}
		clearInputBuffer();

		switch (choice)
		{
		case 1:

			clearScreen();
			printf("\n==================================================== ");
			printf("\n              Hospital Management System              ");
			printf("\n====================================================\n");
			printf("\n[ 1 ] CREATE ACCOUNT");
			printf("\n[ 2 ] LOGIN\n");
			printf("\n[ 3 ] EXIT\n");
			printf("\nEnter your choice: ");

			if (scanf("%d", &choice) != 1)
			{
				clearInputBuffer();
				printf("Invalid input! Please enter a number.\n");
				continue;
			}
			clearInputBuffer();

			switch (choice)
			{
			case 1:
				createAccount();
				break;
			case 2:
				userLogin();
				break;
			case 3:
				break;
			default:
				printf("Invalid Choice\n");
			}
			break;

		case 2:
			clearScreen();
			printf("\n==================================================== ");
			printf("\n                    ADMIN              ");
			printf("\n====================================================\n");
			printf("\nEnter Username: ");
			scanf("%19s", username);
			printf("Enter Password: ");
			scanf("%19s", password);

			if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0)
			{
				adminMenu();
			}
			else
			{
				printf("Invalid username or password!\n");
				sleep(2);
			}
			break;

		default:
			printf("Invalid choice! Please select 1 or 2.\n");
		}

		printf("\nPress Enter to continue...");
		getchar();
	}

	return 0;
}
