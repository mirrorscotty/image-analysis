//Source: http://stackoverflow.com/questions/30897072/vector-merge-algorithm-c
#include <vector>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cv.h>

void disp( const std::vector< int >& a )
{
    for ( const auto item : a )
    {
        std::cout << item << ',';
    }
    std::cout << "\n";
}

void disp( const std::vector< std::vector< int > >& matrix )
{
    for ( const auto& row : matrix )
    {
        disp( row );
    }
}

void disp( const cv::Vec< float,2 >& a )
{
    std::cout << a[0] << ", " << a[1];
    std::cout << "\n";
}

void disp( const std::vector< cv::Vec< float,2 > >& matrix )
{
    for ( const auto& row : matrix )
    {
        disp( row );
    }
}

// I think there shall be some easier way for this.
bool hasCommonElements( const std::vector< int >& aVector1, const std::vector< int >& aVector2 )
{
    for ( const auto item1 : aVector1 )
    {
        for ( const auto item2 : aVector2 )
        {
            if ( item1 == item2 )
            {
                return true;
            }
        }
    }
    return false;
}

void makeAllElementsUnique( std::vector< int >& aRow )
{
    std::sort( aRow.begin(), aRow.end() );
    aRow.erase( std::unique( aRow.begin(), aRow.end() ), aRow.end() );
}

void mergeRowsWithCommonValues( std::vector< std::vector< int > >& aMatrix )
{
    for ( auto it = aMatrix.begin(); it != aMatrix.end(); ++it )
    {
        auto it2 = it + 1;
        while ( it2 != aMatrix.end() )
        {
            if ( hasCommonElements( *it, *it2 ) )
            {
                (*it).insert( (*it).end(), (*it2).begin(), (*it2).end() ); // Merge the rows with the common value(s).
                makeAllElementsUnique( (*it) );

                it2 = aMatrix.erase( it2 ); // Remove the merged row.
            }
            else
            {
                ++it2;
            }
        }
    }
}

void example()
{
    std::vector< std::vector< int > > matrix;

    matrix.push_back( { 1, 2, 3 } );
    matrix.push_back( { 2, 3, 4 } );
    matrix.push_back( { 5, 6 } );

    disp( matrix );
    mergeRowsWithCommonValues( matrix );
    disp( matrix );
}

