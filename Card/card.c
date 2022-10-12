#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "card.h"

uint8_t i = 0;
/*Implement getCardHolderName function
1- This function will ask for the cardholder's name and store it into card data.
2- Cardholder name is 24 alphabetic characters string max and 20 min.
3-If the cardholder name is NULL, less than 20 characters 
or more than 24 will return a WRONG_NAME error, else return CARD_OK..*/

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{

	printf("Please Enter Card Holder Name\n");
	printf("Name must be ( 20 - 24 ) characters\nNumbers are not allowed.\n\nName : ");
	#pragma warning(suppress : 4996) //C4996 Warning
	scanf("%[^\n]%*c", &cardData->cardHolderName);

	//check name validation
	if (((cardData->cardHolderName) == NULL) || strlen(cardData->cardHolderName) < 20 || strlen(cardData->cardHolderName) > 24)
		return WRONG_NAME;

	//no digits validation

	for (i = 0; i <= 23; i++)
	{
		if (isdigit(cardData->cardHolderName[i]))
			return WRONG_NAME;
	}

	return CARD_OK;
}

/*Implement getCardExpiryDate function
1- This function will ask for the card expiry date and store it in card data.
2- Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
3- If the card expiry date is NULL, less or more than 5 characters, 
or has the wrong format will return the WRONG_EXP_DATE error, else return CARD_OK.*/

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{

	printf("Please enter card expiry date: ");
	#pragma warning(suppress : 4996) //C4996 Warning
	scanf("%s", &cardData->cardExpirationDate);

	/*check the length validation*/
	if ((strlen(cardData->cardExpirationDate) < 5) || (strlen(cardData->cardExpirationDate) > 5) || (cardData->cardExpirationDate == NULL))
		return WRONG_EXP_DATE;

	/*check "MM/YY" Format*/
	if (cardData->cardExpirationDate[2] != '/')
		return WRONG_EXP_DATE;


	for (i = 0; i <= 1; i++)
	{
		if (!(isdigit(cardData->cardExpirationDate[i])))
			return WRONG_EXP_DATE;
	}

	for (i = 3; i <= 4; i++)
	{
		if (!(isdigit(cardData->cardExpirationDate[i])))
			return WRONG_EXP_DATE;
	}

	/*check month not greater than 12 and year is less than 2022 */
	if (((cardData->cardExpirationDate[0] - '0') > 1) || ((cardData->cardExpirationDate[0] - '0' == 1) && (cardData->cardExpirationDate[1] - '0' > 2)))
		return WRONG_EXP_DATE;
	return CARD_OK;
}

/*Implement getCardPAN function
1- This function will ask for the card's Primary Account Number and store it in card data.
2- PAN is 20 numeric characters string, 19 character max, and 16 character min.
3- If the PAN is NULL, less than 16 or more than 19 characters,
will return the WRONG_PAN error, else return CARD_OK.
*/
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{/*get PAN*/
	printf("Please Enter the Primary Account Number (PAN)\n");
	#pragma warning(suppress : 4996) //C4996 Warning
	scanf("%s", &cardData->primaryAccountNumber);

	/*Check lenth (16-20) and not NULL*/
	if ((strlen(cardData->primaryAccountNumber) < 16) || (strlen(cardData->primaryAccountNumber) > 19) || (cardData->primaryAccountNumber == NULL))
		return WRONG_PAN;



	return CARD_OK;

	
}



/*Test*/
void getCardHolderNameTest(void)
{
	
	uint8_t TestCardHolderName[30];
	uint8_t copyTestCardHolderName[30];
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: getCardHolderNameTest\n");
		for (int n = 1; n <= 5; n++)
		{

			printf("Test Case : %d\n", n);
			printf("Input Data: ");
			#pragma warning(suppress : 4996) //C4996 Warning
			scanf("%[^\n]%*c", &TestCardHolderName);
			if (((TestCardHolderName) == NULL) || strlen(TestCardHolderName) < 20 || strlen(TestCardHolderName) > 24)
			{
				uint8_t AcopyTestCardHolderName[25] = "WRONG_NAME";
				printf("Expected Result: WRONG_NAME\n");
				printf("Actual Result: %s\n\n", AcopyTestCardHolderName);
			}

			else if (isdigit(TestCardHolderName[i]))
			{
				int16_t z = 0;
			for (int i = 0; i <= 23; i++)
			{
				if (isdigit(TestCardHolderName[i]))
					z = z + i;
			}
			if (z > 0) 
				{
				uint8_t BcopyTestCardHolderName[30] = "WRONG_NAME";
				printf("Expected Result: WRONG_NAME\n");
				printf("Actual Result: %s\n\n", BcopyTestCardHolderName);
				}
			}
			
			else
			{
				#pragma warning(suppress : 4996) //C4996 Warning
				strcpy(copyTestCardHolderName, TestCardHolderName);
				printf("Expected Result: OK Name\n");
				printf("Actual Result: %s\n\n", copyTestCardHolderName);
			}
		}

}

