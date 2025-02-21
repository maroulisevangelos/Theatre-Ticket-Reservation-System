#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "p3200101-p3200178.h"

int bank;		//logariasmos stin trapeza
double suc=0;		//epitiximenes sinallages
double not_space=0;	//apotiximenes logo elipsis theseon
double not_card=0;	//apotiximenes logo kartas
pthread_mutex_t sucMutex;		//mutex gia to suc
pthread_mutex_t not_spaceMutex;	//mutex gia to not_space
pthread_mutex_t not_cardMutex;	//mutex gia to not_card
double anam = 0;		//xronos anamonis
pthread_mutex_t anamMutex;	//mutex gia to anam 
double exip;			//xronos exipiretisis
pthread_mutex_t exipMutex;	//mutex gia to exip	
int plan [Nzonea+Nzoneb][Nseat];   //plano = [10+20][10]
int *custArray = NULL;	//lista ton customers
int s;			//o sporos
int cust;		//o arithmos ton pelaton
pthread_mutex_t tilMutex;		//mutex gia tous tilefonites
pthread_cond_t tilholdCondition;	//condition gia tous tilefonites
pthread_mutex_t planMutex[Nzonea+Nzoneb][Nseat] ;		//mutex gia to plano
int til=0;		//tilefonites pou miloun
int tam=0;		//tamies pou eksipiretoun
pthread_mutex_t tamMutex;		//mutex gia tous tamies
pthread_cond_t tamholdCondition;	//condition gia tous tamies
pthread_mutex_t bankMutex;		//mutex gia to logariasmo trapezas

