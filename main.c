#include <malloc/malloc.h>
//#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define SIZE 20
#define FILENAME "output.txt"

typedef struct Node {
    char * key;
    char * info;
	struct Node * left;
	struct Node * right;
} Node;

Node * root = 0;

int add (char * key, char * info) {
	if (!root) {
		root = calloc(1, sizeof(struct Node));
        root->key = calloc(20, sizeof(char));
		strncpy(root->key, key, strlen(key));
        root->info = calloc(20, sizeof(char));
		root->right = NULL;
		root->left = NULL;
	    strncpy(root->info, info, strlen(info));
        
		return 0;
	}
    
	Node * now = root;
	Node * last = NULL;
	int right = 0;
    
	while (now && now->info) {
        if (strcmp(now->key, key) == 0) {
			return 1;
        }
        
		last = now;
		if (strcmp(key,now->key) > 0) {
			now = now->right;
			right = 1;
		} else {
			now = now->left;
			right = 0;
		}
	}
    
    if (!now) {
		now = calloc(1, sizeof(struct Node));
        
        if (!now) {
			return 2;
        }
        
		now->right = 0;
		now->left = 0;
	}
    
    if (last) {
        if (right) {
			last->right = now;
        } else {
			last->left = now;
        }
	}
    
	now->key = calloc(20, sizeof(char));
	now->info = calloc(20, sizeof(char));
	strcpy(now->info, info);
	strncpy(now->key,key,strlen(key));
	return 0;
}

int rebuild(Node * tree) {
    if (!tree) {
        return 1;
    }
    
    if (tree->left) {
        rebuild(tree->left);
    }
    
    if (tree->right) {
        rebuild(tree->right);
    }
    
	if (tree->info) {
		add(tree->key, tree->info);
		free(tree->info);
	}
    
	free(tree);
	return 0;
}

int rm (char * key) {
    Node * now = root;
	Node * last = NULL;
    
    if (!now) {
		return 1;
    }
    
    do {
    	last = now;
        
        if (strcmp(key, now->key) == 0) {
    		break;
        } else if (strcmp(key, now->key) > 0) {
            now = now->right;
        } else {
            now = now->left;
        }
        
        if (!now) {
            return 1;
        }
        
	}
    
	while (strcmp(key, now->key) != 0);
	
    if (last == now) {
		last = NULL;
    } else {
        if (strcmp(last->key, now->key) < 0) {
			last->right = NULL;
        } else {
			last->left = NULL;
        }
	}
    
	rebuild(now->left);
	rebuild(now->right);
    
    if (now->info) {
		free(now->info);
    }
    
    free(now->key);
	free(now);
	return 0;
}

int find_r (Node * tree, char * key) {
    if (!root) {
		return 1;
    }
    
    if (tree->left) {
		find_r(tree->left, key);
    }
    
    if ((tree->info) && (strncmp(tree->key, key, strlen(key)) == 0)) {
        printf("%s :> %s\n", tree->key, tree->info);
    }
    
    if (tree->right) {
		find_r(tree->right, key);
    }
    
	return 0;
}

int find(char * key, int b) {
	Node * now = root;
    
    if (!now) {
		return 1;
    }
    
    while (strcmp(key,now->key) != 0) {
        if (strcmp(key,now->key) > 0) {
            now = now->right;
        } else {
            now = now->left;
        }
        
        if (!now) {
            return 1;
        }
    }
    
    if (!now->info) {
		return 2;
    }
    
    if (b == 0) {
        printf("%s\n", now->info);
    }
	return 0;
}

int print_all(Node * tree) {
    if (!tree) {
		return 1;
    }
    
    if (tree->left) {
		print_all(tree->left);
    }
    
    if (tree->info) {
		printf("%s :> %s\n", tree->key, tree->info);
    }
    
    if (tree->right) {
		print_all(tree->right);
    }
    
    return 0;
}

int print_all_tab(Node * tree, int depth) {
	int space, depth1, i;
	depth1 = depth;
	space = 0;
    
    if (!tree) {
		return 1;
    }
    
	if (tree->info) {
        for (i = 0; i<depth; i++) {
			printf(" ");
        }
        
		printf("%s :> %s",tree->key, tree->info);
        
        for (i = 0; i<space; i++ ) {
			printf(" ");
        }
        
		space = 2*(depth1-depth);
	}
    
    if (tree->left) {
		print_all(tree->left);
    }
    
    if (tree->right) {
        print_all(tree->right);
    }
    
	return 0;
}