void getCardExpiryDateTest(void)
{
	uint8_t cardExpirationDateTest[6];
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: getCardExpiryDateTest\n");
	for (int n = 1; n <= 5; n++)
	{
		printf("Test Case : %d\n",n);
		printf("Input Data: ");
		#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%s", &cardExpirationDateTest);

		/*check the length validation*/
		if ((strlen(cardExpirationDateTest) < 5) || (strlen(cardExpirationDateTest) > 5) || (cardExpirationDateTest == NULL))
		{
			printf("Expected Result: WRONG_Date\n");
			printf("Actual Result: WRONG_Date \n\n");
		}

		/*check "MM/YY" Format*/
		else if (cardExpirationDateTest[2] != '/')
		{
			printf("Expected Result: WRONG_Date\n");
			printf("Actual Result: WRONG_Date \n\n");
		}

		else if (!(isdigit(cardExpirationDateTest[0])))
		{

			printf("Expected Result: WRONG_Date\n");
			printf("Actual Result: WRONG_Date \n\n");
		}

		else if (!(isdigit(cardExpirationDateTest[1])))
		{

			printf("Expected Result: WRONG_Date\n");
			printf("Actual Result: WRONG_Date \n\n");
		}

		else if (!(isdigit(cardExpirationDateTest[3])))
		{

			printf("Expected Result: WRONG_Date\n");
			printf("Actual Result: WRONG_Date \n\n");
		}

		else if (!(isdigit(cardExpirationDateTest[4])))
		{

			printf("Expected Result: WRONG_Date\n");
			printf("Actual Result: WRONG_Date \n\n");
		}
		
		/*check month not greater than 12 and year is less than 2022 */
		else if (((cardExpirationDateTest[0] - '0') > 1) || ((cardExpirationDateTest[0] - '0' == 1) && (cardExpirationDateTest[1] - '0' > 2)))
		{
			printf("Expected Result: WRONG_Date\n");
			printf("Actual Result: WRONG_Date \n\n");
		}
		else
		{
			printf("Expected Result: OK_Date\n");
			printf("Actual Result: %s\n\n", cardExpirationDateTest);
		}
	}
}


void getCardPANTest(void)
{
	uint8_t TestPrimaryAccountNumber[20];
	printf("Tester Name: Hazem Abdelgalil Alwakil\n");
	printf("Function Name: getCardPANTest\n");
	for (int n = 1; n <= 5; n++)
	{
	
		printf("Test Case : %d\n", n);
		printf("Input Data: ");
	#pragma warning(suppress : 4996) //C4996 Warning
		scanf("%s", &TestPrimaryAccountNumber);
		uint8_t z = 0;
		for (i = 0; i < strlen(TestPrimaryAccountNumber); i++)
		{
			if (!(isdigit(TestPrimaryAccountNumber[i])))
			{

				z = z + i;

			}
		}
			if(z>0)
			{
				printf("Expected Result: WRONG_PAN\n");
				printf("Actual Result: WRONG_PAN \n\n");
			}
			else if ((strlen(TestPrimaryAccountNumber) < 16) || (strlen(TestPrimaryAccountNumber) > 19) || (TestPrimaryAccountNumber == NULL))
			{
				printf("Expected Result: WRONG_PAN\n");
				printf("Actual Result: WRONG_PAN \n\n");
			}
			else
			{
				printf("Expected Result: OK_PAN\n");
				printf("Actual Result: %s\n\n", TestPrimaryAccountNumber);
			}
		
	}
}