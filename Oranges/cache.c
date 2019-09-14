#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cache.h"
#include "list.h"
#include <assert.h>

int calc_weight_curr(List curCompList);
int doesExist(char **allCompanies, int length, char* company);
int getSizeOfAll(Cache cache);
int insertCompany(char **allCompanies, int *index, char *company);
int getAllCompanies(Cache cache, char ***companies);
int cacheGetAmountOfExistingSizes(Cache cache);
Orange iter(Cache cache, int counter);

//func 1
Cache cacheCreate()
{
    Cache cache;
    if ((cache = (Cache)malloc(sizeof(struct cache_t))) == NULL){
            return NULL;
    }
    cache->cache_size=ORANGE_CACHE_SIZE;

    if ((cache->container = (List*)malloc((sizeof(struct List_t*))*(cache->cache_size))) == NULL){
           free(cache);
           return NULL;
       }

       cache->iterator=NULL;
       for(int i=0 ; i<cache->cache_size ; i++){
           cache->container[i] = listCreate();
           if((cache->container[i]==NULL)){
               for(int j=0;j<i;j++){
                    listDestroy(cache->container[j]);
               }
               free(cache->container);
                if(cache!=NULL){
               free(cache);
                }
               return NULL;
            }

       }
       return cache;
}


Cache cacheCopy(Cache src)//להרוס את כל הרשימות שבקאש קרייט יצר כי הקופי ליסט גם עושה אלוקייט
{
     if(src == NULL){
          return NULL;
     }
     Cache cache = cacheCreate();
     for(int i = 0; i < cache->cache_size; i++){
         listDestroy(cache->container[i]);
         cache->container[i] = listCopy(src->container[i]);
         if(cache->container[i]==NULL){
            cacheDestroy(cache);
                return NULL;
         }
     }
     cache->iterator=NULL;
     return cache;
}

//func 3
CacheResult cachePush(Cache cache, Orange orange)
{
    if((cache == NULL) || (orange==NULL))
        return CACHE_ILLEGAL_ARGUMENT;
    if((orange->size)>(cache->cache_size))
        return CACHE_OUT_OF_RANGE;
    Orange orange_cpy=orangeCopy(orange);
    if(orange_cpy==NULL)
        return CACHE_OUT_OF_MEMORY;
    listInsertFirst((cache->container[(orange->size-1)]),orange_cpy);
    orangeDestroy(orange_cpy);
    return CACHE_SUCCESS;
}

//func 4
CacheResult cacheFreeOrange(Cache cache, int index)
{
    if(cache==NULL){
        return CACHE_ILLEGAL_ARGUMENT;
    }
    if((index<1) || (index>cache->cache_size)){
        return CACHE_OUT_OF_RANGE;
    }
    if(cache->container[index-1]==NULL){
        return CACHE_OUT_OF_MEMORY;
    }
    if((listGetFirst(cache->container[index-1]))==NULL){
        return CACHE_NO_ELEMENTS_IN_CELL;
    }
    listRemoveCurrent(cache->container[index-1]);
    if((listGetFirst(cache->container[index-1]))==NULL){
        return CACHE_SUCCESS_LIST_EMPTY;
    }

    return CACHE_SUCCESS;
}

//func 5
CacheResult cacheGet(Cache cache, int index, Orange* org)
{
    if(cache==NULL){
        return CACHE_ILLEGAL_ARGUMENT;
    }
    if((index<1) || (index>cache->cache_size)){
        return CACHE_OUT_OF_RANGE;
    }
    if((listGetFirst(cache->container[index-1]))==NULL){
        return CACHE_NO_ELEMENTS_IN_CELL;
        }
    Orange first = listGetFirst(cache->container[index-1]);
    Orange org_cpy = orangeCopy(first);
    if(org_cpy==NULL){
        return CACHE_OUT_OF_MEMORY;
    }
    *org=org_cpy;
    cacheFreeOrange(cache,index);
    return CACHE_SUCCESS;
}

//func 6
List cacheGetFirst(Cache cache)
{
    assert(cache!=NULL);
    if(cache->container[0]==NULL){
         return NULL;
    }
    cache->iterator=cache->container[0];
    return (cache->iterator);
}

