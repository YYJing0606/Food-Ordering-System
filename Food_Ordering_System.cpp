#include <iostream>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <fstream>
#include <sstream>
#include <string>

#define AMOUNT 50			// Maximum Amount of Menu
#define MAX_TABLE 30		// Maximum Number of Table

using namespace std;

typedef struct
{
	char Code[5];			// Dish Code
	char Name[20];			// Dish Name
	double Price;			// Dish Price
	int OQty;				// Current Order Qty
	double TPrice;			// Total Price per Order
	int TPre_Time;			// Total Preparation Time per Order
	double Over_Sale;		// Daily Overall Sale
	int Over_Qty;			// Daily Order Overall Qty
	int Over_Pre_Time;		// Overall Daily Preparation Time
} FOOD;						// Food Type

typedef struct
{
	int Menu_Size;			// Menu Size
	int Dine_In_Order;		// Total Dine In Order
	int Take_Away_Order;	// Total Take Away Order
	int Total_Order;		// Total Dine In + Take Away Order
	int Date;				// Date
} STATISTICS;				// Statistics Today

typedef struct
{
	int Date[3];			// Date-Month-Year
	int Clock[2];			// Hour: Minute
} TIME;						// Time

void Read_File(FOOD Menu[], STATISTICS& Status);													// Read File Function
void Refresh();																						// Refresh Screen or Clear Screen Function
void Get_Current_Time(STATISTICS& Status, TIME& Time, string Page);									// Get Current Date & Time Function
void Check_Date(FOOD Menu[], STATISTICS& Status, TIME& Time);										// Check Date Today to Refresh Statistics Fuction
int Check_Answer(string Answer, string Slide);														// Check Answer is What Problem Wrong Function
void Dish(FOOD Menu[], STATISTICS& Status);															// Display Menu Function
void Main_Menu(FOOD Menu[], STATISTICS& Status, TIME& Time);										// Main Menu Screen Function
void Dish_Edit(FOOD Menu[], STATISTICS& Status);													// Edit Dish Screen Fucntion
void Add_Menu(FOOD Menu[], STATISTICS& Status);														// Add Menu Screen Function
string Check_Code();																				// Check Requirement of Dish Code Function
double Get_Price();																					// Get & Check Price Function
void Plus_Menu(FOOD Menu[], STATISTICS& Status, string Code, string Name, double Price);			// Adding Dish Process Function
void Drop_Menu(FOOD Menu[], STATISTICS& Status);													// Drop Menu Screen Function
string Double_Confirm(string Page);																	// Double Confirm the Selection Process Function
void Delete_Menu(FOOD Menu[], STATISTICS& Status, string Code, int index);							// Deleting Menu Process Function
void Order(FOOD Menu[], STATISTICS& Status, TIME& Time);											// Customer Order System Screen Function
void Take_Order(FOOD Menu[], STATISTICS& Status);													// Customer Take or Add Dish Screen Function
void Cancel(FOOD Menu[], STATISTICS& Status);														// Customer Delete or Drop Dish Screen Function
int Drop_Dish(FOOD Menu[], string Code, int index, int Qty);										// Customer Cancelling Dish Process Function
int Confirm_Drop(FOOD Menu[], int index, int Qty, string Slide);									// Get Confirmation from Customer to Avoid Careless Drop Dish Function
void Confirm_Return(string Page);																	// Get Confirmation from Customer to Exit Compute Screen Function
void Compute(FOOD Menu[], STATISTICS& Status, TIME& Time);											// Customer Check Price Screen Function
void Statistics(FOOD Menu[], STATISTICS& Status, TIME& Time);										// Bossku Check Statistics Today Screen Function

int main()
{
	FOOD Menu[AMOUNT];					// Create Menu
	STATISTICS Status;					// Create Status
	TIME Time;							// Create Time
	Time.Date[0] = 0;					// Date
	Time.Date[1] = 0;					// Month
	Time.Date[2] = 0;					// Year
	Time.Clock[0] = 0;					// Hour
	Time.Clock[1] = 0;					// Minute

	Read_File(Menu, Status);			// Read File Function
	Main_Menu(Menu, Status, Time);		// Turn to Main Menu

	return 0;
}

void Read_File(FOOD Menu[], STATISTICS& Status)
{
	ifstream in_status("Status.txt");																// Open & Read File

	if (!in_status)																					// If Cannot Open File Return Warning
		cout << "Error: Could not open file 'Status.txt'." << endl;
	else
	{
		in_status >> Status.Menu_Size >> Status.Dine_In_Order
				>> Status.Take_Away_Order >> Status.Total_Order >> Status.Date;						// Write from Data to Program
		in_status.close();																			// Close File
	}

	ifstream in_food("Food2.txt");																	// Open & Read File

	if (!in_food)																					// If Cannot Open File Return Warning
		cout << "Error: Could not open file 'Food2.txt'." << endl;
	else
	{
		int index = 0;
		string line;

		while (index < Status.Menu_Size && getline(in_food, line))									// Write from Data to Program
		{
			istringstream lineStream(line);
			string token;

			getline(lineStream, token, '|');
			strncpy_s(Menu[index].Code, token.c_str(), sizeof(Menu[index].Code));
			Menu[index].Code[sizeof(Menu[index].Code) - 1] = '\0';									// Ensure null-termination

			getline(lineStream, token, '|');
			strncpy_s(Menu[index].Name, token.c_str(), sizeof(Menu[index].Name));
			Menu[index].Name[sizeof(Menu[index].Name) - 1] = '\0';									// Ensure null-termination

			getline(lineStream, token, '|');
			Menu[index].Price = stod(token);

			getline(lineStream, token, '|');
			Menu[index].OQty = stoi(token);

			getline(lineStream, token, '|');
			Menu[index].TPrice = stoi(token);

			getline(lineStream, token, '|');
			Menu[index].TPre_Time = stoi(token);

			getline(lineStream, token, '|');
			Menu[index].Over_Sale = stoi(token);

			getline(lineStream, token, '|');
			Menu[index].Over_Qty = stoi(token);

			getline(lineStream, token, '|');
			Menu[index].Over_Pre_Time = stoi(token);

			index++;
		}

		in_food.close();																			// Close File
	}
}

