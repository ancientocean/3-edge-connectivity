/*Implementation of "A simple 3-edge-connected component algorithm revisited"

  By:
  Nima Norouzi 
  Copyright ©2006
*/

/*This C program accepts a multigraph as the input graph.
  You need to make an executable program file after compiling this program by
  using gcc compiler or another C compiler. Your input graph must 
  be ready in a text file having the number of vertices at the first line 
  and the adjacency lists of vertices in the following lines; each list starts 
  at a new line. The name of the file has to be passed to the executable program 
  as the first argument; e.g "./3edge graph.txt"

  An acceptable input graph example (i.e. graph.txt):
  5 
  1>2>5>2
  2>5>4>4>3>3>1>1
  3>2>2
  4>2>2
  5>2>1  
*/

/*This program computes all 3-edge-connected components in the input graph.
*/

/*To iterate through 3-edge-connected components, uncomment areas that start 
  and end with "//3edge"; make sure commented areas that start and end with "//YesOrNo"
  are not uncommented. Also, to display 3-edge-connected components, 
  uncomment commented areas that start and end with "//PRINT-3edge".
*/

/*To have the algorithm say only the input graph is 3-edge-connected (Yes instance) or not (No instance),
  uncomment commented areas that start and end with "//YesOrNo"; make sure areas that start and end with    
  "//3edge" or "//PRINT-3edge" are commented.
*/

/*System resources may be managaed, when possible, 
  (using "ulimit -s: in Linux or other ways) 
  to avoid stack overflows during recursive calls at run times. 
*/

/*Run this program along with "time" command to get an execution 
  time; e.g. "time ./3edge graph.txt"
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define max_length 10
/*Since we used "int" variables to store vertices, the lergest positive number 
  can be represented by a 32bit "int" data type is 2147483647. 
  the number 2147483647 has 10 digits.
*/

struct adjacent_with_u_in_G { 
   /*is used to define adjacency lists
   */
   int u;   
   /*to represent an edge (w,u) in the list of vertex w;
     note that this edge is repeated in the list of vertex u as well.
   */
   struct adjacent_with_u_in_G *more;
};
typedef struct adjacent_with_u_in_G* adjacentG;

adjacentG *LG;
adjacentG edge, edge2;

int Vnum;	 
int edgeNum = 0; //initilizing the number of edges in G
int count, Pu, compNum=0, degree, tmp2, parent, child;

int *pre, *lowpt, *nd, *next_on_path, *deg, *next_sigma_element;
char *visited, *outgoing_tree_edge;

//******************************************************************************

void abrt(char ch[50]) {
   printf("\n%s", ch);
   //getchar();
   exit(1);
}

void freeMem(int v) {
   int r;
   for (r=1;r<v;r++) {
      while (LG[r] != NULL) {
         edge = LG[r]->more;
         free(LG[r]);
         LG[r] = edge;
      }
   }
   
   free(LG);
   free(nd);
   free(deg);   
   free(pre);
   free(lowpt);
   free(visited);
   free(next_on_path);
   free(outgoing_tree_edge);
   free(next_sigma_element);
}

void absorb_path(int x0, int xi, int end) {
   int xi_1 = x0;
   if (xi_1 != xi && xi_1 != end) {
      while (xi_1 != xi) {
         deg[x0] = deg[x0] + deg[xi] - 2;

         xi_1 = next_sigma_element[x0];
         next_sigma_element[x0] = next_sigma_element[xi];
         next_sigma_element[xi] = xi_1;
         /*using xi_1 as a temp variable
         */
         xi_1 = xi;
         if (xi != end)
            xi = next_on_path[xi];
      }
   }
}

