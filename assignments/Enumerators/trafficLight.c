#include <stdio.h>
#include <unistd.h>

enum TrafficLight{
	RED,YELLOW,GREEN
};

int main(){
	enum TrafficLight light=RED;
	for(int i=0;i<5;i++){
		switch(light){
			case RED:
				printf("STOP ! Red light\n");
				light=GREEN;
				break;
			case YELLOW:
				printf("PREPARE ! Yellow light\n");
				light=RED;
				break;
			case GREEN:
				printf("GOOO ! Green light\n");
				light=YELLOW;
				break;
			    
		}
		sleep(1);
	}
	return 0;
}