void Refresh()
{
	cout << "\033[H\033[J";				// Refresh Screen
	cout << "\033[2J\033[1;1H";
}

void Get_Current_Time(STATISTICS& Status, TIME& Time, string Page)
{
	time_t now = time(0);

	tm localTime;

	localtime_s(&localTime, &now);

	Time.Date[0] = localTime.tm_mday;																			// Store Date
	Time.Date[1] = localTime.tm_mon + 1;																		// Store Month
	Time.Date[2] = localTime.tm_year + 1900;																	// Store Year
	Time.Clock[0] = localTime.tm_hour;																			// Store Hour
	Time.Clock[1] = localTime.tm_min;																			// Store Minute

	if (Page == "Compute")																						// Check is from Compute Screen		
	{
		Status.Date = Time.Date[0];																				// Change Status Date to Current Date
		cout << "Order Date: " << Time.Date[0] << "/" << Time.Date[1] << "/" << Time.Date[2]					// Display Order Date
			<< " Order Time: " << Time.Clock[0] << ":" << Time.Clock[1] << endl;								// Display Order Time
	}
	else if (Page == "Status")																					// Check is from Statistics Screen
		cout << "Current Date: " << Time.Date[0] << "/" << Time.Date[1] << "/" << Time.Date[2]					// Display Current Date
			<< " Current Time: " << Time.Clock[0] << ":" << Time.Clock[1] << endl;								// Display Current Time
}

void Check_Date(FOOD Menu[], STATISTICS& Status, TIME& Time)
{
	string Page;

	Get_Current_Time(Status, Time, Page);																		// Get Current Date

	if (Status.Date != Time.Date[0])																			// Check Status Date is equal Current Date to Choose Refresh or Not
	{
		ofstream out_menu("Food2.txt");																			// Prepare Overwrite File
		ofstream out_status("Status.txt");																		// Prepare Overwrite File

		if (!out_menu)																							// If Cannot Open File Return Warning
			cout << "Error opening output file: Food2.txt\n";
		else if (!out_status)																					// If Cannot Open File Return Warning
			cout << "Error opening output file: Status.txt\n";
		else
		{
			for (int index = 0; index < Status.Menu_Size; index++)												// Overwrite the File Data
				out_menu << Menu[index].Code << "|" << Menu[index].Name << "|" << Menu[index].Price
						<< "|" << Menu[index].OQty << "|0|0|0|0|0" << endl;

			out_status << Status.Menu_Size << " 0 0 0 0" << endl;												// Overwrite the File Data
		}

		out_menu.close();																						// Close File
		out_status.close();																						// Close File

		Read_File(Menu, Status);																				// Open & Read File to Refresh Data & Statistics
	}
}

int Check_Answer(string Answer, string Slide)
{
	int n = -1;
	double p = -1;

	do
	{
		bool Is_True = false;																			// If False may Exit Loop, If True give Extra Feedback & Continue Loop until Correct Requirement Answer
		int count = 0, check;
		int count2 = 0, check2;
		int count3 = 0, check3;
		int count4 = 0, check4;

		if (Slide == "Number")																			// Check is from Take Order or Cancel Screen
		{
			cout << "Enter a quantity: ";
			getline(cin, Answer);																		// Enter Quantity to Add or Drop Dish
		}
		else if (Slide == "Table")																		// Check is from Compute(Tables) Screen
		{
			cout << "Enter table number(MIN 1, MAX 30): ";
			getline(cin, Answer);																		// Enter Table Number
		}

		for (int i = 0; i < Answer.length(); i++)														// Check User Answer Enter
		{
			check = isalpha(Answer[i]);																	// Check Alphabet
			if (check)
				count++;

			check2 = ispunct(Answer[i]);																// Chcek Punctuation
			if (check2)
				count2++;

			check3 = isdigit(Answer[i]);																// Check Digit
			if (check3)
				count3++;

			check4 = isspace(Answer[i]);																// Check Space
			if (check4)
				count4++;
		}

		// Display Feedback
		if (count == 1 && count2 == 0 && count3 == 0 && count4 == 0)									// Alphabet Feedback
		{
			Is_True = true;																				// Need Loop

			if (Slide == "Main" || Slide == "Number" || Slide == "Table")								// Those Not Require Alphabet Screen
				cout << "No alphabet! ";

			else if (Slide == "Menu" || Slide == "Drop_Menu" || Slide == "Order" ||						// Those may Require Alphabet but Not True Screen
					Slide == "Take" || Slide == "Cancel" || Slide == "Yes_Drop" ||
					Slide == "Return" || Slide == "Sure" || Slide == "Compute")
				cout << "Invalid alphabet! ";
		}
		else if (count2 == 1 && count == 0 && count3 == 0 && count4 == 0)								// Symbol or Punctuation Feedback
		{
			Is_True = true;																				// Need Loop
			cout << "No symbol! ";																		// No any Screen Require Symbol or Punctuation
		}
		else if ((count4 == 0 || count4 > 0) && count == 0 && count2 == 0 && count3 == 0)				// Space or NULL Feedback
		{
			Is_True = true;																				// Need Loop
			cout << "No space! ";																		// No any Screen Require Space or NULL Answer
		}
		else if (Answer[0] == '-' && count == 0 && count2 == 1 && count3 > 0)							// Negative Number Feedback
		{
			Is_True = true;																				// Need Loop
			cout << "No negative number! ";																// No any Screen Require Negative Number
		}
		else if (Answer[0] == '0' && count == 0 && count2 == 0 && count3 > 0 && count4 == 0)			// E.g. 012 not equal 12 Feedback
		{
			Is_True = true;																				// Need Loop
			cout << "Invalid number! ";																	// No any Screen Require
		}
		else if (count == 0 && count2 == 0 && count3 > 0 && count4 == 0)								// Digit Feedback
		{
			Is_True = true;

			if (Slide == "Main" || Slide == "Drop_Menu" || Slide == "Take" || Slide == "Cancel")		// Those may Require Number but Not True Screen
				cout << "Invalid number! ";

			else if (Slide == "Menu" || Slide == "Order" || Slide == "Yes_Drop" ||						// Those Not Require Number Screen
					Slide == "Return" || Slide == "Sure" || Slide == "Compute")
				cout << "No number! ";

			else if (Slide == "Number" || Slide == "Table")												// Those Require Number Screen
			{
				Is_True = false;
				n = stoi(Answer);																		// Convert string Answer to integer n
			}
		}
		else
		{
			Is_True = true;																				// If Not All Above then Judge Become Sentences
			cout << "No sentences! ";
		}

		if (Slide == "Table" && n <= 0 && n > MAX_TABLE)												// Only 1 to 30 Tables 
		{
			Is_True = true;																				// Need Loop
			cout << "Here have " << MAX_TABLE << " tables only! ";										// Display Warning & Reminder
		}

		if (Is_True)																					// Display Extra Reminder
		{
			if (Slide == "Main" || Slide == "Menu" || Slide == "Drop_Menu" ||
				Slide == "Order" || Slide == "Take" || Slide == "Cancel" ||
				Slide == "Yes_Drop" || Slide == "Return" || Slide == "Sure")
				cout << "Please enter correct selection...\n\n";

			else if (Slide == "Number")
				cout << "Please enter correct number...\n\n";

			else if (Slide == "Table")
				cout << "Please enter correct table number...\n\n";
		}

		if (Slide != "Number" && Slide != "Table" && Is_True)											// Force Exit Loop
			break;

	} while ((Slide == "Number" && n < 0) || (Slide == "Table" && n <= 0 && n > MAX_TABLE));			// Services for Take Order, Cancel or Compute(Tables) Screen

	return n;																							// Return Integer Number to Other Function
}

