#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
using namespace std;
const string ClientFileName = "Client.text";
const string UsersFileName = "Users.txt";

struct sClient {
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

struct sUser {
	string UserName;
	string Passwred;
	int Permissions;
	bool MarkForDelete = false;

};

sUser CurrentUser;

//enum enAccessUsers{eAdmen=-1,eShowList=1,eAdd=2,eDelete=4,eFind=8,eUpdate=16};

enum enWhatChoose { eListClient = 1, eAddNewClient = 2, eDleletClient = 3, eUpdateClient = 4, eFindClient = 5, eShowTransaction = 6,eManage=7, LogOut = 8 };

enum enTransactionMenueOption { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMianMenue = 4 };

enum enMangeUser{eShowListUser=1,eAddNewUser=2,eDeleteUser=3,eUpdateUser=4,eFindUser=5,eMainMenue=6};

enum enMainMenuePermission{
	eAll=-1,pListClients=1,pAddNewClients=2,
	DeleteClients=4,pUpdateClients=8,
	FindClients=16,vTransaction=32,
	pManageUser=64};


void ShowTransactionClientScreen();
void ShowMainMenue();
void Login();
void ShowAccessDeniedMessage() {
	system("cls");
	cout << "\n------------------------------------------------------\n";
	cout << "Access Denid ,\n";
	cout << "you do Not have permission to Do this ,\n";
	cout << "plase conact your Admin.";
	cout << "\n------------------------------------------------------\n";
}
bool CheckAccessPermissions(enMainMenuePermission Permission);
void ShowManageUsersScreen();

string ReadClientAccountNumber() {
	string AccountNumber = " ";
	cout << "plase enter Account Number : ";
	getline(cin >> ws, AccountNumber);
	return AccountNumber;
}


void GoBackToMainMenue() {
	cout << "\n\nPress any key to go back to Main Menue .... ";
	system("pause>0");
	ShowMainMenue();
}

vector<string>  SplitString(string S1, string deline = "") {
	vector<string> vString;
	int pos = 0;
	string sWord;
	while ((pos = S1.find(deline)) != std::string::npos) {
		sWord = S1.substr(0, pos);
		if (sWord != "") {
			vString.push_back(sWord);
		}
		S1.erase(0, pos + deline.length());
	}
	if (S1 != "")
		vString.push_back(S1);
	return vString;
}

sClient ConvertLineToClientRecord(string line, string seperator = "#//#") {
	vector<string> vClient;
	sClient client;
	vClient = SplitString(line, seperator);
	client.AccountNumber = vClient[0];
	client.PinCode = vClient[1];
	client.Name = vClient[2];
	client.Phone = vClient[3];
	client.AccountBalance = stod(vClient[4]);
	return client;
}



vector<sClient> LoadClientDataFromFile(string FileName) {
	fstream fClient;
	vector<sClient>vClient;
	fClient.open(FileName, ios::in);
	if (fClient.is_open()) {
		string line;
		sClient Client;
		while (getline(fClient, line)) {
			Client = ConvertLineToClientRecord(line);
			vClient.push_back(Client);

		}
		fClient.close();

	}
	return vClient;
}

void PrintClientRecord(sClient Client) {
	cout << "|" << left << setw(17) << Client.AccountNumber;
	cout << "|" << left << setw(15) << Client.PinCode;
	cout << "|" << left << setw(25) << Client.Name;
	cout << "|" << left << setw(13) << Client.Phone;
	cout << "|" << left << setw(15) << Client.AccountBalance << " |\n";
	cout << "---------------------------------------------------------------------------------------------\n";


}

void ShowAllClientScreen() {
	if (!CheckAccessPermissions(enMainMenuePermission::pListClients)) {
		ShowAccessDeniedMessage();
	
	}
	vector<sClient>vClient;
	vClient = LoadClientDataFromFile(ClientFileName);
	cout << "\n\t\t\tClient List ( " << vClient.size() << ") Client (s).\n\n";

	cout << "---------------------------------------------------------------------------------------------\n";
	cout << "|" << left << setw(17) << " Account Number";
	cout << "|" << left << setw(15) << " Pin Code";
	cout << "|" << left << setw(25) << " Client name";
	cout << "|" << left << setw(13) << " phone";
	cout << "|" << left << setw(15) << " Balance  " << " |\n";
	cout << "---------------------------------------------------------------------------------------------\n";
	if (vClient.size() == 0)
		cout << "\n\t\t NO Client avalibie Inthe system \n";
	else
		for (sClient& client : vClient) {
			PrintClientRecord(client);
		}

}

string ConvertRecordToLine(sClient client, string Seperator = "#//#") {

	string info = "";
	info += client.AccountNumber + Seperator;
	info += client.PinCode + Seperator;
	info += client.Name + Seperator;
	info += client.Phone + Seperator;
	info += to_string(client.AccountBalance);
	return info;
}

void AddDataLineToFile(string FileName, string stDataLine) {
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open()) {
		MyFile << stDataLine << endl;
		MyFile.close();
	}

}





