#include <iostream>
#include <thread>
#include <mutex>

typedef struct bankAccount
{
	int accountNumber;
	std::string accountName;
	double balance;
}BankAccount_t;
class Bank
{
public:
	Bank();
	~Bank();
	std::mutex Balancemtx;
	static int NumOfAccounts;
	BankAccount_t* accountsDataBase[100];
	BankAccount_t* CreateAcount(std::string name, double balance);
	void DeleteAccount(int AccountNumber);
	bool AccountWithdral(int AccountNumber, double amount);
	void AccountDeposit(int AccountNumber, double amount);
private:

};

bool Bank::AccountWithdral(int AccountNumber, double amount)
{
	bool bStatus = false; 
	BankAccount_t* currentAccount = accountsDataBase[AccountNumber];
	if (amount <= currentAccount->balance)
	{
		std::lock_guard<std::mutex> lock(Balancemtx);
		currentAccount->balance -= amount; 
		std::cout << "Remaining Balanc :" << currentAccount->balance << std::endl;
		bStatus = true; 
	}
	else
	{
		std::cout << "No suffecient balance \n";
		bStatus = false;
	}
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
	return bStatus; 
}

void Bank::AccountDeposit(int AccountNumber, double amount)
{
	BankAccount_t* currentAccount = accountsDataBase[AccountNumber];
	std::lock_guard<std::mutex> lock(Balancemtx);
	currentAccount->balance += amount;
}

void Bank::DeleteAccount(int AccountNumber)
{
	if (accountsDataBase[AccountNumber] != nullptr)
	{
		delete accountsDataBase[AccountNumber];
		accountsDataBase[AccountNumber] = nullptr;
	}
	else
	{
		std::cout << "Account not found" << std::endl;
	}
}

BankAccount_t* Bank::CreateAcount(std::string name, double balance)
{
	BankAccount_t* newAccount = new BankAccount_t;
	newAccount->accountNumber = NumOfAccounts;
	newAccount->accountName = name;
	newAccount->balance = balance;
	accountsDataBase[NumOfAccounts] = newAccount;
	NumOfAccounts++;
	return newAccount;
}

Bank::Bank()
{
}

Bank::~Bank()
{
}
int Bank::NumOfAccounts = 0;

int main() {
	Bank myBank;
	BankAccount_t* account = myBank.CreateAcount("khaled", 50000);
	while (1)
	{
		std::thread withdraw100(&Bank::AccountWithdral, &myBank, account->accountNumber, 100);
		std::thread withdraw50(&Bank::AccountWithdral, &myBank, account->accountNumber, 50);
		withdraw100.join(); // Wait for thread to finish
		withdraw50.join();
	}
}