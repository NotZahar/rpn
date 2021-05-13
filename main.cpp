#include <string>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cmath>
#include <iomanip>

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

	std::string input_func, input_k;
	std::vector<std::string> rpn;
	std::stack<char> operation_stack;
	std::stack<int> calculation_stack;
	std::vector<std::vector<int>> table;
	std::vector<char> list_of_variables;
	std::vector<int> list_of_function_values;
	int k;

	while (1)
	{
		k = -1;
		input_k.clear();
		input_func.clear();
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
		list_of_variables.clear();
		list_of_function_values.clear();

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

		/*std::cout << "\n\nrpn: ";
		for (auto a : rpn)
		{
			std::cout << a;
		}*/

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