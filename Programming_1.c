#include<stdio.h>
#include<stdlib.h>
#define UTF8_space_1 194
#define UTF8_space_2 160

struct node{
	int key;
	struct node* left;
	struct node* right;
	int priority;
};
typedef struct node S_node;

struct node_linked_list{
	int next_assigned;
	struct node* key;
	struct node_linked_list* next;
};
typedef struct node_linked_list S_node_linked_list;


S_node* Create_node();
void Copy_node(struct node*, struct node*);
void Standard_push(struct node*, int);
int Splay_push(struct node*, int, int, int);
void Left_rotate(struct node*);
void Right_rotate(struct node*);
int Print_line(struct node*, struct node*, int, FILE*);
void Print_tree_and_left_boundary(struct node*, FILE*, FILE*);
void Set_priority_and_left_boundary(struct node*, int*, S_node_linked_list*);
S_node_linked_list* Create_node_linked_list(struct node*);
void Spacing(int, FILE*);
int Num_length(int);
void Destory_tree(struct node*);

int main(){
	int input,input_char;
	int i;
	//Open  files
	FILE *f_input = fopen("input.txt", "r");
	FILE *f_BTree = fopen("BTree.txt", "w");
	FILE *f_BTree_PRep = fopen("BTree_PRep.txt", "w");
	FILE *f_BTree_boundary = fopen("BTree_boundary.txt", "w");
	FILE *f_STree = fopen("STree.txt", "w");
	FILE *f_STree_PRep = fopen("STree_PRep.txt", "w");
	FILE *f_STree_boundary = fopen("STree_boundary.txt", "w");
	
	//Write files
	for(i=0;i<100;i++){
		//Tree construction
		struct node* t_standard = Create_node();
		struct node* t_splay = Create_node();
		input = 0;
		do{
			input_char = fgetc(f_input);
			if(input_char == UTF8_space_1){
				input_char = fgetc(f_input);
			}
			if(input_char==UTF8_space_2 || input_char=='\n'){
				Standard_push(t_standard, input);
				Splay_push(t_splay, input, 0, 0);
				input = 0;
				continue;
			}
			input = input*10+(input_char-'0');
		}while(input_char != '\n');
		//Write output files
		Print_line(t_standard, t_standard, 0, f_BTree_PRep);
		Print_line(t_splay, t_splay, 0, f_STree_PRep);
		Print_tree_and_left_boundary(t_standard, f_BTree, f_BTree_boundary);
		Print_tree_and_left_boundary(t_splay, f_STree, f_STree_boundary);
		//Free spaces
		Destory_tree(t_standard);
		Destory_tree(t_splay);
	}
	
	//Close files
	fclose(f_input);
	fclose(f_BTree);
	fclose(f_BTree_PRep);
	fclose(f_BTree_boundary);
	fclose(f_STree);
	fclose(f_STree_PRep);
	fclose(f_STree_boundary);
	return 0;
}

S_node* Create_node(){
	struct node* new_node = (struct node*)malloc(sizeof(struct node));
	new_node->key = 0;
	return new_node;
}

void Copy_node(struct node* x, struct node* y){
	x->key = y->key;
	if(x->key != 0){
		x->left = y->left;
		x->right = y->right;
	}
	return ;
}

void Standard_push(struct node* current_node, int input){
	if(current_node->key == 0){
		current_node->key = input;
		current_node->left = Create_node();
		current_node->right = Create_node();
		return ;
	}
	if(input < current_node->key){
		Standard_push(current_node->left, input);
	}
	else{
		Standard_push(current_node->right, input);
	}
	return ;
}

int Splay_push(struct node* current_node, int input, int direction, int depth){
	if(current_node->key == 0){
		current_node->key = input;
		current_node->left = Create_node();
		current_node->right = Create_node();
		return 1;
	}
	if(input < current_node->key){
		int rotate = Splay_push(current_node->left, input, 1, depth+1);
		if(rotate == 2){
			Right_rotate(current_node);
			Right_rotate(current_node);
			return 1;
		}
		if(rotate == 3){
			Left_rotate(current_node->left);
			Right_rotate(current_node);
			return 1;
		}
		if(depth == 0){
			Right_rotate(current_node);
			return 0;
		}
		if(direction == 1){
			return 2;
		}
		if(direction == 2){
			return 3;
		}
	}
	else{
		int rotate = Splay_push(current_node->right, input, 2, depth+1);
		if(rotate == 2){
			Left_rotate(current_node);
			Left_rotate(current_node);
			return 1;
		}
		if(rotate == 3){
			Right_rotate(current_node->right);
			Left_rotate(current_node);
			return 1;
		}
		if(depth == 0){
			Left_rotate(current_node);
			return 0;
		}
		if(direction == 2){
			return 2;
		}
		if(direction == 1){
			return 3;
		}
	}
}