void Dish(FOOD Menu[], STATISTICS& Status)																// Display Dish from Menu: Code, Name, Price and Current Quantity
{
	cout << "**********************************************" << endl;
	cout << "* Dish Menu:                             Qty *" << endl;

	for (int index = 0; index < Status.Menu_Size; index++)
		cout << "* " << Menu[index].Code << ", " << left << setw(20) << Menu[index].Name
			<< ", RM" << setw(5) << fixed << setprecision(2) << Menu[index].Price << " :"
			<< right << setw(5) << Menu[index].OQty << " *" << endl;

	cout << "**********************************************" << endl;
}

void Main_Menu(FOOD Menu[], STATISTICS& Status, TIME& Time)
{
	string Choice, Slide = "Main";

	do
	{
		bool Exit = true;																		// False will Exit, True will Loop

		for (int index = 0; index < Status.Menu_Size; index++)									// Ensure All Dish Current Quantity Become NULL
			Menu[index].OQty = 0;

		Check_Date(Menu, Status, Time);															// Judge Statistics should Refresh or Not
		Refresh();
		cout << "*****************" << endl;
		cout << "* Main Menu:    *" << endl;
		cout << "* 1) Dish Menu  *" << endl;
		cout << "* 2) Order      *" << endl;
		cout << "* 3) Statistics *" << endl;
		cout << "* 4) Exit       *" << endl;
		cout << "*****************" << endl;
		cout << endl << "Enter your selection: ";

		do
		{
			getline(cin, Choice);																// Enter Selection

			if (Choice == "1")																	// If 1 then Turn to Dish Edit Screen
			{
				Exit = false;																	// Later Dish Edit Function Done will Exit Loop but Still in Big Loop to make Refresh Screen
				Dish_Edit(Menu, Status);														// Turn to Dish Edit Screen
			}
			else if (Choice == "2")																// If 2 then Turn to Customer Order Screen
			{
				Exit = false;																	// Later Function Done to make Refresh Screen
				Order(Menu, Status, Time);														// Turn to Customer Order Screen
			}
			else if (Choice == "3")																// If 3 then Turn to Boss Statistics Screen
			{
				if (Status.Total_Order > 0)														// Check have Order for Today
				{
					Exit = false;																// Later Function Done to make Refresh Screen
					Statistics(Menu, Status, Time);												// Turn to Boss Statistics Screen
				}
				else
				{
					cout << "Total order today is " << Status.Total_Order << endl;				// If No Order means that No Statistics Data Can View
					cout << "There is no statistic can view." << endl << endl;
					cout << "Enter your selection again: ";
				}
			}
			else if (Choice == "4")																// If 4 then Exit Current Function & Close Program
				Exit = false;																	// Open Door or Exit Current Loop To Close Program

			else
			{
				Check_Answer(Choice, Slide);													// If Not Answer Above then Check Answer to Give Feedback
				cout << "Selection (1/2/3/4): ";
			}
		} while (Exit);
	} while (Choice != "4");

	cout << endl << "The system is closing..." << endl;
}

