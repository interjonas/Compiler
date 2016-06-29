#include <iostream>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "parser.h"
#include "compiler.h"

using namespace std;

LexicalAnalyzer lexer;
ValueNode list[20];
int counter = 0;
struct StatementNode *head = NULL;
struct StatementNode *current = head;
struct StatementNode* body();

void test()
{
	cout << "ENTERING TEST" << endl;
	struct StatementNode *test = new struct StatementNode;
	struct ValueNode *test2 = new struct ValueNode;
	struct ValueNode test3;
	test2 = &list[0];
	test2->value = 5;
	cout << "TEST A: " <<test2->value << endl;
	//cout << "TEST B: " << test3.value << endl;
	test2->value = 99;
	struct AssignmentStatement *please = new struct AssignmentStatement;
	please->operand1 = test2;
	test->assign_stmt = please;
	head = test;
	cout << "TEST A: " << test2->value << endl;
	//cout << "TEST B: " << test3.value << endl;
	cout << "TEST C: " << please->operand1->value << endl;
	cout << "TEST D: " << test->assign_stmt->operand1->value << endl;
	cout << "TEST E: " << head->assign_stmt->operand1->value << endl;
	test2->value = 77;
	cout << "TEST A: " << test2->value << endl;
	//cout << "TEST B: " << test3.value << endl;
	cout << "TEST C: " << please->operand1->value << endl;
	cout << "TEST D: " << test->assign_stmt->operand1->value << endl;
	cout << "TEST E: " << head->assign_stmt->operand1->value << endl;
	cout << "EXITING TEST" << endl;
}
struct StatementNode* print_stmt()
{
	struct StatementNode *state = new struct StatementNode;
	struct PrintStatement *print = new struct PrintStatement;
	struct ValueNode *val = new struct ValueNode;
	state->type = PRINT_STMT;
	Token token1 = lexer.GetToken();
	if (token1.token_type==PRINT)
	{
		Token token2 = lexer.GetToken();
		if (token2.token_type==ID)
		{
			for (int i = 0; i < counter; i++)
			{
				if (token2.lexeme == list[i].name)
				{
					val = &list[i];
					print->id = val;
					state->print_stmt = print;
					Token token3 = lexer.GetToken();
					if (token3.token_type == SEMICOLON)
					{
						//cout << "NAME: " << state->print_stmt->id->name << endl;
						//cout << "VALUE: " << state->print_stmt->id->value << endl;

						return state;
					}
				}
			}
		}
	}
	return state;
}
struct StatementNode* assign_stmt()
{
	//cout << "ENTERING ASSIGNMENT" << endl;
	struct AssignmentStatement *assign = new struct AssignmentStatement;
	struct StatementNode *state = new struct StatementNode;
	state->type = ASSIGN_STMT;
	Token token1=lexer.GetToken();
	Token token2=lexer.GetToken();
	if (token1.token_type == ID && token2.token_type == EQUAL)
	{
		for (int i = 0; i < counter; i++)
		{
			if (list[i].name == token1.lexeme)
			{
				struct ValueNode *left = new struct ValueNode;
				left = &list[i];
				assign->left_hand_side=left;
			}
		}
		Token token3 = lexer.GetToken();
		if (token3.token_type == ID)
		{
			for (int i = 0; i < counter; i++)
			{
				if (list[i].name == token3.lexeme)
				{
					struct ValueNode *operand1 = new struct ValueNode;
					operand1 = &list[i];
					assign->operand1=operand1;
				}
			}
		}
		if (token3.token_type==NUM)
		{
			int val = stoi(token3.lexeme);
			struct ValueNode *operand1 = new struct ValueNode;
			operand1->value = val;
			assign->operand1 = operand1;
		}
		Token token4 = lexer.GetToken();
		if (token4.token_type == SEMICOLON)
		{
			assign->op = OPERATOR_NONE;
		}
		else if (token4.token_type == PLUS || token4.token_type == MINUS || token4.token_type == MULT || token4.token_type == DIV)
		{
			if (token4.token_type==PLUS)
			{
				assign->op = OPERATOR_PLUS;
			}
			if (token4.token_type == MINUS)
			{
				assign->op = OPERATOR_MINUS;
			}
			if (token4.token_type == MULT)
			{
				assign->op = OPERATOR_MULT;
			}
			if (token4.token_type == DIV)
			{
				assign->op = OPERATOR_DIV;
			}
			Token token5 = lexer.GetToken();
			if (token5.token_type == ID)
			{
				for (int i = 0; i < counter; i++)
				{
					if (list[i].name == token5.lexeme)
					{
						struct ValueNode *operand2 = new struct ValueNode;
						operand2 = &list[i];
						assign->operand2 = operand2;
					}
				}
			}
			if (token5.token_type == NUM)
			{
				int val = stoi(token5.lexeme);
				struct ValueNode *operand2 = new struct ValueNode;
				operand2->value = val;
				assign->operand2 = operand2;
			}
			Token token6 = lexer.GetToken();
			if (token6.token_type == SEMICOLON)
			{

			}
		}
		else
		{
			assign->op = OPERATOR_NONE;
			lexer.UngetToken(token4);
		}
	}
	state->assign_stmt = assign;
	return state;
	//cout << "EXITING ASSIGNMENT" << endl;
}
struct StatementNode* stmt()
{
	//cout << "ENTERING STMT" << endl;
	struct StatementNode* st;
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == ID)
	{
		lexer.UngetToken(token1);
		st=assign_stmt();
		return st;
		//test();
	}
	if (token1.token_type == PRINT)
	{
		lexer.UngetToken(token1);
		st=print_stmt();
		return st;
		//test();
	}
	if (token1.token_type == IF)
	{
		struct StatementNode *state = new struct StatementNode;
		struct IfStatement *if_node = new struct IfStatement;
		state->type = IF_STMT;
		state->if_stmt = if_node;

		Token token2 = lexer.GetToken();
		if (token2.token_type == ID)
		{
			for (int i = 0; i < counter; i++)
			{
				if (list[i].name == token2.lexeme)
				{
					struct ValueNode *operand1 = new struct ValueNode;
					operand1 = &list[i];
					if_node->condition_operand1 = operand1;
				}
			}
		}
		if (token2.token_type == NUM)
		{
			int val = stoi(token2.lexeme);
			struct ValueNode *operand1 = new struct ValueNode;
			operand1->value = val;
			if_node->condition_operand1 = operand1;
		}

		Token token3 = lexer.GetToken();
		if (token3.token_type==GREATER)
		{
			if_node->condition_op = CONDITION_GREATER;
		}
		if (token3.token_type == LESS)
		{
			if_node->condition_op = CONDITION_LESS;
		}
		if (token3.token_type == NOTEQUAL)
		{
			if_node->condition_op = CONDITION_NOTEQUAL;
		}

		Token token4 = lexer.GetToken();
		if (token4.token_type == ID)
		{
			for (int i = 0; i < counter; i++)
			{
				if (list[i].name == token4.lexeme)
				{
					struct ValueNode *operand2 = new struct ValueNode;
					operand2 = &list[i];
					if_node->condition_operand2 = operand2;
				}
			}
		}
		if (token4.token_type == NUM)
		{
			int val = stoi(token4.lexeme);
			struct ValueNode *operand2 = new struct ValueNode;
			operand2->value = val;
			if_node->condition_operand2 = operand2;
		}

		if_node->true_branch = body();
		struct StatementNode *noop = new struct StatementNode;
		noop->type = NOOP_STMT;
		noop->next = NULL;
		struct IfStatement *tmp = new struct IfStatement;
		tmp->true_branch = if_node->true_branch;
		while (if_node->true_branch->next != NULL)
		{
			if_node->true_branch = if_node->true_branch->next;
		}
		if_node->true_branch->next = noop;
		if_node->true_branch = tmp->true_branch;
		if_node->false_branch = noop;
		state->next = noop;
		st = state;
		return st;
	}
	if (token1.token_type == WHILE)
	{
		struct StatementNode *state = new struct StatementNode;
		struct IfStatement *if_node = new struct IfStatement;
		state->type = IF_STMT;
		state->if_stmt = if_node;

		Token token2 = lexer.GetToken();
		if (token2.token_type == ID)
		{
			for (int i = 0; i < counter; i++)
			{
				if (list[i].name == token2.lexeme)
				{
					struct ValueNode *operand1 = new struct ValueNode;
					operand1 = &list[i];
					if_node->condition_operand1 = operand1;
				}
			}
		}
		if (token2.token_type == NUM)
		{
			int val = stoi(token2.lexeme);
			struct ValueNode *operand1 = new struct ValueNode;
			operand1->value = val;
			if_node->condition_operand1 = operand1;
		}

		Token token3 = lexer.GetToken();
		if (token3.token_type == GREATER)
		{
			if_node->condition_op = CONDITION_GREATER;
		}
		if (token3.token_type == LESS)
		{
			if_node->condition_op = CONDITION_LESS;
		}
		if (token3.token_type == NOTEQUAL)
		{
			if_node->condition_op = CONDITION_NOTEQUAL;
		}

		Token token4 = lexer.GetToken();
		if (token4.token_type == ID)
		{
			for (int i = 0; i < counter; i++)
			{
				if (list[i].name == token4.lexeme)
				{
					struct ValueNode *operand2 = new struct ValueNode;
					operand2 = &list[i];
					if_node->condition_operand2 = operand2;
				}
			}
		}
		if (token4.token_type == NUM)
		{
			int val = stoi(token4.lexeme);
			struct ValueNode *operand2 = new struct ValueNode;
			operand2->value = val;
			if_node->condition_operand2 = operand2;
		}

		if_node->true_branch = body();
		struct StatementNode *gt = new struct StatementNode;
		gt->type = GOTO_STMT;
		struct GotoStatement *goto_node = new struct GotoStatement;
		gt->goto_stmt = goto_node;
		goto_node->target = state;

		struct StatementNode *noop = new struct StatementNode;
		noop->type = NOOP_STMT;
		while (if_node->true_branch->next != NULL)
		{
			if_node->true_branch = if_node->true_branch->next;
		}
		if_node->true_branch->next = gt;
		if_node->false_branch = noop;
		state->next = noop;
		st = state;
		return st;
	}
	else
	{
		st = NULL;
		return st;
	}
	//cout << "EXITING STMT" << endl;
}
struct StatementNode* stmt_list()
{
	//cout << "ENTERING STMT LIST" << endl;
	struct StatementNode* st;
	struct StatementNode* stList;
	st = stmt();
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == ID || token1.token_type == PRINT || token1.token_type == IF || token1.token_type == SWITCH || token1.token_type == WHILE)
	{
		lexer.UngetToken(token1);
		stList=stmt_list();
		if (st->type == IF_STMT)
		{
			//st->next->next = stList;
			st->if_stmt->false_branch->next = stList;
		}
		else
		{
			st->next = stList;
		}
		return st;
	}
	else
	{
		lexer.UngetToken(token1);
		st->next = NULL;
		return st;
	}
	//cout << "EXITING STMT LIST" << endl;
}
struct StatementNode* body()
{
	//cout << "ENTERING BODY" << endl;
	struct StatementNode *st1;
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == LBRACE)
	{
		st1=stmt_list();
		token1 = lexer.GetToken();
		if (token1.token_type == RBRACE)
		{
			return st1;
		}
		else
		{
			st1 = NULL;
			return st1;
		}
	}
	else
	{
		st1 = NULL;
		return st1;
	}
	//cout << "EXITING BODY" << endl;
}
void id_list()
{
	//cout << "ENTERING ID LIST" << endl;
	Token token1;
	token1 = lexer.GetToken();
	list[counter].name = token1.lexeme;
	list[counter].value = 0;
	counter++;
	token1 = lexer.GetToken();
	if (token1.token_type == COMMA)
	{
		id_list();
	}
	else
	{
		lexer.UngetToken(token1);
	}
	//cout << "EXITING ID LIST" << endl;
}
void var_section()
{
	//cout << "ENTERING VAR SECTION" << endl;
	id_list();
	Token token1;
	token1 = lexer.GetToken();
	if (token1.token_type == SEMICOLON)
	{
		//cout << "DONE" << endl;
	}
	else
	{
		cout << "ERROR" << endl;
	}
	//cout << "EXITING VAR SECTION" << endl;
}
struct StatementNode* program()
{
	//cout << "ENTERING PROGRAM" << endl;
	struct StatementNode *head;
	var_section();
	head=body();
	return head;
	//cout << "EXITING PROGRAM" << endl;
}
struct StatementNode * parse_generate_intermediate_representation()
{
	return program();
}
/*
int main()
{
	program();

	LexicalAnalyzer lexer;
	Token token;
	while (token.token_type != END_OF_FILE)
	{
		token = lexer.GetToken();
		//token.Print();
	}
	int x;
	cin >> x;
}
*/


