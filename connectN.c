/*
*
*
*
*
*
*/


#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>

// Function Prototypes
int InitializeBoard(int** connectNBoard,int numRowsInBoard);
int MakeMove(int** connectNBoard, int numRowsInBoard, int playerID, int columnChosen);
int DisplayBoard(int** connectNBoard, int numRowsInBoard);
int CheckWinner(int** connectNBoard, int numRowsInBoard, int numConnect, int columnChosen, int playerID); 

// Initializing constant variables with fixed values.
const int MAXSIZE = 25;
const int MINSIZE = 8;

//int main prompts the user to put in the size of the board (it'll loop till they get it within our requirements)
//calls initializeboard
//also prompts them to give the numConnect (also loops till we get within our requirements)
//then prompts them to make a move along with displayboard to see the updated board after the previous try (or an empty board for first try)
//counts for wrong tries and upon 3 wrong tries itll forfiet the player's turn and go to the next player
// calls displayboard to display the board after every move along with the required information about who's turn is it
//also calls checkwinner to see if the tile placed has won the game or not
//then it exits if we've won or the while(1) loops again if we havent

int main() 

{
    // Initializing all the necessary variables
    int numRows=0;
    int numToConnect=0;
    int playerID=1;
    int flag=0;
    int i=0;
    int columnChosen=0;
    int turn=0;
    int winner=0;
    int tries=0;
    int result;
    int result_b;
    int border=0;
    int result_c;
    // While loop to take the dimensions of the board, will keep on looping
    //till we get a number between 8 and 25 (both included)
    while(flag==0)
    {
        printf("Enter the number of squares along each edge of board\n");
        result = scanf("%d",&numRows);
        if(result !=1)
        {
            printf("ERROR: The value provided was not an integer\n");
            while(getchar()!='\n');
        }
        else if (numRows<MINSIZE)
        {
            printf("ERROR: Board size too small (<8)\n");
            while(getchar()!='\n');
        }
        else if (numRows>MAXSIZE)
        {
            printf("ERROR: Board size too large (>25)\n");
            while(getchar()!='\n');
        }
        else
        {
            flag=1;
        }
       
    }
    border=numRows-4;
    flag=0;
    // While loop to get the number of tiles which should connect for a player
    //to win. will loop till we get a number between 4 and numRows-4
    while(flag==0)
    {
        printf("Enter the number of pieces that must form a line to win\n");
        result_b = scanf("%d",&numToConnect);
        if(result_b !=1)
        {
            printf("ERROR: The value provided was not an integer\n");
            while(getchar()!='\n');
        }
        else if (numToConnect<4)
        {
            printf("ERROR: Number to connect is too small (<4)\n");
            while(getchar()!='\n');
        }
        else if (numToConnect>numRows-4)
        {
            printf("ERROR: Number to connect is too large (>%d)\n",border);
            while(getchar()!='\n');
        }
        else
        {
            flag=1;
        }
    }
    //dynamically allocating memory for 2D array(our board)
    int** connectNboard ;
    connectNboard = (int**) malloc(numRows*sizeof(int*));
    connectNboard[0]= (int*) malloc(numRows*numRows*sizeof(int));
    for (i=1;i<numRows;i++)
    {
        connectNboard[i]=connectNboard[0]+numRows*i;
    }
    //checking if its initialized properly
    if(!InitializeBoard(connectNboard, numRows))
    {
        printf("ERROR: Could not initialize the game board\n");
        return 0;
    }
    //the game begins here
    while(1)
    {   

        tries=0;
        printf("\n\n");
        //checking if its displayed properly
        if(!DisplayBoard(connectNboard, numRows))
        {
            printf("ERROR: Could not display the game board\n");
            return 0;
        }
        //this line here alternates between those two players
        playerID=(turn%2)+1;

        printf("\n%s moves\n",(playerID==1)? "Red":"Black");
        flag=0;
        //this code checks for the 3 tries rule. if we do 3 wrong tries
        //then it'll exit the code
        while(flag==0 && tries<3)
        {
            printf("Enter the column number where you want to put your piece\n");  
            result_c = scanf("%d",&columnChosen);
            while(getchar()!='\n');
            if(result_c !=1)
            {   
                printf("ERROR: The value provided was not an number\n");
                tries++;
            }
            else if (columnChosen>=numRows || columnChosen<0)
            {
                printf("ERROR: Column %d is not on the board: try again\n",columnChosen);
                printf("ERROR: Column number should be >=0 and <%d\n",numRows);
                tries++;
            }
            else
            {
                flag=1;
            }
        }
        while(tries<3)
        {
            if(MakeMove(connectNboard, numRows, playerID, columnChosen))
            {
                break;
            }
            else
            {
                tries++;
            }
        }
        //this prints the illegal moves error and exits the loop
        if(tries==3)
        {
            printf("TOO MANY ILLEGAL MOVES\n");
            printf("%s has forfeited a turn\n",(playerID==1)? "Red":"Black");
            tries=0;
        }
        else
        {
            //we check for winner. if it returns 1 then it means the current player has won
            winner=CheckWinner(connectNboard,numRows, numToConnect, columnChosen, playerID);
            if (winner==1)
            {
                //this checks for which player won
                if(playerID==1)
                {
                    printf("Red has won\n");
                    DisplayBoard(connectNboard, numRows);
                    printf("\n");
                    break;
                }
                else if(playerID==2)
                {
                    printf("Black has won\n");
                    DisplayBoard(connectNboard, numRows);
                    printf("\n");
                    break;
                }

            }
        }
        //this alternates the player turns
        turn++;
    }
    
    //freeing up the memory allocating
    free(connectNboard[0]);
    
    free(connectNboard);

    return 0; 

}

