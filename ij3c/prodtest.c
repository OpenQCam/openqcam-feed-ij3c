#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MAX_QUERY_TOKEN_COUNT 6
#define RW_BLOCK_SIZE 1024

void endWithErrorMsg(char* msg){
    printf("\"success\":false,");
    printf("\"reason\":\"%s\"}\n", msg);
    exit(1);
}

void readFile(FILE* fp, char* out){
    char buff[4 * RW_BLOCK_SIZE];
    while(fgets(buff, sizeof(buff), fp))
        strcat(out, buff);
}

void readErrorMsg(char* error, char* out){
    int i = 0;
    error = error + 8;
    while(*error != '"'){
        out[i++] = *error;
        error += 1;
    }
    out[i] = '\0';
}

void doSystemCall(char* cmd, char* res){
    char temp[512];

    FILE* fp;
    fp = popen(cmd, "r");
    if(fp == NULL){
        sprintf(temp, "Failed to do command: %s", cmd);
        endWithErrorMsg(temp);
    }

    readFile(fp, res);
    int code = WEXITSTATUS(pclose(fp));

    //Chech if it's any specified command error.
    if(code == 1){
        sprintf(temp, "Invalid command usage: %s", cmd);
        endWithErrorMsg(temp);
    }
    else if(code == 127){
        sprintf(temp, "Command not found: %s", cmd);
        endWithErrorMsg(temp);
    }

    //Remove the trailing new line.
    char* nl;
    if((nl = strrchr(res, '\n')) != NULL)
        *nl = '\0';

    //Check if the returning result contains an error.
    char* pch = strstr(res, "error");
    if(pch != NULL){
        char errorMsg[RW_BLOCK_SIZE];
        readErrorMsg(pch, errorMsg);
        endWithErrorMsg(errorMsg);
    }
}

void doProtectedSystemCall(char* cmd, char* res){
    char* ptr;

    /* make sure no invalid characters */
    ptr = cmd;
    while(*ptr)
    {
        char c = *ptr;
        printf("%c", c);
        if(c >= 'A' && c <= 'Z')
        {
            // pass
        }else if(c >= 'a' && c <= 'z')
        {
            // pass
        }else if(c >= '0' && c <= '9')
        {
            // pass
        }else if(c == '_' || c == ' ')
        {
            // pass
        }else if(c == '.')
        {
            if(strncmp(ptr, "./", 2) == 0)
            {
                ptr++;
            }else if(strncmp(ptr, ".sh", 3) == 0)
            {
                ptr+=2;
            }else{
                endWithErrorMsg("Found prohibited characters after '.'\n");
            }
        }else
        {
            endWithErrorMsg("Found prohibited characters.\n");
        }
        ptr++;
    }

    doSystemCall(cmd, res);
}

char *strcpy_s(char *dest, int max, const char *src)
{
    if(strlen(src) > max)
        endWithErrorMsg("Fatal - buffer overflow!\n");

    return strcpy(dest, src);
}

int main(int argc, char* args[]){

    //Output HTTP header.
    printf(
	"\
	Server: %s\n\
	Pragma: no-cache\n\
	Content-type: application/json\n",
	getenv("SERVER_SOFTWARE"));
	printf("\n{");

    //Retrieve query string from REQUEST_URI.
    char* reqUri;
    /*
    reqUri = args[1];//FOR DEBUG.
    /*/
    reqUri = (char*)getenv("REQUEST_URI");
    //*/
    if(reqUri == NULL){
        endWithErrorMsg("Fatal - REQUEST_URI not found!");
    }
    char* questionMark = strchr(reqUri, '?');
    char queryString[512];
    strcpy_s(queryString, 512, questionMark + 1);
    printf("\"queryString\":\"%s\",", queryString);

    //Split query string into tokens.
    /*
        Query string of every 'prodtest.cgi' request must come in this form:
        cmd={command}[&p1={param1}][&p2={param2}][&p3={param3}]...
        The number of 'p1, p2, ...' is optional. However, 'cmd', as the first
        query argument, is mandatory.
    */
    char tokens[MAX_QUERY_TOKEN_COUNT][64];
    char* pch;
    pch = strtok(queryString, "&");
    int count = 0;
    while(pch != NULL){
        // check number of parameters
        if(count >= MAX_QUERY_TOKEN_COUNT)
            endWithErrorMsg("Fatal - too many parameters.");
        strcpy_s(tokens[count++], 64, pch);
        pch = strtok(NULL, "&");
    }

    //Examine whether the query string contains "cmd".
    if(strcmp("cmd", strtok(tokens[0], "=")) != 0)
        endWithErrorMsg("You must specify \"cmd\" as the first parameter.");

    //Resolve command "cmd".
    char cmd[64];
    strcpy_s(cmd, 64, tokens[0] + 4);//"cmd=???"
    if(cmd == NULL || strcmp(cmd, "") == 0)
        endWithErrorMsg("\"cmd\" is NULL or empty.");

    //Resolve parameter tokens.
    char params[MAX_QUERY_TOKEN_COUNT - 1][64];
    int i;
    for(i=1; i<count; i++){
        //Examine whether the parameters is correctly given.
        char temp[2];
        sprintf(temp, "p%d", i);
        if(strcmp(temp, strtok(tokens[i], "=")) != 0){
            endWithErrorMsg(
                "Parameters are not correctly given. (Give p1, p2, p3 etc.)"
            );
        }
        //Parse it.
        strcpy_s(params[i - 1], 64, tokens[i] + 3);
    }

    //Assemble the final command.
    char final[RW_BLOCK_SIZE];
    strcpy_s(final, RW_BLOCK_SIZE, "./");   // don't search system path
    strcat(final, cmd);
    for(i=1; i<count; i++){
        strcat(final, " ");
        strcat(final, params[i - 1]);
    }
    printf("\"command\":\"%s\",", final);

    //Execute the command and get the result.
    char res[4 * RW_BLOCK_SIZE];
    doProtectedSystemCall(final, res);
    printf("\"result\":%s,", res);

    //If success, output the ending sequence.
    printf("\"success\":true");
    printf("}\n");

    return 0;
}
