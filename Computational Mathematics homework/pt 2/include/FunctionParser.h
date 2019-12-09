#include <string>
#include <sstream>
#include <vector>
#include <queue>

class FunctionParser {
    public:
    typedef double Numeric;

    FunctionParser(const std::string & formula) {
        formula_steps.push_back(formula);
        size_t index = 0;
        while (index < formula_steps.size()) {
            parse(index);
            index++;
        }
    }

    Numeric call(Numeric x) {
        return calc(x);
    }

    private:
    std::vector<std::string> formula_steps;

    static const char LITERA = '#';
    static std::string literal(size_t index) {
        std::stringstream ss;
        ss << LITERA << index;
        return ss.str();
    }
    static std::string single_literal_operation(size_t index, std::string operation) {
        std::stringstream ss;
        ss << operation << LITERA << index;
        return ss.str();
    }
    static std::string two_literals_operation(size_t index_left, size_t index_right, std::string operation) {
        std::stringstream ss;
        ss << operation << LITERA << index_left << LITERA << index_right;
        return ss.str();
    }

    void handle_single_side_action(const std::string & operation, size_t position, std::string & str) {
        if (position != std::string::npos) {
            size_t action_index = formula_steps.size();
            std::string action = str.substr(position + 1, str.size() - position - 1);
            //std::cout << "DEBUG: " << "\tAdding action at " << action_index << ": " << action << std::endl;
            formula_steps.push_back(action);

            str = single_literal_operation(action_index, "-");
            //std::cout << "DEBUG: " << "\tNew form: " << str << std::endl;
        }
    }
    void handle_two_sides_action(const std::string & operation, size_t position, std::string & str) {
        if (position != std::string::npos) {
            size_t left_action_index = formula_steps.size();
            std::string left_action = str.substr(0, position);
            //std::cout << "DEBUG: " << "\tAdding left action at " << left_action_index << ": " << left_action << std::endl;
            formula_steps.push_back(left_action);

            size_t right_action_index = formula_steps.size();
            std::string right_action = str.substr(position + 1, str.size() - position - 1);
            //std::cout << "DEBUG: " << "\tAdding right action at " << right_action_index << ": " << right_action << std::endl;
            formula_steps.push_back(right_action);

            str = two_literals_operation(left_action_index, right_action_index, operation);
            //std::cout << "DEBUG: " << "\tNew form: " << str << std::endl;
        }
    }
    void parse(size_t index) {
        std::string str = formula_steps[index];
        //std::cout << "DEBUG: " << "Parsing #" << index << ":  " << str << std::endl;
        // BRACKETS "(#)"
        size_t l_bracket_pos = str.find_first_of("(");
        size_t r_bracket_pos = str.find_last_of(")");
        if (l_bracket_pos != std::string::npos && r_bracket_pos != std::string::npos) {
            size_t new_action_index = formula_steps.size();
            std::string new_action = str.substr(l_bracket_pos + 1, r_bracket_pos - l_bracket_pos - 1);
            //std::cout << "DEBUG: " << "\tAdding action at " << new_action_index << ": " << new_action << std::endl;
            formula_steps.push_back(new_action);
            str.replace(l_bracket_pos, r_bracket_pos - l_bracket_pos + 1, literal(new_action_index));
            //std::cout << "DEBUG: " << "\tNew form: " << str << std::endl;
        }

        size_t position = std::string::npos;
        std::string operation = "";

        // COS "COS#"
        operation = "COS";
        position = str.find_first_of(operation);
        handle_single_side_action(operation, position, str);
        // SIN "SIN#"
        operation = "SIN";
        position = str.find_first_of(operation);
        handle_single_side_action(operation, position, str);
        // LN "LN#"
        operation = "LN";
        position = str.find_first_of(operation);
        handle_single_side_action(operation, position, str);

        // ADDITION "#+#"
        operation = "+";
        position = str.find_first_of(operation);
        handle_two_sides_action(operation, position, str);
        // SUBTRACTION "#-#"
        operation = "-";
        position = str.find_last_of(operation);
        handle_two_sides_action(operation, position, str);

        // MULTIPLICATION "#*#"
        operation = "*";
        position = str.find_first_of(operation);
        handle_two_sides_action(operation, position, str);
        // DIVISION "#/#"
        operation = "/";
        position = str.find_last_of(operation);
        handle_two_sides_action(operation, position, str);

        // EXPONENTIATION "#^#"
        operation = "^";
        position = str.find_first_of(operation);
        handle_two_sides_action(operation, position, str);

        //std::cout << "DEBUG: " << "Parsed  #" << index << ":  " << str << std::endl;
        formula_steps[index] = str;
    }

