#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <map>
#include <boost/algorithm/string.hpp>

enum class operation_priorities
{
	opening_bracket,
	binary_multiplication,
	unary_minus
};

int get_priority(char operation)
{
	switch (operation)
	{
	case '(':
	{
		return static_cast<int>(operation_priorities::opening_bracket);
	}
	case '*':
	{
		return static_cast<int>(operation_priorities::binary_multiplication);
	}
	case '-':
	{
		return static_cast<int>(operation_priorities::unary_minus);
	}
	}
}

void show_info()
{
	std::cout << "\n\n";
	std::cout << "Glukhov N. V.\n";
	std::cout << "g. 4211  n. 8\n";
	std::cout << "functions: -x;  x*y\n";
	std::cout << "standard form: 1st form";
}

int number_of_digits(int number)
{ 
	return (number /= 10) ? 1 + number_of_digits(number) : 1;
}

int main()
{
	system("color F0");

	std::string input_func, input_k, input_E;
	std::vector<std::string> rpn;
	std::stack<char> operation_stack;
	std::stack<int> calculation_stack;
	std::vector<std::vector<int>> table, table_E;
	std::vector<char> list_of_variables;
	std::vector<int> list_of_function_values, E, aux_E;
	std::map <std::vector<int>, int> map_table;
	int k;

	while (1)
	{
		k = -1;
		input_k.clear();
		input_func.clear();
		input_E.clear();
		rpn.clear();
		while (!operation_stack.empty())
		{
			operation_stack.pop();
		}
		while (!calculation_stack.empty())
		{
			calculation_stack.pop();
		}
		for (size_t i = 0u; i < table.size(); i++)
		{
			table[i].clear();
		}
		table.clear();
		for (size_t i = 0u; i < table_E.size(); i++)
		{
			table_E[i].clear();
		}
		table_E.clear();
		list_of_variables.clear();
		list_of_function_values.clear();
		E.clear();
		aux_E.clear();
		map_table.clear();

		show_info();

		std::cout << "\n\nenter the k: ";
		std::getline(std::cin, input_k);
		try
		{
			k = std::stoi(input_k);
		}
		catch (std::invalid_argument)
		{
			std::cout << "\ninvalid input";
			continue;
		}
		catch (std::out_of_range)
		{
			std::cout << "\nout of range input";
			continue;
		}

		if (k < 2)
		{
			std::cout << "\nk should be more than 1";
			continue;
		}

		std::cout << "enter the function: ";
		std::getline(std::cin, input_func);

		for (char& ch : input_func)
		{
			if (ch > 96 && ch < 123
				|| ch > 39 && ch < 43
				|| ch > 47 && ch < 58
				|| ch == 45)
			{

			}
			else
			{
				ch = ' ';
			}
		}
		input_func.erase(std::remove(input_func.begin(), input_func.end(), ' '), input_func.end());

		for (size_t i = 0u; i < input_func.length(); i++)
		{
			if (input_func[i] > 47 && input_func[i] < 58)
			{
				size_t counter = 0u;
				while (input_func[i + counter] > 47 && input_func[i + counter] < 58)
				{
					counter++;
				}

				int if_delete = -1;
				try
				{
					if_delete = std::stoi(input_func.substr(i, counter));
				}
				catch (std::out_of_range)
				{
					input_func.erase(i, counter);
					i = i - 1;
					continue;
				}

				if (if_delete >= k)
				{
					input_func.erase(i, counter);
					i = i - 1;
				}
				else
				{
					i = i + counter;
				}
			}
		}

		if (input_func == "")
		{
			std::cout << "\ninvalid function";
			continue;
		}

		std::cout << "the function is: " << input_func;

		// main processing
		bool invalid_function = false;
		size_t func_length = input_func.size();
		for (size_t i = 0u; i < func_length; i++)
		{
			char current_symbol = input_func[i];
			if (current_symbol > 96 && current_symbol < 123)
			{
				rpn.push_back(std::string() + current_symbol);
				auto add_condition = std::find(list_of_variables.begin(), list_of_variables.end(), current_symbol);
				if (add_condition == list_of_variables.end())
				{
					list_of_variables.push_back(current_symbol);
				}
			}
			else if (current_symbol > 47 && current_symbol < 58)
			{
				size_t counter = 0u;
				while (input_func[i + counter] > 47 && input_func[i + counter] < 58)
				{
					counter++;
				}
				rpn.push_back(input_func.substr(i, counter));
				i = i + counter - 1;
			}
			else if (current_symbol == '(')
			{
				operation_stack.push(current_symbol);
			}
			else if (current_symbol == '*')
			{
				if (operation_stack.empty())
				{
					operation_stack.push(current_symbol);
				}
				else if (get_priority(operation_stack.top()) < get_priority(current_symbol))
				{
					operation_stack.push(current_symbol);
				}
				else if (get_priority(operation_stack.top()) >= get_priority(current_symbol))
				{
					while (get_priority(operation_stack.top()) >= get_priority(current_symbol))
					{
						rpn.push_back(std::string() + operation_stack.top());
						operation_stack.pop();

						if (operation_stack.empty())
						{
							break;
						}
					}

					operation_stack.push(current_symbol);
				}
			}
			else if (current_symbol == '-')
			{
				if (operation_stack.empty())
				{
					operation_stack.push(current_symbol);
				}
				else if (get_priority(operation_stack.top()) <= get_priority(current_symbol))
				{
					operation_stack.push(current_symbol);
				}
				else if (get_priority(operation_stack.top()) > get_priority(current_symbol))
				{
					while (get_priority(operation_stack.top()) > get_priority(current_symbol))
					{
						rpn.push_back(std::string() + operation_stack.top());
						operation_stack.pop();

						if (operation_stack.empty())
						{
							break;
						}
					}

					operation_stack.push(current_symbol);
				}
			}
			else if (current_symbol == ')')
			{
				if (operation_stack.empty())
				{
					invalid_function = true;
					break;
				}

				while (operation_stack.top() != '(')
				{
					rpn.push_back(std::string() + operation_stack.top());
					operation_stack.pop();

					if (operation_stack.empty())
					{
						break;
					}
				}

				if (operation_stack.empty())
				{
					invalid_function = true;
					break;
				}

				if (operation_stack.top() == '(')
				{
					operation_stack.pop();
				}
			}
		}

		if (invalid_function)
		{
			std::cout << "\n\ninvalid function";
			continue;
		}

		while (!operation_stack.empty())
		{
			rpn.push_back(std::string() + operation_stack.top());
			operation_stack.pop();
		}

		size_t table_length = list_of_variables.size();
		size_t table_width = static_cast<size_t>(std::pow(k, table_length));
		for (size_t i = 0u; i < table_length; i++)
		{
			table.push_back(std::vector<int>());
		}

		int value;
		size_t step = table_width;
		for (size_t i = 0u; i < table_length; i++)
		{
			value = 0;
			step = step / static_cast<size_t>(k);
			for (size_t t = 0u; t < static_cast<size_t>(std::pow(k, i + 1)); t++)
			{
				for (size_t j = 0u; j < step; j++)
				{
					table[i].push_back(value);
				}
				
				if (value == k - 1)
				{
					value = 0;
				}
				else
				{
					value++;
				}
			}
		}

		for (size_t t = 0u; t < table_width; t++)
		{
			for (size_t i = 0u; i < rpn.size(); i++)
			{
				std::string current_elem = rpn[i];
				if (current_elem[0] > 47 && current_elem[0] < 58)
				{
					calculation_stack.push(std::stoi(current_elem));
				}
				else if (current_elem[0] > 96 && current_elem[0] < 123)
				{
					char variable = current_elem[0];
					auto iterator = std::find(list_of_variables.begin(), list_of_variables.end(), variable);
					size_t index = std::distance(list_of_variables.begin(), iterator);

					calculation_stack.push(table[index][t]);
				}
				else if (current_elem[0] == '*')
				{
					if (calculation_stack.empty())
					{
						invalid_function = true;
						break;
					}
					int operand_1 = calculation_stack.top();
					calculation_stack.pop();
					if (calculation_stack.empty())
					{
						invalid_function = true;
						break;
					}
					int operand_2 = calculation_stack.top();
					calculation_stack.pop();

					if (current_elem[0] == '*')
					{
						calculation_stack.push((operand_1 * operand_2) % k);
					}
				}
				else if (current_elem[0] == '-')
				{
					if (calculation_stack.empty())
					{
						invalid_function = true;
						break;
					}
					int operand = calculation_stack.top();
					calculation_stack.pop();

					if (current_elem[0] == '-')
					{
						if (operand == 0)
						{
							calculation_stack.push(0);
						}
						else
						{
							calculation_stack.push(k - operand);
						}
					}
				}
			}

			if (invalid_function)
			{
				break;
			}

			if (calculation_stack.size() > 1u)
			{
				invalid_function = true;
				break;
			}
			else
			{
				list_of_function_values.push_back(calculation_stack.top());
				calculation_stack.pop();
			}
		}
	    
		if (invalid_function)
		{
			std::cout << "\n\ninvalid function";
			continue;
		}

		std::cout << "\n\ntable:\n\n";
		for (size_t i = 0u; i < table_length; i++)
		{
			std::cout << std::setw(6) << std::left << list_of_variables[i];
		}
		std::cout << std::setw(6) << std::left << 'f';
		std::cout << '\n';
		for (size_t i = 0u; i < table_width; i++)
		{
			for (size_t j = 0u; j < table_length; j++)
			{
				std::cout << std::setw(6) << std::left << table[j][i];
				if (j == table_length - 1u)
				{
					std::cout << std::setw(6) << std::left << list_of_function_values[i];
				}
			}
			std::cout << '\n';
		}

		size_t number_of_variables = list_of_variables.size();
		std::cout << "\nf(";
		for (size_t i = 0u; i < number_of_variables; i++)
		{
			std::cout << list_of_variables[i];
			if (i != number_of_variables - 1u)
			{
				std::cout << ", ";
			}
		}
		std::cout << ") = ";
		for (size_t i = 0u; i < table_width; i++)
		{
			if (list_of_function_values[i] != 0)
			{
				if (list_of_function_values[i] != k - 1)
				{
					std::cout << list_of_function_values[i];
					for (size_t j = 0u; j < table_length; j++)
					{
						std::cout << "&J_" << table[j][i] << '(' << list_of_variables[j] << ')';
					}
				}
				else
				{
					for (size_t j = 0u; j < table_length; j++)
					{
						if (j == 0u)
						{
							std::cout << "J_" << table[j][i] << '(' << list_of_variables[j] << ')';
						}
						else
						{
							std::cout << "&J_" << table[j][i] << '(' << list_of_variables[j] << ')';
						}
					}
				}
				
				if (i != table_width - 1u)
				{
					std::cout << " v ";
				}
			}
		}

		std::cout << '\n';
		std::vector<std::string> input_E_result, input_E_result_final;
		bool wrong_input;
		while (1)
		{
			input_E_result.clear();
			input_E_result_final.clear();
			input_E.clear();
			wrong_input = false;
			aux_E.clear();

			std::cout << "\nenter the E (in this format: 1 2 3 ...): ";
			std::getline(std::cin, input_E);

			if (input_E.empty())
			{
				continue;
			}

			boost::split(input_E_result, input_E, boost::is_any_of(" "));
			for (auto& str : input_E_result)
			{
				if (!str.empty())
				{
					input_E_result_final.push_back(str);
				}
			}

			for (auto& str : input_E_result_final)
			{
				try
				{
					int e = std::stoi(str);
					aux_E.push_back(e);
				}
				catch (std::invalid_argument)
				{
					wrong_input = true;
					break;
				}
				catch (std::out_of_range)
				{
					wrong_input = true;
					break;
				}
			}

			if (wrong_input)
			{
				continue;
			}
			else
			{
				for (int& e : aux_E)
				{
					if (e > k - 1)
					{
						wrong_input = true;
						break;
					}
				}

				if (wrong_input)
				{
					continue;
				}
				else
				{
					break;
				}
			}
		}

		for (int& e : aux_E)
		{
			auto iterator = std::find(E.begin(), E.end(), e);
			if (iterator == E.end())
			{
				E.push_back(e);
			}
		}
		std::sort(E.begin(), E.end());

		size_t E_length = E.size();
		size_t table_E_length = table_length;
		size_t table_E_width = static_cast<size_t>(std::pow(E_length, table_E_length));
		for (size_t i = 0u; i < table_E_length; i++)
		{
			table_E.push_back(std::vector<int>());
		}

		size_t i_E;
		size_t step_E = table_E_width;
		for (size_t i = 0u; i < table_E_length; i++)
		{
			i_E = 0;
			step_E = step_E / E_length;
			for (size_t t = 0u; t < static_cast<size_t>(std::pow(E_length, i + 1)); t++)
			{
				for (size_t j = 0u; j < step_E; j++)
				{
					table_E[i].push_back(E[i_E]);
				}

				if (i_E == E_length - 1)
				{
					i_E = 0;
				}
				else
				{
					i_E++;
				}
			}
		}

		for (size_t i = 0u; i < table_width; i++)
		{
			std::vector<int> tmp;
			for (size_t j = 0u; j < table_length; j++)
			{
				tmp.push_back(table[j][i]);
			}
			map_table.insert(std::make_pair(tmp, list_of_function_values[i]));
		}

		bool function_belongs_to_the_class = true;
		for (size_t i = 0u; i < table_E_width; i++)
		{
			std::vector<int> tmp;
			for (size_t j = 0u; j < table_E_length; j++)
			{
				tmp.push_back(table_E[j][i]);
			}

			auto it = map_table.find(tmp);
			auto it_2 = std::find(E.begin(), E.end(), it->second);
			if (it_2 == E.end())
			{
				function_belongs_to_the_class = false;
				break;
			}
		}

		std::cout << "\nfunction ";
		if (function_belongs_to_the_class)
		{
			std::cout << "belongs";
		}
		else
		{
			std::cout << "doesn't belong";
		}
		std::cout << " to the class T({";
		for (size_t i = 0u; i < E_length; i++)
		{
			std::cout << E[i];
			if (i != E_length - 1u)
			{
				std::cout << ", ";
			}
		}
		std::cout << "})";
		// end of main processing

	    std::string choice;
		bool main_exit = false;
		while (1)
		{
			std::cout << "\n\ndo you want to repeat?  y/n  ";
			std::getline(std::cin, choice);

			if (choice == "y")
			{
				break;
			}
			else if (choice == "n")
			{
				main_exit = true;
				break;
			}
			else
			{
				std::cout << "\n...try again...";
				continue;
			}
		}

		if (main_exit)
		{
			break;
		}
	}

	system("pause");
	return 0;
}