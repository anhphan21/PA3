#include "./multilevel.h"

void multilevel::print_cube(cube c) {
	for (int j = 0; j < c.size(); j++)
        cout << get_var_name(c[j]) << " ";
		// cout << c[j] << " ";
}

void multilevel::print_cube_set_with_name(vector<cube> c0) {
	for (int i = 0; i < c0.size(); i++) {
        print_cube(c0[i]);
        if (i != c0.size()-1)
            cout << "+ ";
    }
    cout << endl;
}

void multilevel::test_co_ker() {
	for (int i = 0; i < func_list.size(); i++) {
		cout << "Function: ";
		print_cube_set_with_name(func_list[i].get_cube_list());
		cout << endl;
		cout << "Co / Kernel list: " << endl;

		for (int j = 0; j < func_list[i].get_kernel_size() ; j++) {
			vector<cube> t_ker = func_list[i].get_kernel_at(j);
			cube t_co = func_list[i].get_co_kernel_at(j);
			cout << j << ". ";
			print_cube_set_with_name(func_list[i].get_kernel_at(j));
			cout << t_ker.size() << " // ";
			print_cube_set_with_name({func_list[i].get_co_kernel_at(j)});
			cout << t_co.size() << endl;
		}
	}
}

multilevel::multilevel() {
	var_list.resize(0);
	input_list.resize(0);
	output_list.resize(0);
	func_list.resize(0);
    synth_func_list;
    synth_var_list;
}

multilevel::~multilevel() {
	var_list.clear();
	input_list.clear();
	output_list.clear();
	func_list.clear();
    synth_func_list.clear();
    synth_var_list.clear();
}

void multilevel::setModuleName(string module_name) {
	this->module_name = module_name;
}

string multilevel::getModuleName() {
	return module_name;
}

void multilevel::setMode(string mode) {
	if (mode == "0")
		debug = 0;
	else
		debug = 1;
} 

void multilevel::add_input_var(string var_name) {		//Read ./input in blif file
	var_list.push_back(input_list.size()*2);
	var_list.push_back(input_list.size()*2+1);
	input_list.push_back(var_name);
}

void multilevel::add_output_var(string out_name) {
	output_list.push_back(out_name);
}

const int multilevel::get_input_idx(string in_name) {
	for (int i = 0; i < input_list.size(); i++)
		if (in_name == input_list[i])
			return i;
}

const string multilevel::get_output_name(int idx) {
	if (idx >= output_list.size())
		return synth_var_list[idx-input_list.size()*2];
	else
		return output_list[idx];
}

const int multilevel::get_output_idx(string out_name) {
	for (int i = 0; i < output_list.size(); i++)
		if (out_name == output_list[i])
			return i; 
}

const string multilevel::get_var_name(int idx) {
	if (idx >= input_list.size()*2) {
		if (!(idx%2))
			return synth_var_list[var_list[idx]-input_list.size()*2];
		else
			return synth_var_list[var_list[idx]-input_list.size()*2]+"_";
		
	}
	else {
		if (!(idx%2))
			return input_list[idx/2];
		else
			return input_list[idx/2]+"_";
	}
}