void Dish_Edit(FOOD Menu[], STATISTICS& Status)
{
	string Choice, Slide = "Menu";

	do
	{
		bool Exit = true;																// False will Exit, True will Loop

		Refresh();
		cout << "-------------" << endl;
		cout << "| Dish Edit |" << endl;
		cout << "-------------" << endl;
		Dish(Menu, Status);
		cout << endl << "Add menu(a/A), drop dish(d/D), return(r/R): ";

		do
		{
			getline(cin, Choice);														// Enter Selection

			if (Choice == "a" || Choice == "A")											// If a or A then Turn to Add Menu Screen
			{
				Exit = false;															// Later Function Done to make Refresh Screen
				Add_Menu(Menu, Status);													// Turn to Add Menu Screen
			}
			else if (Choice == "d" || Choice == "D")									// If d or D then Turn to Drop Menu Screen
			{
				Exit = false;															// Later Function Done to make Refresh Screen
				Drop_Menu(Menu, Status);												// Turn to Drop Menu Screen
			}
			else if (Choice == "r" || Choice == "R")									// If r or R then Exit Current Function
				Exit = false;															// Open Door or Exit Current Loop To Exit Current Function

			else
			{
				Check_Answer(Choice, Slide);											// If Not Answer Above then Check Answer to Give Feedback
				cout << "Add menu(a/A), drop dish(d/D), return(r/R): ";
			}
		} while (Exit);
	} while (Choice != "r" && Choice != "R");
}

void Add_Menu(FOOD Menu[], STATISTICS& Status)
{
	string Name, Code, Slide = "Add_Menu";

	do
	{
		bool Exit = false;																			// False will Exit, True will Loop

		Refresh();
		cout << "-------------------" << endl;
		cout << "| Add Menu System |" << endl;
		cout << "-------------------" << endl;
		Dish(Menu, Status);
		cout << endl << "Enter food name first or return(r/R): ";

		do
		{
			getline(cin, Name);																		// Enter Food Name for New Dish or Exit Function

			if (Name == "r" || Name == "R")															// If r or R then Exit Current Function
				break;																				// Force Exit Current Loop & Exit Current Function

			cout << endl << "Enter food code, code must 4 words, no symbol and no space. Or enter (r/R) to reset: ";

			Code = Check_Code();																	// Check Number of Code Length Function

			if (Code == "r" || Code == "R")															// If r or R then Start Over Current Function
				break;																				// Force Exit Current Loop & Start Over Current Function

			double Price = Get_Price();																// Get Price for New Dish

			Status.Menu_Size++;																		// Increases Number of Dish from Menu
			Plus_Menu(Menu, Status, Code, Name, Price);												// Adding Dish Process Function
		} while (Exit);
	} while (Name != "r" && Name != "R");
}

string Check_Code()
{
	string Code;
	int len = 0, count2, check2, count4, check4;
	bool Exit = false;

	do
	{
		Exit = false;

		getline(cin, Code);																					// Enter Food Code for New Dish or Start Over

		int len = Code.length();																			// Get Number of Code Length
		count2 = 0;
		count4 = 0;

		for (int i = 0; i < Code.length(); i++)																// Check User Answer Enter
		{
			check2 = ispunct(Code[i]);																		// Chcek Punctuation
			if (check2)
				count2++;

			check4 = isspace(Code[i]);																		// Check Space
			if (check4)
				count4++;
		}

		if (Code == "r" || Code == "R")																		// If r or R then Exit Current Loop
			break;																							// Force Exit Current Loop

		else if (len != 4 || count2 > 0 || count4 > 0)														// If Number of Code Length not equal 4, Has Symbol & Has Space, Need Enter Again
		{
			Exit = true;
			cout << "Food code must 4 words! No Space and No Symbol! " << endl << endl;
			cout << "Enter food code or enter(r/R) to reset : ";
		}
	} while (Exit);																							// Determine Number of Code Length, Has Symbol, Has Space or Start Over

	return Code;																							// Return String Code to Other Function
}

double Get_Price()																							// It Same like Check Answer Function but It Only for Check Price,
{																											// due to Price is double however quantity & number of table is integer. So cannot combine
	string Answer;
	double p = -1;

	do
	{
		bool Is_True = false;
		int count = 0, check;
		int count2 = 0, check2;
		int count3 = 0, check3;
		int count4 = 0, check4;

		cout << endl << "Enter food price, just need to enter numbers: ";
		getline(cin, Answer);

		for (int i = 0; i < Answer.length(); i++)
		{
			check = isalpha(Answer[i]);
			if (check)
				count++;

			check2 = ispunct(Answer[i]);
			if (check2)
				count2++;

			check3 = isdigit(Answer[i]);
			if (check3)
				count3++;

			check4 = isspace(Answer[i]);
			if (check4)
				count4++;
		}

		if (count == 1 && count2 == 0 && count3 == 0 && count4 == 0)
		{
			Is_True = true;
			cout << "No alphabet! ";
		}
		else if (count2 == 1 && count == 0 && count3 == 0 && count4 == 0)
		{
			Is_True = true;
			cout << "No symbol! ";
		}
		else if ((count4 == 0 || count4 > 0) && count == 0 && count2 == 0 && count3 == 0)
		{
			Is_True = true;
			cout << "No space! ";
		}
		else if (Answer[0] == '-' && count == 0 && count2 == 1 && count3 > 0)
		{
			Is_True = true;
			cout << "No negative number! ";
		}
		else if (Answer[0] == '0' && count == 0 && count2 == 0 && count3 > 0 && count4 == 0)
		{
			Is_True = true;
			cout << "Invalid number! ";
		}
		else if (count == 0 && (count2 == 0 || count2 == 1) && count3 > 0 && count4 == 0)
		{
			Is_True = false;
			p = stod(Answer);
			cout << p;
		}
		else
		{
			Is_True = true;
			cout << "No sentences! ";
		}

		if (Is_True)
			cout << "Please enter correct number...\n";
	} while (p <= 0);

	return p;																								// Return Double Price to Other Function
}