void Left_rotate(struct node* current_node){
	int x_key = current_node->key;
	int y_key = current_node->right->key;
	struct node* a = Create_node();
	Copy_node(a, current_node->left);
	free(current_node->left);
	struct node* b = current_node->right->left;
	struct node* c = current_node->right->right;
	current_node->key = y_key;
	current_node->left = current_node->right;
	current_node->right = c;
	current_node->left->key = x_key;
	current_node->left->left = a;
	current_node->left->right = b;
	return ;
}

void Right_rotate(struct node* current_node){
	int x_key = current_node->left->key;
	int y_key = current_node->key;
	struct node* a = current_node->left->left;
	struct node* b = current_node->left->right;
	struct node* c = Create_node();
	Copy_node(c, current_node->right);
	free(current_node->right);
	current_node->key = x_key;
	current_node->right = current_node->left;
	current_node->left = a;
	current_node->right->key = y_key;
	current_node->right->left = b;
	current_node->right->right = c;
	return ;
}

int Print_line(struct node* current_node, struct node* root, int space, FILE* f_PRep){
	if(current_node->key == 0){
		if(space == 1){
			Spacing(1, f_PRep);
		}
		fprintf(f_PRep, "-");
		return 1;
	}
	else{
		if(space == 1){
			Spacing(1, f_PRep);
		}
		fprintf(f_PRep, "%d", current_node->key);
		space = 1;
	}
	if((current_node->left->key == 0) && (current_node->right->key == 0) ){
		return space;
	}
	fprintf(f_PRep, "(");
	space = Print_line(current_node->left, root, 0, f_PRep);
	space = Print_line(current_node->right, root, space, f_PRep);
	fprintf(f_PRep, ")");
	if(current_node == root){
		fprintf(f_PRep, "\n");
	}
	return space;
}

void Print_tree_and_left_boundary(struct node* root, FILE* f_Tree, FILE* f_Tree_boundary){
	int num = 1;
	struct node_linked_list* left_boundary = Create_node_linked_list(root);
	Set_priority_and_left_boundary(root, &num, left_boundary);
	struct node_linked_list* current_list = Create_node_linked_list(root);
	struct node_linked_list* tail_list = current_list;
	struct node_linked_list* discard;
	int cursor;
	do{
		if(current_list->next_assigned != 0){
			discard = current_list;
			current_list = current_list->next;
			free(discard);
		}
		if(current_list->key == left_boundary->key){
			fprintf(f_Tree_boundary, "%d", left_boundary->key->key);
			if(left_boundary->next_assigned != 0){
				Spacing(1, f_Tree_boundary);
				discard = left_boundary;
				left_boundary = left_boundary->next;
				free(discard);
			}
			if(current_list->key != root){
				fprintf(f_Tree, "\n");
			}
			cursor = 0;
		}
		Spacing((current_list->key->priority-1)*3-cursor, f_Tree);
		fprintf(f_Tree, "%d", current_list->key->key);
		cursor = (current_list->key->priority-1)*3+Num_length(current_list->key->key);
		if(current_list->key->left->key != 0){
			tail_list->next = Create_node_linked_list(current_list->key->left);
			tail_list->next_assigned = 1;
			tail_list = tail_list->next;
		}
		if(current_list->key->right->key != 0){
			tail_list->next = Create_node_linked_list(current_list->key->right);
			tail_list->next_assigned = 1;
			tail_list = tail_list->next;
		}
	}while(current_list->next_assigned != 0);
	free(left_boundary);
	free(current_list);
	fprintf(f_Tree, "\n");
	fprintf(f_Tree_boundary, "\n");
	return ;
}

void Set_priority_and_left_boundary(struct node* current_node, int* num, struct node_linked_list* left_boundary){
	if(current_node->left->key != 0){
		if(left_boundary->next_assigned == 0){
			left_boundary->next = Create_node_linked_list(current_node->left);
			left_boundary->next_assigned = 1;
		}
		Set_priority_and_left_boundary(current_node->left, num, left_boundary->next);
	}
	current_node->priority = *num;
	*num = *num+1;
	if(current_node->right->key != 0){
		if(left_boundary->next_assigned == 0){
			left_boundary->next = Create_node_linked_list(current_node->right);
			left_boundary->next_assigned = 1;
		}
		Set_priority_and_left_boundary(current_node->right, num, left_boundary->next);
	}
	return ;
}

S_node_linked_list* Create_node_linked_list(struct node* x){
	struct node_linked_list* new_list = (struct node_linked_list*)malloc(sizeof(struct node_linked_list));
	new_list->next_assigned = 0;
	new_list->key = x;
	return new_list;
}

void Spacing(int num, FILE* f){
	int i;
	for(i=0;i<num;i++){
		fprintf(f, "%c%c", UTF8_space_1, UTF8_space_2);
	}
	return ;
}

int Num_length(int num){
	int ans = 0;
	while(num!=0){
		num/=10;
		ans++;
	}
	return ans;
}

void Destory_tree(struct node* current_node){
	if(current_node->key == 0){
		free(current_node);
		return ;
	}
	Destory_tree(current_node->left);
	Destory_tree(current_node->right);
	free(current_node);
	return ;
}