// This block of function gets connectNBoard which is a 2d array
// It initializes every element to 0
// It also shows error if the memory allocation failed
// It shows error when the row number is out of bounds

int InitializeBoard(int** connectNBoard,int numRowsInBoard)
{
    //checking if its initialized properly
    if (connectNBoard == NULL)
	{
		printf("ERROR: cannot initialize the board because the pointer to the board is NULL\n");
		return 0;
	}
    if(numRowsInBoard<=MINSIZE||numRowsInBoard>=MAXSIZE)
    {
        printf("ERROR: illegal number of rows in board\n");
        return 0;
    }
    int i;
    int j;
    for(i=0;i<numRowsInBoard;i++)
    {
        for(j=0;j<numRowsInBoard;j++)
        {
            connectNBoard[i][j]=0;
        }
    }
    return 1;
}

//this is makemove, it takes connectNboard 2D array as a pass by reference to modify its elements with the appropriate
//playerID and takes the max no. of rows in board, the playerID of the player whose turn it is and the column which they've
//chosen as a pass by value to append the array with the playerID

int MakeMove(int** connectNBoard, int numRowsInBoard, int playerID, int columnChosen) 
{
    if (connectNBoard==NULL)
    {
        printf("ERROR: cannot print the board because the pointer to the board is NULL\n");
        return 0;
    }
    int row;
    //this code makes the move in the bottom most row available in the column chosen
    for (row=numRowsInBoard-1;row>=-1;row--) 
    {
        if(columnChosen>(numRowsInBoard-1) || columnChosen<0 || row==-1)
        {
            printf("Illegal move\n");
            return 0;
        }
        else if (connectNBoard[row][columnChosen]==0) 
        {
            connectNBoard[row][columnChosen]=playerID;
            break;
        }
    }
    if (row!=0)
    {
        printf("%s has moved\n", (playerID == 1) ? "Red" : "Black");
        return 1;
    }
    else
    {
        return 0;
    }
}

// This block of code gets the 2d array named connectnBoard and also the row numbers
// It prints all the elements to the screen as output

int DisplayBoard(int** connectNBoard, int numRowsInBoard)
{
    if (connectNBoard == NULL)
	{
		printf("ERROR: cannot initialize the board because the pointer to the board is NULL\n");
		return 0;
	}
    if(numRowsInBoard<MINSIZE||numRowsInBoard>MAXSIZE)
    {
        printf("ERROR: illegal number of rows in board\n");
        return 0;
    }
    //this prints the board after each move
    int k;
    int l;
    int m;
    int n=0;
    char black = 'B';
    char red = 'R';
    char zero ='o';
    printf("   ");
    for(m=0;m<numRowsInBoard;m++)
    {
        printf("%3d",m);
    }

    for(k=0;k<numRowsInBoard;k++)
    {
        printf("\n");
        if ( n < numRowsInBoard)
        {
            printf("%3d",k);
        }
        n++;
        //this code swaps all the zeroes with 'o' and playerID 1 with red(R) and playerID 2 with black(B)
        for(l=0;l<numRowsInBoard;l++)
        {
    
            
                if(connectNBoard[k][l]==0)
                {
                    printf("%3c",zero);
                }
                else if(connectNBoard[k][l]==2)
                {
                    printf("%3c",black);
                }
                else if(connectNBoard[k][l]==1)
                {
                    printf("%3c",red);
                }
            
        }        
    }
}

