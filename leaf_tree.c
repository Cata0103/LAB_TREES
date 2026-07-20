#include <stdio.h>
#include <stdlib.h>
#include "xmalloc.h"
#include "leaf_tree.h"

tree_node_t *get_node_leaf()
{ 
   tree_node_t *tmp = (tree_node_t*)xmalloc(sizeof(tree_node_t));
   tmp->height = 0;
   tmp->key = 0;
   tmp->left = NULL;
   tmp->right = NULL;
   tmp->left_link = NULL;
   tmp->right_link = NULL;
   return tmp;
}


void return_node_leaf(tree_node_t *node)
{  
   if(node->right==NULL){
      free(node);
      return;
   }
   return_node_leaf(node->left);
   return_node_leaf(node->right);
   free(node);
}

tree_node_t *create_tree_leaf(void)
{  
   tree_node_t *tmp_node;
   tmp_node = get_node_leaf();
   tmp_node->left = NULL;
   return( tmp_node );
}

void return_tree_leaf(tree_node_t * leaf_tree)
{
   if(leaf_tree->left!=NULL){
      return_node_leaf(leaf_tree->left);
   }
   free(leaf_tree);
}

void left_rotation_leaf(tree_node_t *n)
{  tree_node_t *tmp_node;
   key_t        tmp_key;
   tmp_node = n->left; 
   tmp_key  = n->key;
   n->left  = n->right;        
   n->key   = n->right->key;
   n->right = n->left->right;  
   n->left->right = n->left->left;
   n->left->left  = tmp_node;
   n->left->key   = tmp_key;
}

void right_rotation_leaf(tree_node_t *n)
{  tree_node_t *tmp_node;
   key_t        tmp_key;
   tmp_node = n->right; 
   tmp_key  = n->key;
   n->right = n->left;        
   n->key   = n->left->key;
   n->left  = n->right->left;  
   n->right->left = n->right->right;
   n->right->right  = tmp_node;
   n->right->key   = tmp_key;
}

long find_leaf(tree_node_t *tree, key_t query_key)
{  tree_node_t *tmp_node;
   long nodes_visited = 0;
   if( tree->left == NULL )
     return(0);
   else
   {  tmp_node = tree;
      nodes_visited++;
      while( tmp_node->right != NULL )
      {  if( query_key < tmp_node->key )
         {
            tmp_node = tmp_node->left;
            nodes_visited++;
         }
         else
         {
             tmp_node = tmp_node->right;
             nodes_visited++;
         }
      }
      return nodes_visited;
      // if( tmp_node->key == query_key )
      //    return( (object_t *) tmp_node->left );
      // else
      //    return( NULL );
   }
}