    enum calc_status { DONE, IN_QUEUE, NOT_DONE };
    Numeric calc(Numeric x, size_t index = 0) {
        std::vector<Numeric> nums(formula_steps.size());
        std::vector<calc_status> nums_status(formula_steps.size(), NOT_DONE);
        std::queue<size_t> calculation_queue;
        nums_status[index] = IN_QUEUE;
        calculation_queue.push(index);
        while (nums_status[index] != DONE) {
            if (calculation_queue.empty()) {
                std::cerr << "Unable to calculate #" << index << std::endl;
                return 0;
            }
            size_t i = calculation_queue.front();
            calculation_queue.pop();
            if (nums_status[i] == DONE) {
                continue;
            }
            nums_status[i] = NOT_DONE;
            const std::string & step = formula_steps[i];
            //std::cout << "DEBUG: " << "Calculation queue size: " << calculation_queue.size() << std::endl;
            //std::cout << "DEBUG: " << "Calculating #" << i << ":  " << step << std::endl;
            Numeric val = 0;
            if (step == "") {
                val = 0;
            } else if (step == "E") {
                val = exp(1);
            } else if (step == "PI") {
                val = acos(-1);
            } else if (step == "x") {
                val = x;
            } else {
                // Calculating operation
                size_t first_litera_position = step.find_first_of(LITERA);
                if (first_litera_position == std::string::npos) {
                    val = atof(step.c_str());
                } else {
                    std::string operation = step.substr(0, first_litera_position);
                    Numeric first_operand, second_operand;
                    size_t second_litera_position = step.find_first_of(LITERA, first_litera_position + 1);
                    if (second_litera_position != std::string::npos) {
                        size_t first_operand_index = atoi(step.substr(first_litera_position + 1, second_litera_position - first_litera_position - 1).c_str());
                        size_t second_operand_index = atoi(step.substr(second_litera_position + 1, step.size() - second_litera_position).c_str());
                        if (nums_status[first_operand_index] != DONE || nums_status[second_operand_index] != DONE) {
                            //std::cout << "DEBUG: " << "Operand #" << i << " depends on something else. Will get to it later." << std::endl;
                            if (nums_status[first_operand_index] == NOT_DONE) {
                                nums_status[first_operand_index] = IN_QUEUE;
                                calculation_queue.push(first_operand_index);
                            }
                            if (nums_status[second_operand_index] == NOT_DONE) {
                                nums_status[second_operand_index] = IN_QUEUE;
                                calculation_queue.push(second_operand_index);
                            }
                            if (nums_status[i] == NOT_DONE) {
                                nums_status[i] = IN_QUEUE;
                                calculation_queue.push(i);
                            }
                            continue;
                        }
                        first_operand = nums[first_operand_index];
                        second_operand = nums[second_operand_index];
                    } else {
                        size_t first_operand_index = atoi(step.substr(first_litera_position + 1, step.size() - first_litera_position).c_str());
                        if (nums_status[first_operand_index] != DONE) {
                            //std::cout << "DEBUG: " << "Operand #" << i << " depends on something else. Will get to it later." << std::endl;
                            if (nums_status[first_operand_index] == NOT_DONE) {
                                nums_status[first_operand_index] = IN_QUEUE;
                                calculation_queue.push(first_operand_index);
                            }
                            if (nums_status[i] == NOT_DONE) {
                                nums_status[i] = IN_QUEUE;
                                calculation_queue.push(i);
                            }
                            continue;
                        }
                        first_operand = nums[first_operand_index];
                        second_operand = 0;
                    }
                    if (operation == "^") {
                        val = pow(first_operand, second_operand);
                    } else if (operation == "*") {
                        val = first_operand * second_operand;
                    } else if (operation == "/") {
                        val = first_operand / second_operand;
                    } else if (operation == "+") {
                        val = first_operand + second_operand;
                    } else if (operation == "-") {
                        val = first_operand - second_operand;
                    } else if (operation == "cos") {
                        val = cos(first_operand);
                    } else if (operation == "sin") {
                        val = sin(first_operand);
                    } else if (operation == "ln") {
                        val = log(first_operand);
                    } else if (operation == "") {
                        val = first_operand;
                    } else {
                        std::cerr << "Unknown operation " << operation << std::endl;
                        val = 0;
                    }
                }
            }
            //std::cout << "DEBUG: " << "Calculated  #" << i << ":  " << val << std::endl;
            nums[i] = val;
            nums_status[i] = DONE;
        }
        return nums[index];
    }
};
