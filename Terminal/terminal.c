#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../Card/card.h"
#include "terminal.h"

uint32_t TransactionYear, TransactionMonth, CardMonth, CardYear;
/*Implement getTransactionDate function
1-This function will ask for the transaction data and store it in terminal data.
2-Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
3-If the transaction date is NULL, less than 10 characters
or wrong format will return WRONG_DATE error, else return OK.*/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	uint32_t TransactionYear, TransactionMonth, TransactionDay;

	time_t t;
	time(&t);
	#pragma warning(suppress : 4996) //C4996 Warning
	struct tm* current = localtime(&t);
	printf("Enter Transaction Date in \"DD/MM/YYYY\"\n it must equal : %02d/%02d/%02d\n", current->tm_mday, current->tm_mon + 1, current->tm_year + 1900);
	#pragma warning(suppress : 4996) //C4996 Warning
	scanf("%s", &termData->transactionDate);

	/*check if transaction date is valid or not*/
	if ((strlen(termData->transactionDate) < 10) || (strlen(termData->transactionDate) > 10) || (termData->transactionDate == NULL))
		return WRONG_DATE;

	/*check the third and sixth character is '/' */
	for (uint8_t i = 0; i < strlen(termData->transactionDate); i++)
	{
		if (i == 2 || i == 5)
		{
			if (termData->transactionDate[i] != '/')
				return WRONG_DATE;
			else
				continue;
		}

	}
	/*To make sure transaction date is the same current date*/
	TransactionYear = ((((termData->transactionDate[6] - '0') * 1000) + ((termData->transactionDate[7] - '0') * 100) + ((termData->transactionDate[8] - '0') * 10)) + ((termData->transactionDate[9] - '0')));
	TransactionMonth = (((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0'));
	TransactionDay = (((termData->transactionDate[0] - '0') * 10) + (termData->transactionDate[1] - '0'));
	if ((TransactionDay != (current->tm_mday)) || (TransactionYear != (current->tm_year + 1900)) || ((TransactionMonth) != (current->tm_mon + 1)))
		return WRONG_DATE;

	return TERMINAL_OK;
}
/*Implement isCardExpried function
1-This function compares the card expiry date with the transaction date.
2-If the card expiration date is before the transaction date will return EXPIRED_CARD, else return OK.*/
EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t* termData)
{
	

	CardMonth = (((cardData->cardExpirationDate[0] - '0') * 10) + (cardData->cardExpirationDate[1] - '0'));
	CardYear = 2000 + ((((cardData->cardExpirationDate[3] - '0') * 10) + (cardData->cardExpirationDate[4] - '0')));
	TransactionYear = ((((termData->transactionDate[6] - '0') * 1000) + ((termData->transactionDate[7] - '0') * 100) + ((termData->transactionDate[8] - '0') * 10)) + ((termData->transactionDate[9] - '0')));
	TransactionMonth = (((termData->transactionDate[3] - '0') * 10) + (termData->transactionDate[4] - '0'));

	/*check card year is greater than transaction year*/
	if (TransactionYear > CardYear)
		return EXPIRED_CARD;

	/*check card Month is greater than transaction Month for the same year*/
	else if (TransactionYear == CardYear)
	{
		if (TransactionMonth > CardMonth)
			return EXPIRED_CARD;

	}

	return TERMINAL_OK;
}
/*implement isValidCard function
1-This function checks if the PAN is a Luhn number or not.
2-If PAN is not a Luhn number will return INVALID_CARD, else return OK.*/
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	uint8_t sumodd = 0, sumeven = 0, sumLuhn = 0, doubleEven = 0;
	/*get sum of odd index*/
	for (char i = strlen(cardData->primaryAccountNumber) - 1; i >= 0; i = i - 2)
	{
		sumodd = sumodd + (cardData->primaryAccountNumber[i] - '0');
	}
	/*get sum of even (index*2)- if sum the double greater than 9 will sum the two numbers*/
	for (char i = strlen(cardData->primaryAccountNumber) - 2; i >= 0; i = i - 2)
	{
		doubleEven = (2 * (cardData->primaryAccountNumber[i] - '0'));
		if (doubleEven > 9)
			doubleEven = (doubleEven - 9);
		sumeven = sumeven + doubleEven;
	}
	sumLuhn = sumodd + sumeven;

	if (sumLuhn % 10 != 0)
		return INVALID_CARD;

	return TERMINAL_OK;
}

