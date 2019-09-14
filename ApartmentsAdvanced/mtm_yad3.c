
#include "mtm_ex2.h"
#include "yad3.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>


#define ODD 1
#define VALID_IMPRESSIONS_FLAG 1
#define IDENTICAL_STRINGS 0
#define NUMBER_OF_FLAGS 2
#define APEARS_ONCE 1
#define MAX_NUMBER_COMMANDS 5
#define VALID_CHMARACTER '#'
#define INPUT_FLAG "-i"
#define OUTPUT_FLAG "-o"
#define CHMARACTERS " \n\r\t "

/** Type used for define the system status */
typedef enum {
	CLOSE_SYSTEM,
	CONTINUE_PROCESS
} SystemStatus;


// *************************Static function******************************
// **********************************************************************

static MtmErrorCode conventFromeYad3ErrorCodeToMtmErrorCode
                                        (Yad3ErrorCode error){
	assert(error!=YAD_3_NULL_PARAMETERS);
	switch (error) {
		case YAD_3_INVALID_PARAMETERS:
			return MTM_INVALID_PARAMETERS;
		case YAD_3_EMAIL_ALREADY_EXISTS:
			return MTM_EMAIL_ALREADY_EXISTS;
		case YAD_3_EMAIL_DOES_NOT_EXIST:
			return MTM_EMAIL_DOES_NOT_EXIST;
		case YAD_3_EMAIL_WRONG_ACCOUNT_TYPE:
			return MTM_EMAIL_WRONG_ACCOUNT_TYPE;
		case YAD_3_ALREADY_REQUESTED:
			return MTM_ALREADY_REQUESTED;
		case YAD_3_NOT_REQUESTED:
			return MTM_NOT_REQUESTED;
		case YAD_3_APARTMENT_SERVICE_ALREADY_EXISTS:
			return MTM_APARTMENT_SERVICE_ALREADY_EXISTS;
		case YAD_3_APARTMENT_SERVICE_DOES_NOT_EXIST:
			return MTM_APARTMENT_SERVICE_DOES_NOT_EXIST;
		case YAD_3_APARTMENT_SERVICE_FULL:
			return MTM_APARTMENT_SERVICE_FULL;
		case YAD_3_APARTMENT_ALREADY_EXISTS:
			return MTM_APARTMENT_ALREADY_EXISTS;
		case YAD_3_APARTMENT_DOES_NOT_EXIST:
			return MTM_APARTMENT_DOES_NOT_EXIST;
		case YAD_3_PURCHASE_WRONG_PROPERTIES:
			return MTM_PURCHASE_WRONG_PROPERTIES;
		case YAD_3_REQUEST_WRONG_PROPERTIES:
			return MTM_REQUEST_WRONG_PROPERTIES;
		case YAD_3_REQUEST_ILLOGICAL_PRICE:
			return MTM_REQUEST_ILLOGICAL_PRICE;
		default: assert(error==YAD_3_OUT_OF_MEMORY);
	}
    return MTM_OUT_OF_MEMORY;
}

static char* commandCopyLine(char* str) {
	assert(str);
    char* str_copy = malloc(strlen(str) + 1);
    if (!str_copy) {
        return NULL;
    }
    return strcpy(str_copy, str);
}

// *******Functions are used to check whether a valid command*************
// ***********************************************************************

static int countCommandAppears(char** array, int array_size, char* str){
	int appearances_counter = 0;
	for (int i = 0; i < array_size; i++) {
		if (strcmp(array[i], str) == IDENTICAL_STRINGS) {
			appearances_counter++;
		}
	}
	return appearances_counter;
}

static bool flagsAppearCommandValid(int argc, char** argv, char**flag_array){
	for (int i = 1; i < argc; i += 2) {
		if ((countCommandAppears(flag_array, NUMBER_OF_FLAGS, argv[i]))
				!=APEARS_ONCE) {
			return false;
		}
	}
	return true;
}

static bool locationFlagsLegal(int argc, char** argv, char**flag_array){
	for (int i = 2; i < argc; i += 2) {
		if (countCommandAppears(flag_array, NUMBER_OF_FLAGS, argv[i])){
			return false;

		}
	}
	return true;
}

static bool flagAppearsOnce(int argc, char** argv, char**flag_array){
	for (int i = 0; i < NUMBER_OF_FLAGS; i++) {
		if (countCommandAppears(argv, argc,
				flag_array[i])>VALID_IMPRESSIONS_FLAG) {
			return false;
		}
	}
	return true;
}

static bool lgalityOfCommand(int argc, char** argv){
	char* flag_array[NUMBER_OF_FLAGS] = { INPUT_FLAG, OUTPUT_FLAG };
	if (argc > MAX_NUMBER_COMMANDS || argc % 2 != ODD) {
		return false;
	}else if(argc==VALID_IMPRESSIONS_FLAG){
		return true;
	}else if (!flagsAppearCommandValid(argc, argv, flag_array)
			|| !flagAppearsOnce(argc, argv, flag_array)
			|| !locationFlagsLegal(argc, argv, flag_array)) {
		return false;
	}
	return true;
}