//this is checkwinner, it takes connectNboard as a pass by reference to see the playerID's filled out
//and checks if the player ID's to the left, right, down and diagonals match and add up to numConnect 
//which we've taken as pass by value along with the 2D array size (numrowsinboard) and the column that they've
//chosen and the ID of the player which is playing right now

int CheckWinner(int** connectNBoard, int numRowsInBoard, int numConnect, int columnChosen, int playerID)
{
    //we check if the pointer to the board is there
    if (connectNBoard == NULL) 
    {
        printf("ERROR: cannot print the board because the pointer to the board is NULL\n");
        return 0;
    }

    if (columnChosen<0 || columnChosen>numRowsInBoard-1) 
    {
        printf("ERROR: invalid column chosen, cannot check for winner\n");
        return 0;
    }

    int row = 0;
    int afterCount = 0;
    int beforeCount = 0;
    int col=0;
    int rowindex=0;
    int ans=0;
    //int ans is initialized to 0. basically if we dont see a win, then by default
    //it will return 0. but if we see a connection then ans will be updated to 1
    //and we return ans
    //we find the row index of the element that we placed in our column chosen
    while(connectNBoard[rowindex][columnChosen]==0)
    {
        rowindex++;
    }
    row=rowindex;
    //this checks to the right for a horizontal connect N
    for (col=columnChosen+1;col<numRowsInBoard;col++) 
    {
        if (connectNBoard[row][col]==playerID) 
        {
            afterCount++;
        } 
        else 
        {
            break;
        }
    }
    row=rowindex;
    //this checks to the left for a horizontal connect N
    for (col = columnChosen - 1; col >= 0; col--)
    {
        if (connectNBoard[row][col] == playerID)
        {
            beforeCount++;
        } 
        else 
        {
            break;
        }
    }
    //this adds the elements to the left and the right and the middle one which we placed
    //if its more than or equal to numConnect then we win
    if (afterCount+beforeCount+1>=numConnect) 
    {
        ans=1;
    }

    afterCount = 0;
    beforeCount = 0;
    //this checks down of the element we placed
    for (row=rowindex+1;row<numRowsInBoard;row++) 
    {
        if (connectNBoard[row][columnChosen]==playerID) 
        {
            afterCount++;
        } 
        else 
        {
            break;
        }
    }
    //this sees if the elements below connect and are longer than numConnect
    if (afterCount+1>=numConnect) 
    {
        ans=1;
    }

    afterCount = 0;
    beforeCount = 0;

    row=rowindex+1;
    col=columnChosen-1;
    //this checks diagonally from left to right and bottom to our tile placed
    while (row<numRowsInBoard && col>=0)
    {
        if (connectNBoard[row][col]==playerID) 
        {
            beforeCount++;
            row++;
            col--;
        } 
        else 
        {
            break;
        }
    }

    row=rowindex-1;
    col=columnChosen+1;
    //this checks diagonally from left to right and upwards of our tile placed
    while (row>=0 && col<numRowsInBoard) 
    {
        if (connectNBoard[row][col] == playerID)
        {
            afterCount++;
            row--;
            col++;
        } 
        else 
        {
            break;
        }
    }
    //we combine them and see if it exceeds numConnect, if it does we update ans to 1
    if (afterCount + beforeCount + 1 >= numConnect) 
    {
        ans=1;
    }

    afterCount = 0;
    beforeCount = 0;
    row = rowindex - 1;
    col = columnChosen - 1;
    //this checks from right to left and upwards of our tile placed
    while (row >= 0 && col >= 0) 
    {
        if (connectNBoard[row][col] == playerID) 
        {
            beforeCount++;
            row--;
            col--;
        } 
        else 
        {
            break;
        }
    }

    row = rowindex+1;
    col = columnChosen + 1;
    //this checks from right to left and bottom to our tile placed
    while (row < numRowsInBoard && col < numRowsInBoard) 
    {
        if (connectNBoard[row][col] == playerID) 
        {
            afterCount++;
            row++;
            col++;
        } 
        else 
        {
            break;
        }
    }

    if (afterCount + beforeCount +1 >= numConnect)
    {
        ans=1;
    }
    //we update ans to 1 whenever an if block runs that checks if left and right connections equal or exceed our numConnect
    //then ans is returned and we go to the main block
    return ans;
}