void multilevel::read_blif_file(string dir_path) {
    ifstream		blif_file(dir_path);
	int 			mode = 0;				//-2: read comment, -1: read module name, 0: read normal, 1: parse input, 2: parse output, 3: parse function
	bool			func_flag = 0;			//0: parse variables, 1: parse operation
	string 			temp;					//Store word from file waiting for process
	vector<string> 	temp_cube;
	vector<string> 	vari_list;

	while(1) {
		blif_file >> temp;
		if (temp[0] == '#') {					//check if we get the comment part
			getline(blif_file, temp);
			continue;
		}
		else if (temp == ".model")	{			//get module name
			if (blif_file.peek() != '\n')
				blif_file >> module_name;
			else
				module_name = dir_path;
				setModuleName(module_name);
			continue;
		}
		else if (temp == ".inputs") {
			mode = 1;
			continue;
		}
		else if (temp == ".outputs") {
			mode = 2;
			continue;
		}
		else if (temp == ".names") {
			mode = 3;
			if(func_flag) {														//Check if ther's still previous equation then add it to the data structure
				func temp_func;
				temp_func.set_output_name(vari_list.back());		//list input of equation to avoid traveling all of input
				vari_list.pop_back();											//Delete the output in the back of list
				
				for (int i = 0; i < vari_list.size(); i++) 						//Compute the idx of input and add to temp func 
					temp_func.set_input(get_input_idx(vari_list[i])*2);
				for (int i = 0; i < temp_cube.size(); i++) {					//Get cube for the expression
					cube temp_m;
					for (int j = 0; j < temp_cube[i].size(); j++) {				//Check the literals in the cube
						if (temp_cube[i][j] == '-')
							continue;
						else if (temp_cube[i][j] == '0') {
							int temp_input_idx = temp_func.get_input_idx(j)+1;
							if (!(temp_func.check_input_exist_in_list(temp_input_idx)))
								temp_func.set_input(temp_input_idx);
							temp_m.push_back(temp_input_idx);					
						}
						else if (temp_cube[i][j] == '1')
							temp_m.push_back(temp_func.get_input_idx(j));
					}
					temp_func.cube_list.push_back(temp_m);
				}
				func_list.push_back(temp_func);
			}

			func_flag = 0;
			vari_list.clear();
			temp_cube.clear();
			continue;
		}
		else if(temp == ".end") {
			if(func_flag) {											//Check if ther's still previous equation then add it to the data structure
				func temp_func;
				temp_func.set_output_name(vari_list.back());		//list input of equation to avoid traveling all of input
				vari_list.pop_back();											//Delete the output in the back of list
				
				for (int i = 0; i < vari_list.size(); i++) 						//Compute the idx of input and add to temp func 
					temp_func.set_input(get_input_idx(vari_list[i])*2);
				for (int i = 0; i < temp_cube.size(); i++) {					//Get cube for the expression
					cube temp_m;
					for (int j = 0; j < temp_cube[i].size(); j++) {				//Check the literals in the cube
						if (temp_cube[i][j] == '-')
							continue;
						else if (temp_cube[i][j] == '0') {
							int temp_input_idx = temp_func.get_input_idx(j)+1;
							if (!(temp_func.check_input_exist_in_list(temp_input_idx)))
								temp_func.set_input(temp_input_idx);
							temp_m.push_back(temp_input_idx);					
						}
						else if (temp_cube[i][j] == '1')
							temp_m.push_back(temp_func.get_input_idx(j));
					}
					temp_func.cube_list.push_back(temp_m);
				}
				func_list.push_back(temp_func);	
			}

			func_flag = 0;
			vari_list.clear();
			temp_cube.clear();
			break;
		}
        else if ((temp == ".default_input_arrival") || (temp == ".default_output_required") || (temp == ".default_input_drive") 
                || (temp == ".default_output_load ") || (temp == ".default_max_input_load")) {
            mode = 0;
            getline(blif_file, temp);
			continue;
        }
		
		if (mode == 1) {
			if (temp == "\\")
				continue;
			else {
				add_input_var(temp);
			}
		}
		else if (mode == 2) {
			if (temp == "\\")
				continue;
			else
				add_output_var(temp);
		}
		else if (mode == 3) {
			if (!func_flag) {													//1st time, code read the variables, if meet the \ continure, if meet 
				if (temp == "\\") {
					continue;
				} else {
					vari_list.push_back(temp);
					if ((blif_file.peek() == '\n'))
						func_flag = 1;
				}
			}
			else {
				temp_cube.push_back(temp);
				blif_file >> temp;
			}
		}
	}
	blif_file.close();
}

