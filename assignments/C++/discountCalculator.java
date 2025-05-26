#include  <iostream.h>

main(){
	double amount , discount ,netPayable;
	amount=0;
	discount=0;
	netPayable=0;
	
	cout<<"please enter the amount of the bill ";
	cint>>amount;
	
	if ( amount > 5000 ) { 
 //calculate amount at 15 % discount 
       discount = amount * (15.0 / 100); 
       netPayable = amount - discount; 
       cout << "The discount at the rate 15 % is Rupees " << discount << endl; 
       cout << "The payable amount is Rupees " << netPayable ; 
    } else { 
 // calculate amount at 10 % discount 
       discount = amount * (10.0 / 100); 
       netPayable = amount - discount; 
       cout << "The discount at the rate 10 % is Rupees " << discount << endl ; 
       cout << "The payable amount is Rupees " << netPayable ; 
           }
}