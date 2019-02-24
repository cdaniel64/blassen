#include <iostream>

using namespace std;

int main(){   
 
int x = 0; //initialize variable x
int y = 0; //initialize variable y
int counter = 11; //initiallize counter
 
	for (int i=0; i <= 98; i++){       //loop to check for validy on numbers divisible by 3       
 
		 if ((counter%3) == 0 ){   //conditional loop, if number in counter multiple of 3, then store in x
			 x = x + counter;           
		 }     
		 else{						//if number is not multiple of 3, then store in y
			 y = y + counter;        
		 }  
	 counter++;   
	}  
	cout << x << " " << y << endl;	//loop counter displays variables values x, and y.
}