bool ClientExistsByAccountNumber(string AccountNumber, string FileName) {
	vector <sClient> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);//read Mode
	if (MyFile.is_open())
	{
		string Line;
		sClient Client;
		while (getline(MyFile, Line))
		{
			Client = ConvertLineToClientRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close(); return true;
			}

			vClients.push_back(Client);
		}

		MyFile.close();
	}

	return false;
}


sClient ReadNewClient() {
	sClient client;
	cout << "  enter Account Number : ";
	getline(cin >> ws, client.AccountNumber);
	while (ClientExistsByAccountNumber(client.AccountNumber, ClientFileName))
	{
		cout << "the Account Number (" << client.AccountNumber << ") is alread exist, plase enter anorhet Account Number : ";
		getline(cin, client.AccountNumber);
	}
	cout << "  enter PinCode : ";
	getline(cin >> ws, client.PinCode);

	cout << "  enter  name : ";
	getline(cin >> ws, client.Name);

	cout << " enter phone : ";
	getline(cin >> ws, client.Phone);

	cout << "  enter Account Balance : ";
	cin >> client.AccountBalance;
	return client;

}


void AddNewClient() {
	sClient client;
	client = ReadNewClient();
	AddDataLineToFile(ClientFileName, ConvertRecordToLine(client));
}
void AddNewClients() {

	char AddMore = 'Y';
	do {
		cout << "Adding New Client ...\n\n";
		AddNewClient();
		cout << "\nCliet Add successfuly, Do you need to add another client ? y/n ?\n";
		cin >> AddMore;

	} while (toupper(AddMore) == 'Y');


}

void ShowAddNewClientsScreen() {
	if (!CheckAccessPermissions(enMainMenuePermission::pAddNewClients)) {
		ShowAccessDeniedMessage();
	
	}
	cout << "\n-------------------------------------------------\n";
	cout << "\t\t Add New Client Screen \n";
	cout << "\n--------------------------------------------------\n";
	AddNewClients();
}

void PrintClientCard(sClient client) {
	cout << "\nThe following are the client details:\n";
	cout << "-------------------------------------------\n";
	cout << "\n Acount number :" << client.AccountNumber;
	cout << "\n PinCode :" << client.PinCode;
	cout << "\n name : " << client.Name;
	cout << "\n phone : " << client.Phone;
	cout << "\n Balance : " << client.AccountBalance;
	cout << "\n-------------------------------------------\n";

}



vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;
	if (MyFile.is_open())
	{
		for (sClient C : vClients)
		{
			if (C.MarkForDelete == false)
			{ //we only write records that are not marked for delete.   
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}

void MarkClientForDeleteByAccountNumber(string AccountNumber, vector<sClient>& vClient) {
	for (sClient& C : vClient) {
		if (C.AccountNumber == AccountNumber) {
			C.MarkForDelete = true;
			break;
		}
	}


}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client)
{
	for (sClient C : vClients) {

		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	} return false;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {

	sClient Client;
	char Answer = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y') {
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveCleintsDataToFile(ClientFileName, vClients); //Refresh Clients
			vClients = LoadClientDataFromFile(ClientFileName);
			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
	return false;


}


void ShowDeleteClientScreen() {
	if (!CheckAccessPermissions(enMainMenuePermission::DeleteClients)) {
		ShowAccessDeniedMessage();
		
	}
	cout << "\n-------------------------------------------------\n";
	cout << "\t\t Delete Client Screen \n";
	cout << "\n--------------------------------------------------\n";
	string AccountNumber = ReadClientAccountNumber();
	vector<sClient>vClient = LoadClientDataFromFile(ClientFileName);
	DeleteClientByAccountNumber(AccountNumber, vClient);

}

//Update Client Info
sClient ChangeClientRecord(string AccountNumber) {
	sClient client;
	client.AccountNumber = AccountNumber;

	cout << "  enter PinCode : ";
	getline(cin >> ws, client.PinCode);

	cout << "  enter  name : ";
	getline(cin >> ws, client.Name);

	cout << " enter phone : ";
	getline(cin >> ws, client.Phone);

	cout << "  enter Account Balance : ";
	cin >> client.AccountBalance;
	return client;

}

void UpdateClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {

	sClient Client;
	char Anser = 'n';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want Update this client? y/n ? ";
		cin >> Anser;
		if (Anser == 'y' || Anser == 'Y') {
			for (sClient& C : vClients) {
				if (C.AccountNumber == AccountNumber) {
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveCleintsDataToFile(ClientFileName, vClients); //Refresh Clients
			cout << "\n\nClient Update Successfully.";

		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";

	}

}


void ShowUpdateClientScreen() {
	if (!CheckAccessPermissions(enMainMenuePermission::pUpdateClients)) {
		ShowAccessDeniedMessage();
	
	}
	cout << "\n-------------------------------------------------\n";
	cout << "\t\t Delete Client Screen \n";
	cout << "\n--------------------------------------------------\n";
	string AccountNumber = ReadClientAccountNumber();
	vector<sClient>vClient = LoadClientDataFromFile(ClientFileName);
	UpdateClientByAccountNumber(AccountNumber, vClient);


}

void ShowFindClientScreen() {
	if (!CheckAccessPermissions(enMainMenuePermission::FindClients)) {
		ShowAccessDeniedMessage();
	
	}
	cout << "\n-------------------------------------------------\n";
	cout << "\t\t Find Client Screen \n";
	cout << "\n--------------------------------------------------\n";
	string AccountNumber = ReadClientAccountNumber();
	vector<sClient>vClient = LoadClientDataFromFile(ClientFileName);
	sClient client;
	if (FindClientByAccountNumber(AccountNumber, vClient, client)) {
		PrintClientCard(client);
	}
	else
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";



}

void ShowTotalBalance() {
	vector<sClient>vClient;
	vClient = LoadClientDataFromFile(ClientFileName);
	cout << "\n\t\t\tClient List ( " << vClient.size() << ") Client (s).\n\n";

	cout << "---------------------------------------------------------------------------------------------\n";
	cout << "|" << left << setw(17) << " Account Number";
	cout << "|" << left << setw(25) << " Client name";
	cout << "|" << left << setw(15) << " Balance  " << "|\n";
	cout << "---------------------------------------------------------------------------------------------\n";
	if (vClient.size() == 0)
		cout << "\n\t\t NO Client avalibie Inthe system \n";
	else
	{
		double TotalBalance = 0;
		for (sClient& client : vClient) {
			TotalBalance += client.AccountBalance;
			cout << "|" << left << setw(17) << client.AccountNumber;
			cout << "|" << left << setw(25) << client.Name;
			cout << "|" << left << setw(15) << client.AccountBalance << " \n";
			cout << "---------------------------------------------------------------------------------------------\n";
		}
		cout << "\n\n Tatal Balance = " << TotalBalance << endl;
	}
}

void ShowTotalBalanceScreen() {
	ShowTotalBalance();
}

bool WithdrawBlalanceToClinetByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClient) {
	char Anser = 'y';
	cout << "\n\n Are you sure you want perform this transaction ?y\n ";
	cin >> Anser;
	if (toupper(Anser) == 'Y') {

		for (sClient& C : vClient) {
			if (C.AccountNumber == AccountNumber) {
				C.AccountBalance += Amount;
				SaveCleintsDataToFile(ClientFileName, vClient);
				cout << "Done successfully .New Balance = " << C.AccountBalance << endl;
				return true;
			}
		}
		return false;
	}
	return false;

}

void ShowWithdrawScreen() {
	cout << "\n---------------------------------\n";
	cout << "          Deposit Screen ";
	cout << "\n---------------------------------\n";
	string AccountNumber;
	AccountNumber = ReadClientAccountNumber();
	vector<sClient>vClient;
	sClient Client;
	vClient = LoadClientDataFromFile(ClientFileName);
	while (!FindClientByAccountNumber(AccountNumber, vClient, Client)) {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	double Amount = 0;
	cout << "\n\nplase enter deposit amount : ";
	cin >> Amount;
	while (Amount > Client.AccountBalance) {
		cout << "\n Amount Exceed the balance , you can withdraw up :" << Client.AccountBalance << endl;
		cout << "plase enter another amount : ";
		cin >> Amount;
	}
	WithdrawBlalanceToClinetByAccountNumber(AccountNumber, Amount * -1, vClient);

}

bool DepositBlalanceToClinetByAccountNumber(string AccountNumber, double Amount, vector<sClient>& vClient) {
	char Anser = 'y';
	cout << "\n\n Are you sure you want perform this transaction ?y\n\n ";
	cin >> Anser;
	if (toupper(Anser) == 'Y') {
		for (sClient& C : vClient) {
			if (C.AccountNumber == AccountNumber) {
				C.AccountBalance += Amount;
				SaveCleintsDataToFile(ClientFileName, vClient);
				cout << "Done successfully .New Balance = " << C.AccountBalance << endl;
				return true;
			}
		}
		return false;
	}
	return false;

}
void ShowDepositScreen() {
	cout << "\n---------------------------------\n";
	cout << "          Deposit Screen ";
	cout << "\n---------------------------------\n";
	string AccountNumber;
	AccountNumber = ReadClientAccountNumber();
	vector<sClient>vClient;
	sClient Client;
	vClient = LoadClientDataFromFile(ClientFileName);
	while (!FindClientByAccountNumber(AccountNumber, vClient, Client)) {
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	double Amount = 0;
	cout << "\n\nplase enter deposit amount :";
	cin >> Amount;
	DepositBlalanceToClinetByAccountNumber(AccountNumber, Amount, vClient);
}

void GoBackToTransactionMenue() {
	cout << "\n\nPress any key to go back to Main Menue .... ";
	system("pause>0");
	ShowTransactionClientScreen();
}

short ReadTransactionScreen() {
	short Choise = 0;
	cout << "\nChoose What do you want to do? [1-4] : ";
	cin >> Choise;
	return Choise;
}

void PerformTransactionMenueOpstion(enTransactionMenueOption TransactionMenueOption) {

	switch (TransactionMenueOption)
	{
	case enTransactionMenueOption::eDeposit:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionMenue();
		break;

	}
	case enTransactionMenueOption::eWithdraw:
	{
		system("cls");
		ShowWithdrawScreen();
		GoBackToTransactionMenue();
		break;

	}
	case enTransactionMenueOption::eShowTotalBalance:
	{
		system("cls");
		ShowTotalBalanceScreen();
		GoBackToTransactionMenue();
		break;

	}
	case enTransactionMenueOption::eShowMianMenue:
	{

		ShowMainMenue();


	}

	}
}

void ShowTransactionClientScreen() {
	if (!CheckAccessPermissions(enMainMenuePermission::vTransaction)) {
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
	
	}
	system("cls");
	cout << "\n==================================================\n";
	cout << "\t\t Transaction Client Screen \n";
	cout << "\n==================================================\n";
	cout << "   [1] Deposit\n";
	cout << "   [2] Withdraw \n";
	cout << "   [3] Total Balances \n";
	cout << "   [4] Main Menue\n";
	cout << "\n==================================================\n";
	PerformTransactionMenueOpstion((enTransactionMenueOption)ReadTransactionScreen());

}
void ShowExitScreen() {
	cout << "\n-------------------------------------------------\n";
	cout << "\t\t Programs End :-) \n";
	cout << "\n--------------------------------------------------\n";
}
short ReadMainMenueScreen() {
	short Choise = 0;
	cout << "\nChoose What do you want to do? [1-8] : ";
	cin >> Choise;
	return Choise;
}

void PrintUserRecord(sUser User) {
	cout << "|" << left << setw(12) << User.UserName << "|" << left << setw(15) << User.Passwred << "|" << left << setw(8)<<User.Permissions << "|\n";
	cout << "\n ------------------------------------------------------------------------\n";

}

void PrintUserCard(sUser User) {
	cout << "\n--------------------------------------\n";
	cout << "User Name : " << User.UserName << endl;
	cout << "Password : " << User.Passwred << endl;
	cout << "Permission : " << User.Permissions << endl;
	cout << "\n--------------------------------------\n";

}


sUser ConvertLineToUserRecord(string line, string seperator = "#//#") {
	vector<string> vClient;
	sUser User;
	vClient = SplitString(line, seperator);
	User.UserName = vClient[0];
	User.Passwred = vClient[1];
	User.Permissions = stoi(vClient[2]);
	return User;
}



vector<sUser> LoadUserDataFromFile(string FileName) {
	fstream fClient;
	vector<sUser>vUser;
	fClient.open(FileName, ios::in);
	if (fClient.is_open()) {
		string line;
		sUser User;
		while (getline(fClient, line)) {
			User = ConvertLineToUserRecord(line);
			vUser.push_back(User);

		}
		fClient.close();

	}
	return vUser;
}

string ConvertUsersRecordToLine(sUser User, string Seperator = "#//#") {

	string info = "";
	info += User.UserName + Seperator;
	info += User.Passwred + Seperator;
	info += to_string(User.Permissions);
	return info;
}

void ShowAllUsersScreen() {
	vector<sUser>vUser;
	vUser = LoadUserDataFromFile(UsersFileName);
	cout << "\n\t\t\t\tUser List (" << vUser.size() << ") User(s)\n";
	cout << "\n ------------------------------------------------------------------------\n";
	cout << "|" << left << setw(12) << "User Name " << "|" << left << setw(15) << "Password " << "|" << left << setw(8) << "Permission" << "|\n";
	cout << "\n ------------------------------------------------------------------------\n";
	if (vUser.size() == 0) {
		cout << "\n \t\t\t\tNo Users Available in System.\n";
	}
	else {
		for (sUser& User : vUser) {
			PrintUserRecord(User);
		}
	}
}
void ShowListUsersScreen() {
	ShowAllUsersScreen();
}

bool UserExistsByUserName(string UserName) {
	vector<sUser>vUser = LoadUserDataFromFile(UsersFileName);
	for (sUser U : vUser) {
		if (U.UserName == UserName)
			return true;
	}
	return false;
}

int ReadPermission() {
	short Permission = 0;
	char Anser='N';
	cout << "\n Do you want to give  full Access ?y/n? ";
	cin >> Anser;
	if (toupper(Anser) == 'Y')
		return -1;
	cout << "\n Do you want to give access to : \n";
	cout << "\n Show Client list ? y/n? ";
	cin >> Anser;
	if (toupper(Anser) == 'Y')
		Permission += enMainMenuePermission::pListClients;

	cout << "\n Add New Client  ? y/n? ";
	cin >> Anser;
	if (toupper(Anser) == 'Y')
		Permission +=enMainMenuePermission::pAddNewClients;

	cout << "\n Delete Client  ? y/n? ";
	cin >> Anser;
	if (toupper(Anser) == 'Y')
		Permission += enMainMenuePermission::DeleteClients;

	cout << "\n Update Client  ? y/n? ";
	cin >> Anser;
	if (toupper(Anser) == 'Y')
		Permission += enMainMenuePermission::pUpdateClients;

	cout << "\n Find Client  ? y/n? ";
	cin >> Anser;
	if (toupper(Anser) == 'Y')
		Permission += enMainMenuePermission::FindClients;

	cout << "\n Transaction  ? y/n? ";
	cin >> Anser;
	if (toupper(Anser) == 'Y')
		Permission +=  enMainMenuePermission::vTransaction;

	cout << "\n Manage User  ? y/n? ";
	cin >> Anser;
	if (toupper(Anser) == 'Y')
		Permission +=  enMainMenuePermission::pManageUser;
	return Permission;

}

sUser ReadNewUser() {
	sUser User;
	cout << "\n enter user name : ";
	getline(cin >> ws, User.UserName);
	while (UserExistsByUserName(User.UserName)) {
		cout << "\n User with [" << User.UserName << "] alread Exist, Enter another User Name : ";
		getline(cin >> ws, User.UserName);
	}
	cout << "\n enter passoerd : ";
	getline(cin >> ws, User.Passwred);

	User.Permissions = ReadPermission();
	return User;
}

void AddNewUser() {
	sUser User;
	User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertUsersRecordToLine(User));
}

void AddNewUsers() {
	char AddMore = 'Y';
	do {
		cout << "\n Adding New User ....\n\n";
		AddNewUser();
		cout << "\n User add successfuly,Do you need to add another user ? y/n ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
	
}

void ShowAddNewUserScreen() {
	cout << "-----------------------------------------------------\n";
	cout << "\n Add New User Screen \n";
	cout << "\n-----------------------------------------------------\n";
	AddNewUsers();
}

string ReadUserName() {
	string UserName = "";
	cout << "\n enter User Name : ";
	getline(cin >> ws, UserName);
	return UserName;
}

bool FindUserByUserName(string UserName, vector<sUser>vUser, sUser& User) {
	for (sUser& U : vUser) {
		if (U.UserName == UserName) {
			User = U;
			return true;
		}
	}
	return false;
}

void MarkUserForDeleteByUserName(string UserName, vector<sUser>&vUser) {
	for (sUser& U : vUser) {
		if (U.UserName == UserName) {
			U.MarkForDelete = true;
			break;
		}
	}
}


vector <sUser> SaveUserDataToFile(string FileName, vector<sUser> vUser) {
	fstream MyFile;
	MyFile.open(FileName, ios::out);//overwrite
	string DataLine;
	if (MyFile.is_open())
	{
		for (sUser U : vUser)
		{
			if (U.MarkForDelete == false)
			{ //we only write records that are not marked for delete.   
				DataLine = ConvertUsersRecordToLine(U);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUser;
}

bool DeleteUsersByUserName(string UserName,vector<sUser>vUser) {
	if (UserName == "Admin") 
	{
		cout << "\n You can NOT Delete this User.";
		return false;
	}
	char Anser = 'Y';

	sUser User;
	
		if (FindUserByUserName(UserName, vUser, User)) {
			PrintUserCard(User);
			cout << "\n Are you sure to delete this User ? y/n ? ";
			cin >> Anser;
			if (toupper(Anser) == 'Y') {
				MarkUserForDeleteByUserName(UserName,vUser);
				SaveUserDataToFile(UsersFileName, vUser);
				vUser = LoadUserDataFromFile(UsersFileName);
				cout << "\n User Delete successfuly.\n";
				return true;
			}
			

		}
		else {
			cout << "\n User with User Name (" << UserName << ") is Not Found!";

		}
	
}

void ShowDeleteUserScreen() {
	cout << "-----------------------------------------------------\n";
	cout << "\n Add New User Screen \n";
	cout << "\n-----------------------------------------------------\n";
	string UserName= ReadUserName();
	vector<sUser>vUser = LoadUserDataFromFile(UsersFileName);
	DeleteUsersByUserName(UserName, vUser);
}

sUser ChangeUserRecord(string UserName) {
	sUser User;
	User.UserName = UserName;
	cout << "\n enter passoerd : ";
	getline(cin >> ws, User.Passwred);

	User.Permissions = ReadPermission();
	return User;
}

bool UpdateUserByUserName(string UserName, vector<sUser>vUser) {
	char Anser = 'Y';
	sUser User;
	
		if (FindUserByUserName(UserName, vUser, User)) {
			PrintUserCard(User);
			cout << "\n Are you sure to Update this User ? y/n ? ";
			cin >> Anser;
			if (toupper(Anser) == 'Y') {
				for (sUser& U : vUser) {
					if (U.UserName == UserName) {
						U = ChangeUserRecord(UserName);

					}
				}
				SaveUserDataToFile(UsersFileName, vUser);
				vUser = LoadUserDataFromFile(UsersFileName);
				cout << "\n User Update successfuly.\n";
				return true;
			}
			

		}
		else {
			cout << "\n User with User Name (" << UserName << ") is Not Found!";
			return false;
		}
	
}

void ShowUpdateUsersScreen() {
	cout << "-----------------------------------------------------\n";
	cout << "\n Update  User Screen \n";
	cout << "\n-----------------------------------------------------\n";
	string UserName = ReadUserName();
	vector<sUser>vUser = LoadUserDataFromFile(UsersFileName);
	UpdateUserByUserName(UserName, vUser);

}

void ShowFindUserByUserName() {
	cout << "-----------------------------------------------------\n";
	cout << "\n Find  User Screen \n";
	cout << "\n-----------------------------------------------------\n";
	vector<sUser>vUser = LoadUserDataFromFile(UsersFileName);
	
		string UserName = ReadUserName();
		sUser User;
		if (FindUserByUserName(UserName, vUser, User)) {
			PrintUserCard(User);
		}
		else {
			cout << "\n User with User Name (" << UserName << ") is Not Found!";

		}
		
}

bool CheckAccessPermissions(enMainMenuePermission Permission) {
	if (CurrentUser.Permissions == enMainMenuePermission::eAll)
		return true;
	if ((Permission & CurrentUser.Permissions) == Permission)
		return true;
	else
		return false;
	
}

void GoBackToManageScreen() {
	cout << "\n\nPress any Key to go back to Manage Screen ...";
	system("pause>0");
	ShowManageUsersScreen();
}

void PerformManageUserOption(enMangeUser Option) {
	switch (Option)
	{
	case eShowListUser:
	{
		system("cls");
		ShowListUsersScreen();
		GoBackToManageScreen();
		break;

	}
	case eAddNewUser:
	{
		system("cls");
		ShowAddNewUserScreen();
		GoBackToManageScreen();

		break;
	}
	case eDeleteUser: {
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageScreen();

		break;
	}
	case eUpdateUser: {
		system("cls");
		ShowUpdateUsersScreen();
		GoBackToManageScreen();
		break;
	}
	case eFindUser: {
		system("cls");
		ShowFindUserByUserName();
		GoBackToManageScreen();
		break;
	}
	case eMainMenue: {
		ShowMainMenue();
		break;
	}
	}
}


short ReadManageUserScreen() {
	short Choise = 0;
	cout << "\nChoose What do you want to do? [1-6] : ";
	cin >> Choise;
	return Choise;
}

void ShowManageUsersScreen() {
	if (!CheckAccessPermissions(enMainMenuePermission::pManageUser)) {
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		
	}
	system("cls");
	cout << "\n==================================================\n";
	cout << "\t\t Manage Users Minue Screen \n";
	cout << "\n==================================================\n";
	cout << "   [1] ListUser\n";
	cout << "   [2] Add New User \n";
	cout << "   [3] Delete User  \n";
	cout << "   [4] Update User\n";
	cout << "   [5] Finde User\n";
	cout << "   [6] Main Menue\n";

	cout << "\n==================================================\n";

	PerformManageUserOption((enMangeUser)ReadManageUserScreen());
}


void PerformMainMenueOpstion( enWhatChoose MainMenueOption) {
	
	switch (MainMenueOption)
	{
	case enWhatChoose::eListClient:
	{
			system("cls");
			ShowAllClientScreen();
			GoBackToMainMenue();
			break;
		

	}
	case enWhatChoose::eAddNewClient:
	{
			system("cls");
			ShowAddNewClientsScreen();
			GoBackToMainMenue();
			break;
	
	}
	case enWhatChoose::eDleletClient:
	{

			system("cls");
			ShowDeleteClientScreen();
			GoBackToMainMenue();
			break;
		

	}
	case enWhatChoose::eUpdateClient:
	{

			system("cls");
			ShowUpdateClientScreen();
			GoBackToMainMenue();
			break;
		

	}
	case enWhatChoose::eFindClient:
	{

			system("cls");
			ShowFindClientScreen();
			GoBackToMainMenue();
			break;
	}
	case enWhatChoose::eShowTransaction:
	{

			system("cls");
			ShowTransactionClientScreen();
			break;
	

	}
	case enWhatChoose::eManage:
	{

			system("cls");
			ShowManageUsersScreen();
			break;
		

	}
	case enWhatChoose::LogOut:
	{
	
		Login();
		break;
	}

	}
}
void ShowMainMenue() {
	system("cls");
	cout << "==============================================================\n\n";
	cout << "                Main Menue Screen\n\n";
	cout << "==============================================================\n\n";
	cout << "   [1] Show Client List\n";
	cout << "   [2] Add New Client \n";
	cout << "   [3] Delete Client \n";
	cout << "   [4] Update Client Info\n";
	cout << "   [5] Find Client \n";
	cout << "   [6] Transaction \n";
	cout << "   [7] Manage Users \n";
	cout << "   [8] LogOut \n";


	cout << "==============================================================\n";
	PerformMainMenueOpstion((enWhatChoose)ReadMainMenueScreen());


}

bool FindUserByUserNameAndPasssored(string UserNmae,string Password,sUser &User) {
	vector<sUser>vUser = LoadUserDataFromFile(UsersFileName);
	for (sUser U : vUser) {
		if (U.Passwred == Password && U.UserName == UserNmae) {
			User = U;
			return true;

		}
	}
	return false;
}



bool LoadUserInfo(string UserName, string Password) {
	if (FindUserByUserNameAndPasssored(UserName, Password, CurrentUser))
		return true;
	else
		return false;
}

void Login() {
	bool LoginFiald = false;
	sUser User;
	do {
		system("cls");
		
		cout << "===================================================\n";
		cout << "                  Login Screen \n";
		cout << "===================================================\n";
		if (LoginFiald) {
			cout << "\n Invalid UserName/Password !. \n";
		}
		cout << "\n enter user name : ";
		getline(cin >> ws, User.UserName);
		cout << "\n enter password : ";
		getline(cin >> ws, User.Passwred);
		LoginFiald = (!LoadUserInfo(User.UserName, User.Passwred));

	} while (LoginFiald);
	ShowMainMenue();
}
int main() {
	Login();
	system("pause>0");
}