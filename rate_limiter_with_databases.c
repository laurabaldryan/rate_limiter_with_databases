#include <unistd.h>
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define RATE_LIMITER_SECONDS 20

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);
}

typedef struct Transaction{
  char card_number[16];
  char  pin_code[4];
  char cardholder_name[20];
  double card_amount;
  char expiration_date[5];
} Transaction;

int main(int argc, char **argv)
{
  
  MYSQL *con = mysql_init(NULL);
  if (con == NULL)
  {
    fprintf(stderr, "mysql_init() failed\n");
    exit(1);
  }

  if (mysql_real_connect(con, "localhost", "laura", "Password123#@!",
  "card", 0, NULL, 0) == NULL)
  {
    finish_with_error(con);
  }

  Transaction trans;
  
  do
  {
    printf("Enter your card number (must be 16 digits): ");
    scanf("%s", trans.card_number);

  } while(strlen(trans.card_number)!=16);

  char query[300];
  snprintf(query, 300, "SELECT pinCode, amount, cardHolderName FROM cards WHERE cardNumber LIKE ('%s')",trans.card_number);
  
  if (mysql_query(con,query))
  {
    printf("Error with storying in buffer");
    finish_with_error(con);
  }

  else
  {

    MYSQL_RES *result = mysql_store_result(con);
    
    if (result == NULL)
    {
      finish_with_error(con);
    }

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
      printf("Enter your pin: ");
      scanf("%s",trans.pin_code);  
      Transaction t2;

      time_t now;
      time(&now);
      printf("%s\n", ctime(&now));


      int value;
      value=strcmp(row[0],trans.pin_code);
      int value1;
      double seconds;

      if( value == 0 )
      {
        printf("%s  amount is ", row[2] ? row[2] : "NULL");
        printf("%s ", row[1] ? row[1] : "NULL");
        printf("\n");
      }

      else
      {

        printf("Enter your pin: ");
        scanf("%s",t2.pin_code);
        time_t now1;
        time(&now1);
        printf("%s \n",ctime(&now1));
        seconds=difftime(now1,now);
        value1=strcmp(row[0],t2.pin_code);
        
        int value2;
        double seconds1;
        Transaction t3;
        
        if (value1 == 0 && seconds < RATE_LIMITER_SECONDS )
        {
            printf("%s  amount is ", row[2] ? row[2] : "NULL");
            printf("%s ", row[1] ? row[1] : "NULL");
            printf("\n");
        }

        else if( value1 == 0 && seconds > RATE_LIMITER_SECONDS )
        {
          printf("Your card is blocked!");
          printf("\n");
        }

        else { 
         
          printf("Enter your pin: ");
          scanf("%s",t3.pin_code);
          value2 = strcmp(row[0],t3.pin_code);
          time_t now2;
          time(&now2);
          printf("%s \n",ctime(&now2));
          seconds1 = difftime(now2,now);
          int value3;

          if( seconds1 < RATE_LIMITER_SECONDS && value2 == 0 )
         
          {
            printf("%s  amount is ", row[2] ? row[2] : "NULL");
            printf("%s ", row[1] ? row[1] : "NULL");
            printf("\n");
          }

          else {

            Transaction t4;
            Transaction t5;
            int sleep_time = 20;
            printf("Wait %d seconds\n",sleep_time);
            sleep(sleep_time); 
            printf("\n");
            printf("Enter your pin: ");
            scanf("%s",t4.pin_code);
            printf("\n");
            value3 = strcmp(row[0],t4.pin_code);
            int value4;

            if (value3 == 0 )
            {
                printf("%s  amount is ", row[2] ? row[2] : "NULL");
                printf("%s ", row[1] ? row[1] : "NULL");
                printf("\n");
            }

            else
            {
                sleep_time*=2;
                printf("Wait %d seconds\n",sleep_time);
                sleep(sleep_time);
                printf("\n");
                printf("Enter your pin: ");
                scanf("%s",t5.pin_code);
                value4=strcmp(row[0],t5.pin_code);
                
                if ( value4 == 0 )
                {
                    printf("\n");
                    printf("%s  amount is ", row[2] ? row[2] : "NULL");
                    printf("%s ", row[1] ? row[1] : "NULL");
                    printf("\n");
                } 

                else
                {
                    printf("\n");
                    printf("Your card is blocked!\n");
                }
            }
           }
         }
      
       }
       printf("\n");
     }
   }
   mysql_close(con);
   exit(0);
}