int print_table (Node * tree, char* key) {
    if (!root) {
			return 1;
    }
    
    if (tree->left) {
		print_table(tree->left, key);
    }
    
    if ((!strlen(key)) && (tree->info)) {
		printf("%s :> %s\n", tree->key, tree->info);
    } else {
        if ((tree->info) && (strcmp(tree->key,key) > 0)) {
			printf("%s :> %s\n", tree->key, tree->info);
        }
	}
    
    if (tree->right) {
		print_table(tree->right, key);
    }
    
	return 0;
}

void print_Tree (Node * p, int level) {
    if (p) {
    	print_Tree(p->right, level + 1);
        
        for(int i = 0; i < level; i++) {
			printf("   ");
        }
        
        printf("%s\n", p->key);
        print_Tree(p->left, level + 1);
    }
}

int send_to_file_r (FILE * fp, Node * now) {
    if (!now || !fp) {
		return 0;
    }
    
    char * key1 = calloc(20, sizeof(char));
    char * info1 = calloc(20, sizeof(char));
    char * s1 = calloc(2, sizeof(char));
    
	if (now->info) {
		s1 = strchr(s1, '\n');
		if (s1 != NULL) {
			strncpy(key1, now->key, strlen(now->key)-1);
			printf("%s", now->info);
		} else {
			strncpy(key1,now->key,strlen(now->key));
		}
        
		strncpy(info1,now->info,strlen(now->info)-1);
		fprintf(fp, "%s\n", key1);
		fprintf(fp, "%s\n", info1);
	}
    
    free(s1);
    free(key1);
    free(info1);
    
    if (now->left) {
		send_to_file_r(fp, now->left);
    }
    
    if (now->right) {
		send_to_file_r(fp, now->right);
    }
    
    return 0;
}

int send_to_file() {
    if (!root) {
        return 0;
    }
    
    FILE *fp;
    fp = fopen(FILENAME, "w+");
    send_to_file_r(fp, root);
    
	fclose(fp);
    return 0;
}

int load_from_file () {
	if (access(FILENAME, 0) != -1) {
		FILE * fp;
        fp = fopen(FILENAME, "r");
		size_t len = 20;
        
        if (fp == NULL) {
            fclose(fp);
            return 0;
        }
		
		while (1) {
            char * line = calloc(20, sizeof(char));
			if (!fgets(line, len, fp)) {
                free(line);
				fclose(fp);
				return 0;
			}
            
            char * key = calloc(20, sizeof(char));
			key = strcpy(key, line);
            
            if (strchr(key,'\n') != NULL) {
				key[strlen(key) - 1] = '\0';
            }
            
			if (!fgets(line, len, fp)) {
                free(line);
                free(key);
				fclose(fp);
                
				return 0;
			}
            
            if (strchr(line, '\n') != NULL) {
				line[strlen(line) - 1] = '\0';
            }
            
			add(key, line);
            free(key);
            free(line);
		}
	}
    
    return 0;
}

int free_tree_r (Node * tree) {
    if (!tree) {
		return 0;
    }
    
    if (tree->left) {
		free_tree_r(tree->left);
    }
    
    if (tree->right) {
		free_tree_r(tree->right);
    }
    
    if (tree->info) {
		free(tree->info);
    }
    
	free(tree);
    return 0;
}

int free_tree () {
    if (!root) {
        return 0;
    }
    
	free_tree_r(root);
    return 0;
}

int print_menu () {
    printf("%s\n", "{1} -> Add\t{3} -> Find\t{5} -> Table since Key\t{7} -> Close\n{2} -> Delete\t{4} -> Find (First Digit)\t{6} -> Show");
    printf("%s", "=> ");
    
    return 0;
}