int insert_leaf(tree_node_t *tree, key_t new_key, object_t *new_object)
{  tree_node_t *tmp_node; //crear nodo temporal para insertar
   int finished;
   int rotations_performed = 0;
   if( tree->left == NULL ) // Caso 1: Árbol vacio.
   {  
      tree->left = (tree_node_t *) new_object;
      tree->key  = new_key;
      tree->height = 0;
      tree->right  = NULL;
      tree->left_link = NULL;
      tree->right_link = NULL;
   }
   else
     {  tree_node_t * path_stack[100]; int  path_st_p = 0;
      tmp_node = tree; 
      while( tmp_node->right != NULL )
      {   path_stack[path_st_p++] = tmp_node;
          if( new_key < tmp_node->key )
               tmp_node = tmp_node->left;
          else
               tmp_node = tmp_node->right;
      }
      /* found the candidate leaf. Test whether key distinct */
      if( tmp_node->key == new_key )
         return( -1 );
      /* key is distinct, now perform the insert */ 
      else
      {  tree_node_t *old_leaf, *new_leaf;
         old_leaf = get_node_leaf();
         old_leaf->left = tmp_node->left; 
         old_leaf->key = tmp_node->key;
         old_leaf->right  = NULL;
         old_leaf->height = 0;
         new_leaf = get_node_leaf();
         new_leaf->left = (tree_node_t *) new_object; 
         new_leaf->key = new_key;
         new_leaf->right  = NULL;
         new_leaf->height = 0; 
         if( tmp_node->key < new_key )
         {   tmp_node->left  = old_leaf;
             tmp_node->right = new_leaf;
             tmp_node->key = new_key;

         } 
         else
         {  tmp_node->left  = new_leaf;
            tmp_node->right = old_leaf;
         } 
         tmp_node->left->right_link = tmp_node->right;
         tmp_node->right->left_link = tmp_node->left;
         tmp_node->right->right_link= tmp_node->right_link;
         tmp_node->left->left_link= tmp_node->left_link;
         tmp_node->right_link = NULL;
         tmp_node->left_link = NULL;
         if(tmp_node->right->right_link!=NULL){
            tmp_node->right->right_link->left_link = tmp_node->right;
         }
         if(tmp_node->left->left_link!=NULL){
            tmp_node->left->left_link->right_link = tmp_node->left;
         }
         tmp_node->height = 1;
      }
      /* rebalance */
      finished = 0;
      while( path_st_p > 0 && !finished )
      {  int tmp_height, old_height;
         tmp_node = path_stack[--path_st_p];
         old_height= tmp_node->height;
         if( tmp_node->left->height - tmp_node->right->height == 2 )
         {  if( tmp_node->left->left->height - tmp_node->right->height == 1 )
            {  right_rotation_leaf( tmp_node );
               rotations_performed++;
               tmp_node->right->height = tmp_node->right->left->height + 1;
               tmp_node->height = tmp_node->right->height + 1;
            }
            else
            {  left_rotation_leaf( tmp_node->left );
               rotations_performed++;
               right_rotation_leaf( tmp_node );
               rotations_performed++;
               tmp_height = tmp_node->left->left->height; 
               tmp_node->left->height  = tmp_height + 1; 
               tmp_node->right->height = tmp_height + 1; 
               tmp_node->height = tmp_height + 2; 
            }
         }
         else if ( tmp_node->left->height - tmp_node->right->height == -2 )
         {  if( tmp_node->right->right->height - tmp_node->left->height == 1 )
            {  left_rotation_leaf( tmp_node ); //aca
               rotations_performed++;
               tmp_node->left->height = tmp_node->left->right->height + 1;
               tmp_node->height = tmp_node->left->height + 1;
            }
            else
            {  right_rotation_leaf( tmp_node->right );
               rotations_performed++;
               left_rotation_leaf( tmp_node );
               rotations_performed++;
               tmp_height = tmp_node->right->right->height; 
               tmp_node->left->height  = tmp_height + 1; 
               tmp_node->right->height = tmp_height + 1; 
               tmp_node->height = tmp_height + 2; 
            }
         }
         else /* update height even if there was no rotation */ 
         {  if( tmp_node->left->height > tmp_node->right->height )
               tmp_node->height = tmp_node->left->height + 1;
            else
               tmp_node->height = tmp_node->right->height + 1;
         }
         if( tmp_node->height == old_height )
            finished = 1;
      }
      
   }
   return( rotations_performed);
}