void three_edge_connect(int w,int v) {

   int u;

   nd[w] = 1;
   adjacentG edge;
   visited[w] = 'Y';

   next_sigma_element[w] = w;
   /*Using next_sigma_element we implement sigma(w) which denotes the set consisting 
     of vertex w and all the vertices that have been absorbed either by w or by 
     vertices that have been absorbed by w.
     It basically creates a chain (set) of all vertices in the same 3-edge-connected component.
     The chain starts with a supervertex (of degree 2) representing the whole chain (set).
     next_sigma_element[w] denotes to the next vertex in the chain.
     if next_sigma_element[w] = w then it means w is at the end of the chain.
     So to initialize next_sigma_element[w], we set next_sigma_element[w] = w
   */
   next_on_path[w] = w;
   /*To represent w-path and u-path; next_on_path[w] is the next vertex that can
     be absorbed on w-path. If next_on_path[w] == w, that means w-path is null.
     In another word next_on_path[w] is a child of w on current w-path in each 
     new resulting graph.
   */

   pre[w]=count;
   lowpt[w]=pre[w];
   count = count + 1;
   edge = LG[w];
   //1
   while (edge != NULL) { 
      /*Do the followings for every edge e=(w,u) which is represented in the adjacency list of w.
      */
      u = edge->u;
			
      deg[w] = deg[w] + 1; 
      /*for each edge e=(w,u) represented in the list of w, deg[w] must be incremented by 1.
        This is to take every edge incident on w into account.
      */

      //1.1
      if (visited[u]=='N') {
         three_edge_connect(u,w);
         nd[w] = nd[w] + nd[u];
         //1.1.1
         if (deg[u]<=2) {
            /*//YesOrNo start
              printf("It's a NO instance!\n");
              freeMem(Vnum);
              exit(1);
            *///YesOrNo end

            deg[w] = deg[w] + deg[u] - 2;
            compNum = compNum + 1;

            /*//PRINT-3edge start
              printf("\nA 3-edge-connected component: %d",u);
            *///PRINT-3edge end

            tmp2 = next_sigma_element[u];
            while (tmp2 != u) {
            /*//PRINT-3edge start
              printf(",%d",tmp2);
            *///PRINT-3edge end
               tmp2 = next_sigma_element[tmp2];
            }

            if (next_on_path[u] == u)
            /*u has just been spitted out, and next_on_path[u] == u means  
              there is nothing on u-path which could be absorbed by w.			
              We set Pu = w; so later we can correctly call the absorb-path procedure
              or update Pw. Basically w + Pu is now the null path.
            */
               Pu = w;
            else
               Pu = next_on_path[u];
               /*equivalent to (Pu=Pu-u) what proposed in the paper.
                 that means the next vertex can be absorbed on u-path is
                 the one after u, without u being absorbed since it has
                 been spitted out.
               */
         }
         else
            Pu = u;
            /*since deg(u) is not 2 then the next vertex can be absorbed on
              u-path is u.
            */				 
         //1.1.2
         if (lowpt[w]<=lowpt[u])
				 
            absorb_path(w, Pu, 0);//(w+Pu)				 
         else {
            lowpt[w]=lowpt[u];
						
            absorb_path(w, next_on_path[w], 0);//(Pw)
            next_on_path[w] = Pu;						
         }
      }//end of if u is not visited 
      else {
         /*when u is visited 
         */
         if (u == v && outgoing_tree_edge[w]=='1') {
            outgoing_tree_edge[w] = '0'; 
            /*Once we encounter an (w,u) edge, such that u is the
              parent of w, we consider this edge as the incoming tree edge of u 
              or outgoing_tree_edge of w.
              From now, no more outgoing_tree-edge of w (w,u) must be encountered!
              It means any other (if exists) (w,u) edge, which u is the parent of w, 
              is a parallel edge and also an outgoing back-edge of w.							
            */
         }
         //1.2 			 
       	 else if (pre[w]>pre[u]) {
            /*(w,u) is an outgoing back-edge of w 
            */
            if (pre[u]<lowpt[w]) {
						
               absorb_path(w, next_on_path[w], 0);//absorb_path(Pw) 
               next_on_path[w] = w;				
               lowpt[w] = pre[u];
            }
         }
         //1.3
         else {
            deg[w] = deg[w] - 2;
            if (next_on_path[w]!=w) {
               /*When pre[w]<pre[u], it means (w,u) is an incoming back-edge of w;
                 however we first check to make sure Pw is not null (by
                 next_on_path[w]!=w) because if it is null there is nothing
                 to be absorbed by w at this time.
               */
               parent = w;
               child = next_on_path[w];
               while ((parent!=child) && (pre[child]<=pre[u]) && (pre[u]<=pre[child] + nd[child] - 1) ) {
                  /*while parent_path in not null and child is an ancestor of u 
                  */                
                  parent = child;
                  child = next_on_path[child];
               }
                  
               absorb_path(w, next_on_path[w], parent);//Pw[w..u]
                  /*starting from child of w absorb everything on Pw[w..u]
                    until x(here parent) is absorbed. x lies on Pw[w..u]! 
                  */
               if (parent == next_on_path[parent])
                  /*if x-path is null then Pw is set to null as well.
                  */                               
                  next_on_path[w] = w;
               else                                
                  /*if x-path is not null then Pw is set to x-path.
                  */
                  next_on_path[w] = next_on_path[parent];
            }
         }
      }
      edge = edge->more;
   }
}//end of three-edge-connect
//******************************************************************************

