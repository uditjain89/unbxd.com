#include<iostream>
#include<fstream>
#include<sstream>
#include<stdlib.h>
#include"Database.cpp"

using namespace std ;


int main()
{
  ifstream myFile ;
  string fileName ;
  Database db ;
  cout << "Enter products file name " << endl ;
  cin>> fileName ;
  myFile.open( fileName.c_str() ) ;
  if( !myFile.good() )
  {
    cout << "Error opening File " << endl ; 
    return 0 ; 
  }
  int id , count;
 // cout << "\n here " << endl ;
  string line , name, genre, artist;
  while( getline( myFile, line ) )
  {
    count = 0 ;
    stringstream stream(line) ;
    char* s ;
    string word ;
    stream >> word ;
    bool readDone = false ;
    while( word != "" )
    {
      count++ ;
      //cout << word << "  " ;
      if( count > 4 )
      {
         cout <<" Wrong file format. Format should be \n<product_id> <product_name> <genre> <artist>\n\n " ;
         return 0; 
      }
      switch( count )
      {
        case 1: s = &word[0] ;
                id = atoi(s) ;
                break ;
        case 2: name = word ;
		break ;
        case 3: genre = word ;
        	break ;
        case 4: artist = word ;
                readDone = true ;
      }
      word = "" ;
      stream >> word ;
    }
    if( readDone )
    {
     // cout <<" here " << endl ;
      db.addProduct( id, name , genre , artist ) ;
    }
  } // product File read 


  cout << "\n\nEnter records file name" << endl ;
  cin>> fileName ;
  ifstream myFile2 ;
  myFile2.open(fileName.c_str() ) ;
  if( !myFile2.good() ) 
  {
    cout <<"Could not open records file\n\n"<< endl ;
    return 0 ;
  } 

  string query , time ;
  while( getline( myFile2, line ) )
  {
    count = 0 ;
    stringstream stream(line) ;
    char* s ;
    string word ;
    stream >> word ;
    bool readDone = false ;
    while( word != "" )
    {
      count++ ;
      //cout << word << "  " ;
      if( count > 3)
      {
         cout <<" Wrong file format. Format should be \n<query> <product_id> <timestamp>\n where query is assumed to be a one word string\n\n " ;
         return 0; 
      }
      switch( count )
      {
        case 1: query = word ;
                break ;
        case 2: s = &word[0] ;
                id = atoi( s ) ;
		break ;
        case 3: time = word ;
        	readDone = true ;
        
      }
      word = "" ;
      stream >> word ;
    }
    if( readDone )
    {
     // cout <<" here " << endl ;
 //     cout << query <<"  " << id << "  " << time << endl ;
      db.createQueryRecord( query , id , time ) ;
    }
  } // Records File read 
  int choice ;
  do
  {
    cout<<"1.Search Query\n,2.Search Artist\n3.End\n\n" ;
    cin>>choice ;
    if( choice<1 && choice > 3 )
      cout << "Invalid choice\n";
    if( choice ==1 )
    {
      cout <<"Enter query\n";
      cin>>query ;
      db.productsQueried( query ) ;
    }
    if( choice == 2 )
    {
      cout <<"Enter artist\n";
      cin>>artist ;
      db.queryForArtist(artist ) ;
    }

  }while( choice!=3 ) ;
 // query = "nike" ;
 // db.productsQueried( query ) ;
  return 0 ;
}