void Plus_Menu(FOOD Menu[], STATISTICS& Status, string Code, string Name, double Price)
{
	ofstream out_menu("Food2.txt", ofstream::app);															// Prepare Append to the File
	ofstream out_status("Status.txt");																		// Prepare Overwrite File

	if (!out_menu)																							// If Cannot Open File Return Warning
		cout << "Error opening output file: Food2.txt\n";
	else if (!out_status)																					// If Cannot Open File Return Warning
		cout << "Error opening output file: Status.txt\n";
	else
	{
		out_menu << Code << "|" << Name << "|" << Price << "|0|0|0|0|0|0" << endl;							// Add New Dish

		out_status << Status.Menu_Size << " " << Status.Dine_In_Order << " "								// Change Menu Size due to Increases Number of Dish from Menu
				<< Status.Take_Away_Order << " " << Status.Total_Order << " " << Status.Date << endl;
	}

	out_menu.close();																						// Close File
	out_status.close();																						// Close File

	Read_File(Menu, Status);																				// Open & Read File to Refresh Menu & Statistics
}

void Drop_Menu(FOOD Menu[], STATISTICS& Status)
{
	string Code, K2, Slide = "Drop_Menu";

	do
	{
		bool Exit = true;																								// False will Exit, True will Loop

		Refresh();
		cout << "--------------------" << endl;
		cout << "| Drop Menu System |" << endl;
		cout << "--------------------" << endl;
		Dish(Menu, Status);
		cout << endl << "Enter food code(Sample E.g. F001) to delete the dish from menu or return(r/R): ";

		do
		{
			getline(cin, Code);																							// Enter Food Code Want to Delete

			for (int index = 0; index < Status.Menu_Size; index++)														// Scan All Food Code
			{
				if (Code == Menu[index].Code)
				{
					Exit = false;																						// Later Function Done to make Refresh Screen

					cout << "Are you sure? (y/Y) Yes OR (n/N) No: ";													// Need to Make Sure for Avoid Careless
					K2 = Double_Confirm(Slide);

					if (K2 == "y" || K2 == "Y")
					{
						Delete_Menu(Menu, Status, Code, index);															// Deleting Menu Process Function
					}
				}
				else if (index == Status.Menu_Size - 1 && (Code == "r" || Code == "R"))									// After Scan All Code and Code is r or R
					Exit = false;																						// Allow Exit Loop & Function
				
				else if (index == Status.Menu_Size - 1)																	// If Scan All Code but Don't Achieve Requirement Above then Excecute 1 Time
				{
					Check_Answer(Code, Slide);																			// If Not Answer Above then Check Answer to Give Feedback
					cout << "Enter food code(Sample E.g.F001) to delete the dish from menu or return(r / R) : ";
				}
			}
		} while (Exit);
	} while (Code != "r" && Code != "R");
}

string Double_Confirm(string Page)
{
	string K2, Slide = "Sure";

	do
	{
		getline(cin, K2);																// Enter Answer to Judge Confirm or Not

		if (K2 == "n" || K2 == "N")														// If n or N then Continue Stay in Current Function
		{
			if (Page == "Compute")
				cout << "Still at Compute System..." << endl << endl;
			else if (Page == "Status")
				cout << "Still at Statistics System..." << endl << endl;
		}
		else if (K2 != "y" && K2 != "Y" && K2 != "n" && K2 != "N")						// If Not Answer Above then Check Answer to Give Feedback
		{
			Check_Answer(K2, Slide);

			cout << "Are you sure? (y/Y) Yes OR (n/N) No: ";
		}
	} while (K2 != "y" && K2 != "Y" && K2 != "n" && K2 != "N");

	return K2;																			// Return String K2 to Other Function
}

void Delete_Menu(FOOD Menu[], STATISTICS& Status, string Code, int index)
{
	for (int index2 = index + 1; index2 < Status.Menu_Size; index2++)															// Copy & Paste The Dish from Menu want to Drop 
	{
		Menu[index] = Menu[index2];
		index++;
	}

	Status.Menu_Size--;																											// Decreases Number of Dish from Menu

	ofstream out_menu("Food2.txt");																								// Prepare Overwrite File
	ofstream out_status("Status.txt");																							// Prepare Overwrite File

	if (!out_menu)																												// If Cannot Open File Return Warning
		cout << "Error opening output file: Food2.txt\n";
	else if (!out_status)																										// If Cannot Open File Return Warning
		cout << "Error opening output file: Status.txt\n";
	else
	{
		for (int index = 0; index < Status.Menu_Size; index++)
		{
			out_menu << Menu[index].Code << "|" << Menu[index].Name << "|" << Menu[index].Price << "|"							// Overwrite File to Drop Dish
				<< Menu[index].OQty << "|" << Menu[index].TPrice << "|" << Menu[index].TPre_Time << "|"
				<< Menu[index].Over_Sale << "|" << Menu[index].Over_Qty << "|" << Menu[index].Over_Pre_Time << endl;
		}

		out_status << Status.Menu_Size << " " << Status.Dine_In_Order << " "													// Change Menu Size due to Increases Number of Dish from Menu
				<< Status.Take_Away_Order << " " << Status.Total_Order << " " << Status.Date << endl;
	}

	out_menu.close();																											// Close File
	out_status.close();																											// Close File

	Read_File(Menu, Status);																									// Open & Read File to Refresh Menu & Statistics
}


