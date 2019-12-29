// JupSystem
// de inicio os codigos estarao ja na memoria de instrução
// 

//declaraçao de variaveis
int process[3];
int name_process[3];
int start_process[3];
int end_process[3];
int process_status[3];

void main(void) {
	 
	int i;
	int current_process;
	int num_process;
	int process_finished;
	int interruption;
	num_process = 3;
	process_fished = 0;
	i = 0;
	current_process = 0;
	//init
	while(i < 3) {
		process[i] = 0;
		name_process[i] = 0
		start_process[i] = 0;
		end_process[i]  = 0;
		process_status[i] = 0;
	}

	//carregar o primeiro processo 
	while(process_finished < num_process) {
		interruption = get_interruption();
		if(interruption == 1) { // end program
			process_status[current_process]  = 1; //finished
			
		} else if(interruption == 2) { // input
			

		} else if (interruption == 3) { // output

		}
	}
}