//func 7
List cacheGetNext(Cache cache)
{
    assert(cache!=NULL);
    if(((cache->iterator) == NULL) || ((cache->iterator)== (cache->container[cache->cache_size-1]))){
        cache->iterator = NULL;
        return NULL;
    }
    int count=0;
    while(((cache->iterator)!=cache->container[count]) && (count<(cache->cache_size))){
        count++;
    }
    if(count==cache->cache_size-1){
        cache->iterator=NULL;
        return NULL;
    }
    cache->iterator=cache->container[count+1];
    return (cache->iterator);
}

//func 8
List cacheGetCurrent(Cache cache)
{
    if(cache==NULL){
         return NULL;
    }
    if((cache->iterator) == NULL){
        return NULL;
    }

    return (cache->iterator);
}

//func 9
void cacheDestroy(Cache cache)
{
    if(!cache) {
        return;
        }
    for( int i = 0 ;i < cache->cache_size ; i++){
        if(cache->container[i]==NULL) {
            continue;
            }
        listDestroy(cache->container[i]);
    }
    free(cache->container);
    cache->iterator=NULL;
    free(cache);
}

//func 10
List cacheGetOrangesOfCompanyFromCell(Cache cache, char* company,int index) {
    if(!(cache && company && index >=1 && index <=256)) {
        return NULL;
    }
    List belongToCompany = listCreate();
       if(!(belongToCompany)){
        return NULL;
    }
    int counter=0;
    bool first=1,last=0,ln=0;
    cache->iterator=cache->container[index-1];
    int size_list=listGetSize(cache->iterator);
    Orange orange = listGetFirst(cache->iterator);

    for(int cur_org=0  ; cur_org<size_list ; cur_org++){//tapuzim
        ln=0;
        if(cur_org==size_list-1){
            last=1;
        }
        if(orange==NULL){
            break;
        }
        if(orange->expirationMonth < CURRENT_MONTH )  {
            if(last){
                if(!listGetFirst(belongToCompany)){
                                return NULL;
                }
                 return belongToCompany;
            }
            orange = listGetNext(cache->iterator);
            counter++;
            continue;
        }
        if(!orangeGetNumberOfFoodCompanies(orange)){
                        if(last){
                            if(!listGetFirst(belongToCompany)){
                                listDestroy(belongToCompany);
                                            return NULL;
                            }
                             return belongToCompany;
                        }
                        orange = listGetNext(cache->iterator);
                        counter++;
                        continue;
        }
        int cur_size_comp=orangeGetNumberOfFoodCompanies(orange);
        for(int curr_company=0 ; curr_company<cur_size_comp ; curr_company++){
            if(!strcmp(orange->foodCompanies[curr_company],company)){
                if(first){
                    orangeSetFoodCompanyForDelivery(orange,company);//
                    listInsertFirst(belongToCompany,orange);
                    listGetFirst(belongToCompany);
                    first=0;
                }
                else{
                    orangeSetFoodCompanyForDelivery(orange,company);//
                    listInsertAfterCurrent(belongToCompany,orange);
                    listGetNext(belongToCompany);
                }
                    listRemoveCurrent(cache->iterator);//
                    orange=iter(cache,counter);//
                    ln=1;
                    break;
             }//if found company
            }//companies
        if(last){
            if(!listGetFirst(belongToCompany)){
                listDestroy(belongToCompany);
                            return NULL;
            }
             return belongToCompany;
        }
        if(!ln){
        orange=listGetNext(cache->iterator);
        counter++;
        }
      }//oranges
    if(!listGetFirst(belongToCompany)){
        listDestroy(belongToCompany);
                    return NULL;
    }
     return belongToCompany;
}//end

Orange iter(Cache cache, int counter)
{
    Orange orange=listGetFirst(cache->iterator);
    while(counter){
        orange=listGetNext(cache->iterator);
        if(orange==NULL){
            return NULL;
        }
        counter--;
    }
    return orange;
}

