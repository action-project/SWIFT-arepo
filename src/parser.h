/*******************************************************************************
 * This file is part of SWIFT.
 * Copyright (c) 2012 Pedro Gonnet (pedro.gonnet@durham.ac.uk)
 *               2015 Matthieu Schaller (matthieu.schaller@durham.ac.uk)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/
#ifndef SWIFT_PARSER_H
#define SWIFT_PARSER_H

/* Needs to be included so that strtok returns char * instead of a int *. */
#include <string.h>

#define MAX_LINE_SIZE 128
#define MAX_NO_OF_PARAMS 4

struct parameter {
    char name [MAX_LINE_SIZE];
    char value [MAX_LINE_SIZE];
};

struct swift_params {
    struct parameter data [MAX_NO_OF_PARAMS];
};

void printParameters(struct swift_params *params);

void getParam(struct swift_params *params, char * name, int * retParam);

void parseFile(struct swift_params *params, const char *file_name) {
 
    FILE *fp;
    char line[MAX_LINE_SIZE];
    int param_count = 0;

    /* Open file for reading */
    fp = fopen(file_name, "r");
    
    if(fp == NULL) {
      error("Error opening parameter file: %s",file_name);
    }
  
    /* Read until the end of the file is reached.*/
    while(!feof(fp)) {
      
      /* Read a line of the file */
      if(fgets(line,MAX_LINE_SIZE,fp)!=NULL) {
        
        /* Check if the line contains a value */
        if(strchr(line,':')) {
          char * token;
          
          token = strtok(&(line[0]),":");
          
          //while (token != NULL) {
            strcpy(params->data[param_count].name,token);
            token = strtok (NULL, ":");
            strcpy(params->data[param_count++].value,token);
          //}
          
         }
      }
    
    }

    

    fclose(fp);
}

void getParam(struct swift_params *params, char * name, int * retParam) {
  
   int i; 
   
   for(i=0; i<MAX_NO_OF_PARAMS; i++) {
     if(strcmp(name,params->data[i].name)) {
       *retParam = atoi(params->data[i].value);       
       return;
     }
   }
}

void printParameters(struct swift_params *params) {

    int i;

    printf("\n--------------------\n");
    printf("SWIFT Parameter File\n");
    printf("--------------------\n");
    
    for(i=0; i<MAX_NO_OF_PARAMS; i++) {
        printf("Name: %s\n",params->data[i].name);
        printf("Value: %s\n",params->data[i].value);
    }

}

#endif /* SWIFT_PARSER_H */