void Order(FOOD Menu[], STATISTICS& Status, TIME& Time)
{
	string Choice, Slide = "Order";

	do
	{
		bool Judge = false, Exit = true;																		// Judging Has Quantity to Cancel or Compute
																												// False will Exit, True will Loop
		for (int index = 0; index < Status.Menu_Size; index++)													// Check Every Dish has Quantity or Not
		{
			if (Menu[index].OQty > 0)																			// If Tes then Allow Turn to Cancel & Compute Function
				Judge = true;
		}

		Refresh();
		cout << "----------------" << endl;
		cout << "| Order System |" << endl;
		cout << "----------------" << endl;
		Dish(Menu, Status);
		cout << endl << "Enter take order(o/O), cancel foods(c/C), compute(t/T) or exit(e/E): ";

		do
		{
			getline(cin, Choice);																				// Enter Selection

			if (Choice == "o" || Choice == "O")																	// If o or O then Turn to Take Order Screen
			{
				Exit = false;																					// Later Function Done to make Refresh Screen
				Take_Order(Menu, Status);																		// Turn to Take Order Screen
			}
			else if (Choice == "c" || Choice == "C")															// If c or C then Turn to Cancel Order Screen
			{
				if (Judge)
				{
					Exit = false;																				// Later Function Done to make Refresh Screen
					Cancel(Menu, Status);																		// Turn to Cancel Order Screen
				}
				else
					cout << "There is no food to cancel." << endl << endl										// If No All Order Don't have Quantity then Cannot Turn to Cancel Screen
						<< "Enter take order(o/O), cancel foods(c/C), compute(t/T) or exit(e/E): ";
			}
			else if (Choice == "t" || Choice == "T")															// If t or T then Turn to Compute Order Screen
			{
				if (Judge)
				{
					Exit = false;																				// Later Function Done to make Refresh Screen
					Compute(Menu, Status, Time);																// Turn to Compute Order Screen
				}
				else
					cout << "There is no food to compute." << endl << endl										// If No All Order Don't have Quantity then Cannot Turn to Compute Screen
						<< "Enter take order(o/O), cancel foods(c/C), compute(t/T) or exit(e/E): ";
			}
			else if (Choice == "e" || Choice == "E")															// If r or R then Exit Current Function
				Exit = false;																					// Open Door or Exit Current Loop To Exit Current Function

			else
			{
				Check_Answer(Choice, Slide);																	// If Not Answer Above then Check Answer to Give Feedback
				cout << "Enter take order(o/O), cancel foods(c/C), compute(t/T) or exit(e/E): ";
			}
		} while (Exit);
	} while (Choice != "e" && Choice != "E");
}

void Take_Order(FOOD Menu[], STATISTICS& Status)
{
	string Code;

	do
	{
		bool Exit = true;																					// False will Exit, True will Loop

		Refresh();
		cout << "--------------" << endl;
		cout << "| Take Order |" << endl;
		cout << "--------------" << endl;
		Dish(Menu, Status);
		cout << endl << "Enter food code(Sample E.g. F001) to take order, (r/R) to return: ";

		do
		{
			string Slide = "Take";																			// Always Keep Every Time is Slide Take
			getline(cin, Code);																				// Enter Selection

			for (int index = 0; index < Status.Menu_Size; index++)											// Check String Code is equal to Current Dish from Menu
			{
				if (Code == Menu[index].Code)																// If Same or Has
				{
					Exit = false;																			// Later Function Done to make Refresh Screen
					Slide = "Number";																		// Change Slide to "Number"

					int Qty = Check_Answer(Code, Slide);													// Check Qty is Valid Number or Not
					Menu[index].OQty += Qty;																// Plus Quantity of Food Order

					break;																					// Force Exit Current Loop & Start Over Current Function
				}
				else if (Code == "r" || Code == "R")														// If r or R then Exit Current Function
					Exit = false;																			// Open Door or Exit Current Loop To Exit Current Function

				else if (index == Status.Menu_Size - 1)														// If Scan All Code but Don't Achieve Requirement Above then Excecute 1 Time
				{
					Check_Answer(Code, Slide);																// If Not Answer Above then Check Answer to Give Feedback
					cout << "Enter food code(Sample E.g. F001) to take order, (r/R) to return: ";
				}
			}
		} while (Exit);
	} while (Code != "r" && Code != "R");
}

void Cancel(FOOD Menu[], STATISTICS& Status)
{
	string Choice;

	do
	{
		bool Exit = true;																								// False will Exit, True will Loop
		int index = 0, Qty = 0;

		Refresh();
		cout << "-----------------" << endl;
		cout << "| Cancel System |" << endl;
		cout << "-----------------" << endl;
		Dish(Menu, Status);
		cout << endl << "Enter food code(Sample E.g. F001) to cancel which food you don't want," << endl;
		cout << "or enter (a/A) to cancel all food," << endl;
		cout << "or enter return(r/R)." << endl;
		cout << endl << "Choice: ";

		do
		{
			string Slide = "Cancel";																					// Always Keep Every Time is Slide Cancel
			getline(cin, Choice);																						// Enter Selection

			if (Choice == "a" || Choice == "A")																			// If a or A then Turn to Take Order Screen
			{
				Qty = Confirm_Drop(Menu, index, Qty, Slide);															// Need to Make Sure for Avoid Careless

				if (Qty > 0)																							// If Yes
					Choice = "Cancel_All";																				// Choice Change to Cancel All
				else
					Exit = false;																						// Later Function Done to make Refresh Screen
			}

			for (index = 0; index < Status.Menu_Size; index++)															// Check String Code is equal to Current Dish from Menu
			{
				if (Choice == "Cancel_All")																				// If Cancel_All then Excecute Delete All Quantity
				{
					Exit = false;																						// Later Function Done to make Refresh Screen
					Menu[index].OQty = 0;																				// Let All Quantity of Dish Becom ZERO
				}
				else if (Choice == Menu[index].Code)																	// If Same or Has
				{
					if (Menu[index].OQty == 0)																			// If All Dish Already No More Quantity
					{
						cout << "There is no " << Menu[index].Name << " to cancel." << endl << endl;					// Display Warning& Reminder
						cout << "Choice: ";

						break;																							// Force Exit Current Loop & Start Over Current Function
					}
					else
					{
						Exit = false;																					// Later Function Done to make Refresh Screen
						Slide = "Number";																				// Change Slide to "Number"

						Qty = Check_Answer(Choice, Slide);																// Check Qty is Valid Number or Not
						Qty = Drop_Dish(Menu, Choice, index, Qty);														// Valid Number Bring to Drop Dish Function

						break;																							// Force Exit Current Loop & Start Over Current Function
					}
				}
				else if (Choice == "r" || Choice == "R")																// If r or R then Exit Current Function
					Exit = false;																						// Open Door or Exit Current Loop To Exit Current Function

				else if (index == Status.Menu_Size - 1)																	// If Scan All Code but Don't Achieve Requirement Above then Excecute 1 Time
				{
					Check_Answer(Choice, Slide);																		// If Not Answer Above then Check Answer to Give Feedback
					cout << "Enter food code(Sample E.g. F001) to cancel which food you don't want," << endl;
					cout << "or enter (a/A) to cancel all food," << endl;
					cout << "or enter return(r/R)." << endl;
					cout << endl << "Choice: ";
				}
			}
		} while (Exit);
	} while (Choice != "r" && Choice != "R" && Choice != "Cancel_All");
}

