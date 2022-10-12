#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"
EN_serverError_t ServerError;

/*Implement server-side accounts database
1- Create a global array of ST_accountsDB_t for the valid accounts database.
2- ST_accountsDB_t accountsDB[255];
3- Fill in the array initially with any valid data.
4- This array has a maximum of 255 element/account data.
5- You can fill up to 10 different accounts for the sake of testing.
6- Example of a running account: {2000.0, RUNNING, "8989374615436851"}.
7- Example of a blocked account, its card is stolen: {100000.0, BLOCKED, "5807007076043875"}. */

/*Account data base for PAN after Luhn checked */
ST_accountsDB_t accountsDB[255] = { {10000.00, RUNNING ,"4325285760741428"},{80000.50,RUNNING, "4325018311588364"},
{5000.00,BLOCKED,"4325103823277878"},{10000.00,RUNNING, "5433657268387670"},{9000.50,BLOCKED, "5428606576650482"},
{7005.00,RUNNING,"5480088524745216"},{15000.25,RUNNING, "5530736301385016"},{11507.00,BLOCKED, "5516164881527170"},
{113054.00,BLOCKED,"5518650068608476"},{1955.55,RUNNING, "5502088000763588"},{4321.00,RUNNING, "5558641045418452"} };

/*Implement server-side transactions' database
1- Create a global array of ST_transaction_t
2- Fill in the array initially with Zeros.
3- This array has a maximum of 255 element/transaction data*/
ST_transaction_t transactionDataBase[255] = { 0 };

/*index is the loction of the PAN if stored in the server data*/
uint8_t index;
uint8_t SearchIndexLocation(ST_cardData_t* cardData)
{
	for (uint8_t i = 0; i < 255; i++)
	{
		if (strcmp(cardData->primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0)
			return i;
	}
	return -1;
}

/*Implement recieveTransactionData function
1- This function will take all transaction data and validate its data, it contains all server logic.
2- It checks the account details and amount availability.
3- If the account does not exist return FRAUD_CARD, if the amount is not available
will return DECLINED_INSUFFECIENT_FUND, if the account is blocked will return DECLINED_STOLEN_CARD,
if a transaction can't be saved will return INTERNAL_SERVER_ERROR , else returns APPROVED.
4- It will update the database with the new balance. */
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	if (isValidAccount(&(transData->cardHolderData), accountsDB->primaryAccountNumber) == ACCOUNT_NOT_FOUND)
	{
		transData->transState = FRAUD_CARD;
		saveTransaction(transData);
		return FRAUD_CARD;
	}



	if (isAmountAvailable(&(transData->terminalData), accountsDB->primaryAccountNumber) == LOW_BALANCE);
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		saveTransaction(transData);
		return DECLINED_INSUFFECIENT_FUND;
	}

	if(isBlockedAccount(&(accountsDB->state) == BLOCKED_ACCOUNT));
	{
		transData->transState = DECLINED_STOLEN_CARD;
		saveTransaction(transData);
		return DECLINED_STOLEN_CARD;
	}
	
	/*update */
	accountsDB[index].balance = accountsDB[index].balance - transData->terminalData.transAmount;

	/*save the transaction in the server database*/
	if (saveTransaction(&(transData)) == SAVING_FAILED);
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		saveTransaction(transData);
		return INTERNAL_SERVER_ERROR;
	}


	return APPROVED;
}
/*Implement isValidAccount function
1- This function will take card data and validate these data.
2- It checks if the PAN exists or not in the server's database.
3- If the PAN doesn't exist will return DECLINED_STOLEN_CARD, else will return OK*/
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{

	if (index == 255)
		return ACCOUNT_NOT_FOUND;
	else
		return APPROVED;


}

/*Implement isBlockedAccount function
1- This function takes a reference to the account into the database and verifies if it is blocked or not.
2- If the account is running it will return SERVER_OK, else if the account is blocked
it will return BLOCKED_ACCOUNT.*/
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
		if (accountsDB[index].state == BLOCKED)
		{
			return BLOCKED_ACCOUNT;
		}
	return SERVER_OK;
}