// ********************** Close the system *******************************
// ***********************************************************************

void closeSystem(SystemUser system_users, FILE* input, FILE* output) {
	if (system_users) {
		systemUsersDestroy(system_users);
	}
	if (input && input != stdin) {
		fclose(input);
	}
	if (output && output != stdout) {
		fclose(output);
	}
	return;
}

// ********************** Errors channel *********************************
// ***********************************************************************

SystemStatus errorsChannel(SystemUser system_users, FILE* input,
		FILE* output,MtmErrorCode code) {
	mtmPrintErrorMessage(stderr,code);

	if (code == MTM_OUT_OF_MEMORY
			|| code == MTM_INVALID_COMMAND_LINE_PARAMETERS
			|| code == MTM_CANNOT_OPEN_FILE) {
		closeSystem(system_users, input, output);
		return CLOSE_SYSTEM;
	}
	return CONTINUE_PROCESS;
}

// ********************** Processing orders ******************************
// ***********************************************************************

bool realtorCommand(SystemUser system, char* line_command,
		Yad3ErrorCode* error_code){
	strtok(line_command, CHMARACTERS);
	char* sub_command = strtok(NULL, CHMARACTERS);
	char* email = strtok(NULL, CHMARACTERS);
	if (strcmp(sub_command, "add") == IDENTICAL_STRINGS) {
		char* company_name = strtok(NULL, CHMARACTERS);
		int tax_percentage = atoi(strtok(NULL, CHMARACTERS));
		return systemUserAddRealtor(system,email,
				company_name,tax_percentage,error_code);
	}else if (strcmp(sub_command, "remove") == IDENTICAL_STRINGS) {
		return systemUserRemoveUser(system,email,REALTOR_USER,
				error_code);
	}else if (strcmp(sub_command, "respond_to_offer")
			== IDENTICAL_STRINGS){
		char* customer_email = strtok(NULL, CHMARACTERS);
		char* choice = strtok(NULL, CHMARACTERS);
		return systemUserRealtorRespondToOffer(system,email,
				customer_email,choice,error_code);
	}
	char* service_name = strtok(NULL, CHMARACTERS);
	if (strcmp(sub_command,"add_apartment_service") == IDENTICAL_STRINGS){
		int max_apartments = atoi(strtok(NULL, CHMARACTERS));
		return systemUserAddToRealtorApartmentService(system,email,
				service_name,max_apartments,error_code);
	}else if (strcmp(sub_command, "remove_apartment_service")
			== IDENTICAL_STRINGS){
		return systemUserRemoveApartmentServiceFromeRealtor(system,email,
				service_name,error_code);
	}else if (strcmp(sub_command, "add_apartment") == IDENTICAL_STRINGS){
		int id = atoi(strtok(NULL, CHMARACTERS));
		int price = atoi(strtok(NULL, CHMARACTERS));
		int width = atoi(strtok(NULL, CHMARACTERS));
		int height = atoi(strtok(NULL, CHMARACTERS));
		char* matrix = strtok(NULL, CHMARACTERS);
		return systemUserAddApartmentToRealtor(system,email,
				service_name,id,price,width,height,matrix,error_code);
	}
	assert(strcmp(sub_command, "remove_apartment") == IDENTICAL_STRINGS);
	int id = atoi(strtok(NULL, CHMARACTERS));
	return systemUserRemoveApartmentFromeRealtor(system,email,
			service_name,id,error_code);
}

bool customerCommand(SystemUser system, char* line_command,
		Yad3ErrorCode* error_code) {
	strtok(line_command, CHMARACTERS);
	char* sub_command = strtok(NULL, CHMARACTERS);
	char* email = strtok(NULL, CHMARACTERS);
	if (strcmp(sub_command, "add") == IDENTICAL_STRINGS) {
		int min_area = atoi(strtok(NULL, CHMARACTERS));
		int min_rooms = atoi(strtok(NULL, CHMARACTERS));
		int max_price = atoi(strtok(NULL, CHMARACTERS));
		return systemUserAddCustomer(system,email,min_area,
				min_rooms,max_price,error_code);
	}else if (strcmp(sub_command, "remove") == IDENTICAL_STRINGS) {
		return systemUserRemoveUser(system,email,CUSTOMER_USER,
				error_code);
	}
	char* realtor_email = strtok(NULL, CHMARACTERS);
	char* service_name = strtok(NULL, CHMARACTERS);
	int apartment_id = atoi(strtok(NULL, CHMARACTERS));
	if (strcmp(sub_command, "purchase") == IDENTICAL_STRINGS) {
		return systemUserCustomerPurchase(system,email,realtor_email,
				service_name,apartment_id,error_code);
	}
	assert(strcmp(sub_command, "make_offer") == IDENTICAL_STRINGS);
	int new_price = atoi(strtok(NULL, CHMARACTERS));
	return systemUserCustomerMakeOffer(system,email,
			realtor_email,service_name,apartment_id,
			new_price,error_code);
}