int Drop_Dish(FOOD Menu[], string Code, int index, int Qty)
{
	string Slide = "Drop_Section";

	if (Menu[index].OQty >= Qty)								// If Current Dish Quantity More Than or Equal to User Enter Quantity to Cancel
	{
		Menu[index].OQty -= Qty;								// Minus Directly
	}
	else if (Menu[index].OQty < Qty)							// If User Enter Quantity to Cancel More Than Current Dish Quantity
	{
		Qty = Confirm_Drop(Menu, index, Qty, Slide);			// Need to Make Sure for Avoid Careless
		Menu[index].OQty -= Qty;
	}

	return Qty;													// Retuan Integer Qty to Other Function
}

int Confirm_Drop(FOOD Menu[], int index, int Qty, string Slide)
{
	string Choice;

	cout << "Did you sure you want delete all? (y/Y) Yes OR (n/N) No: ";

	do
	{
		getline(cin, Choice);																	// Enter Choice

		if (Choice != "y" && Choice != "Y" && Choice != "n" && Choice != "N")					// If Not Correct Choice
		{
			if (Slide == "Drop_Section")														// If Come From Slide Drop_Selection then Change Slide to Yes_Drop
				Slide = "Yes_Drop";

			Check_Answer(Choice, Slide);														// If Not Correct Answer then Check Answer to Give Feedback
			cout << "Did you sure you want delete all? (y/Y) Yes OR (n/N) No: ";
		}
	} while (Choice != "y" && Choice != "Y" && Choice != "n" && Choice != "N");

	if (Choice == "y" || Choice == "Y")															// If y or Y
	{
		if (Slide == "Drop_Section" || Slide == "Yes_Drop")
			return Menu[index].OQty;															// Return Same Value to Make Sure Later Minus Section is equal to ZERO

		else if (Slide == "Cancel")
			return Qty = 1;																		// Return Integer Value to Judge Cancel All or Not
	}

	return Qty = 0;																				// If Choice equal to n or N then Return Integer Number 0, Means Later Decreases Nothing
}

void Confirm_Return(string Page)
{
	string K, K2, Slide = "Return";

	do
	{
		bool Exit = false;													// False will Exit, True will Loop

		cout << "Press (k/K) to return: ";

		do
		{
			getline(cin, K);												// Enter Selection

			if (K == "k" || K == "K")										// If k or K
			{
				Exit = false;												// Open Door or Exit Current Loop To Exit Current Function

				cout << "Are you sure? (y/Y) Yes OR (n/N) No: ";
				K2 = Double_Confirm(Page);									// Need to Make Sure for Avoid Careless
			}
			else
			{
				Exit = true;												// False will Exit, True will Loop

				Check_Answer(K, Slide);										// If Not Correct Answer then Check Answer to Give Feedback
				cout << "Please enter (k/K) to return: ";
			}
		} while (Exit);
	} while (K2 != "y" && K2 != "Y");
}