void multilevel::write_blif_file()  {
	string full_filename = getModuleName()+".blif";
	string output_dir = "./out/" + full_filename;
	ofstream output_file(output_dir);

    output_file << ".model " << getModuleName() << endl;
	
	output_file << ".inputs";
	for (int i = 0; i < input_list.size(); i++)
		output_file << " "  << input_list[i];
	output_file << endl;

	output_file << ".outputs";
	for (int i = 0; i < output_list.size(); i++)
		output_file << " "  << output_list[i];
	output_file << endl;
	
	for (int i = 0; i < func_list.size(); i++) {
		int func_input_size = 0;

		for (int j = 0; j < func_list[i].get_no_input(); j++) {
			if (!(func_list[i].get_input_idx(j)%2))
				func_input_size++;
		}

		output_file << ".names";

		//
		for (int j = 0; j < func_input_size; j++)
			output_file << " " << get_var_name(func_list[i].input_list[j]);
		output_file << " " << func_list[i].get_output_name() << endl;
		//
		for (int j = 0; j < func_list[i].get_no_cube(); j++) {	//Check each cube of function	
			for (int k = 0; k < func_input_size; k++) {			//Write the 01- line
				int n = 0;
				for (; n < func_list[i][j].size(); n++) {
					if (func_list[i][j][n]%2) {
						if ((func_list[i][j][n]-1) == func_list[i].input_list[k]) {
							output_file << "0";
							break;
						}
					}
					else {
						if (func_list[i][j][n] == func_list[i].input_list[k]) {
							output_file << "1";
							break;
						}
					}
				}
				if (n == func_list[i][j].size())
					output_file << "-";
			}
			output_file << " 1" << endl;
		}
	}

	for (int i = 0; i < synth_func_list.size(); i++) {

		int func_input_size = 0;
		for (int j = 0; j < synth_func_list[i].get_no_input(); j++) {
			if (!(synth_func_list[i].get_input_idx(j)%2))
				func_input_size++;
			// cout << synth_func_list[i].input_list[j] << endl;
		}

		if (debug) {
			cout << "Printing function: " << synth_func_list[i].get_output_name() << endl;
			cout << "No input in function: " << func_input_size << endl;
		}

		output_file << ".names";
		for (int j = 0; j < func_input_size; j++)
			output_file << " " << get_var_name(synth_func_list[i].input_list[j]);
		output_file << " " << synth_func_list[i].get_output_name() << endl;
		
		for (int j = 0; j < synth_func_list[i].get_no_cube(); j++) {	//Check each cube of function	
			for (int k = 0; k < func_input_size; k++) {			//Write the 01- line
				int n = 0;
				for (; n < synth_func_list[i][j].size(); n++) {
					if (synth_func_list[i][j][n]%2) {
						if ((synth_func_list[i][j][n]-1) == synth_func_list[i].input_list[k]) {
							output_file << "0";
							break;
						}
					}
					else {
						if (synth_func_list[i][j][n] == synth_func_list[i].input_list[k]) {
							output_file << "1";
							break;
						}
					}
				}
				if (n == synth_func_list[i][j].size())
					output_file << "-";
			}
			output_file << " 1" << endl;
		}
	}
	output_file << ".end";
}