//func 11
List cacheGetAllOrangesOfCompany(Cache cache, char* company) {
    Cache allWeights = cacheCreate();
    if(allWeights==NULL){
        return NULL;
    }
    List orangesOfCompany = listCreate();
    if(orangesOfCompany==NULL){
        cacheDestroy(allWeights);
            return NULL;
    }
    Orange cur_org = NULL;
    List curWeight = NULL;
    bool first = 1;
    int i,j;
    for(i = 0 , cacheGetFirst(cache) ;cache->iterator && i < cache->cache_size   ; i++ , cacheGetNext(cache) ){
        listDestroy(allWeights->container[i]);
        allWeights->container[i] = cacheGetOrangesOfCompanyFromCell(cache,company, i+1);
    }
    curWeight = allWeights->container[0];
    for(i = 0 ; i < cache->cache_size  ; i++  ){
        if((curWeight = allWeights->container[i])==NULL) {
            continue;
        }
        cur_org = listGetFirst(curWeight);
        for(j = 1 ;  (cur_org != NULL) && j <= listGetSize(curWeight) ; j++, cur_org = listGetNext(curWeight)) {
                        if(first) {
                            listInsertFirst(orangesOfCompany, cur_org);
                            first = 0;
                            listGetFirst(orangesOfCompany);
                        }
                        else {
                            listInsertAfterCurrent(orangesOfCompany, cur_org);
                            listGetNext(orangesOfCompany);
                             }
        }
     }
    cacheDestroy(allWeights);
    if(orangesOfCompany==NULL){
        return NULL;
    }
    return orangesOfCompany;
}

//func 12
List cacheDeliverOrangesWithBiggestWeight(Cache cache)
{
    assert(cache != NULL);
    Cache orangesByCompanies  = cacheCreate();
    if(orangesByCompanies==NULL){
        return NULL;
    }
    char **companies = NULL;
    int numOfCompanies = getAllCompanies(cache, &companies);
    if(numOfCompanies==-1){
        cacheDestroy(orangesByCompanies);
        return NULL;
    }
    orangesByCompanies->cache_size = numOfCompanies;
    int i;
    List curList = NULL;
    int max_weight = 1, cur_weight = 0;
    char *max_comp = NULL;
    int maxList;
    int existing_sizes = cacheGetAmountOfExistingSizes(cache);
    if(!existing_sizes){
        cacheDestroy(orangesByCompanies);
        return NULL;
    }
    for(int j=numOfCompanies ; j< 256 ; j++){
        listDestroy(orangesByCompanies->container[j]);
    }
    for(i = 0; i < numOfCompanies; i++){
        Cache cacheCpy;
        cacheCpy = cacheCopy(cache);
        if(cacheCpy==NULL){
            cacheDestroy(orangesByCompanies);
            return NULL;
        }
        listDestroy(orangesByCompanies->container[i]);
        orangesByCompanies->container[i] =  cacheGetAllOrangesOfCompany(cacheCpy, companies[i]);
        if(orangesByCompanies->container[i]==NULL){
            cacheDestroy(orangesByCompanies);
        }
        cacheDestroy(cacheCpy);
    }
    for(i = 0 , curList = cacheGetFirst(orangesByCompanies)
        ;orangesByCompanies->iterator && i < orangesByCompanies->cache_size
        ; i++ ,curList = cacheGetNext(orangesByCompanies)){
        cur_weight = calc_weight_curr(curList);
        if(cur_weight >  max_weight)
        {
            max_weight = cur_weight;
            max_comp = companies[i];
            maxList = i;
        }
        else if (cur_weight == max_weight)
        {
            if(!max_comp)
            {
                max_comp = companies[i];
                maxList = i;
            }
            else
                if(strcmp(companies[i],max_comp)>0)

                {
                    max_comp = companies[i];
                    maxList = i;
                }
        }
    }
    List final_list=listCopy(orangesByCompanies->container[maxList]);
    cacheDestroy(orangesByCompanies);
return final_list;

}