/*Implement gatTransactionAmount function
* 1-This function asks for the transaction amount and saves it into terminal data.
2-If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return OK.*/
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("Maximum limit is  %0.2f\n", termData->maxTransAmount);
	printf("Enter the transaction amount: ");
	#pragma warning(suppress : 4996) //C4996 Warning
	scanf("%f", &(termData->transAmount));
	//void listSavedTransactions(void);
	/*check the amount is greater than zero*/
	if ((termData->transAmount <= 0))
		return INVALID_AMOUNT;

	return TERMINAL_OK;
}

/*Implement isBelowMaxAmount function
1-This function compares the transaction amount with the terminal max amount.
2-If the transaction amount is larger than the terminal max amount will return EXCEED_MAX_AMOUNT, else return OK.*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if ((termData->transAmount) > (termData->maxTransAmount))
		return EXCEED_MAX_AMOUNT;
	return TERMINAL_OK;
}

/*Implement setMaxAmount function
1- This function sets the maximum allowed amount into terminal data.
2-Transaction max amount is a float number.
3- If transaction max amount less than or equal to 0 will return INVALID_MAX_AMOUNT error, else return OK.*/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{	/*Set Max Amount*/
	printf("\nEnter Maximum Amount limit : ");
	#pragma warning(suppress : 4996) //C4996 Warning
	scanf("%f", &(termData->maxTransAmount));
	
	/*Check greater than Zero*/

	if (termData->maxTransAmount <= 0)
		return INVALID_MAX_AMOUNT;
	
	return TERMINAL_OK;

}

/*Test Functions*/
void getTransactionDateTest(void)
{
	uint8_t GetTransactionDateTest[11];
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: getTransactionDateTest\n");
	printf("Assumed transaction date is 09/2022\n");
	for (int n = 1; n <= 5; n++)
	{

		printf("Test Case : %d\n", n);
		printf("Input Data: ");
#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%s", &GetTransactionDateTest);
		uint32_t TransactionYear, TransactionMonth, TransactionDay;

		time_t t;
		time(&t);
#pragma warning(suppress : 4996) //C4996 Warning
		struct tm* current = localtime(&t);

		/*check if transaction date is valid or not*/
		TransactionYear = ((((GetTransactionDateTest[6] - '0') * 1000) + ((GetTransactionDateTest[7] - '0') * 100) + ((GetTransactionDateTest[8] - '0') * 10)) + ((GetTransactionDateTest[9] - '0')));
		TransactionMonth = (((GetTransactionDateTest[3] - '0') * 10) + (GetTransactionDateTest[4] - '0'));
		TransactionDay = (((GetTransactionDateTest[0] - '0') * 10) + (GetTransactionDateTest[1] - '0'));
		 if ((strlen(GetTransactionDateTest) < 10) || (strlen(GetTransactionDateTest) > 10) || (GetTransactionDateTest == NULL))
		{
			printf("Expected Result: Wrong_Transaction_Date\n");
			printf("Actual Result: Wrong_Transaction_Date \n\n");
		}

		/*check the third and sixth character is '/' */

		else if (GetTransactionDateTest[2] != '/')
				{
					printf("Expected Result: Wrong_Transaction_Date\n");
					printf("Actual Result: Wrong_Transaction_Date \n\n");
				}
		else if (GetTransactionDateTest[5] != '/')
		{
			printf("Expected Result: Wrong_Transaction_Date\n");
			printf("Actual Result: Wrong_Transaction_Date \n\n");
		}
		/*To make sure transaction date is the same current date*/
		
		else if ((TransactionDay != (current->tm_mday)) || (TransactionYear != (current->tm_year + 1900)) || ((TransactionMonth) != (current->tm_mon + 1)))
		{
			 TransactionYear = ((((GetTransactionDateTest[6] - '0') * 1000) + ((GetTransactionDateTest[7] - '0') * 100) + ((GetTransactionDateTest[8] - '0') * 10)) + ((GetTransactionDateTest[9] - '0')));
			 TransactionMonth = (((GetTransactionDateTest[3] - '0') * 10) + (GetTransactionDateTest[4] - '0'));
			 TransactionDay = (((GetTransactionDateTest[0] - '0') * 10) + (GetTransactionDateTest[1] - '0'));
			printf("Expected Result: Wrong_Transaction_Date\n");
			printf("Actual Result: Wrong_Transaction_Date \n\n");
		}

		else
		{
			printf("Expected Result: OK_Transaction_Date\n");
			printf("Actual Result: %s\n\n", GetTransactionDateTest);
		}

	}

}