void multilevel::single_extraction(func &func0, func &func1) {
	vector<cube> aux;
	vector<int> var_temp = var_list;
	vector<cube> check_0;
	cube check_1;

	if (debug) {
		cout << "Original function" << endl;
		print_cube_set_with_name(func0.cube_list);
		print_cube_set_with_name(func1.cube_list);
		cout << "------" << endl;
	}
	
	//add input to var_List_temp
	for (int i = 0; i < 2; i++) {
		var_temp.push_back(input_list.size()*2+i);
		var_temp.push_back(input_list.size()*2+i);
	}

	//Create aux func
	for (int i = 0; i < func0.get_no_cube(); i++) 
		aux.push_back(func0[i] + (input_list.size()*2));
	for (int i = 0; i < func1.get_no_cube(); i++)
		aux.push_back(func1[i] + (input_list.size()*2+1));
	

	//Check aux function 
	if (debug) {
		for (int i = 0; i < aux.size(); i++) {
			for (int j = 0; j < aux[i].size(); j++) {
				cout << aux[i][j] << " ";
			}
			cout << "_";
		}
		cout << endl;
	}

	vector<vector<cube>> aux_kernel = get_kernel(aux, check_0, check_1);
	vector<cube> aux_co_kernel = get_co_kernel(aux, aux_kernel);
	
	if (debug) {
		cout << "Done getting kernel / co_kernel" << endl;
		// print_cube_set_with_name(aux_co_kernel);
		cout << endl;
		cout << var_temp.size() << " " << input_list.size() << endl;
	}
	

	vector<cube> common_co_ker;
	for (int i = 0; i < aux_co_kernel.size(); i++) {
		int j = 0;
		for (; j < aux_co_kernel[i].size(); j++) {
			if (aux_co_kernel[i][j] >= (input_list.size()*2))
				break;
		}
		if(j == aux_co_kernel[i].size())
			common_co_ker.push_back(aux_co_kernel[i]);
	}

	for (auto i = common_co_ker.begin(); i != common_co_ker.end();) {
		if (i->size() == 1)
			i = common_co_ker.erase(i);
		else
			i++;
	}
	

	int max = 0;
	cube result;
	for (int i = 0; i < common_co_ker.size(); i++) {
		int cnt = 0;

		for (int j = 0; j < func0.get_no_cube(); j++) {
			if (func0[j]<=common_co_ker[i])
				cnt++;
		}
		for (int j = 0; j < func1.get_no_cube(); j++) {
			if (func1[j]<=common_co_ker[i])
				cnt++;
		}
	
		cnt = cnt*common_co_ker[i].size();
		if (cnt > max) {
			max = cnt;
			result = common_co_ker[i];
		}
	}

	if (debug) {
		cout << max << endl;
		cout << "------" << endl;
		print_cube(result);
		cout << endl;
		cout << "------" << endl;
		print_cube_set_with_name(common_co_ker);
		cout << "------" << endl;

		cout << "Function: ";
		// print_cube_set_with_name(aux);
		cout << endl;
		cout << "Co / Kernel list: " << endl;
		for (int j = 0; j < aux_kernel.size() ; j++) {
			cout << j << ". ";
			// print_cube_set_with_name(aux_kernel[j]);
			cout << aux_kernel[j].size();
			cout << " // ";
			// print_cube_set_with_name({aux_co_kernel[j]});
			cout << aux_co_kernel[j].size() << " " << endl;
			cout << "Result size: " << result.size() << endl;
		}
		print_cube(check_1);
		cout << endl;
		cout << "Done extracting single cube!!" << endl;
	}
	
	if (result.size() > 1) {
		// //Create new function variable
		int new_gate_idx = add_new_gate({result});
		cube new_cube({new_gate_idx});
		// //modify old gate
		vector<cube> new_func0 = get_div_by_cube(func0.cube_list, result);
		vector<cube> new_func1 = get_div_by_cube(func1.cube_list, result);
		
		for (int i = 0; i < new_func0.size(); i++)
			new_func0[i] = new_func0[i]+new_cube;

		for (int i = 0; i < new_func1.size(); i++)
			new_func1[i] = new_func1[i]+new_cube;
		
		func0.set_cube_list(new_func0);
		func1.set_cube_list(new_func1);
	}
}

int multilevel::add_new_gate(vector<cube> Q) {		//Return var_list idx corresponding to output_var
	func new_gate;
	
	//Add name_name for new gate
	string func_name = "st_"+to_string(synth_var_list.size());
	new_gate.set_output_name(func_name);

	//Set cube list and update input list, no input for gate
	new_gate.set_cube_list(Q);

	//put back output idx to var_list
	var_list.push_back(2*input_list.size()+synth_var_list.size());
	var_list.push_back(2*input_list.size()+synth_var_list.size());
	
	//Add output var to synth var name
	synth_var_list.push_back(func_name);
	//add gate to synth gate list 
	synth_func_list.push_back(new_gate);

	//Return the idx of the var_name in var_list
	if (debug)
		cout << "Gate " << synth_func_list.back().output_name << " has been created by add_new_gate" << endl;
	cout << "Gate: " << synth_func_list[synth_func_list.size()-2].output_name << "still there " << endl;
	return (input_list.size()*2+(synth_var_list.size()-1)*2);
}