object_t *delete_node_leaf(tree_node_t *tree, key_t delete_key)
{  tree_node_t *tmp_node, *upper_node, *other_node;
   object_t *deleted_object; int finished;
   if( tree->left == NULL ) // arbol vacio
      return( NULL );
   else if( tree->right == NULL ) // raiz es el unico nodo
   {  if(  tree->key == delete_key )
      {  deleted_object = (object_t *) tree->left;
         tree->left = NULL;
         return( deleted_object );
      }
      else
         return( NULL );
   }
   else
   {  tree_node_t * path_stack[100]; int path_st_p = 0;
      tmp_node = tree;
      while( tmp_node->right != NULL )
      {   path_stack[path_st_p++] = tmp_node;  
          upper_node = tmp_node;
          if( delete_key < tmp_node->key )
          {  tmp_node   = upper_node->left; 
             other_node = upper_node->right;
          } 
          else
          {  tmp_node   = upper_node->right; 
             other_node = upper_node->left;
          } 
      }
      if( tmp_node->key != delete_key )
         deleted_object = NULL; //elemento a eliminar no está en el arbol
      else
      {  upper_node->key   = other_node->key;
         upper_node->left  = other_node->left;
         upper_node->right = other_node->right;
         upper_node->height = other_node->height;
         if(other_node->left_link==tmp_node){
            upper_node->right_link = other_node->right_link;
            if(other_node->right_link!=NULL){
               upper_node->right_link->left_link = upper_node;
            }
            upper_node->left_link = tmp_node->left_link;
            if(tmp_node->left_link!=NULL){
               upper_node->left_link->right_link = upper_node;
            }
         }else{
            upper_node->left_link = other_node->left_link;
            if(other_node->left_link!=NULL){
               upper_node->left_link->right_link = upper_node;
            }
            upper_node->right_link = tmp_node->right_link;
            if(tmp_node->right_link!=NULL){
               upper_node->right_link->left_link = upper_node;
            }
         }
         deleted_object = (object_t *) tmp_node->left;
         return_node_leaf( tmp_node );
         return_node_leaf( other_node );

      }
      /*start rebalance*/  
      finished = 0; path_st_p -= 1;
      while( path_st_p > 0 && !finished )
      {  int tmp_height, old_height;
         tmp_node = path_stack[--path_st_p];
         old_height= tmp_node->height;
         if( tmp_node->left->height - tmp_node->right->height == 2 )
         {  if( tmp_node->left->left->height - tmp_node->right->height == 1 )
	    {  right_rotation_leaf( tmp_node ); 
               tmp_node->right->height = tmp_node->right->left->height + 1;
               tmp_node->height = tmp_node->right->height + 1;
            }
            else
	    {  left_rotation_leaf( tmp_node->left ); 
               right_rotation_leaf( tmp_node );
               tmp_height = tmp_node->left->left->height; 
               tmp_node->left->height  = tmp_height + 1; 
               tmp_node->right->height = tmp_height + 1; 
               tmp_node->height = tmp_height + 2; 
            }
         }
         else if ( tmp_node->left->height - tmp_node->right->height == -2 )
         {  if( tmp_node->right->right->height - tmp_node->left->height == 1 )
	         {  left_rotation_leaf( tmp_node ); 
               tmp_node->left->height = tmp_node->left->right->height + 1;
               tmp_node->height = tmp_node->left->height + 1;
            }
            else
            {  right_rotation_leaf( tmp_node->right );
               left_rotation_leaf( tmp_node );
               tmp_height = tmp_node->right->right->height; 
               tmp_node->left->height  = tmp_height + 1; 
               tmp_node->right->height = tmp_height + 1; 
               tmp_node->height = tmp_height + 2; 
            }
         }
         else /* update height even if there was no rotation */ 
         {  if( tmp_node->left->height > tmp_node->right->height )
               tmp_node->height = tmp_node->left->height + 1;
            else
               tmp_node->height = tmp_node->right->height + 1;
         }
         if( tmp_node->height == old_height )
	    finished = 1;
      }
      /*end rebalance*/
      return( deleted_object );
   }
}

void print_list_ordered_leaf(tree_node_t * n) // Impresión de nodos hoja del arbol.
{
   while(n->right!=NULL){
      n = n->left;
   }
   //printf("{ ");
   while(n!=NULL){
      // printf("%d ",n->key);
      n = n->right_link;
   }
   //printf("}\n");
}

void print_list_inverted_leaf(tree_node_t * n)
// Impresión de nodos hoja del arbol en orden inverso. Correctitud del doble enlace en
// las hojas del arbol.
{
   while(n->right!=NULL){
      n = n->right;
   }
   printf("{ ");
   while(n!=NULL){
      printf("%d ",n->key);
      n = n->left_link;
   }
   printf("}\n");
}

void print_node_leaf(tree_node_t * n)
{
    if(n->right == NULL){
        printf("%d(%d) ",n->key,n->height);
        return;
    } 
    print_node_leaf(n->left);
    printf("%d(%d) ",n->key,n->height);
    print_node_leaf(n->right);
}

void print_tree_leaf(tree_node_t * tree) 
// función de comprobación de la correctitud de la estructura del árbol. 
// Imprime todos los nodos in-order, no solo las hojas, verificar que las alturas sea correctas.
{
    if(tree->left == NULL){
         printf("empty tree.\n");
         return;
      }
    printf("{ ");
    print_node_leaf(tree);
    printf("}\n");
}

range_return_t range_search_leaf(tree_node_t * tree, int a, int b)
{
   range_return_t result = {0, 0};
   tree_node_t * travel = tree;
   while(travel->right!=NULL){
      if(a < travel->key){
         travel = travel -> left;
         result.visited_nodes++;
      }else{
         travel = travel -> right;
         result.visited_nodes++;
      }
   }
   // printf("{ ");
   while (travel!=NULL)
   {
      if(travel->key >= a && travel->key <= b){
         // printf("%d ",travel->key);
         result.printed_nodes++;
         result.visited_nodes++;
      }
      else if(travel->key > b){
         break;
      }
      travel = travel->right_link;
   }
   // printf("}\n");
   return result;
}