/*Implement isAmountAvailable function
1-This function will take terminal data and a reference to the account in the database and check if 
the account has a sufficient amount to withdraw or not.
2 -It checks if the transaction's amount is available or not.
3- If the transaction amount is greater than the balance in the database will return LOW_BALANCE
, else will return SERVER_OK.*/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if ((termData->transAmount) > (accountsDB[index].balance))
		return LOW_BALANCE;

	return SERVER_OK;
}

//not yet
/*Implement saveTransaction function
1- This function will take all transaction data into the transactions database.
2- It gives a sequence number to a transaction,
this number is incremented once a transaction is processed into the server.
3- If saves any type of transaction, APPROVED or DECLINED, with the specific reason for declining/transaction state.
4- If transaction can't be saved will return SAVING_FAILED, else will return OK*/

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	transactionDataBase[index].cardHolderData = transData->cardHolderData;
	transactionDataBase[index].terminalData = transData->terminalData;
	transactionDataBase[index].transState = transData->transState;
	transData->transactionSequenceNumber = rand();
	transactionDataBase[index].transactionSequenceNumber = transData->transactionSequenceNumber ;
	
	if (transactionDataBase[index].transactionSequenceNumber == 0)
	{
		transData->transState = SAVING_FAILED;
		return SAVING_FAILED;
	}
	return SERVER_OK;
}



