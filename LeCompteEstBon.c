#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBERS (6)

#define NB_ARGS (NUMBERS + 2)


#define ARGS_ERROR   (-1)
#define RES_ERROR    (0)
#define SOLUTION_LEN (128)

#define MIN_RESULT (100)
#define MAX_RESULT (1000)





static   int Result = ARGS_ERROR;

static unsigned Accuracy = UINT16_MAX;

static char Solution [SOLUTION_LEN] = {0};


static bool LeCompteEstBon(unsigned Numbers[], size_t Size, char* Sol);




static void  ParseArgs(int argc, char * argv[], unsigned  Numbers[])
{
   if(( NB_ARGS == argc) && ( MIN_RESULT < ( Result = atoi(*(++argv)))) && 
                              (Result < MAX_RESULT ))
   {
      int Number = ARGS_ERROR;

      for ( int Index = 0; Index < NUMBERS ;Index ++)
      {
         if (0 < (Number = atoi(*(++argv))) && (Number <= MIN_RESULT))
         {
            Numbers[Index] = (unsigned) Number;
         }
         else
         {
            Result = ARGS_ERROR;
         }
      }
   }
   else
   {
      Result = ARGS_ERROR;
   }
}


static bool Calculate( char Operation , uint8_t First, uint8_t Second, 
                           unsigned  Numbers[], size_t Size, char * Sol)
{
   bool Stop = false;
   int Res = RES_ERROR;
   unsigned Op1 = Numbers[First];
   unsigned Op2 = Numbers[Second];

   switch (Operation)
   {
      case '+':
         Res = Op1 + Op2;
         break;
         
      case '-':
         Res = Op1 - Op2;
         break;
         
      case '*':
         Res = Op1 * Op2;
         break;
         
      case '/':
         Res = (Op1 % Op2)? RES_ERROR: Op1 / Op2; 
         break;

      default:
         fprintf(stderr, "Bad Operation = %c\n", Operation);
   }
   
   if ( Res)
   {
      char NewSol [SOLUTION_LEN] = {0};
      unsigned NewAccuracy = abs(Res - Result);
      
      snprintf(NewSol , SOLUTION_LEN , "%s\n%u %c %u = %d", 
                                             Sol, Op1, Operation, Op2, Res);
    
      
      if ( NewAccuracy < Accuracy)
      {
         Accuracy = NewAccuracy;
#ifdef _WIN64
         strcpy_s(Solution, sizeof Solution, NewSol);
#else
         strcpy(Solution, NewSol);
#endif
      }
      if (!NewAccuracy)
      {
         Stop = true;
      }
      else if (Size > 2)
      {
         unsigned  NewArray[Size] ;
         Size--;
         
         memcpy(NewArray, Numbers, sizeof NewArray);
         NewArray[First] = Res;
         NewArray[Second] = Numbers[Size];
         Stop = LeCompteEstBon(NewArray, Size, NewSol);
      }
   }

   return Stop;
}

static bool LeCompteEstBon( unsigned  Numbers[], size_t Size, char* Sol)
{
   bool Stop = false;
   int First, Second;

   for ( int i = 0; (i< Size -1) && (!Stop); i++)
   {
      for (int j = i +1 ; (j < Size) && (!Stop); j++)
      {
         Stop =   Calculate( '+', i , j, Numbers, Size , Sol) ;

         if ( !Stop)
         {
            if (Numbers[i] < Numbers[j])
            {
               First = j;
               Second = i; 
            }
            else
            {
               First = i;
               Second =j;
            }
            

            Stop = ( ( 1 == Numbers[Second]) ?
                           false : 
                           Calculate( '*', First, Second, Numbers, Size , Sol) ||
                            Calculate( '/', First, Second, Numbers, Size , Sol))
                  ||
                     ( (Numbers[First] == Numbers[Second])? 
                           false: 
                           Calculate( '-', First, Second, Numbers, Size , Sol));
                     
                     
         }
      }
   }
   return Stop;
}

int main(int argc, char *argv[])
{
   unsigned  Numbers[NUMBERS]={0};
   
   ParseArgs(argc, argv, Numbers);
   
   if( ARGS_ERROR != Result)
   {
      char Sol[SOLUTION_LEN] = {0};
      
      LeCompteEstBon( Numbers, NUMBERS, Sol);
      puts(Solution);
      Result = EXIT_SUCCESS;
   }
   else
   {
      printf( "%s <Result> <N1> <N2> ... <N6>\n"
               "Result  : 101 .. 999\n"
               "N1 ... N6 :1 .. 100\n", *argv);
      Result = EXIT_FAILURE;
   }
   
   return Result;
}