int main(int argc, char **argv) {

   if (argc < 2) {
      printf("No input!\n");
      abort();
   }
   FILE *in;
   const char* in_filename = argv[1];
   int r, n, ch, v, indx;
   int next_list = 1;
   char ch2[max_length];//the maximum allowed number of digits is max_length
   
   printf("\nComputing 3-edge-connected components...\n");	 	 
	 
   if ((in = fopen(in_filename, "rt")) == NULL)
      abrt("Cannot open input file.");
   indx = 0;		
   while ( (ch = fgetc(in)) != 10) { //reading the first number as the number of vertices
      ch2[indx] = (char)ch;
      indx = indx + 1;
   }
   ch2[indx] = '\0';	 	 
   Vnum = atoi(ch2) + 1;
    
//*********************************Memory allocation
   if (( LG = (adjacentG*)malloc(Vnum * sizeof(struct adjacent_with_u_in_G)) ) == NULL)
      abrt("Not enough memory to allocate buffer");

   if (( lowpt = (int *)malloc(Vnum * sizeof(int)) ) == NULL)
      abrt("Not enough memory to allocate buffer");

   if (( deg = (int *)malloc(Vnum * sizeof(int)) ) == NULL)
      abrt("Not enough memory to allocate buffer");

   if (( next_sigma_element = (int *)malloc(Vnum * sizeof(int)) ) == NULL)
      abrt("Not enough memory to allocate buffer");

   if (( pre = (int *)malloc(Vnum * sizeof(int)) ) == NULL)
      abrt("Not enough memory to allocate buffer");

   if (( nd = (int *)malloc(Vnum * sizeof(int)) ) == NULL)
      abrt("Not enough memory to allocate buffer");

   if (( next_on_path = (int *)malloc(Vnum * sizeof(int)) ) == NULL)
      abrt("Not enough memory to allocate buffer");
			
   if (( visited = (char *)malloc(Vnum * sizeof(char)) ) == NULL)
      abrt("Not enough memory to allocate buffer");
			
   if (( outgoing_tree_edge = (char *)malloc(Vnum * sizeof(char)) ) == NULL)
      abrt("Not enough memory to allocate buffer");						
			
   for (indx = 0; indx < Vnum; indx++) {
      LG[indx] = NULL;
      visited[indx] = 'N';
      outgoing_tree_edge[indx] = '1';
      deg[indx] = 0;
   }			
   indx = 0;		
   while ( (ch = fgetc(in)) != EOF) {
      if (indx == 10 && ch != 62 && ch != 10) 
         /*after we reach to the maximum length,
           we only accept a '>' or new line. 
         */
         abrt("Your input file has an error!");
      else if (ch == 62) {
         ch2[indx] = '\0';
         indx = 0;
         n = atoi(ch2);
         if (next_list) {
            v = n;
            /*determine the next list
            */
            next_list = 0;
         }
         else {
            if (( edge = (adjacentG)malloc(sizeof(struct adjacent_with_u_in_G)) ) == NULL)
               abrt("Not enough memory to allocate buffer23");
            edge->more = NULL;
            edge->u = n;
      	    edge->more = LG[v];
            LG[v] = edge;
            edgeNum = edgeNum + 1;						
         }
      }
      else if (ch == 10) {
         ch2[indx] = '\0';
         indx = 0;
         if (!next_list) {
            n = atoi(ch2);
            if (( edge = (adjacentG)malloc(sizeof(struct adjacent_with_u_in_G)) ) == NULL)
               abrt("Not enough memory to allocate buffer24");
            edge->more = NULL;
            edge->u = n;
            edge->more = LG[v];
            LG[v] = edge;
            edgeNum = edgeNum + 1;						
            next_list = 1;						
         }
      }
      else {
         ch2[indx] = (char)ch;
         indx = indx + 1;
      }
   }
   if (!next_list) {
      ch2[indx] = '\0';
      n = atoi(ch2);
      if (( edge = (adjacentG)malloc(sizeof(struct adjacent_with_u_in_G)) ) == NULL)
         abrt("Not enough memory to allocate buffer25");
      edge->more = NULL;
      edge->u = n;
      edge->more = LG[v];
      LG[v] = edge;
      edgeNum = edgeNum + 1;						
   }
   edgeNum = edgeNum / 2;	 
   fclose(in);
	 
   printf("\nComplexity of the input graph:\n|V| + |E| = %d + %d = %d\n",Vnum-1,edgeNum,Vnum+edgeNum-1);

   count = 1;
   //r = 1;
   for (r=1;r<Vnum;r++) {
      if (visited[r]=='N') {

      /*//YesOrNo start
        if (r>1) {
           printf("It's a NO instance!\n");
           freeMem(Vnum); 
           exit(1);
        }
      *///YesOrNo end

      three_edge_connect(r,0);
      compNum++;
      /*//PRINT-3edge start
        printf("\nA 3-edge-connected component: %d",r);
      *///PRINT-3edge end
      //3edge start
      tmp2 = next_sigma_element[r];
      while (tmp2 != r) {
      //3edge end	 
      /*//PRINT-3edge start
        printf(",%d",tmp2);
      *///PRINT-3edge end
      //3edge start
         tmp2 = next_sigma_element[tmp2];
      }
      //3edge end	 
   }
}
   
//   for (indx=1;indx<Vnum;indx++) 
//      printf("\ndeg[%d]=%d",indx,deg[indx]);   
   
freeMem(Vnum);    
/*//YesOrNo Start
  printf("It's a YES instance!\n");
*///YesOrNo

//3edge start
printf("\n# of 3-edge-connected components: %d\n",compNum);
//3edge end

return 0;
}