int getAllCompanies(Cache cache, char ***companies)
{
    assert(cache!=NULL);
    int size = getSizeOfAll(cache);
    if(size == 0){
    return -1;}
    int i,j, index = 0;
    Orange cur_org = NULL;
    char **allCompanies = (char**) malloc (sizeof(char*)*(size));
    if(allCompanies==NULL){
    return -1;
    }
    for(i = 0; i < size; i++){
        allCompanies[i] = NULL;
    }
    for(i = 0 , cacheGetFirst(cache) ;cache->iterator &&
    i < cache->cache_size  ; i++, cacheGetNext(cache)){
            int size_curr = listGetSize(cache->iterator);
            if(size_curr == 0){
                continue;
            }
            for(j = 1 , cur_org = listGetFirst(cache->iterator);
            j <= size_curr ; j++, cur_org = listGetNext(cache->iterator)){
                if(!cur_org){
                     break;
                }
                int curNumberOfCompanies = orangeGetNumberOfFoodCompanies(cur_org);
                if(!curNumberOfCompanies){
                     continue;
                }
                char * const* curCompaniesOfOrange = orangeGetFoodCompanies(cur_org);
                for(int k = 0; k < curNumberOfCompanies; k++){
                    char *cur_company = curCompaniesOfOrange[k];
                    if(!doesExist(allCompanies, index, cur_company)){
                        if(insertCompany(allCompanies, &index, cur_company)==-1){
                            free(allCompanies);
                            return -1;
                        }
                    }
                }
            }
    }
    char **allCompaniesToReturn = (char**) malloc (sizeof(char*)*(index));
    if(allCompaniesToReturn==NULL){
        free(allCompanies);
        return -1;
    }
        for(int l = 0; l < index; l++){
            allCompaniesToReturn[l] = (char*) malloc (sizeof(char)*((strlen(allCompanies[l]))+1));
            if(allCompaniesToReturn[l]==NULL){
                free(allCompaniesToReturn);
                free(allCompanies);
                return -1;
            }
            strcpy(allCompaniesToReturn[l],allCompanies[l]);
            free(allCompanies[l]);
        }
    free(allCompanies);
*companies = allCompaniesToReturn;
return index;
}

int insertCompany(char **allCompanies, int *index, char *company)
{
    assert(allCompanies!=NULL);
    assert(company != NULL);
    char *new_company = (char*) malloc (sizeof(char)*((strlen(company))+1));
    if(new_company==NULL){
         return -1;
    }
    strcpy(new_company,company);
    allCompanies[*index] = new_company;
    (*index)++;
    return 0;
}

int doesExist(char **allCompanies, int cur_length, char* company)
{
    assert(allCompanies != NULL);
    assert(company != NULL);
    for(int i = 0; i <= cur_length; i++){
        if(!allCompanies[i]){
             continue;
        }
        if(!strcmp(allCompanies[i],company)){
            return 1;
        }
    }
    return 0;
}



int getSizeOfAll(Cache cache)
{
    assert(cache  !=NULL);
    int i,j;
    int sizeSum = 0;
    Orange cur_org = NULL;
    for(i = 0 , cacheGetFirst(cache) ;
    i < cache->cache_size && cache->iterator ; i++ , cacheGetNext(cache)){
        assert(cache->iterator);
        int size_curr = listGetSize(cache->iterator);
        if(size_curr == 0){ continue;
        }
        for(j = 1 , cur_org = listGetFirst(cache->iterator);
        j <= size_curr && (cur_org != NULL) ; j++, cur_org = listGetNext(cache->iterator)){
                sizeSum += orangeGetNumberOfFoodCompanies(cur_org);
            }
    }
    return sizeSum;
}

int cacheGetAmountOfExistingSizes(Cache cache)
{
    int i;
    int count = 0;
    for(i = 0 , cacheGetFirst(cache) ; i < cache->cache_size
    && cache->iterator ; i++ , cacheGetNext(cache)){
            assert(cache->iterator);
            int size_curr = listGetSize(cache->iterator);
            if(size_curr >= 1){
                count++;
            }
    }
    return count;

}

int calc_weight_curr(List curCompList)
{
    int sum=0;
    Orange current_orange=listGetFirst(curCompList);
    for(int i=0 ; i<listGetSize(curCompList) && current_orange!=NULL ;
    i++ , current_orange = listGetNext(curCompList)){
        sum+=(current_orange->size);
    }

    return sum;
}