void Compute(FOOD Menu[], STATISTICS& Status, TIME& Time)
{
	string Choice, Type, Table, Page = "Compute";
	double Total_Price = 0;
	int NoTable = 0;

	Check_Date(Menu, Status, Time);																							// Judge Statistics should Refresh or Not
	Refresh();
	cout << "------------------" << endl;
	cout << "| Compute System |" << endl;
	cout << "------------------" << endl << endl;
	cout << "Dine in(e/E) or Take away(t/T) or Cancel(c/C): ";

	do
	{
		getline(cin, Type);																									// Enter Order Type

		if (Type == "e" || Type == "E")																						// If e or E then Dine In Order Increases
			Status.Dine_In_Order++;

		else if (Type == "t" || Type == "T")																				// If t or T then Dine In Order Increases
			Status.Take_Away_Order++;

		else if (Type == "c" || Type == "C")																				// If c or C then Exit Current Function
			return;
		else
		{
			Check_Answer(Type, Page);																						// If Not Correct Answer then Check Answer to Give Feedback
			cout << "Dine in(e/E) or Take away(t/T) or Cancel(c/C): ";
		}
	} while (Type != "e" && Type != "E" && Type != "t" &&
			Type != "T" && Type != "c" && Type != "C");

	for (int index = 0; index < Status.Menu_Size; index++)																	// Compute Price
	{
		Menu[index].TPrice = Menu[index].OQty * Menu[index].Price;
		Total_Price += Menu[index].TPrice;
		Menu[index].Over_Sale += Menu[index].TPrice;
		Menu[index].Over_Qty += Menu[index].OQty;
	}

	Status.Total_Order++;																									// Total Order Increases

	if (Type == "e" || Type == "E")																							// If Just Now Choose Dine In(e or E) then Require Enter No Teble
	{
		Page = "Table";
		NoTable = Check_Answer(Table, Page);																				// If Not Correct Answer then Check Answer to Give Feedback
	}

	Page = "Compute";

	cout << endl << "*****************************************************************************" << endl;				// Display Total Quantity & Total Price that Customer Order

	if (Type == "e" || Type == "E")
	{
		cout << "* Order Type: Dine in                                                       *" << endl;
		cout << "* No. Table: " << setw(3) << NoTable << setw(61) << " *" << endl;
	}
	else
		cout << "* Order Type: Take away                                                     *" << endl;

	cout << "* Food \t\t\t  Each Price(RM) \t  Quantity \t  Price(RM) *" << endl;

	for (int index = 0; index < Status.Menu_Size; index++)																	// Only Display Dish that Order by Customer
		cout << "* " << left << setw(20) << Menu[index].Name << right << setw(18) << Menu[index].Price << setw(18)
			<< Menu[index].OQty << setw(17) << fixed << setprecision(2) << Menu[index].TPrice << " *" << endl;

	cout << "*                                                                 _________ *" << endl;
	cout << "* Total Price" << setw(62) << fixed << setprecision(2) << Total_Price << " *" << endl;
	cout << "*                                                                 --------- *" << endl;
	cout << "*****************************************************************************" << endl;

	for (int index = 0; index < Status.Menu_Size; index++)																	// Clear Data of Dish
	{
		Menu[index].TPrice = 0;
		Menu[index].OQty = 0;
	}

	Get_Current_Time(Status, Time, Page);																					// Display Current Time
	cout << "Total Order: " << Status.Total_Order << endl << endl;
	cout << "Attention!!! Please take a screenshot for these page for a history record. ";
	cout << "Hope you have a pleasant eating experience.Thank you." << endl << endl;

	ofstream out_menu("Food2.txt");																							// Prepare Overwrite File
	ofstream out_status("Status.txt");																						// Prepare Overwrite File

	if (!out_menu)																											// If Cannot Open File Return Warning
		cout << "Error opening output file: Food2.txt\n";
	else if (!out_status)																									// If Cannot Open File Return Warning
		cout << "Error opening output file: Status.txt\n";
	else
	{
		for (int index = 0; index < Status.Menu_Size; index++)																// Overwrite File
		{
			out_menu << Menu[index].Code << "|" << Menu[index].Name << "|" << Menu[index].Price << "|"
				<< Menu[index].OQty << "|" << Menu[index].TPrice << "|" << Menu[index].TPre_Time << "|"
				<< Menu[index].Over_Sale << "|" << Menu[index].Over_Qty << "|" << Menu[index].Over_Pre_Time << endl;
		}

		out_status << Status.Menu_Size << " " << Status.Dine_In_Order << " "												// Store Current Date to Status File
				<< Status.Take_Away_Order << " " << Status.Total_Order << " " << Time.Date[0] << endl;
	}

	out_menu.close();																										// Close File
	out_status.close();																										// Close File

	Confirm_Return(Page);																									// Need to Make Sure for Avoid Careless
}

void Statistics(FOOD Menu[], STATISTICS& Status, TIME& Time)
{
	string Page = "Status";

	double Total_Price = 0;

	Check_Date(Menu, Status, Time);																							// Judge Statistics should Refresh or Not
	Refresh();
	cout << "---------------------" << endl;
	cout << "| Statistics System |" << endl;
	cout << "---------------------" << endl;
	cout << "****************************************************************************" << endl;
	cout << "* Daily Dine In Order: " << setw(51) << Status.Dine_In_Order << " *" << endl;
	cout << "* Daily Take Away Order: " << setw(49) << Status.Take_Away_Order << " *" << endl;
	cout << "*                                                                _________ *" << endl;
	cout << "* Daily Total Order: " << setw(53) << Status.Total_Order << " *" << endl;
	cout << "*                                                                --------- *" << endl;
	cout << "*                                                                          *" << endl;
	cout << "* Daily sales overall \t\t\t\t\t\t Price(RM) *" << endl;

	for (int index = 0; index < Status.Menu_Size; index++)																	// Only Display Dish that Has Sale for Today
	{
		cout << "* " << Menu[index].Code << ", " << left << setw(20) << Menu[index].Name << ": "							// Display Overall Sale Price Today
			<< right << setw(44) << fixed << setprecision(2) << Menu[index].Over_Sale << " *" << endl;

		Total_Price += Menu[index].Over_Sale;																				// Find Sum of All Dish Total Price
	}

	cout << "*                                                                _________ *" << endl;
	cout << "* Total Price" << setw(61) << fixed << setprecision(2) << Total_Price << " *" << endl;
	cout << "*                                                                --------- *" << endl;
	cout << "*                                                                          *" << endl;
	cout << "* Daily order overall\t\t\t\t\t\t\t\bQty *" << endl;

	for (int index = 0; index < Status.Menu_Size; index++)																	// Only Display Dish that Has Sale for Today
		cout << "* " << Menu[index].Code << ", " << left << setw(20)														// Display Overall Sale Quantity Today
			<< Menu[index].Name << ": " << right << setw(44) << Menu[index].Over_Qty << " *" << endl;

	cout << "****************************************************************************" << endl;
	Get_Current_Time(Status, Time, Page);																					// Display Current Time

	Confirm_Return(Page);																									// Need to Make Sure for Avoid Careless
}