int D_Timing()
{
    int n = 10;
    int k;
    int cnt = 1000000;
    int i;
    int m;
    int key[10000];
    
    
    char line[2] = "a";
    
	clock_t first;
    clock_t last;
    
	srand(time(NULL));
    
    root = NULL;
    
	while (n-- > 0) {
        for (i = 0; i < 10000; ++i) {
			key[i] = rand() % 1000000;
        }

		for (i = 0; i < cnt; ) {
            char * s1 = calloc(200, sizeof(char));
            k = rand() % 1000000;
			sprintf(s1, "%d", k);
            if (add(s1, line)) {
				++i;
            }
            free(s1);
		}
        
        m = 0;
        first = clock();
        for (i = 0; i < 10000; ++i) {
            char * s1 = calloc(200, sizeof(char));
            sprintf(s1,"%d", key[i]);
            if (find(s1, 1) == 0) {
                ++m;
            }
            free(s1);
        }
        
        last = clock();
        printf("%d items was found\n", m);
        printf("test #%d, number of nodes = %d, time = %ld\n", 10 - n, (10 - n) * cnt, last - first);
    
    }
    
	free_tree();
        
	return 1;
}

int action()
{
	char * input = calloc(20, sizeof(char));
    char * key;
	char * s;
    int timing = 1;
    fgets(input, 20, stdin);
    int ret;
	int action_number = atoi(input);
    
	if (action_number < 1 || action_number > 7) {
		free(input);
		return 1;
	}
    
    switch (action_number) {
    
        case 1:
            free(input);
            input = calloc(20, sizeof(char));
            printf("Key :> ");
            fgets(input, 20, stdin);
            key = calloc(20, sizeof(char));
            s = strchr(input, '\n');
        
            if (s != NULL) {
                strncpy(key,input,strlen(input)-1);
            } else {
                strncpy(key,input,strlen(input));
            }
        
            printf("Info :> ");
            fgets(input, 20, stdin);
            ret = add(key, input);
        
            if (ret == 1) {
                printf("[-] Error! Already exists!\n");
            }
        
            if (ret == 2) {
                printf("[-] Error! Lack of memory\n");
            }

            free(key);
            break;
    
        case 2:
            printf("Key :> ");
            fgets(input, 20, stdin);
            key = calloc(20,sizeof(char));
            s = strchr(input, '\n');
            if (s!=NULL) {
                strncpy(key,input,strlen(input)-1);
            } else {
                strncpy(key,input,strlen(input));
            }
        
            ret = rm(key);
        
            if (ret == 1) {
                printf("[-] Error! Not created!\n");
            }
            
            free(key);
            break;
    
        case 3:
            printf("Key :> ");
            fgets(input, 20, stdin);
            key = calloc(20, sizeof(char));
            s = strchr(input, '\n');
        
            if (s != NULL) {
                strncpy(key, input, strlen(input)-1);
            } else {
                strncpy(key, input, strlen(input));
            }
        
            ret = find(key, 0);
        
            if (ret == 1) {
                printf("[-] Error! Not created\n");
            }
        
            if (ret == 2) {
                printf("[-] Error! Removed\n");
            }

            free(key);
            break;
    
        case 4:
            printf("Key :> ");
            fgets(input,20,stdin);
            key = calloc(20,sizeof(char));
            s = strchr(input, '\n');
        
            if (s != NULL) {
                strncpy(key,input,strlen(input)-1);
            } else {
                strncpy(key,input,strlen(input));
            }
        
            find_r(root, key);
        
            free(key);
            break;
    
        case 5:
            printf("Key :> ");
            fgets(input,20,stdin);
            key = calloc(20,sizeof(char));
            s = strchr(input, '\n');
        
            if (s != NULL) {
                strncpy(key,input,strlen(input)-1);
            } else {
                strncpy(key,input,strlen(input));
            }
        
            ret = print_table(root, key);
        
            if (ret == 1) {
                printf("[0] Table is clear!\n");
            }

            free(key);
            break;
    
        case 6:
            print_Tree(root, 1);
            break;
    
        case 7:
            send_to_file();
            free_tree();
        
        
            printf("\nTiming (Yes/No => 1/0) :> ");
            scanf("%d", &timing);
            printf("\n");
        
            if (timing) {
                D_Timing();
            }
        
            free(input);
            free(root);
            exit(0);
            break;
    }
    
    free(input);
	printf("\n");
    return 0;
}

int main() {
	load_from_file();
    
	while(1)
	{
		print_menu();
		action();
	}
}
