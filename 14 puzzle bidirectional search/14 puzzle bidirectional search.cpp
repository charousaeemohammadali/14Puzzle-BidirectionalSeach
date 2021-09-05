#include <iostream>
using namespace std;

struct node {
	node* parent;
	int s[4][4];
	char action;
};

struct listnode {
	node* n;
	listnode* next;
};

//breaking the while loop in the main
bool stop = false;

//tedad amal ha
int movescounter = 1;
listnode* startfrontier = NULL;
listnode* startexplored = NULL;
listnode* goalfrontier = NULL;
listnode* goalexplored = NULL;

listnode* insert(listnode*, node* n);
node* create_initial_node();
node* create_goal_node();
void free_blocks(node*, int[2][2]);
listnode* expand(node*, listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2);
listnode* move_up(node*, int[2], listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2);
listnode* move_down(node*, int[2], listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2);
listnode* move_left(node*, int[2], listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2);
listnode* move_right(node*, int[2], listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2);
bool check_goal(node*);
node* search(node*, listnode*);
listnode* add_frontier(node*);
listnode* delete_first_frontier(listnode* frontier);
listnode* add_explored(node*);
void print_output(node*);
void print_output2(node*);

int main()
{
	startfrontier = insert(startfrontier, create_initial_node());
	goalfrontier = insert(goalfrontier, create_goal_node());
	while (true)
	{
		if (startfrontier == NULL)
		{
			cout << "FRONTIER IS EMPTY , SOLUTION FAILED ... " << endl;
			system("pause");
			return 0;
		}
		//save the memory of frontier
		node* frontier_temp;
		frontier_temp = startfrontier->n;

		//remove from frontier
		startfrontier = delete_first_frontier(startfrontier);

		//frontier->n > s; add to explored
		startexplored = insert(startexplored, frontier_temp);

		//kole farzandan add frontier
		startfrontier = expand(frontier_temp, startfrontier, startexplored, goalfrontier, goalexplored);
		if (stop)
			break;

		if (goalfrontier == NULL)
		{
			cout << "FRONTIER IS EMPTY , SOLUTION FAILED ... " << endl;
			system("pause");
			return 0;
		}
		//save the memory of frontier
		node* goal_temp;
		goal_temp = goalfrontier->n;

		//remove from frontier
		goalfrontier = delete_first_frontier(goalfrontier);

		//frontier->n > s; add to explored
		goalexplored = insert(goalexplored, goal_temp);

		//kole farzandan add frontier
		goalfrontier = expand(goal_temp, goalfrontier, goalexplored, startfrontier, startexplored);
		if (stop)
			break;
	}
	system("pause");
	return 0;
}

listnode* insert(listnode* first, node* n)
{
	listnode* temp = new listnode;
	temp->n = n;
	temp->next = NULL;
	if (!first)
		return temp;
	listnode* last = first;
	while (last->next)
		last = last->next;
	last->next = temp;
	return first;
}

node* create_initial_node()
{
	node* n = new node;
	int matrix[4][4] = { { 1, 2, 3, 4 },{ 0,6,7,8 },{ 5,10,11,12 },{ 9,0,14,13 } };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			n->s[i][j] = matrix[i][j];
	n->parent = NULL;
	return n;
}

node* create_goal_node()
{
	node* n = new node;
	int matrix[4][4] = { { 1, 2, 3, 4 },{ 5,6,7,8 },{ 9,10,11,12 },{ 13,14,0,0 } };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			n->s[i][j] = matrix[i][j];
	n->parent = NULL;
	return n;
}

void free_blocks(node* s1, int location[2][2])
{
	int flag = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (s1->s[i][j] == 0)
			{
				location[flag][0] = i;
				location[flag][1] = j;
				flag++;
			}
}

listnode* expand(node* L, listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2)
{
	int location[2][2];
	free_blocks(L, location);

	frontier1 = move_left(L, location[0], frontier1, explored1, frontier2, explored2);
	frontier1 = move_right(L, location[0], frontier1, explored1, frontier2, explored2);
	frontier1 = move_up(L, location[0], frontier1, explored1, frontier2, explored2);
	frontier1 = move_down(L, location[0], frontier1, explored1, frontier2, explored2);

	frontier1 = move_left(L, location[1], frontier1, explored1, frontier2, explored2);
	frontier1 = move_right(L, location[1], frontier1, explored1, frontier2, explored2);
	frontier1 = move_up(L, location[1], frontier1, explored1, frontier2, explored2);
	frontier1 = move_down(L, location[1], frontier1, explored1, frontier2, explored2);
	return frontier1;
}

listnode* move_up(node* p, int location[2], listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2)
{
	int i = location[0];
	int j = location[1];
	if (i == 0 || stop == true) return frontier1;
	node* child = new node;
	child->parent = p;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			child->s[x][y] = p->s[x][y];
	int v = child->s[i][j];
	child->s[i][j] = child->s[i - 1][j];
	child->s[i - 1][j] = v;
	child->action = 'U';
	if (!search(child, frontier1) && !search(child, explored1) && !search(child, frontier2))
		frontier1 = insert(frontier1, child);
	node* joint = search(child, frontier2);
	if (joint)
	{
		if (frontier1 == startfrontier)
		{
			print_output(child);
			print_output2(joint);
		}
		else
		{
			print_output(joint);
			print_output2(child);
		}
		stop = true;
	}
	return frontier1;
}