bool reportCommand(SystemUser system, char* line_command, FILE* output,
		Yad3ErrorCode* error_code) {
	strtok(line_command, CHMARACTERS);
	char* sub_command = strtok(NULL, CHMARACTERS);
	if (strcmp(sub_command, "relevant_realtors") == IDENTICAL_STRINGS) {
		char* customer_email = strtok(NULL, CHMARACTERS);
		return systemUseReportRelevantRealtors(system,customer_email,
				output,error_code);
	}
	int count = atoi(strtok(NULL, CHMARACTERS));
	if (strcmp(sub_command, "most_paying_customers")
			== IDENTICAL_STRINGS) {
		return systemUserReportMostPayingCustomers(system,count,
				output,error_code);
	}
	assert(strcmp(sub_command, "significant_realtors")
			== IDENTICAL_STRINGS);
	return systemUserReportSignificantRealtors(system,count,
			output,error_code);
}

bool routingCommandsSuccess(SystemUser system_users, char* line_command,
		FILE* input, FILE* output, MtmErrorCode* mtm_error){
	char* copy_line_command = commandCopyLine(line_command);
	if (copy_line_command == NULL) {
		*mtm_error = MTM_OUT_OF_MEMORY;
		return false;
	}
	Yad3ErrorCode error_code;
	bool outcome=true;
	char* command = strtok(copy_line_command, CHMARACTERS);
	if (strcmp(command, "realtor") == IDENTICAL_STRINGS) {
		outcome=realtorCommand(system_users, line_command,
				&error_code);
	}else if (strcmp(command, "customer") == IDENTICAL_STRINGS) {
		outcome=customerCommand(system_users, line_command,
				&error_code);
	}else if(strcmp(command, "report") == IDENTICAL_STRINGS){
	  outcome=reportCommand(system_users,
			  line_command,output,&error_code);
	}
	free(copy_line_command);
	if(!outcome){
		*mtm_error=conventFromeYad3ErrorCodeToMtmErrorCode(error_code);
	}
	return outcome;
}

void processingCommandOfSystemUsers(SystemUser system_users, FILE* input,
		FILE* output){
	char line_command[MAX_LEN + 1];
	while (fgets(line_command, MAX_LEN + 1, input)) {
		char* copy_line_command = commandCopyLine(line_command);
		if (copy_line_command == NULL) {
			errorsChannel(system_users, input, output, MTM_OUT_OF_MEMORY);
			return;
		}
		char* command = strtok(copy_line_command, CHMARACTERS);
		if (!command || *command == VALID_CHMARACTER) {
			free(copy_line_command);
			continue;
		}
		free(copy_line_command);
		MtmErrorCode error_code;
		if (!routingCommandsSuccess(system_users,line_command,input,output,
				&error_code)) {
			SystemStatus system_status = errorsChannel(system_users, input,
					output, error_code);
			if (system_status == CLOSE_SYSTEM) {
				return;
			}
		}
	}
}

// ********************** Running the program ****************************
// ***********************************************************************

FILE* InputFile(int argc, char** argv){
	for (int i = 1; i < argc; i += 2) {
		if (strcmp(argv[i], INPUT_FLAG) == 0) {
			return fopen(argv[i + 1], "r");
		}
	}
	return stdin;
}

FILE* OutputFile(int argc, char** argv){
	for (int i = 1; i < argc; i += 2) {
		if (strcmp(argv[i], OUTPUT_FLAG) == 0) {
			return fopen(argv[i + 1], "w");
		}
	}
	return stdout;
}

int main(int argc, char** argv){
	if (!(lgalityOfCommand(argc, argv))) {
		errorsChannel(NULL,NULL,NULL,MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return 0;
	}
	FILE* input = InputFile(argc, argv);
	if (!input) {
		errorsChannel(NULL, NULL, NULL, MTM_CANNOT_OPEN_FILE);
		return 0;
	}
	FILE* output = OutputFile(argc, argv);
	if (!input) {
		errorsChannel(NULL, input, NULL, MTM_CANNOT_OPEN_FILE);
		return 0;
	}
	SystemUser system_users = systemUsersCreate();
	if (!system_users) {
		errorsChannel(NULL, input, NULL, MTM_OUT_OF_MEMORY);
		return 0;
	}
	processingCommandOfSystemUsers(system_users, input, output);
	closeSystem(system_users, input, output);
	return 0;
}
