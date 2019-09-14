
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "orange.h"
#include <assert.h>
#include <string.h>

bool input_check(int sellingPrice, int size, int expirationMonth, int maxNumberOfFoodCompanies);

//func 1
Orange orangeCreate(short size, Month expirationMonth, int maxNumberOfFoodCompanies,
    int sellingPrice)
{
    if (input_check(sellingPrice, size, expirationMonth, maxNumberOfFoodCompanies)==1)
        return NULL;
    Orange orange;
    if ((orange = (Orange)malloc(sizeof(struct orange_t))) == NULL)
        return NULL;

    orange->sellingPrice = sellingPrice;
    orange->size = size;
    orange->expirationMonth = expirationMonth;
    orange->maxNumberOfFoodCompanies = maxNumberOfFoodCompanies;
    if((orange->foodCompanies = (char**) malloc ((sizeof(char*)*maxNumberOfFoodCompanies)))==NULL)
    {
    	free(orange);
    	return NULL;
    }
    for(int i = 0; i < orange->maxNumberOfFoodCompanies; i++)
        orange->foodCompanies[i] = NULL;
    return orange;
}

bool input_check(int sellingPrice, int size, int expirationMonth, int maxNumberOfFoodCompanies)
{
    return ((size>ORANGE_MAX_SIZE || size<1) || (expirationMonth>11 || expirationMonth<0)||
        (sellingPrice<0) || (maxNumberOfFoodCompanies<0));
}
//func 2

Orange orangeCopy(const Orange src)
{
    if(src == NULL){
     return NULL;
    }
    Orange orange=orangeCreate(src->size,src->expirationMonth,src->maxNumberOfFoodCompanies,src->sellingPrice);
    if(orange == NULL)
        return NULL;
    for(int i=0; i < src->maxNumberOfFoodCompanies &&  (src->foodCompanies[i]!=NULL)  ;i++){
        if(orangeAddFoodCompany(orange,src->foodCompanies[i])== ORANGE_OUT_OF_MEMORY){
        	orangeDestroy(orange);
        		return NULL;
        	}
    }
    return orange;
}

//func 3
int orangeCompareBySize(const Orange orange1, const Orange orange2)
{
    assert (orange1!=NULL || orange2!=NULL);
    return ((orange1->size)-(orange2->size));
}
//func 4
OrangeResult orangeAddFoodCompany(Orange orange,const char* foodCompany)
{
    if(!(orange && foodCompany)) return ORANGE_ILLEGAL_ARGUMENT;
    for(int i_check = 0; i_check < orange->maxNumberOfFoodCompanies; i_check++)
    {
        if(orange->foodCompanies[i_check])
        {
            if(!strcmp(orange->foodCompanies[i_check],foodCompany))
                return ORANGE_COMPANY_ALREADY_EXISTS;
        }
        else
        {
        	orange->foodCompanies[i_check] = (char*)malloc((sizeof(char))*(strlen(foodCompany)+1));
        	if(orange->foodCompanies[i_check]==NULL)
        		return ORANGE_OUT_OF_MEMORY;

        	    strcpy(orange->foodCompanies[i_check],foodCompany);
        	   // printf("%s\n",orange->foodCompanies[i_check]);
        	    return ORANGE_SUCCESS;
        }
    }
    return ORANGE_NO_PLACE_FOR_COMPANY;
}

//func 5
OrangeResult orangeRemoveFoodCompany(Orange orange,const char* foodCompany) {
    if(!(orange && foodCompany)) return ORANGE_ILLEGAL_ARGUMENT;
    char *tmp = NULL;
    for(int i_check = 0; i_check < orange->maxNumberOfFoodCompanies; i_check++){
            if(!(orange->foodCompanies[i_check]))
                break;
            else if(!(strcmp(orange->foodCompanies[i_check],foodCompany)))
            {
                //AZAZA func
                tmp = orange->foodCompanies[i_check];
                int j = i_check;
                for(; j < orange->maxNumberOfFoodCompanies-1; j++) {
                    orange->foodCompanies[j] = orange->foodCompanies[j+1];
                }
               orange->foodCompanies[j] = NULL;
               free (tmp);
             //  printf("%s",*tmp);
               return ORANGE_SUCCESS;
            }
        }
 return ORANGE_COMPANY_DOESNT_EXISTS;
}

