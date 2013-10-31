#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "utf8.h"


int unichars(const char* src)
{
   long ssz = strlen(src);
   u_int32_t* unic = NULL;
   u_int32_t* myunic = NULL;
   char* mys = NULL;

   int i = 0;
   int j = 0;
   int n = 0, nn = 0;
   int m = 0;
   bool repeated = false;

   unic = (u_int32_t*) malloc(ssz*sizeof(u_int32_t));
   myunic = (u_int32_t*) malloc(ssz*sizeof(u_int32_t));
   mys = (char*) malloc(ssz*sizeof(char));

   if(unic==NULL)
	exit(1);
   else if(myunic==NULL)
   {
	free(unic);
	exit(2);
   }
   else if(mys==NULL)
   {
	free(unic);
	free(myunic);
	exit(3);
   }

   m = u8_toucs(unic, ssz, (char*)src, -1);

   for(i=0; i<m; i++)
   {
	repeated = false;
	for(j=0; j<i; j++)
	{
	   if(unic[i] == unic[j])
	   {
		repeated = true;
		break;
	   }
	}
	if(unic[i]>127 && !repeated)
	   myunic[n++] = unic[i];
   }
   myunic[n] = 0;

   nn = u8_toutf8(mys, ssz, myunic, -1);

   printf("\n*************************\n");
   printf("%s", mys);
   printf("\n*************************\n");

   free(mys);
   free(unic);
   free(myunic);

   return nn;
}


int main(int argc, char* argv[])
{
   FILE * pFile;
   long lSize;
   char * buffer;
   size_t result;
   int n;

   pFile = fopen ( argv[1] , "rb" );
   if (pFile==NULL) {fputs ("File error\n",stderr); exit (1);}

  // obtain file size:
   fseek (pFile , 0 , SEEK_END);
   lSize = ftell (pFile);
   rewind (pFile);

  // allocate memory to contain the whole file:
   buffer = (char*) malloc (sizeof(char)*(lSize+1));
   if (buffer == NULL) {fputs ("Memory error\n",stderr); exit (2);}

  // copy the file into the buffer:
   result = fread (buffer,1,lSize,pFile);
   if (result != lSize) {fputs ("Reading error\n",stderr); exit (3);}

  /* the whole file is now loaded in the memory buffer. */
   
   *(buffer+lSize) = 0;
   printf("file size: %ld\n", lSize);
   n = unichars(buffer);
   printf("Number of unichars: %d\n", n);

  // terminate
   fclose (pFile);
   free (buffer);
   return 0;
}