void *eksipiretisi(void *x){
	int rc,time,i,y,men,z,flag,plir;	//diafores metablites pou xreiazontai
	double t;				//----------//-----------//----------
	int zone;		//zone pou epilegei o pelatis
	int pl;		//plithos thesewn pou thelei o pelatis
	char *z_name;		//A / B
	int line;		//arithmos grammis
	int price =0;		//kostos ton theseon poy thelei na kleisei
	struct timespec start,stop;	//gia ti metrisi tis anamonis
	struct timespec startexip,stopexip;	//gia ti metrisi tis exipiretisis
	int id = *(int *)x;	//to id tou pelati
	
	clock_gettime( CLOCK_REALTIME, &start);	//xekina na metras gia anamoni
	clock_gettime( CLOCK_REALTIME, &startexip);	//xekina na metras gia exipiretisi
	
	rc = pthread_mutex_lock(&tilMutex);	//lock tilMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	while (til==Ntel){	//an oloi oi tilefonites miloun perimene
		rc = pthread_cond_wait(&tilholdCondition, &tilMutex);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);
			pthread_exit(&rc);
		}
	}	//den perimenei allo-ksekinaei na milaei
	
	til++;		//afksise tous tilefonites pou miloun
	
	rc = pthread_mutex_unlock(&tilMutex);	//unlock tilMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	rc = pthread_mutex_lock(&anamMutex);	//lock anamMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	clock_gettime( CLOCK_REALTIME, &stop);	//stamata na metras afou ksekinise na milaei
	t = stop.tv_sec - start.tv_sec;	//ipologise to xrono
	anam += t;		//prosthese to xrono sti sinoliki anamoni
	
	rc = pthread_mutex_unlock(&anamMutex);	//unlock anamMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	zone = rand_r(&s) %100 + 1;		  //epilogi zonis
	pl = rand_r(&s) %Nseathigh + Nseatlow;	  //epilogi plithous thesewn
	time = rand_r(&s) %tseathigh + tseatlow; //xronos pou apaiteitai gia na brethoun theseis
	sleep(time);
	for (i=0;i<Nzonea+Nzoneb;i++){ 
		for (y=0;y<Nseat;y++){
			rc = pthread_mutex_lock(&planMutex[i][y]);	//lock planMutex 
			if (rc != 0) {
    				printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
       			pthread_exit(&rc);
			}	
		}
	}
	if (zone <= Pzonea){		//epelekse Zone A
		z_name = "A";
		flag =0;		//simaia - den exei brei akoma theseis
		i=0;	
		while(i<Nzonea){	//gia kathe grammi tis zonis A
			y=0;
			while (y < Nseat){	//elekse kathe thesi
				if (plan[i][y] != 0){	//kratimeni thesi
					y++;
				}else{	//mia toulaxiston eleftheri thesi
					break;
				}
			}
			men = Nseat-y;	//theseis pou menoun se mia grammi
			if (men >= pl) {	//an ftanoun oi kenes theseis tis grammis
				for (z=y;z<y+pl;z++) {	//desmefse tis theseis aftes me to id tou pelati
					plan[i][z] = id;
				}
				z=y;	//krata tin proti thesi apo aftes pou desmefses 
				line = i;	//i grammi ton theseon
				price = Czonea *pl;	//to kostos gia oles tis theseis
				flag =1;	//brikes tis theseis
				break;
			}	
			i++;
		}
	}else{		//epelexe zoni B
		z_name = "B";
		flag =0;	//simaia - den exei brei akoma theseis
		i=Nzonea;
		while(i<Nzonea+Nzoneb){	//gia kathe grammi tis zonis B
			y=0;
			while (y < Nseat){	//elekse kathe thesi
				if (plan[i][y] != 0){	//kratimeni thesi
					y++;
				}else{	//mia toulaxiston eleftheri thesi
					break;
				}
			}
			men = Nseat-y;	//theseis pou menoun se mia grammi
			if (men >= pl) {	//an ftanoun oi kenes theseis tis grammis
				for (z=y;z<y+pl;z++) {	//desmefse tis theseis aftes me to id tou pelati
					plan[i][z] = id;
				}
				z=y;	//krata tin proti thesi apo aftes pou desmefses
				line = i;	//i grammi ton theseon
				price = Czoneb *pl;	//to kostos gia oles tis theseis
				flag =1;	//brikes tis theseis
				break;
			}	
			i++;
		}
	}
	if (flag !=1){		//den iparxoun oi katalliles theseis
		printf("%d: The reservation didn't succeed because there are not the proper seats \n",id);
		rc = pthread_mutex_lock(&exipMutex);	//lock exipMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
	
		clock_gettime( CLOCK_REALTIME, &stopexip);	//stamata na metras to xrono eksipiretisis
		t = stopexip.tv_sec - startexip.tv_sec;	//ipologise to xrono
		exip += t;		//prosthese to xrono sti sinoliki exipiretisi
	
		rc = pthread_mutex_unlock(&exipMutex);	//unlock exipMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
		rc = pthread_mutex_lock(&tilMutex);	//lock tilMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
		til--;	//stamataei na mialei me tilefoniti o pelatis - meiose tous tilefonites
		rc = pthread_mutex_unlock(&tilMutex);	//unlock tilMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
		rc = pthread_mutex_lock(&not_spaceMutex);	//lock not_spaceMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
		not_space++;	//afksise ton arithmo ton sinallagon pou apetixan logo eleipsis ton katallilon theseon
		rc = pthread_mutex_unlock(&not_spaceMutex);	//unlock not_spaceMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
		for (i=0;i<Nzonea+Nzoneb;i++){ 
			for (y=0;y<Nseat;y++){
				rc = pthread_mutex_unlock(&planMutex[i][y]);	//unlock planMutex
				if (rc != 0) {
    					printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
       				pthread_exit(&rc);
				}	
			}
		}
		rc = pthread_cond_signal(&tilholdCondition);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);
			pthread_exit(&rc);
		}
		pthread_exit(NULL);	//exodos apo to thread
	}
	
	rc = pthread_mutex_lock(&tilMutex);	//lock tilMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	til--;	//stamataei na mialei me tilefoniti o pelatis - meiose tous tilefonites
	rc = pthread_mutex_unlock(&tilMutex);	//unlock tilMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	clock_gettime( CLOCK_REALTIME, &start);	//xekina na metras ton xrono anamonis
		
	rc = pthread_cond_signal(&tilholdCondition);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);
		pthread_exit(&rc);
	}
	
		
	rc = pthread_mutex_lock(&tamMutex);	//lock tamMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	while (tam==Ncash){	//an oloi oi tamies miloun perimene
		rc = pthread_cond_wait(&tamholdCondition, &tamMutex);
		if (rc != 0) {	
			printf("ERROR: return code from pthread_cond_wait() is %d\n", rc);
			pthread_exit(&rc);
		}
	}	//ksekianei na eksipireteitai apo tamia
	
	tam++;		//afksise tous tamies pou miloun
	
	rc = pthread_mutex_unlock(&tamMutex);	//unlock tamMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	
	rc = pthread_mutex_lock(&anamMutex);	//lock anamMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	clock_gettime( CLOCK_REALTIME, &stop);	//stamata na metras to xrono anamonis
	t = stop.tv_sec - start.tv_sec;	//ipologise to xrono
	anam += t;		//prosthese to xrono sti sinoliki anamoni
	
	rc = pthread_mutex_unlock(&anamMutex);	//unlock anamMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	plir = rand_r(&s)% 100 + 1;	//petixe i pliromi;
	time = rand_r(&s) %tcashhigh + tcashlow; //xronos pou apaiteitai gia na dokimasei tin pliromi o tamias
	sleep(time);
	if (plir <= 90){	//petixe i pliromi
		rc = pthread_mutex_lock(&bankMutex);	//lock bankMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
		bank += price;	//prosthese to kostos ton theseon sto logariasmo 
		printf("%d: The reservtion succeed.Your seats are in Zone %s, in Line %d, Number: ",id,z_name,line+1);
		for (i=z+1;i<z+pl;i++){
			printf("%d, ",i);	//tipose tis theseis pou kratithikan
		}
		printf("%d ",z+pl);
		printf("and the cost of the transaction is %d euros.\n", price);
		rc = pthread_mutex_lock(&exipMutex);	//lock exipMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
	
		clock_gettime( CLOCK_REALTIME, &stopexip);	//stamata na metras to xrono eksipiretisis
		t = stopexip.tv_sec - startexip.tv_sec;	//ipologise to xrono
		exip += t;		//prosthese to xrono sti sinoliki exipiretisi
	
		rc = pthread_mutex_unlock(&exipMutex);	//unlock exipMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
		rc = pthread_mutex_lock(&sucMutex);	//lock sucMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
		suc++;	//afksise ton arithmo ton epitiximenon sinallagon
		rc = pthread_mutex_unlock(&sucMutex);	//unlock sucMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
		rc = pthread_mutex_unlock(&bankMutex);	//unlock bankMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
	}else{		//den petixe i pliromi
		for (i=z;i<z+pl;i++){
			plan[line][i] = 0;	//apodesmefse tis theseis pou exoun desmefthei apo auton ton pelati
		}
		printf("%d: The reservation didn't succeed because the transaction with the credit card wasn't accepted.\n",id);
		rc = pthread_mutex_lock(&exipMutex);	//lock exipMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
	
		clock_gettime( CLOCK_REALTIME, &stopexip);	//stamata na metras to xrono eksipiretisis
		t = stopexip.tv_sec - startexip.tv_sec;	//ipologise to xrono
		exip += t;		//prosthese to xrono sti sinoliki exipiretisi
	
		rc = pthread_mutex_unlock(&exipMutex);	//unlock exipMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		} 
		rc = pthread_mutex_lock(&not_cardMutex);	//lock not_cardMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
			pthread_exit(&rc);
		}
		not_card++;	//afksise ton arithmo ton sinallagon pou apeteixan logo tis kartas
		rc = pthread_mutex_unlock(&not_cardMutex);	//unlock not_cardMutex
		if (rc != 0) {	
			printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
			pthread_exit(&rc);
		}
	}
	for (i=0;i<Nzonea+Nzoneb;i++){ 
		for (y=0;y<Nseat;y++){
			rc = pthread_mutex_unlock(&planMutex[i][y]);	//unlock planMutex
			if (rc != 0) {
    				printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
       			pthread_exit(&rc);
			}	
		}
	}
	
	rc = pthread_mutex_lock(&tamMutex);	//lock tamMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_lock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	tam--;	//teleiose i douleia tou tamia - meiose tous tameies pou miloun
	
	rc = pthread_mutex_unlock(&tamMutex);	//unlock tamMutex
	if (rc != 0) {	
		printf("ERROR: return code from pthread_mutex_unlock() is %d\n", rc);
		pthread_exit(&rc);
	}
	
	rc = pthread_cond_signal(&tamholdCondition);
	if (rc != 0) {	
		printf("ERROR: return code from pthread_cond_signal() is %d\n", rc);
		pthread_exit(&rc);
	}
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	int i,y,rc,flag;	//diafores metablites pou xreiazontai
	
	if (argc != 3) {	//elegxos gia to plithos ton orismaton
		printf("ERROR: the program should take two arguments, the number of customers and the seed!\n");
		exit(-1);
	}	
	
	
	cust = atoi(argv[1]);	//arithmos pelaton
	s = atoi(argv[2]);	//sporos
	
	if (cust < 0 || s<0) {	//elegxos oste ta orismata na einai thetika
		printf("ERROR: the two arguments should be positive numbers. Current numbers given: %d and %d.\n", cust, s);
		exit(-1);
	}
	
	 
	for (i=0;i<Nzonea+Nzoneb;i++){ //arxikopoiisi planou
		for (y=0;y<Nseat;y++){
			plan[i][y]=0;
		}
	}
	
	pthread_t *threads;

	threads = malloc(cust * sizeof(pthread_t)); //lista ton threads
	if (threads == NULL) {
		printf("NOT ENOUGH MEMORY!\n");
		return -1;
	}
		
	custArray = malloc(cust * sizeof(int));	//lista ton pelaton
	
	rc = pthread_mutex_init(&sucMutex, NULL);  	//arxikopoiisi tou sucMutex 
	if (rc != 0) {
    		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       		exit(-1);
	}
	
	rc = pthread_mutex_init(&not_spaceMutex, NULL);  	//arxikopoiisi tou not_spaceMutex 
	if (rc != 0) {
    		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       		exit(-1);
	}
	
	rc = pthread_mutex_init(&not_cardMutex, NULL);  	//arxikopoiisi tou not_cardMutex 
	if (rc != 0) {
    		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       		exit(-1);
	}
	
	rc = pthread_mutex_init(&anamMutex, NULL);  	//arxikopoiisi tou anamMutex 
	if (rc != 0) {
    		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       		exit(-1);
	}
	
	rc = pthread_mutex_init(&exipMutex, NULL);  	//arxikopoiisi tou exipMutex 
	if (rc != 0) {
    		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       		exit(-1);
	}
	
  	rc = pthread_mutex_init(&tilMutex, NULL);  	//arxikopoiisi tou tilMutex 
	if (rc != 0) {
    		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       		exit(-1);
	}

  	rc = pthread_cond_init(&tilholdCondition, NULL);   //arxikopoiisi tou tilholdCondition
	if (rc != 0) {
    		printf("ERROR: return code from pthread_cond_init() is %d\n", rc);
       		exit(-1);
	}
	
	for (i=0;i<Nzonea+Nzoneb;i++){ 
		for (y=0;y<Nseat;y++){
			rc = pthread_mutex_init(&planMutex[i][y], NULL);  	//arxikopoiisi tou planMutex 
			if (rc != 0) {
    				printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       			exit(-1);
			}	
		}
	}
	
	
	rc = pthread_mutex_init(&tamMutex, NULL);  	//arxikopoiisi tou tamMutex 
	if (rc != 0) {
    		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       		exit(-1);
	}
	
	rc = pthread_cond_init(&tamholdCondition, NULL);   //arxikopoiisi tou tamholdCondition
	if (rc != 0) {
    		printf("ERROR: return code from pthread_cond_init() is %d\n", rc);
       		exit(-1);
	}
	
	rc = pthread_mutex_init(&bankMutex, NULL);  	//arxikopoiisi tou bankMutex 
	if (rc != 0) {
    		printf("ERROR: return code from pthread_mutex_init() is %d\n", rc);
       		exit(-1);
	}

	int time;	
   	for(i= 0; i< cust;i++) {	//dimiourgia olon ton threads
		custArray[i] = i + 1;
  		rc = pthread_create(&threads[i], NULL,eksipiretisi, &custArray[i]);//dimiourgia tou thread		
  		time = rand_r(&s)%treshigh+treslow; //o xronos mexri ton epomeno pelati
  		sleep(time);
    		if (rc != 0) {		//elegxos oti to thread dimiourgithike swsta
    			printf("ERROR: return code from pthread_create() is %d\n", rc);
       			exit(-1);
       		}
    	}

	void *status;
	
  	for (i = 0; i < cust; i++) {	//join gia ola ta threads
    		rc = pthread_join(threads[i], &status);
		if (rc != 0) {
			printf("ERROR: return code from pthread_join() is %d\n", rc);
			exit(-1);		
		}
  	}
	
	flag=0;	//simaia pou boithaei stin ektiposi ton pliroforion
 	for (i=0;i<Nzonea;i++){	//tipose plirofories gia ti zoni A
 		for (y=0;y<Nseat;y++){
 			if (plan [i][y]!= 0){
 				if (flag==1){
 					printf(", ");
 				}
 				flag=1;
 				printf("Zone A/ Line %d/ Seat %d/ Customer %d",i+1,y+1,plan[i][y]);
 			}
 		}
 	}
 	
 	for (i=Nzonea;i<Nzonea+Nzoneb;i++){	//tipose plirofories gia ti zoni B
 		for (y=0;y<Nseat;y++){
 			if (plan [i][y]!= 0){
 				if (flag==1){
 					printf(", ");
 				}
 				flag =1;
 				printf("Zone B/ Line %d/ Seat %d/ Customer %d",i+1,y+1,plan[i][y]);
 			}
 		}
 	}
 	
 	printf(".\n");
 	
 	printf("The total income from the sales is %d euros.\n",bank);	//tipose ta sinolika esoda
 	
 	if (cust!=0){	//ektipose ta pososta
 		printf("The rate of successful reservations is: %.2f%%.\n",suc/cust*100);
 		printf("The rate of unsuccessful reservations because of the limited number of seats is: %.2f%%.\n",not_space/cust*100);
 		printf("The rate of unsuccessful reservations because of the credit card is: %.2f%%.\n",not_card/cust*100);
 	}
 	
 	printf("The average time of waiting is: %.2f sec.\n",anam/cust);	//tipose to meso xrono anamonis ana pelati
 	
 	printf("The average time of service is: %.2f sec.\n",exip/cust);	//tipose to meso xrono exipiretisis ana pelati
	
	rc = pthread_mutex_destroy(&sucMutex);	//"katastrofi" sucMutex
	if (rc != 0) {		
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	rc = pthread_mutex_destroy(&not_spaceMutex);	//"katastrofi" not_spaceMutex
	if (rc != 0) {		
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	rc = pthread_mutex_destroy(&not_cardMutex);	//"katastrofi" not_cardMutex
	if (rc != 0) {		
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	rc = pthread_mutex_destroy(&anamMutex);	//"katastrofi" anamMutex
	if (rc != 0) {		
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	rc = pthread_mutex_destroy(&exipMutex);	//"katastrofi" exipMutex
	if (rc != 0) {		
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
		exit(-1);		
	}
	
  	rc = pthread_mutex_destroy(&tilMutex);	//"katastrofi" tilMutex
	if (rc != 0) {		
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	rc = pthread_cond_destroy(&tilholdCondition);	//"katastrofi" tilholdCondition
	if (rc != 0) {		
		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	for (i=0;i<Nzonea+Nzoneb;i++){ 
		for (y=0;y<Nseat;y++){
			rc = pthread_mutex_destroy(&planMutex[i][y]);	//"katastrofi" planMutex tou planMutex 
			if (rc != 0) {
    				printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
       			exit(-1);
			}	
		}
	}
	
	
	rc = pthread_mutex_destroy(&tamMutex);	//"katastrofi" tamMutex
	if (rc != 0) {		
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	rc = pthread_cond_destroy(&tamholdCondition);	//"katastrofi" tamholdCondition
	if (rc != 0) {		
		printf("ERROR: return code from pthread_cond_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	rc = pthread_mutex_destroy(&bankMutex);	//"katastrofi" bankMutex
	if (rc != 0) {		
		printf("ERROR: return code from pthread_mutex_destroy() is %d\n", rc);
		exit(-1);		
	}
	
	//apeleftherosi mnimis
	free(custArray);
	free(threads);
	return 1;
}