//func 6
OrangeResult orangeSetFoodCompanyForDelivery(Orange orange, const char* foodCompany)
{
	if(!(orange && foodCompany)) return ORANGE_ILLEGAL_ARGUMENT;

	int comp_ind_in_arr = (orange->maxNumberOfFoodCompanies)-1;
	int i_check;
	for(i_check = 0; i_check < orange->maxNumberOfFoodCompanies; i_check++)
	{
		if(((orange->foodCompanies[i_check])==NULL)|| (!strcmp(orange->foodCompanies[i_check],foodCompany)))
		{
			comp_ind_in_arr = i_check;
			break;
		}
	}
	free(orange->foodCompanies[comp_ind_in_arr]);
	for(i_check =comp_ind_in_arr; i_check > 0; i_check--)
	{
		orange->foodCompanies[i_check] = orange->foodCompanies[i_check-1];
	}
	if((orange->foodCompanies[0]=(char*)malloc(sizeof(char*)*(strlen(foodCompany)+1)))==NULL)
		return ORANGE_OUT_OF_MEMORY;
	strcpy(orange->foodCompanies[0],foodCompany);
	return ORANGE_SUCCESS;
}

//func 7
unsigned int orangeGetPrice(const Orange orange)
{
	assert(orange!=NULL);
	return (orange->sellingPrice);
}

//func 8
unsigned int orangeGetSize(const Orange orange)
{
	assert(orange!=NULL);
	return (orange->size);
}

//func 9
Month orangeGetExpirationMonth(const Orange orange)
{
	assert(orange!=NULL);
	return (orange->expirationMonth);
}

//func 10
char* const * orangeGetFoodCompanies(const Orange orange)
{
	assert(orange!=NULL);
	return (orange->foodCompanies);
}

//func 11
unsigned int orangeGetNumberOfFoodCompanies(const Orange org)
{
	assert(org!=NULL);
	int counter=0;
	while((counter < (org->maxNumberOfFoodCompanies)) && (org->foodCompanies[counter]))
	{
		counter++;
	}
	return counter;
}

//func 12
void orangeDestroy(Orange orange)
{
	if(orange==NULL)return;
	for(int i=0 ; i<orange->maxNumberOfFoodCompanies; i++)
	{
		if((orange->foodCompanies[i])==NULL) continue;
		free(orange->foodCompanies[i]);
	}
		free(orange->foodCompanies);
	free(orange);
}

/*
int main()
{
    Orange orange1 = orangeCreate(50,8,6,7);
    Orange orange2 = orangeCopy(orange1);
   // orange2->size=15;
    printf("%d\n",orangeCompareBySize(orange1,orange2));

    printf("%d\n", orange2->size);
    orangeAddFoodCompany(orange1,"OrangeBest");
   orangeAddFoodCompany(orange1,"yanti1");
    orangeAddFoodCompany(orange1,"yanti2");
    orangeAddFoodCompany(orange1,"yanti3");
   // orangeAddFoodCompany(orange1,"yanti4");
   // orangeAddFoodCompany(orange1,"yanti5");
    for(int i=0;i<6;i++)
     {
     	printf ("%s\n",orange1->foodCompanies[i]);
     }
    printf("%d\n",orangeSetFoodCompanyForDelivery(orange1,"yanti2"));
    for(int i=0;i<6;i++)
    {
    	printf ("%s\n",orange1->foodCompanies[i]);
    }

    printf("%d\n",orangeGetPrice(orange1));
    printf("%d\n",orangeGetSize(orange1));
    printf("%d\n",orangeGetExpirationMonth(orange1));
    printf("%d\n",orangeGetNumberOfFoodCompanies(orange1));
    printf("%s\n",orange1->foodCompanies[5]);
    Orange orange3 = orangeCopy(orange1);
    for(int i=0;i<6;i++)
        {
        	printf ("%s\n",orange3->foodCompanies[i]);
        }
    printf("%s$$$\n",orange3->foodCompanies[4]);
    orangeRemoveFoodCompany(orange3,"yanti2");
    printf("\n");
    for(int i=0;i<6;i++)
            {
            	printf ("%s\n",orange3->foodCompanies[i]);
            }
    printf("\n");
    printf("%s$$$\n",orange3->foodCompanies[4]);
    orangeAddFoodCompany(orange3,"yantiaaaaaaaaaaa");
    for(int i=0;i<6;i++)
            {
            	printf ("%s\n",orange3->foodCompanies[i]);
            }



return 0;
}
*/
