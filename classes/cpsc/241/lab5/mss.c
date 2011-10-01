//              Algorithms for the Maximum Subsequence Sum problem
//
//  This code is taken from: 
//            Weiss, "Data Structures and Algorithm Analysis in C++"
//
//  Each of the following functions implements an algorithm for solving the
//  maximum subsequence sum problem: Given a sequence of integers, find the
//  maximum of the sums of all the subsequences of the sequence.  In each
//  function, the first parameter, A, is an integer array and the second, N,
//  is the number of elements from A to be used as the sequence.  (That is,
//  the sequence is A[0..N-1].)
//


//----------------- "Algorithm 1" from Weiss ---------------------------
//  This algorithm is O(n^3).
int Max_Subsequence_Sum1( const int A[ ], const unsigned int N )
{
    int This_Sum = 0, Max_Sum = 0;

    for( int i = 0; i < N; i++ )
        for( int j = i; j < N; j++ )
        {
            This_Sum = 0;
            for( int k = i; k <= j; k++ )
                This_Sum += A[ k ];

            if( This_Sum > Max_Sum )
                Max_Sum = This_Sum;
        }
    return( Max_Sum );
}



//----------------- "Algorithm 2" from Weiss ---------------------------
// This algorithm is O(n^2).
int Max_Subsequence_Sum2( const int A[ ], const unsigned int N )
{
    int This_Sum = 0, Max_Sum = 0;

    for( int i = 0; i < N; i++ )
    {
        This_Sum = 0;
        for( int j = i; j < N; j++ )
        {
            This_Sum += A[ j ];

            if( This_Sum > Max_Sum )
                Max_Sum = This_Sum;
        }
    }
    return( Max_Sum );
}



//----------------- "Algorithm 3" from Weiss ---------------------------
// This algorithm is O(n lg n).

int Max3(int x, int y, int z) { //return the max of x, y, and z.
   if (x>y) {
      if (x>z) return x; 
      else return z; 
   } 
   else if (y>z) return y; 
        else return z; 
} 
   
int Max_Sub_Sum3( const int A[ ], const int Left, const int Right )
{
    int Max_Left_Sum = 0, Max_Right_Sum = 0;
    int Max_Left_Border_Sum = 0, Max_Right_Border_Sum = 0;
    int Left_Border_Sum = 0, Right_Border_Sum = 0;
    int Center = ( Left + Right ) / 2;

    if( Left == Right ) 	// Base Case.
        if( A[ Left ] > 0 )
            return A[ Left ];
        else
            return 0;

    Max_Left_Sum  = Max_Sub_Sum3( A, Left, Center );
    Max_Right_Sum = Max_Sub_Sum3( A, Center + 1, Right );

    for( int i = Center; i >= Left; i-- )
    {
        Left_Border_Sum += A[ i ];
        if( Left_Border_Sum > Max_Left_Border_Sum )
            Max_Left_Border_Sum = Left_Border_Sum;
    }

    for( i = Center + 1; i <= Right; i++ )
    {
        Right_Border_Sum += A[ i ];
        if( Right_Border_Sum > Max_Right_Border_Sum )
            Max_Right_Border_Sum = Right_Border_Sum;
    }

    return Max3( Max_Left_Sum, Max_Right_Sum,
                Max_Left_Border_Sum + Max_Right_Border_Sum );
}

int Max_Subsequence_Sum3( const int A[ ], const unsigned int N )
{
    return Max_Sub_Sum3( A, 0, N - 1 );
}



//----------------- "Algorithm 4" from Weiss ---------------------------
// This algorithm is O(n).
int Max_Subsequence_Sum4( const int A[ ], const unsigned int N )
{
    int This_Sum = 0, Max_Sum = 0, Seq_Start = 0;

    for( int Seq_End = 0; Seq_End < N; Seq_End++ )
    {
        This_Sum += A[ Seq_End ];

        if( This_Sum > Max_Sum )
            Max_Sum = This_Sum;
        else
        if( This_Sum < 0 )
        {
            Seq_Start = Seq_End + 1;
            This_Sum = 0;
        }
    }
    return( Max_Sum );
}
