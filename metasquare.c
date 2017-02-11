#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

unsigned long long int printAllBoards(FILE*, int);
void print(FILE*, unsigned long long int, int);
int getspot(unsigned long long int, int, int, int);
int square(unsigned long long int, int);
bool legal(unsigned long long int, int);
int START = 6;
int END = 6;

int main(){
    clock_t startTime = clock();
    unsigned long long int ar[END];
    FILE* fp = fopen("metasquare.txt","w");
    
    for (int n=START; n<=END; n++){
        if (n != 2) fprintf(fp,"\n\n\n");
        fprintf(fp,"LIST OF ALL %dx%d BOARDS\n",n,n);
        ar[n] = printAllBoards(fp,n);
    }
    
    fprintf(fp,"\n\n\n\n\nNOTES\n\n");
    
    fprintf(fp,"Boards that the same except 1's are swapped with 0's and vice versa are only listed once.\n");
    fprintf(fp,"For example, of the two following 2x2 boards only the first is listed.\n\n");
    fprintf(fp,"1 0   0 1\n0 0   1 1\n");
    
    fprintf(fp,"\n\n\nDATA TABLE\n\n");
    
    fprintf(fp,"board width     number of legal boards     number of illegal boards     total number of boards\n");
    
    for (int n=START; n<END; n++){
        fprintf(fp,"%-6d          %-6ld                     %-6ld                       %-6ld\n", n, ar[n]/1000000, ar[n]%1000000, ar[n]/1000000 + ar[n]%1000000);
    }
    printf("total run time is %.1lf seconds\n",(double)(clock()-startTime)/CLOCKS_PER_SEC);
}

unsigned long long int printAllBoards(FILE* fp, int width){
    int k;
    unsigned long long int bound = pow(2,width*width);
    unsigned long long int t = 0, temp;
    
    unsigned long long int legalCount = 0, illegalCount = 0;
    
    while (t<bound/2){
        k = square(t,width);
        if (k == -1){
            if (legal(t,width)){
                fprintf(fp,"Legal:\n");
                legalCount++;
            }
            else{
                fprintf(fp,"Not Legal:\n");
                illegalCount++;
            }
            print(fp,t,width);
            t++;
        }
        else{
            temp = pow(2,k);
            t = temp*(t/temp + 1);
        }
    }
    
    return legalCount*1000000 + illegalCount;
}

int square(unsigned long long int t, int width){
    // returns the position to add to t if it conains a square, otherwise returns -1
    // dx and dy are the distances  in the x and y direction from the first to second points
    
    // dx is on [1,width-1]
    // dx+dy < width
    
    for (int y=width; y>=0; y--){
        for (int x=width-2; x>=0; x--){
            for (int dx=1; x+dx<width; dx++){
                for (int dy=0; dy<=x && y+dx+dy<width; dy++){
                    if ((getspot(t,y,x,width) && getspot(t,y+dy,x+dx,width) && getspot(t,y+dy+dx,x+dx-dy,width) && getspot(t,y+dx,x-dy,width)) || (!getspot(t,y,x,width) && !getspot(t,y+dy,x+dx,width) && !getspot(t,y+dy+dx,x+dx-dy,width) && !getspot(t,y+dx,x-dy,width))){
                        return y*width+x;
    }}}}}
    
    return -1;
}

void print(FILE* fp, unsigned long long int t, int width){
    for (int n=0; n<width; n++){
        for (int m=0; m<width; m++){
            fprintf(fp,"%d ",getspot(t,n,m,width));
        }
        fprintf(fp,"\n");
    }
    fprintf(fp,"\n");
}

int getspot(unsigned long long int t, int y, int x, int width){
    return (t/(unsigned long long int) pow(2,y*width+x))%2;
}

bool legal(unsigned long long int t, int width){
    int ones = 0, zeros = 0;
    if (width%2 == 1){
        for (int n=0; n<width*width; n++){
            if (getspot(t,n/width,n%width,width)) ones++;
            else zeros++;
            if (width*width/2 +1 < zeros || width*width/2 +1 < ones) return false;
        }
    }
    else{
        for (int n=0; n<width*width; n++){
            if (getspot(t,n/width,n%width,width)) ones++;
            else zeros++;
            if (width*width/2 < zeros || width*width/2 < ones) return false;
        }
    }
    return true;
}