void isCardExpriedTest(void)
{

	uint8_t GetTransactionDateTest[11], CardDateTest[5];
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: isCardExpriedTest\n");
	printf("Enter Transaction Date: ");
	#pragma warning(suppress : 4996) //C4996 Warning
	//scanf("%s", &GetTransactionDateTest);
	for (int n = 1; n <= 5; n++)
	{

		printf("Test Case : %d\n", n);
		printf("Input Data: ");
		#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%s", &CardDateTest);
	
		CardMonth = (((CardDateTest[0] - '0') * 10) + (CardDateTest[1] - '0'));
		CardYear = 2000 + ((((CardDateTest[3] - '0') * 10) + (CardDateTest[4] - '0')));
		TransactionYear = 2022;
		TransactionMonth = 9;

		if (TransactionYear > CardYear)
		{
			printf("Expected Result: Expired Card\n");
			printf("Actual Result: Expired Card \n\n");
		}
		else if ((TransactionYear == CardYear)&& (TransactionMonth > CardMonth))
		{
				printf("Expected Result:Expired Card\n");
				printf("Actual Result: Expired Card \n\n");
		}
		else
		{
			printf("Expected Result: OK_Card\n");
			printf("Actual Result: %s > 09/22\n\n", CardDateTest);
		}
	}
}

void getTransactionAmountTest(void)
{
	float TestTransAmount;
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: getTransactionAmountTest\n");
	for (int n = 1; n <= 5; n++)
	{

		printf("Test Case : %d\n", n);
		printf("Input Data: ");
		#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%f", &TestTransAmount);
		
		if ((TestTransAmount <= 0))
		{
			printf("Expected Result: Invalid_Amount\n");
			printf("Actual Result: Invalid_Amount \n\n");
		}
		else
		{
			printf("Expected Result: OK_Amount\n");
			printf("Actual Result: %0.4f\n\n", TestTransAmount);
		}

	}
}

void isBelowMaxAmountTest(void)
{
	float TestTransAmount, TestMaxTransAmount;
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: isBelowMaxAmountTest\n");
	printf("Enter Max Amount is : ");
#pragma warning(suppress : 4996) //C4996 Warning
	scanf("%f", &TestMaxTransAmount);
	for (int n = 1; n <= 5; n++)
	{

		printf("Test Case : %d\n", n);
		printf("Input Data: ");
	#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%f", &TestTransAmount);
		if ((TestTransAmount) > (TestMaxTransAmount))
		{
			printf("Expected Result: EXCEED_MAX_AMOUNT\n");
			printf("Actual Result: EXCEED_MAX_AMOUNT \n\n");
		}
		else
		{
			printf("Expected Result: OK_Amount\n");
			printf("Actual Result: %0.4f < %0.04f\n\n", TestTransAmount, TestMaxTransAmount);
		}

	}
}

void setMaxAmountTest(void)
{
	float TestMaxTransAmount;
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: setMaxAmountTest\n");
	for (int n = 1; n <= 5; n++)
	{

		printf("Test Case : %d\n", n);
		printf("Input Data: ");
#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%f", &TestMaxTransAmount);
		if ((TestMaxTransAmount <= 0))
		{
			printf("Expected Result: INVALID_MAX_AMOUNT\n");
			printf("Actual Result: INVALID_MAX_AMOUNT \n\n");
		}
		else
		{
			printf("Expected Result: OK_MAX_AMOUNT\n");
			printf("Actual Result: %0.04f\n\n",  TestMaxTransAmount);
		}
	

	}
}

void isValidCardPANTest(void)
{
	uint8_t TestPrimaryAccountNumber[20];
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: isValidCardPANTest\n");
	for (int n = 1; n <= 5; n++)
	{

		printf("Test Case : %d\n", n);
		printf("Input Data: ");
#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%s", &TestPrimaryAccountNumber);

		//
		uint8_t sumodd = 0, sumeven = 0, sumLuhn = 0, doubleEven = 0;
		/*get sum of odd index*/
		for (char i = strlen(TestPrimaryAccountNumber) - 1; i >= 0; i = i - 2)
		{
			sumodd = sumodd + (TestPrimaryAccountNumber[i] - '0');
		}
		/*get sum of even (index*2)- if sum the double greater than 9 will sum the two numbers*/
		for (char i = strlen(TestPrimaryAccountNumber) - 2; i >= 0; i = i - 2)
		{
			doubleEven = (2 * (TestPrimaryAccountNumber[i] - '0'));
			if (doubleEven > 9)
				doubleEven = (doubleEven - 9);
			sumeven = sumeven + doubleEven;
		}
		sumLuhn = sumodd + sumeven;

		if (sumLuhn % 10 != 0)
		{
			printf("Expected Result: INVALID_CARD\n");
			printf("Actual Result: INVALID_CARD \n\n");
		}
		else
		{
			printf("Expected Result: LUHN is OK\n");
			printf("Actual Result: %s\n\n", TestPrimaryAccountNumber);
		}


	}
}