/*Implement listSavedTransactions function
1- This function prints all transactions found in the transactions DB. */
void listSavedTransactions(void)
{
	
	printf("#########################\n");
	printf("Transaction Sequence Number: %i\n", transactionDataBase[index].transactionSequenceNumber);
	printf("Transaction Date : %s \n", transactionDataBase[index].terminalData.transactionDate);
	printf("Transaction Amount : %0.2f\n", transactionDataBase[index].terminalData.transAmount);
	//printf("Transaction State: %s\n", transactionDataBase[index]);
	printf("Terminal Max Amount: %0.2f\n", transactionDataBase[index].terminalData.maxTransAmount);
	printf("Cardholder Name: %s\n", transactionDataBase[index].cardHolderData.cardHolderName);
	printf("PAN : %s\n", transactionDataBase[index].cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s\n", transactionDataBase[index].cardHolderData.cardExpirationDate);
	printf("#########################\n\n");
}



/*Test Functions*/
void recieveTransactionDataTest(void)
{
	uint16_t T, TestAmount;

	ST_accountsDB_t TestAccountsDB[255] = { 7005.00,RUNNING,"5480088524745216" };
	uint8_t UserPrimaryAccountNumber[20];
	float TestTransAmount;




	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: recieveTransactionDataTest\n");
	printf("Enter PAN: ");

	

	for (int n = 1; n < 5; n++)
	{
		printf("\nTest Case : %d\n", n);
		printf("Input Data: ");
#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%s", &UserPrimaryAccountNumber);
		
		

		if ((strcmp(UserPrimaryAccountNumber, TestAccountsDB->primaryAccountNumber) == 0))
		{
			printf("balance= %0.4f\n", TestAccountsDB->balance);
			printf("Enter Amount: ");
#pragma warning(suppress : 4996) //C4996 Warning
			scanf("%f", &TestTransAmount);
			if ((TestAccountsDB->state == BLOCKED))
			{
				printf("Expected Result: BLOCKED_ACCOUNT\n");
				printf("Actual Result: BLOCKED_ACCOUNT\n\n");
			}

			if (((TestTransAmount) > (TestAccountsDB->balance)))
			{
				printf("Expected Result: LOW_BALANCE\n");
				printf("Actual Result: LOW_BALANCE\n\n");
			}
			else
			{
				printf("Expected Result: OK_Transaction\n");
				printf("Actual Result: OK_Transaction\n");
				
			}
		}
		else
		{
			printf("Expected Result: Fraud_CARD\n");
			printf("Actual Result: Fraud_CARD\n");
			
		}

	}
}




void isValidAccountTest(void)
{
	uint8_t TestPrimaryAccountNumber[20] ="4325285760741428";
	uint8_t UserPrimaryAccountNumber[20];

	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: isValidAccountTest\n");
	for (int n = 1; n <4; n++)
	{
		
		
			printf("Test Case : %d\n", n);
			printf("Input Data: ");
			#pragma warning(suppress : 4996) //C4996 Warning
			scanf("%s", &UserPrimaryAccountNumber);
			
		
				
		if (strcmp(UserPrimaryAccountNumber, TestPrimaryAccountNumber) == 0)
		{
			printf("Expected Result: Valid_CARD\n");
			printf("Actual Result: %s\n\n", TestPrimaryAccountNumber);
		}
		else
		{
			printf("Expected Result: ACCOUNT_NOT_FOUND\n");
			printf("Actual Result: ACCOUNT_NOT_FOUND\n\n");
		}	
		
	}
}

void isBlockedAccountTest(void)
{
	ST_accountsDB_t TestAccountsDB[255] = { 9000.50,BLOCKED, "5428606576650482" };
	uint8_t UserPrimaryAccountNumber[20];

	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: isBlockedAccountTest\n");
	for (int n = 1; n < 4; n++)
	{


		printf("Test Case : %d\n", n);
		printf("Input Data: ");
#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%s", &UserPrimaryAccountNumber);



		if (strcmp(UserPrimaryAccountNumber, TestAccountsDB->primaryAccountNumber) == 0)
		{
			
			if (TestAccountsDB->state == BLOCKED)
			{
				printf("Expected Result: BLOCKED_ACCOUNT\n");
				printf("Actual Result: BLOCKED_ACCOUNT\n\n");
			}
			else
			{
				printf("Expected Result: NOt_Blocked_ACCOUNT\n");
				printf("Actual Result: NOt_Blocked_ACCOUNT\n\n");
			}
		}
	}
}

void isAmountAvailableTest(void)
{

	ST_accountsDB_t TestAccountsDB[255] = { 7005.00,RUNNING,"5480088524745216" };
	uint8_t UserPrimaryAccountNumber[20];
		float TestTransAmount;


	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: isAmountAvailableTest\n");
	printf("Enter PAN: ");
#pragma warning(suppress : 4996) //C4996 Warning
	scanf("%s", &UserPrimaryAccountNumber);
	printf("balance= %0.4f\n", TestAccountsDB->balance);
	
	for (int n = 1; n < 5; n++)
	{
		printf("Test Case : %d\n", n);
		printf("Input Data: ");
		#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%f", &TestTransAmount);


			if ((TestTransAmount) > (TestAccountsDB->balance))
			{
				printf("Expected Result: LOW_BALANCE\n");
				printf("Actual Result: LOW_BALANCE\n\n");
			}
			else
			{
				printf("Expected Result: Transaction is OK\n");
				printf("Actual Result: Transaction is OK\n\n");
			}
		
	}
}

void saveTransactionTest(void)
{

}

void listSavedTransactionsTest(void)
{
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: listSavedTransactionsTest\n");
	for (int n = 1; n < 4; n++)
	{
		printf("Test Case : %d\n", n);
		printf("Input Data: ");

		uint16_t TransactionSequenceNumber =rand(), tRansamount = 0, mAXnsamount = 500;
		uint8_t trANsactiondate[10] = "22/09/2022", cArdNAme[25] = "Hazm Abdelgalil Alwakil", pAn[20], eXpdate[5], sTate[10] = "Accepted";
		printf("Enter PAN, card date,transaction amount, max amount \n");
#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%s ", &pAn);
#pragma warning(suppress : 4996) //C4996 Warning
		scanf(" %s ", &eXpdate);
#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%d", &tRansamount);
#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%d", &mAXnsamount);



		printf("#########################\n");
		printf("Transaction Sequence Number: %i\n", TransactionSequenceNumber);
		printf("Transaction Date : %s \n", trANsactiondate);
		printf("Transaction Amount : %i\n", tRansamount);
		printf("Transaction State: %s\n", sTate);
		printf("Terminal Max Amount: %i\n", mAXnsamount);
		printf("Cardholder Name: %s\n", cArdNAme);
		printf("PAN : %s\n", pAn);
		printf("Card Expiration Date: %s\n", eXpdate);
		printf("#########################\n\n");
	}
}