void multilevel::decompose(func &f) {
	//Get kernel
	if (f.get_no_cube() != 1) {
		vector<cube> no_var;
		cube pre;
		vector<vector<cube>> kernel_check = get_kernel(f.cube_list, no_var, pre);

		if (debug)  {
			cout << "\n----------" << endl;
			cout << "Decomposing function: " << endl;
			print_cube_set_with_name(f.cube_list);
			cout << "No kernel: " << kernel_check.size() << endl;
			for (int i = 0; i < kernel_check.size(); i++)
				print_cube_set_with_name(kernel_check[i]);
			if (f.output_name == "st_1") {
				cout << "Check this bitch!!" << endl;
				cout << "Size of synthesis vector: " << synth_func_list.size() << endl;
			}
		}

		if (kernel_check.size() != 1) {

			if (debug) {
				cout << "-------- Start Decomposing --------" << endl;
			}

			int ker_id;
			if (f.cube_list == kernel_check.back()){
				int max = 0;
				for (int i = 0; i < kernel_check.size()-1; i++) {
					if (kernel_check[i].size() > max) {
						ker_id = i;
						max = kernel_check[i].size();
					}
				}
			}
			else
				ker_id = kernel_check.size()-1;

			if (debug) {
				cout << "Getting ker id: " << ker_id << endl;
				cout << "Kernel coresponding to ker id: ";
				print_cube_set_with_name(kernel_check[ker_id]);
			}
			
			func::div mess= get_div(f.cube_list, kernel_check[ker_id]);

			if (debug) {
				cout << "Decomposition: " << endl;
				cout << "-> Quotient: ";
				print_cube_set_with_name(mess.Q);
				cout << "-> Remainer: ";
				print_cube_set_with_name(mess.R);
				if (f.output_name == "st_1") {
					cout << "Still watching u " << f.output_name << endl;
				}
			}
			
			cout << "Before create new func for decom: " << f.output_name << endl;

			int co_idx;
			if (mess.Q[0].size() > 1) {
				if (debug)
					cout << "Create gate for co_kernel" << endl;
				co_idx = add_new_gate(mess.Q);
			}
			else
				co_idx = mess.Q[0][0];
			
			func *temp2bug = &f;
			cout << synth_func_list.size() << endl;
			////////////////////////////////////////////
			//Adding new function corresponding to kernel and co_kernel
			int ker_idx = add_new_gate(kernel_check[ker_id]);		// <====== Problem here
			f.print_function();
			//////////////////////////////////////////////////
			cout << synth_func_list.size() << endl;


			cube decom({ker_idx,co_idx});

			vector<cube> new_func_cube_list;
			if (mess.R.size()==0)
				new_func_cube_list = {decom};
			else
				new_func_cube_list = {decom, mess.R[0]};

			if (debug) {
				cout << "Ker / Co id: " << ker_idx << "___" << co_idx << endl;
				cout << "Decompose part: ";
				print_cube(decom);
				cout << endl;
				cout << "New function create!!!" << endl;
				cout << "Function: " << synth_func_list.back().output_name << " = ";
				print_cube_set_with_name(synth_func_list.back().cube_list);
			}

			//Modify old gate
			temp2bug->set_cube_list(new_func_cube_list);

			if (debug) {
				cout << "Original function after decomposing!!!" << endl;
				cout << "Function: " << f.output_name << " = ";
				print_cube_set_with_name(f.cube_list);
				cout << endl;
			}

			decompose(synth_func_list.back());
		}
		else
			if (debug)
				cout << "Original function cannot decompose anymore" << endl;
	}
}

void multilevel::test_decompose() {
	for (auto i = func_list.begin(); i != func_list.end(); i++)
		decompose(*i);
}

int multilevel::cal_literal() {
	int no_lit = 0;
	for (int i = 0; i < func_list.size(); i++) {
		for (int j = 0; j < func_list[i].cube_list.size(); j++) {
			no_lit+=func_list[i][j].size();
		}
	}
	for (int i = 0; i < synth_func_list.size(); i++) {
		for (int j = 0; j < synth_func_list[i].cube_list.size(); j++) {	
			no_lit+=synth_func_list[i][j].size();
		}
	}
	return no_lit;
}

void multilevel::single_extraction_for_synth_list(int start, int end) {
	if (start != end) {
		for (int i = start; i < end/2; i++)
			single_extraction(synth_func_list[i],synth_func_list[end-i-1]);
		single_extraction_for_synth_list(end, synth_func_list.size());
	}
}

void multilevel::single_extraction_network() {
	int no_func = func_list.size();
	for (int i = 0; i < no_func/2; i++) {
		single_extraction(func_list[i],func_list[no_func-i-1]);
	}
	single_extraction_for_synth_list(0, synth_func_list.size());
}

void multilevel::synthesis() {
	if (func_list[0].cube_list.size() == 1 )
		single_extraction_network();
	else
		test_decompose();
}