listnode* move_down(node* p, int location[2], listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2)
{
	int i = location[0];
	int j = location[1];
	if (i == 3 || stop == true) return frontier1;
	node* child = new node;
	child->parent = p;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			child->s[x][y] = p->s[x][y];
	int v = child->s[i][j];
	child->s[i][j] = child->s[i + 1][j];
	child->s[i + 1][j] = v;
	child->action = 'D';
	if (!search(child, frontier1) && !search(child, explored1) && !search(child, frontier2))
		frontier1 = insert(frontier1, child);
	node* joint = search(child, frontier2);
	if (joint)
	{
		if (frontier1 == startfrontier)
		{
			print_output(child);
			print_output2(joint);
		}
		else
		{
			print_output(joint);
			print_output2(child);
		}
		stop = true;
	}
	return frontier1;
}

listnode* move_left(node* p, int location[2], listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2)
{
	int i = location[0];
	int j = location[1];
	if (j == 0 || stop == true) return frontier1;
	node* child = new node;
	child->parent = p;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			child->s[x][y] = p->s[x][y];
	int v = child->s[i][j];
	child->s[i][j] = child->s[i][j - 1];
	child->s[i][j - 1] = v;
	child->action = 'L';
	if (!search(child, frontier1) && !search(child, explored1) && !search(child, frontier2))
		frontier1 = insert(frontier1, child);
	node* joint = search(child, frontier2);
	if (joint)
	{
		if (frontier1 == startfrontier)
		{
			print_output(child);
			print_output2(joint);
		}
		else
		{
			print_output(joint);
			print_output2(child);
		}
		stop = true;
	}
	return frontier1;
}

listnode* move_right(node* p, int location[2], listnode* frontier1, listnode* explored1, listnode* frontier2, listnode* explored2)
{
	int i = location[0];
	int j = location[1];
	if (j == 3 || stop == true) return frontier1;
	node* child = new node;
	child->parent = p;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			child->s[x][y] = p->s[x][y];
	int v = child->s[i][j];
	child->s[i][j] = child->s[i][j + 1];
	child->s[i][j + 1] = v;
	child->action = 'R';
	if (!search(child, frontier1) && !search(child, explored1) && !search(child, frontier2))
		frontier1 = insert(frontier1, child);
	node* joint = search(child, frontier2);
	if (joint)
	{
		if (frontier1 == startfrontier)
		{
			print_output(child);
			print_output2(joint);
		}
		else
		{
			print_output(joint);
			print_output2(child);
		}
		stop = true;
	}
	return frontier1;
}

bool check_goal(node* s1)
{
	int goal_matrix[4][4] = { { 1, 2, 3, 4 },{ 5,6,7,8 },{ 9,10,11,12 },{ 13,14,0,0 } };
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (s1->s[i][j] != goal_matrix[i][j])
				return false;
	return true;
}

bool compare(int m1[4][4], int m2[4][4])
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (m1[i][j] != m2[i][j])
				return false;
	return true;
}

node* search(node* n, listnode* first)
{
	while (first != NULL)
	{
		if (compare(n->s, first->n->s))
			return first->n;
		first = first->next;
	}
	return NULL;
}

listnode* delete_first_frontier(listnode* frontier)
{
	listnode* temp;
	temp = frontier;
	frontier = frontier->next;
	delete temp;
	return frontier;
}

void print_output(node* joint)
{
	listnode* temp = startfrontier;
	node* temp2;
	temp2 = joint;
	listnode* reverse = new listnode;
	reverse->n = temp2;
	reverse->next = NULL;
	temp2 = temp2->parent;
	while (temp2)
	{
		listnode* temp = new listnode;
		temp->n = temp2;
		temp->next = reverse;
		reverse = temp;
		temp2 = temp2->parent;
	}
	while (reverse)
	{
		cout << movescounter << endl;
		if (movescounter != 1)
			cout << "action = " << reverse->n->action << endl;
		movescounter++;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				cout << reverse->n->s[i][j] << " ";
			cout << endl;
		}
		cout << "-------------------" << endl << endl;
		reverse = reverse->next;
	}
}

void print_output2(node* joint)
{
	listnode* temp;
	temp = new listnode;
	temp->n = joint->parent;
	node* myaction = joint;
	node* temp2 = temp->n;
	while (temp2)
	{

		if (myaction->action == 'U') myaction->action = 'D';
		else if (myaction->action == 'D') myaction->action = 'U';
		else if (myaction->action == 'L') myaction->action = 'R';
		else if (myaction->action == 'R') myaction->action = 'L';

		cout << movescounter << endl;
		movescounter++;
		if (movescounter != 1)
			cout << "action = " << myaction->action << endl;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
				cout << temp2->s[i][j] << " ";
			cout << endl;
		}
		cout << "-------------------" << endl << endl;
		myaction = myaction->parent;
		temp2 = temp2->parent;
	}
}