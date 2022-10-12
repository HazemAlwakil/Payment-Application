#include <stdio.h>
#include <string.h>
#include "app.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"
#include<conio.h>


extern ST_accountsDB_t accountsDB[255];
extern uint8_t index;
float  balance = 0;
void main()
{
	printf("\tPayment Application\n_____________________________________\n");
	appStart();

	/*Test Functions*/

	//getCardHolderNameTest();
	//getCardExpiryDateTest();
	//getCardPANTest();
	//getTransactionDateTest();
	//isCardExpriedTest();
	//getTransactionAmountTest();
	//isBelowMaxAmountTest();
	//setMaxAmountTest();
	//isValidCardPANTest();
	//recieveTransactionDataTest();
	//isValidAccountTest();
	//isBlockedAccountTest();
	//isAmountAvailableTest();
	//saveTransactionTest();
	//listSavedTransactionsTest();
}
	


void appStart(void)
{

	EN_cardError_t CardError;
	EN_terminalError_t TerminalError;
	EN_transState_t TransStateError;
	EN_serverError_t ServerError;

	/*cardHolderData: {"cardHolderName","cardExpirationDate","primaryAccountNumber"}*/
	/*terminalData: {transAmount,maxTransAmount,"transactionDate"}*/

	/* to store user data { {"cardHolderData"}, {"terminalData"}, transState, transactionSequenceNumber }*/
	ST_transaction_t getUserData = { {0,0,0}, {0,0,0},0,0};
	
	//Ask for Name
	CardError = getCardHolderName(&getUserData.cardHolderData);
	if (CardError == WRONG_NAME)
	{
		printf("\n******* Wrong Name *******\nApplication had been terminated.\n\n");
		exit(1);
	}
	printf("_____________________________________\n\tName Accepted\n_____________________________________\n");

	//Ask for expiry date
	CardError = getCardExpiryDate(&getUserData.cardHolderData);
	if (CardError == WRONG_EXP_DATE)
	{
		printf("\nWrong Expiry Date or not in \"MM/YY\" Format\nApplication had been terminated.\n\n");
		exit(1);
	}
	printf("_____________________________________\n\tDate Accepted\n");


	//Ask for PAN
	CardError = getCardPAN(&getUserData.cardHolderData);
	if (CardError == WRONG_PAN)
	{
		printf("\n****** Wrong Card Number ******\nApplication had been terminated.\n\n");
		exit(1);
	}

	//Check if PAN is LUHN validated
	TerminalError = isValidCardPAN(&getUserData.cardHolderData);
	if (TerminalError == INVALID_CARD)
	{
		printf("\n****** Invalid Card Number ******\nApplication had been terminated.\n\n");
		exit(1);
	}




	
		index = SearchIndexLocation(&getUserData.cardHolderData);


	TerminalError = getTransactionDate(&getUserData.terminalData);
	if (TerminalError == WRONG_DATE)
	{
		printf("\n\tWrong Date.\n");
		printf("*****************************\n");
		printf("*You must enter today's date*\n");
		printf("*****************************\n\n");
		exit(1);
	}

	TerminalError = isCardExpired(&getUserData.cardHolderData, &getUserData.terminalData);
	if (TerminalError == EXPIRED_CARD)
	{
		printf("\n***** The Card is Expired *****\n\nApplication had been terminated.\n\n");
		exit(1);
	}
	printf("_______________________________\nDate Accepted\n\nChecking for card validation\n");
	
	/*As a delay function*/
	uint16_t c, d;
	for (c = 1; c <= 30000; c++)
		for (d = 1; d <= 30000; d++)
		{}
	/*Check Account in Database or not*/
	ServerError= isValidAccount(&getUserData.cardHolderData, accountsDB->primaryAccountNumber);
		if (ServerError == FRAUD_CARD)
		{
			printf("_______________________________\n*Account information not Found*\n\nApplication had been terminated.\n\n");
			exit(1);
		}


		ServerError = isBlockedAccount(&getUserData.transState);
		if (ServerError ==BLOCKED_ACCOUNT)
		{
			printf("_______________________________\n*Stolen Card*\n\nApplication had been terminated.\n\n");
			exit(1);
		}

	TransStateError = recieveTransactionData(&getUserData);



	printf("\n--------> Card Accepted <---------\n");
	TerminalError = setMaxAmount(&getUserData.terminalData, getUserData.terminalData.maxTransAmount);
	if (TerminalError == INVALID_MAX_AMOUNT)
	{
		
		printf("INVALID MAX AMOUNT.\n");
		exit(1);
	}

	balance = accountsDB[index].balance + getUserData.terminalData.transAmount;
	printf("Your curent balance is %0.4f.\n", balance);
	TerminalError = getTransactionAmount(&getUserData.terminalData);
	
	if (TerminalError == INVALID_AMOUNT)
	{
		printf("INVALID AMOUNT.\n");
		exit(1);
	}

	if (isBelowMaxAmount(&getUserData.terminalData))
	{
		printf("_______________________________\n*Exceeded Max Amount *\n\nApplication had been terminated.\n\n\n");
		exit(1);
	}
	ServerError= isAmountAvailable(&getUserData.terminalData,&getUserData.terminalData.maxTransAmount);
	 if (ServerError == LOW_BALANCE)
	{
		printf("Declined Insufficient Funds!\n");
		exit(1);
	}
	 if (ServerError == INTERNAL_SERVER_ERROR)
	{
		printf("Server Down\n");
		exit(1);
	}
	

	 listSavedTransactions();
				
}


